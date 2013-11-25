
#ifndef _DRV_FRAME_COPY_H_
#define _DRV_FRAME_COPY_H_

#include <drv.h>
#include <drv_dma.h>

typedef struct {

  Uint8 *srcPhysAddr; ///< if NULL, do virtual to physical translation inside API
  Uint8 *srcVirtAddr; ///< if NULL, fill destination buffer with 'blank' color

  Uint8 *dstPhysAddr; ///< if NULL, do virtual to physical translation inside API
  Uint8 *dstVirtAddr; ///< cannot be NULL
  
  int srcOffsetH;    ///< Source Line Offset H, in pixels
  int srcOffsetV;    ///< Source Line Offset V, in lines, chorma offset is srcOffsetH*srcOffsetV

  int dstOffsetH;    ///< Source Line Offset H, in pixels
  int dstOffsetV;    ///< Source Line Offset V, in lines, chorma offset is dstOffsetH*dstOffsetV

  int copyWidth;     ///< in pixels
  int copyHeight;    ///< in lines
  
  int dataFormat;    ///< DRV_DATA_FORMAT_YUV422, DRV_DATA_FORMAT_YUV420
  
  int srcStartX;     ///< in pixels 
  int srcStartY;     ///< in lines
  
  int dstStartX;     ///< in pixels
  int dstStartY;     ///< in lines 

} DRV_FrameCopyPrm;


int DRV_frameCopyInit();
int DRV_frameCopyExit();


int DRV_frameCopy(DRV_DmaChHndl *dmaHndl, DRV_FrameCopyPrm *prm);




#endif /* _DRV_FRAME_COPY_H_  */
