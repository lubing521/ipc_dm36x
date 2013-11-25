/*
 * Copyright (C) 2005-2006 WIS Technologies International Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and the associated README documentation file (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
// An interface to the WIS GO7007 capture device.
// Implementation

#include "APPROInput.hh"
//#include "Options.hh"
#include "Err.hh"
#include "Base64.hh"
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <linux/soundcard.h>

#define STREAM_DETAIL_OUTPUT
#define TIME_GET_WAY (0)
#define STREAM_CHECK_DURATION_MS	(16666)

extern unsigned audioSamplingFrequency;
extern unsigned audioNumChannels;
extern int audio_enable;
extern void sigterm(int dummy);

////////// OpenFileSource definition //////////

// A common "FramedSource" subclass, used for reading from an open file:

class OpenFileSource: public FramedSource {
public:
  int  uSecsToDelay;
  int  uSecsToDelayMax;
  int  srcType;
protected:
  OpenFileSource(UsageEnvironment& env, APPROInput& input);
  virtual ~OpenFileSource();

  virtual int readFromFile() = 0;

private: // redefined virtual functions:
  virtual void doGetNextFrame();

private:
  static void incomingDataHandler(OpenFileSource* source);
  void incomingDataHandler1();

protected:
  APPROInput& fInput;

//  int fFileNo;
};


////////// VideoOpenFileSource definition //////////

class VideoOpenFileSource: public OpenFileSource {
public:
  VideoOpenFileSource(UsageEnvironment& env, APPROInput& input);
  virtual ~VideoOpenFileSource();

protected: // redefined virtual functions:
  virtual int readFromFile();
  virtual int readFromFile264();
  unsigned int SerialBook;
  unsigned int SerialLock;
  int StreamFlag;
  struct timeval fPresentationTimePre;
  int IsStart;
  int nal_state;
  int Checkduration;
};

#define STREAM_GET_VOL    0x0001
#define STREAM_NEW_GOP    0x0002

////////// AudioOpenFileSource definition //////////

class AudioOpenFileSource: public OpenFileSource {
public:
  AudioOpenFileSource(UsageEnvironment& env, APPROInput& input);
  virtual ~AudioOpenFileSource();

protected: // redefined virtual functions:
  virtual int readFromFile();
  int getAudioData();

  unsigned int AudioBook;
  unsigned int AudioLock;
  struct timeval fPresentationTimePre;
  int IsStart;

};

long timevaldiff(struct timeval *starttime, struct timeval *finishtime)
{
  long msec;
  msec=(finishtime->tv_sec-starttime->tv_sec)*1000;
  msec+=(finishtime->tv_usec-starttime->tv_usec)/1000;
  return msec;
}

void printErr(UsageEnvironment& env, char const* str = NULL) {
  if (str != NULL) err(env) << str;
  env << ": " << strerror(env.getErrno()) << "\n";
}

////////// APPROInput implementation //////////

APPROInput* APPROInput::createNew(UsageEnvironment& env, int vType) {
  return new APPROInput(env, vType);
}

FramedSource* APPROInput::videoSource() {

  if (fOurVideoSource == NULL) {
    fOurVideoSource = new VideoOpenFileSource(envir(), *this);
  }

  return fOurVideoSource;
}

FramedSource* APPROInput::audioSource() {

  if (fOurAudioSource == NULL) {
    fOurAudioSource = new AudioOpenFileSource(envir(), *this);
  }

  return fOurAudioSource;
}

APPROInput::APPROInput(UsageEnvironment& env, int vType)
  : Medium(env), videoType(vType), fOurVideoSource(NULL), fOurAudioSource(NULL) {
}

APPROInput::~APPROInput() {
 if( fOurVideoSource )
 {
 	delete (VideoOpenFileSource *)fOurVideoSource;
 	fOurVideoSource = NULL;
 }

 if( fOurAudioSource )
 {
 	delete (AudioOpenFileSource *)fOurAudioSource;
 	fOurAudioSource = NULL;
 }

}

#include <stdio.h>
#include <stdlib.h>
FILE *pFile = NULL;
void OpenFileHdl(void)
{
	if( pFile == NULL )
	{
		pFile = fopen("test.264", "rb");
		if( pFile == NULL )
		{
			fprintf(stderr,"h264 open file fail!!\n");
		}
	}
}

void CloseFileHdl(void)
{
	if( pFile != NULL )
	{
		fclose(pFile);
		pFile = NULL;
	}
}


char FrameBuff[1024*1024];

int NAL_Search(int offsetNow)
{
	unsigned long testflg = 0;
	int IsFail = 0;

	for(;;)
	{
		fseek(pFile, offsetNow, SEEK_SET);
		if( fread(&testflg, sizeof(testflg), 1, pFile) <=  0 )
		{
			IsFail = -1;
			break;
		}

		//printf("testflg=0x%x \n",(int)testflg );

		if( testflg == 0x01000000 )
		{
			break;
		}


		offsetNow++;

	}
	if( IsFail != 0 )
		return IsFail;

	return offsetNow;

}

void *GetFileFrame(int *pSize,int IsReset)
{
	static int offset = 0;
	int offset1 = 0;
	int offset2 = 0;
	int framesize = 0;

	*pSize = 0;

	if( pFile == NULL )
		return NULL;

	if( IsReset == 1 )
	{
		offset = 0;
		fseek(pFile, 0, SEEK_SET);
	}

	if( pFile )
	{
		fseek(pFile, offset, SEEK_SET);

		offset1 = NAL_Search(offset);
		offset2 = NAL_Search(offset1+4);


		framesize = offset2 - offset1;

		/*reset position*/
		fseek(pFile, offset1, SEEK_SET);
		fread(FrameBuff, framesize, 1, pFile);

		offset = offset2;

		*pSize = framesize;
	}

	return FrameBuff;
}



