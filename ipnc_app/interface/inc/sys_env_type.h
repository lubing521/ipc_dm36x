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
* @file sys_env_type.h
* @brief System evironment structure and Global definition.
*/
#ifndef __SYS_ENV_TYPE_H__
#define __SYS_ENV_TYPE_H__

#include <asm/types.h>
#include <netinet/in.h>
#include <time.h>

/*
MAGIC_NUM log
0x9A131904
0x56313930
0x56323030
0x56313600
*/
#define MAGIC_NUM			0x9A131904

#define SUCCESS 0
#define FAIL	-1

#define SYS_ENV_SIZE		sizeof(SysInfo)
#define MAX_LOG_PAGE_NUM	20
#define NUM_LOG_PER_PAGE	20
#define LOG_ENTRY_SIZE		sizeof(LogEntry_t)

#define MAX_DOMAIN_NAME			40 		///< Maximum length of domain name. Ex: www.xxx.com
#define MAX_LANCAM_TITLE_LEN	11 		///< Maximum length of LANCAM title.
#define OSDTEXTLEN				24
#define MAX_FQDN_LENGTH			256 	///< Maximum length of FQDN.
#define MAX_NAME_LENGTH			256 	///< Maximum length of normal string.
#define MAX_OSD_STREAMS			3 		///< Maximum length of normal string.
#define MAX_CODEC_STREAMS		3 		///< Maximum length of normal string.
#define MAX_STREAM_NAME			75  //25///< Maximum length of normal string.
#define MAC_LENGTH				6 		///< Length of MAC address.
#define SCHDULE_NUM				8 		///< How many schedules will be stored in system.
#define MAX_FILE_NAME			128		///< Maximum length of file name.

#define ACOUNT_NUM				16 		///< How many acounts which are stored in system.
#define USER_LEN				32 		///< Maximum of acount username length.
#define PASSWORD_LEN			16 		///< Maximum of acount password length.
#define IP_STR_LEN				20		///< IP string length

#define MOTION_BLK_LEN			(4) 	///< Motion block size

#define OSD_MSG_FALSE			(0)
#define OSD_MSG_TRUE			(1)
#define OSD_PARAMS_UPDATED		(4)

/*
 The following flags is defined for alarmstatus.
 Modify these sould also check where the alarmstatus is used.
 Be sure that you also check your web UI coding.
*/
#define FLG_UI_EXT 				(1 << 0)
#define FLG_UI_MOTION 			(1 << 1)
#define FLG_UI_ETH 				(1 << 2)
#define FLG_UI_AUDIO			(1 << 3)
#define FLG_UI_RECORD 			(1 << 4)
#define FLG_UI_AVI 				(1 << 5)
#define FLG_UI_JPG 				(1 << 6)

#define LOGIN_TIMEOUT		1800 	/* Set for 30 min */

#define INVALID_IP			0
#define ETH_NAME			"eth0"
#define USB_PATH			"/proc/usb_appro"
#define USB_CONNECT_PATH	"/proc/usb_connect"
#define USB_CFG_FILE		"/mnt/ramdisk/NET.TXT"

/* High : on */
#define LED_ON					1
#define LED_OFF					0
#define GIO_LED_1				62
#define GIO_LED_2				63
#define GIO_LED_3				64
#define GIO_TV_CONNECT			7
#define GIO_ALARMOUT 			81
#define GIO_ALARMIN 			82
#define GIO_ALARMRESET 			35
#define GIO_ALARMRESET_OLD 		80
#define GIO_RESET_TO_DEFAULT	79 /*Low triger*/
#define GIO_DHCP				84

#define H264_1_PORTNUM	(8557)
#define H264_2_PORTNUM	(8556)
#define MPEG4_1_PORTNUM	(8553)
#define MPEG4_2_PORTNUM	(8554)
#define MJPEG_PORTNUM	(8555)

typedef enum{
	CHIP_NONE = -1,
	CHIP_DM365 = 0,
	CHIP_DM368
}CHIP_DM36X;

typedef enum{
	H264_CODEC = 0,
	MPEG4_CODEC,
	MJPEG_CODEC,
	NO_CODEC
}CODEC_TYPE;

typedef enum{
	BINNING = 0,
	SKIPPING,
	WINDOW
}SENSOR_MODE;

