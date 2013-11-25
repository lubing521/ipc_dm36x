/* ===========================================================================
* @file file_mng.c
*
* @path $(IPNCPATH)\sys_server\src\
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <file_mng.h>
#include <sys_env_type.h>
#include <system_default.h>
#include <ipnc_ver.h>

#ifdef __FILE_DEBUG
#define DBG(fmt, args...) fprintf(stdout, "FileMng: Debug " fmt, ##args)
#else
#define DBG(fmt, args...)
#endif

#define ERROR(fmt,args...)	printf("FileMng: Error " fmt, ##args)

typedef struct LogData_t{
	LogEntry_t tLogData;
	struct LogData_t* pNext;
}LogData_t;

static LogData_t* gLogHead = NULL;

static SysInfo SysInfoDefault =
{
	REL_UNIQUE_ID,
	ACOUNT_DEFAULT,
	DEVICE_TYPE_DEFAULT,
  	{
		MOTIONENABLE_DEFAULT,
		MOTIONCENABLE_DEFAULT,
		MOTIONLEVEL_DEFAULT,
		MOTIONCVALUE_DEFAULT,
		MOTIONBLOCK_DEFAULT
  	},
	/* face_config */
	{
		FDETECT_DEF,
		0,
		0,
		1280,
		720,
		1280,
		720,
		75,
		0,
		0,
		75,
		1,
		0,
		0
	},
	/* audio_config */
	{
		0,
		0,
		2,
		75,
		0,
		0,
		0,
		64000,
		50,
		0,
		75,
		0,
		"192.168.1.1"
	},
	/* osd_config */
	{
		{
			0,
			0,
			0,
			0,
			0,
			1,
			"text1",
			0
		},
		{
			0,
			0,
			0,
			0,
			0,
			1,
			"text2",
			0
		},
		{
			0,
			0,
			0,
			0,
			0,
			1,
			"text3",
			0
		}
	},
	/* codec_advconfig */
	{
		{
			30,
			0,
			28,
			1,
			42,
			0,
			100,
			0,
			0
		},
		{
			30,
			0,
			28,
			1,
			42,
			0,
			100,
			0,
			0
		},
		{
			30,
			0,
			28,
			1,
			42,
			0,
			100,
			0,
			0
		}
	},
	/* codec_roiconfig */
	{
		{
			0,
			1280,
			720,
			{
				{
					0,
					0,
					0,
					0,
				},
				{
					0,
					0,
					0,
					0,
				},
				{
					0,
					0,
					0,
					0,
				},
			},
		},
		{
			0,
			640,
			352,
			{
				{
					0,
					0,
					0,
					0,
				},
				{
					0,
					0,
					0,
					0,
				},
				{
					0,
					0,
					0,
					0,
				},
			},
		},
		{
			0,
			320,
			192,
			{
				{
					0,
					0,
					0,
					0,
				},
				{
					0,
					0,
					0,
					0,
				},
				{
					0,
					0,
					0,
					0,
				},
			},
		},
	},
	{
		{
			1280,
			720,
			8557,
			"H264",
			"h264"
		},
		{
			640,
			352,
			8555,
			"JPEG",
			"mjpeg"
		},
		{
			320,
			192,
			8556,
			"H264",
			"h264"
		},
	},
	{
		FTP_SERVER_IP_DEFAULT,
		FTP_USERNAME_DEFAULT,
		FTP_PASSWORD_DEFAULT,
		FTP_FOLDERNAME_DEFAULT,
		FTP_IMAGEAMOUNT_DEFAULT,
		FTP_PID_DEFAULT,
		FTP_PORT_DEFAULT,
        RFTPENABLE_DEFAULT,
	    FTPFILEFORMAT_DEFAULT
	},
	{
		SMTP_SERVER_IP_DEFAULT,
		SMTP_SERVER_PORT_DEFAULT,
		SMTP_USERNAME_DEFAULT,
		SMTP_PASSWORD_DEFAULT,
		SMTP_AUTHENTICATION_DEFAULT,
		SMTP_SNEDER_EMAIL_DEFAULT,
		SMTP_RECEIVER_EMAIL_DEFAULT,
		SMTP_CC_DEFAULT,
		SMTP_SUBJECT_DEFAULT,
		SMTP_TEXT_DEFAULT,
		SMTP_ATTACHMENTS_DEFAULT,
		SMTP_VIEW_DEFAULT,
		ASMTPATTACH_DEFAULT,
		ATTFILEFORMAT_DEFAULT
	},
	{
		SDFILEFORMAT_DEFAULT,
		SDRENABLE_DEFAULT,
		SD_INSERT_DEFAULT
    },
	{
		TITLE_DEFAULT,
		WHITEBALANCE_DEFAULT,
		DAY_NIGHT_DEFAULT,
		BINNING_DEFAULT,
		BACKLIGHT_CONTROL_DEFAULT,
		BACKLIGHT_VALUE_DEFAULT,
		BRIGHTNESS_DEFAULT,
		CONTRAST_DEFAULT,
		SATURATION_DEFAULT,
		SHARPNESS_DEFAULT,
		LOSTALARM_DEFAULT,
		SDAENABLE_DEFAULT,
		AFTPENABLE_DEFAULT,
		ASMTPENABLE_DEFAULT,
		STREAMTYPE_DEFAULT,
		VCODECMODE_DEFAULT,
		VCODECCOMBO_DEFAULT,
		VCODECRES_DEFAULT,
		RATECONTROL_DEFAULT,
		FRAMERATE1_DEFAULT,
		FRAMERATE2_DEFAULT,
		FRAMERATE3_DEFAULT,
		JPEGQUALITY_DEF,
		MPEG41BITRATE_DEFAULT,
		MPEG42BITRATE_DEFAULT,
		MPEG41XSIZE_DEFAULT,
		MPEG41YSIZE_DEFAULT,
		MPEG42XSIZE_DEFAULT,
		MPEG42YSIZE_DEFAULT,
		JPEGXSIZE_DEFAULT,
		JPEGYSIZE_DEFAULT,
		MPEG41XSIZE_DEFAULT,
		MPEG41YSIZE_DEFAULT,
		MPEG42XSIZE_DEFAULT,
		MPEG42YSIZE_DEFAULT,
		SUPPORTSTREAM1_DEFAULT,
		SUPPORTSTREAM2_DEFAULT,
		SUPPORTSTREAM3_DEFAULT,
		SUPPORTSTREAM4_DEFAULT,
		SUPPORTSTREAM5_DEFAULT,
		SUPPORTSTREAM6_DEFAULT,
		ALARMDURATION_DEFAULT,
		AEW_SWITCH_DEFAULT,
		GIOINENABLE_DEFAULT,
		GIOINTYPE_DEFAULT,
		GIOOUTENABLE_DEFAULT,
		GIOOUTTYPE_DEFAULT,
		TSTAMPENABLE_DEFAULT,
		AVIDURATION_DEFAULT,
		AVIFORMAT_DEFAULT,
		ALARMSTATUS_DEFAULT,
      	MIRROR_DEF,
      	ADVMODE_DEF,
      	DEMOCFG_DEF,
      	REGUSR_DEF,
      	OSDSTREAM_DEF,
      	OSDWINNUM_DEF,
      	OSDWIN_DEF,
      	OSDTEXT_DEF,
		AEW_TYPE_DEFAULT,
		HISTOGRAM_DEF,
		DYNRANGE_DEF,
		0,
		0,
		DATEFORMAT_DEFAULT,
		TSTAMPFORMAT_DEFAULT,
		1,
		1,
		0,
		"snap",
		0,
		RATECONTROL_DEFAULT,
		RATECONTROL_DEFAULT,
		0,
		LOCALDISPLAY_DEFAULT,
		1,
 		0,
		0,
		0,
		0,
		52,
		1, /* Enable infinite schedule */
		0,
		0,
		0,
		H264_CODEC,
		H264_CODEC,
		H264_CODEC,
		SCHEDULE_DEFAULT,
     	0,
     	0,
     	0,
     	0,
     	MODELNAME_DEFAULT,
     	MAX_FRAMERATE_DEFAULT,
     	MAX_FRAMERATE_DEFAULT,
     	MAX_FRAMERATE_DEFAULT,
     	DUMMY_DEF,
		{
			{IP_DEFAULT},
			{NETMASK_DEFAULT},
			{GATEWAY_DEAFULT},
			{DNS_DEFAULT},
			HTTP_PORT_DEFAULT,
			HTTPS_PORT_DEFAULT,
			DHCP_ENABLE_DEFAULT,
			DHCP_CONFIG_DEFAULT,
			NTP_SERVER_DEFAULT,
			NTP_TIMEZONE_DEFAULT,
			NTP_FREQUENCY_DEFAULT,
			TIME_FORMAT_DEFAULT,
			DAYLIGHT_DEFAULT,
			MAC_DEFAULT,
			MPEG4_RESOLUTION_DEF,
			MPEG4_RESOLUTION2_DEF,
			LIVE_RESOLUTION_DEF,
			MPEG4QUALITY_DEF,
			SUPPORTMPEG4_DEFAULT,
			IMAGEFORMAT_DEFAULT,
			IMAGESOURCE_DEFAULT,
			DEFAULTSTORAGE_DEFAULT,
			DEFAULTCARDGETHTM_DEFAULT,
			BRANDURL_DEFAULT,
			BRANDNAME_DEFAULT,
			SUPPORTTSTAMP_DEFAULT,
			SUPPORTMOTION_DEFAULT,
			SUPPORTWIRELESS_DEFAULT,
			SERVICEFTPCLIENT_DEFAULT,
			SERVICESMTPCLIENT_DEFAULT,
			SERVICEPPPOE_DEFAULT,
			SERVICESNTPCLIENT_DEFAULT,
			SERVICEDDNSCLIENT_DEFAULT,
			SUPPORTMASKAREA_DEFAULT,
			MAXCHANNEL_DEFAULT,
			SUPPORTRS485_DEFAULT,
			SUPPORTRS232_DEFAULT,
			LAYOUTNUM_DEFAULT,
			SUPPORTMUI_DEFAULT,
			MUI_DEFAULT,
			SUPPORTSEQUENCE_DEFAULT,
			QUADMODESELECT_DEFAULT,
			SERVICEIPFILTER_DEFAULT,
			OEMFLAG0_DEFAULT,
			SUPPORTDNCONTROL_DEFAULT,
			SUPPORTAVC_DEFAULT,
			SUPPORTAUDIO_DEFAULT,
			SUPPORTPTZPAGE_DEFAULT,
			MULTICAST_ENABLE_DEFAULT
      	},
	},
	{
		PTZZOOMIN_DEFAULT,
		PTZZOOMOUT_DEFAULT,
		PTZPANUP_DEFAULT,
		PTZPANDOWN_DEFAULT,
		PTZPANLEFT_DEFAULT,
		PTZPANRIGHT_DEFAULT
  	},
	CUR_LOG_DEFAULT
};