////////// OpenFileSource implementation //////////

OpenFileSource
::OpenFileSource(UsageEnvironment& env, APPROInput& input)
  : FramedSource(env),
    fInput(input) {
}

OpenFileSource::~OpenFileSource() {
	CloseFileHdl();
}

void OpenFileSource::doGetNextFrame() {
	incomingDataHandler(this);
}

void OpenFileSource
::incomingDataHandler(OpenFileSource* source) {
  source->incomingDataHandler1();
}

void OpenFileSource::incomingDataHandler1() {
	int ret;

	if (!isCurrentlyAwaitingData()) return; // we're not ready for the data yet

	ret = readFromFile();
	if (ret < 0) {
		handleClosure(this);
		fprintf(stderr,"In Grab Image, the source stops being readable!!!!\n");
	}
	else if (ret == 0)
	{

		if( uSecsToDelay >= uSecsToDelayMax )
		{
			uSecsToDelay = uSecsToDelayMax;
		}else{
			uSecsToDelay *= 2;
		}
		nextTask() = envir().taskScheduler().scheduleDelayedTask(uSecsToDelay,
			      (TaskFunc*)incomingDataHandler, this);
	}
	else {

		nextTask() = envir().taskScheduler().scheduleDelayedTask(0, (TaskFunc*)afterGetting, this);
	}
}

static int av_field[8][7] = {
	{AV_OP_LOCK_MP4_VOL,
	AV_OP_UNLOCK_MP4_VOL,
	AV_OP_LOCK_MP4,
	AV_OP_LOCK_MP4_IFRAME,
	AV_OP_UNLOCK_MP4,
	AV_OP_GET_MPEG4_SERIAL,
	AV_OP_WAIT_NEW_MPEG4_SERIAL},

	{AV_OP_LOCK_MP4_CIF_VOL,
	AV_OP_UNLOCK_MP4_CIF_VOL,
	AV_OP_LOCK_MP4_CIF,
	AV_OP_LOCK_MP4_CIF_IFRAME,
	AV_OP_UNLOCK_MP4_CIF,
	AV_OP_GET_MPEG4_CIF_SERIAL,
	AV_OP_WAIT_NEW_MPEG4_CIF_SERIAL},

	{AV_OP_LOCK_MP4_VOL,
	AV_OP_UNLOCK_MP4_VOL,
	AV_OP_LOCK_MP4,
	AV_OP_LOCK_MP4_IFRAME,
	AV_OP_UNLOCK_MP4,
	AV_OP_GET_MPEG4_SERIAL,
	AV_OP_WAIT_NEW_MPEG4_SERIAL},

	{AV_OP_LOCK_MP4_VOL,
	AV_OP_UNLOCK_MP4_VOL,
	AV_OP_LOCK_MP4,
	AV_OP_LOCK_MP4_IFRAME,
	AV_OP_UNLOCK_MP4,
	AV_OP_GET_MPEG4_SERIAL,
	AV_OP_WAIT_NEW_MPEG4_SERIAL},

	{AV_OP_LOCK_MP4_VOL,
	AV_OP_UNLOCK_MP4_VOL,
	AV_OP_LOCK_MP4,
	AV_OP_LOCK_MP4_IFRAME,
	AV_OP_UNLOCK_MP4,
	AV_OP_GET_MPEG4_SERIAL,
	AV_OP_WAIT_NEW_MPEG4_SERIAL},

	{AV_OP_LOCK_MP4_VOL,
	AV_OP_UNLOCK_MP4_VOL,
	AV_OP_LOCK_MP4,
	AV_OP_LOCK_MP4_IFRAME,
	AV_OP_UNLOCK_MP4,
	AV_OP_GET_MPEG4_SERIAL,
	AV_OP_WAIT_NEW_MPEG4_SERIAL},

	{AV_OP_LOCK_MP4_VOL,
	AV_OP_UNLOCK_MP4_VOL,
	AV_OP_LOCK_MP4,
	AV_OP_LOCK_MP4_IFRAME,
	AV_OP_UNLOCK_MP4,
	AV_OP_GET_MPEG4_SERIAL,
	AV_OP_WAIT_NEW_MPEG4_SERIAL},

	{AV_OP_LOCK_MP4_CIF_VOL,
	AV_OP_UNLOCK_MP4_CIF_VOL,
	AV_OP_LOCK_MP4_CIF,
	AV_OP_LOCK_MP4_CIF_IFRAME,
	AV_OP_UNLOCK_MP4_CIF,
	AV_OP_GET_MPEG4_CIF_SERIAL,
	AV_OP_WAIT_NEW_MPEG4_CIF_SERIAL},
};
#define AV_LOCK_MP4_VOL	0
#define AV_UNLOCK_MP4_VOL 1
#define AV_LOCK_MP4		2
#define AV_LOCK_MP4_IFRAME	3
#define AV_UNLOCK_MP4	4
#define AV_GET_MPEG4_SERIAL 5
#define AV_WAIT_NEW_MPEG4_SERIAL 6

////////// VideoOpenFileSource implementation //////////

VideoOpenFileSource
::VideoOpenFileSource(UsageEnvironment& env, APPROInput& input)
  : OpenFileSource(env, input), SerialBook(0), SerialLock(0), StreamFlag(STREAM_GET_VOL),IsStart(1),nal_state(0),Checkduration(0) {

 uSecsToDelay = 1000;
 uSecsToDelayMax = 1666;//33000/4;
 srcType = 0;


}

