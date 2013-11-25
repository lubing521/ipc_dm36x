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
* @file sem_util.h
* @brief Semaphore API
*/
#ifndef __SEM_UTIL_H__
#define __SEM_UTIL_H__
/**
* @ingroup UTILITY API
* @defgroup SEM_DRV Semaphore API

* This is the communication interface of semaphore manager. 
* @{
*/
typedef void* SemHandl_t;

SemHandl_t MakeSem(); ///< Initialize the semaphore.
int SemRelease(SemHandl_t hndlSem); ///< Unlock the semaphore.
int SemWait(SemHandl_t hndlSem); ///< Lock the semaphore.
int DestroySem(SemHandl_t hndlSem); ///< Destory the semaphore.
/** @} */
#endif
/**
* @page SEM_DRV_HOW How to use semaphore API?
* 1. Call the API MakeSem() to initialize semaphore.\n
* 2. Now you can use any semaphore API functions as you want.\n
* 3. Call the API DestroySem(SemHandl_t hndlSem) to cleanup semaphore.
* @section SEM_DRV_HOW_EX Example
* @code
#include <sem_util.h>
int main()
{
	SemHandl_t hndlDrvSem = NULL;
	hndlDrvSem = MakeSem();
	if(hndlDrvSem == NULL){
		return -1;
	}
	// More semaphore API
	DestroySem(hndlSem);
	return 0;
}
* @endcode
*/
