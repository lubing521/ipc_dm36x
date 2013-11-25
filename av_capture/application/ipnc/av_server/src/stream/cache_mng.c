/** ===========================================================================
* @file cache_mng.c
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


/****************************
*		Includes		      *
****************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <linux/errno.h>
#include <stdlib.h>
#include <string.h>

#include <cache_mng.h>
#include "cmem.h"
#include <stream.h>
#include <drv_frameCopy.h>


//#define __CACHE_DEBUG

#ifdef __CACHE_DEBUG
#define __D(fmt, args...) fprintf(stderr, "Cache Debug: " fmt, ## args)
#else
#define __D(fmt, args...)
#endif

#define __E(fmt, args...) fprintf(stderr, "Cache Error: " fmt, ## args)

#define CACHEMNG_DMA	(0)

#if CACHEMNG_DMA
static DRV_DmaChHndl dmaHndl;
#endif

int CacheMng_MemCpy_open(void);
int CacheMng_MemCpy_close(void);


CACHE_MNG	Cache_mng_default[VIDOE_INFO_END] =
{
	{
		.video_type 		= VIDEO_MJPG,
		.start_addr			= 0,
		.start_phy			= 0,
		.size				= MJPG_CACHE_SIZE,
		.blk_sz				= MJPG_CACHE_BLK_SIZE,
		.blk_num			= (MJPG_CACHE_SIZE/MJPG_CACHE_BLK_SIZE),
		.cache_num			= (MJPG_CACHE_SIZE/MJPG_CACHE_BLK_SIZE),
		.cache 				= NULL,
		.blk 				= NULL,
	},
	{
		.video_type 		= VIDEO_MP4,
		.start_addr			= 0,
		.start_phy			= 0,
		.size				= MP4_CACHE_SIZE,
		.blk_sz				= MP4_CACHE_BLK_SIZE,
		.blk_num			= (MP4_CACHE_SIZE/MP4_CACHE_BLK_SIZE),
		.cache_num			= (MP4_CACHE_SIZE/MP4_CACHE_BLK_SIZE),
		.cache 				= NULL,
		.blk 				= NULL,
	},
	{
		.video_type 		= VIDEO_MP4,
		.start_addr			= 0,
		.start_phy			= 0,
		.size				= MP4_EXT_CACHE_SIZE,
		.blk_sz				= MP4_EXT_CACHE_BLK_SIZE,
		.blk_num			= (MP4_EXT_CACHE_SIZE/MP4_EXT_CACHE_BLK_SIZE),
		.cache_num			= (MP4_EXT_CACHE_SIZE/MP4_EXT_CACHE_BLK_SIZE),
		.cache 				= NULL,
		.blk 				= NULL,
	},
	{
		.video_type 		= AUDIO_G711,
		.start_addr			= 0,
		.start_phy			= 0,
		.size				= AUDIO_CACHE_SIZE,
		.blk_sz				= AUDIO_CACHE_BLK_SIZE,
		.blk_num			= (AUDIO_CACHE_SIZE/AUDIO_CACHE_BLK_SIZE),
		.cache_num			= (AUDIO_CACHE_SIZE/AUDIO_CACHE_BLK_SIZE),
		.cache 				= NULL,
		.blk 				= NULL,
	}
};

//static int LockCnt = 0;

/**
 * @brief	Show cache info
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @param	"int blk_use"
 * @return	none
 */
void ShowCacheInfo(VIDEO_BLK_INFO *pVidInfo , int blk_use)
{
	__D("blk_use = %d\n",blk_use);
	__D("blk_num = %d\n",pVidInfo->cachemng->blk_num);
	__D("blk_sz = %d\n",pVidInfo->cachemng->blk_sz);

	int cnt = 0;
	int loops = pVidInfo->cachemng->blk_num;

	__D("video_type = %d\n",pVidInfo->cachemng->video_type);
	//__D("LockCnt = %d\n",LockCnt);


	for( cnt = 0; cnt < loops; cnt++ )
	{
		__D("blk[%d].IsFree = %d\n",cnt,pVidInfo->cachemng->blk[cnt].IsFree);
		__D("blk[%d].serial = %d\n",cnt,pVidInfo->cachemng->blk[cnt].serial);
	}

}

/**
 * @brief	Show lock count
 */
