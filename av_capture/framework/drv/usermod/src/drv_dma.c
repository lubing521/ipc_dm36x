
#include <osa.h>
#include <drv_dma.h>
#include <dev_dma.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct {

  int fd;
  Bool isInitDone;

} DRV_DmaCtrl;

DRV_DmaCtrl gDRV_dmaCtrl = { -1, FALSE };

int DRV_dmaInit()
{
  char deviceName[20];

  if(!gDRV_dmaCtrl.isInitDone) {

    sprintf(deviceName, "/dev/%s", DMA_DRV_NAME);

    gDRV_dmaCtrl.fd = open(deviceName, O_RDWR);
    if(gDRV_dmaCtrl.fd<0)
      return OSA_EFAIL;

    gDRV_dmaCtrl.isInitDone = TRUE;
  }

  return OSA_SOK;
}

int DRV_dmaExit()
{
  return close(gDRV_dmaCtrl.fd);
}

int DRV_dmaOpen(DRV_DmaChHndl *hndl, int dmaMode, int maxTransfers)
{
  DMA_OpenClosePrm prm;
  Uint32 cmd;
  int status;

  prm.chId = -1;
  prm.mode = dmaMode;
  prm.maxTransfers = maxTransfers;

  cmd = DMA_IOCTL_CMD_MAKE(DMA_CMD_CH_OPEN);
  status = ioctl(gDRV_dmaCtrl.fd, cmd, &prm);

  if(status==0)
    hndl->chId = prm.chId;
  else
    hndl->chId = -1;

  return status;
}

int DRV_dmaClose(DRV_DmaChHndl *hndl) {
  DMA_OpenClosePrm prm;
  Uint32 cmd;
  int status;

  if(hndl->chId < 0)
    return OSA_EFAIL;

  prm.chId = hndl->chId;
  prm.mode = 0;
  prm.maxTransfers = 0;

  cmd = DMA_IOCTL_CMD_MAKE(DMA_CMD_CH_CLOSE);
  status = ioctl(gDRV_dmaCtrl.fd, cmd, &prm);

  hndl->chId = -1;

  return status;
}

int DRV_dmaCopyFillRun(DRV_DmaChHndl *hndl, void *pPrm, int count, int copyFillType)
{
  DMA_CopyFillPrm prm;
  Uint32 cmd;
  int status;

  if(hndl->chId < 0)
    return OSA_EFAIL;

  prm.chId = hndl->chId;
  prm.count = count;
  prm.copyFillType = copyFillType;
  prm.prm = pPrm;

  cmd = DMA_IOCTL_CMD_MAKE(prm.copyFillType);
  status = ioctl(gDRV_dmaCtrl.fd, cmd, &prm);

  return status;
}

int DRV_dmaCopy2D(DRV_DmaChHndl *hndl, DRV_DmaCopy2D *prm, int count )
{
  return DRV_dmaCopyFillRun(hndl, prm, count, DMA_CMD_COPY_2D);
}

int DRV_dmaCopy1D(DRV_DmaChHndl *hndl, DRV_DmaCopy1D *prm, int count )
{
  return DRV_dmaCopyFillRun(hndl, prm, count, DMA_CMD_COPY_1D);
}

int DRV_dmaFill2D(DRV_DmaChHndl *hndl, DRV_DmaFill2D *prm, int count )
{
  return DRV_dmaCopyFillRun(hndl, prm, count, DMA_CMD_FILL_2D);
}

int DRV_dmaFill1D(DRV_DmaChHndl *hndl, DRV_DmaFill1D *prm, int count )
{
  return DRV_dmaCopyFillRun(hndl, prm, count, DMA_CMD_FILL_1D);
}

unsigned long DRV_dmaGetPhysAddr(unsigned long virtAddr)
{
  DMA_GetPhysAddrPrm prm;
  Uint32 cmd;
  int status;

  prm.virtAddr = virtAddr;
  prm.physAddr = 0;

  cmd = DMA_IOCTL_CMD_MAKE(DMA_CMD_GET_PHYS_ADDR);
  status = ioctl(gDRV_dmaCtrl.fd, cmd, &prm);

  if(status==0)
    return prm.physAddr;

  return 0;
}