VideoOpenFileSource::~VideoOpenFileSource() {

  fInput.fOurVideoSource = NULL;

  if ((fInput.videoType <= VIDEO_TYPE_H264_CIF) && SerialLock > 0)
  {
    GetAVData(av_field[fInput.videoType][AV_UNLOCK_MP4], SerialLock, NULL);
  }
  SerialLock = 0;

}

int VideoOpenFileSource::readFromFile264()
{
#if 1
	void *pframe = NULL;
	int framesize = 0;
	int ret = 0;
	int offset = 0;
	OpenFileHdl();

	if (StreamFlag & STREAM_GET_VOL)
	{
		pframe = GetFileFrame(&framesize,1);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;

		StreamFlag &= ~(STREAM_GET_VOL|STREAM_NEW_GOP);
		StreamFlag |= STREAM_NEW_GOP;

	}
	else if (StreamFlag & STREAM_NEW_GOP) {
		pframe = GetFileFrame(&framesize,0);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;
		pframe = GetFileFrame(&framesize,0);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;
		pframe = GetFileFrame(&framesize,0);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;
		pframe = GetFileFrame(&framesize,0);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;
		pframe = GetFileFrame(&framesize,0);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;
		pframe = GetFileFrame(&framesize,0);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;
	}

	if (1) {
		fFrameSize = offset;
		if (fFrameSize > fMaxSize) {
			printf("Frame Truncated\n");
			printf("fFrameSize = %d\n",fFrameSize);
			printf("fMaxSize = %d\n",fMaxSize);
			fNumTruncatedBytes = fFrameSize - fMaxSize;
			fFrameSize = fMaxSize;
		}
		else {
			fNumTruncatedBytes = 0;
		}
		//memcpy(fTo+offset, av_data.ptr, fFrameSize-offset);


		// Note the timestamp and size:
		gettimeofday(&fPresentationTime, NULL);

		return 1;
	}
	else if (ret == RET_NO_VALID_DATA) {
		return 0;
	}
	else {
		StreamFlag |= STREAM_NEW_GOP;
		return 0;
	}
#else
	return 0;
#endif

}



static char mpeg4_header[] = {0x00, 0x00, 0x01, 0xb0, 0x01, 0x00, 0x00, 0x01, 0xb5, 0x09};


void printheader(char *pData, int size)
{
	int cnt = 0;

	printf("printheader = %d\n",size);
	for(cnt = 0 ;cnt < size; cnt++ )
	{

		printf(" 0x%X ",*pData++);
		if( cnt!=0 && cnt%4 == 0 )
		printf("\n");
	}
}
int WaitVideoStart( int vType,int sleep_unit )
{
	AV_DATA av_data;
	int cnt = 0;
	int serialnum = -1;
	int *cmd = av_field[vType];

	while(1)
	{
		av_data.serial = -1;
		GetAVData(cmd[AV_GET_MPEG4_SERIAL], -1, &av_data );

		if( (int)av_data.serial < 0 )
		{
			printf("Stream %d is not avaliable~~~~~~~~\n",vType);
			sigterm(0);
			break;
		}

		if( av_data.flags != AV_FLAGS_MP4_I_FRAME )
		{
			usleep(sleep_unit);
		}else{

			serialnum = av_data.serial ;

			break;
		}
		cnt++;
		if( cnt > 10000 )
			break;
	}

	return serialnum;
}

int GetVideoSerial( int vType )
{
	AV_DATA av_data;
	int *cmd = av_field[vType];

	GetAVData(cmd[AV_GET_MPEG4_SERIAL], -1, &av_data );

	return av_data.serial;
}

#ifdef STREAM_DETAIL_OUTPUT
#define	RUN_FRAME_NUM	(1000)
void PrintStreamDetails(void)
{
	static int strmfrmCnt = 0;
	static int strmfrmSkip = 0;
	static struct timeval startTime, endTime, passTime;
	double calcTime;

	if (strmfrmSkip < 150) {
		strmfrmSkip++;
	} else {
		if (strmfrmCnt == 0)
			gettimeofday(&startTime, NULL);
		if (strmfrmCnt == RUN_FRAME_NUM) {
			gettimeofday(&endTime, NULL);
			printf("\n==================== STREAMING DETAILS ====================\n");
			printf("Start Time : %ldsec %06ldusec\n", (long)startTime.tv_sec, (long)startTime.tv_usec);
			printf("End Time   : %ldsec %06ldusec\n", (long)endTime.tv_sec, (long)endTime.tv_usec);
			timersub(&endTime, &startTime, &passTime);
			calcTime = (double)passTime.tv_sec*1000.0 + (double)passTime.tv_usec/1000.0;
			printf("Total Time to stream %d frames: %.3f msec\n", RUN_FRAME_NUM , calcTime);
			printf("Time per frame: %3.4f msec\n", calcTime/RUN_FRAME_NUM);
			printf("Streaming Performance in FPS: %3.4f\n", RUN_FRAME_NUM/(calcTime/1000));
			printf("===========================================================\n");
			strmfrmCnt 	= 0;
			strmfrmSkip = 0;
		} else {
			strmfrmCnt++;
		}
	}
}
#endif