/**
 * @brief	check magic number
 * @param	fp [I ]file pointer
 * @return	error code : SUCCESS(0) or FAIL(-1)
 */
int check_magic_num(FILE *fp)
{
	int ret;
	unsigned long MagicNum;
	if(fread(&MagicNum, 1, sizeof(MagicNum), fp) != sizeof(MagicNum)){
		ret = FAIL;
	} else {
		if(MagicNum == MAGIC_NUM){
			ret = SUCCESS;
		} else {
			ret = FAIL;
		}
	}
	return ret;
}
/**
 * @brief	Add new log event to tail
 * @return	error code : SUCCESS(0) or FAIL(-1)
 */
int AddLogToTail(LogEntry_t *pLog)
{
	LogData_t *pLogData, *pTail = NULL;
	DBG("pLog->tLogData.event=%s\n", pLog->event);
	DBG("pLog->tLogData.time=%s\n", asctime(& pLog->time));
	pLogData = (LogData_t*) malloc(sizeof(LogData_t));
	if(pLogData == NULL){
		DBG("No enough memory\n");
		if(gLogHead == NULL)
			return FAIL;
		/* Do Nothing if no more memory */
	} else {
		memcpy(&pLogData->tLogData, pLog, sizeof(LogEntry_t));
		pLogData->pNext = NULL;
		if(gLogHead == NULL){
			gLogHead = pLogData;
		} else {
			for(pTail = gLogHead;pTail->pNext != NULL;)
				pTail = pTail->pNext;
			pTail->pNext = pLogData;
		}
	}
	return SUCCESS;
}
/**
 * @brief	Add new log event
 * @return	error code : SUCCESS(0) or FAIL(-1)
 */
