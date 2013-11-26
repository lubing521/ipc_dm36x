/** ===========================================================================
* @file mem_mng.c
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

#include <mem_mng.h>
#include <cmem.h>
#include <stream.h>

#include <osa_cmem.h>
#include <drv_frameCopy.h>


#ifdef __MEM_MNG_DEBUG
#define __D(fmt, args...) fprintf(stderr, "Debug: " fmt, ## args)
#else
#define __D(fmt, args...)
#endif


#define __E(fmt, args...) fprintf(stderr, "Error: " fmt, ## args)


//#define STREAM_CMEM_ALLOC OSA_cmemAlloc
#define STREAM_CMEM_ALLOC OSA_cmemAllocCached

#define MEMMNG_DMA	(1)

#if	MEMMNG_DMA
static DRV_DmaChHndl dmaHndl;
#endif

int MemMng_memcpy_open(void);
int MemMng_memcpy_close(void);

MEM_LAYOUT mem_layout_setting[MEM_LAYOUT_NUM] = {
	[MEM_LAYOUT_0] = {	//single stream non MJPEG usecases
					.profiles[VIDOE_INFO_MJPG] =
					{
						.cache_size 		= 0x000000,
						.cache_blk_size 	= (50*1024),
						.mem_size 			= 0x000000,
						.mem_blk_size 		= (50*1024),
						.ext_size 			= 0,
					},
					.profiles[VIDOE_INFO_MP4] =
					{
						.cache_size 		= 0xA00000,
						.cache_blk_size 	= (50*1024),
						.mem_size 			= 0xA00000,
						.mem_blk_size 		= (50*1024),
						.ext_size 			= MP4_1_EXTRA_SIZE,
					},
					.profiles[VIDOE_INFO_MP4_EXT] =
					{
						.cache_size 		= 0x000000,
						.cache_blk_size 	= (50*1024),
						.mem_size 			= 0x000000,
						.mem_blk_size 		= (50*1024),
						.ext_size 			= MP4_2_EXTRA_SIZE,
					},
					.profiles[AUDIO_INFO_G711] =
					{
						.cache_size 		= AUDIO_CACHE_SIZE,
						.cache_blk_size 	= AUDIO_CACHE_BLK_SIZE,
						.mem_size 			= AUDIO_MEM_SIZE,
						.mem_blk_size 		= AUDIO_BLK_SIZE,
						.ext_size 			= 0,
					},
					.totalsizes = 0,

				},
	[MEM_LAYOUT_1] = { // only for megapixel H264 mode
					.profiles[VIDOE_INFO_MJPG] =
					{
						.cache_size 		= 0x000000,
						.cache_blk_size 	= (50*1024),
						.mem_size 			= 0x000000,
						.mem_blk_size 		= (50*1024),
						.ext_size 			= 0,
					},
					.profiles[VIDOE_INFO_MP4] =
					{
						.cache_size 		= 0x500000,
						.cache_blk_size 	= (50*1024),
						.mem_size 			= 0x500000,
						.mem_blk_size 		= (50*1024),
						.ext_size 			= MP4_1_EXTRA_SIZE,
					},
					.profiles[VIDOE_INFO_MP4_EXT] =
					{
						.cache_size 		= 0x000000,
						.cache_blk_size 	= (50*1024),
						.mem_size 			= 0x000000,
						.mem_blk_size 		= (50*1024),
						.ext_size 			= MP4_2_EXTRA_SIZE,
					},
					.profiles[AUDIO_INFO_G711] =
					{
						.cache_size 		= AUDIO_CACHE_SIZE,
						.cache_blk_size 	= AUDIO_CACHE_BLK_SIZE,
						.mem_size 			= AUDIO_MEM_SIZE,
						.mem_blk_size 		= AUDIO_BLK_SIZE,
						.ext_size 			= 0,
					},
					.totalsizes = 0,

				},
	[MEM_LAYOUT_2] = { // only for megapixel JPEG mode
					.profiles[VIDOE_INFO_MJPG] =
					{
						.cache_size 		= 0xA00000,
						.cache_blk_size 	= (100*1024),
						.mem_size 			= 0xA00000,
						.mem_blk_size 		= (100*1024),
						.ext_size 			= 0,
					},
					.profiles[VIDOE_INFO_MP4] =
					{
						.cache_size 		= 0x000000,
						.cache_blk_size 	= (100*1024),
						.mem_size 			= 0x000000,
						.mem_blk_size 		= (100*1024),
						.ext_size 			= MP4_1_EXTRA_SIZE,
					},
					.profiles[VIDOE_INFO_MP4_EXT] =
					{
						.cache_size 		= 0x000000,
						.cache_blk_size 	= (100*1024),
						.mem_size 			= 0x000000,
						.mem_blk_size 		= (100*1024),
						.ext_size 			= MP4_2_EXTRA_SIZE,
					},
					.profiles[AUDIO_INFO_G711] =
					{
						.cache_size 		= AUDIO_CACHE_SIZE,
						.cache_blk_size 	= AUDIO_CACHE_BLK_SIZE,
						.mem_size 			= AUDIO_MEM_SIZE,
						.mem_blk_size 		= AUDIO_BLK_SIZE,
						.ext_size 			= 0,
					},
					.totalsizes = 0,

				},
	[MEM_LAYOUT_3] = {	//dual stream D1+D1 usecases without JPEG
					.profiles[VIDOE_INFO_MJPG] =
					{
						.cache_size 		= 0x000000,
						.cache_blk_size 	= (50*1024),
						.mem_size 			= 0x000000,
						.mem_blk_size 		= (50*1024),
						.ext_size 			= 0,
					},
					.profiles[VIDOE_INFO_MP4] =
					{
						.cache_size 		= 0x500000,
						.cache_blk_size 	= (30*1024),
						.mem_size 			= 0x500000,
						.mem_blk_size 		= (30*1024),
						.ext_size 			= MP4_1_EXTRA_SIZE,
					},
					.profiles[VIDOE_INFO_MP4_EXT] =
					{
						.cache_size 		= 0x500000,
						.cache_blk_size 	= (30*1024),
						.mem_size 			= 0x500000,
						.mem_blk_size 		= (30*1024),
						.ext_size 			= MP4_2_EXTRA_SIZE,
					},
					.profiles[AUDIO_INFO_G711] =
					{
						.cache_size 		= AUDIO_CACHE_SIZE,
						.cache_blk_size 	= AUDIO_CACHE_BLK_SIZE,
						.mem_size 			= AUDIO_MEM_SIZE,
						.mem_blk_size 		= AUDIO_BLK_SIZE,
						.ext_size 			= 0,
					},
					.totalsizes = 0,

				},
	[MEM_LAYOUT_4] = {	//dual stream D1+D1 usecases with JPEG
					.profiles[VIDOE_INFO_MJPG] =
					{
						.cache_size 		= 0x500000,
						.cache_blk_size 	= (50*1024),
						.mem_size 			= 0x500000,
						.mem_blk_size 		= (50*1024),
						.ext_size 			= 0,
					},
					.profiles[VIDOE_INFO_MP4] =
					{
						.cache_size 		= 0x500000,
						.cache_blk_size 	= (30*1024),
						.mem_size 			= 0x500000,
						.mem_blk_size 		= (30*1024),
						.ext_size 			= MP4_1_EXTRA_SIZE,
					},
					.profiles[VIDOE_INFO_MP4_EXT] =
					{
						.cache_size 		= 0x000000,
						.cache_blk_size 	= (30*1024),
						.mem_size 			= 0x000000,
						.mem_blk_size 		= (30*1024),
						.ext_size 			= MP4_2_EXTRA_SIZE,
					},
					.profiles[AUDIO_INFO_G711] =
					{
						.cache_size 		= AUDIO_CACHE_SIZE,
						.cache_blk_size 	= AUDIO_CACHE_BLK_SIZE,
						.mem_size 			= AUDIO_MEM_SIZE,
						.mem_blk_size 		= AUDIO_BLK_SIZE,
						.ext_size 			= 0,
					},
					.totalsizes = 0,

				},
	[MEM_LAYOUT_5] = {	//dual stream cases without JPEG
					.profiles[VIDOE_INFO_MJPG] =
					{
						.cache_size 		= 0x000000,
						.cache_blk_size 	= (50*1024),
						.mem_size 			= 0x000000,
						.mem_blk_size 		= (50*1024),
						.ext_size 			= 0,
					},
					.profiles[VIDOE_INFO_MP4] =
					{
						.cache_size 		= 0x700000,
						.cache_blk_size 	= (30*1024),
						.mem_size 			= 0x700000,
						.mem_blk_size 		= (30*1024),
						.ext_size 			= MP4_1_EXTRA_SIZE,
					},
					.profiles[VIDOE_INFO_MP4_EXT] =
					{
						.cache_size 		= 0x300000,
						.cache_blk_size 	= (20*1024),
						.mem_size 			= 0x300000,
						.mem_blk_size 		= (20*1024),
						.ext_size 			= MP4_2_EXTRA_SIZE,
					},
					.profiles[AUDIO_INFO_G711] =
					{
						.cache_size 		= AUDIO_CACHE_SIZE,
						.cache_blk_size 	= AUDIO_CACHE_BLK_SIZE,
						.mem_size 			= AUDIO_MEM_SIZE,
						.mem_blk_size 		= AUDIO_BLK_SIZE,
						.ext_size 			= 0,
					},
					.totalsizes = 0,

				},
	[MEM_LAYOUT_6] = {	// dual stream cases with JPEG
					.profiles[VIDOE_INFO_MJPG] =
					{
						.cache_size 		= 0x300000,
						.cache_blk_size 	= (50*1024),
						.mem_size 			= 0x300000,
						.mem_blk_size 		= (50*1024),
						.ext_size 			= 0,
					},
					.profiles[VIDOE_INFO_MP4] =
					{
						.cache_size 		= 0x700000,
						.cache_blk_size 	= (30*1024),
						.mem_size 			= 0x700000,
						.mem_blk_size 		= (30*1024),
						.ext_size 			= MP4_1_EXTRA_SIZE,
					},
					.profiles[VIDOE_INFO_MP4_EXT] =
					{
						.cache_size 		= 0x000000,
						.cache_blk_size 	= (30*1024),
						.mem_size 			= 0x000000,
						.mem_blk_size 		= (30*1024),
						.ext_size 			= MP4_2_EXTRA_SIZE,
					},
					.profiles[AUDIO_INFO_G711] =
					{
						.cache_size 		= AUDIO_CACHE_SIZE,
						.cache_blk_size 	= AUDIO_CACHE_BLK_SIZE,
						.mem_size 			= AUDIO_MEM_SIZE,
						.mem_blk_size 		= AUDIO_BLK_SIZE,
						.ext_size 			= 0,
					},
					.totalsizes = 0,

				},
	[MEM_LAYOUT_7] = { //tristream usecases
					.profiles[VIDOE_INFO_MJPG] =
					{
						.cache_size 		= MJPG_CACHE_SIZE,
						.cache_blk_size 	= MJPG_CACHE_BLK_SIZE,
						.mem_size 			= MJPG_MEM_SIZE,
						.mem_blk_size 		= MJPG_BLK_SIZE,
						.ext_size 			= 0,
					},

					.profiles[VIDOE_INFO_MP4] =
					{
						.cache_size 		= MP4_CACHE_SIZE,
						.cache_blk_size 	= MP4_CACHE_BLK_SIZE,
						.mem_size 			= MP4_MEM_SIZE,
						.mem_blk_size 		= MP4_BLK_SIZE,
						.ext_size 			= MP4_1_EXTRA_SIZE,
					},
					.profiles[VIDOE_INFO_MP4_EXT] =
					{
						.cache_size 		= MP4_EXT_CACHE_SIZE,
						.cache_blk_size 	= MP4_EXT_CACHE_BLK_SIZE,
						.mem_size 			= MP4_EXT_MEM_SIZE,
						.mem_blk_size 		= MP4_EXT_BLK_SIZE,
						.ext_size 			= MP4_2_EXTRA_SIZE,
					},
					.profiles[AUDIO_INFO_G711] =
					{
						.cache_size 		= AUDIO_CACHE_SIZE,
						.cache_blk_size 	= AUDIO_CACHE_BLK_SIZE,
						.mem_size 			= AUDIO_MEM_SIZE,
						.mem_blk_size 		= AUDIO_BLK_SIZE,
						.ext_size 			= 0,
					},
					.totalsizes = 0,
				}
};


VIDEO_BLK_INFO	Video_info_default[VIDOE_INFO_END] =
{
	{
		.video_type = VIDEO_MJPG,
		.size		= MJPG_MEM_SIZE,
		.extradata	= NULL,
		.extrasize	= 0,
		.IsCache	= 0,
		.blk_sz		= MJPG_BLK_SIZE,
		.blk_num	= (MJPG_MEM_SIZE/MJPG_BLK_SIZE),
		.blk_free	= (MJPG_MEM_SIZE/MJPG_BLK_SIZE),
		.frame_num	= 0,
		.cur_frame	= -1,
		.cur_serial = -1,
		.cur_blk	= -1,
		.timestamp	= -1,
		.gop		= {
						.last_Start = -1,
						.last_Start_serial = -1,
						.last_End = -1,
						.last_End_serial = -1,
						.lastest_I = -1,
						.lastest_I_serial = -1,
					},
		.frame		= NULL,

	},
	{
		.video_type = VIDEO_MP4,
		.size		= MP4_MEM_SIZE,
		.extradata	= NULL,
		.extrasize	= MP4_1_EXTRA_SIZE,
		.IsCache	= 0,
		.blk_sz		= MP4_BLK_SIZE,
		.blk_num	= (MP4_MEM_SIZE/MP4_BLK_SIZE),
		.blk_free	= (MP4_MEM_SIZE/MP4_BLK_SIZE),
		.frame_num	= 0,
		.cur_frame	= -1,
		.cur_serial = -1,
		.cur_blk	= -1,
		.timestamp	= -1,
		.gop		= {
						.last_Start = -1,
						.last_Start_serial = -1,
						.last_End = -1,
						.last_End_serial = -1,
						.lastest_I = -1,
						.lastest_I_serial = -1,
					},
		.frame		= NULL,

	},
	{
		.video_type = VIDEO_MP4,
		.size		= MP4_EXT_MEM_SIZE,
		.extradata	= NULL,
		.extrasize	= MP4_2_EXTRA_SIZE,
		.IsCache	= 0,
		.blk_sz		= MP4_EXT_BLK_SIZE,
		.blk_num	= (MP4_EXT_MEM_SIZE/MP4_EXT_BLK_SIZE),
		.blk_free	= (MP4_EXT_MEM_SIZE/MP4_EXT_BLK_SIZE),
		.frame_num	= 0,
		.cur_frame	= -1,
		.cur_serial = -1,
		.cur_blk	= -1,
		.timestamp	= -1,
		.gop		= {
						.last_Start = -1,
						.last_Start_serial = -1,
						.last_End = -1,
						.last_End_serial = -1,
						.lastest_I = -1,
						.lastest_I_serial = -1,
					},
		.frame		= NULL,

	},
	{
		.video_type = AUDIO_G711,
		.size		= AUDIO_MEM_SIZE,
		.extradata	= NULL,
		.extrasize	= 0,
		.IsCache	= 0,
		.blk_sz		= AUDIO_BLK_SIZE,
		.blk_num	= (AUDIO_MEM_SIZE/AUDIO_BLK_SIZE),
		.blk_free	= (AUDIO_MEM_SIZE/AUDIO_BLK_SIZE),
		.frame_num	= 0,
		.cur_frame	= -1,
		.cur_serial = -1,
		.cur_blk	= -1,
		.timestamp	= -1,
		.gop		= {
						.last_Start = -1,
						.last_Start_serial = -1,
						.last_End = -1,
						.last_End_serial = -1,
						.lastest_I = -1,
						.lastest_I_serial = -1,
					},
		.frame		= NULL,

	}
};

/**
 * @brief	Reset video frame
 * @param	"VIDEO_FRAME *pframe" : video frame
 * @return	none
 */
