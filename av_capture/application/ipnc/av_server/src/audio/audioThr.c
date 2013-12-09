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


#include <osa_cmem.h>
#include <avserver.h>
#include <alarm_msg_drv.h>

//#define AUDIO_CMEM_ALLOC OSA_cmemAlloc
#define AUDIO_CMEM_ALLOC OSA_cmemAllocCached

/* The number of channels of the audio codec */
#define NUM_CHANNELS		(1)
/* Number of samples to process at once */
#define NUMSAMPLES       	(1024)
#define RAWBUFSIZE       	(NUMSAMPLES * 2)
#define INPUTBUFSIZE     	(RAWBUFSIZE)
/* The max amount of bytes of speech data to process at once */
#define OUPUTBUFSIZE     	(RAWBUFSIZE)

ALG_AudEncPrm 	audEncPrm;


/////////////CONSTANTS//////////////////
#define SHM_KEY 		((key_t)0x11) //key for shared memory
#define SEM_KEY 		((key_t)0x21) //key for semaphore
#define RCV_MSG_KEY		((key_t)0x32)
#define SND_MSG_KEY		((key_t)0x31)

/////////////DATA STRUCTURES/////////////////
typedef struct {
	int running;
	int cur_wr_index;
	int cur_rd_index;
	int pending;
	unsigned int pktSize;
	unsigned int bufferCnt;
	unsigned char buf[5][1400];
} shared_audio_dat;

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

static struct sembuf op_lock[2]={
	{0,0,0},		  	//wait until sem #0 becomes 0
	{0,1,SEM_UNDO}      //then increment sem #0 by 1
};

static struct sembuf op_unlock[1]={
  {0,-1,SEM_UNDO} //decr sem #0 by 1
};
/////////////////////////////////////////////////


//////////COMMON VARS/////////////////////////////////////
int shmid;
int semid;
int snd_msg_qid;
int rcv_msg_qid;

message_buf  sbuf;
message_buf  rbuf;
shared_audio_dat* sharedAudioData = NULL;

void set_sem_value( int semid, int i, int val )
{
	union semun initval;
	initval.val=val;
	semctl( semid,i, SETVAL,initval);
}

void wait_and_lock( int semid )
{
	semop( semid,op_lock,2);
}

void signal_and_unlock( int semid )
{
	semop( semid,op_unlock,1);
}

int snd_msg(int qid)
{
	int status;
	int buf_length;

	sbuf.mtype = 2;
	buf_length = sizeof(sbuf) - sizeof(long);
	status = msgsnd(qid, &sbuf, buf_length, IPC_NOWAIT);

	return status;
}

int rcv_msg(int qid, int msg_type)
{
	return msgrcv(qid, &rbuf, sizeof(rbuf) - sizeof(long), msg_type, 0);
}

