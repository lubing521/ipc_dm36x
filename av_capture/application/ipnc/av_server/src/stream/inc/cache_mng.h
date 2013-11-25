/** ===========================================================================
* @file cache_mng.h
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

#ifndef _CACHE_MNG_H_
#define _CACHE_MNG_H_

#include <mem_mng.h>

CACHE_DATA *CacheMng_GetCacheBySerial( int serial, VIDEO_BLK_INFO *pVidInfo );
int CacheMng_GetCacheInfoBySerial( int serial, CACHE_DATA_INFO *pCache_info, VIDEO_BLK_INFO *pVidInfo );

int CacheMng_Video_CacheLock( int serial , VIDEO_BLK_INFO *pVidInfo );
int CacheMng_Video_CacheRead( void *pDest, int *pSize, int *pFrm_type , int bufflimit, int serial , VIDEO_BLK_INFO *pVidInfo );
int CacheMng_Video_CacheUnlock( int serial , VIDEO_BLK_INFO *pVidInfo );

int CacheMng_Release(MEM_MNG_INFO *pInfo);

CACHE_MNG *CacheMng_Get_BaseMem(MEM_MNG_INFO *pInfo);

int CacheMng_Init(MEM_MNG_INFO *pInfo);

#endif