void Video_Frame_reset( VIDEO_FRAME *pframe)
{
	int cnt = 0;

	pframe->fram_type	= EMPTY_FRAME;
	pframe->serial 		= -1;
	pframe->blkindex	= -1;
	pframe->blks		= -1;
	pframe->realsize	= -1;
	pframe->flag		= 0;
	pframe->timestamp	= 0;
	for( cnt = 0; cnt < VIDOE_INFO_END; cnt++ )
	{
		pframe->ref_serial[cnt] = -1;
	}
}

/**
 * @brief	Video memory allocate
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	0 : success ; -1 : fail
 */
int Video_Mem_Malloc( MEM_MNG_INFO *pInfo )
{
	int	cnt = 0;
	int loops = 0;
	int cnt2 = 0;

	loops = sizeof( Video_info_default )/ sizeof( Video_info_default[0] );

	pInfo->video_info_nums  = loops;
	pInfo->video_info		= calloc( pInfo->video_info_nums, sizeof(VIDEO_BLK_INFO) );
	memcpy( pInfo->video_info, Video_info_default, sizeof( Video_info_default ) );

	for( cnt = 0; cnt < loops ; cnt++ )
	{
		pInfo->video_info[cnt].start 		= pInfo->start_addr + pInfo->offset;
		pInfo->video_info[cnt].start_phy 	= pInfo->start_phyAddr + pInfo->offset;;
		pInfo->video_info[cnt].frame_num	= pInfo->video_info[cnt].blk_num;
		pInfo->video_info[cnt].frame		= calloc( 	pInfo->video_info[cnt].blk_num ,
														sizeof(VIDEO_FRAME) );


		for( cnt2 = 0; cnt2 < pInfo->video_info[cnt].blk_num ; cnt2++ )
		{
			Video_Frame_reset( &(pInfo->video_info[cnt].frame[cnt2]) );

		}

		pInfo->freesize -= pInfo->video_info[cnt].size;
		pInfo->offset	+= pInfo->video_info[cnt].size;

		if( pInfo->video_info[cnt].extrasize > 0 )
		{
			pInfo->video_info[cnt].extradata = (void *)(pInfo->start_addr + pInfo->offset);
			pInfo->freesize -= pInfo->video_info[cnt].extrasize;
			pInfo->offset	+= pInfo->video_info[cnt].extrasize;
		}
		if( pInfo->freesize < 0 || pInfo->video_info[cnt].frame == NULL)
		{
			__E("pInfo->freesize = %d\n",pInfo->freesize);
			__E("Video_Mem_Malloc FAIL\n");
				return -1;
		}

	}

	return 0;


}