void ShowLockInfo(void)
{
	//__D("LockCnt = %d\n",LockCnt);

}


/**
 * @brief	Reset cache
 * @param	"CACHE_DATA *pcache" : pointer to cache data
 * @return	none
 */
void CacheMng_cache_reset( CACHE_DATA *pcache)
{
	int cnt = 0;
	if( pcache )
	{
		pcache->fram_type	= EMPTY_CACHE;
		pcache->serial 		= -1;
		pcache->blkindex	= -1;
		pcache->blks		= -1;
		pcache->realsize	= -1;
		pcache->cnt			= 0;
		pcache->flag		= -1;
		pcache->timestamp	= 0;
		pcache->temporalId	= -1;
		for( cnt = 0; cnt < VIDOE_INFO_END; cnt++ )
		{
			pcache->ref_serial[cnt] = -1;
		}
	}else{
		__E("CacheMng_cache_reset : NULL \n");
	}
}

/**
 * @brief	Set cache block
 * @param	"CACHE_BLK *pblk" : cache block
 * @param	"int serial" : serial number
 * @return	none
 */
void CacheMng_blk_set( CACHE_BLK *pblk, int serial)
{
	if( pblk )
	{
		pblk->IsFree	= 0;
		pblk->serial	= serial;
	}else{
		__E("CacheMng_blk_set : NULL \n");
	}
}

/**
 * @brief	Unset cache block
 * @param	"CACHE_BLK *pblk" : cache block
 * @return	none
 */
void CacheMng_blk_unset( CACHE_BLK *pblk)
{
	if( pblk )
	{
		pblk->IsFree	= 1;
		pblk->serial	= -1;
	}else{
		__E("CacheMng_blk_unset : NULL \n");
	}
}

/**
 * @brief	Reset cache block
 * @param	"CACHE_BLK *pblk" : cache block
 * @param	"int offset" : offset
 * @return	none
 */
void CacheMng_blk_reset( CACHE_BLK *pblk, int offset)
{
	if( pblk )
	{
		pblk->IsFree	= 1;
		pblk->offset	= offset;
		pblk->serial	= -1;
	}else{
		__E("CacheMng_blk_reset : NULL \n");
	}
}


/**
 * @brief	Open a memory copy
 * @param	none
 * @return	0 : success ; -1 : fail
 */
int CacheMng_MemCpy_open(void)
{
#if	CACHEMNG_DMA
	DRV_dmaOpen(&dmaHndl, DRV_DMA_MODE_NORMAL, 1);
#endif

	return 0;
}

/**
 * @brief	Memory copy funtion
 * @param	"void *Des" : destination address
 * @param	"void *Src" : source address
 * @param	"int size" : size
 * @return	0 : success ; -1 : fail
 */
int CacheMng_MemCpy( void *Des, void *Src, int size )
{
	if( Des == NULL || Src == NULL )
	{
		return CACHE_PARM_ERR;
	}

#if	CACHEMNG_DMA

	DRV_DmaCopy1D copy1D;

	Sem_lock( STREAM_SEM_CACHECPY);

	copy1D.srcPhysAddr 	= (unsigned long)DRV_dmaGetPhysAddr((unsigned long)Src);
	copy1D.dstPhysAddr 	= (unsigned long)DRV_dmaGetPhysAddr((unsigned long)Des);
	copy1D.size 				=	size;
	DRV_dmaCopy1D(&dmaHndl, &copy1D, 1);

	Sem_unlock( STREAM_SEM_CACHECPY);

#else
	memcpy( Des, Src , size );
#endif

	return 0;
}


/**
 * @brief	Close a memory copy
 * @param	none
 * @return	0 : success ; -1 : fail
 */
int CacheMng_MemCpy_close(void)
{
#if	CACHEMNG_DMA
	DRV_dmaClose(&dmaHndl);
#endif
	return 0;

}


/**
 * @brief	Get cache manager
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	cachemng ; NULL
 */
CACHE_MNG *CacheMng_Get_BaseMem(MEM_MNG_INFO *pInfo)
{
	int cnt = 0;
	int Isfind = 0;
	for( cnt = 0; cnt < pInfo->video_info_nums ; cnt++ )
	{
		if( pInfo->video_info[cnt].IsCache )
		{
			Isfind = 1;
			break;
		}
	}

	if( Isfind == 1 )
	{
		return pInfo->video_info[cnt].cachemng;
	}else{
		return NULL;
	}
}

