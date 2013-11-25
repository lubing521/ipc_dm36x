/* ===========================================================================
* @path $(IPNCPATH)\sys_server\inc\
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
* @file proc_alarm.h
* @brief Alarm processing thread.
*/
#ifndef __PROC_ALARM_H__
#define __PROC_ALARM_H__
#include <Msg_Def.h>
#include <sem_util.h>
#define PROC_SUCESS	(void*) 0
#define PROC_FAIL	(void*) -1

#define AUD_ALARM_START 0x900
#define AUD_ALARM_END   0x901

/**
* @brief Structure to save alarm message.
*/
typedef struct AlarmParam_t{
	ALARM_MSG_BUF msg_buf;
	int msg_id;
	int audMsg_id;
	SemHandl_t hAlarmSem;
}AlarmParam_t;

#define MSG_SZ 32

typedef struct {

  long mtype;
  int  msg;
  int  sampleRate;

} AudMsgBuf_t;

void *ProcAlarmThread(void *arg);

#endif /* __PROC_ALARM_H__ */