/**
 * @brief	Free video memory
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	0 : success ; -1 : fail
 */
int Video_Mem_Free( MEM_MNG_INFO *pInfo )
{
	int	cnt = 0;


	pInfo->video_info_nums = sizeof( Video_info_default )/ sizeof( Video_info_default[0] );

	if( pInfo->video_info == NULL )
	{
		return -1;
	}

	for( cnt = 0; cnt < pInfo->video_info_nums ; cnt++ )
	{
		if( pInfo->video_info[cnt].frame )
			free(pInfo->video_info[cnt].frame);
	}
	return 0;
}

int MemMng_Mem_layout( int mem_layout )
{
	extern CACHE_MNG	Cache_mng_default[VIDOE_INFO_END];
	int cnt = 0;
	MEM_LAYOUT *pLayout = NULL;
	int totalsize = 0;
	int IsPrint = 1;

	if( mem_layout >= MEM_LAYOUT_NUM || mem_layout < 0)
	{
		mem_layout = MEM_LAYOUT_DEFAULT;
	}

	pLayout = &mem_layout_setting[mem_layout]; //&mem_layout_setting[7]
	if( IsPrint )
	{
		printf("\n mem_layout        = %d \n", mem_layout);// mem_layout = 7
	}

	for( cnt = 0; cnt < VIDOE_INFO_END; cnt++ ) //VIDOE_INFO_END = 4
	{
			Video_info_default[cnt].size		= pLayout->profiles[cnt].mem_size;
			Video_info_default[cnt].extrasize	= pLayout->profiles[cnt].ext_size;
			Video_info_default[cnt].blk_sz		= pLayout->profiles[cnt].mem_blk_size;
			Video_info_default[cnt].blk_num		= pLayout->profiles[cnt].mem_size/pLayout->profiles[cnt].mem_blk_size;
			Video_info_default[cnt].blk_free	= pLayout->profiles[cnt].mem_size/pLayout->profiles[cnt].mem_blk_size;

			Cache_mng_default[cnt].size			= pLayout->profiles[cnt].cache_size;
			Cache_mng_default[cnt].blk_sz		= pLayout->profiles[cnt].cache_blk_size;
			Cache_mng_default[cnt].blk_num		= (pLayout->profiles[cnt].cache_size/pLayout->profiles[cnt].cache_blk_size);
			Cache_mng_default[cnt].cache_num	= (pLayout->profiles[cnt].cache_size/pLayout->profiles[cnt].cache_blk_size);

			totalsize += Video_info_default[cnt].size;
			totalsize += Video_info_default[cnt].extrasize;
			totalsize += Cache_mng_default[cnt].size;

			if( IsPrint )
			{
				printf("mem_size[%d]       =%8d byte = %f M\n",cnt, (unsigned int)Video_info_default[cnt].size, (unsigned int)Video_info_default[cnt].size/1024/1024.0);
				printf("mem_blk_size[%d]   =%8d  \n",cnt,Video_info_default[cnt].blk_sz);
				printf("cache_size[%d]     =%8d  \n",cnt,Cache_mng_default[cnt].size);
				printf("cache_blk_size[%d] =%8d  \n",cnt,Cache_mng_default[cnt].blk_sz);
			}
	}
	if( IsPrint )
	{
		printf("totalsize          =%8d byte = %f M \n", totalsize, totalsize/1024/1024.0);
		printf("TOTAL_MEM_SIZE     =%8d byte = %f M \n", TOTAL_MEM_SIZE, TOTAL_MEM_SIZE/1024/1024.0);
	}

	return totalsize;

}



