/* ===========================================================================
* @path $(IPNCPATH)\sys_adm\system_server
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2009
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
/**
* @file av_server_ctrl.h
* @brief Functions about AV Server control.
*/
#ifndef __AV_SERVER_CTRL_H__
#define __AV_SERVER_CTRL_H__
#include <sys_env_type.h>
/**
* @brief Stream settings.
*/
typedef struct StreamEnv_t{
	unsigned char nDayNight;
	unsigned char nWhiteBalance;
	unsigned char nBackLight;
	unsigned char nBrightness;
	unsigned char nContrast;
	unsigned char nSaturation;
	unsigned char nSharpness;
	unsigned char nVideocodec;
	unsigned char nVideocodecmode;
	unsigned char nVideocodecres;
	unsigned char nBinning;
	unsigned char nJpegQuality;
	unsigned char nMirror;
	unsigned char AdvanceMode;
	unsigned char nFaceDetect;
	unsigned char nDemoCfg;
	unsigned char tStampEnable;
	unsigned char tStampFormat;
	unsigned char nAEWswitch;
	unsigned char nAEWtype;
	unsigned char expPriority;
	unsigned char nRateControl1;
	unsigned char nRateControl2;
	unsigned char nFrameRate1;
	unsigned char nFrameRate2;
	unsigned char nFrameRate3;
	unsigned char nMEConfig1;
	unsigned char nMEConfig2;
	unsigned char nMEConfig3;
	unsigned char imagesource;
	unsigned char nDisplay;
	unsigned char histogram;
	unsigned char dynRange;
	unsigned char audioenable;
	unsigned char audiocodectype;
	unsigned char audiobitrate;
	unsigned char audioinvolume;
	unsigned char audiooutvolume;
	unsigned char audioSampleRate;
	unsigned char audioalarm;
	unsigned char audioalarmlevel;
	int nBitrate1;
	int nBitrate2;
} StreamEnv_t;

extern const unsigned int enc_framerate_day_1_dm365[10][6][4];
extern const unsigned int enc_framerate_day_2_dm365[10][6][4];
extern const unsigned int enc_framerate_day_3_dm365[10][6][4];
extern const unsigned int enc_framerate_night_1_dm365[10][6][4];
extern const unsigned int enc_framerate_night_2_dm365[10][6][4];
extern const unsigned int enc_framerate_night_3_dm365[10][6][4];
extern const unsigned int enc_framerate_day_1_dm368[10][6][4];
extern const unsigned int enc_framerate_day_2_dm368[10][6][4];
extern const unsigned int enc_framerate_day_3_dm368[10][6][4];
extern const unsigned int enc_framerate_night_1_dm368[10][6][4];
extern const unsigned int enc_framerate_night_2_dm368[10][6][4];
extern const unsigned int enc_framerate_night_3_dm368[10][6][4];

