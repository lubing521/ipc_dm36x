#ifndef _AV_SERVER_CONFIG_H_
#define _AV_SERVER_CONFIG_H_

#include <osa.h>

#define AVSERVER_MAX_STREAMS 		(4)
#define AVSERVER_MAX_FRAMERATE 		(30)

#define AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN        (0)
#define AVSERVER_CAPTURE_RAW_IN_MODE_DDR_IN         (1)

#define MAX_STRING_LENGTH 24

typedef struct {

  Uint32 samplingRate;
  Uint32 bitRate;
  Uint16 codecType;
  Bool   audioUpdate;

  Bool   audioAlarm;
  Uint16 alarmLevel;

  Bool   fileSaveEnable;

} AUDIO_Config;

typedef struct {

  Uint16 captureStreamId;
  Bool   fdEnable;
  Bool   fdTracker;
  Bool   frIdentify;         /* ANR - FR */
  Bool   frRegUsr;
  Bool   frDelUsr;
  Bool 	 privacyMaskEnable;
  Bool   fdROIEnable;

  Uint16 startX;
  Uint16 startY;
  Uint16 width;
  Uint16 height;
  Uint16 fdconflevel;
  Uint16 fddirection;
  Uint16 frconflevel;
  Uint16 frdatabase;
  Uint16 maskoption;

} VIDEO_FaceDetectConfig;

typedef struct {

  Uint16 captureStreamId;
  Uint16 width;
  Uint16 height;
  Uint16 expandH;
  Uint16 expandV;

} VIDEO_DisplayConfig;

typedef enum{                       //Defined by Rajiv:  SWOSD Test

SWOSD_BASIC = 0,
SWOSD_DETAIL,
SWOSD_COMPLETE

}SWOSD_Type ;

typedef enum{
	SWOSD_FMT_TOP_LEFT = 0,
	SWOSD_FMT_TOP_RIGHT,
	SWOSD_FMT_TOP_NONE
}SWOSD_TOP_POS_FMT;

typedef enum{
	SWOSD_FMT_BOTTOM_LEFT = 0,
	SWOSD_FMT_BOTTOM_RIGHT,
	SWOSD_FMT_BOTTOM_NONE
}SWOSD_BOTTOM_POS_FMT;

typedef enum{
	SWOSD_FMT_YMD = 0,
	SWOSD_FMT_MDY,
	SWOSD_FMT_DMY
}SWOSD_DATE_FMT;

typedef enum{
	SWOSD_FMT_12HR = 0,
	SWOSD_FMT_24HR
}SWOSD_TIME_FMT;

typedef enum{
	AEWB_NONE = 0,
	AEWB_APPRO,
	AEWB_TI
}AEWB_VENDOR;

typedef enum{
	AEWB_OFF = 0,
	AEWB_AE,
	AEWB_AWB,
	AEWB_AEWB
}AEWB_TYPE;

typedef enum{
	AEWB_FRAMERATE_PRIORITY = 0,
	AEWB_FRAMERATE_EXPOSURE
}AEWB_PRIORITY;

typedef enum{
	FLICKER_NTSC = 0,
	FLICKER_PAL,
	FLICKER_NONE
}FLICKER_TYPE;

typedef enum{
	SALDRE_LOW = 0,
	SALDRE_MED,
	SALDRE_HIGH
}SALDRE_LEVEL;

typedef enum{
	SALDRE_OFF = 0,
	SALDRE_GLOBAL,
	SALDRE_LOCAL
}SALDRE_MODE;

typedef struct{

	Bool swosdEnable;
	Bool swosdLogoStringUpdate;
	SWOSD_Type  swosdType;
	Bool swosdDateEnable;
	char swosdDateFormat;
	char swosdDatePos;
	Bool swosdTimeEnable;
	char swosdTimeFormat;
	char swosdTimePos;
	Bool swosdDateTimeUpdate;
	Bool swosdTextEnable;
	char swosdTextPos;
	char swosdDispString[MAX_STRING_LENGTH];
	Bool swosdLogoEnable;
	char swosdLogoPos;
	Bool swosdDetailEnable;
	Bool swosdHistEnable;
	Bool swosdFREnable[8];   //ANR - FR
} VIDEO_SWOSDConfig;   //Defined by Rajiv:  SWOSD Test