/**
 * @brief	Memory manager initialization
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	0 : success ; -1 : fail
 */
int MemMng_Init( MEM_MNG_INFO *pInfo )
{
	int mem_layout = pInfo->mem_layout;
	memset( pInfo, 0 , sizeof(MEM_MNG_INFO) );

	pInfo->totalsize		= MemMng_Mem_layout(mem_layout);
	pInfo->start_addr		= (unsigned long)STREAM_CMEM_ALLOC(pInfo->totalsize, 32);

	if( pInfo->start_addr == 0 )
		goto MEM_INIT_FAIL;

	pInfo->start_phyAddr	= CMEM_getPhys((void *)pInfo->start_addr);
	if(pInfo->start_phyAddr == 0)
		fprintf(stderr, "Failed to get physical address of %#x\n",
                    (unsigned int) pInfo->start_phyAddr);
	pInfo->freesize			= pInfo->totalsize;
	pInfo->offset			= 0;
	pInfo->video_info_nums	= 0;

	if(Video_Mem_Malloc( pInfo )==0)
	__D(" MEM_INIT Success \n");
	else
        return -1;

  	MemMng_memcpy_open();

	return 0;

MEM_INIT_FAIL:
	__E(" MEM_INIT_FAIL \n");
	return -1;
}



/**
 * @brief	Release memory manager
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	0 : success ; -1 : fail
 */