typedef enum{
	AUTO = 0,
	CUSTOM,
	SVC
}ENCPRESET_MODE;

/**
* @brief Infomation of network status and user settings.
*/
typedef struct
{
	struct in_addr	ip; ///< IP address in static IP mode
	struct in_addr	netmask; ///< netmask in static IP mode
	struct in_addr	gateway; ///< gateway in static IP mode
	struct in_addr	dns; ///< DNS IP in static IP mode
	__u16		http_port; ///< HTTP port in web site.
	__u16		https_port; ///< HTTP port in web site.
	int			dhcp_enable; ///< current DHCP status.
	int			dhcp_config; ///< DHCP config
	char		ntp_server[MAX_DOMAIN_NAME+1]; ///< NTP server FQDN
	__u8		ntp_timezone; ///< current time zone
	__u8		ntp_frequency; ///< NTP server query frequence
	__u8		time_format; ///< time display format
	__u8		daylight_time; ///< daylight saving time mode
	__u8		MAC[MAC_LENGTH]; ///< hardware MAC address
	__u8		mpeg4resolution; ///< MPEG4_1 resolution
	__u8		mpeg42resolution; ///< MPEG4_2 resolution
	__u8		liveresolution; ///< live resolution
	__u8		mpeg4quality; ///< MPEG4 quality
	__u8		supportmpeg4; ///< support MPEG4
	__u8		imageformat; ///< image format
	__u8		imagesource; ///< image source (NTSC/PAL)
	__u8		defaultstorage; ///< default storage
	char		defaultcardgethtm[MAX_NAME_LENGTH]; ///< default card get page
	char		brandurl[MAX_NAME_LENGTH]; ///< brand URL
	char		brandname[MAX_NAME_LENGTH]; ///< brand name
	__u8		supporttstamp; ///< support time stamp
	__u8		supportmotion; ///< support motion detection
	__u8		supportwireless; ///< support wireless
	__u8		serviceftpclient; ///< ftp client support
	__u8		servicesmtpclient; ///< smtp client support
	__u8		servicepppoe; ///< PPPoE support
	__u8		servicesntpclient; ///< sntp client support
	__u8		serviceddnsclient; ///< DNS client support
	__u8		supportmaskarea; ///< mask area support
	__u8		maxchannel; ///< max channel
	__u8		supportrs485; ///< RS485 support
	__u8		supportrs232; ///< RS232 support
	__u8		layoutnum; ///< layout No.
	__u8		supportmui; ///< support MUI
	__u8		mui; ///< MUI
	__u8		supportsequence; ///< support sequence
	__s8		quadmodeselect; ///< quadmode select
	__u8		serviceipfilter; ///< service IP filter
	__u8		oemflag0; ///< OEM flag
	__u8		supportdncontrol; ///< Daynight control support
	__u8		supportavc; ///< avc support
	__u8		supportaudio; ///< support audio
	__u8		supportptzpage; ///< PTZ support
	__u8		multicast_enable; ///< multicast enable
} Network_Config_Data;

/**
* @brief Infomation about ftp configuration.
*/
typedef struct
{
	char		servier_ip[MAX_DOMAIN_NAME+1]; ///< FTP server address
	char		username[USER_LEN]; ///< FTP login username
	char		password[PASSWORD_LEN]; ///< FTP login password
	char		foldername[MAX_FILE_NAME]; ///< FTP upload folder
	int			image_acount; ///< Image count
	int			pid; ///< PID
	__u16		port; ///< FTP port
 	__u8		rftpenable; ///< RFTP enable
	__u8        ftpfileformat; ///< file format
} Ftp_Config_Data;

/**
* @brief Infomation about SMTP configuration.
*/
typedef struct
{
	char		servier_ip[MAX_DOMAIN_NAME+1]; ///< SMTP server address
	__u16		server_port; ///< SMTP server port
	char		username[USER_LEN]; ///< SMTP username
	char		password[PASSWORD_LEN]; ///< SMTP password
	__u8		authentication; ///< SMTP authentication
	char		sender_email[MAX_NAME_LENGTH]; ///< sender E-mail address
	char		receiver_email[MAX_NAME_LENGTH]; ///< receiver E-mail address
	char		CC[MAX_NAME_LENGTH]; ///< CC E-mail address
	char		subject[MAX_NAME_LENGTH]; ///< mail subject
	char		text[MAX_NAME_LENGTH]; ///< mail text
	__u8		attachments; ///< mail attachment
	__u8		view; ///< smtp view
	__u8		asmtpattach; ///< attatched file numbers
	__u8        attfileformat; ///< attachment file format
} Smtp_Config_Data;