int StartStream(StreamEnv_t* pConfig);
int SetBinning(unsigned char value);
int SetCamDayNight(unsigned char value);
int SetMP41Framerate(unsigned char value);
int SetMP42Framerate(unsigned char value);
int SetJPGFramerate(unsigned char value);
int SetCamWhiteBalance(unsigned char value);
int SetCamBacklight(unsigned char value);
int SetCamBrightness(unsigned char value);
int SetCamContrast(unsigned char value);
int SetCamSaturation(unsigned char value);
int SetCamSharpness(unsigned char value);
int SetClearSysLog(unsigned char value);
int SetClearAccessLog(unsigned char value);
int SetMJPEGQuality(unsigned char value);
int SetMirror(unsigned char value);
int SetAdvMode(unsigned int value);
int SetFaceDetect(unsigned char value);
int SetDemoCfg(unsigned char value);
int SetOSDStream(unsigned char value);
int SetOSDWinNum(unsigned char value);
int SetOSDWin(unsigned char value);
int SetOSDHistogram(unsigned char value);
int SetMP41bitrate(unsigned int value);
int SetMP42bitrate(unsigned int value);
int SetImageAEW(unsigned char value);
int SetImageAEWType(unsigned char value);
int SetImagesource(unsigned char value);
int SetTStampEnable(unsigned char value);
int SetTStampFormat(unsigned char value);
int SetFramerate1(unsigned char value);
int SetFramerate2(unsigned char value);
int SetFramerate3(unsigned char value);
int InitAV_Server(StreamEnv_t* pConfig);
int SetVstabValue(unsigned char value);
int SetLdcValue(unsigned char value);
int SetMaxExposureValue(unsigned char value);
int SetMaxGainValue(unsigned char value);
int SetSnfValue(unsigned char value);
int SetTnfValue(unsigned char value);
int SetAudioMode(unsigned char value);
int SetAudioEncode(unsigned char value);
int SetAudioSampleRate(unsigned char value);
int SetAudioBitrate(unsigned char value);
int SetRateControl1(unsigned char value);
int SetRateControl2(unsigned char value);
int SetDateStampEnable1(unsigned char value);
int SetDateStampEnable2(unsigned char value);
int SetDateStampEnable3(unsigned char value);
int SetExpPriority(unsigned char value);
int SetClickSnapStorage(unsigned char value);
int SetClickSnapFilename(void * buf, int length);
int SetTimeStampEnable1(unsigned char value);
int SetTimeStampEnable2(unsigned char value);
int SetTimeStampEnable3(unsigned char value);
int SetLogoEnable1(unsigned char value);
int SetLogoEnable2(unsigned char value);
int SetLogoEnable3(unsigned char value);
int SetLogoPosition1(unsigned char value);
int SetLogoPosition2(unsigned char value);
int SetLogoPosition3(unsigned char value);
int SetTextEnable1(unsigned char value);
int SetTextEnable2(unsigned char value);
int SetTextEnable3(unsigned char value);
int SetOverlayText1(void * buf, int length);
int SetOverlayText2(void * buf, int length);
int SetOverlayText3(void * buf, int length);
int SetTextPosition1(unsigned char value);
int SetTextPosition2(unsigned char value);
int SetTextPosition3(unsigned char value);
int SetLocalDisplay(unsigned char value);
int SetIpratio1(unsigned int value);
int SetIpratio2(unsigned int value);
int SetIpratio3(unsigned int value);
int SetForceIframe1(unsigned char value);
int SetForceIframe2(unsigned char value);
int SetForceIframe3(unsigned char value);
int SetQPInit1(unsigned char value);
int SetQPInit2(unsigned char value);
int SetQPInit3(unsigned char value);
int SetQPMin1(unsigned char value);
int SetQPMin2(unsigned char value);
int SetQPMin3(unsigned char value);
int SetQPMax1(unsigned char value);
int SetQPMax2(unsigned char value);
int SetQPMax3(unsigned char value);
int SetMEConfig1(unsigned char value);
int SetMEConfig2(unsigned char value);
int SetMEConfig3(unsigned char value);
int SetPacketSize1(unsigned char value);
int SetPacketSize2(unsigned char value);
int SetPacketSize3(unsigned char value);
int SetROIEnable1(unsigned char value);
int SetROIEnable2(unsigned char value);
int SetROIEnable3(unsigned char value);
int SetStr1X1(unsigned int value);
int SetStr1Y1(unsigned int value);
int SetStr1W1(unsigned int value);
int SetStr1H1(unsigned int value);
int SetStr1X2(unsigned int value);
int SetStr1Y2(unsigned int value);
int SetStr1W2(unsigned int value);
int SetStr1H2(unsigned int value);
int SetStr1X3(unsigned int value);
int SetStr1Y3(unsigned int value);
int SetStr1W3(unsigned int value);
int SetStr1H3(unsigned int value);
int SetStr2X1(unsigned int value);
int SetStr2Y1(unsigned int value);
int SetStr2W1(unsigned int value);
int SetStr2H1(unsigned int value);
int SetStr2X2(unsigned int value);
int SetStr2Y2(unsigned int value);
int SetStr2W2(unsigned int value);
int SetStr2H2(unsigned int value);
int SetStr2X3(unsigned int value);
int SetStr2Y3(unsigned int value);
int SetStr2W3(unsigned int value);
int SetStr2H3(unsigned int value);
int SetStr3X1(unsigned int value);
int SetStr3Y1(unsigned int value);
int SetStr3W1(unsigned int value);
int SetStr3H1(unsigned int value);
int SetStr3X2(unsigned int value);
int SetStr3Y2(unsigned int value);
int SetStr3W2(unsigned int value);
int SetStr3H2(unsigned int value);
int SetStr3X3(unsigned int value);
int SetStr3Y3(unsigned int value);
int SetStr3W3(unsigned int value);
int SetStr3H3(unsigned int value);
int SetDetailInfo1(unsigned char value);
int SetDetailInfo2(unsigned char value);
int SetDetailInfo3(unsigned char value);
int SetHistogram(unsigned char value);
int SetDynRange(unsigned char value);
int SetAudioON(unsigned char value);
int SetFDX(unsigned int value);
int SetFDY(unsigned int value);
int SetFDW(unsigned int value);
int SetFDH(unsigned int value);
int SetFDConfLevel(unsigned char value);
int SetFDDirection(unsigned char value);
int SetFRecognition(unsigned char value);
int SetFRConfLevel(unsigned char value);
int SetFRDatabase(unsigned char value);
int SetPrivacyMask(unsigned char value);
int SetMaskOptions(unsigned char value);
int SetDateFormat(unsigned char value);
int SetDatePosition(unsigned char value);
int SetTimePosition(unsigned char value);
int SetAudioAlarmLevel(unsigned char value);
int SetEncryptVideo(unsigned char value);
void SetFaceDetectPrm(void);
void CheckAudioParam(void);
void SetAudioParams(void);
int SetOSDDetail(int id);
int SetTimeDateDetail(void);
void ResetBasicDefault(void);
void SetCodecROIParam(int id);
void SetCodecAdvParam(int id);
int SetAudioReceiverEnable(unsigned char value);
int SetAudioSeverIp(void * buf, int length);
int SetLinearWdr(unsigned char value);
int SetFlickerDetect(int value);

#endif /*__AV_SERVER_CTRL_H__*/