int MemMng_release( MEM_MNG_INFO *pInfo )
{
	Video_Mem_Free( pInfo );
	if(pInfo->video_info)
	{
		free(pInfo->video_info);
		pInfo->video_info = NULL;
	}
	if( pInfo->start_addr )
	{
		OSA_cmemFree((void *)pInfo->start_addr);
		pInfo->start_addr = 0;
	}

	MemMng_memcpy_close();

	return 0;
}

/**
 * @brief	Open memory copy
 * @param	none
 * @return	0 : success ; -1 : fail
 */
int MemMng_memcpy_open(void)
{
#if	MEMMNG_DMA

	DRV_dmaOpen(&dmaHndl, DRV_DMA_MODE_NORMAL, 1);

#endif

	return 0;
}


/**
 * @brief	Do memory copy
 * @param	"void *pDes" : destination address
 * @param	"void *pSrc" : source address
 * @param	"int size" : size
 * @return	0 : success ; others : fail
 */
int MemMng_memcpy(void *pDes, void *pSrc, int size )
{
#if	MEMMNG_DMA

	DRV_DmaCopy1D copy1D;

	Sem_lock( STREAM_SEM_MEMCPY);

	copy1D.srcPhysAddr 	= (unsigned long)DRV_dmaGetPhysAddr((unsigned long)pSrc);
	copy1D.dstPhysAddr 	= (unsigned long)DRV_dmaGetPhysAddr((unsigned long)pDes);
	copy1D.size 				=	size ;
	DRV_dmaCopy1D(&dmaHndl, &copy1D, 1);

	Sem_unlock( STREAM_SEM_MEMCPY);

#else

	memcpy( pDes, pSrc, size );
#endif

	return 0;
}