/**
* @brief custom data structure for time.
*/
typedef struct{
	__u8	nHour;	///< Hour from 0 to 23.
	__u8	nMin;	///< Minute from 0 to 59.
	__u8	nSec;	///< Second from 0 to 59.
} Time_t;

/**
* @brief custom data structure for schedule entry.
*/
typedef struct{
	__u8	bStatus;	///< schedule status ( 0:disable 1:enable }
	__u8	nDay;		///< schedule day of week (1:Mon 2:Tue 3:Wed 4:Thr 5:Fri 6:Sat 7:Sun 8:Everyday)
	Time_t	tStart;		///< schedule start time
	Time_t	tDuration;	///< schedule duration
} Schedule_t;

/**
* @brief IPCAM configuration data.
*/
typedef struct
{
	char			title[MAX_LANCAM_TITLE_LEN+1];	///< camera title
	__u8			nWhiteBalance;					///< white balance mode
	__u8			nDayNight;						///< Daynight mode
	__u8			nBinning;						///< binning/skipping setting
	__u8			nBacklightControl;				///< backlight control setting
	__u8			nBackLight;						///< backlight value
	__u8			nBrightness;					///< brightness value
	__u8			nContrast;						///< contrast value
	__u8			nSaturation;					///< saturation value
	__u8			nSharpness;						///< sharpness value
	__u8            lostalarm;						///< ethernet lost alarm
	__u8			bSdaEnable;						///< alarm save into SD card
	__u8			bAFtpEnable;					///< alarm upload via FTP
	__u8			bASmtpEnable;					///< alarm upload via SMTP
	__u8 			nStreamType;
	__u8			nVideocodecmode;				///< selected codec mode
	__u8			nVideocombo;
	__u8			nVideocodecres;					///< selected codec resolution
	__u8			nRateControl;					///< bitrate control setting
	__u8			nFrameRate1;					///< MPEG4_1 frame rate
	__u8			nFrameRate2;					///< MPEG4_2 frame rate
	__u8			nFrameRate3;					///< JPEG frame rate
	__u8			njpegquality; 					///< jpeg quality
	int				nMpeg41bitrate;					///< MPEG4_1 bitrate
	int				nMpeg42bitrate;					///< MPEG4_2 bitrate
	int				Mpeg41Xsize;					///< MPEG4_1 horizontal resolution
	int				Mpeg41Ysize;					///< MPEG4_1 vertical resolution
	int				Mpeg42Xsize;					///< MPEG4_2 horizontal resolution
	int				Mpeg42Ysize;					///< MPEG4_2 vertical resolution
	int				JpegXsize;						///< JPEG horizontal resolution
	int				JpegYsize;						///< JPEG vertical resolution
	int				Avc1Xsize;						///< H264_1 horizontal resolution
	int				Avc1Ysize;						///< H264_1 vertical resolution
	int				Avc2Xsize;						///< H264_2 horizontal resolution
	int				Avc2Ysize;						///< H264_2 vertical resolution
	__u8			Supportstream1;					///< support stream1 (JPEG)
	__u8			Supportstream2;					///< support stream2 (MPEG4_1)
	__u8			Supportstream3;					///< support stream3 (MPEG4_2)
	__u8			Supportstream4;					///< support stream4
	__u8			Supportstream5;					///< support stream5 (AVC_1)
	__u8			Supportstream6;					///< support stream6 (AVC_2)
	__u8			nAlarmDuration;					///< alarm duration
	__u8			nAEWswitch;						///< 2A engine selection
	__u8			gioinenable;					///< GIO input enable
	__u8			giointype;						///< GIO input type
	__u8			giooutenable;					///< GIO output enable
	__u8			gioouttype;						///< GIO output type
	__u8            tstampenable;					///< enable timestamp
	__u8            aviduration;					///< avi record duration
	__u8			aviformat;						///< avirecord format.
	__u16			alarmstatus;					///< Current alarm status.
	__u8			mirror; 						///< mirroring
	__u8		    AdvanceMode; 					    ///< video stabilization (ON/OFF)
	__u8			democfg;						///< demo config
	__u8			regusr[MAX_LANCAM_TITLE_LEN+1];	///< register user name
	__u8			osdstream;						///< register user name
	__u8			osdwinnum;						///< register user name
	__u8			osdwin;							///< register user name
	__u8			osdtext[OSDTEXTLEN];			///< register user name
	__u8			nAEWtype;						///< 2A engine selection
	__u8			histogram;						///< Histogram switch ON/OFF
	__u8			dynRange;						///< Dynamic Range switch ON/OFF
	__u8			maxexposure;
	__u8			maxgain;
	__u8            dateformat;
	__u8            tstampformat;
	__u8            dateposition;
	__u8            timeposition;
	__u8			rs485config;
	__u8			nClickSnapFilename[OSDTEXTLEN];	///< click snap filename
	__u8			nClickSnapStorage;				///< click snap storage setting
	__u8			nRateControl1;					///< stream1 bitrate control setting
	__u8			nRateControl2;					///< stream2 bitrate control setting
	__u8			nEncryptVideo;   				///< encrypt video setting (ON/OFF)
	__u8		    nLocalDisplay; 			        ///< local display video enable/disable
	__u8            nAlarmEnable;                    ///< alarm enable/disable setting
	__u8		    nExtAlarm; 			            ///< external trigger alarm
	__u8		    nAlarmAudioPlay; 			    ///< alarm audio play enable/disable
	__u8		    nAlarmAudioFile; 			    ///< alarm audio file
	__u8		    nScheduleRepeatEnable; 			///< schedule record repeat enable/disable
	__u8			nScheduleNumWeeks;   		    ///< scheduled number of weeks
	__u8		    nScheduleInfiniteEnable; 		///< schedule infinite times enable/disable
	__u8			alarmlocal;
	__u8			recordlocal;
	__u8			expPriority;
	__u8			codectype1;
	__u8			codectype2;
	__u8			codectype3;
	Schedule_t		aSchedules[SCHDULE_NUM];		///< schedule data
	int				schedCurDay;
	int				schedCurYear;
	__u8			reloadFlag;
	__u8			chipConfig;
	__u8			modelname[OSDTEXTLEN];
	int				frameRateVal1;
	int				frameRateVal2;
	int				frameRateVal3;
	__u8			dummy;							///< dummy for end of config
	Network_Config_Data	net;						///< network status and user settings
	__u8			linearwdr;
	int			flickerdetect;
} Lancam_Config_Data;

