#ifndef _AV_SERVER_H_
#define _AV_SERVER_H_

#include <avserver_debug.h>
#include <avserver_config.h>
#include <avserver_thr.h>
#include <video.h>
#include <audio.h>

#include <avserver_api.h>
#include <videoSwosd.h>


// from AVSERVER Main to other TSKs
#define AVSERVER_CMD_CREATE    (0x300)
#define AVSERVER_CMD_DELETE    (0x301)
#define AVSERVER_CMD_START     (0x302)
#define AVSERVER_CMD_NEW_DATA  (0x303)

// from UI thread to AVSERVER Main
#define AVSERVER_MAIN_CMD_START  (0x400)
#define AVSERVER_MAIN_CMD_STOP   (0x401)

// AVSERVER Main State's
#define AVSERVER_MAIN_STATE_IDLE       (0)
#define AVSERVER_MAIN_STATE_RUNNING    (1)

typedef enum{
	PLATFORM_NONE = -1,
	PLATFORM_DM365 = 0,
	PLATFORM_DM368
}PLATFORM_DM36X;

typedef struct {

  OSA_TskHndl   mainTsk;
  OSA_MbxHndl   uiMbx;
  OSA_MutexHndl lockMutex;

  OSA_PrfHndl capturePrf;
  OSA_PrfHndl ipipePrf;
  OSA_PrfHndl ldcPrf[4];
  OSA_PrfHndl resizePrf;
  OSA_PrfHndl vsPrf;
  OSA_PrfHndl fdPrf;
  OSA_PrfHndl fdtrkPrf;
  OSA_PrfHndl aewbPrf;
  OSA_PrfHndl afPrf;
  OSA_PrfHndl encodePrf[4];
  OSA_PrfHndl streamPrf;
  OSA_PrfHndl miscPrf[4];
  OSA_PrfHndl vnfPrf[4];
  OSA_PrfHndl swosdPrf[4];
  OSA_PrfHndl audioPrf;
  OSA_PrfHndl motionPrf;
  OSA_PrfHndl saldrePrf;

} AVSERVER_Ctrl;

//add by zeng
typedef struct _OSDBASE_{
	int		bTimeEn;
	int		bTextEn;
	int		bEnglish;
	char		szText[32];
	char		szTime[48];
}OSDBASE;

typedef struct _OSDPARAM11_{
	char	szTime[48*24];
	char	szText[32*24];
	int		iTimeLen;
	int		iTextLen;

	char	szTimeMask[48*24*8];
	char	szTextMask[32*24*8];

}OSDPARAM11;

typedef struct _OSDPARAM16_{
	char	szTime[48*32];
	char	szText[32*32];
	int		iTimeLen;
	int		iTextLen;

	char	szTimeMask[48*32*8];
	char	szTextMask[32*32*8];

}OSDPARAM16;

typedef struct _OSDPARAM32_{
	char	szTime[48*128];
	char	szText[32*128];
	int		iTimeLen;
	int		iTextLen;

	char	szTimeMask[48*32*32];
	char	szTextMask[32*32*32];

}OSDPARAM32;

typedef struct _AVSVR_AREA_{

	int		bMaskEn;
	int		iX0;
	int		iY0;
	int		iX1;
	int		iY1;
}AVSVR_AREA;

typedef struct _MASK_CFG_{
	AVSVR_AREA	rcMask[4];
}MASK_CFG;




extern AVSERVER_Ctrl gAVSERVER_ctrl;
extern AVSERVER_Config gAVSERVER_config;
extern VIDEO_Ctrl gVIDEO_ctrl;
extern AUDIO_Ctrl gAUDIO_ctrl;
extern AUDIO_Ctrl gAUDIOPLAY_ctrl;

OSA_BufInfo *AVSERVER_bufGetEmpty(int tskId, int streamId, int *bufId, int timeout);
OSA_BufInfo *AVSERVER_bufPutFull(int tskId, int streamId, int bufId);

OSA_BufInfo *AVSERVER_bufGetFull(int tskId, int streamId, int *bufId, int timeout);
OSA_BufInfo *AVSERVER_bufPutEmpty(int tskId, int streamId, int bufId);

Uint32 AVSERVER_getFrameSkipMask(int fps);

int AVSERVER_tskConnectInit();
int AVSERVER_tskConnectReset();
int AVSERVER_tskConnectExit();

int AVSERVER_bufAlloc();

int AVSERVER_mainCreate();
int AVSERVER_mainDelete();

int AVSERVER_SetOSDPixel(int iWidth, int iHeight, char* pVideoBuf0, char* pVideoBuf1);

int AVSERVER_SetOSD(OSDBASE* pOsdBase);

void AVSERVER_SetOsdEnable(int nType , int bEnable);
void AVSERVER_SetOsdTextStr(char* pStr ,int nLen);

int AVSERVER_SetVideoMask(int nNumber , int bEnable ,int nx0,int ny0,int nx1,int ny1);


#endif  /*  _AV_SERVER_H_  */

