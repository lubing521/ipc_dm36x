#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

#include <pthread.h>
#include <semaphore.h>


#include "AudioSink.hh"
#include "GroupsockHelper.hh"
#include "OutputFile.hh"

typedef struct
{
    int bufAddr;
    int bufSize;
    int sampleRate;
}AUD_MSG;

////////// AudioSink //////////

void AudioSink::set_sem_value( int semid, int i, int val )
{
      union semun initval;
      initval.val=val;
      semctl( semid,i, SETVAL,initval);
}

void AudioSink::wait_and_lock( int semid )
{
	semop( semid,op_lock,2);
}


void AudioSink::signal_and_unlock( int semid )
{
	semop( semid,op_unlock,1);
}

int AudioSink::snd_msg(int qid)
{
	int status;
	int buf_length;
	sbuf.mtype = 1;
	buf_length = sizeof(sbuf) - sizeof(long);
	status = msgsnd(qid, &sbuf, buf_length, IPC_NOWAIT);
	//printf("\nSVR: msg_sent :length(%d)", buf_length);

	return status;
}

int AudioSink::rcv_msg(int qid)
{
	int status;

	status = msgrcv(qid, &rbuf, sizeof(rbuf) - sizeof(long), 2, 0);
		//printf("\nCLI: msg received");

	return status;
}

int AudioSink::init_all()
{
	//create a shared memory area which will be used as stack
	shmid= shmget( SHM_KEY, sizeof(shared_audio_dat), IPC_CREAT | 0600);

	//attach created shared area to the process.
	sharedAudioData=(shared_audio_dat *)shmat( shmid, 0,0);

	//initialize shared data
	sharedAudioData->running = -1; //initialize stack top
	sharedAudioData->cur_wr_index = -1; //initialize stack top
	sharedAudioData->cur_rd_index = -1;
	sharedAudioData->cur_wr_index = -1;
	sharedAudioData->pending = 0;
	sharedAudioData->pktSize = 0;
	sharedAudioData->bufferCnt = 0;

	//create a semaphore for process synchronization
	semid= semget( SEM_KEY,1,IPC_CREAT );
	set_sem_value(semid,0,1); //set initial value of semaphore

	//initialize the message queues
	 snd_msg_qid = msgget(SND_MSG_KEY, IPC_CREAT | 0666 );
	 if(snd_msg_qid < 0) {
        perror("msgget");
        return -1;
    }

	 rcv_msg_qid = msgget(RCV_MSG_KEY, IPC_CREAT | 0666 );
	 if(rcv_msg_qid < 0) {
        perror("msgget");
        return -1;
    }

	return 0;
}

int AudioSink::close_all()
{
	shmdt( sharedAudioData ); //dettach the shared memory area.
	shmctl(shmid, IPC_RMID,0); //remove the shared memory area
	semctl(semid, IPC_RMID,0); //remove the semaphore.

	return 0;
}

void AudioSink::closeRemoteStream() {

	int buf_length;
	sbuf.mtype = 1;
	//strcpy(sbuf.mtext, "END");
	//buf_length = strlen(sbuf.mtext) + 1 ;

	sbuf.msg = AUD_STREAM_STOP;
	msgsnd(snd_msg_qid, &sbuf, sizeof(sbuf) - sizeof(long), 0);

	wait_and_lock( semid );
	/*critical section starts*/
	  sharedAudioData->running = -1;
	/*critical section ends*/
	//unlock access to shared area for other processes.
	signal_and_unlock( semid );
}


/////////////////////////////////////////////////////
//////////////////////////////////////////////
AudioSink::AudioSink(UsageEnvironment& env, FILE* fid, unsigned bufferSize)
  : MediaSink(env), fOutFid(fid), fBufferSize(bufferSize) {

  	fBuffer = new unsigned char[bufferSize];
	sharedAudioData = new shared_audio_dat;

	init_all();
}

//////////////////////////////////////////////
AudioSink::~AudioSink() {
  	delete[] fBuffer;
  	//if (fOutFid != NULL) fclose(fOutFid);

	closeRemoteStream();

	//close shared memory
  	close_all();
}

//////////////////////////////////////////////
AudioSink* AudioSink::createNew(UsageEnvironment& env, char const* fileName, unsigned bufferSize) {
  do {

    FILE* fid = NULL;
    // Normal case: create the fid once
    //fid = OpenOutputFile(env, fileName);
    //if (fid == NULL) break;
    return new AudioSink(env, fid, bufferSize);
  } while (0);

  return NULL;
}




////////////////////////////////////////////
Boolean AudioSink::continuePlaying() {
  if (fSource == NULL) {
	closeRemoteStream();
	return False;
  }
  fSource->getNextFrame(fBuffer, fBufferSize, afterGettingFrame, this, onSourceClosure, this);

  return True;
}

////////////////////////////////////////////
void AudioSink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned /*numTruncatedBytes*/,
				 struct timeval presentationTime, unsigned /*durationInMicroseconds*/) {

  AudioSink* sink = (AudioSink*)clientData;
  sink->afterGettingFrame1(frameSize, presentationTime);

}

///////////////////////////////////////////
void AudioSink::afterGettingFrame1(unsigned frameSize, struct timeval presentationTime) {

  writeAudioData (fBuffer, frameSize, presentationTime);

  // Then try getting the next frame:
  continuePlaying();
}

////////////////////////////////////////////
void AudioSink::writeAudioData(unsigned char* data, unsigned dataSize, struct timeval presentationTime) {

    // Copy the audio frame into the buffer on the sahred memory
	memcpy(sharedAudioData->buf[0], data, dataSize);
	sharedAudioData->pktSize = dataSize;

    // Sample rate for the audio frame just received
	sbuf.sampleRate = 8000;
	sbuf.msg        = AUD_STREAM_NEWFRAME;

	//send message that data written
	snd_msg(snd_msg_qid);


	//fwrite(sharedAudioData->buf, sharedAudioData->bufSize, 1, fOutFid);
	//fwrite(data, 1, dataSize, fOutFid);

	//wait for ACK
	rcv_msg(snd_msg_qid);
}