int VideoOpenFileSource::readFromFile()
{
	AV_DATA av_data;
	int ret;

	if (fInput.videoType == VIDEO_TYPE_MJPEG)
	{
		av_data.serial = -1;
		GetAVData(AV_OP_GET_MJPEG_SERIAL, -1, &av_data);
		if (av_data.serial <= SerialLock)
			return 0;
		if( (int)av_data.serial<0 )
		{
			printf("Stream %d is not avaliable~~~~~~~~\n",fInput.videoType);
			sigterm(0);
			return 0;
		}

		ret = GetAVData(AV_OP_LOCK_MJPEG, av_data.serial, &av_data );
		if (ret == RET_SUCCESS) {
			fFrameSize = av_data.size;
			if (fFrameSize > fMaxSize) {
				printf("Frame Truncated\n");
				fNumTruncatedBytes = fFrameSize - fMaxSize;
				fFrameSize = fMaxSize;
			}
			else {
				fNumTruncatedBytes = 0;
			}
			memcpy(fTo, av_data.ptr, fFrameSize);
#ifdef STREAM_DETAIL_OUTPUT
			PrintStreamDetails();
#endif
			SerialLock = av_data.serial;
			//printf("av_data.serial = %d\n", av_data.serial);
			GetAVData(AV_OP_UNLOCK_MJPEG, SerialLock, NULL);

			// Note the timestamp and size:
#if TIME_GET_WAY
			gettimeofday(&fPresentationTime, NULL);
#else
			fPresentationTime.tv_sec = av_data.timestamp/1000;
			fPresentationTime.tv_usec = (av_data.timestamp%1000)*1000;
#endif
			if( IsStart )
			{
				AV_DATA av_data_media;
				IsStart = 0;

				GetAVData(AV_OP_GET_MEDIA_INFO, -1, &av_data_media );
				if( av_data_media.flags == 1 )
				{
					Checkduration = STREAM_CHECK_DURATION_MS;
				}else{
					Checkduration = STREAM_CHECK_DURATION_MS;
				}

			}
			fDurationInMicroseconds = Checkduration;

			return 1;
		}
		return 0;

	}
	else if ( fInput.videoType == VIDEO_TYPE_MPEG4 ||
			  fInput.videoType == VIDEO_TYPE_MPEG4_CIF )
	{
		int offset = 0;
		int *cmd = av_field[fInput.videoType];

		if (StreamFlag & STREAM_GET_VOL) {
			AV_DATA vol_data;
			memcpy(fTo, mpeg4_header, sizeof(mpeg4_header));
			offset = sizeof(mpeg4_header);
			if(GetAVData(cmd[AV_LOCK_MP4_VOL], -1, &vol_data) != RET_SUCCESS) {
				printf("Error on Get Vol data\n");
				return -1;
			}
			memcpy(fTo+offset, vol_data.ptr, vol_data.size);
			offset += vol_data.size;

			GetAVData(cmd[AV_UNLOCK_MP4_VOL], -1, &vol_data);

			if( vol_data.size != 18 )
			{
				printf("Stream %d is not avaliable~~~~~~~~\n",fInput.videoType);
				sigterm(0);
			}

			WaitVideoStart( fInput.videoType ,300);

			ret = GetAVData(cmd[AV_LOCK_MP4_IFRAME], -1, &av_data);
			SerialBook = av_data.serial;
			StreamFlag &= ~(STREAM_GET_VOL|STREAM_NEW_GOP);
		}
		else if (StreamFlag & STREAM_NEW_GOP) {

			WaitVideoStart( fInput.videoType ,5000);

			ret = GetAVData(cmd[AV_LOCK_MP4_IFRAME], -1, &av_data );
			SerialBook = av_data.serial;
			StreamFlag &= ~STREAM_NEW_GOP;
		}
		else {
			ret = GetAVData(cmd[AV_LOCK_MP4], SerialBook, &av_data );
		}

		if (ret == RET_SUCCESS)
		{
			static int IscheckKey = 1;
			if( av_data.flags == AV_FLAGS_MP4_I_FRAME && IscheckKey == 1 )
			{
				int serial_now;
				serial_now = GetVideoSerial(fInput.videoType);
				IscheckKey = 0;
				//printf("serial_now = %d SerialBook = %d \n",serial_now,SerialBook);
				if( (serial_now - SerialBook) > 30 )
				{
					GetAVData(cmd[AV_UNLOCK_MP4], SerialBook, &av_data);
					StreamFlag |= STREAM_NEW_GOP;
					return 0;
				}
			}else{
				IscheckKey = 1;
			}
		}

		if (ret == RET_SUCCESS) {
			fFrameSize = av_data.size + offset;
			if (fFrameSize > fMaxSize) {
				printf("Frame Truncated\n");
				fNumTruncatedBytes = fFrameSize - fMaxSize;
				fFrameSize = fMaxSize;
			}
			else {
				fNumTruncatedBytes = 0;
			}
			memcpy(fTo+offset, av_data.ptr, fFrameSize-offset);

#ifdef STREAM_DETAIL_OUTPUT
			PrintStreamDetails();
#endif
			if (SerialLock > 0) {
				GetAVData(cmd[AV_UNLOCK_MP4], SerialLock, &av_data);
			}
			SerialLock = SerialBook;

			// Note the timestamp and size:
#if TIME_GET_WAY
			gettimeofday(&fPresentationTime, NULL);
#else
			fPresentationTime.tv_sec = av_data.timestamp/1000;
			fPresentationTime.tv_usec = (av_data.timestamp%1000)*1000;
#endif
			if( IsStart )
			{
				AV_DATA av_data_media;
				IsStart = 0;

				GetAVData(AV_OP_GET_MEDIA_INFO, -1, &av_data_media );
				if( av_data_media.flags == 1 )
				{
					Checkduration = STREAM_CHECK_DURATION_MS;
				}else{
					Checkduration = STREAM_CHECK_DURATION_MS;
				}

			}
			fDurationInMicroseconds = Checkduration;
			SerialBook++;
			return 1;
		}
		else if (ret == RET_NO_VALID_DATA) {
			return 0;
		}
		else {
			StreamFlag |= STREAM_NEW_GOP;
			return 0;
		}
	}

 	else if (	fInput.videoType == VIDEO_TYPE_H264||
				fInput.videoType == VIDEO_TYPE_H264_CIF)
 	{
#if 1
		int offset = 0;
		int start = 0;
		int *cmd = av_field[fInput.videoType];

		if (StreamFlag & STREAM_GET_VOL) {
			AV_DATA vol_data;
			if(GetAVData(cmd[AV_LOCK_MP4_VOL], -1, &vol_data) != RET_SUCCESS) {
				printf("Error on Get Vol data\n");
				return -1;
			}
			if( (vol_data.size == 18) || (vol_data.size  >= 256)  )
			{
				GetAVData(cmd[AV_UNLOCK_MP4_VOL], -1, &vol_data);
				printf("Stream %d is not avaliable~~~~~~~~\n",fInput.videoType);
				sigterm(0);
			}
			if(0)
			{
				memcpy(fTo+offset, vol_data.ptr, vol_data.size);
				offset += vol_data.size;
				fFrameSize = 0;
				nal_state = 5;
			}else{
				unsigned int i, sps=0, sps_len=0,pps=0;
				char *ptr =(char *)vol_data.ptr;
				if( nal_state == 0 )
				{

					for (i=0; i<vol_data.size; i++) {
						if (ptr[i]==0 && ptr[i+1]==0 && ptr[i+2]==0 && ptr[i+3]==1 && (ptr[i+4]&0x1f)==7)
							sps = i+4;
						if (sps) {
							if (ptr[i]==0 && ptr[i+1]==0 && ptr[i+2]==0 && ptr[i+3]==1 && (ptr[i+4]&0x1f)==8)
							{
								sps_len = i-sps;
								break;
							}
						}
					}
					if (sps_len) {

						fFrameSize = sps_len;
						if (fFrameSize > fMaxSize) {
							printf("Frame Truncated\n");
							fNumTruncatedBytes = fFrameSize - fMaxSize;
							fFrameSize = fMaxSize;
						}
						else {
							fNumTruncatedBytes = 0;
						}
						memcpy(fTo, ptr+sps, fFrameSize);
					}
					else {
						printf("SPS not found\n");
						return -1;
					}

				}else if(nal_state == 1 ){
					for (i=0; i<vol_data.size; i++) {
						if (ptr[i]==0 && ptr[i+1]==0 && ptr[i+2]==0 && ptr[i+3]==1 && (ptr[i+4]&0x1f)==8)
						{
							pps = i+4;
							break;
						}
					}
					if (pps) {

						fFrameSize = 4;
						if (fFrameSize > fMaxSize) {
							printf("Frame Truncated\n");
							fNumTruncatedBytes = fFrameSize - fMaxSize;
							fFrameSize = fMaxSize;
						}
						else {
							fNumTruncatedBytes = 0;
						}
						memcpy(fTo, ptr+pps, fFrameSize);
					}

				}
			}
			GetAVData(cmd[AV_UNLOCK_MP4_VOL], -1, &vol_data);

			if( (vol_data.size == 18) || (vol_data.size  >= 256)  )
			{
				printf("Stream %d is not avaliable~~~~~~~~\n",fInput.videoType);
				sigterm(0);
			}


			if( nal_state < 2 )
			{
				#if TIME_GET_WAY
				gettimeofday(&fPresentationTime, NULL);
				#else
				fPresentationTime.tv_sec = av_data.timestamp/1000;
				fPresentationTime.tv_usec = (av_data.timestamp%1000)*1000;
				#endif
				nal_state++;
				return 1;
			}

			WaitVideoStart( fInput.videoType,300 );

			ret = GetAVData(cmd[AV_LOCK_MP4_IFRAME], -1, &av_data);
			SerialBook = av_data.serial;
			StreamFlag &= ~(STREAM_GET_VOL|STREAM_NEW_GOP);
		}
		else if (StreamFlag & STREAM_NEW_GOP) {

			WaitVideoStart( fInput.videoType,5000);

			ret = GetAVData(cmd[AV_LOCK_MP4_IFRAME], -1, &av_data );
			SerialBook = av_data.serial;
			StreamFlag &= ~STREAM_NEW_GOP;
		}
		else {
			ret = GetAVData(cmd[AV_LOCK_MP4], SerialBook, &av_data );
		}

 		if (ret == RET_SUCCESS)
 		{
 			static int IscheckKey = 1;
 			if( av_data.flags == AV_FLAGS_MP4_I_FRAME && IscheckKey == 1 )
 			{
 				int serial_now;
 				serial_now = GetVideoSerial(fInput.videoType);
 				IscheckKey = 0;
 				//printf("serial_now = %d SerialBook = %d \n",serial_now,SerialBook);
 				if( (serial_now - SerialBook) > 30 )
 				{
 					GetAVData(cmd[AV_UNLOCK_MP4], SerialBook, &av_data);
 					StreamFlag |= STREAM_NEW_GOP;
 					return 0;
 				}
 			}else{
 				IscheckKey = 1;
 			}
 		}


 		start = 4;
 		if ((ret == RET_SUCCESS)/*&&(av_data.flags == AV_FLAGS_MP4_I_FRAME)*/)
 		{
 			unsigned int i;
			char *ptr = (char *)av_data.ptr;

			for (i=0; i<av_data.size; i++)
			{
				if (ptr[i]==0 && ptr[i+1]==0 && ptr[i+2]==0 && ptr[i+3]==1 && (ptr[i+4]&0x1f)<=5)
				{
					start = i+4;
					break;
				}
			}
			//printheader((char *)av_data.ptr, 300);

 		}

		if (ret == RET_SUCCESS) {
			fFrameSize = av_data.size + offset - start;
			if (fFrameSize > fMaxSize) {
				printf("Frame Truncated\n");
				fNumTruncatedBytes = fFrameSize - fMaxSize;
				fFrameSize = fMaxSize;
			}
			else {
				fNumTruncatedBytes = 0;
			}
			memcpy(fTo+offset, av_data.ptr+start, fFrameSize);

#ifdef STREAM_DETAIL_OUTPUT
			PrintStreamDetails();
#endif
			if (SerialLock > 0) {
				GetAVData(cmd[AV_UNLOCK_MP4], SerialLock, &av_data);
			}
			SerialLock = SerialBook;

			// Note the timestamp and size:
#if TIME_GET_WAY
			gettimeofday(&fPresentationTime, NULL);
#else
			fPresentationTime.tv_sec = av_data.timestamp/1000;
			fPresentationTime.tv_usec = (av_data.timestamp%1000)*1000;
#endif
			if( IsStart )
			{
				AV_DATA av_data_media;
				IsStart = 0;

				GetAVData(AV_OP_GET_MEDIA_INFO, -1, &av_data_media );
				if( av_data_media.flags == 1 )
				{
					Checkduration = STREAM_CHECK_DURATION_MS;
				}else{
					Checkduration = STREAM_CHECK_DURATION_MS;
				}

			}
			fDurationInMicroseconds = Checkduration;

			SerialBook++;
			return 1;
		}
		else if (ret == RET_NO_VALID_DATA) {
			return 0;
		}
		else {
			StreamFlag |= STREAM_NEW_GOP;
			return 0;
		}
#else

		return readFromFile264();
#endif

 	}
#if 1
 	else if ((fInput.videoType == VIDEO_TYPE_H264_SVC_30FPS)|| (fInput.videoType == VIDEO_TYPE_H264_SVC_15FPS)
				|| (fInput.videoType == VIDEO_TYPE_H264_SVC_7FPS) || (fInput.videoType == VIDEO_TYPE_H264_SVC_3FPS))
 	{
#if 1
		int offset = 0;
		int start = 0;
		int *cmd = av_field[fInput.videoType];

		if (StreamFlag & STREAM_GET_VOL) {
			AV_DATA vol_data;

			if(GetAVData(cmd[AV_LOCK_MP4_VOL], -1, &vol_data) != RET_SUCCESS) {
				printf("Error on Get Vol data\n");
				return -1;
			}
			if( (vol_data.size == 18) || (vol_data.size  >= 256)  )
			{
				GetAVData(cmd[AV_UNLOCK_MP4_VOL], -1, &vol_data);
				printf("Stream %d is not avaliable~~~~~~~~\n",fInput.videoType);
				sigterm(0);
			}
			if(0)
			{
				memcpy(fTo+offset, vol_data.ptr, vol_data.size);
				offset += vol_data.size;
				fFrameSize = 0;
				nal_state = 5;
			}else{
				unsigned int i, sps=0, sps_len=0,pps=0;
				char *ptr =(char *)vol_data.ptr;
				if( nal_state == 0 )
				{

					for (i=0; i<vol_data.size; i++) {
						if (ptr[i]==0 && ptr[i+1]==0 && ptr[i+2]==0 && ptr[i+3]==1 && (ptr[i+4]&0x1f)==7)
							sps = i+4;
						if (sps) {
							if (ptr[i]==0 && ptr[i+1]==0 && ptr[i+2]==0 && ptr[i+3]==1 && (ptr[i+4]&0x1f)==8)
							{
								sps_len = i-sps;
								break;
							}
						}
					}
					if (sps_len) {

						fFrameSize = sps_len;
						if (fFrameSize > fMaxSize) {
							printf("Frame Truncated\n");
							fNumTruncatedBytes = fFrameSize - fMaxSize;
							fFrameSize = fMaxSize;
						}
						else {
							fNumTruncatedBytes = 0;
						}
						memcpy(fTo, ptr+sps, fFrameSize);
					}
					else {
						printf("SPS not found\n");
						return -1;
					}

				}else if(nal_state == 1 ){
					for (i=0; i<vol_data.size; i++) {
						if (ptr[i]==0 && ptr[i+1]==0 && ptr[i+2]==0 && ptr[i+3]==1 && (ptr[i+4]&0x1f)==8)
						{
							pps = i+4;
							break;
						}
					}
					if (pps) {

						fFrameSize = 4;
						if (fFrameSize > fMaxSize) {
							printf("Frame Truncated\n");
							fNumTruncatedBytes = fFrameSize - fMaxSize;
							fFrameSize = fMaxSize;
						}
						else {
							fNumTruncatedBytes = 0;
						}
						memcpy(fTo, ptr+pps, fFrameSize);
					}

				}
			}
			GetAVData(cmd[AV_UNLOCK_MP4_VOL], -1, &vol_data);

			if( (vol_data.size == 18) || (vol_data.size  >= 256)  )
			{
				printf("Stream %d is not avaliable~~~~~~~~\n",fInput.videoType);
				sigterm(0);
			}


			if( nal_state < 2 )
			{
				#if TIME_GET_WAY
				gettimeofday(&fPresentationTime, NULL);
				#else
				fPresentationTime.tv_sec = av_data.timestamp/1000;
				fPresentationTime.tv_usec = (av_data.timestamp%1000)*1000;
				#endif
				nal_state++;
				return 1;
			}

			WaitVideoStart( fInput.videoType,300 );

			ret = GetAVData(cmd[AV_LOCK_MP4_IFRAME], -1, &av_data);
			SerialBook = av_data.serial;
			StreamFlag &= ~(STREAM_GET_VOL|STREAM_NEW_GOP);

		}
		else if (StreamFlag & STREAM_NEW_GOP) {

			WaitVideoStart( fInput.videoType,5000);

			ret = GetAVData(cmd[AV_LOCK_MP4_IFRAME], -1, &av_data );
			SerialBook = av_data.serial;
			StreamFlag &= ~STREAM_NEW_GOP;
		}
		else {
			ret = GetAVData(cmd[AV_LOCK_MP4], SerialBook, &av_data );
		}

 		if (ret == RET_SUCCESS)
 		{
 			static int IscheckKey = 1;
 			if( av_data.flags == AV_FLAGS_MP4_I_FRAME && IscheckKey == 1 )
 			{
 				int serial_now;
 				serial_now = GetVideoSerial(fInput.videoType);
 				IscheckKey = 0;
 				//printf("serial_now = %d SerialBook = %d \n",serial_now,SerialBook);
 				if( (serial_now - SerialBook) > 30 )
 				{
 					GetAVData(cmd[AV_UNLOCK_MP4], SerialBook, &av_data);
 					StreamFlag |= STREAM_NEW_GOP;
 					return 0;
 				}
 			}else{
 				IscheckKey = 1;
 			}
 		}


 		start = 4;
 		if ((ret == RET_SUCCESS)/*&&(av_data.flags == AV_FLAGS_MP4_I_FRAME)*/)
 		{
 			unsigned int i;
			char *ptr = (char *)av_data.ptr;

			for (i=0; i<av_data.size; i++)
			{
				if (ptr[i]==0 && ptr[i+1]==0 && ptr[i+2]==0 && ptr[i+3]==1 && (ptr[i+4]&0x1f)<=5)
				{
					start = i+4;
					break;
				}
			}
			//printheader((char *)av_data.ptr, 300);
 		}
		if (ret == RET_SUCCESS) {
			//printf("SVC DEBUG MSG: %d\n", av_data.temporalId);
			if ((fInput.videoType == VIDEO_TYPE_H264_SVC_30FPS) ||
				((fInput.videoType == VIDEO_TYPE_H264_SVC_15FPS) && (av_data.temporalId <=2 )) ||
					((fInput.videoType == VIDEO_TYPE_H264_SVC_7FPS) && (av_data.temporalId <= 1)) ||
						((fInput.videoType == VIDEO_TYPE_H264_SVC_3FPS) && (av_data.temporalId == 0))) {
				fFrameSize = av_data.size + offset - start;
				if (fFrameSize > fMaxSize) {
					printf("Frame Truncated\n");
					fNumTruncatedBytes = fFrameSize - fMaxSize;
					fFrameSize = fMaxSize;
				}
				else {
					fNumTruncatedBytes = 0;
				}
				memcpy(fTo+offset, av_data.ptr+start, fFrameSize);
			}
			else {
				fFrameSize = 0;
				fNumTruncatedBytes = 0;
			}

#ifdef STREAM_DETAIL_OUTPUT
			PrintStreamDetails();
#endif
			if (SerialLock > 0) {
				GetAVData(cmd[AV_UNLOCK_MP4], SerialLock, &av_data);
			}
			SerialLock = SerialBook;

			// Note the timestamp and size:
#if TIME_GET_WAY
			gettimeofday(&fPresentationTime, NULL);
#else
			fPresentationTime.tv_sec = av_data.timestamp/1000;
			fPresentationTime.tv_usec = (av_data.timestamp%1000)*1000;
#endif
			if( IsStart )
			{
				AV_DATA av_data_media;
				IsStart = 0;

				GetAVData(AV_OP_GET_MEDIA_INFO, -1, &av_data_media );
				if( av_data_media.flags == 1 )
				{
					Checkduration = STREAM_CHECK_DURATION_MS;
				}else{
					Checkduration = STREAM_CHECK_DURATION_MS;
				}

			}
			fDurationInMicroseconds = Checkduration;

			SerialBook++;
			return 1;
		}
		else if (ret == RET_NO_VALID_DATA) {
			return 0;
		}
		else {
			StreamFlag |= STREAM_NEW_GOP;
			return 0;
		}
#else
		return readFromFile264();
#endif

 	}
#endif
	else
	{
		return 0;
	}
}