int InitAudioPlayMsg(void)
{
	//create a shared memory area which will be used as stack
	shmid= shmget( SHM_KEY, sizeof(shared_audio_dat), IPC_CREAT | 0600);

	//attach created shared area to the process.
	sharedAudioData=(shared_audio_dat *)shmat( shmid, 0,0);

	sharedAudioData->running = -1; //initialize stack top

	//create a semaphore for process synchronization
	semid= semget( SEM_KEY,1,IPC_CREAT );

	set_sem_value(semid,0,1); //set initial value of semaphore

	//initializa message queues
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

int CloseAudioPlayMsg(void)
{
	shmdt( sharedAudioData ); //dettach the shared memory area.
	shmctl(shmid, IPC_RMID,0); //remove the shared memory area
	semctl(semid, IPC_RMID,0); //remove the semaphore.

	return 0;
}

//////////////////////////////////////////////////////////

Uint32 AUDIO_GetTimeStamp(void)
{
	struct timeval timeval;

	gettimeofday(&timeval, NULL);

	return (timeval.tv_sec * 1000) + (timeval.tv_usec + 500) / 1000;;
}

int AUDIO_GetClkDiv(void)
{

	int dev_fp;
	void *pMem_map;
	unsigned long phyAddr= 0x01C40000;
	unsigned long length = 0x50;
	unsigned int *pPERI_CLKCTL = NULL;
	int audioClkDiv = -1;

	if ((dev_fp=open("/dev/mem",O_RDWR|O_SYNC))==-1)
	{
		OSA_ERROR("dev_fp Fail!! \n");
		return audioClkDiv;
	}
	pMem_map=mmap(	(void	*)phyAddr,length,
					PROT_READ|PROT_WRITE|PROT_EXEC,MAP_SHARED,
					dev_fp,phyAddr);

	if (pMem_map!=(void *)phyAddr)
	{
		OSA_ERROR("pMem_map Fail!! \n");
		return audioClkDiv;
	}


	pPERI_CLKCTL 		= (unsigned int *)( (char *)pMem_map+0x48);

	audioClkDiv = (((*pPERI_CLKCTL)>>7)&(0x1ff));

	if( pMem_map )
		munmap(pMem_map, length);

	if( dev_fp >= 0)
		close(dev_fp);

	//OSA_printf("audioClkDiv = %d \n", audioClkDiv);

	return (audioClkDiv+1);
}

int AUDIO_GetSampleRate( void )
{
	static int CurrentStatus = -255;
	int fd_proc = -1;
	int ret = -1;
	char StrBuffer[300];
	char TmpBuf[50];
	char *pStr = NULL;
	int getval = 0;
	char delima_buf[] = ":\r\n ";
	int clkdiv = 0;
	double sample_rate = 0;

	if( CurrentStatus >= -1 )
	{
		OSA_ERROR("CurrentStatus is = %d \n", CurrentStatus);
		return CurrentStatus;
	}

	fd_proc = open("/proc/davinci_clocks", O_RDONLY);
	if( fd_proc < 0 )
	{
		OSA_ERROR("GetSampleRate open file fail \n");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = read(fd_proc,  StrBuffer, sizeof(StrBuffer)-1);
	if( ret <= 0 )
	{
		OSA_ERROR("read device error !!");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = -1;
	StrBuffer[sizeof(StrBuffer)-1] = '\0';
	pStr = strtok(StrBuffer,delima_buf );

	while( pStr != NULL )
	{
		sscanf( pStr,"%s",TmpBuf);

		if( strncmp(TmpBuf, "VOICECODEC_CLK", sizeof("VOICECODEC_CLK")) == 0 )
		{

			pStr = strtok(NULL, delima_buf);

			sscanf( pStr,"%d",&getval);

			//OSA_printf("VOICECODEC_CLK = %d \n", getval);

			ret = getval;
			goto CHECK_CPU_END;


		}

		pStr = strtok(NULL, delima_buf);

	}

CHECK_CPU_END:

	if(fd_proc >= 0)
	  close(fd_proc);

	clkdiv = AUDIO_GetClkDiv();
	if( clkdiv > 0 )
	{
		sample_rate = 1.0/((1.0/((double)ret/(double)clkdiv))*256.0);
	}else{
		OSA_ERROR("Audio get clk div fail!! Set sample rate as default 16000!!!");
		sample_rate = 16000;
	}

	ret = (int)(sample_rate+50)/1000*1000;

	CurrentStatus = ret;

	return ret;
}

Uint32 AUDIO_audioEncode(Uint8 *src, Uint8 *dst, Uint32 bufsize)
{
	int ret = OSA_EFAIL;
	int audAlarm = FALSE;
	ALG_AudioRunPrm 	runPrm;
	ALG_AudioRunStatus 	status;

	if(gAVSERVER_config.audioConfig.audioUpdate == TRUE) {
		gAVSERVER_config.audioConfig.audioUpdate = FALSE;
		audEncPrm.bitRate = gAVSERVER_config.audioConfig.bitRate;
		ret = ALG_audDynparams(gAUDIO_ctrl.algEncHndl, &audEncPrm);
		if( ret != OSA_SOK ) {
      		OSA_ERROR("ALG_audEncUpdate Failure: CodecType(%d)", audEncPrm.codec);
      		return 0;
		}
	}

	runPrm.inAddr	= (Uint8 *)src;
	runPrm.outAddr	= (Uint8 *)dst;

	runPrm.inDataSize		= bufsize;
	runPrm.outBufMaxSize	= bufsize;

    OSA_prfBegin(&gAVSERVER_ctrl.audioPrf);

	ret = ALG_audEncRun(gAUDIO_ctrl.algEncHndl, &audEncPrm, &runPrm, &status);

    OSA_prfEnd(&gAVSERVER_ctrl.audioPrf, 1);

	if(gAVSERVER_config.audioConfig.audioAlarm) {
		audAlarm = ALG_audAlarmCheck(runPrm.inAddr, audEncPrm.numSamples, gAVSERVER_config.audioConfig.alarmLevel);
		if(audAlarm == TRUE) {
#ifdef AVSERVER_DEBUG_AUDIO_THR
			OSA_printf("Audio Alarm ON\n");
#endif
			if(gAVSERVER_config.alarmEnable)
				SendAlarmAudioTrigger();
		}
	}

	return status.outDataSize;
}

int AUDIO_audioTskCreate(void)
{
	int ret = OSA_EFAIL;
	DRV_AudioConfig Audioconfig;

	Audioconfig.deviceId			= 0;
	Audioconfig.numChannels			= NUM_CHANNELS;
	Audioconfig.buff_Of_Samples 	= NUMSAMPLES;
	Audioconfig.samplingRate		= gAVSERVER_config.audioConfig.samplingRate;
/*  //add by jsk 
  	ret = DRV_audioOpen(&gAUDIO_ctrl.audioHndl, &Audioconfig);
 	if( ret != OSA_SOK )
 	{
 		return ret;
 	}
*/
	ret = OSA_SOK;

  	/* Allocate input buffer */
  	gAUDIO_ctrl.inputBuffer 	= (Uint8 *)AUDIO_CMEM_ALLOC(INPUTBUFSIZE, 32);
	if (gAUDIO_ctrl.inputBuffer == NULL)
	{
		OSA_ERROR("Failed to allocate audio input buffer\n");
		return OSA_EFAIL;
	}

  	/* Allocate output buffer */
	gAUDIO_ctrl.outputBuffer 	= (Uint8 *)AUDIO_CMEM_ALLOC(OUPUTBUFSIZE, 32);
	if (gAUDIO_ctrl.outputBuffer == NULL)
	{
		OSA_ERROR("Failed to allocate audio output buffer\n");
		return OSA_EFAIL;

	}

    audEncPrm.codec      	= gAVSERVER_config.audioConfig.codecType;
    audEncPrm.encMode  		= (gAVSERVER_config.audioConfig.codecType==ALG_AUD_CODEC_G711)?PCM_ULAW:MPEG4_AUDIO;
    audEncPrm.numSamples 	= NUMSAMPLES;
    audEncPrm.sampRate     	= gAVSERVER_config.audioConfig.samplingRate;
    audEncPrm.bitRate      	= gAVSERVER_config.audioConfig.bitRate;

	gAUDIO_ctrl.algEncHndl = ALG_audEncCreate(&audEncPrm);
    if(gAUDIO_ctrl.algEncHndl==NULL) {
      OSA_ERROR("ALG_audEncCreate Failure: CodecType(%d)", audEncPrm.codec);
      return OSA_EFAIL;
    }

    return OSA_SOK;
}

int AUDIOPLAY_audioTskCreate(void)
{
	int ret = OSA_EFAIL;

	DRV_AudioConfig Audioplayconfig;
#if 0
	Audioplayconfig.samplingRate		= 8000;
	gAUDIOPLAY_ctrl.curPlaySampleRate   = Audioplayconfig.samplingRate;
	Audioplayconfig.deviceId			= 0;
	Audioplayconfig.numChannels 		= 1;
	Audioplayconfig.buff_Of_Samples 	= 128;
	Audioplayconfig.bufferSize			= 1024;
#endif
	Audioplayconfig.samplingRate		= 8000;
	gAUDIOPLAY_ctrl.curPlaySampleRate	= Audioplayconfig.samplingRate;

	Audioplayconfig.deviceId			= 0;
	Audioplayconfig.numChannels 		= 1;		/*单声道*/
	Audioplayconfig.buff_Of_Samples 	= 640;//640
	Audioplayconfig.bufferSize			= 1280;//1280
	

	ret = DRV_audioplayOpen(&gAUDIOPLAY_ctrl.audioHndl, &Audioplayconfig);
	if( ret != OSA_SOK )
	{
		return ret;
	}


	gAUDIOPLAY_ctrl.audPlayMode = AUD_PLAY_MODE_PLAY;

	InitAudioPlayMsg();

    return OSA_SOK;
}

int AUDIO_audioTskRun(void)
{
	int		rc = 0;
	unsigned int numBytes = RAWBUFSIZE;
	int     encodedBufferSize = 0;
	AUDIO_BufInfo AudioBufInfo;
	static Uint32	AudioLastTime = 0;
	int AudioInterval =  ((NUMSAMPLES*1000)/gAVSERVER_config.audioConfig.samplingRate);
	int AudioNow =  0;
	int AudioTmp =  0;

	/* Read stereo data from codec */
	rc = DRV_audioRead(&gAUDIO_ctrl.audioHndl, gAUDIO_ctrl.inputBuffer, NUMSAMPLES);
	if (rc == 0)
	{
		/* EPIPE means overrun */
		return OSA_SOK;
	}
	else if (rc < 0)
	{
		return OSA_EFAIL;
	}
	else if (rc != (int)NUMSAMPLES)
	{
		OSA_ERROR("Audio short read, read %d frames\n", rc);
		numBytes = rc*2; // as it is 16bit data
	}
	else
	{
		numBytes = RAWBUFSIZE;
	}

	if( AudioLastTime == 0 )
	{
		AudioNow =  AudioLastTime = AUDIO_GetTimeStamp();
	}
	else
	{
		AudioNow = AUDIO_GetTimeStamp();
		if( (AudioNow - AudioLastTime) < AudioInterval )
		{
			if( (AudioNow - AudioLastTime) < 10 )
			{
				return OSA_SOK;
			}
			AudioTmp = AudioNow;
			AudioNow = AudioLastTime+AudioInterval;
			AudioLastTime = AudioTmp;
		}
		else
		{
			AudioLastTime = AudioNow;
		}
	}


	AudioBufInfo.timestamp = AudioNow;

	encodedBufferSize = AUDIO_audioEncode(gAUDIO_ctrl.inputBuffer, gAUDIO_ctrl.outputBuffer, numBytes);

	if( encodedBufferSize <= 0 )
	{
		OSA_ERROR("Audio encode error !!\n");
		return OSA_EFAIL;
	}

	AudioBufInfo.encFrameSize 	= encodedBufferSize;
	AudioBufInfo.virtAddr		= gAUDIO_ctrl.outputBuffer;

	AUDIO_streamShmCopy(&AudioBufInfo);

	return OSA_SOK;
}

int AUDIOPLAY_audioTskRun(Bool *pDone, int fd)
{
	int rc = -1;
	DRV_AudioConfig Audioplayconfig;
	struct timeval time_val;
	rcv_msg(snd_msg_qid, 1);

	switch(rbuf.msg)
	{
	    case AUD_STREAM_NEWFRAME:

			// If the sample rate of the received frame is different from the current sample rate of the audio play device
			// then change the audio play device sample rate
			
			if((gAUDIOPLAY_ctrl.curPlaySampleRate != rbuf.sampleRate) &&
			   (gAUDIOPLAY_ctrl.audPlayMode == AUD_PLAY_MODE_PLAY))
			{
				// Change the sample rate for the audio play device
				// Close the device
				DRV_audioClose(&gAUDIOPLAY_ctrl.audioHndl);

				// Open the device
				Audioplayconfig.samplingRate		= 8000;
				Audioplayconfig.deviceId			= 0;
				Audioplayconfig.numChannels 		= 1;  		/*单声道*/
				Audioplayconfig.buff_Of_Samples 	= 640;	//640
				Audioplayconfig.bufferSize			= 1280;	//1280

				rc = DRV_audioplayOpen(&gAUDIOPLAY_ctrl.audioHndl, &Audioplayconfig);
				if(rc != OSA_SOK)
				{
					snd_msg(snd_msg_qid);
					return rc;
				}

				gAUDIOPLAY_ctrl.curPlaySampleRate = rbuf.sampleRate;
			}

			if(gAUDIOPLAY_ctrl.audPlayMode == AUD_PLAY_MODE_PLAY)
			{
				while(1){
				gettimeofday(&time_val, NULL);
				sharedAudioData->pktSize = 1024;
				//printf ("##before DRV_audioWrite%-10d %-10d\n", time_val.tv_sec, time_val.tv_usec);
				rc = DRV_audioWrite(&gAUDIOPLAY_ctrl.audioHndl, sharedAudioData->buf[1], sharedAudioData->pktSize);
				gettimeofday(&time_val, NULL);
				//printf ("##behind DRV_audioWrite%-10d %-10d\n\n", time_val.tv_sec, time_val.tv_usec);
				//write(fd, sharedAudioData->buf[1], sharedAudioData->pktSize);
				}
			}

			break;

		case AUD_STREAM_STOP:

  			#ifdef AVSERVER_DEBUG_AUDIO_THR
		    OSA_printf("Audio receiver process stopped\n");
			#endif
		    break;

        case AUD_ALARM_START:

			#ifdef AVSERVER_DEBUG_AUDIO_THR
			OSA_printf("AUD_ALARM_START cmd received\n");
			#endif

			// Audio alarm is started so close the aud play device
			DRV_audioClose(&gAUDIOPLAY_ctrl.audioHndl);

			// Set the play mode to PAUSE
			gAUDIOPLAY_ctrl.audPlayMode = AUD_PLAY_MODE_PAUSE;

			break;

		case AUD_ALARM_END:

			#ifdef AVSERVER_DEBUG_AUDIO_THR
			OSA_printf("AUD_ALARM_END cmd received\n");
			#endif

			//Open the audio device
			Audioplayconfig.samplingRate	= gAUDIOPLAY_ctrl.curPlaySampleRate;
			Audioplayconfig.deviceId		= 0;
			Audioplayconfig.numChannels 	= 1;
			Audioplayconfig.buff_Of_Samples = 640; //640
			Audioplayconfig.bufferSize		= 1280; //1280
			rc = DRV_audioplayOpen(&gAUDIOPLAY_ctrl.audioHndl, &Audioplayconfig);
			if(rc != OSA_SOK)
			{
				snd_msg(snd_msg_qid);
				return rc;
			}

			// change the aud play mode to PLAY
			gAUDIOPLAY_ctrl.audPlayMode = AUD_PLAY_MODE_PLAY;

			break;

		case AVSERVER_CMD_DELETE:

			*pDone = TRUE;

            break;

		default:

		    #ifdef AVSERVER_DEBUG_AUDIO_THR
		    OSA_printf("AudioPlay(None): Invalid msg received\n");
		    #endif

		    break;
	}

    // Send the ack
	snd_msg(snd_msg_qid);

	return OSA_SOK;
}

int AUDIO_audioTskDelete(void)
{
	DRV_audioClose(&gAUDIO_ctrl.audioHndl);

	if( gAUDIO_ctrl.inputBuffer )
	{
		OSA_cmemFree(gAUDIO_ctrl.inputBuffer);
	}

	if( gAUDIO_ctrl.outputBuffer )
	{
		OSA_cmemFree(gAUDIO_ctrl.outputBuffer);
	}

	ALG_audEncDelete(gAUDIO_ctrl.algEncHndl, &audEncPrm);

	return OSA_SOK;
}

int AUDIOPLAY_audioTskDelete(void)
{
	DRV_audioClose(&gAUDIOPLAY_ctrl.audioHndl);

	if( gAUDIOPLAY_ctrl.inputBuffer )
	{
		OSA_cmemFree(gAUDIOPLAY_ctrl.inputBuffer);
	}

	if( gAUDIOPLAY_ctrl.outputBuffer )
	{
		OSA_cmemFree(gAUDIOPLAY_ctrl.outputBuffer);
	}

	CloseAudioPlayMsg();

	return OSA_SOK;
}

int AUDIO_audioTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  OSA_setTskName("audio");
  
  #ifdef AVSERVER_DEBUG_AUDIO_THR
  OSA_printf(" AUDIO: Recevied CMD = 0x%04x\n", cmd);
  #endif

  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_AUDIO_THR
  OSA_printf(" AUDIO: Create...\n");
  #endif

  status = AUDIO_audioTskCreate();

  OSA_tskAckOrFreeMsg(pMsg, status);

  if(status!=OSA_SOK) {
    OSA_ERROR("AUDIO_audioTskCreate()\n");
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_AUDIO_THR
  OSA_printf(" AUDIO: Create...DONE\n");
  #endif

  status = OSA_tskWaitMsg(pTsk, &pMsg);
  if(status!=OSA_SOK)
    return OSA_SOK;

  cmd = OSA_msgGetCmd(pMsg);

  if(cmd==AVSERVER_CMD_DELETE) {

    done = TRUE;
    ackMsg = TRUE;

  } else {

    #ifdef AVSERVER_DEBUG_AUDIO_THR
    OSA_printf(" AUDIO: Start...\n");
    #endif

    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
  }

  #ifdef AVSERVER_DEBUG_AUDIO_THR
  OSA_printf(" AUDIO: Start...DONE\n");
  #endif

  while(!done) {

    status = AUDIO_audioTskRun();

    if(status!=OSA_SOK) {
      OSA_ERROR("AUDIO_audioTskRun()\n");
      break;
    }

    status = OSA_tskCheckMsg(pTsk, &pMsg);

    if(status!=OSA_SOK)
      continue;

    #ifdef AVSERVER_DEBUG_AUDIO_THR
    OSA_printf(" AUDIO: Received CMD = 0x%04x\n", cmd);
    #endif

    cmd = OSA_msgGetCmd(pMsg);

    switch(cmd) {
      case AVSERVER_CMD_DELETE:
        done = TRUE;
        ackMsg = TRUE;
        break;

      default:

        #ifdef AVSERVER_DEBUG_AUDIO_THR
        OSA_printf(" AUDIO: Unknown CMD = 0x%04x\n", cmd);
        #endif

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
    }
  }

  #ifdef AVSERVER_DEBUG_AUDIO_THR
  OSA_printf(" AUDIO: Delete...\n");
  #endif

  AUDIO_audioTskDelete();

  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  #ifdef AVSERVER_DEBUG_AUDIO_THR
  OSA_printf(" AUDIO: Delete...DONE\n");
  #endif

  return OSA_SOK;
}

int AUDIOPLAY_audioTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  OSA_setTskName("audioplay");
  
  #ifdef AVSERVER_DEBUG_AUDIO_THR
  OSA_printf(" AUDIOPLAY: Recevied CMD = 0x%04x\n", cmd);
  #endif

  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_AUDIO_THR
  OSA_printf(" AUDIOPLAY: Create...\n");
  #endif

  status = AUDIOPLAY_audioTskCreate();

  OSA_tskAckOrFreeMsg(pMsg, status);

  if(status!=OSA_SOK) {
    OSA_ERROR("AUDIOPLAY_audioTskCreate()\n");
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_AUDIO_THR
  OSA_printf(" AUDIOPLAY: Create...DONE\n");
  #endif

  status = OSA_tskWaitMsg(pTsk, &pMsg);
  if(status!=OSA_SOK)
    return OSA_SOK;

  cmd = OSA_msgGetCmd(pMsg);

  if(cmd==AVSERVER_CMD_DELETE) {

    done = TRUE;
    ackMsg = TRUE;

  } else {

    #ifdef AVSERVER_DEBUG_AUDIO_THR
    OSA_printf(" AUDIOPLAY: Start...\n");
    #endif

    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
  }

  #ifdef AVSERVER_DEBUG_AUDIO_THR
  OSA_printf(" AUDIOPLAY: Start...DONE\n");
  #endif
int fd;
  fd = open("audio.pcm", O_RDONLY);
  if (fd <= 0){
  	printf ("$$$$$$$$$$$$$##################open audio.pcm error");
  }


  while(!done) {

    status = AUDIOPLAY_audioTskRun(&done, fd );

    if(status!=OSA_SOK) {
      OSA_ERROR("AUDIOPLAY_audioTskRun()\n");
      break;
    }

    status = OSA_tskCheckMsg(pTsk, &pMsg);

    if(status!=OSA_SOK)
      continue;

    #ifdef AVSERVER_DEBUG_AUDIO_THR
    OSA_printf(" AUDIOPLAY: Received CMD = 0x%04x\n", cmd);
    #endif

    cmd = OSA_msgGetCmd(pMsg);

    switch(cmd) {
      case AVSERVER_CMD_DELETE:
        done = TRUE;
        ackMsg = TRUE;
        break;

      default:
        #ifdef AVSERVER_DEBUG_AUDIO_THR
        OSA_printf(" AUDIOPLAY: Unknown CMD = 0x%04x\n", cmd);
        #endif

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
    }
  }

  #ifdef AVSERVER_DEBUG_AUDIO_THR
  OSA_printf(" AUDIOPLAY: Delete...\n");
  #endif

  AUDIOPLAY_audioTskDelete();

  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  #ifdef AVSERVER_DEBUG_AUDIO_THR
  OSA_printf(" AUDIOPLAY: Delete...DONE\n");
  #endif

  return OSA_SOK;
}

int AUDIO_audioCreate()
{

  int status;

  status = OSA_tskCreate( &gAUDIO_ctrl.audioTsk, AUDIO_audioTskMain, AUDIO_CAPTURE_THR_PRI, AUDIO_CAPTURE_STACK_SIZE, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }

  return status;
}

int AUDIOPLAY_audioCreate()
{

  int status;

  status = OSA_tskCreate( &gAUDIOPLAY_ctrl.audioTsk, AUDIOPLAY_audioTskMain, AUDIO_PLAYBACK_THR_PRI, AUDIO_CAPTURE_STACK_SIZE, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }

  return status;
}

int AUDIO_audioDelete()
{
  int status;

  status = OSA_tskDelete( &gAUDIO_ctrl.audioTsk );

  return status;
}

int AUDIOPLAY_audioDelete()
{
  int status;

  status = OSA_tskDelete( &gAUDIOPLAY_ctrl.audioTsk );

  return status;
}