/**
 * @brief	Initialize cache manager
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	0 : success
 */
int CacheMng_Init(MEM_MNG_INFO *pInfo)
{
	int	cnt = 0;
	int cnt2 = 0;
	int loops = 0;

	if( pInfo->freesize <= 0 )
	{
		__E("CacheMng_Init FAIL\n");
			return CACHE_NOMEM;
	}


	loops = sizeof( Cache_mng_default )/ sizeof( Cache_mng_default[0] );


	for( cnt = 0; cnt < loops ; cnt++ )
	{

		if( Cache_mng_default[cnt].video_type == pInfo->video_info[cnt].video_type )
		{
			pInfo->video_info[cnt].IsCache = 1;
			pInfo->video_info[cnt].cachemng = calloc( 1, sizeof(CACHE_MNG) );
			memcpy( pInfo->video_info[cnt].cachemng , &Cache_mng_default[cnt], sizeof(CACHE_MNG) );

			pInfo->video_info[cnt].cachemng->start_addr = pInfo->start_addr + pInfo->offset;
			pInfo->video_info[cnt].cachemng->start_phy 	= pInfo->start_phyAddr + pInfo->offset;
			pInfo->video_info[cnt].cachemng->cache		= calloc( pInfo->video_info[cnt].cachemng->cache_num ,
															sizeof(CACHE_DATA) );
			pInfo->video_info[cnt].cachemng->blk		= calloc( pInfo->video_info[cnt].cachemng->blk_num ,
															sizeof(CACHE_BLK) );

			pInfo->freesize -= pInfo->video_info[cnt].cachemng->size;
			pInfo->offset	+= pInfo->video_info[cnt].cachemng->size;

			if( pInfo->freesize < 0 ||
				pInfo->video_info[cnt].cachemng == NULL ||
				pInfo->video_info[cnt].cachemng->blk == NULL ||
				pInfo->video_info[cnt].cachemng->cache == NULL )
			{
				__E("CacheMng_Init malloc FAIL \n");
					return CACHE_PARM_ERR;
			}

			for( cnt2 =0 ; cnt2 < pInfo->video_info[cnt].cachemng->cache_num ; cnt2++ )
			{
				CacheMng_cache_reset( &(pInfo->video_info[cnt].cachemng->cache[cnt2]) );
			}

			for( cnt2 =0 ; cnt2 < pInfo->video_info[cnt].cachemng->blk_num ; cnt2++ )
			{
				CacheMng_blk_reset( &(pInfo->video_info[cnt].cachemng->blk[cnt2]),
									(cnt2*pInfo->video_info[cnt].cachemng->blk_sz) );
			}
			//ShowCacheInfo(&pInfo->video_info[cnt],0);

		}
		else{
			pInfo->video_info[cnt].IsCache = 0;
		}


	}


	CacheMng_MemCpy_open();

	return 0;
}



/**
 * @brief	find free cache blocks
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @param	"int blk_use" : request block number
 * @return	blkindex ; CACHE_ERR
 */
int CacheMng_Find_Freeblk( VIDEO_BLK_INFO *pVidInfo , int blk_use )
{
	int cnt = 0;
	int cnt2 = 0;
	int loops = pVidInfo->cachemng->blk_num - blk_use + 1;
	int findblk = 0;
	int blkindex = 0;

	for( cnt = 0; cnt < loops; cnt++ )
	{
		findblk = 1;
		for( cnt2 = 0; cnt2 < blk_use ; cnt2++ )
		{
			if( pVidInfo->cachemng->blk[cnt+cnt2].IsFree != 1 )
			{
				findblk = 0;
				break;
			}
		}

		if( findblk == 1 )
		{
			blkindex = cnt;
			break;
		}

	}

	if( findblk == 1 )
	{
		return blkindex;
	}
	else
	{
		ShowCacheInfo(pVidInfo , blk_use);
		return CACHE_ERR;
	}

}


/**
 * @brief	find free cache
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	0 : success
 */
