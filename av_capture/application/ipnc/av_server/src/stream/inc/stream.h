/** ===========================================================================
* @file stream.h
*
* @path $(IPNCPATH)\multimedia\encode_stream\stream
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */

#ifndef _STREAM_H_
#define _STREAM_H_


#include <rendezvous.h>
#include <mem_mng.h>
#include <Msg_Def.h>
#include <cache_mng.h>
#include <gop_lock.h>
#include <semaphore_util.h>

enum{
	STREAM_FAILURE   = -1,
	STREAM_SUCCESS   = 0
};

enum{
	STREAM_MP4 = 0,
	STREAM_MJPG ,
	STREAM_MP4_EXT ,
	STREAM_AUDIO ,
	STREAM_H264_1,
	STREAM_H264_2,
};

enum{
	STREAM_SEM_MPEG4 = 0,
	STREAM_SEM_MPEG4_EXT,
	STREAM_SEM_JPEG,
	STREAM_SEM_AUDIO,
	STREAM_SEM_MEMCPY,
	STREAM_SEM_CACHECPY,
	STREAM_SEM_GOP,
	STREAM_SEM_NUM
};

enum{
	STREAM_FEATURE_BIT_RATE1 = 0,
	STREAM_FEATURE_BIT_RATE2,
	STREAM_FEATURE_JPG_QUALITY,
	STREAM_FEATURE_AAC_BITRATE,
	STREAM_FEATURE_FRAMERATE1,
	STREAM_FEATURE_FRAMERATE2,
	STREAM_FEATURE_FRAMERATE3,
	STREAM_FEATURE_BLC,
	STREAM_FEATURE_SATURATION,
	STREAM_FEATURE_AWB_MODE,
	STREAM_FEATURE_AE_MODE,
	STREAM_FEATURE_BRIGHTNESS,
	STREAM_FEATURE_CONTRAST,
	STREAM_FEATURE_SHARPNESS,
	STREAM_FEATURE_AEW_TYPE,
	STREAM_FEATURE_AEW_PRIORITY,
	STREAM_FEATURE_ENV_50_60HZ,
	STREAM_FEATURE_BINNING_SKIP,
	STREAM_FEATURE_LOCALDISPLAY,
	STREAM_FEATURE_TSTAMPENABLE,
	STREAM_FEATURE_PTZ,
	STREAM_FEATURE_MOTION,
	STREAM_FEATURE_ROICFG,
	STREAM_FEATURE_OSDTEXT_EN,
	STREAM_FEATURE_HIST_EN,
	STREAM_FEATURE_OSDLOGO_EN,
	STREAM_FEATURE_OSDTEXT,
	STREAM_FEATURE_FACE_SETUP,
	STREAM_FEATURE_DATETIMEPRM,
	STREAM_FEATURE_OSDPRM1,
	STREAM_FEATURE_OSDPRM2,
	STREAM_FEATURE_OSDPRM3,
	STREAM_FEATURE_CLICKNAME,
	STREAM_FEATURE_CLICKSTORAGE,
	STREAM_FEATURE_ROIPRM1,
	STREAM_FEATURE_ROIPRM2,
	STREAM_FEATURE_ROIPRM3,
	STREAM_FEATURE_ADVCODECPRM1,
	STREAM_FEATURE_ADVCODECPRM2,
	STREAM_FEATURE_ADVCODECPRM3,
	STREAM_FEATURE_OSDENABLE,
	STREAM_FEATURE_IFRAME,
	STREAM_FEATURE_MIRROR,
	STREAM_FEATURE_VNFSTATUS1,
	STREAM_FEATURE_VNFSTATUS2,
	STREAM_FEATURE_VNFSTATUS3,
	STREAM_FEATURE_AUDIO_ALARM_LEVEL,
	STREAM_FEATURE_AUDIO_ALARM_FLAG,
	STREAM_FEATURE_DYNRANGE,
	STREAM_FEATURE_NUM
};

enum{
	STREAM_EXT_MP4_CIF=0,
	STREAM_EXT_JPG,
	STREAM_EXT_NUM
};

typedef struct _STREAM_SET{
	int				ImageWidth;
	int				ImageHeight;
	int				ImageWidth_Ext[STREAM_EXT_NUM];
	int				ImageHeight_Ext[STREAM_EXT_NUM];
	int				JpgQuality;
	int				Mpeg4Quality;
	int				Mem_layout;
}	STREAM_SET;


typedef struct _STREAM_PARM{
	MEM_MNG_INFO 	MemInfo;
	int 			MemMngSemId[STREAM_SEM_NUM];
	pthread_t 		threadControl;
	Rendezvous_Obj	objRv[GOP_INDEX_NUM];
	int 			checkNewFrame[GOP_INDEX_NUM];
	int 			lockNewFrame[GOP_INDEX_NUM];
	int				IsQuit;
	int				qid;
	int				ImageWidth;
	int				ImageHeight;
	int				ImageWidth_Ext[STREAM_EXT_NUM];
	int				ImageHeight_Ext[STREAM_EXT_NUM];
	int				JpgQuality;
	int				Mpeg4Quality;
}	STREAM_PARM;
#ifndef JSK_DEBUG
#define JSK_DEBUG(fmt, args...) printf ("FILE[%s] LINE[%d] "fmt"\n", __FILE__, __LINE__, ##args)
#endif

STREAM_PARM *stream_get_handle(void);
int stream_init( STREAM_PARM *pParm , STREAM_SET *pSet);
int stream_write(void *pAddr, int size, int frame_type ,int stream_type, unsigned int timestamp , unsigned int temporalId ,STREAM_PARM *pParm);
int stream_end(STREAM_PARM *pParm);

#endif
