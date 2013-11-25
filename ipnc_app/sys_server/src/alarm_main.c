/* ===========================================================================
* @file alarm_main.c
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
#include <Msg_Def.h>
#include "proc_alarm.h"
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <alarm_schedule.h>
#include <avi_mng.h>
#include <file_dispatcher.h>
#include <jpg_mng.h>
#include <file_msg_drv.h>
volatile SemHandl_t hAlarmStatusSem;
volatile SemHandl_t hGIOSem;
volatile SemHandl_t audioSem;
SysInfo *pAlarmSysInfo;
/**
* @brief Alarm server main loop
* @param qid [I ] Message queue ID used to receive alarm message.
* @param hAlarmSem [I ] Semaphore to control access to alarm server globals.
* @return 0 on alarm server normal end.
*/
int alarm_main_loop(int qid, int audQid, SemHandl_t hAlarmSem)
{
	ALARM_MSG_BUF msgbuf;
	int msg_size,ret = 0;
	AlarmParam_t* pEvironment;
	pthread_t thread;

	pAlarmSysInfo = GetSysInfo();
	if((hGIOSem = MakeSem()) == NULL){
		return -1;
	}
	while(1){
		/* Get Message */
		msg_size = msgrcv( qid, &msgbuf, sizeof(msgbuf) - sizeof(long),
				MSG_TYPE_MSG1, 0);
		if( msg_size < 0 ){
			printf("Alarm Receive msg fail \n");
			ret = -1;
			break;
		} else if(msgbuf.src == MSG_TYPE_MSG1 || msgbuf.src < 0){
			printf("Got Error message\n");
			ret = -1;
			break;
		} else if(msgbuf.event == ALARM_EVENT_QUIT){
			printf("Recieved Quit event\n");
			break;
		} else if(IsRestart()){
			// If already restart flag is set then ignore the messages
		    continue;
		} else {
			/* Process alarm event */
			pEvironment = (AlarmParam_t*)malloc(sizeof(AlarmParam_t));
			if(pEvironment == NULL){
				printf("Error: More memory need for Alarm server\n");
				break;
			}
			pEvironment -> msg_id    = qid;
			pEvironment -> audMsg_id = audQid;
			pEvironment->hAlarmSem   = hAlarmSem;
			memcpy(&pEvironment -> msg_buf, &msgbuf, sizeof(msgbuf));
			if((ret = pthread_create(&thread, NULL, ProcAlarmThread, pEvironment)) != 0){
				free(pEvironment);
				printf("Error: Alarm server creat thread fail\n");
				break;
			}
			pthread_detach(thread);
		}
	}
	DestroySem(hGIOSem);
	return ret;
}
/**
* @brief Main thread of alarm server.
* @param arg [I ] Not used.
* @return 0 on alarm server normal end.
*/
void *AlramThread(void *arg) {
	int qid;
	int audQid;
	SemHandl_t hAlarmSem;
	pthread_t thread;
	void *ret;
	qid = Msg_Init(ALARM_KEY);
	if(qid < 0)
		return PROC_FAIL;
	audQid = Msg_Init(AUD_MSG_KEY);
	if(audQid < 0)
		return PROC_FAIL;
	hAlarmSem = MakeSem();
	if(hAlarmSem == NULL){
		Msg_Kill(qid);
		return PROC_FAIL;
	}
	hAlarmStatusSem = MakeSem();
	if(hAlarmStatusSem == NULL){
		DestroySem(hAlarmSem);
		Msg_Kill(qid);
		return PROC_FAIL;
	}
	audioSem = MakeSem();
	if(audioSem == NULL){
		DestroySem(hAlarmStatusSem);
		DestroySem(hAlarmSem);
		Msg_Kill(qid);
		return PROC_FAIL;
	}
	if(pthread_create(&thread, NULL, AlramScheduler, NULL)){
		DestroySem(hAlarmStatusSem);
		DestroySem(audioSem);
		DestroySem(hAlarmSem);
		Msg_Kill(qid);
		return PROC_FAIL;
	}
	if(AviMngInit()){
		AlarmScheduleExit();
		pthread_join(thread, &ret);
		DestroySem(hAlarmStatusSem);
		DestroySem(audioSem);
		DestroySem(hAlarmSem);
		Msg_Kill(qid);
		return PROC_FAIL;
	}
	if(JpgMngInit()){
		AviMngExit();
		AlarmScheduleExit();
		pthread_join(thread, &ret);
		DestroySem(hAlarmStatusSem);
		DestroySem(audioSem);
		DestroySem(hAlarmSem);
		Msg_Kill(qid);
		return PROC_FAIL;
	}
	if(FileDispatcherInit()){
		JpgMngExit();
		AviMngExit();
		AlarmScheduleExit();
		pthread_join(thread, &ret);
		DestroySem(hAlarmStatusSem);
		DestroySem(audioSem);
		DestroySem(hAlarmSem);
		Msg_Kill(qid);
		return PROC_FAIL;
	}
	alarm_main_loop(qid, audQid, hAlarmSem);
	AviMngExit();
	JpgMngExit();
	FileDispatcherExit();
	DestroySem(hAlarmStatusSem);
	DestroySem(audioSem);
	DestroySem(hAlarmSem);
	Msg_Kill(qid);
	Msg_Kill(audQid);
	return PROC_SUCESS;
}