int CacheMng_Find_FreeCache( VIDEO_BLK_INFO *pVidInfo )
{
	int cnt = 0;
	int loops = pVidInfo->cachemng->cache_num;
	int findcache= 0;
	int cacheindex = 0;

	for( cnt = 0; cnt < loops; cnt++ )
	{
		if( pVidInfo->cachemng->cache[cnt].fram_type == EMPTY_CACHE )
		{
			findcache = 1;
			cacheindex = cnt;
			break;
		}
	}

	if( findcache == 1 )
	{
		return cacheindex;
	}else{
		return CACHE_ERR;
	}
}

/**
 * @brief	find cache block by serial
 * @param	"int serial" : serial no.
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	blkindex ; CACHE_ERR
 */
int CacheMng_FindCacheBySerial( int serial, VIDEO_BLK_INFO *pVidInfo )
{
	int cnt = 0;
	int loops = pVidInfo->cachemng->cache_num;
	int findcache= 0;
	int cacheindex = 0;

	if( serial > pVidInfo->cur_serial )
	{
		return CACHE_ERR;
	}

	for( cnt = 0; cnt < loops; cnt++ )
	{
		if( pVidInfo->cachemng->cache[cnt].serial == serial &&
			pVidInfo->cachemng->cache[cnt].fram_type != EMPTY_CACHE)
		{
			findcache = 1;
			cacheindex = cnt;
			break;
		}
	}

	if( findcache == 1 )
	{
		return cacheindex;
	}else{
		return CACHE_ERR;
	}
}

/**
 * @brief	Get cache block by serial
 * @param	"int serial" : serial no.
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	cache address ; CACHE_ERR
 */
CACHE_DATA *CacheMng_GetCacheBySerial( int serial, VIDEO_BLK_INFO *pVidInfo )
{
	int cnt = 0;
	int loops = pVidInfo->cachemng->cache_num;
	int findcache= 0;
	int cacheindex = 0;

	if( serial > pVidInfo->cur_serial )
	{
		return NULL;
	}

	for( cnt = 0; cnt < loops; cnt++ )
	{
		if( pVidInfo->cachemng->cache[cnt].serial == serial &&
			pVidInfo->cachemng->cache[cnt].fram_type != EMPTY_CACHE)
		{
			findcache = 1;
			cacheindex = cnt;
			break;
		}
	}

	if( findcache == 1 )
	{
		return &(pVidInfo->cachemng->cache[cacheindex]);
	}else{
		return NULL;
	}
}


/**
 * @brief	Get cache info by serial
 * @param	"int serial" : serial no.
 * @param	"CACHE_DATA_INFO *pCache_info"
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	0 ; CACHE_ERR
 */
int CacheMng_GetCacheInfoBySerial( int serial, CACHE_DATA_INFO *pCache_info, VIDEO_BLK_INFO *pVidInfo )
{
	CACHE_DATA *pCache = NULL;
	CACHE_BLK	*pblk  = NULL;
	int cnt = 0;
	int loops = pVidInfo->cachemng->cache_num;
	int findcache= 0;
	int cacheindex = 0;

	for( cnt = 0; cnt < loops; cnt++ )
	{
		if( pVidInfo->cachemng->cache[cnt].serial == serial &&
			pVidInfo->cachemng->cache[cnt].fram_type != EMPTY_CACHE)
		{
			findcache = 1;
			cacheindex = cnt;
			break;
		}
	}

	if( findcache == 1 )
	{
		pCache = &(pVidInfo->cachemng->cache[cacheindex]);
		pblk = &(pVidInfo->cachemng->blk[ pCache->blkindex ]);

		pCache_info->serial		= pCache->serial;
		pCache_info->fram_type	= pCache->fram_type;
		pCache_info->start_addr	= pVidInfo->cachemng->start_addr + pblk->offset;
		pCache_info->start_phy	= pVidInfo->cachemng->start_phy + pblk->offset;
		pCache_info->realsize	= pCache->realsize;
		pCache_info->flag		= pCache->flag;
		pCache_info->timestamp	= pCache->timestamp;
		pCache_info->temporalId	= pCache->temporalId;
		for( cnt = 0; cnt < VIDOE_INFO_END; cnt++ )
		{
			pCache_info->ref_serial[cnt] = pCache->ref_serial[cnt];
		}
		return 0;
	}else{
		return CACHE_ERR;
	}
}

/**
 * @brief	Write cache
 * @param	"CACHE_DATA *pCache"
 * @param	"VIDEO_FRAME *pFrame"
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	0 ; CACHE_PARM_ERR
 */
