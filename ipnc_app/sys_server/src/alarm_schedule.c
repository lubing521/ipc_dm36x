/* ===========================================================================
* @file alarm_schedule.c
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
#include <alarm_schedule.h>
#include <proc_alarm.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <avi_mng.h>
#include <file_msg_drv.h>
#include <file_dispatcher.h>
#include <jpg_mng.h>
#include <sem_util.h>

#define NON_STOP_TIME	-1
#define ALARM_SCHEDULE_MASK (FLG_UI_EXT | FLG_UI_MOTION | FLG_UI_ETH | FLG_UI_AUDIO)

#ifdef AS_DEBUG
#define A_DBG(fmt, args...)	fprintf(stderr, fmt, ##args)
#else
#define A_DBG(fmt, args...)
#endif
#define A_ERR(fmt, args...)	fprintf(stderr, fmt, ##args)

typedef struct AlarmSchedule_t{
	struct tm tStart;
	int nDuration;
}AlarmSchedule_t;

extern SemHandl_t hAlarmStatusSem;
extern char gbSD_WriteProtect;

AlarmSchedule_t gAlarmSchedules[ALARM_NUM];
int gAS_Run= 1;
int gAS_Pause = 0;
static int gAS_Updated = 0;
/**
* @brief Let alarm schedule leave.
*/
void AlarmScheduleExit()
{
	gAS_Run = 0;
}
/**
* @brief Add an alarm schedule

* Add an alarm schedule, and it will be executed for a period.
* @param index [I ] Index of alarm schedule.
* @param tmnow [I ] Current time.
* @param pSysInfo [I ] Pointer to system information.
* @retval 0 Success
* @retval -1 Fail
*/
int AddSchedule(ScheduleIndex_t index, struct tm *tmnow, SysInfo *pSysInfo)
{
	int aDurations[] = {10, 30, 60, 300, 600, NON_STOP_TIME};
	if(pSysInfo->lan_config.nAlarmDuration >=
			(sizeof(aDurations) / sizeof(aDurations[0]))){
		return -1;
	}
	gAlarmSchedules[index].nDuration = aDurations[pSysInfo->lan_config.nAlarmDuration];
	memcpy(&gAlarmSchedules[index].tStart, tmnow, sizeof(struct tm));
	return 0;
}
/**
* @brief Remove specific alarm schedule.
* @param index [I ] Index of alarm schedule.
* @retval 0 Success
*/
int RemoveSchedule(ScheduleIndex_t index)
{
	memset(&gAlarmSchedules[index], 0, sizeof(gAlarmSchedules[0]));
	return 0;
}
/**
* @brief Reset all alarm schedule.
*/
int ResetSchedule()
{
	memset(gAlarmSchedules, 0, sizeof(gAlarmSchedules));
	return 0;
}


