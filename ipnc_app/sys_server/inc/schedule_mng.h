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
* @file schedule_mng.h
* @brief Schedule manager.
*/
#ifndef _SCHEDULE_MNG_H_
#define _SCHEDULE_MNG_H_
#include <sys_env_type.h>

void *scheduleThrFxn(void* args);
int ScheduleMngInit(SysInfo *pSysInfo);
void PauseSchedule(void);
void ResumeSchedule(void);
struct tm *schedGetCurTime(void);
#endif
