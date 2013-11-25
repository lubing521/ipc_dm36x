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
* @file alarm_msg_drv.h
* @brief Alarm message driver
*/
#ifndef _ALARM_MSG_DRV_H_
#define _ALARM_MSG_DRV_H_
#include <Msg_Def.h>
#include <sys_env_type.h>
/**
* @defgroup ALARM_MSG_DRV Alarm message driver

* This is the communication interface of alarm manager. Every process needs a
*  message ID (which is defined in @ref Alarm_Msg_Def.h) to communicate with
* alarm manager. More detail please refer to (@ref ALARM_MSG_DRV_HOW).
* @{
*/
/* This function should be called at process innitial !!! */
int AlarmDrvInit(int proc_id);
int AlarmDrvExit();
/* API */
void SendAlarmQuitCmd();
void SendAlarmMotionrDetect(int Serial);
void SendAlarmAudioTrigger();
void SendMotionAlarmAudio(int alarmaudio_duration, int alarm_audio_enable);
void SendAlarmResetTrigger();
void SendAlarmInTrigger();
void SendAlarmEthTrigger();
void SendAlarmSchedule(int index);
void SendAlarmScheduleEnd();
/** @} */
#endif /* _ALARM_MSG_DRV_H_ */
/**
* @page ALARM_MSG_DRV_HOW How to use alarm message driver?
* 1. Add new message ID in \ref Alarm_Msg_Def.h if needed.\n
* 2. Call the API AlarmDrvInit(int proc_id) with the ID you added to initialize alarm
*  message driver.\n
* 3. Now you can use any alarm message driver functions as you want.\n
* 4. Call the API AlarmDrvExit() to cleanup alarm message driver.
* @section ALARM_MSG_DRV_HOW_EX Example
* @code
#include <alarm_msg_drv.h>
int main()
{
	SysInfo *pSysInfo;
	if( AlarmDrvInit(ALARM_SYS_MSG) < 0)
		return -1;
		// more alarm message driver API
	AlarmDrvExit();
	return 0;
}
* @endcode
*/