/**
 * @brief	Close memory copy
 * @param	none
 * @return	0 : success
 */
int MemMng_memcpy_close(void)
{
#if	MEMMNG_DMA

	DRV_dmaClose(&dmaHndl);

#endif
	return 0;

}


/**
 * @brief	Free video frame
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @param	"int frame_id" : frame ID
 * @return	0 : success ; -1 : fail
 */
int MemMng_VidFrame_Free( VIDEO_BLK_INFO *pVidInfo, int frame_id)
{
	int blkStart = -1;

	if( frame_id >= pVidInfo->frame_num || frame_id < 0 )
	{
		__E("invalidate frame_id\n");
		return -1;
	}

	if( pVidInfo->frame[frame_id].fram_type == EMPTY_FRAME )
	{
		__D("free frame already\n");
		return 0;
	}

	if( pVidInfo->frame[frame_id].blks > 0 )
	{
		pVidInfo->blk_free += pVidInfo->frame[frame_id].blks;
		blkStart = pVidInfo->frame[frame_id].blkindex;
	}

	if( pVidInfo->gop.last_Start > 0 )
	{
		if( pVidInfo->gop.last_Start == frame_id )
		{
			pVidInfo->gop.last_Start		= -1;
			pVidInfo->gop.last_Start_serial	= -1;
			pVidInfo->gop.last_End			= -1;
			pVidInfo->gop.last_End_serial	= -1;
		}
	}

	if( pVidInfo->gop.lastest_I > 0 )
	{
		if( pVidInfo->gop.lastest_I == frame_id )
		{
			pVidInfo->gop.lastest_I = -1;
			pVidInfo->gop.last_End_serial = -1;

		}
	}

	Video_Frame_reset( &(pVidInfo->frame[frame_id]) );

	return blkStart;
}

/**
 * @brief	Set flag to video frame
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	none
 */
void MemMng_VidFrame_SetFlag(VIDEO_BLK_INFO *pVidInfo)
{
	int last_frame = 0;
	int frame_type = pVidInfo->frame[pVidInfo->cur_frame].fram_type;

	if(  frame_type == I_FRAME && pVidInfo->video_type == VIDEO_MP4 )
	{
		pVidInfo->frame[pVidInfo->cur_frame].flag = 0x0001;

		/* check if the first frame */
		if( ( pVidInfo->cur_frame - 1 ) > 0 )
		{
			last_frame = pVidInfo->cur_frame - 1;
		}else{
			last_frame = pVidInfo->frame_num - 1;
		}
		if( pVidInfo->frame[last_frame].fram_type == P_FRAME ||
			pVidInfo->frame[last_frame].fram_type == B_FRAME 	)
		{
			pVidInfo->frame[last_frame].flag = 0x0002;
		}

	}
	else
	{
		pVidInfo->frame[pVidInfo->cur_frame].flag = 0;
	}

}


void printheader(char *pData, int size)
{
	int cnt = 0;

	printf("\n\nprintheader = %d\n",size);
	for(cnt = 0 ;cnt < size; cnt++ )
	{
		if( cnt!=0 && cnt%4 == 0 )
		printf("\n");

		printf(" 0x%X ",*pData++);

	}
	printf("\n");
}

/**
 * @brief	Insert video frame
 * @param	"void *pData"
 * @param	"int size"
 * @param	"int blks"
 * @param	"int frame_type"
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	0 : success ; -1 : fail
 */
