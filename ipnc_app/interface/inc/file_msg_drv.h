/* ===========================================================================
* @path $(IPNCPATH)\include
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
/**
* @file file_msg_drv.h
* @brief File message driver
*/
#ifndef _FILE_MSG_DRV_H_
#define _FILE_MSG_DRV_H_
#include <Msg_Def.h>
#include <sys_env_type.h>
/**
* @defgroup FILE_MSG_DRV File message driver
* @brief APIs to control file manager.

* This is the communication interface of file manager. Every process needs a
*  message ID (which is defined in @ref File_Msg_Def.h) to communicate with
* file manager. More detail please refer to (@ref FILE_MSG_DRV_HOW).
* @{
*/
/* This function should be called at process innitial !!! */
int InitFileMsgDrv(int key,int iProcId);
void CleanupFileMsgDrv();
/* API */
void SendFileQuitCmd();
SysInfo *GetSysInfo();
int GetSysLog(int nPageNum, int nItemNum, LogEntry_t * pBuf);
int fSetNetMask(struct in_addr netmask);
int fSetIp(struct in_addr ip);
int fSetHttpPort(unsigned short port);
int fSetTitle(void *buf, int len);
int fSetRegUsr(void *buf, int len);
int fSetOSDText(void *buf, int len);
int fSetGateway(struct in_addr gateway);
int fSetDhcpEnable(int enable);
int fSetFtpFqdn(void *buf, int len);
int fSetFtpPort(unsigned short port);
int fSetFtpUsername(void *buf, int len);
int fSetFtpPassword(void *buf, int len);
int fSetFtpFoldname(void *buf, int len);
int fSetFtpImageacount(int imageacount);
int fSetFtpPid(int pid);
int fSetSmtpServerIp(void * buf, int len);
int fSetSmtpServerPort(unsigned short port);
int fSetSmtpAuthentication(unsigned char authentication);
int fSetSmtpUsername(void * buf, int len);
int fSetSmtpPassword(void * buf, int len);
int fSetSmtpSenderEmail(void * buf, int len);
int fSetSmtpReceiverEmail(void * buf, int len);
int fSetSmtpCC(void * buf, int len);
int fSetSmtpSubject(void * buf, int len);
int fSetSmtpText(void * buf, int len);
int fSetSmtpAttachments(unsigned char attachments);
int fSetSmtpView(unsigned char view);
int fSetDns(struct in_addr ip);
int fSetSntpServer(void *buf, int len);
int fSetTimeFormat(unsigned char value);
int fSetDaylightTime(unsigned char value);
int fSetTimeZone(unsigned char value);
int fSetCamDayNight(unsigned char value);
int fSetCamWhiteBalance(unsigned char value);
int fSetCamBacklight(unsigned char value);
int fSetCamBrightness(unsigned char value);
int fSetCamContrast(unsigned char value);
int fSetCamSaturation(unsigned char value);
int fSetCamSharpness(unsigned char value);
int fSetClearSysLog(unsigned char value);
int fSetClearAccessLog(unsigned char value);
int fSetJpegQuality(unsigned char value);
int fSetMirror(unsigned char value);
int fSetAdvMode(unsigned char value);
int fSetFaceDetect(unsigned char value);
int fSetDemoCfg(unsigned char value);
int fSetOSDWin(unsigned char value);
int fSetHistogram(unsigned char value);
int fSetDynRange(unsigned char value);
int fSetVideoCodecMode(unsigned char value);
int fSetVideoCodecRes(unsigned char value);
int fSetImageFormat(unsigned char value);
int fSetResolution(unsigned char value);
int fSetMPEG4Res(unsigned char value);
int fSetMPEG42Res(unsigned char value);
int fSetSchedule(int index,Schedule_t* pSchedule, int day, int year);
int fSetLostAlarm(unsigned char value);
int fSetSDAlarmEnable(unsigned char value);
int fSetFTPAlarmEnable(unsigned char value);
int fSetSMTPAlarmEnable(unsigned char value);
int fSetAlarmDuration(unsigned char value);
int fSetImageSource(unsigned char value);
int fSetBinning(unsigned char value);
int fSetBlc(unsigned char value);
int fSetMP41bitrate(int value);
int fSetMP42bitrate(int value);
int fSetEncFramerate(unsigned char stream, unsigned char value);
int fSetRateControl(unsigned char value);
int fSetAVIDuration(unsigned char value);
int fSetAVIFormat(unsigned char value);
int fSetFTPFileFormat(unsigned char value);
int fSetSDFileFormat(unsigned char value);
int fSetAttFileFormat(unsigned char value);
int fSetAudioEnable(unsigned char value);
int fSetASmtpAttach(unsigned char value);
int fSetRftpenable(unsigned char value);
int fSetSdReEnable(unsigned char value);
int fSetImageAEW(unsigned char value);
int fSetImageAEWType(unsigned char value);
int fSetVideoSize(unsigned char stream, int Xvalue, int Yvalue);
int fSetStreamActive(unsigned char stream1, unsigned char stream2, unsigned char stream3,
                     unsigned char stream4, unsigned char stream5, unsigned char stream6);
