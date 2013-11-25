
#define _AUDIO_SINK_HH

#ifndef _MEDIA_SINK_HH
#include "MediaSink.hh"
#endif

#include <Appro_interface.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>


//////////////CONSTANTS/////////////////////////

#define SHM_KEY 		((key_t)0x11) //key for shared memory
#define SEM_KEY 		((key_t)0x21) //key for semaphore
#define RCV_MSG_KEY	((key_t)0x32)
#define SND_MSG_KEY	((key_t)0x31)
#define SHM_SZ		(10000)

#define AUD_STREAM_NEWFRAME 0x800
#define AUD_STREAM_STOP     0x801

//////////STRUCT DEFINITIONS//////////////////////

union semun { 
	int val; 
	struct semid_ds *buf; 
	unsigned short *array; 
};

typedef struct {
         long    mtype;
         int     msg;
         int sampleRate;
         } message_buf;
 
typedef struct {
	int running;
	int cur_wr_index;
	int cur_rd_index;
	int pending;
	unsigned int pktSize;
	unsigned int bufferCnt;
	unsigned char buf[5][1400];
} shared_audio_dat;

  static struct sembuf op_lock[2]={
      0,0,0, 			//wait until sem #0 becomes 0
      0,1,SEM_UNDO 	//then increment sem #0 by 1
  };
 
  static struct sembuf op_unlock[1]={ 
	0,-1,SEM_UNDO //decr sem #0 by 1
  };

///////////////////////////////////////////////CLASS///////////////
class AudioSink: public MediaSink {
public:
  static AudioSink* createNew(UsageEnvironment& env, char const* fileName, unsigned bufferSize = 20000);
  void writeAudioData(unsigned char* data, unsigned dataSize, struct timeval presentationTime);
  void closeRemoteStream();

protected:
  AudioSink(UsageEnvironment& env, FILE* fid, unsigned bufferSize);
  virtual ~AudioSink();
  
protected:

  FILE* fOutFid;
  unsigned char* fBuffer;
  unsigned fBufferSize;

  int shmid;
  int semid;
  int snd_msg_qid;
  int rcv_msg_qid;

  
  shared_audio_dat *sharedAudioData;
  message_buf  sbuf;
  message_buf  rbuf;

  static void afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
				struct timeval presentationTime,  unsigned durationInMicroseconds);
				
  virtual void afterGettingFrame1(unsigned frameSize, struct timeval presentationTime);

  int  init_all(void);  
  int close_all(void);

  void set_sem_value( int semid, int i, int val );
  void wait_and_lock( int semid );
  void signal_and_unlock( int semid );
  int snd_msg(int qid);
  int rcv_msg(int qid);

private: // redefined virtual functions:
  virtual Boolean continuePlaying();
};