int DRV_dmaDemux(DRV_DmaChHndl *hndl, DRV_DmaDemux *pPrm)
{
  DMA_DemuxPrm prm;
  Uint32 cmd;
  int status;

  if(hndl->chId < 0)
    return OSA_EFAIL;

  prm.chId = hndl->chId;
  memcpy(&prm.prm, pPrm, sizeof(DRV_DmaDemux));

  cmd = DMA_IOCTL_CMD_MAKE(DMA_CMD_DEMUX);
  status = ioctl(gDRV_dmaCtrl.fd, cmd, &prm);

  return status;
}

#include <osa_cmem.h>
#include <osa_prf.h>

int DRV_dmaTestCopyFill(Uint8 *srcVirtAddr, Uint8 *dstVirtAddr, int size, int width, int height)
{
  int status, i, diff;
  DRV_DmaChHndl dmaHndl;
  Uint32 *pCurAddr;
  DRV_DmaCopy2D copy2D;
  DRV_DmaCopy1D copy1D;
  DRV_DmaFill2D fill2D;
  DRV_DmaFill1D fill1D;
  OSA_PrfHndl dmaPrf;
  int count=79, k;
  Uint8 *srcPhysAddr, *dstPhysAddr;

  srcPhysAddr = (Uint8*)DRV_dmaGetPhysAddr((unsigned long)srcVirtAddr);
  dstPhysAddr = (Uint8*)DRV_dmaGetPhysAddr((unsigned long)dstVirtAddr);

  if(srcPhysAddr==NULL||dstPhysAddr==NULL)
    return -1;

  memset(srcVirtAddr, 0, size);
  memset(dstVirtAddr, 0x55, size);

  pCurAddr = (Uint32*)srcVirtAddr;
  for(i=0; i<size/4; i++) {
    *pCurAddr++ = i;
  }

  status = DRV_dmaOpen(&dmaHndl, DRV_DMA_MODE_NORMAL, 1);
  if(status==OSA_SOK) {

    #if 1
    copy2D.srcPhysAddr = (unsigned long)srcPhysAddr;
    copy2D.dstPhysAddr = (unsigned long)dstPhysAddr;

    copy2D.copyWidth   = width;
    copy2D.copyHeight  = height;

    copy2D.srcOffsetH = width;
    copy2D.dstOffsetH = width;

    copy2D.skipH = 0;

    OSA_prfReset(&dmaPrf);
    OSA_prfBegin(&dmaPrf);

    for(k=0; k<count; k++)
      DRV_dmaCopy2D(&dmaHndl, &copy2D, 1);

    OSA_prfEnd(&dmaPrf, width*height*count);

    diff = memcmp(srcVirtAddr, dstVirtAddr, width*height);

    OSA_printf("\n");
    if(diff) {
      OSA_printf(" DRV TEST: DRV_dmaCopy2D() Verify ERROR (%d)\n", diff);
    } else {
      OSA_printf(" DRV TEST: DRV_dmaCopy2D() Verify SUCCESS \n");
    }

    OSA_prfPrint(&dmaPrf, "DMA Copy2D", OSA_PRF_PRINT_DEFAULT);

    memset(dstVirtAddr, 0x55, size);

    copy1D.srcPhysAddr = (unsigned long)srcPhysAddr;
    copy1D.dstPhysAddr = (unsigned long)dstPhysAddr;

    copy1D.size   = size-1;

    OSA_prfReset(&dmaPrf);
    OSA_prfBegin(&dmaPrf);

    for(k=0; k<count; k++)
      DRV_dmaCopy1D(&dmaHndl, &copy1D, 1);

    OSA_prfEnd(&dmaPrf, copy1D.size*count);

    diff = memcmp(srcVirtAddr, dstVirtAddr, copy1D.size);

    OSA_printf("\n");
    if(diff) {
      OSA_printf(" DRV TEST: DRV_dmaCopy1D() Verify ERROR (%d)\n", diff);
    } else {
      OSA_printf(" DRV TEST: DRV_dmaCopy1D() Verify SUCCESS \n");
    }

    OSA_prfPrint(&dmaPrf, "DMA Copy1D", OSA_PRF_PRINT_DEFAULT);

    memset(dstVirtAddr, 0x55, size);

    fill1D.dstVirtAddr = (unsigned long)dstVirtAddr;
    fill1D.dstPhysAddr = (unsigned long)dstPhysAddr;

    fill1D.size = size-4;
    fill1D.fillValue  = 0xFF80FE60;

    OSA_prfReset(&dmaPrf);
    OSA_prfBegin(&dmaPrf);

    for(k=0; k<count; k++)
      DRV_dmaFill1D(&dmaHndl, &fill1D, 1);

    OSA_prfEnd(&dmaPrf, fill1D.size*count);

    diff=0;
    pCurAddr = (Uint32*)dstVirtAddr;
    for(i=0;i<(fill1D.size)/4;i++) {
      if(*pCurAddr++ != fill1D.fillValue)
        diff++;
    }
    pCurAddr = (Uint32*)dstVirtAddr;

    OSA_printf("\n");
    if(diff) {
      OSA_printf(" DRV TEST: DRV_dmaFill1D() Verify ERROR (%d) (%x)\n", diff, pCurAddr[0]);
    } else {
      OSA_printf(" DRV TEST: DRV_dmaFill1D() Verify SUCCESS \n");
    }

    OSA_prfPrint(&dmaPrf, "DMA Fill1D", OSA_PRF_PRINT_DEFAULT);

    memset(dstVirtAddr, 0x55, size);

    fill2D.dstVirtAddr = (unsigned long)dstVirtAddr;
    fill2D.dstPhysAddr = (unsigned long)dstPhysAddr;

    fill2D.copyWidth = width;
    fill2D.copyHeight = height;
    fill2D.dstOffsetH = width;
    fill2D.fillValue  = 0xFF80FE7F;

    OSA_prfReset(&dmaPrf);
    OSA_prfBegin(&dmaPrf);

    for(k=0; k<count; k++)
      DRV_dmaFill2D(&dmaHndl, &fill2D, 1);

    OSA_prfEnd(&dmaPrf, width*height*count);

    diff=0;
    pCurAddr = (Uint32*)dstVirtAddr;
    for(i=0;i<(width*height)/4;i++) {
      if(*pCurAddr++ != fill2D.fillValue)
        diff++;
    }

    OSA_printf("\n");
    if(diff) {
      OSA_printf(" DRV TEST: DRV_dmaFill2D() Verify ERROR (%d)\n", diff);
    } else {
      OSA_printf(" DRV TEST: DRV_dmaFill2D() Verify SUCCESS \n");
    }

    OSA_prfPrint(&dmaPrf, "DMA Fill2D", OSA_PRF_PRINT_DEFAULT);

    memset(dstVirtAddr, 0x55, size);
    #endif

    status = DRV_dmaClose(&dmaHndl);
  }

  return status;
}

