/* ===========================================================================
* @file sem_util.c
*
* @path $(IPNCPATH)/util/
*
* @desc Semaphore functions implemented by POSIX mutex.
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "sem_util.h"

#ifdef __SEM_DEBUG__
#define __D(fmt, args...) fprintf(stderr,  fmt, ## args)
#else
#define __D(fmt, args...)
#endif

#define __E(fmt, args...) fprintf(stderr, "Error: " fmt, ## args)
/**
* @brief Initialize semaphore driver.

* Initialize semaphore.
* @note This API must be used before use any other semaphore driver API.
* @return semaphore hamdler
* @retval NULL Fail to initialize semaphore.
*/
SemHandl_t MakeSem()
{
	SemHandl_t hndlSem = malloc(sizeof(pthread_mutex_t));
	if(hndlSem == NULL){
    	__E("Not enough memory!!\n");
		return NULL;
	}
	/* Initialize the mutex which protects the global data */
    if( pthread_mutex_init(hndlSem, NULL) != 0 )
    {
    	__E("Sem_Creat init faill!!\n");
		free(hndlSem);
		return NULL;
    }
	return hndlSem;
}
/**
* @brief Release semaphore.

* Release semaphore.
* @param hndlSem [i] semaphore handler
* @return semaphore hamdler
* @retval -1 Invalid semaphore handler.
* @retval 0 unlock the semaphore.
*/
int SemRelease(SemHandl_t hndlSem)
{
	if(hndlSem == NULL){
		__E("SemRelease: Invalid Semaphore handler\n");
		return -1;
	}
	return pthread_mutex_unlock(hndlSem);
}
/**
* @brief Lock semaphore.

* Lock semaphore.
* @param hndlSem [i] semaphore handler
* @retval -1 Invalid semaphore handler.
* @retval 0 lock the semaphore.
*/
int SemWait(SemHandl_t hndlSem)
{
	if(hndlSem == NULL){
		__E("SemWait: Invalid Semaphore handler\n");
		return -1;
	}
	return pthread_mutex_lock(hndlSem);
}
/**
* @brief Destory semaphore.

* Destroy semaphore.
* @param hndlSem [i] semaphore handler
* @retval -1 Invalid semaphore handler.
* @retval 0 Semaphore killed.
*/
int DestroySem(SemHandl_t hndlSem)
{
	if(hndlSem == NULL){
		__E("DestroySem: Invalid Semaphore handler\n");
		return -1;
	}
	pthread_mutex_lock(hndlSem);
	pthread_mutex_unlock(hndlSem);
	if( pthread_mutex_destroy(hndlSem)!= 0 )
	{
		__E("Sem_kill faill!!\n");
	}
	free(hndlSem);
	return 0;
}