int AddLog(LogEntry_t *pLog)
{
	LogData_t *pLogData, *pFrontData = NULL;
	DBG("pLog->tLogData.event=%s\n", pLog->event);
	DBG("pLog->tLogData.time=%s\n", asctime(& pLog->time));
	pLogData = (LogData_t*) malloc(sizeof(LogData_t));
	if(pLogData == NULL){
		DBG("No enough memory\n");
		if(gLogHead == NULL)
			return FAIL;
		/* If no more memory, replace the oldest one with current. */
		pLogData = gLogHead;
		while(pLogData->pNext != NULL){
			pFrontData = pLogData;
			pLogData = pLogData->pNext;
		}
		memcpy(&pLogData->tLogData, pLog, sizeof(LogEntry_t));
		if(pFrontData != NULL){
			pFrontData->pNext = NULL;
			pLogData->pNext = gLogHead;
			gLogHead = pLogData;
		}
	} else {
		memcpy(&pLogData->tLogData, pLog, sizeof(LogEntry_t));
		pLogData->pNext = gLogHead;
		gLogHead = pLogData;
	}
	DBG("gLogHead->tLogData.event=%s\n", gLogHead->tLogData.event);
	DBG("gLogHead->tLogData.time=%s\n", asctime(& gLogHead->tLogData.time));
	return SUCCESS;
}
/**
 * @brief	Show all system log
 * @return
 */
