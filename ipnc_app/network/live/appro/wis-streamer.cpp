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
// An application that streams audio/video captured by a WIS GO7007,
// using a built-in RTSP server.
// main program

#include <signal.h>
#include <BasicUsageEnvironment.hh>
#include <getopt.h>
#include <liveMedia.hh>
#include "Err.hh"
#include "WISJPEGVideoServerMediaSubsession.hh"
#include "WISMPEG4VideoServerMediaSubsession.hh"
#include "WISH264VideoServerMediaSubsession.hh"
#include "WISPCMAudioServerMediaSubsession.hh"
#include <ApproDrvMsg.h>
#include <Appro_interface.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <GroupsockHelper.hh>
#include "WISJPEGStreamSource.hh"


enum  StreamingMode
{
	STREAMING_UNICAST,
	STREAMING_UNICAST_THROUGH_DARWIN,
	STREAMING_MULTICAST_ASM,
	STREAMING_MULTICAST_SSM
};

enum{
	AUDIO_G711 = 0,
	AUDIO_AAC
};

portNumBits rtspServerPortNum = 8557;
char const* MjpegStreamName = "0?videoCodecType=MJPEG";  //mjpeg  0?videoCodecType=MJPEG
char const* Mpeg4StreamName = "1?videoCodecType=MPEG4";  //mpeg4  1?videoCodecType=MPEG4
char const* H264StreamName = "2?videoCodecType=H.264";    //h264  2?videoCodecType=H.264
char const* streamDescription = "RTSP/RTP stream from IPNC";

int MjpegVideoBitrate = 12000000;
int Mpeg4VideoBitrate = 12000000;
int H264VideoBitrate = 12000000;
int audioOutputBitrate = 128000;

unsigned audioSamplingFrequency = 16000;
unsigned audioNumChannels = 1;
int audio_enable = 1;
unsigned audioType = AUDIO_G711;
char watchVariable = 0;
char videoType = -1;
char IsAudioAlarm = 0;

int qid = 0;

void sigterm(int dummy)
{
	printf("caught SIGTERM: shutting down\n");
	ApproInterfaceExit();
	exit(1);
}

void sigint(int dummy)
{
	if( IsAudioAlarm == 1 )
	{
		printf("Audio Alarm!!\n");
		IsAudioAlarm = 0;
		return;
	}

	printf("caught SIGINT: shutting down\n");
	watchVariable = 1;
	alarm(1);
}

void sig_audio_enable(int dummy)
{
	printf("Audio enabled!\n");
	audio_enable = 1;
	IsAudioAlarm = 1;
	alarm(2);
}

void sig_audio_disable(int dummy)
{
	printf("Audio disabled!\n");
	audio_enable = 0;
	IsAudioAlarm = 1;
	alarm(2);
}

void init_signals(void)
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGTERM);
	sigaddset(&sa.sa_mask, SIGINT);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sigaddset(&sa.sa_mask, SIGALRM);

	sa.sa_flags = 0;
	sa.sa_handler = sigterm;
	sigaction(SIGTERM, &sa, NULL);

	sa.sa_flags = 0;
	sa.sa_handler = sigint;
	sigaction(SIGINT, &sa, NULL);

	sa.sa_flags = 0;
	sa.sa_handler = sig_audio_enable;
	sigaction(SIGUSR1, &sa, NULL);

	sa.sa_flags = 0;
	sa.sa_handler = sig_audio_disable;
	sigaction(SIGUSR2, &sa, NULL);

	sa.sa_flags = 0;
	sa.sa_handler = sigint;
	sigaction( SIGALRM, &sa, NULL );
}

