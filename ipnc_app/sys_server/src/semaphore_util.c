/** ===========================================================================
* @file semaphore_util.c
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

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <pthread.h>
#include "semaphore_util.h"

#define __SEM_DEBUG

#ifdef __SEM_DEBUG
#define __D(fmt, args...) fprintf(stderr,  fmt, ## args)
#else
#define __D(fmt, args...)
#endif

#define __E(fmt, args...) fprintf(stderr, "Error: " fmt, ## args)

static pthread_mutex_t mutex;               /* Mutex to protect the global data */

/**
 * @brief	Initialize the mutex which protects the global data
 * @param	none
 * @retval	0: success; -1: fail
 */
int Sem_Creat()
{
	/* Initialize the mutex which protects the global data */
    if( pthread_mutex_init(&mutex, NULL) != 0 )
    {
    	__E("Sem_Creat init faill!!\n");
		return -1;
    }
	return 0;
}

/**
 * @brief	Unlock the semephore
 * @param	none
 * @retval	0: success
 */
int Sem_unlock()
{
	pthread_mutex_unlock(&mutex);
	return 0;
}

/**
 * @brief	Lock the semephore
 * @param	none
 * @retval	0: success
 */
int Sem_lock()
{
	pthread_mutex_lock(&mutex);
	return 0;
}

/**
 * @brief	Destroy the semephore
 * @param	none
 * @retval	0: success; -1: fail
 */
int Sem_kill()
{
	if( pthread_mutex_destroy(&mutex)!= 0 )
	{
		__E("Sem_kill faill!!\n");
		return -1;
	}
	return 0;
}
