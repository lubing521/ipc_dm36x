#ifndef _AV_SERVER_API_H_
#define _AV_SERVER_API_H_

#include <avserver_config.h>

int AVSERVER_init();
int AVSERVER_exit();

int AVSERVER_start(AVSERVER_Config *config);
int AVSERVER_stop();

int AUDIO_GetSampleRate(void);

int AVSERVER_ldcEnable(int streamId, Bool enable);
int AVSERVER_snfEnable(int streamId, Bool enable);
int AVSERVER_tnfEnable(int streamId, Bool enable);
int AVSERVER_vsEnable(int streamId, Bool enable);
int AVSERVER_swosdEnable(int streamId, Bool enable);
int AVSERVER_fileSaveEnable(int streamId, Bool enable);
int AVSERVER_encryptEnable(int streamId, Bool enable);
int AVSERVER_faceDetectEnable(Bool enable);
int AVSERVER_pMaskEnable(Bool enable);
int AVSERVER_faceRegUsrEnable(Bool enable);
int AVSERVER_faceRegUsrDelete(Bool enable);
int AVSERVER_faceRecogIdentifyEnable(Bool enable);
int AVSERVER_faceRecogClear(void);
int AVSERVER_faceTrackerEnable(Bool enable);
int AVSERVER_winmodeEnable(Bool enable);
int AVSERVER_aewbEnable(Bool enable);
int AVSERVER_afEnable(Bool enable);
int AVSERVER_aewbSetType(int type);
int AVSERVER_aewbPriority(int value);
int AVSERVER_SetDisplay(int type);

int AVSERVER_setAACBitrate(int bitrate);
/////zeng add 
int AVSERVER_setEncRcType(int streamId, int RcType);

int AVSERVER_setEncBitrate(int streamId, int bitrate);
int AVSERVER_setEncFramerate(int streamId, unsigned int fps);
int AVSERVER_getCaptureFrameRate(int encodeId, float *fr);

int AVSERVER_setMotion(int MotioneEnable, int MotioneCEnable, int MotioneCValue, int MotioneLevel, int block);
int AVSERVER_lock();
int AVSERVER_unlock();

int AVSERVER_profileInfoShow();
int AVSERVER_swosdText(char *strText, int nLength);
int AVSERVER_swosdEnableText(int enable);
int AVSERVER_swosdEnableDateTimeStamp(int enable);
int AVSERVER_swosdEnableLogo(int enable);
int AVSERVER_histEnable(int enable);
int AVSERVER_saldreEnable(int enable);
int AVSERVER_saldreLevel(int level);
int AVSERVER_saldreMode(int mode);
int AVSERVER_snapLocation(int enable);
int AVSERVER_snapName(char *strText, int nLength);
int AVSERVER_setMirrValue(int value);
int AVSERVER_fdROIEnable(Bool enable);
int AVSERVER_fxdROIEnable(Bool enable);
int AVSERVER_audioAlarmLevel(int value);
int AVSERVER_audioAlarmEnable(int enable);

#endif  /*  _AV_SERVER_H_  */

