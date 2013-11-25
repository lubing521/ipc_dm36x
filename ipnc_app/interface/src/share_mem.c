/* ===========================================================================
* @file share_mem.c
*
* @path $(IPNCPATH)/util/
*
* @desc Share memory functions.
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
#include <stdio.h>
#include <string.h>
#include "share_mem.h"
#include <sys/ipc.h>
#include <sys/shm.h>

#ifdef DEBUG
#define __D(fmt, args...) fprintf(stderr, "Debug: " fmt, ## args)
#else
#define __D(fmt, args...)
#endif

static int mid;
/**
* @brief Initialize shared memory driver.

* Initialize shared momory driver.
* @note Once initialed, the memory ID is saved to global variable.
* @return Memory ID
* @retval -1 Fail to initialize shared memory.
*/
int ShareMemInit(int key)
{
	mid = shmget(key, PROC_MEM_SIZE * MAX_SHARE_PROC, IPC_CREAT | 0660);
	if(mid < 0)
		mid = shmget(key, 0, 0);
	__D("shared memory id:%d\n",mid);
	if(mid < 0)
		return -1;
	return mid;
}
/**
* @brief Initialize pshared memory driver.

* Initialize pshared momory driver.
* @note The memory ID isn't saved to global variable.
* @return Memory ID
* @retval -1 Fail to initialize shared memory.
*/
int pShareMemInit(int key)
{
	int mid = shmget(key, PROC_MEM_SIZE * MAX_SHARE_PROC, IPC_CREAT | 0660);
	__D("shared memory size %d\n", PROC_MEM_SIZE * MAX_SHARE_PROC);
	if(mid < 0)
		mid = shmget(key, 0, 0);
	__D("shared memory id:%d\n",mid);
	if(mid < 0)
		return -1;
	return mid;
}
/**
* @brief Read shared memory driver.

* Read shared momory.
* @param offset [i]  memory offset
* @param *buf [i]  pointer to memory buffer
* @param length [i]  data length to read
*/
void ShareMemRead(int offset,void *buf, int length)
{
	char *pSrc = shmat(mid,0,0);
	__D("%s\n",__func__);
	__D("offset: %d\n", offset);
	__D("buf: %x\n", (unsigned int) buf);
	__D("length: %d\n", length);
	memcpy(buf, pSrc + offset, length);
	shmdt(pSrc);
}
/**
* @brief Write shared memory driver.

* Write shared momory.
* @param offset [i]  memory offset
* @param *buf [i]  pointer to memory buffer
* @param length [i]  data length to read
*/
void ShareMemWrite(int offset,void *buf, int length)
{
	char *pDst = shmat(mid,0,0);
	__D("%s\n",__func__);
	__D("offset: %d\n", offset);
	__D("buf: %x\n", (unsigned int) buf);
	__D("length: %d\n", length);
	__D("pDst: %p\n", pDst);
	__D("pDst + offset: %p\n", pDst + offset);
	memcpy(pDst + offset, buf, length);
	shmdt(pDst);
}