typedef struct {

  Uint16 width;
  Uint16 height;
  Bool   ldcEnable;
  Bool   snfEnable;
  Bool   tnfEnable;
  Bool   vsEnable;

  Bool   flipH;
  Bool   flipV;
  Bool   mirrUpdate;

  int numEncodes;
  int encodeStreamId[AVSERVER_MAX_STREAMS];

  Uint32 frameSkipMask;

  VIDEO_SWOSDConfig  swosdConfig;       //Defined by Rajiv:  SWOSD Test

} VIDEO_CaptureConfig;

typedef struct {
	Uint16 startx;
	Uint16 starty;
	Uint16 width;
	Uint16 height;
} REGION_roi;

typedef struct {

  Uint16 captureStreamId;
  Uint16 cropWidth;
  Uint16 cropHeight;
  Uint32 frameRateBase;
  Uint32 frameSkipMask;
  Uint16 codecType;
  Uint32 codecBitrate;
  Bool   encryptEnable;
  Bool   fileSaveEnable;
  Bool   motionVectorOutputEnable;
  Int16  qValue;
  Uint16 rateControlType;

  ///////zeng add
  Uint16 profileIdc; ////100=HighProfile 77=main 66=baseline

  Int16  	 numROI;
  REGION_roi prmROI[AVSERVER_MAX_STREAMS];

  Uint16 newCodecPrm;
  Uint16 ipRatio;
  Uint16 fIframe;
  Uint16 qpInit;
  Uint16 qpMin;
  Uint16 qpMax;
  Uint16 encodePreset;
  Uint16 packetSize;
  Uint16 resetCodec;

} VIDEO_EncodeConfig;

typedef struct {

  int mem_layou_set;

} STREAM_Config;

typedef struct {
  Uint16 snapLocation;
  Uint16 snapEnable;
  char snapName[MAX_STRING_LENGTH];
} SNAP_config;

typedef struct {
  Bool   aewbEnable;
  Bool   aewbBinEnable;
  Bool   afEnable;
  Bool   saldreEnable;
  Bool   saldreState;
  int    saldreMode;
  int    saldreLevel;
  int    aewbVendor;
  int    aewbType;
  int    aewbPriority;
  int    autoFocusVal;
  int    reduceShutter;
  int    flickerType;
} AEWB_config;

typedef struct {

  Uint16 sensorMode;
  Uint16 sensorFps;

  Bool   audioTskEnable;
  Bool   vstabTskEnable;
  Bool   ldcTskEnable;
  Bool   vnfTskEnable;
  Bool   encryptTskEnable;
  Bool   displayEnable;
  Bool   alarmEnable;

  Bool   winmodeEnable;
  Bool	 fxdROIEnable;
  Bool	 vnfDemoEnable;

  Bool   captureSingleResize;
  Uint16 captureRawInMode;
  Uint16 resizerClkdivN;
  Uint16 captureYuvFormat;
  Uint16 numCaptureStream;
  Uint16 numEncodeStream;

  VIDEO_CaptureConfig     captureConfig[AVSERVER_MAX_STREAMS];
  VIDEO_EncodeConfig      encodeConfig[AVSERVER_MAX_STREAMS];
  VIDEO_FaceDetectConfig  faceDetectConfig;
  VIDEO_DisplayConfig     displayConfig;
  AUDIO_Config            audioConfig;
  STREAM_Config			  streamConfig;
  SNAP_config			  snap_config;
  AEWB_config			  aewb_config;

} AVSERVER_Config;

#endif  /*  _AV_SERVER_CONFIG_H_  */

