/* ===========================================================================
* @file sysctrl.c
*
* @path $(IPNCPATH)/util/
*
* @desc System control interface.
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
#include <stdio.h>
#include "sysctrl.h"
#include <sys_env_type.h>
#include "sys_msg_drv.h"
/**
* @brief Send commands.

* Send commands to system server.
* @param field [I ] command.
* @param *data [I ] pointer to data needed for command.
* @param len [I ] Data size.
* @return Function to excute command.
* @retval -1 Fail to send command.
*/
int ControlSystemData(unsigned int field, void *data, unsigned int len)
{
	int ret = 0;
	if(len == 0 || data == NULL)
		return -1;
	switch(field){
		case SFIELD_GET_DHCPC_ENABLE:
			ret = GetDhcpEn(data, len);
			break;
		case SFIELD_GET_HTTPPORT:
			ret = GetHttpPort(data, len);
			break;
		case SFIELD_GET_SNTP_FQDN:
			ret = GetSntpFqdn(data, len);
			break;
		case SFIELD_GET_DEVICE_TYPE:
			ret = GetDeviceType(data, len);
			break;
		case SFIELD_GET_TITLE:
			ret = GetTitle(data, len);
			break;
		case SFIELD_GET_REGUSR:
			ret = GetRegUsr(data, len);
			break;
		case SFIELD_GET_OSDTEXT:
			ret = GetOSDText(data, len);
			break;
		case SFIELD_GET_MAC:
			ret = GetMAC(data, len);
			break;
		case SFIELD_GET_IP:
			ret = GetIP(data, len);
			break;
		case SFIELD_GET_GATEWAY:
			ret = GetGateway(data, len);
			break;
		case SFIELD_GET_NETMASK:
			ret = GetNetmask(data, len);
			break;
		case SFIELD_GET_FTP_SERVER_IP:
			ret = GetFtpServerIp(data, len);
			break;
		case SFIELD_GET_FTP_USERNAME:
			ret = GetFtpUsername(data, len);
			break;
		case SFIELD_GET_FTP_PASSWORD:
			ret = GetFtpPassword(data, len);
			break;
		case SFIELD_GET_FTP_FOLDNAME:
			ret = GetFtpFoldername(data, len);
			break;
		case SFIELD_GET_FTP_IMAGEAMOUNT:
			ret = GetFtpImageAmount(data, len);
			break;
		case SFIELD_GET_FTP_PID:
			ret = GetFtpPid(data, len);
			break;
		case SFIELD_GET_FTP_PORT:
			ret = GetSysFtpPort(data, len);
			break;
		case SFIELD_GET_SMTP_SERVER_IP:
			ret = GetSmtpServerIp(data, len);
			break;
		case SFIELD_GET_SMTP_PORT:
			ret = GetSmtpServerPort(data, len);
			break;
		case SFIELD_GET_SMTP_AUTHENTICATION:
			ret = GetSmtpAuthentication(data, len);
			break;
		case SFIELD_GET_SMTP_USERNAME:
			ret = GetsmtpUsername(data, len);
			break;
		case SFIELD_GET_SMTP_PASSWORD:
			ret = GetSmtpPassword(data, len);
			break;
		case SFIELD_GET_SMTP_SENDER_EMAIL_ADDRESS:
			ret = GetSmtpSenderEmail(data, len);
			break;
		case SFIELD_GET_SMTP_RECEIVER_EMAIL_ADDRESS:
			ret = GetSmtpReceiverEmail(data, len);
			break;
		case SFIELD_GET_SMTP_CC:
			ret = GetSmtpCC(data, len);
			break;
		case SFIELD_GET_SD_FILE_NAME:
			ret = SysGetSdFileName(data, len);
			break;
		case SFIELD_GET_SMTP_SUBJECT:
			ret = GetSmtpSubject(data, len);
			break;
		case SFIELD_GET_SMTP_TEXT:
			ret = GetSmtpText(data, len);
			break;
		case SFIELD_GET_SMTP_ATTACHMENTS:
			ret = GetSmtpAttachments(data, len);
			break;
		case SFIELD_GET_SMTP_VIEW:
			ret = GetSmtpView(data, len);
			break;
		/*case SFIELD_GET_UPNP_PORT:
			ret = GetSysUpnpPort(data, len);
			break;*/
		case SFIELD_SET_NETMASK:
			ret = SetNetMask(data, len);
			break;
		case SFIELD_SET_TIMEFORMAT:
			ret = SetSysTimeFormat(data, len);
			break;
		case SFIELD_SET_DAYLIGHT:
			ret = SetSysDaylight(data, len);
			break;
		case SFIELD_SET_TIMEZONE:
			ret = SetSysTimeZone(data, len);
			break;
		case SFIELD_SET_IP:
			ret = SetIp(data, len);
			break;
		case SFIELD_SET_HTTPPORT:
			ret = SetHttpPort(data, len);
			break;
		case SFIELD_SET_TITLE:
			ret = SetTitle(data, len);
			break;
		case SFIELD_SET_REGUSR:
			ret = SetRegUsr(data, len);
			break;
		case SFIELD_SET_OSDTEXT:
			ret = SetOSDText(data, len);
			break;
		case SFIELD_SET_GATEWAY:
			ret = SetGateway(data, len);
			break;
		case SFIELD_SET_DHCPC_ENABLE:
			ret = SetDhcpEnable(data, len);
			break;
		case SFIELD_SET_FTP_SERVER_IP:
			ret = SetSysFtpFqdn(data, len);
			break;
		case SFIELD_SET_FTP_USERNAME:
			ret = SetFtpUsername(data, len);
			break;
		case SFIELD_SET_FTP_PASSWORD:
			ret = SetFtpPassword(data, len);
			break;
		case SFIELD_SET_FTP_FOLDNAME:
			ret = SetFtpFoldname(data, len);
			break;
		case SFIELD_SET_FTP_IMAGEAMOUNT:
			ret = SetFtpImageacount(data, len);
			break;
		case SFIELD_SET_FTP_PID:
			ret = SetFtpPid(data, len);
			break;
		case SFIELD_SET_FTP_PORT:
			ret = SetSysFtpPort(data, len);
			break;
		case SFIELD_SET_SMTP_SERVER_IP:
			ret = SetSmtpServerIp(data, len);
			break;
		case SFIELD_SET_SMTP_PORT:
			ret = SetSmtpServerPort(data, len);
			break;
		case SFIELD_SET_SMTP_AUTHENTICATION:
			ret = SetSmtpAuthentication(data, len);
			break;
		case SFIELD_SET_SMTP_USERNAME:
			ret = SetSmtpUsername(data, len);
			break;
		case SFIELD_SET_SMTP_PASSWORD:
			ret = SetSmtpPassword(data, len);
			break;
		case SFIELD_SET_SMTP_SENDER_EMAIL_ADDRESS:
			ret = SetSmtpSenderEmail(data, len);
			break;
		case SFIELD_SET_SMTP_RECEIVER_EMAIL_ADDRESS:
			ret = SetSmtpReceiverEmail(data, len);
			break;
		case SFIELD_SET_SMTP_CC:
			ret = SetSmtpCC(data, len);
			break;
		case SFIELD_SET_SMTP_SUBJECT:
			ret = SetSmtpSubject(data, len);
			break;
		case SFIELD_SET_SMTP_TEXT:
			ret = SetSmtpText(data, len);
			break;
		case SFIELD_SET_SMTP_ATTACHMENTS:
			ret = SetSmtpAttachments(data, len);
			break;
		case SFIELD_SET_SMTP_VIEW:
			ret = SetSmtpView(data, len);
			break;
		case SFIELD_GET_DNS:
			ret = GetDns(data, len);
			break;
		case SFIELD_SET_DNS:
			ret = SetDns(data, len);
			break;
		case SFIELD_SET_SNTP_SERVER:
			ret = SetSntpServer(data, len);
			break;
		case SFIELD_SET_DAY_NIGHT:
			ret = SetSysDayNight(data, len);
			break;
		case SFIELD_SET_WHITE_BALANCE:
			ret = SetSysWhiteBalance(data, len);
			break;
		case SFIELD_SET_BACKLIGHT:
			ret = SetSysBacklight(data, len);
			break;
		case SFIELD_SET_BRIGHTNESS:
			ret = SetSysBrightness(data, len);
			break;
		case SFIELD_SET_CONTRAST:
			ret = SetSysContrast(data, len);
			break;
		case SFIELD_SET_SATURATION:
			ret = SetSysSaturation(data, len);
			break;
		case SFIELD_SET_SHARPNESS:
			ret = SetSysSharpness(data, len);
			break;
		case SFIELD_SET_VSTAB:
			ret = SetSysCommon(data, len, SYS_MSG_SET_VSTAB);
			break;
		case SFIELD_SET_LDC:
			ret = SetSysCommon(data, len, SYS_MSG_SET_LDC);
			break;
		case SFIELD_SET_SNF:
			ret = SetSysCommon(data, len, SYS_MSG_SET_SNF);
			break;
		case SFIELD_SET_TNF:
			ret = SetSysCommon(data, len, SYS_MSG_SET_TNF);
			break;
		case SFIELD_SET_MAXEXPOSURE:
			ret = SetSysCommon(data, len, SYS_MSG_SET_MAXEXPOSURE);
			break;
		case SFIELD_SET_MAXGAIN:
			ret = SetSysCommon(data, len, SYS_MSG_SET_MAXGAIN);
			break;
		case SFIELD_SET_EXPPRIORITY:
			ret = SetSysCommon(data, len, SYS_MSG_SET_EXPPRIORITY);
			break;
 		case SFIELD_SET_AUDIOMODE:
			ret = SetSysCommon(data, len, SYS_MSG_SET_AUDIOMODE);
			break;
		case SFIELD_SET_AUDIO_ENCODE:
			ret = SetSysCommon(data, len, SYS_MSG_SET_AUDIO_ENCODE);
			break;
		case SFIELD_SET_AUDIO_SAMPLERATE:
			ret = SetSysCommon(data, len, SYS_MSG_SET_AUDIO_SAMPLERATE);
			break;
		case SFIELD_SET_AUDIO_BITRATE:
			ret = SetSysCommon(data, len, SYS_MSG_SET_AUDIO_BITRATE);
			break;
		case SFIELD_SET_AUDIO_ALARMLEVEL:
			ret = SetSysCommon(data, len, SYS_MSG_SET_AUDIO_ALARMLEVEL);
			break;
		case SFIELD_SET_AUDIOOUTVOLUME:
			ret = SetSysCommon(data, len, SYS_MSG_SET_AUDIOOUTVOLUME);
			break;
		case SFIELD_SET_AUDIORECEIVERENABLE:
			ret = SetSysAudioReceiverEnable(data, len);
			break;
		case SFIELD_SET_AUDIOSERVERIP:
			ret = SetSysAudioServerIp(data, len);
			break;
		case SFIELD_SET_DEFAULT:
			ret = SetSysCommon(data, len, SYS_MSG_SET_DEFAULT);
			break;
		case SFIELD_SET_CLEARSYSLOG:
			ret = SetSysClearSysLog(data, len);
			break;
		case SFIELD_SET_CLEARACCESSLOG:
			ret = SetSysClearAccessLog(data, len);
			break;
		case SFIELD_SET_JQUALITY:
			ret = SetSysJpegQuality(data, len);
			break;
		case SFIELD_SET_FDETECT:
			ret = SetSysfdetect(data, len);
			break;
		case SFIELD_SET_FDX:
			ret = SetSysfdx(data, len);
			break;
		case SFIELD_SET_FDY:
			ret = SetSysfdy(data, len);
			break;
		case SFIELD_SET_FDW:
			ret = SetSysfdw(data, len);
			break;
		case SFIELD_SET_FDH:
			ret = SetSysfdh(data, len);
			break;
		case SFIELD_SET_FDCONF_LEVEL:
			ret = SetSysfdconf(data, len);
			break;
		case SFIELD_SET_FD_DIRECTION:
			ret = SetSysfddir(data, len);
			break;
	       case SFIELD_SET_FRECOGNITION:
		   	ret = SetSysfrecog(data, len);
			break;
		case SFIELD_SET_FRCONF_LEVEL:
			ret = SetSysfrconf(data, len);
			break;
		case SFIELD_SET_FR_DATABASE:
			ret = SetSysfrdbase(data, len);
			break;
		case SFIELD_SET_PRIVACY_MASK:
			ret = SetSyspmask(data, len);
			break;
		case SFIELD_SET_MASK_OPTIONS:
			ret = SetSyspmaskopt(data, len);
			break;
		case SFIELD_SET_ADVMODE:
			ret = SetSysAdvMode(data, len);
			break;
		case SFIELD_SET_MIRROR:
			ret = SetSysMirror(data, len);
			break;
		case SFIELD_SET_DEMOCFG:
			ret = SetSysDemoCfg(data, len);
			break;
		case SFIELD_SET_OSDWIN:
			ret = SetSysOSDWin(data, len);
			break;
		case SFIELD_SET_HISTOGRAM:
			ret = SetSysHistogram(data, len);
			break;
		case SFIELD_SET_DYNRANGE:
			ret = SetSysDynRange(data, len);
			break;
		case SFIELD_SET_OSDWINNUM:
			ret = SetSysOSDWinNum(data, len);
			break;
		case SFIELD_SET_OSDSTREAM:
			ret = SetSysOSDStream(data, len);
			break;
		case SFIELD_SET_VIDEO_MODE:
			ret = SetSysVideoMode(data, len);
			break;
		case SFIELD_SET_VIDEOCODECCOMBO:
			ret = SetSysVideoCodecCombo(data, len);
			break;
		case SFIELD_SET_VIDEOCODECRES:
			ret = SetSysVideoCodecRes(data, len);
			break;
		case SFIELD_SET_IMAGESOURCE:
			ret = SetSysImageSource(data, len);
			break;
		case SFIELD_SET_SCHEDULE:
			ret = SetSysSchedule(data, len);
			break;
		case SFIELD_SET_IMAGEFORMAT:
			ret = SetSysIMGFormat(data, len);
			break;
		case SFIELD_SET_RESOLUTION:
			ret = SetSysResolution(data, len);
			break;
		case SFIELD_SET_MPEG4_RES:
			ret = SetSysMPEG4Res(data, len);
			break;
		case SFIELD_SET_MPEG42_RES:
			ret = SetSysMPEG42Res(data, len);
			break;
        case SFIELD_SET_LOSTALARM:
			ret = SetSysLostAlarm(data, len);
			break;
		case SFIELD_SET_SDAENABLE:
			ret = SetSysSDAlarmEnable(data, len);
			break;
		case SFIELD_SET_AFTPENABLE:
			ret = SetSysFtpAlarmEnable(data, len);
			break;
		case SFIELD_SET_ASMTPENABLE:
			ret = SetSysSmtpAlarmEnable(data, len);
			break;
		case SFIELD_SET_ALARMDURATION:
			ret = SetSysAlarmDuration(data, len);
			break;
		case SFIELD_SET_AVIDURATION:
			ret = SetAVIDuration(data, len);
			break;
		case SFIELD_SET_AVIFORMAT:
			ret = SetAVIFormat(data, len);
			break;
		case SFIELD_SET_FTPFILEFORMAT:
			ret = SetFTPFileFormat(data, len);
			break;
		case SFIELD_SET_SDFILEFORMAT:
			ret = SetSDFileFormat(data, len);
			break;
		case SFIELD_SET_ATTFILEFORMAT:
			ret = SetAttFileFormat(data, len);
			break;
		case SFIELD_SET_AUDIOENABLE:
			ret = SetAudioEnable(data, len);
			break;
		case SFIELD_SET_ASMTPATTACH:
			ret = SetASmtpAttach(data, len);
			break;
		case SFIELD_SET_FTP_RFTPENABLE:
			ret = SetRftpEnable(data, len);
			break;
		case SFIELD_SET_SD_SDRENABLE:
			ret = SetSdReEnable(data, len);
			break;
		case SFIELD_SET_MOTIONENABLE:
			ret = SetMotionEnable(data, len);
			break;
		case SFIELD_SET_MOTIONCENABLE:
			ret = SetMotionCEnable(data, len);
			break;
		case SFIELD_SET_MOTIONLEVEL:
			ret = SetMotionLevel(data, len);
			break;
		case SFIELD_SET_MOTIONCVALUE:
			ret = SetMotionCValue(data, len);
			break;
		case SFIELD_SET_MOTIONBLOCK:
			ret = SetMotionBlock(data, len);
			break;
		case SFIELD_GET_DHCP_CONFIG:
			ret = GetSysDhcpConfig(data, len);
			break;
		case SFIELD_DEL_SCHEDULE:
			ret = DelSysSchedule(data, len);
			break;
		case SFIELD_SET_IMAGE2A:
			ret = SetSysImage2A(data, len);
			break;
		case SFIELD_SET_IMAGE2ATYPE:
			ret = SetSysImage2AType(data, len);
			break;
		case SFIELD_SET_BINNING:
			ret = SetSysBinning(data, len);
			break;
		case SFIELD_SET_BLC:
			ret = SetSysBLC(data, len);
			break;
		case SFIELD_SET_MPEG41_BITRATE:
			ret = SetMPEG41Bitrate(data, len);
			break;
		case SFIELD_SET_MPEG42_BITRATE:
			ret = SetMPEG42Bitrate(data, len);
			break;
		case SFIELD_SET_CLICK_SNAP_FILENAME :
			ret = SetSysClickSnapFilename(data, len);
			break;
		case  SFIELD_SET_CLICK_SNAP_STORAGE :
			ret = SetSysClickSnapStorage(data, len);
			break;
		case SFIELD_SET_RATE_CONTROL1:
			 ret = SetSysRateControl1(data, len);
			 break;
		case SFIELD_SET_RATE_CONTROL2:
			 ret = SetSysRateControl2(data, len);
			 break;
	       case SFIELD_SET_DSTAMPENABLE1:
		   	ret = SetSysDateStampEnable1(data, len);
			break;
		case SFIELD_SET_DSTAMPENABLE2:
		   	ret = SetSysDateStampEnable2(data, len);
			break;
		case SFIELD_SET_DSTAMPENABLE3:
		   	ret = SetSysDateStampEnable3(data, len);
			break;
		case SFIELD_SET_TSTAMPENABLE1:
			ret = SetSysTimeStampEnable1(data, len);
			break;
		case SFIELD_SET_TSTAMPENABLE2:
			ret = SetSysTimeStampEnable2(data, len);
			break;
		case SFIELD_SET_TSTAMPENABLE3:
			ret = SetSysTimeStampEnable3(data, len);
			break;
		case SFIELD_SET_LOGOENABLE1:
			ret = SetSysLogoEnable1(data, len);
			break;
		case SFIELD_SET_LOGOENABLE2:
			ret = SetSysLogoEnable2(data, len);
			break;
		case SFIELD_SET_LOGOENABLE3:
			ret = SetSysLogoEnable3(data, len);
			break;
		case SFIELD_SET_LOGO_POSITION1:
			ret = SetSysLogoPosition1(data, len);
			break;
		case SFIELD_SET_LOGO_POSITION2:
			ret = SetSysLogoPosition2(data, len);
			break;
		case SFIELD_SET_LOGO_POSITION3:
			ret = SetSysLogoPosition3(data, len);
			break;
		case SFIELD_SET_TEXTENABLE1:
			ret = SetSysTextEnable1(data, len);
			break;
		case SFIELD_SET_TEXTENABLE2:
			ret = SetSysTextEnable2(data, len);
			break;
		case SFIELD_SET_TEXTENABLE3:
			ret = SetSysTextEnable3(data, len);
			break;
		case SFIELD_SET_TEXT_POSITION1:
			ret = SetSysTextPosition1(data, len);
			break;
		case SFIELD_SET_TEXT_POSITION2:
			ret = SetSysTextPosition2(data, len);
			break;
		case SFIELD_SET_TEXT_POSITION3:
			ret = SetSysTextPosition3(data, len);
			break;
		case SFIELD_SET_OVERLAY_TEXT1:
			ret = SetSysOverlayText1(data, len);
			break;
		case SFIELD_SET_OVERLAY_TEXT2:
			ret = SetSysOverlayText2(data, len);
			break;
		case SFIELD_SET_OVERLAY_TEXT3:
			ret = SetSysOverlayText3(data, len);
			break;
		case SFIELD_SET_DETAIL_INFO1:
			ret = SetSysCommon(data, len, SYS_MSG_SET_DETAIL_INFO1);
			break;
		case SFIELD_SET_DETAIL_INFO2:
			ret = SetSysCommon(data, len, SYS_MSG_SET_DETAIL_INFO2);
			break;
		case SFIELD_SET_DETAIL_INFO3:
			ret = SetSysCommon(data, len, SYS_MSG_SET_DETAIL_INFO3);
			break;
		case SFIELD_SET_ALARMLOCAL:
			ret = SetSysCommon(data, len, SYS_MSG_SET_ALARMLOCAL);
			break;
		case SFIELD_SET_RECORDLOCAL:
			ret = SetSysCommon(data, len, SYS_MSG_SET_RECORDLOCAL);
			break;
		case SFIELD_SET_ENCRYPT_VIDEO:
			ret = SetSysEncryptVideo(data, len);
			break;
		case SFIELD_SET_LOCAL_DISPLAY:
			ret = SetSysLocalDisplay(data, len);
			break;
		case SFIELD_SET_IPRATIO1:
			ret = SetSysIpratio1(data, len);
			break;
		case SFIELD_SET_IPRATIO2:
			ret = SetSysIpratio2(data, len);
			break;
		case SFIELD_SET_IPRATIO3:
			ret = SetSysIpratio3(data, len);
			break;
		case SFIELD_SET_FORCE_IFRAME1:
			ret = SetSysForceIframe1(data, len);
			break;
		case SFIELD_SET_FORCE_IFRAME2:
			ret = SetSysForceIframe2(data, len);
			break;
		case SFIELD_SET_FORCE_IFRAME3:
			ret = SetSysForceIframe3(data, len);
			break;
		case SFIELD_SET_QPINIT1:
			ret = SetSysQPInit1(data, len);
			break;
		case SFIELD_SET_QPINIT2:
			ret = SetSysQPInit2(data, len);
			break;
		case SFIELD_SET_QPINIT3:
			ret = SetSysQPInit3(data, len);
			break;
		case SFIELD_SET_QPMIN1:
			ret = SetSysQPMin1(data, len);
			break;
		case SFIELD_SET_QPMIN2:
			ret = SetSysQPMin2(data, len);
			break;
		case SFIELD_SET_QPMIN3:
			ret = SetSysQPMin3(data, len);
			break;
		case SFIELD_SET_QPMAX1:
			ret = SetSysQPMax1(data, len);
			break;
		case SFIELD_SET_QPMAX2:
			ret = SetSysQPMax2(data, len);
			break;
		case SFIELD_SET_QPMAX3:
			ret = SetSysQPMax3(data, len);
			break;
		case SFIELD_SET_MECONFIG1:
			ret = SetSysMEConfig1(data, len);
			break;
		case SFIELD_SET_MECONFIG2:
			ret = SetSysMEConfig2(data, len);
			break;
		case SFIELD_SET_MECONFIG3:
			ret = SetSysMEConfig3(data, len);
			break;
		case SFIELD_SET_PACKETSIZE1:
			ret = SetSysPacketSize1(data, len);
			break;
		case SFIELD_SET_PACKETSIZE2:
			ret = SetSysPacketSize2(data, len);
			break;
		case SFIELD_SET_PACKETSIZE3:
			ret = SetSysPacketSize3(data, len);
			break;
		case SFIELD_SET_ROI_ENABLE1:
			ret = SetSysROIEnable1(data, len);
			break;
		case SFIELD_SET_ROI_ENABLE2:
			ret = SetSysROIEnable2(data, len);
			break;
	    case SFIELD_SET_ROI_ENABLE3:
			ret = SetSysROIEnable3(data, len);
			break;
		case SFIELD_SET_STR1X1:
			ret = SetSysStr1X1(data, len);
			break;
		case SFIELD_SET_STR1Y1:
			ret = SetSysStr1Y1(data, len);
			break;
        case SFIELD_SET_STR1W1:
			ret = SetSysStr1W1(data, len);
			break;
		case SFIELD_SET_STR1H1:
			ret = SetSysStr1H1(data, len);
			break;
		case SFIELD_SET_STR1X2:
			ret = SetSysStr1X2(data, len);
			break;
		case SFIELD_SET_STR1Y2:
			ret = SetSysStr1Y2(data, len);
			break;
        case SFIELD_SET_STR1W2:
			ret = SetSysStr1W2(data, len);
			break;
		case SFIELD_SET_STR1H2:
			ret = SetSysStr1H2(data, len);
			break;
		case SFIELD_SET_STR1X3:
			ret = SetSysStr1X3(data, len);
			break;
		case SFIELD_SET_STR1Y3:
			ret = SetSysStr1Y3(data, len);
			break;
        case SFIELD_SET_STR1W3:
			ret = SetSysStr1W3(data, len);
			break;
		case SFIELD_SET_STR1H3:
			ret = SetSysStr1H3(data, len);
			break;
		case SFIELD_SET_STR2X1:
			ret = SetSysStr2X1(data, len);
			break;
		case SFIELD_SET_STR2Y1:
			ret = SetSysStr2Y1(data, len);
			break;
        case SFIELD_SET_STR2W1:
			ret = SetSysStr2W1(data, len);
			break;
		case SFIELD_SET_STR2H1:
			ret = SetSysStr2H1(data, len);
			break;
		case SFIELD_SET_STR2X2:
			ret = SetSysStr2X2(data, len);
			break;
		case SFIELD_SET_STR2Y2:
			ret = SetSysStr2Y2(data, len);
			break;
        case SFIELD_SET_STR2W2:
			ret = SetSysStr2W2(data, len);
			break;
		case SFIELD_SET_STR2H2:
			ret = SetSysStr2H2(data, len);
			break;
		case SFIELD_SET_STR2X3:
			ret = SetSysStr2X3(data, len);
			break;
		case SFIELD_SET_STR2Y3:
			ret = SetSysStr2Y3(data, len);
			break;
        case SFIELD_SET_STR2W3:
			ret = SetSysStr2W3(data, len);
			break;
		case SFIELD_SET_STR2H3:
			ret = SetSysStr2H3(data, len);
			break;
		case SFIELD_SET_STR3X1:
			ret = SetSysStr3X1(data, len);
			break;
		case SFIELD_SET_STR3Y1:
			ret = SetSysStr3Y1(data, len);
			break;
        case SFIELD_SET_STR3W1:
			ret = SetSysStr3W1(data, len);
			break;
		case SFIELD_SET_STR3H1:
			ret = SetSysStr3H1(data, len);
			break;
		case SFIELD_SET_STR3X2:
			ret = SetSysStr3X2(data, len);
			break;
		case SFIELD_SET_STR3Y2:
			ret = SetSysStr3Y2(data, len);
			break;
        case SFIELD_SET_STR3W2:
			ret = SetSysStr3W2(data, len);
			break;
		case SFIELD_SET_STR3H2:
			ret = SetSysStr3H2(data, len);
			break;
		case SFIELD_SET_STR3X3:
			ret = SetSysStr3X3(data, len);
			break;
		case SFIELD_SET_STR3Y3:
			ret = SetSysStr3Y3(data, len);
			break;
        case SFIELD_SET_STR3W3:
			ret = SetSysStr3W3(data, len);
			break;
		case SFIELD_SET_STR3H3:
			ret = SetSysStr3H3(data, len);
			break;
		case SFIELD_SET_IMAGEDEFAULT:
			ret = SetImageDefault(data, len);
			break;
		case SFIELD_DO_LOGIN:
			ret = DoSysLogin(data, len);
			break;
		case SFIELD_GET_USER_AUTHORITY:
			ret = GetSysUserAuthority(data, len);
			break;
		case SFIELD_ADD_USER:
			ret = SysAddUser(data, len);
			break;
		case SFIELD_DEL_USER:
			ret = SysDelUser(data, len);
			break;
		case SFIELD_IPNCPTZ:
			ret = SetIpncPtz(data, len);
			break;
		case SFIELD_INIT_IPNCPTZ:
			ret = InitIpncPtz(data, len);
			break;
		case SFIELD_SD_FORMAT:
			ret = SysSdFormat(data, len);
			break;
		case SFIELD_SD_UNMOUNT:
			ret = SysSdUnmount(data, len);
			break;
		case SFIELD_SET_GIOINENABLE:
			ret = SetGIOInEnable(data, len);
			break;
		case SFIELD_SET_GIOINTYPE:
			ret = SetGIOInType(data, len);
			break;
		case SFIELD_SET_GIOOUTENABLE:
			ret = SetGIOOutEnable(data, len);
			break;
		case SFIELD_SET_GIOOUTTYPE:
			ret = SetGIOOutType(data, len);
			break;
		case SFIELD_SET_TSTAMPENABLE:
			ret = SetTStampEnable(data, len);
			break;
		case SFIELD_SET_DATEFORMAT:
		    ret = SetSysCommon(data, len, SYS_MSG_SET_DATEFORMAT);
			break;
		case SFIELD_SET_TSTAMPFORMAT:
		    ret = SetSysCommon(data, len, SYS_MSG_SET_TSTAMPFORMAT);
			break;
		case SFIELD_SET_DATEPPOS:
		    ret = SetSysCommon(data, len, SYS_MSG_SET_DATEPPOS);
			break;
		case SFIELD_SET_TIMEPOS:
		    ret = SetSysCommon(data, len, SYS_MSG_SET_TIMEPOS);
			break;
		case SFIELD_SET_AUDIOINVOLUME:
			ret = SetAudioinVolume(data, len);
			break;
		case SFIELD_SET_RATE_CONTROL:
			ret = SetSysRateControl(data, len);
			break;
		case SFIELD_SET_FRAMERATE1:
			ret = SetSysFramerate1(data, len);
			break;
		case SFIELD_SET_FRAMERATE2:
			ret = SetSysFramerate2(data, len);
			break;
		case SFIELD_SET_FRAMERATE3:
			ret = SetSysFramerate3(data, len);
			break;
		case SFIELD_SCHEDULE_STOP:
			ret = SysScheduleStop(data, len);
			break;
		case SFIELD_SCHEDULE_RESUME:
			ret = SysScheduleResume(data, len);
			break;
		case SFIELD_SET_MULTICAST:
			ret = SetSysMulticast(data, len);
			break;
		case SFIELD_SET_HTTPSPORT:
			ret = SetSysCommon(data, len, SYS_MSG_SET_HTTPSPORT);
			break;
		case SFIELD_SET_RS485:
			ret = SetSysCommon(data, len, SYS_MSG_SET_RS485);
			break;
		case SFIELD_SET_ALARM_ENABLE:
			ret = SetSysCommon(data, len, SYS_MSG_SET_ALARM_ENABLE);
			break;
		case SFIELD_SET_EXT_ALARM:
			ret = SetSysCommon(data, len, SYS_MSG_SET_EXT_ALARM);
			break;
		case SFIELD_SET_AUDIOALARM:
			ret = SetSysCommon(data, len, SYS_MSG_SET_AUDIOALARM);
			break;
		case SFIELD_SET_ALARM_AUDIOPLAY:
			ret = SetSysCommon(data, len, SYS_MSG_SET_ALARM_AUDIOPLAY);
			break;
		case SFIELD_SET_ALARM_AUDIOFILE:
			ret = SetSysCommon(data, len, SYS_MSG_SET_ALARM_AUDIOFILE);
			break;
		case SFIELD_SET_SCHEDULE_REPEAT_ENABLE:
			ret = SetSysCommon(data, len, SYS_MSG_SET_SCHEDULE_REPEAT_ENABLE);
			break;
		case SFIELD_SET_SCHEDULE_NUMWEEKS:
			ret = SetSysCommon(data, len, SYS_MSG_SET_SCHEDULE_NUM_WEEKS);
			break;
		case SFIELD_SET_SCHEDULE_INFINITE_ENABLE:
			ret = SetSysCommon(data, len, SYS_MSG_SET_SCHEDULE_INFINITE_ENABLE);
			break;
		case SFIELD_SET_LINEARWDR:
			ret = SetSysLinearWdr(data, len);
			break;
		case SFIELD_SET_FLICKER_DETECT:
			ret = SetSysFlickerDetect(data, len);
			break;
		default:
			ret = -1;
			break;
	}
	return ret;
}
