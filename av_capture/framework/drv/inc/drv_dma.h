
#ifndef _DRV_DMA_H_
#define _DRV_DMA_H_

#define DRV_DMA_MODE_NORMAL     0   // normal memcpy or memfill type DMA's
#define DRV_DMA_MODE_DEMUX      1   // line-by-line demux DMA

#define DRV_DMA_MAX_NORMAL_TRANSFERS    (16)
#define DRV_DMA_MAX_DEMUX_TRANSFERS     (4100)

#define DRV_DMA_MAX_DEMUX_PARAM         (64) // must be multiple of 2, must be >=2

typedef struct {

  unsigned short numLines; 
  unsigned short copyWidth;

  int srcChromaOffset;
  int dstChromaOffset;
  
  unsigned long *srcPhysAddrList; // array of address's
  unsigned long *dstPhysAddrList; // array of address's

} DRV_DmaDemux;

typedef struct {

  unsigned long srcPhysAddr;
  unsigned long dstPhysAddr;
  
  int copyWidth;   // in bytes
  int copyHeight;  // in lines
  
  int srcOffsetH;  // in bytes
  int dstOffsetH;  // in bytes
  
  int skipH; // Horizontal Skip, 0: No skip, >= 1 copy 'skipH' bytes, skip 'skipH' bytes. Eg to skip alternate bytes set skipH to 1  
  
} DRV_DmaCopy2D;

typedef struct {

  unsigned long srcPhysAddr;
  unsigned long dstPhysAddr;
  
  int size;  // in bytes
  
} DRV_DmaCopy1D;

typedef struct {

  int fillValue;  // 32-bit fill value
  
  unsigned long dstPhysAddr;
  unsigned long dstVirtAddr;
  
  int copyWidth;   // in bytes, must be multiple of 4 bytes, must be >= 16 bytes
  int copyHeight;  // in lines

  int dstOffsetH; // in bytes, must be multiple of 16 bytes
  
} DRV_DmaFill2D;

typedef struct {

  int fillValue; // 32-bit fill value
  
  unsigned long dstPhysAddr;  
  unsigned long dstVirtAddr;  
  
  int size; // in bytes, must be multiple of 4 bytes, must be >= 32 bytes
  
} DRV_DmaFill1D;

#ifndef KERNEL_LEVEL
// only used in user space

typedef struct {

  int chId; // DMA channel ID, initialzed internally when DRV_dmaOpen() is called

} DRV_DmaChHndl;

int DRV_dmaInit();
int DRV_dmaExit();

int DRV_dmaOpen(DRV_DmaChHndl *hndl, int dmaMode, int maxTransfers);
int DRV_dmaClose(DRV_DmaChHndl *hndl);

// count must less <= maxTransfers
int DRV_dmaCopy2D(DRV_DmaChHndl *hndl, DRV_DmaCopy2D *prm, int count );
int DRV_dmaCopy1D(DRV_DmaChHndl *hndl, DRV_DmaCopy1D *prm, int count );
int DRV_dmaFill2D(DRV_DmaChHndl *hndl, DRV_DmaFill2D *prm, int count );
int DRV_dmaFill1D(DRV_DmaChHndl *hndl, DRV_DmaFill1D *prm, int count );

unsigned long DRV_dmaGetPhysAddr(unsigned long virtAddr);

int DRV_dmaDemux(DRV_DmaChHndl *hndl, DRV_DmaDemux *prm);

#endif  /* KERNEL_LEVEL */ 

#endif  /* _DRV_DMA_H_ */