int DRV_dmaTestDemux(Uint8 *srcVirtAddr, Uint8 *dstVirtAddr, int size, int width, int height)
{
  int status, i, diff;
  DRV_DmaChHndl dmaHndl;
  DRV_DmaDemux demuxPrm;
  Uint32 *pCurAddr;
  Uint8 *pSrc, *pDst;
  OSA_PrfHndl dmaPrf;
  int count=120, k;
  Uint8 *srcPhysAddr, *dstPhysAddr;

  srcPhysAddr = OSA_cmemGetPhysAddr(srcVirtAddr);
  dstPhysAddr = OSA_cmemGetPhysAddr(dstVirtAddr);

  if(srcPhysAddr==NULL||dstPhysAddr==NULL)
    return -1;

  memset(srcVirtAddr, 0, size);
  memset(dstVirtAddr, 0x55, size);

  pCurAddr = (Uint32*)srcVirtAddr;
  for(i=0; i<size/4; i++) {
    *pCurAddr++ = i;
  }

  width = 720*2;
  height = 480;

  demuxPrm.numLines = height;
  demuxPrm.copyWidth = width;

  demuxPrm.srcChromaOffset = 0;
  demuxPrm.dstChromaOffset = 0;

  demuxPrm.srcPhysAddrList = OSA_memAlloc( sizeof(unsigned long)*demuxPrm.numLines );
  demuxPrm.dstPhysAddrList = OSA_memAlloc( sizeof(unsigned long)*demuxPrm.numLines );

  if(demuxPrm.srcPhysAddrList==NULL||demuxPrm.dstPhysAddrList==NULL)
  {
    OSA_ERROR(" DRV TEST: No memory\n");
    if(demuxPrm.srcPhysAddrList)
      OSA_memFree(demuxPrm.srcPhysAddrList);
    if(demuxPrm.dstPhysAddrList)
      OSA_memFree(demuxPrm.dstPhysAddrList);

    return -1;
  }

  pSrc = srcPhysAddr;
  pDst = dstPhysAddr;

  for(i=0; i<demuxPrm.numLines; i++) {
    demuxPrm.srcPhysAddrList[i] = (unsigned long)pSrc;
    demuxPrm.dstPhysAddrList[i] = (unsigned long)pDst;

    pSrc += width;
    pDst += width;
  }

  status = DRV_dmaOpen(&dmaHndl, DRV_DMA_MODE_DEMUX, height);
  if(status==OSA_SOK) {

    #if 1
    OSA_prfReset(&dmaPrf);
    OSA_prfBegin(&dmaPrf);

    for(k=0; k<count; k++)
      DRV_dmaDemux(&dmaHndl, &demuxPrm);

    OSA_prfEnd(&dmaPrf, width*height*count);

    diff = memcmp(srcVirtAddr, dstVirtAddr, width*height);

    OSA_printf("\n");
    if(diff) {
      OSA_printf(" DRV TEST: DRV_dmaDemux() Verify ERROR (%d)\n", diff);
    } else {
      OSA_printf(" DRV TEST: DRV_dmaDemux() Verify SUCCESS \n");
    }

    OSA_prfPrint(&dmaPrf, "DMA Demux", OSA_PRF_PRINT_DEFAULT);

    #endif

    DRV_dmaClose(&dmaHndl);
  }

  return status;
}