void share_memory_init(int msg_type)
{
  if(ApproDrvInit(msg_type))
    exit(1);
  if (func_get_mem(&qid)) {
	ApproDrvExit();
    exit(1);
  }
}
#include <fcntl.h>
int GetSampleRate( void )
{
	static int CurrentStatus = -255;
	int fd_proc = -1;
	int ret = -1;
	char StrBuffer[300];
	char TmpBuf[50];
	char *pStr = NULL;
	int samplerate = 0;
	char delima_buf[] = ":\r\n ";

	if( CurrentStatus >= -1 )
	{
		fprintf(stderr,"CurrentStatus is = %d \n", CurrentStatus);
		return CurrentStatus;
	}


	fd_proc = open("/proc/asound/card0/pcm0c/sub0/hw_params", O_RDONLY);
	if( fd_proc < 0 )
	{
		fprintf(stderr,"GetSampleRate open file fail \n");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = read(fd_proc,  StrBuffer, sizeof(StrBuffer)-1);
	if( ret <= 0 )
	{
		fprintf(stderr,"read device error !!");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = -1;
	StrBuffer[sizeof(StrBuffer)-1] = '\0';
	pStr = strtok(StrBuffer,delima_buf );

	while( pStr != NULL )
	{
		sscanf( pStr,"%s",TmpBuf);

		if( strncmp(TmpBuf, "rate", sizeof("rate")) == 0 )
		{

			pStr = strtok(NULL, delima_buf);

			sscanf( pStr,"%d",&samplerate);

			//fprintf(stderr,"samplerate = %d \n", samplerate);

			ret = samplerate;
			goto CHECK_CPU_END;


		}

		pStr = strtok(NULL, delima_buf);

	}

CHECK_CPU_END:

	if( fd_proc >= 0 )
		close(fd_proc);

	CurrentStatus = ret;

	return ret;
}

int main(int argc, char** argv) {
  init_signals();
  setpriority(PRIO_PROCESS, 0, 0);
  int IsSilence = 0;
  int svcEnable = 0;
  int cnt=0;
  int activePortCnt=0;
  if( GetSampleRate() == 16000 )
  {
	audioOutputBitrate = 128000;
	audioSamplingFrequency = 16000;
  }else{
	audioOutputBitrate = 64000;
	audioSamplingFrequency = 8000;
  }
  // Begin by setting up our usage environment:
  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
  int msg_type, video_type;
  APPROInput* MjpegInputDevice = NULL;
  APPROInput* H264InputDevice = NULL;
  APPROInput* Mpeg4InputDevice = NULL;
  static pid_t child[4] = {
	-1,-1,-1,-1
  };

  StreamingMode streamingMode = STREAMING_UNICAST;
  netAddressBits multicastAddress = 0;//our_inet_addr("224.1.4.6");
  portNumBits videoRTPPortNum = 0;
  portNumBits audioRTPPortNum = 0;

  IsSilence = 0;
  svcEnable = 0;
  audioType = AUDIO_G711;
  streamingMode = STREAMING_UNICAST;

  for( cnt = 1; cnt < argc ;cnt++ )
  {
	if( strcmp( argv[cnt],"-m" )== 0  )
	{
		streamingMode = STREAMING_MULTICAST_SSM;
	}

	if( strcmp( argv[cnt],"-s" )== 0  )
	{
		IsSilence = 1;
	}

	if( strcmp( argv[cnt],"-a" )== 0  )
	{
		audioType = AUDIO_AAC;
	}

	if( strcmp( argv[cnt],"-v" )== 0  )
	{
		svcEnable = 1;
	}
  }

#if 0
  printf("###########IsSilence = %d ################\n",IsSilence);
  printf("###########streamingMode = %d ################\n",streamingMode);
  printf("###########audioType = %d ################\n",audioType);
  printf("###########svcEnable = %d ################\n",svcEnable);
#endif

  child[0] = fork();

  if( child[0] != 0 )
  {
	child[1] = fork();
  }

  if( child[0] != 0 && child[1] != 0 )
  {
	child[2] = fork();
  }

  if( child[0] != 0 && child[1] != 0 && child[2] != 0 )
  {
	child[3] = fork();
  }

  if(svcEnable) {
	  if( child[0] != 0 && child[1] != 0 && child[2] != 0 && child[3] != 0)
	  {
		child[4] = fork();
	  }

	  if( child[0] != 0 && child[1] != 0 && child[2] != 0 && child[3] != 0 && child[4] != 0)
	  {
		child[5] = fork();
	  }

	  if( child[0] != 0 && child[1] != 0 && child[2] != 0 && child[3] != 0 && child[4] != 0 && child[5] != 0)
	  {
		child[6] = fork();
	  }

	  if( child[0] != 0 && child[1] != 0 && child[2] != 0 && child[3] != 0 && child[4] != 0 && child[5] != 0 && child[6] != 0)
	  {
		child[7] = fork();
	  }
  }

  if( child[0] == 0 )
  {
	/* parent, success */
	msg_type = LIVE_MSG_TYPE4;
	video_type = VIDEO_TYPE_H264_CIF;
	rtspServerPortNum = 8556;
	H264VideoBitrate = 12000000;
	videoRTPPortNum = 6012;
	audioRTPPortNum = 6014;
  }
  if( child[1] == 0 )
  {
	/* parent, success */
	msg_type = LIVE_MSG_TYPE3;
	video_type = VIDEO_TYPE_MJPEG;
	rtspServerPortNum = 8555;
	MjpegVideoBitrate = 12000000;
	videoRTPPortNum = 6008;
	audioRTPPortNum = 6010;
  }
  if( child[2] == 0 )
  {
	/* parent, success */
	msg_type = LIVE_MSG_TYPE;
	video_type = VIDEO_TYPE_MPEG4;
	rtspServerPortNum = 8553;
	Mpeg4VideoBitrate = 12000000;
	videoRTPPortNum = 6000;
	audioRTPPortNum = 6002;
  }
  if( child[3] == 0 )
  {
	/* parent, success */
	msg_type = LIVE_MSG_TYPE2;
	video_type = VIDEO_TYPE_MPEG4_CIF;
	rtspServerPortNum = 8554;
	Mpeg4VideoBitrate = 12000000;
	videoRTPPortNum = 6004;
	audioRTPPortNum = 6006;
  }

  if(svcEnable) {
	  if( child[4] == 0 )
	  {
		/* parent, success */
		msg_type = LIVE_MSG_TYPE5;
		video_type = VIDEO_TYPE_H264_SVC_30FPS;
		rtspServerPortNum = 8601;
		H264VideoBitrate = 12000000;
		videoRTPPortNum = 6016;
		audioRTPPortNum = 6018;
	  }
	  if( child[5] == 0 )
	  {
		/* parent, success */
		msg_type = LIVE_MSG_TYPE6;
		video_type = VIDEO_TYPE_H264_SVC_15FPS;
		rtspServerPortNum = 8602;
		H264VideoBitrate = 12000000;
		videoRTPPortNum = 6020;
		audioRTPPortNum = 6022;
	  }
	  if( child[6] == 0 )
	  {
		/* parent, success */
		msg_type = LIVE_MSG_TYPE7;
		video_type = VIDEO_TYPE_H264_SVC_7FPS;
		rtspServerPortNum = 8603;
		H264VideoBitrate = 12000000;
		videoRTPPortNum = 6024;
		audioRTPPortNum = 6026;
	  }
	  if( child[7] == 0 )
	  {
		/* parent, success */
		msg_type = LIVE_MSG_TYPE8;
		video_type = VIDEO_TYPE_H264_SVC_3FPS;
		rtspServerPortNum = 8604;
		H264VideoBitrate = 12000000;
		videoRTPPortNum = 6028;
		audioRTPPortNum = 6030;
	  }
	  if( child[0] != 0 && child[1] != 0 && child[2] != 0 && child[3] != 0 && child[4] != 0 && child[5] != 0 && child[6] != 0 && child[7] != 0)
	  {
		/* parent, success */
		msg_type = LIVE_MSG_TYPE9;
		video_type = VIDEO_TYPE_H264;
		rtspServerPortNum = 8557;
		H264VideoBitrate = 12000000;
		videoRTPPortNum = 6032;
		audioRTPPortNum = 6034;
	  }
 }
 else {
  	  if( child[0] != 0 && child[1] != 0 && child[2] != 0 && child[3] != 0)
	  {
		/* parent, success */
		msg_type = LIVE_MSG_TYPE5;
		video_type = VIDEO_TYPE_H264;
		rtspServerPortNum = 8557;
		H264VideoBitrate = 12000000;
		videoRTPPortNum = 6032;
		audioRTPPortNum = 6034;
	  }
 }

  videoType = video_type;

  // Objects used for multicast streaming:
  static Groupsock* rtpGroupsockAudio = NULL;
  static Groupsock* rtcpGroupsockAudio = NULL;
  static Groupsock* rtpGroupsockVideo = NULL;
  static Groupsock* rtcpGroupsockVideo = NULL;
  static FramedSource* sourceAudio = NULL;
  static RTPSink* sinkAudio = NULL;
  static RTCPInstance* rtcpAudio = NULL;
  static FramedSource* sourceVideo = NULL;
  static RTPSink* sinkVideo = NULL;
  static RTCPInstance* rtcpVideo = NULL;

  share_memory_init(msg_type);

  //init_signals();

  *env << "Initializing...\n";


  // Initialize the WIS input device:
  if( video_type == VIDEO_TYPE_MJPEG)
  {
	  MjpegInputDevice = APPROInput::createNew(*env, VIDEO_TYPE_MJPEG);
	  if (MjpegInputDevice == NULL) {
	    err(*env) << "Failed to create MJPEG input device\n";
	    exit(1);
	  }
  }

  if( video_type == VIDEO_TYPE_H264 || video_type == VIDEO_TYPE_H264_CIF || video_type == VIDEO_TYPE_H264_SVC_30FPS ||
		video_type == VIDEO_TYPE_H264_SVC_15FPS || video_type == VIDEO_TYPE_H264_SVC_7FPS || video_type == VIDEO_TYPE_H264_SVC_3FPS)
  {
	  H264InputDevice = APPROInput::createNew(*env, video_type);
	  if (H264InputDevice == NULL) {
	    err(*env) << "Failed to create MJPEG input device\n";
	    exit(1);
	  }
  }

  if( video_type == VIDEO_TYPE_MPEG4 || video_type == VIDEO_TYPE_MPEG4_CIF )
  {
	  Mpeg4InputDevice = APPROInput::createNew(*env, video_type);
	  if (Mpeg4InputDevice == NULL) {
		err(*env) << "Failed to create MPEG4 input device\n";
		exit(1);
	  }
  }

  // Create the RTSP server:
  RTSPServer* rtspServer = NULL;
  // Normal case: Streaming from a built-in RTSP server:
  rtspServer = RTSPServer::createNew(*env, rtspServerPortNum, NULL);
  if (rtspServer == NULL) {
    *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
    exit(1);
  }

  *env << "...done initializing\n";

  if( streamingMode == STREAMING_UNICAST )
  {
	  if( video_type == VIDEO_TYPE_MJPEG)
	  {
	    ServerMediaSession* sms
	      = ServerMediaSession::createNew(*env, MjpegStreamName, MjpegStreamName, streamDescription,streamingMode == STREAMING_MULTICAST_SSM);
	    sms->addSubsession(WISJPEGVideoServerMediaSubsession
				 ::createNew(sms->envir(), *MjpegInputDevice, MjpegVideoBitrate));
	    if( IsSilence == 0)
	    {
			sms->addSubsession(WISPCMAudioServerMediaSubsession::createNew(sms->envir(), *MjpegInputDevice));
	    }

	    rtspServer->addServerMediaSession(sms);

	    char *url = rtspServer->rtspURL(sms);
	    *env << "Play this stream using the URL:\n\t" << url << "\n";
	    delete[] url;
	  }

	  if( video_type == VIDEO_TYPE_H264 || video_type == VIDEO_TYPE_H264_CIF || video_type == VIDEO_TYPE_H264_SVC_30FPS ||
			video_type == VIDEO_TYPE_H264_SVC_15FPS || video_type == VIDEO_TYPE_H264_SVC_7FPS || video_type ==VIDEO_TYPE_H264_SVC_3FPS)
	  {
            ServerMediaSession* sms;
            sms
	      = ServerMediaSession::createNew(*env, H264StreamName, H264StreamName, streamDescription,streamingMode == STREAMING_MULTICAST_SSM);
	    sms->addSubsession(WISH264VideoServerMediaSubsession
				 ::createNew(sms->envir(), *H264InputDevice, H264VideoBitrate));
	    if( IsSilence == 0)
	    {
	    	sms->addSubsession(WISPCMAudioServerMediaSubsession::createNew(sms->envir(), *H264InputDevice));

	    }
	    rtspServer->addServerMediaSession(sms);

	    char *url = rtspServer->rtspURL(sms);
	    *env << "Play this stream using the URL:\n\t" << url << "\n";
	    delete[] url;
	  }

	    // Create a record describing the media to be streamed:
	  if( video_type == VIDEO_TYPE_MPEG4 || video_type == VIDEO_TYPE_MPEG4_CIF )
	  {
	    ServerMediaSession* sms
	      = ServerMediaSession::createNew(*env, Mpeg4StreamName, Mpeg4StreamName, streamDescription,streamingMode == STREAMING_MULTICAST_SSM);
	    sms->addSubsession(WISMPEG4VideoServerMediaSubsession
				 ::createNew(sms->envir(), *Mpeg4InputDevice, Mpeg4VideoBitrate));
	    if( IsSilence == 0)
	    {
	    	sms->addSubsession(WISPCMAudioServerMediaSubsession::createNew(sms->envir(), *Mpeg4InputDevice));
	    }

	    rtspServer->addServerMediaSession(sms);


	    char *url = rtspServer->rtspURL(sms);
	    *env << "Play this stream using the URL:\n\t" << url << "\n";
	    delete[] url;
	  }
  }else{


	if (streamingMode == STREAMING_MULTICAST_SSM)
	{
		if (multicastAddress == 0)
			multicastAddress = chooseRandomIPv4SSMAddress(*env);
	} else if (multicastAddress != 0) {
		streamingMode = STREAMING_MULTICAST_ASM;
	}

	struct in_addr dest; dest.s_addr = multicastAddress;
	const unsigned char ttl = 255;

	// For RTCP:
	const unsigned maxCNAMElen = 100;
	unsigned char CNAME[maxCNAMElen + 1];
	gethostname((char *) CNAME, maxCNAMElen);
	CNAME[maxCNAMElen] = '\0';      // just in case

	ServerMediaSession* sms=NULL;

	if( video_type == VIDEO_TYPE_MJPEG)
	{
		sms = ServerMediaSession::createNew(*env, MjpegStreamName, MjpegStreamName, streamDescription,streamingMode == STREAMING_MULTICAST_SSM);

		sourceAudio = MjpegInputDevice->audioSource();
		sourceVideo = WISJPEGStreamSource::createNew(MjpegInputDevice->videoSource());
		// Create 'groupsocks' for RTP and RTCP:
	    const Port rtpPortVideo(videoRTPPortNum);
	    const Port rtcpPortVideo(videoRTPPortNum+1);
	    rtpGroupsockVideo = new Groupsock(*env, dest, rtpPortVideo, ttl);
	    rtcpGroupsockVideo = new Groupsock(*env, dest, rtcpPortVideo, ttl);
	    if (streamingMode == STREAMING_MULTICAST_SSM) {
	      rtpGroupsockVideo->multicastSendOnly();
	      rtcpGroupsockVideo->multicastSendOnly();
	    }
		setVideoRTPSinkBufferSize();
		sinkVideo = JPEGVideoRTPSink::createNew(*env, rtpGroupsockVideo);

	}

	if( video_type == VIDEO_TYPE_H264 || video_type == VIDEO_TYPE_H264_CIF ||
		video_type == VIDEO_TYPE_H264_SVC_30FPS || video_type == VIDEO_TYPE_H264_SVC_15FPS ||
			video_type == VIDEO_TYPE_H264_SVC_7FPS || video_type == VIDEO_TYPE_H264_SVC_3FPS)
	{
 		sms = ServerMediaSession::createNew(*env, H264StreamName, H264StreamName, streamDescription,streamingMode == STREAMING_MULTICAST_SSM);

		sourceAudio = H264InputDevice->audioSource();
		sourceVideo = H264VideoStreamFramer::createNew(*env, H264InputDevice->videoSource());

		// Create 'groupsocks' for RTP and RTCP:
	    const Port rtpPortVideo(videoRTPPortNum);
	    const Port rtcpPortVideo(videoRTPPortNum+1);
	    rtpGroupsockVideo = new Groupsock(*env, dest, rtpPortVideo, ttl);
	    rtcpGroupsockVideo = new Groupsock(*env, dest, rtcpPortVideo, ttl);
	    if (streamingMode == STREAMING_MULTICAST_SSM) {
	      rtpGroupsockVideo->multicastSendOnly();
	      rtcpGroupsockVideo->multicastSendOnly();
	    }
		setVideoRTPSinkBufferSize();
		{
			char BuffStr[200];
			extern int GetSprop(void *pBuff, char vType);
			GetSprop(BuffStr,video_type);
			sinkVideo = H264VideoRTPSink::createNew(*env, rtpGroupsockVideo,96, 0x64001F,BuffStr);
		}

	}

	// Create a record describing the media to be streamed:
	if( video_type == VIDEO_TYPE_MPEG4 || video_type == VIDEO_TYPE_MPEG4_CIF )
	{
		sms = ServerMediaSession::createNew(*env, Mpeg4StreamName, Mpeg4StreamName, streamDescription,streamingMode == STREAMING_MULTICAST_SSM);

		sourceAudio = Mpeg4InputDevice->audioSource();
		sourceVideo = MPEG4VideoStreamDiscreteFramer::createNew(*env, Mpeg4InputDevice->videoSource());

		// Create 'groupsocks' for RTP and RTCP:
	    const Port rtpPortVideo(videoRTPPortNum);
	    const Port rtcpPortVideo(videoRTPPortNum+1);
	    rtpGroupsockVideo = new Groupsock(*env, dest, rtpPortVideo, ttl);
	    rtcpGroupsockVideo = new Groupsock(*env, dest, rtcpPortVideo, ttl);
	    if (streamingMode == STREAMING_MULTICAST_SSM) {
	      rtpGroupsockVideo->multicastSendOnly();
	      rtcpGroupsockVideo->multicastSendOnly();
	    }
		setVideoRTPSinkBufferSize();
		sinkVideo = MPEG4ESVideoRTPSink::createNew(*env, rtpGroupsockVideo,97);

	}
	/* VIDEO Channel initial */
	if(1)
	{
		// Create (and start) a 'RTCP instance' for this RTP sink:
		unsigned totalSessionBandwidthVideo = (Mpeg4VideoBitrate+500)/1000; // in kbps; for RTCP b/w share
		rtcpVideo = RTCPInstance::createNew(*env, rtcpGroupsockVideo,
					totalSessionBandwidthVideo, CNAME,
					sinkVideo, NULL /* we're a server */ ,
					streamingMode == STREAMING_MULTICAST_SSM);
	    // Note: This starts RTCP running automatically
		sms->addSubsession(PassiveServerMediaSubsession::createNew(*sinkVideo, rtcpVideo));

		// Start streaming:
		sinkVideo->startPlaying(*sourceVideo, NULL, NULL);
	}
	/* AUDIO Channel initial */
	if( IsSilence == 0)
	{
		// there's a separate RTP stream for audio
		// Create 'groupsocks' for RTP and RTCP:
		const Port rtpPortAudio(audioRTPPortNum);
		const Port rtcpPortAudio(audioRTPPortNum+1);

		rtpGroupsockAudio = new Groupsock(*env, dest, rtpPortAudio, ttl);
		rtcpGroupsockAudio = new Groupsock(*env, dest, rtcpPortAudio, ttl);

		if (streamingMode == STREAMING_MULTICAST_SSM)
		{
			rtpGroupsockAudio->multicastSendOnly();
			rtcpGroupsockAudio->multicastSendOnly();
		}
		if( audioSamplingFrequency == 16000 )
		{

			if( audioType == AUDIO_G711)
			{
				sinkAudio = SimpleRTPSink::createNew(*env, rtpGroupsockAudio, 96, audioSamplingFrequency, "audio", "PCMU", 1);
			}
			else
			{
				char const* encoderConfigStr = "1408";// (2<<3)|(8>>1) = 0x14 ; ((8<<7)&0xFF)|(1<<3)=0x08 ;
				sinkAudio = MPEG4GenericRTPSink::createNew(*env, rtpGroupsockAudio,
						       96,
						       audioSamplingFrequency,
						       "audio", "AAC-hbr",
						       encoderConfigStr, audioNumChannels);
			}
		}
		else{
			if(audioType == AUDIO_G711)
			{
				sinkAudio = SimpleRTPSink::createNew(*env, rtpGroupsockAudio, 0, audioSamplingFrequency, "audio", "PCMU", 1);
			}
			else{
				char const* encoderConfigStr =  "1588";// (2<<3)|(11>>1) = 0x15 ; ((11<<7)&0xFF)|(1<<3)=0x88 ;
				sinkAudio = MPEG4GenericRTPSink::createNew(*env, rtpGroupsockAudio,
						       96,
						       audioSamplingFrequency,
						       "audio", "AAC-hbr",
						       encoderConfigStr, audioNumChannels);

			}
		}

		// Create (and start) a 'RTCP instance' for this RTP sink:
		unsigned totalSessionBandwidthAudio = (audioOutputBitrate+500)/1000; // in kbps; for RTCP b/w share
		rtcpAudio = RTCPInstance::createNew(*env, rtcpGroupsockAudio,
					  totalSessionBandwidthAudio, CNAME,
					  sinkAudio, NULL /* we're a server */,
					  streamingMode == STREAMING_MULTICAST_SSM);
		// Note: This starts RTCP running automatically
		sms->addSubsession(PassiveServerMediaSubsession::createNew(*sinkAudio, rtcpAudio));

		// Start streaming:
		sinkAudio->startPlaying(*sourceAudio, NULL, NULL);
    }

	rtspServer->addServerMediaSession(sms);
	{
		struct in_addr dest; dest.s_addr = multicastAddress;
		char *url = rtspServer->rtspURL(sms);
		//char *url2 = inet_ntoa(dest);
		*env << "Mulicast Play this stream using the URL:\n\t" << url << "\n";
		//*env << "2 Mulicast addr:\n\t" << url2 << "\n";
		delete[] url;
	}
  }


  // Begin the LIVE555 event loop:
  env->taskScheduler().doEventLoop(&watchVariable); // does not return


  if( streamingMode!= STREAMING_UNICAST )
  {
	Medium::close(rtcpAudio);
	Medium::close(sinkAudio);
	Medium::close(sourceAudio);
	delete rtpGroupsockAudio;
	delete rtcpGroupsockAudio;

	Medium::close(rtcpVideo);
	Medium::close(sinkVideo);
	Medium::close(sourceVideo);
	delete rtpGroupsockVideo;
	delete rtcpGroupsockVideo;

  }

  Medium::close(rtspServer); // will also reclaim "sms" and its "ServerMediaSubsession"s
  if( MjpegInputDevice != NULL )
  {
	Medium::close(MjpegInputDevice);
  }

  if( H264InputDevice != NULL )
  {
	Medium::close(H264InputDevice);
  }

  if( Mpeg4InputDevice != NULL )
  {
	Medium::close(Mpeg4InputDevice);
  }

  env->reclaim();

  delete scheduler;

  ApproInterfaceExit();

  return 0; // only to prevent compiler warning

}