int MemMng_VidFrame_Insert( void *pData, int size, int blks,
							int frame_type, VIDEO_BLK_INFO *pVidInfo)
{

	if( pVidInfo == NULL ||
		frame_type >= END_FRAME_TYPE || blks <= 0 )
	{
		return -1;
	}
	if( frame_type != DUMMY_FRAME )
	{
		pVidInfo->cur_serial++;
	}
	pVidInfo->cur_blk	= ( pVidInfo->cur_blk+1 ) % pVidInfo->blk_num;
	pVidInfo->cur_frame = ( pVidInfo->cur_frame+1 )% pVidInfo->frame_num;

	pVidInfo->frame[pVidInfo->cur_frame].serial		= pVidInfo->cur_serial;
	pVidInfo->frame[pVidInfo->cur_frame].fram_type	= frame_type;
	pVidInfo->frame[pVidInfo->cur_frame].blkindex	= pVidInfo->cur_blk;
	pVidInfo->frame[pVidInfo->cur_frame].blks		= blks;
	pVidInfo->frame[pVidInfo->cur_frame].realsize	= size;
	pVidInfo->frame[pVidInfo->cur_frame].timestamp	= pVidInfo->timestamp;
	pVidInfo->frame[pVidInfo->cur_frame].temporalId	= pVidInfo->temporalId;

	if( frame_type != DUMMY_FRAME )
	{
		unsigned long WriteAddr = pVidInfo->start +
									pVidInfo->cur_blk*pVidInfo->blk_sz;
		if( !WriteAddr || pData == NULL )
		{
			__E("MemMng_memcpy fail!!\n");
			return -1;
		}

		MemMng_memcpy( (void *)WriteAddr, pData , size );

	}

	if( frame_type == I_FRAME && pVidInfo->video_type == VIDEO_MP4)
	{
		if( pVidInfo->gop.last_Start < 0 )
		{
			pVidInfo->gop.last_Start =
					pVidInfo->gop.lastest_I =
									pVidInfo->cur_frame;

			pVidInfo->gop.last_Start_serial =
					pVidInfo->gop.lastest_I_serial =
									pVidInfo->cur_serial;

			pVidInfo->gop.last_End =
					pVidInfo->gop.last_End_serial = -1;

		}else{

			pVidInfo->gop.last_Start = pVidInfo->gop.lastest_I;
			pVidInfo->gop.last_Start_serial = pVidInfo->gop.lastest_I_serial;
			if( (pVidInfo->cur_frame - 1) > 0 )
			{
				pVidInfo->gop.last_End = pVidInfo->cur_frame - 1;
			}else{
				pVidInfo->gop.last_End = pVidInfo->frame_num - 1;
			}
			pVidInfo->gop.last_End_serial = pVidInfo->cur_serial - 1;

			pVidInfo->gop.lastest_I = pVidInfo->cur_frame;
			pVidInfo->gop.lastest_I_serial = pVidInfo->cur_serial;

		}
	}
	if(pVidInfo->cur_serial == 0 && frame_type != DUMMY_FRAME)
	{
		if(pVidInfo->extradata != NULL && pVidInfo->extrasize != 0)
		{
			int cnt = 0;
			int loops = pVidInfo->extrasize;

			for( cnt = 0; cnt < loops ; cnt++ )
			{
				char *pCur = (char *)pData+cnt;
				if( *pCur == 0 &&
					*(pCur+1) == 0 &&
					*(pCur+2) == 0 &&
					*(pCur+3) == 1 &&
					*(pCur+4) == 0x65 )
				{
					//printf("cnt = %d ~~~~~~~~~~\n",cnt);
					break;
				}

				if( *pCur == 0 &&
					*(pCur+1) == 0 &&
					*(pCur+2) == 1 &&
					*(pCur+3) == 0xB6  )
				{
					//printf("cnt = %d ~~~~~~~~~~\n",cnt);
					break;
				}

			}
			pVidInfo->extrasize = cnt;
			memcpy(pVidInfo->extradata,pData,pVidInfo->extrasize);
			//printheader(pData,pVidInfo->extrasize);
		}
	}

	MemMng_VidFrame_SetFlag(pVidInfo);

	/* Update  global info */
	pVidInfo->cur_blk = (pVidInfo->cur_blk + blks - 1) % pVidInfo->blk_num;
	pVidInfo->blk_free -= blks;

	return 0;
}



/**
 * @brief	Video write
 * @param	"void *pData"
 * @param	"int size"
 * @param	"int frame_type"
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	0 : success ; -1 : fail
 */
int MemMng_Video_Write( void *pData, int size, int frame_type,
						VIDEO_BLK_INFO *pVidInfo )
{

	int blk_use = (size + pVidInfo->blk_sz-1) / pVidInfo->blk_sz;
	int IsContiguousBlk = 0;
	int	free_cnt = 0;

	if( blk_use > pVidInfo->blk_num || size == 0 )
	{
		goto MEM_MNG_WRITE_FAIL;
	}

	do{

		if( blk_use > pVidInfo->blk_free )
		{
			int nextframe = pVidInfo->cur_frame + 1;
			int freeframe = (nextframe + free_cnt)%pVidInfo->frame_num;
			/* Not enough free blk */
			if( MemMng_VidFrame_Free(  pVidInfo,  freeframe) < 0 )
			{
				goto MEM_MNG_WRITE_FAIL;
			}else{
				free_cnt++;
				continue;
			}
		}else{
			int nextblk = (pVidInfo->cur_blk + 1)%pVidInfo->blk_num;
			if( (nextblk+ blk_use ) > pVidInfo->blk_num )
			{
				/* No Contiguous Blk */
				int dummy_blk_cnt = pVidInfo->blk_num - nextblk;
				if( MemMng_VidFrame_Insert( NULL, 0, dummy_blk_cnt, DUMMY_FRAME, pVidInfo ) < 0 )
				{
					goto MEM_MNG_WRITE_FAIL;
				}else{
					free_cnt = 0;
					continue;
				}

			}else{
				IsContiguousBlk = 1;
			}

		}

	}while( !IsContiguousBlk );

	if( MemMng_VidFrame_Insert( pData, size, blk_use, frame_type, pVidInfo ) < 0 )
	{
		goto MEM_MNG_WRITE_FAIL;
	}



	return 0;

MEM_MNG_WRITE_FAIL:
	__E("MEM_MNG_WRITE_FAIL \n");
	return -1;
}


