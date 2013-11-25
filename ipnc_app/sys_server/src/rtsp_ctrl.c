/* ===========================================================================
* @file rtsp_ctrl.c
*
* @path $(IPNCPATH)\sys_server\src\
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2010
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <asm/types.h>
#include <sem_util.h>
#include <file_msg_drv.h>
#include <rtsp_ctrl.h>

#ifndef __RTSP_DEBUG__
#define R_DB(fmt, args...)
#else
#define R_DB(fmt, args...)	fprintf(stderr, fmt, ##args)
#endif

static SemHandl_t hndlRTSP_Sem = NULL;
static int gRTSP_Mode;

int RTSP_ControlInit()
{
	if((hndlRTSP_Sem = MakeSem()) == NULL){
		return -1;
	}
	SemWait(hndlRTSP_Sem);
	gRTSP_Mode = 0;
	R_DB("gRTSP_Mode = %d\n", gRTSP_Mode);
	SemRelease(hndlRTSP_Sem);
	return 0;
}

static int TurnOffRTSPStreaming()
{
	system("killall -2 wis-streamer\n");
	sleep(2);
	system("killall -9 wis-streamer\n");
	sleep(2);
	gRTSP_Mode = 0;
	return 0;
}

int GetRTSPStreamOption(void)
{
	int nExtFunc = 0;
	SysInfo *pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	if(pSysInfo->audio_config.audioenable == 0)
		nExtFunc |= RTSP_MUTE;

	if(pSysInfo->audio_config.codectype != 0)
		nExtFunc |= RTSP_AAC;

	if(pSysInfo->codec_advconfig[0].meConfig>>1)
		nExtFunc |= RTSP_SVC;

	if(pSysInfo->lan_config.net.multicast_enable)
		nExtFunc |= RTSP_MULTICAST;

	return nExtFunc;
}

int SetRTSPStreamingON(int nExtFunc)
{
	char opt[32] = "./wis-streamer";
	int ret = 0;

	if(((int)(~RTSP_FUNC_MASK)) & nExtFunc){
		fprintf(stderr,"ERROR: Invalid parameter to Control RTSP!!!\n");
		return -1;
	}
	SemWait(hndlRTSP_Sem);
	if(gRTSP_Mode & RTSP_RUN){
		if((gRTSP_Mode & RTSP_FUNC_MASK) == nExtFunc){
			/* run in the same mode */
			SemRelease(hndlRTSP_Sem);
			return 0;
		}else {
			/* run in other mode */
			ret = TurnOffRTSPStreaming();
		}
	}
	if(RTSP_MUTE & nExtFunc)
		strcat(opt, " -s");

	if(RTSP_AAC & nExtFunc)
		strcat(opt, " -a");

	if(RTSP_SVC & nExtFunc)
		strcat(opt, " -v");

	if(RTSP_MULTICAST & nExtFunc)
		strcat(opt, " -m");

	strcat(opt, "&\n");

	//printf(opt);
	if(ret == 0)
		ret = system(opt);
	if(ret == 0){
		gRTSP_Mode = RTSP_RUN | nExtFunc;
		sleep(2);
	}
	R_DB("gRTSP_Mode = %d\n", gRTSP_Mode);
	SemRelease(hndlRTSP_Sem);
	return ret;
}

int SetRTSPStreamingOFF(void)
{
	SemWait(hndlRTSP_Sem);
	if(gRTSP_Mode == 0){
		SemRelease(hndlRTSP_Sem);
		return 0;
	}
	TurnOffRTSPStreaming();
	R_DB("gRTSP_Mode = %d\n", gRTSP_Mode);
	SemRelease(hndlRTSP_Sem);
	return 0;
}

int RTSP_ControlExit()
{
	SetRTSPStreamingOFF();
	if(DestroySem(hndlRTSP_Sem) != 0){
		return -1;
	}
	hndlRTSP_Sem = NULL;
	return 0;
}

int RTSP_AudioRecvON(void)
{
	char opt[128];
	SysInfo *pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

    if(pSysInfo->audio_config.audiorecvenable) {
		sprintf(opt,"./audio-receiver rtsp://%s &\n", pSysInfo->audio_config.audioServerIp);
		fprintf(stderr,"%s",opt);
		system(opt);
	}

	return 0;
}