/**
* @brief Check if now have schedule run.
* @param rawtime [I ] temp space to store current time.
* @return Status of current running schedule.
Bit 0 EXT_SCHEDULE
Bit 1 MOTION_SCHEDULE
Bit 2 ETH_SCHEDULE
Bit 3 AUDIO_SCHEDULE
* @retval 0 No schedule running.
*/
unsigned short HaveSchedule(time_t* rawtime)
{
	int i;
	unsigned short nAlarmStatus = 0;
	double differ;

	time ( rawtime );
	for(i = 0;i < ALARM_NUM;i++){
		if(gAlarmSchedules[i].nDuration == NON_STOP_TIME){
			nAlarmStatus |= (1 << i);
			continue;
		}
		if((differ = difftime(*rawtime, mktime(&gAlarmSchedules[i].tStart))) < 0)
			continue;
		if(differ < gAlarmSchedules[i].nDuration){
			nAlarmStatus |= (1 << i);
			continue;
		}
	}

	return nAlarmStatus;
}
/**
* @brief Main thread of alarm schedule.
* @param arg [I ] Not used.
* @return 0 on alarm schedule normal end.
*/
void *AlramScheduler(void *arg)
{
	time_t curTime;
	SysInfo* pSysInfo = GetSysInfo();
	int nflag = 0, bIsAviRun = 0, count = 0, temp;
	unsigned short nAlarmStatus;
	static unsigned short nLastAlarmStatus;
	nLastAlarmStatus = pSysInfo->lan_config.alarmstatus;
	while(gAS_Run){
		if(gAS_Pause)
			nAlarmStatus = 0;
		else
			nAlarmStatus = HaveSchedule(&curTime);
		if(nAlarmStatus){
			A_DBG("HaveSchedule\n");
			/* We'll call AviRun, JpgRun every 10 seconds. */
			if(count == 0){
				int isFTPAVI,isSDAVI,isSMTPAVI;
				int supportJPG=0,supportH264=0,supportH264cif=0,supportMpeg4=0,supportMpeg4cif=0,supportAVI=0;
				isFTPAVI=pSysInfo->ftp_config.ftpfileformat;
				isSDAVI=pSysInfo->sdcard_config.sdfileformat;
				isSMTPAVI=pSysInfo->smtp_config.attfileformat;
				supportJPG = pSysInfo->lan_config.Supportstream1;
				supportMpeg4 = pSysInfo->lan_config.Supportstream2;
				supportMpeg4cif = pSysInfo->lan_config.Supportstream3;
				supportH264 = pSysInfo->lan_config.Supportstream5;
				supportH264cif = pSysInfo->lan_config.Supportstream6;
				supportAVI = supportH264|supportH264cif|supportMpeg4|supportMpeg4cif;
				nflag =0;
				if(pSysInfo->sdcard_config.sdinsert<=1){
					bIsAviRun = 0;
					if(supportAVI&&pSysInfo->lan_config.bAFtpEnable&&isFTPAVI==0){
						nflag |= AVI_TO_FTP;
						bIsAviRun = 1;
					}
					if(supportAVI&&pSysInfo->lan_config.bASmtpEnable&&isSMTPAVI==0){
						nflag |= AVI_TO_SMTP;
						bIsAviRun = 1;
					}
					if(bIsAviRun){
						AviRun(0, nflag);
					}
					nflag = 0;
					/* Megapixel JPEG mode*/
					if(!supportAVI){
						if(pSysInfo->lan_config.bAFtpEnable)
							nflag |= JPG_TO_FTP;
						if(pSysInfo->lan_config.bASmtpEnable)
							nflag |= JPG_TO_SMTP;
					}else{
					if(isFTPAVI==1 &&pSysInfo->lan_config.bAFtpEnable)
						nflag |= JPG_TO_FTP;
					if(isSMTPAVI==1&&pSysInfo->lan_config.bASmtpEnable)
						nflag |= JPG_TO_SMTP;
					}
					if(nflag)
						JpgRun(nflag, 0 );
				} else {
					/* AVI no stop case */
					/* Megapixel JPEG mode*/
					if(!supportAVI){
						if(pSysInfo->lan_config.bSdaEnable)
							nflag |= JPG_TO_SD;
						if(pSysInfo->lan_config.bAFtpEnable)
							nflag |= JPG_TO_FTP;
						if(pSysInfo->lan_config.bASmtpEnable)
							nflag |= JPG_TO_SMTP;
					}else{
					if(isSDAVI==1&&pSysInfo->lan_config.bSdaEnable)
						nflag |= JPG_TO_SD;
					if(supportJPG&&pSysInfo->lan_config.bAFtpEnable)
						nflag |= JPG_TO_FTP;
					if(supportJPG&&pSysInfo->lan_config.bASmtpEnable)
						nflag |= JPG_TO_SMTP;
					}
					if(!nLastAlarmStatus){
						if(supportAVI&&pSysInfo->lan_config.bSdaEnable&&isSDAVI==0){
							AviRun(1, AVI_TO_SD);
							bIsAviRun = 1;
							A_DBG("AviRun in continuous mode\n");
						}
						if(nflag)
							JpgRun(nflag, 1 );
					}
				}
				if(nLastAlarmStatus != nAlarmStatus){
					/* Update alarmstatus in UI */
					if(SemWait(hAlarmStatusSem) == 0){
						temp = nAlarmStatus & ALARM_SCHEDULE_MASK;
						if(temp == 0)
							temp = FLG_UI_EXT;
						fSetAlarmStatus((pSysInfo->lan_config.alarmstatus &
							(~ALARM_SCHEDULE_MASK)) | temp);
						SemRelease(hAlarmStatusSem);
					} else
						A_ERR("Alarm status update fail ! \n");
				}
				count = 49;
			}/* end if(count == 0) */
			count --;
		}else if(nLastAlarmStatus){
			/* Condition at a schedule end. */
			count = 0;
			if(pSysInfo->sdcard_config.sdinsert==3){
				/* AVI no stop case */
				if(bIsAviRun){
					AviStop();
					A_DBG("AviStop\n");
					bIsAviRun = 0;
				}
				if(nflag){
					JpgStop(1);
					A_DBG("JpgStop\n");
				}
			}
			/* Update alarmstatus in UI */
			if(SemWait(hAlarmStatusSem) == 0){
				temp = nAlarmStatus & ALARM_SCHEDULE_MASK;
				if(temp == 0 && (nAlarmStatus & ~ALARM_SCHEDULE_MASK))
					temp = FLG_UI_EXT;
				fSetAlarmStatus((pSysInfo->lan_config.alarmstatus &
					(~ALARM_SCHEDULE_MASK)) | temp);
				SemRelease(hAlarmStatusSem);
			} else
				A_ERR("Alarm status update fail ! \n");
		}
		nLastAlarmStatus = nAlarmStatus;
		gAS_Updated = 1;
		usleep(200000);
	}

	return PROC_FAIL;
}

/**
* @brief Wait until AlarmStatus updated.
* @param arg [I ] Not used.
* @return 0 on success.
*/
int WaitAlarmStatusUpdated()
{
	int update;
	if(SemWait(hAlarmStatusSem) != 0)
		return -1;
	update = gAS_Updated = 0;
	if(SemRelease(hAlarmStatusSem) != 0){
		A_ERR("Can't release semaphore <hAlarmStatusSem>, Critical fail!!!\n");
		return -1;
	}
	while(update == 0){
		usleep(100000);
		if(SemWait(hAlarmStatusSem) == 0){
			update = gAS_Updated;
			if(SemRelease(hAlarmStatusSem) != 0){
				A_ERR("Can't release <hAlarmStatusSem>, Critical fail!!!\n");
				return -1;
			}
		}
		A_DBG("Update = %d\n", update);
	}

	return 0;
}