void ShowAllLog()
{
	LogData_t *ptr;
	for(ptr = gLogHead;ptr != NULL;ptr = ptr->pNext){
		fprintf(stderr, "Event:%s\n", ptr->tLogData.event);
		fprintf(stderr, "Time:%s\n", asctime(&ptr->tLogData.time));
	}
}
/**
 * @brief	Clean system log
 * @return
 */
void CleanLog()
{
	LogData_t *ptr;
	while(gLogHead != NULL){
		ptr = gLogHead;
		gLogHead = gLogHead->pNext;
		free(ptr);
	}
}
/**
 * @brief	read log from log file
 * @return	error code : SUCCESS(0) or FAIL(-1)
 */
int ReadLog()
{
	FILE *fp;
	char Buffer[LOG_ENTRY_SIZE];
	int ret = SUCCESS, count = 0;
	if((fp = fopen(LOG_FILE, "rb")) == NULL){
		/* log file not exist */
		ret = FAIL;
	} else {
		if(check_magic_num(fp) == SUCCESS){
			while(count < NUM_LOG_PER_PAGE * MAX_LOG_PAGE_NUM)
				if(fread(Buffer, 1, LOG_ENTRY_SIZE,fp) != LOG_ENTRY_SIZE){
					break;
				} else {
					if(AddLogToTail((LogEntry_t *)Buffer) != SUCCESS){
						ret = FAIL;
						break;
					}
					count++;
				}
		} else {
			ret = FAIL;
		}
		fclose(fp);
	}
	return ret;
}
/**
 * @brief	create log file
 * @param	"int nPageNum" : [IN]log page number
 * @param	"int nItemIndex" : [IN]log index in that page
 * @return	LogEntry_t*: log data
 */
LogEntry_t* GetLog(int nPageNum, int nItemIndex)
{
	LogData_t* pLog;
	int count, index = nPageNum * NUM_LOG_PER_PAGE + nItemIndex;
	if(nPageNum < 0 || nItemIndex < 0)
		return NULL;
	for(count = 0, pLog = gLogHead;(count < index) && (pLog != NULL);count ++)
		pLog = pLog->pNext;
	return &pLog->tLogData;
}
/**
 * @brief	create log file
 * @param	name [I ]File name to create in nand.
 * @param	pLogData  [I ]pointer to log list
 * @return	error code : SUCCESS(0) or FAIL(-1)
 */
int create_log_file(char *name, LogData_t*pLogData){
	FILE *fp;
	int ret, count = 0;
	unsigned long MagicNum = MAGIC_NUM;
	if((fp = fopen(name, "wb")) == NULL){
		ERROR("Can't create log file\n");
		ret = FAIL;
	} else {
		if(fwrite(&MagicNum, 1, sizeof(MagicNum), fp) != sizeof(MagicNum)){
			ERROR("Writing Magic Number fail\n");
			ret = FAIL;
		} else {
			ret = SUCCESS;
			while(pLogData != NULL &&
					count < NUM_LOG_PER_PAGE * MAX_LOG_PAGE_NUM)
				if(fwrite(&pLogData->tLogData, 1, LOG_ENTRY_SIZE, fp) !=
						LOG_ENTRY_SIZE){
					ERROR("Writing log fail\n");
					ret = FAIL;
					break;
				} else {
					count++;
					pLogData = pLogData->pNext;
				}
		}
		fclose(fp);
	}
	return ret;
}
/**
 * @brief	create system file
 * @param	name [I ]File name to create in nand.
 * @param	Global [I ]Pointer to System information
 * @return	error code : SUCCESS(0) or FAIL(-1)
 */
int create_sys_file(char *name, void *Global){
	FILE *fp;
	int ret;
	unsigned long MagicNum = MAGIC_NUM;
	if((fp = fopen(name, "wb")) == NULL){
		ERROR("Can't create system file\n");
		ret = FAIL;
	} else {
		if(fwrite(&MagicNum, 1, sizeof(MagicNum), fp) != sizeof(MagicNum)){
			ERROR("Writing Magic Number fail\n");
			ret = FAIL;
		} else {
			if(fwrite(Global, 1, SYS_ENV_SIZE, fp) != SYS_ENV_SIZE){
				ERROR("Writing global fail\n");
				ret = FAIL;
			} else {
				ret = SUCCESS;
			}
		}
		fclose(fp);
	}
	return ret;
}

