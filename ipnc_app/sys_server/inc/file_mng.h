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
* @file file_mng.h
* @brief File manager.
*/
#ifndef __FILE_MNG_H__
#define __FILE_MNG_H__

#include <sys_env_type.h>
int FileMngInit(void *ShareMem);
int FileMngExit();
int ReadGlobal(void *Buffer);
int WriteGlobal(void *Buffer);
int FileMngReset(void *ShareMem);
LogEntry_t* GetLog(int nPageNum, int nItemIndex);
int IsFileThreadQuit();
int AddLog(LogEntry_t *pLog);
/* clear system and access logs */
int ClearSystemLog();
int ClearAccessLog();
#endif   /* __FILE_MNG_H__ */