int fSetMotionEnable(unsigned char value);
int fSetMotionCEnable(unsigned char value);
int fSetMotionLevel(unsigned char value);
int fSetMotionCValue(unsigned char value);
int fSetMotionBlock(char *pValue,int len);
int fSetSDInsert(unsigned char value);
int fSetAcount(int index, Acount_t *acout);
int fSetIpncPtz(unsigned char value);
int fSetGIOInEnable(unsigned char value);
int fSetGIOInType(unsigned char value);
int fSetGIOOutEnable(unsigned char value);
int fSetGIOOutType(unsigned char value);
int fSetTStampEnable(unsigned char value);
int fSetTStampFormat(unsigned char value);
int fSetAudioInVolume(unsigned char value);
int fSetAudioOutVolume(unsigned char value);
int fResetSysInfo();
int fSetSysLog(LogEntry_t* value);
int fSetAlarmStatus(unsigned short value);
int fSetPtzSupport(unsigned char value);
int fSetNetMulticast(unsigned char value);
int fSetVideoCodecCombo(unsigned char value);
int fSetVideoMode(unsigned char value);
int fSetStreamConfig(unsigned char stream, int width, int height, char *name, int portnum);
int fSetLocalDisplay(unsigned char value);
int fSetFramerate1(unsigned char value);
int fSetFramerate2(unsigned char value);
int fSetFramerate3(unsigned char value);
int fSetFramerateVal1(unsigned int value);
int fSetFramerateVal2(unsigned int value);
int fSetFramerateVal3(unsigned int value);
int fSetMaxExposureValue(unsigned char value);
int fSetMaxGainValue(unsigned char value);
int fSetAlarmAudioPlay(unsigned char value);
int fSetAudioON(unsigned char value);
int fSetAudioMode(unsigned char value);
int fSetAudioEncode(unsigned char value);
int fSetAudioSampleRate(unsigned char value);
int fSetAudioBitrate(unsigned char value);
int fSetAudioBitrateValue(int value);
int fSetAudioAlarmLevel(unsigned char value);
int fSetFDX(unsigned int value);
int fSetFDY(unsigned int value);
int fSetFDW(unsigned int value);
int fSetFDH(unsigned int value);
int fSetFDConfLevel(unsigned char value);
int fSetFDDirection(unsigned char value);
int fSetFRecognition(unsigned char value);
int fSetFRConfLevel(unsigned char value);
int fSetFRDatabase(unsigned char value);
int fSetPrivacyMask(unsigned char value);
int fSetMaskOptions(unsigned char value);
int fSetClickSnapFilename(void *buf, int len);
int fSetClickSnapStorage(unsigned char value);
int fSetExpPriority(unsigned char value);
int fSetOSDWinNum(unsigned char value);
int fSetRateControl1(unsigned char value);
int fSetRateControl2(unsigned char value);
int fSetDateFormat(unsigned char value);
int fSetDatePosition(unsigned char value);
int fSetTimePosition(unsigned char value);
int fSetDateStampEnable1(unsigned char value);
int fSetDateStampEnable2(unsigned char value);
int fSetDateStampEnable3(unsigned char value);
int fSetTimeStampEnable1(unsigned char value);
int fSetTimeStampEnable2(unsigned char value);
int fSetTimeStampEnable3(unsigned char value);
int fSetLogoEnable1(unsigned char value);
int fSetLogoEnable2(unsigned char value);
int fSetLogoEnable3(unsigned char value);
int fSetLogoPosition1(unsigned char value);
int fSetLogoPosition2(unsigned char value);
int fSetLogoPosition3(unsigned char value);
int fSetTextPosition1(unsigned char value);
int fSetTextPosition2(unsigned char value);
int fSetTextPosition3(unsigned char value);
int fSetTextEnable1(unsigned char value);
int fSetTextEnable2(unsigned char value);
int fSetTextEnable3(unsigned char value);
int fSetOverlayText1(void *buf, int len);
int fSetOverlayText2(void *buf, int len);
int fSetOverlayText3(void *buf, int len);
int fSetDetailInfo1(unsigned char value);
int fSetDetailInfo2(unsigned char value);
int fSetDetailInfo3(unsigned char value);
int fSetEncryptVideo(unsigned char value);
int fSetROIEnable1(unsigned char value);
int fSetROIEnable2(unsigned char value);
int fSetROIEnable3(unsigned char value);
int fSetStr1X1(unsigned int value);
int fSetStr1Y1(unsigned int value);
int fSetStr1W1(unsigned int value);
int fSetStr1H1(unsigned int value);
int fSetStr1X2(unsigned int value);
int fSetStr1Y2(unsigned int value);
int fSetStr1W2(unsigned int value);
int fSetStr1H2(unsigned int value);
int fSetStr1X3(unsigned int value);
int fSetStr1Y3(unsigned int value);
int fSetStr1W3(unsigned int value);
int fSetStr1H3(unsigned int value);
int fSetStr2X1(unsigned int value);
int fSetStr2Y1(unsigned int value);
int fSetStr2W1(unsigned int value);
int fSetStr2H1(unsigned int value);
int fSetStr2X2(unsigned int value);
int fSetStr2Y2(unsigned int value);
int fSetStr2W2(unsigned int value);
int fSetStr2H2(unsigned int value);
int fSetStr2X3(unsigned int value);
int fSetStr2Y3(unsigned int value);
int fSetStr2W3(unsigned int value);
int fSetStr2H3(unsigned int value);
int fSetStr3X1(unsigned int value);
int fSetStr3Y1(unsigned int value);
int fSetStr3W1(unsigned int value);
int fSetStr3H1(unsigned int value);
int fSetStr3X2(unsigned int value);
int fSetStr3Y2(unsigned int value);
int fSetStr3W2(unsigned int value);
int fSetStr3H2(unsigned int value);
int fSetStr3X3(unsigned int value);
int fSetStr3Y3(unsigned int value);
int fSetStr3W3(unsigned int value);
int fSetStr3H3(unsigned int value);
int fSetQPMax1(unsigned char value);
int fSetQPMax2(unsigned char value);
int fSetQPMax3(unsigned char value);
int fSetMEConfig1(unsigned char value);
int fSetMEConfig2(unsigned char value);
int fSetMEConfig3(unsigned char value);
int fSetPacketSize1(unsigned char value);
int fSetPacketSize2(unsigned char value);
int fSetPacketSize3(unsigned char value);
int fSetIpratio1(unsigned int value);
int fSetIpratio2(unsigned int value);
int fSetIpratio3(unsigned int value);
int fSetForceIframe1(unsigned char value);
int fSetForceIframe2(unsigned char value);
int fSetForceIframe3(unsigned char value);
int fSetQPInit1(unsigned char value);
int fSetQPInit2(unsigned char value);
int fSetQPInit3(unsigned char value);
int fSetQPMin1(unsigned char value);
int fSetQPMin2(unsigned char value);
int fSetQPMin3(unsigned char value);
int fSetAlarmEnable(unsigned char value);
int fSetExtAlarm(unsigned char value);
int fSetAudioAlarm(unsigned char value);
int fSetAlarmAudioFile(unsigned char value);
int fSetScheduleRepeatEnable(unsigned char value);
int fSetScheduleNumWeeks(unsigned char value);
int fSetScheduleInfiniteEnable(unsigned char value);
int fSetAlarmStorage(unsigned char value);
int fSetRecordStorage(unsigned char value);
int fSetHttpsPort(unsigned short value);
int fSetRS485Port(unsigned short value);
int fSetChipConfig(unsigned char value);
void fResetCodecROIParam(int stream, int width, int height);
void fResetFDROIParam(int stream, int width, int height);
int fSetAudioReceiverEnable(unsigned char value);
int fSetAudioSeverIp(void *buf, int len);
int fSetLinearWdr(unsigned char value);
int fSetFlickerDetect(unsigned int value);
/** @} */
#endif /* _FILE_MSG_DRV_H_ */
/**
* @page FILE_MSG_DRV_HOW How to use file message driver?

* 1. Add new message ID in \ref File_Msg_Def.h if needed.\n
* 2. Call the API InitFileMsgDrv(int key,int iProcId) with the ID you added to initialize file
*  message driver.\n
* 3. Now you can use any file message driver functions as you want.\n
* 4. Call the API CleanupFileMsgDrv() to cleanup file message driver.
* @section FILE_MSG_DRV_HOW_EX Example
* @code
#include <file_msg_drv.h>
int main()
{
	SysInfo *pSysInfo;
	if( InitFileMsgDrv(FILE_MSG_KEY, FILE_SYS_MSG) != 0)
		return -1;
	pSysInfo = GetSysInfo();
	// more file message driver API
	CleanupFileMsgDrv();
	return 0;
}
* @endcode
* \b See \b also \ref SYS_MSG_DRV_HOW
*/