int CacheMng_cache_write( CACHE_DATA *pCache, VIDEO_FRAME *pFrame, VIDEO_BLK_INFO *pVidInfo )
{
	CACHE_BLK	*pblk	= NULL;
	unsigned long Src_Addr = 0;
	unsigned long Des_Addr = 0;
	int size = 0;
	int cnt = 0;

	if( pCache==NULL || pFrame==NULL || pVidInfo==NULL )
	{
		__E("CacheMng_cache_write: invalidate parameter!! \n");
		return CACHE_PARM_ERR;
	}

	pCache->fram_type	= pFrame->fram_type;
	pCache->serial 		= pFrame->serial;
	pCache->realsize	= pFrame->realsize;
	pCache->flag		= pFrame->flag;
	pCache->timestamp	= pFrame->timestamp;
	pCache->temporalId	= pFrame->temporalId;

	for( cnt = 0; cnt < VIDOE_INFO_END; cnt++ )
	{
		pCache->ref_serial[cnt] = pFrame->ref_serial[cnt];
	}

	for( cnt = 0; cnt < pCache->blks ;cnt ++ )
	{
		pblk = &(pVidInfo->cachemng->blk[ pCache->blkindex + cnt ]);
		CacheMng_blk_set( pblk, pCache->serial );
	}

	pblk = &(pVidInfo->cachemng->blk[ pCache->blkindex ]);

	Des_Addr = pVidInfo->cachemng->start_addr + pblk->offset;
	Src_Addr = pVidInfo->start + pVidInfo->blk_sz*pFrame->blkindex;

	size = pCache->realsize;

	if( CacheMng_MemCpy( (void *)Des_Addr, (void *)Src_Addr, size ) < 0 )
	{
		__E("CacheMng_cache_write: error memcpy \n");
		return CACHE_PARM_ERR;
	}

	return 0;
}

/**
 * @brief	Video cache lock
 * @param	"int serial" : serial no.
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	0 ; CACHE_PARM_ERR
 */
int CacheMng_Video_CacheLock( int serial , VIDEO_BLK_INFO *pVidInfo )
{
	VIDEO_FRAME *pFrame = NULL;
	CACHE_DATA	*pCache = NULL;

	int		blk_use = 0;
	int		blk_index = 0;
	int		blk_sz = 0;
	int		cache_index = 0;
	int 	ret = 0;

	if( !pVidInfo || serial<0 )
	{
		__E("CacheMng_Video_Cache: error parameter \n");
		return CACHE_PARM_ERR;
	}
	//ShowLockInfo();


	/* check if cached */
	cache_index = CacheMng_FindCacheBySerial( serial, pVidInfo );
	if( cache_index >= 0 )
	{
		pCache = &(pVidInfo->cachemng->cache[cache_index]);
		pCache->cnt += 1;
		//LockCnt++;
		return 0;
	}

	pFrame = MemMng_GetFrameBySerial( serial , pVidInfo);

	if( pFrame )
	{
		blk_sz 		= pVidInfo->cachemng->blk_sz;
		blk_use 	= ( pFrame->realsize + blk_sz - 1)/blk_sz;
		blk_index 	= CacheMng_Find_Freeblk( pVidInfo , blk_use );
		if( blk_index < 0 )
		{
			__E("pFrame->realsize = %d \t blk_index = %d %d %d\n",pFrame->realsize, blk_index, blk_sz, blk_use);
			__E("CacheMng_Video_Cache: CacheMng_Find_Freeblk Fail \n");
			ret = CACHE_NOMEM;
			goto CACHE_FAIL;
		}
		cache_index = CacheMng_Find_FreeCache( pVidInfo );
		if( cache_index < 0 )
		{
			__E("CacheMng_Video_Cache: CacheMng_Find_FreeCache Fail \n");
			ret = CACHE_NOMEM;
			goto CACHE_FAIL;
		}

		pCache 	= &(pVidInfo->cachemng->cache[cache_index]);

		pCache->blkindex	= blk_index;
		pCache->blks		= blk_use;
		pCache->cnt 		= 1;

		if(CacheMng_cache_write( pCache, pFrame, pVidInfo ) < 0 )
		{
			ret = CACHE_PARM_ERR;
			goto CACHE_FAIL;
		}

	}else{
		__D("CacheMng_Video_Cache: search frame fail!! \n");
		return CACHE_ERR;
	}

	//LockCnt++;
	return 0;

CACHE_FAIL:
	__E("CacheMng_Video_Cache: CACHE_FAIL \n");
	return ret;
}