/**
 * @brief	read SysInfo from system file
 * @param	"void *Buffer" : [OUT]buffer to store SysInfo
 * @return	error code : SUCCESS(0) or FAIL(-1)
 */
int ReadGlobal(void *Buffer)
{
	FILE *fp;
	int ret;
	if((fp = fopen(SYS_FILE, "rb")) == NULL){
		/* System file not exist */
		ret = FAIL;
	} else {
		if(check_magic_num(fp) == SUCCESS){
			if(fread(Buffer, 1, SYS_ENV_SIZE,fp) != SYS_ENV_SIZE){
				ret = FAIL;
			} else {
				ret = SUCCESS;
			}
		} else {
			ret = FAIL;
		}
		fclose(fp);
	}
	return ret;
}

/**
 * @brief	write SysInfo to system file
 * @param	"void *Buffer" : [IN]buffer of SysInfo
 * @return	error code : SUCCESS(0) or FAIL(-1)
 */
int WriteGlobal(void *Buffer)
{
	int ret;
	ret = create_log_file(LOG_FILE, gLogHead);
	return ret | create_sys_file(SYS_FILE, Buffer);
}

/**
 * @brief	file manager initialization
 * @param	ShareMem [O ]Pointer to share memory where system information will
 be stored.
 * @return	error code : SUCCESS(0) or FAIL(-1)
 */
int FileMngInit(void *ShareMem)
{
	int ret;
	DBG("Global value size:%d\n", SYS_ENV_SIZE);
	ret = ReadGlobal(ShareMem);
	if(ret == FAIL){
		ret = create_sys_file(SYS_FILE, &SysInfoDefault);
		if(ret == SUCCESS)
			memcpy(ShareMem, &SysInfoDefault, SYS_ENV_SIZE);
		else
			ERROR("Initialize fail (FileMgnInit)\n");
	}
	if(ret == SUCCESS){
		ret = ReadLog();
		if(ret == FAIL)
			ret = create_log_file(LOG_FILE, NULL);
#ifdef __FILE_DEBUG
		else
			ShowAllLog();
#endif
	}
	return ret;
}

/**
 * @brief	file manager exit
 * @return	error code : SUCCESS(0) or FAIL(-1)
 */
int FileMngExit()
{
	CleanLog();
	return 0;
}

/**
 * @brief	file manager reset
 * @param	"void *ShareMem" : [IN]pointer to share memory
 * @return	error code : SUCCESS(0) or FAIL(-1)
 */
int FileMngReset(void *ShareMem)
{
	int ret;
	ret = create_sys_file(SYS_FILE, &SysInfoDefault);
	if(ret == SUCCESS)
		memcpy(ShareMem, &SysInfoDefault, SYS_ENV_SIZE);
	return ret;
}

/* clear system and access logs */
int ClearSystemLog()
{
    LogData_t *pLog = gLogHead;
    LogData_t *pNextLog,*pPrevLog = NULL;
    int logListCount = 0;

    while(pLog != NULL)
    {
		if(strstr(pLog->tLogData.event,"login") != NULL)
		{
			if(logListCount == 0)
			    gLogHead = pLog;

            pNextLog    = pLog->pNext;
            pLog->pNext = NULL;
            if(pPrevLog != NULL)
            {
			    pPrevLog->pNext = pLog;
			}
            pPrevLog = pLog;

			pLog = pNextLog; // goto next entry
			logListCount ++;
		    continue;
		}

	    pNextLog = pLog->pNext;
	    free(pLog);
	    pLog = pNextLog;
	}

	if(logListCount == 0)
	    gLogHead = NULL;

    return 0;
}

int ClearAccessLog()
{
    LogData_t *pLog = gLogHead;
    LogData_t *pNextLog,*pPrevLog = NULL;
    int logListCount = 0;

    while(pLog != NULL)
    {
		if(strstr(pLog->tLogData.event,"login") == NULL)
		{
			if(logListCount == 0)
			    gLogHead = pLog;

            pNextLog    = pLog->pNext;
            pLog->pNext = NULL;
            if(pPrevLog != NULL)
            {
			    pPrevLog->pNext = pLog;
			}
            pPrevLog = pLog;

			pLog = pNextLog; // goto next entry
			logListCount ++;
		    continue;
		}

	    pNextLog = pLog->pNext;
	    free(pLog);
	    pLog = pNextLog;
	}

	if(logListCount == 0)
	    gLogHead = NULL;

    return 0;
}
