/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#ifndef _CSL_DMA_H
#define _CSL_DMA_H

#include <csl_soc.h>

#define CSL_DMA_IPIPE_BSC     0
#define CSL_DMA_IPIPE_HISTO   1
#define CSL_DMA_MAX           2

int CSL_dmaInit(void);
int CSL_dmaExit(void);
int CSL_dmaStart(int id);
int CSL_dmaStop(int id);
int CSL_dmaSetDstAddr(int id, Uint32 dstPhysAddr);
int CSL_dmaSetPrm(int id, Uint32 dstPhysAddr, Uint32 srcPhysAddr, Uint16 aCnt, Uint16 bCnt);
int CSL_dmaPrintCount(int id);
int CSL_dmaClearCount(int id);

#endif
