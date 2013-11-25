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
* @file semaphore_util.h
* @brief Semaphore API
*/
#ifndef _SEM_UTIL_H_
#define _SEM_UTIL_H_

int Sem_Creat();
int Sem_unlock();
int Sem_lock();
int Sem_kill();


#endif
