#ifndef _CACHE_MNG_H_
#define _CACHE_MNG_H_

typedef struct _CACHE_MNG_BLK{
	int IsFree;
	int next;
}CACHE_MNG_BLK;

typedef struct _CACHE_MNG_INFO{
	int 			video_type;
	unsigned long 	start_addr;
	unsigned long 	start_phyAddr;
	int				totalsize;
	int				totalsize_phy;
	int				blksize;
	int				blknum;
	int				blkfree;
	CACHE_MNG_BLK	*cache;
} CACHE_MNG_INFO;


#endif
