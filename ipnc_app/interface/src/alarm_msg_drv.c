/* ===========================================================================
* @file alarm_msg_drv.c
*
* @path $(IPNCPATH)\util
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
#include <string.h>
#include "alarm_msg_drv.h"
#include <share_mem.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define DEBUG

#ifdef DEBUG
#define DBG(fmt, args...)	printf("AlarmMsgDrv: Debug\n" fmt, ##args)
#else
#define DBG(fmt, args...)
#endif

#ifndef JSK_DEBUG
#define JSK_DEBUG(fmt, args...) printf ("FILE[%s] LINE[%d] "fmt"\n", __FILE__, __LINE__, ##args)
#endif

#define ERR(fmt, args...)	printf("AlarmMsgDrv: Error\n" fmt, ##args)

static int gProcId = MSG_TYPE_MSG1;
static int qid = -1;

void SetAlarmProcId(int proc_id)
{
	if(proc_id < MSG_TYPE_MSG1 || proc_id >= MSG_TYPE_END){
		ERR("Error Process ID\n");
		gProcId = MSG_TYPE_MSG1;
		return;
	} else
		gProcId = proc_id;
}
/**
* @brief Save message ID in alarm message driver

* Save a message ID to alarm message driver. This ID will be used to identify
*  who is communicating with alarm manager. This function is old method, please
*  use AlarmDrvInit(int proc_id) instead.
* @param iqid [I ] message ID
*/
void AlarmSaveQid(int iqid)
{
	qid = iqid;
}
/**
* @brief Initialize alarm message driver.

* Initialize alarm message driver. Please see \ref ALARM_MSG_DRV_HOW to learn more.
* @note This API must be used before use any other alarm message driver API.
* @param proc_id [I ] Message ID(Which was define in \ref Alarm_Msg_Def.h) to initialize alarm message driver.
* @retval 0 Success.
* @retval -1 Fail.
*/
int AlarmDrvInit(int proc_id)
{
	if((qid = Msg_Init(ALARM_KEY)) < 0){
		ERR("Message queue init fail\n");
		return -1;
	}
	gProcId = proc_id;
	return 0;
}
/**
* @brief Cleanup alarm message driver.

* This API shoud be called at the end of process.
*/
int AlarmDrvExit()
{
	gProcId = MSG_TYPE_MSG1;
	qid = -1;
	return 0;
}
/**
* @brief Send quit command to alarm manager.

* This command will make alarm manager stop running. After you called this, all
*  the other process can't get system information because alarm manager is down.
*/
void SendAlarmQuitCmd()
{
	ALARM_MSG_BUF msgbuf;

	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = 0;
	msgbuf.event = ALARM_EVENT_QUIT;
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
}
/**
* @brief Send motion detection alarm command to alarm manager
*
* param Serial [I ] Serial number of JPEG which cause motion.
*/
void SendAlarmMotionrDetect(int Serial)
{
	ALARM_MSG_BUF msgbuf;

	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = 0;
	msgbuf.event = ALARM_EVENT_MOTION_DETECT;
	msgbuf.serial = Serial;
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
}
void SendAlarmAudioTrigger()
{
	ALARM_MSG_BUF msgbuf;

	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = 0;
	msgbuf.event = ALARM_EVENT_AUDIO_TRIG;
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
}
/**
* @brief Send motion detection audio alarm command to alarm manager
*/
void SendMotionAlarmAudio(int alarmaudio_duration, int alarm_audio_enable)
{
	ALARM_MSG_BUF msgbuf;

	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = 0;
	msgbuf.event = ALARM_EVENT_MOTION_AUDIO;
	msgbuf.alarmaudio_duration = alarmaudio_duration;
	msgbuf.alarmaudio_enable = alarm_audio_enable;
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
}
/**
* @brief Send  alarm reset trigger command to alarm manager
*/
void SendAlarmResetTrigger()
{
	ALARM_MSG_BUF msgbuf;

	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = 0;
	msgbuf.event = ALARM_EVENT_ALARMRESET_TRIG;
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
}
/**
* @brief Send  external alarm trigger command to alarm manager
*/
void SendAlarmInTrigger()
{
	ALARM_MSG_BUF msgbuf;

	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = 0;
	msgbuf.event = ALARM_EVENT_ALARMIN_TRIG;
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
}
/**
* @brief Send ethernet lost alarm command to alarm manager
*/
void SendAlarmEthTrigger()
{
	ALARM_MSG_BUF msgbuf;

	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = 0;
	msgbuf.event = ALARM_EVENT_ALARMETH_TRIG;
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
}
/**
* @brief Send schedule alarm command to alarm manager
* @param index [I ] Schedule index.
*/
void SendAlarmSchedule(int index)
{
	ALARM_MSG_BUF msgbuf;

	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = 0;
	msgbuf.event = ALARM_EVENT_SCHEDULE;
	msgbuf.index = index;
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
}
/**
* @brief Send schedule stop alarm command to alarm manager
*/
void SendAlarmScheduleEnd()
{
	ALARM_MSG_BUF msgbuf;

	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = 0;
	msgbuf.event = ALARM_EVENT_SCHEDULE_END;
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
}