////////// AudioOpenFileSource implementation //////////

AudioOpenFileSource
::AudioOpenFileSource(UsageEnvironment& env, APPROInput& input)
  : OpenFileSource(env, input), AudioBook(0), AudioLock(0), IsStart(1) {
  uSecsToDelay = 5000;
  uSecsToDelayMax = 125000;
  srcType = 1;
}

AudioOpenFileSource::~AudioOpenFileSource() {
  fInput.fOurAudioSource = NULL;
  if (AudioLock > 0) {
    GetAVData(AV_OP_UNLOCK_ULAW, AudioLock, NULL);
    AudioLock = 0;
  }

}

int AudioOpenFileSource::getAudioData() {
	AV_DATA av_data;
	int ret;

	if (AudioBook == 0) {
		GetAVData(AV_OP_GET_ULAW_SERIAL, -1, &av_data );
		if (av_data.serial <= AudioLock) {
			printf("av_data.serial <= audio_lock!!!\n");
			return 0;
		}
		AudioBook = av_data.serial;
	}

	ret = GetAVData(AV_OP_LOCK_ULAW, AudioBook, &av_data );
	if (ret == RET_SUCCESS) {
		if (AudioLock > 0) {
			GetAVData(AV_OP_UNLOCK_ULAW, AudioLock, NULL);
			AudioLock = 0;
		}
		if (av_data.size > fMaxSize)
			av_data.size = fMaxSize;
		memcpy(fTo, av_data.ptr, av_data.size);

		AudioLock = av_data.serial;
		AudioBook = av_data.serial + 1;
#if TIME_GET_WAY
		gettimeofday(&fPresentationTime, NULL);
#else
		fPresentationTime.tv_sec = av_data.timestamp/1000;
		fPresentationTime.tv_usec = (av_data.timestamp%1000)*1000;
#endif
 		return av_data.size;
	}
	else if (ret == RET_NO_VALID_DATA) {
		return 0;
	}
	else {
		AudioBook = 0;
		dbg("ERROR, ret=%d\n", ret);
		return -1;
	}
}


