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
* @file sys_msg_drv.h
* @brief System message driver
*/
#ifndef _SYS_MSG_DRV_H_
#define _SYS_MSG_DRV_H_
#include <Msg_Def.h>
/**
* @defgroup SYS_MSG_DRV System message driver.
* @brief APIs to control system server.

* This is the communication interface to control system server. The interface
* is implemented by Linux POSIX queue and share memory. So it will pause you
*  thread until command result return.
* @note Call SysDrvInit() at initial, and call SysDrvExit() at the end of
 process.

* @see FILE_MSG_DRV
 * @{
*/
void SetSysProcId(int iProcId);
void SaveSysMsgId(int qid);
/* This function should be called at process initial !!! */
int SysDrvInit(int proc_id);
int SysDrvExit();
/* API */
void SendSysQuitCmd();
int SetSysCommon(void * data, unsigned int len, int cmd);
int GetDhcpEn(void *data, unsigned int len);
int GetHttpPort(void *data, unsigned int len);
int GetSntpFqdn(void *data, unsigned int len);
int GetDeviceType(void *data, unsigned int len);
int SetSysTimeFormat(void *data, unsigned int len);
int SetSysDaylight(void *data, unsigned int len);
int SetSysTimeZone(void *data, unsigned int len);
int GetTitle(void *data, unsigned int len);
int GetRegUsr(void *data, unsigned int len);
int GetOSDText(void *data, unsigned int len);
int GetMAC(void *data, unsigned int len);
int GetIP(void *data, unsigned int len);
int GetGateway(void *data, unsigned int len);
int GetNetmask(void *data, unsigned int len);
int GetFtpServerIp(void *data, unsigned int len);
int GetFtpUsername(void *data, unsigned int len);
int GetFtpPassword(void *data, unsigned int len);
int GetFtpFoldername(void *data, unsigned int len);
int GetFtpImageAmount(void *data, unsigned int len);
int GetFtpPid(void *data, unsigned int len);
int GetSysFtpPort(void *data, unsigned int len);
int GetSmtpServerIp(void *data, unsigned int len);
int GetSmtpServerPort(void *data, unsigned int len);
int GetSmtpAuthentication(void *data, unsigned int len);
int GetsmtpUsername(void *data, unsigned int len);
int GetSmtpPassword(void *data, unsigned int len);
int GetSmtpSenderEmail(void *data, unsigned int len);
int GetSmtpReceiverEmail(void *data, unsigned int len);
int SysGetSdFileName(void *data, unsigned int len);
int GetSmtpCC(void *data, unsigned int len);
int GetSmtpSubject(void *data, unsigned int len);
int GetSmtpText(void *data, unsigned int len);
int GetSmtpAttachments(void *data, unsigned int len);
int GetSmtpView(void *data, unsigned int len);
int GetSysDhcpConfig(void *data, unsigned int len);
int GetSysUpnpPort(void *data, unsigned int len);
int GetDns(void *data, unsigned int len);
int SetNetMask(void *data, unsigned int len);
int SetIp(void *data, unsigned int len);
int SetHttpPort(void *data, unsigned int len);
int SetTitle(void *data, unsigned int len);
int SetRegUsr(void *data, unsigned int len);
int SetOSDText(void *data, unsigned int len);
int SetGateway(void *data, unsigned int len);
int SetDhcpEnable(void *data, unsigned int len);
int SetSysFtpFqdn(void *data, unsigned int len);
int SetFtpUsername(void *data, unsigned int len);
int SetFtpPassword(void *data, unsigned int len);
int SetFtpFoldname(void *data, unsigned int len);
int SetFtpImageacount(void *data, unsigned int len);
int SetFtpPid(void *data, unsigned int len);
int SetSysFtpPort(void *data, unsigned int len);
int SetSmtpServerIp(void *data, unsigned int len);
int SetSmtpServerPort(void *data, unsigned int len);
int SetSmtpAuthentication(void *data, unsigned int len);
int SetSmtpUsername(void *data, unsigned int len);
int SetSmtpPassword(void *data, unsigned int len);
int SetSmtpSenderEmail(void *data, unsigned int len);
int SetSmtpReceiverEmail(void *data, unsigned int len);
int SetSmtpCC(void *data, unsigned int len);
int SetSmtpSubject(void *data, unsigned int len);
int SetSmtpText(void *data, unsigned int len);
int SetSmtpAttachments(void *data, unsigned int len);
int SetSmtpView(void *data, unsigned int len);
int SetDns(void *data, unsigned int len);
int SetSntpServer(void *data, unsigned int len);
int SetSysDayNight(void *data, unsigned int len);
int SetSysWhiteBalance(void *data, unsigned int len);
int SetSysBacklight(void *data, unsigned int len);
int SetSysBrightness(void *data, unsigned int len);
int SetSysContrast(void *data, unsigned int len);
int SetSysSaturation(void *data, unsigned int len);
int SetSysSharpness(void *data, unsigned int len);
int SetSysClearSysLog(void *data, unsigned int len);
int SetSysClearAccessLog(void *data, unsigned int len);
int SetSysJpegQuality(void *data, unsigned int len);
int SetSysMirror(void *data, unsigned int len);
int SetSysAdvMode(void *data, unsigned int len);
int SetSysDemoCfg(void *data, unsigned int len);
int SetSysOSDWin(void *data, unsigned int len);
int SetSysHistogram(void *data, unsigned int len);
int SetSysDynRange(void *data, unsigned int len);
int SetSysOSDWinNum(void *data, unsigned int len);
int SetSysOSDStream(void *data, unsigned int len);
int SetSysVideoCodecRes(void *data, unsigned int len);
int SetSysImageSource(void *data, unsigned int len);
int SetSysVideoMode(void *data, unsigned int len);
int SetSysIMGFormat(void *data, unsigned int len);
int SetSysResolution(void *data, unsigned int len);
int SetSysMPEG4Res(void *data, unsigned int len);
int SetSysMPEG42Res(void *data, unsigned int len);
int SetSysSchedule(void *data, unsigned int len);
int DelSysSchedule(void *data, unsigned int len);
int SetSysSDAlarmEnable(void *data, unsigned int len);
int SetSysFtpAlarmEnable(void *data, unsigned int len);
int SetSysSmtpAlarmEnable(void *data, unsigned int len);
int SetSysAlarmDuration(void *data, unsigned int len);
int SetAVIDuration(void *data, unsigned int len);
int SetAVIFormat(void *data, unsigned int len);
int SetFTPFileFormat(void *data, unsigned int len);
int SetSDFileFormat(void *data, unsigned int len);
int SetAttFileFormat(void *data, unsigned int len);
int SetAudioEnable(void *data, unsigned int len);
int SetASmtpAttach(void *data, unsigned int len);
int SetSysLostAlarm(void *data, unsigned int len);
int SetRftpEnable(void *data, unsigned int len);
int SetSdReEnable(void *data, unsigned int len);
int SetSysImage2A(void *data, unsigned int len);
int SetSysImage2AType(void *data, unsigned int len);
int SetSysBinning(void *data, unsigned int len);
int SetSysBLC(void *data, unsigned int len);
int SetMotionEnable(void *data, unsigned int len);
int SetMotionCEnable(void *data, unsigned int len);
int SetMotionLevel(void *data, unsigned int len);
int SetMotionCValue(void *data, unsigned int len);
int SetMotionBlock(void *data, unsigned int len);
int SetMPEG41Bitrate(void *data, unsigned int len);
int SetMPEG42Bitrate(void *data, unsigned int len);
int SetImageDefault(void *data, unsigned int len);
int DoSysLogin(void *data, unsigned int len);
int GetSysUserAuthority(void *data, unsigned int len);
int SysAddUser(void *data, unsigned int len);
int SysDelUser(void *data, unsigned int len);
int SetGIOInEnable(void *data, unsigned int len);
int SetGIOInType(void *data, unsigned int len);
int SetGIOOutEnable(void *data, unsigned int len);
int SetGIOOutType(void *data, unsigned int len);
int SetIpncPtz(void *data, unsigned int len);
int InitIpncPtz(void *data, unsigned int len);
int SetTStampEnable(void *data, unsigned int len);
int SetTStampFormat(void *data, unsigned int len);
int SetAudioinVolume(void *data, unsigned int len);
int SetSysRateControl(void *data, unsigned int len);
int SetMPEG41Framerate(void *data, unsigned int len);
int SetMPEG42Framerate(void *data, unsigned int len);
int SetJPEGFramerate(void *data, unsigned int len);
int SysSdFormat(void *data, unsigned int len);
int SysSdUnmount(void *data, unsigned int len);
int SysScheduleStop(void *data, unsigned int len);
int SysScheduleResume(void *data, unsigned int len);
int SetSysfdetect(void * data, unsigned int len);
int SetSysfdx(void * data, unsigned int len);
int SetSysfdy(void * data, unsigned int len);
int SetSysfdw(void * data, unsigned int len);
int SetSysfdh(void * data, unsigned int len);
int SetSysfdconf(void * data, unsigned int len);
int SetSysfddir(void * data, unsigned int len);
int SetSysfrecog(void * data, unsigned int len);
int SetSysfrconf(void * data, unsigned int len);
int SetSysfrdbase(void * data, unsigned int len);
int SetSyspmask(void * data, unsigned int len);
int SetSyspmaskopt(void * data, unsigned int len);
int SysDoBootProc(void *data, unsigned int len);
int SetSysVideoCodecCombo(void *data, unsigned int len);
int SetSysClickSnapFilename(void *data, unsigned int len);
int SetSysClickSnapStorage(void *data, unsigned int len);
int SetSysRateControl1(void *data, unsigned int len);
int SetSysRateControl2(void *data, unsigned int len);
int SetSysDateStampEnable1(void *data, unsigned int len);
int SetSysDateStampEnable2(void *data, unsigned int len);
int SetSysDateStampEnable3(void *data, unsigned int len);
int SetSysTimeStampEnable1(void *data, unsigned int len);
int SetSysTimeStampEnable2(void *data, unsigned int len);
int SetSysTimeStampEnable3(void *data, unsigned int len);
int SetSysLogoEnable1(void *data, unsigned int len);
int SetSysLogoEnable2(void *data, unsigned int len);
int SetSysLogoEnable3(void *data, unsigned int len);
int SetSysLogoPosition1(void *data, unsigned int len);
int SetSysLogoPosition2(void *data, unsigned int len);
int SetSysLogoPosition3(void *data, unsigned int len);
int SetSysTextEnable1(void *data, unsigned int len);
int SetSysTextEnable2(void *data, unsigned int len);
int SetSysTextEnable3(void *data, unsigned int len);
int SetSysTextPosition1(void *data, unsigned int len);
int SetSysTextPosition2(void *data, unsigned int len);
int SetSysTextPosition3(void *data, unsigned int len);
int SetSysOverlayText1(void *data, unsigned int len);
int SetSysOverlayText2(void *data, unsigned int len);
int SetSysOverlayText3(void *data, unsigned int len);
int SetSysEncryptVideo(void *data, unsigned int len);
int SetSysLocalDisplay(void *data, unsigned int len);
int SetSysIpratio1(void *data, unsigned int len);
int SetSysIpratio2(void *data, unsigned int len);
int SetSysIpratio3(void *data, unsigned int len);
int SetSysForceIframe1(void *data, unsigned int len);
int SetSysForceIframe2(void *data, unsigned int len);
int SetSysForceIframe3(void *data, unsigned int len);
int SetSysQPInit1(void *data, unsigned int len);
int SetSysQPInit2(void *data, unsigned int len);
int SetSysQPInit3(void *data, unsigned int len);
int SetSysQPMin1(void *data, unsigned int len);
int SetSysQPMin2(void *data, unsigned int len);
int SetSysQPMin3(void *data, unsigned int len);
int SetSysQPMax1(void *data, unsigned int len);
int SetSysQPMax2(void *data, unsigned int len);
int SetSysQPMax3(void *data, unsigned int len);
int SetSysMEConfig1(void *data, unsigned int len);
int SetSysMEConfig2(void *data, unsigned int len);
int SetSysMEConfig3(void *data, unsigned int len);
int SetSysPacketSize1(void *data, unsigned int len);
int SetSysPacketSize2(void *data, unsigned int len);
int SetSysPacketSize3(void *data, unsigned int len);
int SetSysROIEnable1(void *data, unsigned int len);
int SetSysROIEnable2(void *data, unsigned int len);
int SetSysROIEnable3(void *data, unsigned int len);
int SetSysStr1X1(void *data, unsigned int len);
int SetSysStr1Y1(void *data, unsigned int len);
int SetSysStr1W1(void *data, unsigned int len);
int SetSysStr1H1(void *data, unsigned int len);
int SetSysStr1X2(void *data, unsigned int len);
int SetSysStr1Y2(void *data, unsigned int len);
int SetSysStr1W2(void *data, unsigned int len);
int SetSysStr1H2(void *data, unsigned int len);
int SetSysStr1X3(void *data, unsigned int len);
int SetSysStr1Y3(void *data, unsigned int len);
int SetSysStr1W3(void *data, unsigned int len);
int SetSysStr1H3(void *data, unsigned int len);
int SetSysStr2X1(void *data, unsigned int len);
int SetSysStr2Y1(void *data, unsigned int len);
int SetSysStr2W1(void *data, unsigned int len);
int SetSysStr2H1(void *data, unsigned int len);
int SetSysStr2X2(void *data, unsigned int len);
int SetSysStr2Y2(void *data, unsigned int len);
int SetSysStr2W2(void *data, unsigned int len);
int SetSysStr2H2(void *data, unsigned int len);
int SetSysStr2X3(void *data, unsigned int len);
int SetSysStr2Y3(void *data, unsigned int len);
int SetSysStr2W3(void *data, unsigned int len);
int SetSysStr2H3(void *data, unsigned int len);
int SetSysStr3X1(void *data, unsigned int len);
int SetSysStr3Y1(void *data, unsigned int len);
int SetSysStr3W1(void *data, unsigned int len);
int SetSysStr3H1(void *data, unsigned int len);
int SetSysStr3X2(void *data, unsigned int len);
int SetSysStr3Y2(void *data, unsigned int len);
int SetSysStr3W2(void *data, unsigned int len);
int SetSysStr3H2(void *data, unsigned int len);
int SetSysStr3X3(void *data, unsigned int len);
int SetSysStr3Y3(void *data, unsigned int len);
int SetSysStr3W3(void *data, unsigned int len);
int SetSysStr3H3(void *data, unsigned int len);
int SetSysFramerate1(void *data, unsigned int len);
int SetSysFramerate2(void *data, unsigned int len);
int SetSysFramerate3(void *data, unsigned int len);
int SetSysMulticast(void *data, unsigned int len);
int SetSysAudioReceiverEnable(void *data, unsigned int len);
int SetSysAudioServerIp(void *data, unsigned int len);
int SetSysLinearWdr(void *data, unsigned int len);
int SetSysFlickerDetect(data, len);

#endif /* _SYS_MSG_DRV_H_ */
/**
* @}
* @page SYS_MSG_DRV_HOW How to use system message driver?

-# Add new message ID in \ref Sys_Msg_Def.h if needed.
-# Call the API SysDrvInit() with the ID you added to initialize file
	message driver.
-# Now you can use any system message driver functions as you want.
-# Call the API SysDrvExit() to cleanup system message driver.

* Your code may like following:
* @code
#include <sys_msg_drv.h>
int main()
{
	if(SysDrvInit(SYS_BOA_MSG) < 0){
		exit(1);
	}
	// Use system message driver API here.
	SysDrvExit();
	return 0;
}
* @endcode
* \b See \b also \ref FILE_MSG_DRV_HOW
*/
