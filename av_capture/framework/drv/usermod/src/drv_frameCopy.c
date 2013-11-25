
#include <drv_frameCopy.h>
#include <osa_mutex.h>

typedef struct {

  DRV_DmaChHndl dmaHndl;
  OSA_MutexHndl lock;

} DRV_FrameCopyObj;

DRV_FrameCopyObj gDRV_frameCopyObj;

int DRV_frameCopyInit()
{
  int status;
  
  status = OSA_mutexCreate(&gDRV_frameCopyObj.lock);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_mutexCreate()\n");
    return OSA_SOK;
  }
  
  status = DRV_dmaOpen(&gDRV_frameCopyObj.dmaHndl, DRV_DMA_MODE_NORMAL, 2);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_dmaOpen()\n");  
    OSA_mutexDelete(&gDRV_frameCopyObj.lock);
    return status;
  }

  return status;
}

int DRV_frameCopyExit()
{
  OSA_mutexDelete(&gDRV_frameCopyObj.lock);
  DRV_dmaClose(&gDRV_frameCopyObj.dmaHndl);
  
  return OSA_SOK;
}


int DRV_frameCopy(DRV_DmaChHndl *dmaHndl, DRV_FrameCopyPrm *prm)
{
  DRV_DmaCopy2D copy2D[2];
  DRV_DmaFill2D fill2D[2];
  unsigned long physAddr;
  Uint32 chromaOffset;
  int status;
  Bool doLock;

  if(prm->dstVirtAddr==NULL || prm->copyWidth<=0 || prm->copyHeight<=0 || prm->dstOffsetH<=0 || prm->dstOffsetV<=0
      || prm->dstStartX < 0 || prm->dstStartY < 0 || prm->srcStartX < 0 || prm->srcStartY < 0 
    ) {
    OSA_ERROR("Invalid Parameters!!!\n");
    return OSA_EFAIL;
  }
    
  if(dmaHndl==NULL) {
    doLock = TRUE;
    dmaHndl = &gDRV_frameCopyObj.dmaHndl;
  } else {
    doLock = FALSE;
  }
  
  prm->srcStartX = OSA_floor(prm->srcStartX, 2);
  prm->srcStartY = OSA_floor(prm->srcStartY, 2);
  prm->dstStartX = OSA_floor(prm->dstStartX, 2);
  prm->dstStartY = OSA_floor(prm->dstStartY, 2);
  prm->copyWidth = OSA_floor(prm->copyWidth, 2);
  prm->copyHeight= OSA_floor(prm->copyHeight, 2);
  
  if(prm->srcVirtAddr==NULL) {
    // do fill
    physAddr = (unsigned long)prm->dstPhysAddr;
    if(physAddr==(unsigned long)NULL)
      physAddr = DRV_dmaGetPhysAddr((unsigned long)prm->dstVirtAddr);

    fill2D[0].dstVirtAddr = (unsigned long)prm->dstVirtAddr;
    fill2D[0].dstPhysAddr = physAddr;
    fill2D[0].copyWidth = prm->copyWidth;
    fill2D[0].copyHeight = prm->copyHeight;
    fill2D[0].dstOffsetH = prm->dstOffsetH;
    fill2D[0].fillValue  = 0x00000000;    
    
    if(prm->dataFormat==DRV_DATA_FORMAT_YUV422) {
      fill2D[0].copyWidth  *= 2;
      fill2D[0].dstOffsetH *= 2;
      fill2D[0].fillValue  = 0x00800080;      
      fill2D[0].dstVirtAddr += prm->dstStartX*2 + prm->dstStartY*fill2D[0].dstOffsetH;
      fill2D[0].dstPhysAddr += prm->dstStartX*2 + prm->dstStartY*fill2D[0].dstOffsetH;                  
    } else {
      fill2D[0].dstVirtAddr += prm->dstStartX + prm->dstStartY*fill2D[0].dstOffsetH;
      fill2D[0].dstPhysAddr += prm->dstStartX + prm->dstStartY*fill2D[0].dstOffsetH;                  
    }
   
    if(prm->dataFormat==DRV_DATA_FORMAT_YUV420) {
      chromaOffset = prm->dstOffsetH*prm->dstOffsetV;
    
      fill2D[1].dstVirtAddr = (Uint32)prm->dstVirtAddr + chromaOffset;
      fill2D[1].dstPhysAddr = physAddr + chromaOffset;

      fill2D[1].dstVirtAddr += prm->dstStartX + prm->dstStartY/2*fill2D[0].dstOffsetH;
      fill2D[1].dstPhysAddr += prm->dstStartX + prm->dstStartY/2*fill2D[0].dstOffsetH;                  
      
      fill2D[1].copyWidth   = fill2D[0].copyWidth;
      fill2D[1].copyHeight  = fill2D[0].copyHeight/2;  
      fill2D[1].dstOffsetH  = fill2D[0].dstOffsetH;
      fill2D[1].fillValue = 0x80808080;
    }
    
    if(doLock)  
      OSA_mutexLock(&gDRV_frameCopyObj.lock);
    
    #if 0
    OSA_printf(" DRV: FrameCopy: D(%08lx %d) C(%d %d)\n", 
      fill2D[0].dstPhysAddr, fill2D[0].dstOffsetH, 
      fill2D[0].copyWidth  , fill2D[0].copyHeight
       );
       
    OSA_printf(" DRV: FrameCopy: D(%08lx %d) C(%d %d)\n", 
      fill2D[1].dstPhysAddr, fill2D[1].dstOffsetH, 
      fill2D[1].copyWidth  , fill2D[1].copyHeight
       );
    #endif  
      
    if(prm->dataFormat==DRV_DATA_FORMAT_YUV422) {
      status = DRV_dmaFill2D(dmaHndl, fill2D, 1);
    } else {
      status = DRV_dmaFill2D(dmaHndl, fill2D, 2);    
    }
    
    if(doLock)      
      OSA_mutexUnlock(&gDRV_frameCopyObj.lock);    
    
  } else {
  
    if(prm->srcOffsetH<=0 || prm->srcOffsetV<=0)
      return OSA_EFAIL;

    physAddr = (unsigned long)prm->dstPhysAddr;
    if(physAddr==(unsigned long)NULL)
      physAddr = DRV_dmaGetPhysAddr((unsigned long)prm->dstVirtAddr);

    copy2D[0].dstPhysAddr = physAddr;
    copy2D[0].skipH = 0;

    physAddr = (unsigned long)prm->srcPhysAddr;
    if(physAddr==(unsigned long)NULL)
      physAddr = DRV_dmaGetPhysAddr((unsigned long)prm->srcVirtAddr);

    copy2D[0].srcPhysAddr = physAddr;
    copy2D[0].copyWidth   = prm->copyWidth;
    copy2D[0].copyHeight  = prm->copyHeight;  
    copy2D[0].dstOffsetH  = prm->dstOffsetH;
    copy2D[0].srcOffsetH  = prm->srcOffsetH;
    
    if(prm->dataFormat==DRV_DATA_FORMAT_YUV422) {
      copy2D[0].copyWidth  *= 2;
      copy2D[0].dstOffsetH *= 2;     
      copy2D[0].srcOffsetH *= 2;      
      copy2D[0].srcPhysAddr += prm->srcStartX*2 + prm->srcStartY*copy2D[0].srcOffsetH;
      copy2D[0].dstPhysAddr += prm->dstStartX*2 + prm->dstStartY*copy2D[0].dstOffsetH;                  
    } else {
    
      copy2D[0].srcPhysAddr += prm->srcStartX + prm->srcStartY*copy2D[0].srcOffsetH;
      copy2D[0].dstPhysAddr += prm->dstStartX + prm->dstStartY*copy2D[0].dstOffsetH;                  
    }
    
    if(prm->dataFormat==DRV_DATA_FORMAT_YUV420) {
    
      physAddr = (unsigned long)prm->dstPhysAddr;
      if(physAddr==(unsigned long)NULL)
        physAddr = DRV_dmaGetPhysAddr((unsigned long)prm->dstVirtAddr);
    
      copy2D[1].dstPhysAddr = physAddr + prm->dstOffsetH*prm->dstOffsetV;
      
      physAddr = (unsigned long)prm->srcPhysAddr;
      if(physAddr==(unsigned long)NULL)
        physAddr = DRV_dmaGetPhysAddr((unsigned long)prm->srcVirtAddr);
      
      copy2D[1].srcPhysAddr = physAddr + prm->srcOffsetH*prm->srcOffsetV;
      
      copy2D[1].copyWidth   = copy2D[0].copyWidth;
      copy2D[1].copyHeight  = copy2D[0].copyHeight/2;  
      copy2D[1].dstOffsetH  = copy2D[0].dstOffsetH;
      copy2D[1].srcOffsetH  = copy2D[0].srcOffsetH;
      copy2D[1].skipH       = 0;
      
      copy2D[1].srcPhysAddr += prm->srcStartX + prm->srcStartY/2*copy2D[1].srcOffsetH;
      copy2D[1].dstPhysAddr += prm->dstStartX + prm->dstStartY/2*copy2D[1].dstOffsetH;                  
    }
    
    #if 0
    OSA_printf(" DRV: FrameCopy: S(%08lx %d) D(%08lx %d) C(%d %d %d)\n", 
      copy2D[0].srcPhysAddr, copy2D[0].srcOffsetH, 
      copy2D[0].dstPhysAddr, copy2D[0].dstOffsetH, 
      copy2D[0].copyWidth  , copy2D[0].copyHeight,      
      copy2D[0].skipH
       );
       
    OSA_printf(" DRV: FrameCopy: S(%08lx %d) D(%08lx %d) C(%d %d %d)\n", 
      copy2D[1].srcPhysAddr, copy2D[1].srcOffsetH, 
      copy2D[1].dstPhysAddr, copy2D[1].dstOffsetH, 
      copy2D[1].copyWidth  , copy2D[1].copyHeight,      
      copy2D[1].skipH
       );
    #endif

    if(doLock)
      OSA_mutexLock(&gDRV_frameCopyObj.lock);
    
    if(prm->dataFormat==DRV_DATA_FORMAT_YUV422) {
      status = DRV_dmaCopy2D(dmaHndl, copy2D, 1);
    } else {
      status = DRV_dmaCopy2D(dmaHndl, copy2D, 2);    
    }
    
    if(doLock)
      OSA_mutexUnlock(&gDRV_frameCopyObj.lock);
  }
  
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_dmaCopyFill2D() (chId = %d)\n", gDRV_frameCopyObj.dmaHndl.chId);
  }
  
  return status;
}