int AudioOpenFileSource::readFromFile() {
  int timeinc;

  if (!audio_enable)  return 0;

  // Read available audio data:
  int ret = getAudioData();

  if (ret <= 0) return 0;
  if (ret < 0) ret = 0;
  fFrameSize = (unsigned)ret;
  fNumTruncatedBytes = 0;

#if (TIME_GET_WAY)
/* PR#2665 fix from Robin
   * Assuming audio format = AFMT_S16_LE
   * Get the current time
   * Substract the time increment of the audio oss buffer, which is equal to
   * buffer_size / channel_number / sample_rate / sample_size ==> 400+ millisec
   */
 timeinc = fFrameSize * 1000 / audioNumChannels / (audioSamplingFrequency/1000) ;/// 2;
 while (fPresentationTime.tv_usec < timeinc)
 {
   fPresentationTime.tv_sec -= 1;
   timeinc -= 1000000;

 }

 fPresentationTime.tv_usec -= timeinc;

#else
  timeinc = fFrameSize*1000 / audioNumChannels / (audioSamplingFrequency/1000);
  if( IsStart )
  {
  	IsStart = 0;
  	fPresentationTimePre = fPresentationTime;
  	fDurationInMicroseconds = timeinc;
  }else{
	fDurationInMicroseconds = timevaldiff(&fPresentationTimePre, &fPresentationTime )*1000;
	fPresentationTimePre = fPresentationTime;
  }


  if( fDurationInMicroseconds < timeinc)
  {
  	unsigned long msec;
  	//printf("1.fPresentationTime.tv_sec = %d fPresentationTime.tv_usec = %d \n",fPresentationTime.tv_sec,fPresentationTime.tv_usec);
	msec = fPresentationTime.tv_usec;
  	msec += (timeinc - fDurationInMicroseconds);
	fPresentationTime.tv_sec += msec/1000000;
	fPresentationTime.tv_usec = msec%1000000;
	//printf("2.fPresentationTime.tv_sec = %d fPresentationTime.tv_usec = %d \n",fPresentationTime.tv_sec,fPresentationTime.tv_usec);
	fDurationInMicroseconds = timeinc;

	fPresentationTimePre = fPresentationTime;
  }
#endif


  return 1;
}