/**
 * @brief	Video read frame
 * @param	"void *pDest" : destination address
 * @param	"int *pSize" : size
 * @param	"int *pFrm_type" : frame type
 * @param	"int bufflimit" : buffer limit
 * @param	"VIDEO_FRAME *pFrame"
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	0 : success ; -1 : fail
 */
int MemMng_Video_ReadFrame( 	void *pDest, int *pSize, int *pFrm_type , int bufflimit,
								VIDEO_FRAME *pFrame, VIDEO_BLK_INFO *pVidInfo )
{


	if( pDest == NULL || pVidInfo == NULL || pFrame == NULL )
	{
		__E("MemMng_Video_ReadFrame: invalidate pointer\n");
		return -1;
	}

	if( pFrame->fram_type == DUMMY_FRAME || pFrame->fram_type == EMPTY_FRAME )
	{
		__E("MemMng_Video_ReadFrame: invalidate frame_type\n");
		return -1;
	}

	if( pFrame->realsize > bufflimit )
	{
		__E("MemMng_Video_ReadFrame: Over bufflimit\n");
		return -1;
	}

	void *pSrc = (void *)(pVidInfo->start +
							pFrame->blkindex*pVidInfo->blk_sz);

	*pSize 		= pFrame->realsize;
	*pFrm_type 	= pFrame->fram_type;

	MemMng_memcpy( pDest, pSrc, pFrame->realsize );


	return 0;
}

/**
 * @brief	Get video frame by serial
 * @param	"int serial" : serial no.
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	frame address ; NULL
 */
VIDEO_FRAME * MemMng_GetFrameBySerial( int serial , VIDEO_BLK_INFO *pVidInfo)
{
	if( serial < 0 || pVidInfo == NULL || serial > pVidInfo->cur_serial)
	{
		//__E("MemMng_GetFrameBySerial: Fail \n");
		return NULL;
	}

	int cnt = 0;
	if( serial == pVidInfo->frame[pVidInfo->cur_frame].serial )
	{
		return &(pVidInfo->frame[pVidInfo->cur_frame]);
	}

	for( cnt = 0; cnt < pVidInfo->frame_num ; cnt++ )
	{
		if( serial == pVidInfo->frame[cnt].serial )
		{
			if( pVidInfo->frame[cnt].fram_type == DUMMY_FRAME ||
				pVidInfo->frame[cnt].fram_type == EMPTY_FRAME )
			{
				//__E("MemMng_GetFrameBySerial: invalidate frame_type\n");
				//return NULL;
				continue;
			}
			return &(pVidInfo->frame[cnt]);
		}
	}
	__D("MemMng_GetFrameBySerial: Search no frame\n");
	return NULL;

}

/**
 * @brief	Get physical base address
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	physical base address
 */
unsigned long GetMemMngPhyBaseAddr(MEM_MNG_INFO *pInfo)
{
	if(pInfo == NULL)
		return 0;
	return pInfo->start_phyAddr;
}

/**
 * @brief	Get total memory size
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	totalsize
 */
unsigned long GetMemMngTotalMemSize(MEM_MNG_INFO *pInfo)
{
	if(pInfo == NULL)
		return 0;
	return pInfo->totalsize;
}

/**
 * @brief	Get current serial number
 * @param	"VIDEO_BLK_INFO *video_info"
 * @return	current serial number
 */
int GetCurrentSerialNo(VIDEO_BLK_INFO *video_info)
{
	return video_info->cur_serial;
}

/**
 * @brief	Get current frame
 * @param	"VIDEO_BLK_INFO *video_info"
 * @return	current frame
 */
VIDEO_FRAME * GetCurrentFrame(VIDEO_BLK_INFO *video_info)
{
	if(video_info == NULL)
		return NULL;
	if(video_info->cur_frame < 0)
		return NULL;
	return &(video_info->frame[video_info->cur_frame]);
}

/**
 * @brief	Get current frame
 * @param	"VIDEO_BLK_INFO *video_info"
 * @return	current frame
 */
VIDEO_FRAME * GetLastI_Frame(VIDEO_BLK_INFO *video_info)
{
	if(video_info == NULL)
		return NULL;
	if(video_info->gop.lastest_I < 0)
		return NULL;
	return &(video_info->frame[video_info->gop.lastest_I]);
}

/**
 * @brief	Get current offset
 * @param	"VIDEO_BLK_INFO *video_info"
 * @return	current offset
 */
int GetCurrentOffset(VIDEO_BLK_INFO *video_info)
{
	return video_info->cur_blk*video_info->blk_sz;
}