/**
* @brief SD card configuration data.
*/
typedef struct
{
   __u8			sdfileformat;	///< file format saved into SD card
   __u8			sdrenable;		///< enable SD card recording
   __u8			sdinsert;		///< SD card inserted
}Sdcard_Config_Data;

/**
* @brief IPCAM user account data.
*/
typedef struct
{
	char	user[USER_LEN];			///< username
	char	password[PASSWORD_LEN];	///< password
	__u8	authority;				///< user authority
}Acount_t;

/**
* @brief motion detection configuration data.
*/
typedef struct
{
  __u8      motionenable;					///< motion detection enable
  __u8      motioncenable;					///< customized sensitivity enable
  __u8      motionlevel;					///< predefined sensitivity level
  __u8      motioncvalue;					///< customized sensitivity value
  __u8	 	motionblock[MOTION_BLK_LEN];	///< motion detection block data
}Motion_Config_Data;

typedef struct
{
  __u8      audioON;
  __u8      audioenable;
  __u8      audiomode;				///< motion detection enable
  __u8      audioinvolume;				///< customized sensitivity enable
  __u8		codectype;
  __u8      samplerate;				///< predefined sensitivity level
  __u8      bitrate;				///< customized sensitivity value
  __u32     bitrateValue;			///< customized sensitivity value
  __u8 		alarmlevel;
  __u8 		alarmON;
  __u8	 	audiooutvolume;	        ///< motion detection block data
  __u8      audiorecvenable;    	///< audio receiver enable
  __u8      audioServerIp[MAX_NAME_LENGTH];        ///< audio server ip address
}Audio_Config_Data;

