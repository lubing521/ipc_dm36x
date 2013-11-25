/** ===========================================================================
* @file semephore_util.c
*
* @path $(IPNCPATH)\multimedia\encode_stream\stream
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
#include <stream.h>

#define __SEM_DEBUG

#ifdef __SEM_DEBUG
#define __D(fmt, args...) fprintf(stderr,  fmt, ## args)
#else
#define __D(fmt, args...)
#endif


#define __E(fmt, args...) fprintf(stderr, "Error: " fmt, ## args)



static pthread_mutex_t stream_mutex[STREAM_SEM_NUM];               /* Mutex to protect the global data */


/**
 * @brief	Create a semaphore
 * @param	"int index" : index
 * @return	index : success ; -1 : fail
 */
int Sem_Creat( int index )
{

	if( index < 0 || index >= STREAM_SEM_NUM)
	{
		__E("Sem_Creat index faill!!\n");
		return -1;
		
	}
	/* Initialize the mutex which protects the global data */
    if( pthread_mutex_init(&stream_mutex[index], NULL) != 0 )
    {
    	__E("Sem_Creat init faill!!\n");
		return -1;
    }else{
		return index;
    }

}

/**
 * @brief	Unlock a semaphore
 * @param	"int index" : index
 * @return	0 : success ; -1 : fail
 */
int Sem_unlock( int index )
{
	if( index < 0  || index >= STREAM_SEM_NUM)
	{
		__E("Sem_unlock faill!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		return -1;
		
	}
	pthread_mutex_unlock(&stream_mutex[index]);
	return 0;
}

/**
 * @brief	Lock a semaphore
 * @param	"int index" : index
 * @return	0 : success ; -1 : fail
 */
int Sem_lock( int index )
{
	if( index < 0  || index >= STREAM_SEM_NUM)
	{
		__E("Sem_lock faill!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		return -1;
		
	}
	pthread_mutex_lock(&stream_mutex[index]);
	return 0;
}

/**
 * @brief	Destroy a semaphore
 * @param	"int index" : index
 * @return	0 : success ; -1 : fail
 */
int Sem_kill( int index )
{
	if( index < 0  || index >= STREAM_SEM_NUM)
	{
		__E("Sem_kill index faill!!\n");
		return -1;
		
	}

	if( pthread_mutex_destroy(&stream_mutex[index])!= 0 )
	{
		__E("Sem_kill faill!!\n");
		return -1;
	}

	

	return 0;
}