/**
 * @brief	Video cache read
 * @param	"void *pDest" : destination
 * @param	"int *pSize" : size
 * @param	"int *pFrm_type" : frame type
 * @param	"int bufflimit" : buffer limit
 * @param	"int serial" : serial no.
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @retval	0 : success
 * @retval	CACHE_PARM_ERR
 * @retval	CACHE_MEM_FAIL
 */
int CacheMng_Video_CacheRead( 	void *pDest, int *pSize, int *pFrm_type , int bufflimit,
								int serial , VIDEO_BLK_INFO *pVidInfo )
{
	CACHE_DATA	*pCache = NULL;
	CACHE_BLK	*pblk	= NULL;
	unsigned long Src_Addr = 0;

	if( !pVidInfo || serial<0 )
	{
		__E("CacheMng_Video_CacheRead: error parameter \n");
		return CACHE_PARM_ERR;
	}

	pCache = CacheMng_GetCacheBySerial( serial, pVidInfo );
	if( pCache == NULL )
	{
		__E("CacheMng_Video_CacheRead: can not serial \n");
		return CACHE_PARM_ERR;
	}

	if( pCache->realsize > bufflimit )
	{
		__E("CacheMng_Video_CacheRead: over bufflimit \n");
		return CACHE_MEM_FAIL;
	}

	*pSize 		= pCache->realsize;
	*pFrm_type 	= pCache->fram_type;

	pblk = &(pVidInfo->cachemng->blk[ pCache->blkindex ]);
	Src_Addr = pVidInfo->cachemng->start_addr + pblk->offset;

	CacheMng_MemCpy( pDest, (void *)Src_Addr, pCache->realsize );

	return 0;

}

/**
 * @brief	Video cache unlock
 * @param	"int serial" : serial no.
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @retval	0 : success
 * @retval	CACHE_PARM_ERR
 * @retval	CACHE_ERR
 */
int CacheMng_Video_CacheUnlock( int serial , VIDEO_BLK_INFO *pVidInfo )
{
	CACHE_DATA	*pCache = NULL;
	CACHE_BLK	*pblk	= NULL;
	int cache_index = 0;
	int cnt = 0;

	if( !pVidInfo || serial<0 )
	{
		__E("CacheMng_Video_Cache: error parameter \n");
		return CACHE_PARM_ERR;
	}
	//ShowLockInfo();

	/* check if cached */
	cache_index = CacheMng_FindCacheBySerial( serial, pVidInfo );
	if( cache_index < 0 )
	{
		__E("CacheMng_Video_Free: search cache fail!! \n");
		return CACHE_ERR;
	}
	pCache = &(pVidInfo->cachemng->cache[cache_index]);

	pCache->cnt -= 1;

	if( pCache->cnt <= 0 )
	{
		for( cnt = 0; cnt < pCache->blks ;cnt ++ )
		{
			pblk = &(pVidInfo->cachemng->blk[ pCache->blkindex + cnt ]);
			CacheMng_blk_unset( pblk );
		}

		CacheMng_cache_reset( pCache );
	}
	//LockCnt--;
	return 0;
}


/**
 * @brief	Release cache
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	0 : success
 */
int CacheMng_Release(MEM_MNG_INFO *pInfo)
{
	int	cnt = 0;

	for( cnt = 0; cnt < pInfo->video_info_nums ; cnt++ )
	{
		if( pInfo->video_info[cnt].IsCache )
		{
			if( pInfo->video_info[cnt].cachemng->cache )
			{
				free( pInfo->video_info[cnt].cachemng->cache );
				pInfo->video_info[cnt].cachemng->cache = NULL;

			}

			if( pInfo->video_info[cnt].cachemng->blk )
			{
				free(pInfo->video_info[cnt].cachemng->blk);
				pInfo->video_info[cnt].cachemng->blk = NULL;
			}

			if( pInfo->video_info[cnt].cachemng )
			{
				free( pInfo->video_info[cnt].cachemng);
				pInfo->video_info[cnt].cachemng = NULL;
			}

		}
	}

	CacheMng_MemCpy_close();

	return 0;
}


