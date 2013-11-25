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
* @file sysctrl.h
* @brief Send commands to system server
*/
#ifndef __SYS_CONTROL_H__
#define __SYS_CONTROL_H__
/**
* @ingroup UTILITY API
* @defgroup SYSCTRL System command API

* This is the communication interface of system server.
* @{
*/
/* Date    2008/01/18 */
/* Version 1.00.01    */
/**
* @brief Command list.
*/
typedef enum
{
	/* Network Get Control */
  SFIELD_GET_DHCPC_ENABLE = 0x0000, ///< Get DHCP status.
  SFIELD_GET_HTTPPORT, ///< Get http port.
  SFIELD_GET_SNTP_FQDN, ///< Get SNTP FQDN.
  SFIELD_GET_DEVICE_TYPE, ///< Get device type.
  SFIELD_GET_TITLE, ///< Get IP netcam title.
  SFIELD_GET_REGUSR, ///< Get IP netcam title.
  SFIELD_GET_OSDTEXT, ///< Get IP netcam title.
  SFIELD_GET_MAC, ///< Get ethernet MAC address.
  SFIELD_GET_IP, ///< Get IP netcam IP address.
  SFIELD_GET_GATEWAY, ///< Get IP netcam gateway.
  SFIELD_GET_NETMASK, ///< Get IP netcam netmask.
  SFIELD_GET_FTP_SERVER_IP, ///< Get FTP FQDN.
  SFIELD_GET_FTP_USERNAME, ///< Get FTP user name.
  SFIELD_GET_FTP_PASSWORD, ///< Get FTP user password.
  SFIELD_GET_FTP_FOLDNAME, ///< Get FTP foleder.
  SFIELD_GET_FTP_IMAGEAMOUNT, ///< Get JPEG count at each FTP connection.
  SFIELD_GET_FTP_PID, ///< Get FTP process ID.
  SFIELD_GET_FTP_PORT, ///< Get FTP connection port.
  SFIELD_GET_SMTP_SERVER_IP, ///< Get SMTP FQDN.
  SFIELD_GET_SMTP_PORT, ///< Get SMTP PORT.
  SFIELD_GET_SMTP_AUTHENTICATION, ///< See if use authentication to send e-mail.
  SFIELD_GET_SMTP_USERNAME, ///< Get SMTP user name.
  SFIELD_GET_SMTP_PASSWORD, ///< Get SMTP password.
  SFIELD_GET_SMTP_SENDER_EMAIL_ADDRESS, ///< Get sender e-mail address.
  SFIELD_GET_SMTP_RECEIVER_EMAIL_ADDRESS, ///< Get receiver e-mail address.
  SFIELD_GET_SMTP_CC, ///< Get e-mail CC.
  SFIELD_GET_SMTP_SUBJECT, ///< Get e-mail subject.
  SFIELD_GET_SMTP_TEXT, ///< Get e-mail text.
  SFIELD_GET_SMTP_ATTACHMENTS, ///< Get e-mail JPEG attachments.
  SFIELD_GET_SMTP_VIEW, ///< Get e-mail style.
  SFIELD_GET_DNS, ///< Get DNS address.
  SFIELD_GET_DHCP_CONFIG, ///< Get DHCP status.
  SFIELD_GET_SD_FILE_NAME,///< Get current recording file name in SD card.
  //SFIELD_GET_UPNP_PORT,
  /* Network Set Control */
  SFIELD_SET_NETMASK      = 0x1000, ///< Set new netmask of IP netcam.
  SFIELD_SET_TIMEFORMAT, ///< Set display time format.
  SFIELD_SET_IP, ///< Set static IP address.
  SFIELD_SET_REGUSR, ///< Set IP netcam title.
  SFIELD_SET_OSDTEXT, ///< Set IP netcam title.
  SFIELD_SET_GATEWAY, ///< Set gateway in static IP mode.
  SFIELD_SET_DHCPC_ENABLE, ///< Set DHCP enable.
  SFIELD_SET_FTP_SERVER_IP, ///< Set FTP FQDN.
  SFIELD_SET_FTP_USERNAME, ///< Set FTP username.
  SFIELD_SET_FTP_PASSWORD, ///< Set FTP password.
  SFIELD_SET_FTP_FOLDNAME, ///< Set FTP folder.
  SFIELD_SET_FTP_IMAGEAMOUNT, ///< Set JPEG count at each FTP connection.
  SFIELD_SET_FTP_PID, ///< Save FTP process ID.
  SFIELD_SET_FTP_PORT, ///< Set FTP port.
  SFIELD_SET_SMTP_SERVER_IP, ///< Set SMTP FQDN.
  SFIELD_SET_SMTP_PORT, ///< Set SMTP PORT.
  SFIELD_SET_SMTP_AUTHENTICATION, ///< Let IP netcam use authentication to send e-mail or not.
  SFIELD_SET_SMTP_USERNAME, ///< Set SMTP user name.
  SFIELD_SET_SMTP_PASSWORD, ///< Set SMTP password.
  SFIELD_SET_SMTP_SENDER_EMAIL_ADDRESS, ///< Set sender e-mail address.
  SFIELD_SET_SMTP_RECEIVER_EMAIL_ADDRESS, ///< Set receiver e-mail address.
  SFIELD_SET_SMTP_CC, ///< Set e-mail CC.
  SFIELD_SET_SMTP_SUBJECT, ///< Set e-mail subject.
  SFIELD_SET_SMTP_TEXT, ///< Set e-mail text.
  SFIELD_SET_SMTP_ATTACHMENTS, ///< Set e-mail JPEG attachments.
  SFIELD_SET_SMTP_VIEW, ///< Set e-mail style.
  SFIELD_SET_DNS, ///< Set DNS address.
  SFIELD_SET_SNTP_SERVER, ///< Set SNTP FQDN.
  SFIELD_SET_ADVMODE,
  SFIELD_SET_DEMOCFG,
  SFIELD_SET_OSDWIN,
  SFIELD_SET_OSDWINNUM,
  SFIELD_SET_OSDSTREAM,
  SFIELD_SET_RATE_CONTROL, ///< Set rate control.
  SFIELD_SET_IMAGESOURCE, ///< Select NTSC/PAL mode.
  SFIELD_SET_SCHEDULE, ///< Set user define schedule.
  SFIELD_SET_SDAENABLE, ///< Enable save file to SD card when alarm happen or not.
  SFIELD_SET_IMAGEFORMAT,
  SFIELD_SET_RESOLUTION, ///< Set JPEG resolution.
  SFIELD_SET_MPEG4_RES, ///< Set MPEG1 resolution.
  SFIELD_SET_MPEG42_RES, ///< Set MPEG2 resolution.
  SFIELD_SET_AVIDURATION, ///< Set length of one AVI file(in seconds).
  SFIELD_SET_AVIFORMAT, ///< Set format of AVI file.
  SFIELD_SET_FTPFILEFORMAT, ///< Sent which kind of files to FTP.
  SFIELD_SET_SDFILEFORMAT, ///< Save which kind of files to SD card.
  SFIELD_SET_ATTFILEFORMAT, ///< Sent which kind of files to SMTP.
  SFIELD_SET_ASMTPATTACH,
  SFIELD_SET_FTP_RFTPENABLE, ///< Enable send file to FTP server at schedule.
  SFIELD_SET_SD_SDRENABLE, ///< Enable save file to SD card at schedule.
  SFIELD_SET_LOSTALARM, ///< Enable ethernet lost alarm or not.
  SFIELD_SET_ALARMDURATION, ///< Set how long will one alarm last.
  SFIELD_SET_AFTPENABLE, ///< Enable send file to FTP server when alarm happen or not.
  SFIELD_SET_ASMTPENABLE, ///< Enable send file to SMTP server when alarm happen or not.
  SFIELD_SET_IMAGEDEFAULT,///< Set all image setting to default.
  /* Live video Page */
  SFIELD_SET_CLICK_SNAP_FILENAME,
  SFIELD_SET_CLICK_SNAP_STORAGE,

  /* Video Image Page */
    SFIELD_SET_TITLE, ///< Set IP netcam title.
  	SFIELD_SET_VIDEO_MODE,
  	SFIELD_SET_VIDEOCODECCOMBO,
  	SFIELD_SET_VIDEOCODECRES, ///< Set video resolution. This operation will take several time.
  	SFIELD_SET_MPEG41_BITRATE, ///< Set MPEG1 bitrate.
  	SFIELD_SET_MPEG42_BITRATE,///< Set MPEG2 bitrate.
  	SFIELD_SET_JQUALITY, ///< Set JPEG encode quality.
  	SFIELD_SET_FRAMERATE1, ///< Set MPEG1 frame rate.
  	SFIELD_SET_FRAMERATE2, ///< Set MPEG2 frame rate.
    SFIELD_SET_FRAMERATE3, ///< Set JPEG frame rate.
	SFIELD_SET_RATE_CONTROL1,
    SFIELD_SET_RATE_CONTROL2,
    SFIELD_SET_DSTAMPENABLE1,
    SFIELD_SET_DSTAMPENABLE2,
    SFIELD_SET_DSTAMPENABLE3,
    SFIELD_SET_TSTAMPENABLE1,
    SFIELD_SET_TSTAMPENABLE2,
    SFIELD_SET_TSTAMPENABLE3,
    SFIELD_SET_LOGOENABLE1,
    SFIELD_SET_LOGOENABLE2,
    SFIELD_SET_LOGOENABLE3,
    SFIELD_SET_LOGO_POSITION1,
    SFIELD_SET_LOGO_POSITION2,
    SFIELD_SET_LOGO_POSITION3,
    SFIELD_SET_TEXTENABLE1,
    SFIELD_SET_TEXTENABLE2,
    SFIELD_SET_TEXTENABLE3,
    SFIELD_SET_TEXT_POSITION1,
    SFIELD_SET_TEXT_POSITION2,
    SFIELD_SET_TEXT_POSITION3,
    SFIELD_SET_OVERLAY_TEXT1,
    SFIELD_SET_OVERLAY_TEXT2,
    SFIELD_SET_OVERLAY_TEXT3,
    SFIELD_SET_DETAIL_INFO1,
    SFIELD_SET_DETAIL_INFO2,
    SFIELD_SET_DETAIL_INFO3,
    SFIELD_SET_ENCRYPT_VIDEO,
    SFIELD_SET_LOCAL_DISPLAY,
     SFIELD_SET_MIRROR,
	 /* Video Advanced Page */
    SFIELD_SET_IPRATIO1,
    SFIELD_SET_IPRATIO2,
    SFIELD_SET_IPRATIO3,
    SFIELD_SET_FORCE_IFRAME1,
    SFIELD_SET_FORCE_IFRAME2,
    SFIELD_SET_FORCE_IFRAME3,
    SFIELD_SET_QPINIT1,
    SFIELD_SET_QPINIT2,
    SFIELD_SET_QPINIT3,
    SFIELD_SET_QPMIN1,
    SFIELD_SET_QPMIN2,
    SFIELD_SET_QPMIN3,
    SFIELD_SET_QPMAX1,
    SFIELD_SET_QPMAX2,
    SFIELD_SET_QPMAX3,
    SFIELD_SET_MECONFIG1,
    SFIELD_SET_MECONFIG2,
    SFIELD_SET_MECONFIG3,
    SFIELD_SET_PACKETSIZE1,
    SFIELD_SET_PACKETSIZE2,
    SFIELD_SET_PACKETSIZE3,
    SFIELD_SET_ROI_ENABLE1,
    SFIELD_SET_ROI_ENABLE2,
    SFIELD_SET_ROI_ENABLE3,
    SFIELD_SET_STR1X1,
    SFIELD_SET_STR1Y1,
    SFIELD_SET_STR1W1,
    SFIELD_SET_STR1H1,
    SFIELD_SET_STR1X2,
    SFIELD_SET_STR1Y2,
    SFIELD_SET_STR1W2,
    SFIELD_SET_STR1H2,
    SFIELD_SET_STR1X3,
    SFIELD_SET_STR1Y3,
    SFIELD_SET_STR1W3,
    SFIELD_SET_STR1H3,
    SFIELD_SET_STR2X1,
    SFIELD_SET_STR2Y1,
    SFIELD_SET_STR2W1,
    SFIELD_SET_STR2H1,
    SFIELD_SET_STR2X2,
    SFIELD_SET_STR2Y2,
    SFIELD_SET_STR2W2,
    SFIELD_SET_STR2H2,
    SFIELD_SET_STR2X3,
    SFIELD_SET_STR2Y3,
    SFIELD_SET_STR2W3,
    SFIELD_SET_STR2H3,
    SFIELD_SET_STR3X1,
    SFIELD_SET_STR3Y1,
    SFIELD_SET_STR3W1,
    SFIELD_SET_STR3H1,
    SFIELD_SET_STR3X2,
    SFIELD_SET_STR3Y2,
    SFIELD_SET_STR3W2,
    SFIELD_SET_STR3H2,
    SFIELD_SET_STR3X3,
    SFIELD_SET_STR3Y3,
    SFIELD_SET_STR3W3,
    SFIELD_SET_STR3H3,

  /* Video Analytics */
  SFIELD_SET_MOTIONENABLE, ///< Enable motion detection or not.
  SFIELD_SET_MOTIONCENABLE, ///< Use customer define or normal mode.
  SFIELD_SET_MOTIONLEVEL, ///< Set motion level in normal mode.
  SFIELD_SET_MOTIONCVALUE, ///< Set customer value at customer mode.
  SFIELD_SET_MOTIONBLOCK, ///< Define motion blocks.
  SFIELD_SET_FDETECT,
  SFIELD_SET_FDX,
  SFIELD_SET_FDY,
  SFIELD_SET_FDW,
  SFIELD_SET_FDH,
  SFIELD_SET_FDCONF_LEVEL,
  SFIELD_SET_FD_DIRECTION,
  SFIELD_SET_FRECOGNITION,
  SFIELD_SET_FRCONF_LEVEL,
  SFIELD_SET_FR_DATABASE,
  SFIELD_SET_PRIVACY_MASK,
  SFIELD_SET_MASK_OPTIONS,
  /* Camera Page */
  SFIELD_SET_BRIGHTNESS, ///< Set brightness.
  SFIELD_SET_CONTRAST, ///< Set contrast.
  SFIELD_SET_SATURATION, ///< Set saturation.
  SFIELD_SET_SHARPNESS, ///< Set sharpness.
  SFIELD_SET_BLC,
  SFIELD_SET_DYNRANGE,
  SFIELD_SET_BACKLIGHT, ///< Set backlight mode of IP netcam.
  SFIELD_SET_WHITE_BALANCE, ///< Set whitebalance moe of IP netcam.
  SFIELD_SET_DAY_NIGHT, ///< Set day/night mode of IP netcam.
  SFIELD_SET_HISTOGRAM,
  SFIELD_SET_VSTAB,
  SFIELD_SET_LDC,
  SFIELD_SET_BINNING, ///< Set binning mode.
  SFIELD_SET_IMAGE2A,
  SFIELD_SET_IMAGE2ATYPE,
  SFIELD_SET_MAXEXPOSURE,
  SFIELD_SET_MAXGAIN,
  SFIELD_SET_EXPPRIORITY,
  SFIELD_SET_SNF,
  SFIELD_SET_TNF,
  /* Audio Page */
  SFIELD_SET_AUDIOENABLE, ///< Enable audio send to web site or not.
  SFIELD_SET_AUDIOMODE,
  SFIELD_SET_AUDIOINVOLUME, ///< Set alarm audio volume
  SFIELD_SET_AUDIO_ENCODE,
  SFIELD_SET_AUDIO_SAMPLERATE,
  SFIELD_SET_AUDIO_BITRATE,
  SFIELD_SET_AUDIO_ALARMLEVEL,
  SFIELD_SET_AUDIOOUTVOLUME,
  SFIELD_SET_AUDIORECEIVERENABLE,
  SFIELD_SET_AUDIOSERVERIP,
  /* Date time Page */
  SFIELD_SET_DAYLIGHT, ///< Set system time to use daylight or not.
  SFIELD_SET_TIMEZONE, ///< Set system time zone.
  SFIELD_SET_DATEFORMAT,
  SFIELD_SET_TSTAMPFORMAT, ///< Set timestamp format.
  SFIELD_SET_DATEPPOS,
  SFIELD_SET_TIMEPOS,
  /* Network & Port Page */
  SFIELD_SET_MULTICAST, ///< Set Multicast.
  SFIELD_SET_HTTPPORT, ///< set web site port. This operation may take serveral time.
  SFIELD_SET_HTTPSPORT,
  SFIELD_SET_GIOINTYPE, ///< Set GIO input alarm triger type(high triger or low triger).
  SFIELD_SET_GIOOUTTYPE, ///< Set GIO output high or low.
  SFIELD_SET_RS485,
  /* Alarm & Storage */
  SFIELD_SET_ALARM_ENABLE,
  SFIELD_SET_EXT_ALARM,
  SFIELD_SET_AUDIOALARM,
  SFIELD_SET_ALARM_AUDIOPLAY,
  SFIELD_SET_ALARM_AUDIOFILE,
  SFIELD_SET_ALARMLOCAL,
  SFIELD_SET_RECORDLOCAL,
  SFIELD_SET_SCHEDULE_REPEAT_ENABLE,
  SFIELD_SET_SCHEDULE_NUMWEEKS,
  SFIELD_SET_SCHEDULE_INFINITE_ENABLE,
  /* Other Contorl */
  SFIELD_DEL_SCHEDULE = 0x2000, ///< Delete all exist schedule.
  SFIELD_DO_LOGIN, ///< Do user login.
  SFIELD_GET_USER_AUTHORITY, ///< Get user authority by name.
  SFIELD_ADD_USER, ///< Add/change user acount.
  SFIELD_DEL_USER, ///< Delete user acount.
  SFIELD_IPNCPTZ, ///< Contorl PTZ.
  SFIELD_INIT_IPNCPTZ, ///< PTZ init 4X.
  SFIELD_SD_FORMAT, ///< Format SD card.
  SFIELD_SD_UNMOUNT, ///< Unmount SD card.
  SFIELD_SCHEDULE_STOP, ///< Stop schedule.
  SFIELD_SCHEDULE_RESUME, ///< Resume schedule.
  /* Maintainence */
  SFIELD_SET_DEFAULT,
  SFIELD_SET_CLEARSYSLOG,
  SFIELD_SET_CLEARACCESSLOG,
//gio
  SFIELD_SET_GIOINENABLE, ///< Enable GIO input alarm or not.
  SFIELD_SET_GIOOUTENABLE, ///< Enable GIO ouput at alarm or not.
  SFIELD_SET_TSTAMPENABLE, ///< Enable timestamp or not.

  SFIELD_SET_FLICKER_DETECT,
  SFIELD_SET_LINEARWDR
} SYSTEM_CONTROL;

int ControlSystemData(unsigned int field, void *data, unsigned int len); ///<  Protype definition.
/** @} */
#endif   /* __SYS_CONTROL_H__ */
/**
* @page SYSCTRL_HOW How to use system command API?

* 1. Add new command to list if needed.\n
* 2. Now you can use any system command functions as you want.\n
* @section SYSCTRL_HOW_EX Example
* @code
#include <sysctrl.h>
int main()
{
	int value = 0;
	if(ControlSystemData( // system command here, &value, sizeof(value)) < 0){
		return -1;
	}
	return 0;
}
* @endcode
*/