int DRV_dmaTestMain(int argc, char **argv)
{
  int status, size = 1*MB, width=1024, height = 1024;
  Uint8 *srcVirtAddr, *dstVirtAddr;

  status = CMEM_init();
  if(status<0) {
    OSA_ERROR(" DRV: ERROR: CMEM_init()\n");
    return status;
  }

  status = DRV_dmaInit();
  if(status<0) {
    OSA_ERROR(" DRV: ERROR: DRV_dmaInit()\n");
    return CMEM_exit();
  }

  srcVirtAddr = OSA_cmemAlloc(size, 32);
  dstVirtAddr = OSA_cmemAlloc(size, 32);

  if(srcVirtAddr==NULL||dstVirtAddr==NULL) {
    OSA_ERROR(" CMEM_heapAlloc()\n");
    goto error_exit;
  }

  DRV_dmaTestCopyFill(srcVirtAddr, dstVirtAddr, size, width, height);
  DRV_dmaTestDemux(srcVirtAddr, dstVirtAddr, size, width, height);

  OSA_printf("\n");

error_exit:
  if(srcVirtAddr)
    OSA_cmemFree(srcVirtAddr);

  if(dstVirtAddr)
    OSA_cmemFree(dstVirtAddr);

  DRV_dmaExit();
  CMEM_exit();

  return status;
}