typedef struct
{
	__u8			dstampenable;					///< stream1 date stamp enable (ON/OFF)
	__u8			tstampenable;   				///< stream1 time stamp enable (ON/OFF)
	__u8			nLogoEnable;					///< stream1 logo enable (ON/OFF)
	__u8			nLogoPosition;   				///< stream1 logo position
	__u8			nTextEnable;					///< stream1 text enable (ON/OFF)
	__u8			nTextPosition;   				///< stream1 text position
	__u8			overlaytext[OSDTEXTLEN];			///< register user name
    __u8            nDetailInfo;
}OSD_config;

typedef struct
{
	__u32 ipRatio;
	__u8 fIframe;
	__u8 qpInit;
	__u8 qpMin;
	__u8 qpMax;
	__u8 meConfig;
	__u8 packetSize;
	__u8 unkown1;
	__u8 unkown2;
}CodecAdvParam;

typedef struct _ROI_Param{
	__u32 startx;
	__u32 starty;
	__u32 width;
	__u32 height;
}ROI_Param;

typedef struct _CodecROIParam{
	__u32 		numROI;
  	__u32     	prevwidth;
  	__u32     	prevheight;
	ROI_Param	roi[3];
}CodecROIParam;

/**
* @brief motion detection configuration data.
*/
typedef struct
{
  __u8	    fdetect; 					    ///< facedetect (ON/OFF)
  __u32     startX;
  __u32     startY;
  __u32     width;
  __u32     height;
  __u32     prevwidth;
  __u32     prevheight;
  __u8      fdconflevel;
  __u8      fddirection;
  __u8      frecog;
  __u8	    frconflevel;
  __u8	    frdatabase;
  __u8      pmask;
  __u8      maskoption;
}Face_Config_Data;

typedef struct
{
  __u16     width;
  __u16     height;
  __u16		portnum;
  __u8		name[MAX_STREAM_NAME];
  __u8 		portname[MAX_STREAM_NAME];
}StreamParam;

/**
* @brief structure for PTZ control data.
*/
typedef struct
{
  char      ptzdata[300];
}Ptz_Data;


/**
* @brief PTZ configuration data.
*/
typedef struct
{
  char      ptzzoomin[6];	///< zoom-in
  char      ptzzoomout[7];	///< zoom-out
  char      ptzpanup[2];	///< pan-up
  char      ptzpandown[4];	///< pan-down
  char	ptzpanleft[4];		///< pan-left
  char	ptzpanright[5];		///< pan-right
}Ptz_Config_Data;

/**
* @brief event log data structure.
*/
typedef struct LogEntry_t{
	char event[50];		///< event description
	struct tm time;		///< event log time
}LogEntry_t;

/**
* @brief user settings structure.
*/
typedef struct{
	char id[USER_LEN];
	char password[PASSWORD_LEN];
	char remote_ip[IP_STR_LEN];
}login_data_t;

typedef struct{
	char user_id[USER_LEN];
	int	authority;
}get_user_authority_t;

typedef struct{
	char	user_id[USER_LEN];
	char	password[PASSWORD_LEN];
	unsigned char	authority;
}add_user_t;

/**
* @brief system info main data structure.
*/
typedef struct SysInfo{
	unsigned int relUniqueId;			///< IPCAM device type
	Acount_t	acounts[ACOUNT_NUM];	///< user account data
	unsigned short DeviceType;			///< IPCAM device type
	Motion_Config_Data motion_config;	///< motion detection configuration data
	Face_Config_Data  face_config;
	Audio_Config_Data audio_config;
	OSD_config osd_config[MAX_OSD_STREAMS];
	CodecAdvParam codec_advconfig[MAX_CODEC_STREAMS];
	CodecROIParam codec_roiconfig[MAX_CODEC_STREAMS];
	StreamParam stream_config[MAX_CODEC_STREAMS];
	Ftp_Config_Data ftp_config;			///< IPCAM FQDN
	Smtp_Config_Data smtp_config;		///< ftp configuration data
	Sdcard_Config_Data sdcard_config;	///< SD card configuration data
	Lancam_Config_Data lan_config;		///< IPCAM configuration data
	Ptz_Config_Data ptz_config;			///< PTZ configuration data
	LogEntry_t	tCurLog;				///< event log
}SysInfo;

#endif /* __SYS_ENV_TYPE_H__ */
