

#ifndef _OSA_CMEM_H_
#define _OSA_CMEM_H_

#include <osa.h>
#include <cmem.h>

Uint8 *OSA_cmemAllocCached(Uint32 size, Uint32 alignment);
Uint8 *OSA_cmemAlloc(Uint32 size, Uint32 alignment);
int    OSA_cmemFree(Uint8 *ptr);
Uint8 *OSA_cmemGetPhysAddr(Uint8 *virtAddr);

#define OSA_cmemCacheWb(virtPtr, size)    CMEM_cacheWb((void*)virtPtr, size)
#define OSA_cmemCacheInv(virtPtr, size)   CMEM_cacheInv((void*)virtPtr, size)
#define OSA_cmemCacheWbInv(virtPtr, size) CMEM_cacheWbInv((void*)virtPtr, size)

#endif  /*  _OSA_CMEM_H_  */

