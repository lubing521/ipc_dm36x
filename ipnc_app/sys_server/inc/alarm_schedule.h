/* ===========================================================================
* @path $(IPNCPATH)\sys_adm\system_server
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
* @file alarm_schedule.h
* @brief Alarm schedule
*/
#ifndef __ALARM_SCHEDULE_H__
#define __ALARM_SCHEDULE_H__
#include <sys_env_type.h>
/**
* @defgroup ALARM_SCHEDULE Alarm Schedule
* @{
*/
/**
* @brief Schedule Index

* This will be used to tell alarm schedule which event happened
*/
typedef enum {
	EXT_SCHEDULE=0,		///< Externerl trigger schedule index
	MOTION_SCHEDULE,	///< Motion detection schedule index
	ETH_SCHEDULE,		///< Ethernet lost schedule index
	AUDIO_SCHEDULE,		///< Audio Trigger schedule index
	ALARM_NUM			///< Total alarm schedule number
}ScheduleIndex_t;

int AddSchedule(ScheduleIndex_t index, struct tm *tmnow, SysInfo *pSysInfo);
int ResetSchedule();
int RemoveSchedule(ScheduleIndex_t index);
void *AlramScheduler(void *arg);
void AlarmScheduleExit();
int WaitAlarmStatusUpdated();
/** @} */
#endif