int GetVolInfo(int vType,void *pBuff,int bufflen)
{

	int *cmd = av_field[vType];
	AV_DATA vol_data;

	if(GetAVData(cmd[AV_LOCK_MP4_VOL], -1, &vol_data) != RET_SUCCESS)
	{
		printf("Error on Get Vol data\n");
		return 0;
	}

	memcpy(pBuff, vol_data.ptr, vol_data.size);
	GetAVData(cmd[AV_UNLOCK_MP4_VOL], -1, &vol_data);

	return vol_data.size;
}

int GetSprop(void *pBuff, char vType)
{
	static char tempBuff[512];
	int ret = 0;
	int cnt = 0;
	int IsSPS = 0;
	int IsPPS = 0;
	int SPS_LEN = 0;
	int PPS_LEN = 4;
	char *pSPS=tempBuff;//0x7
	char *pPPS=tempBuff;//0x8
	char *pSPSEncode=NULL;
	char *pPPSEncode=NULL;

	ret = GetVolInfo(vType,tempBuff,sizeof(tempBuff));

	for(;;)
	{
		if(pSPS[0]==0&&pSPS[1]==0&&pSPS[2]==0&&pSPS[3]==1)
		{
			if( (pSPS[4]& 0x1F) == 7 )
			{
				IsSPS = 1;
				break;
			}
		}
		pSPS++;
		cnt++;
		if( (cnt+4)>ret )
			break;
	}
	if(IsSPS)
		pSPS += 4;

	cnt = 0;
	for(;;)
	{
		if(pPPS[0]==0&&pPPS[1]==0&&pPPS[2]==0&&pPPS[3]==1)
		{
			if( (pPPS[4]& 0x1F) == 8 )
			{
				IsPPS = 1;
				break;
			}
		}
		pPPS++;
		cnt++;
		if( (cnt+4)>ret )
			break;
	}

	if(IsPPS)
		pPPS += 4;

	SPS_LEN = (unsigned int)pPPS - (unsigned int)pSPS;

	pSPSEncode = base64Encode(pSPS,SPS_LEN);
	pPPSEncode = base64Encode(pPPS,PPS_LEN);

	sprintf((char *)pBuff,"%s,%s",(char *)pSPSEncode,(char *)pPPSEncode);
	//printf("vType = %d pBuff = %s \n",vType,(char *)pBuff);

	delete[] pSPSEncode;
	delete[] pPPSEncode;

	return 1;
}

