
#include <csl_ipipe.h>
#include <csl_sysDrv.h>
#include <csl_dma.h>

CSL_Status CSL_ipipeBscBufInit(CSL_IpipeHandle hndl, CSL_BufInit * bufInit)
{
  CSL_Status status;

  status = CSL_bufInit(&hndl->bscOutBuf, bufInit);

  hndl->bscCurBufInfo.id = CSL_BUF_ID_INVALID;
  
  CSL_dmaClearCount(CSL_DMA_IPIPE_BSC);
  CSL_dmaSetDstAddr(CSL_DMA_IPIPE_BSC, bufInit->bufAddr[0]);   

  return status;
}

CSL_Status CSL_ipipeBscBufSwitchEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
  hndl->bscOutBufSwitchEnable = enable;

  return CSL_SOK;
}

CSL_Status CSL_ipipeBscBufGetFull(CSL_IpipeHandle hndl, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout)
{
  CSL_Status status;

  status = CSL_bufGetFull(&hndl->bscOutBuf, buf, minBuf, timeout);

  return status;
}

CSL_Status CSL_ipipeBscBufPutEmpty(CSL_IpipeHandle hndl, CSL_BufInfo * buf)
{
  CSL_Status status;

  status = CSL_bufPutEmpty(&hndl->bscOutBuf, buf);

  return status;
}

CSL_Status CSL_ipipeBscBufSwitch(CSL_IpipeHandle hndl)
{
  CSL_Status status = CSL_SOK;

  hndl->bscCount++;

  if (hndl->bscOutBufSwitchEnable) {
    status = CSL_bufSwitchFull(&hndl->bscOutBuf, &hndl->bscCurBufInfo, 1, 0, hndl->bscCount);

    if (hndl->bscCurBufInfo.id != CSL_BUF_ID_INVALID) {
      CSL_dmaSetDstAddr(CSL_DMA_IPIPE_BSC, hndl->bscCurBufInfo.addr);    
    }
  }


  return status;
}

CSL_Status CSL_ipipeBoxcarBufInit(CSL_IpipeHandle hndl, CSL_BufInit * bufInit)
{
  CSL_Status status;

  status = CSL_bufInit(&hndl->boxcarOutBuf, bufInit);

  hndl->boxcarCurBufInfo.id = CSL_BUF_ID_INVALID;
  
  CSL_ipipeBoxcarSetOutAddr(hndl, (Uint8*)bufInit->bufAddr[0]);   

  return status;
}

CSL_Status CSL_ipipeBoxcarBufSwitchEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
  hndl->boxcarOutBufSwitchEnable = enable;

  return CSL_SOK;
}

CSL_Status CSL_ipipeBoxcarBufGetFull(CSL_IpipeHandle hndl, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout)
{
  CSL_Status status;

  status = CSL_bufGetFull(&hndl->boxcarOutBuf, buf, minBuf, timeout);

  return status;
}

CSL_Status CSL_ipipeBoxcarBufPutEmpty(CSL_IpipeHandle hndl, CSL_BufInfo * buf)
{
  CSL_Status status;

  status = CSL_bufPutEmpty(&hndl->boxcarOutBuf, buf);

  return status;
}

CSL_Status CSL_ipipeBoxcarBufSwitch(CSL_IpipeHandle hndl)
{
  CSL_Status status = CSL_SOK;

  hndl->boxcarCount++;

  if (hndl->boxcarOutBufSwitchEnable) {
    status = CSL_bufSwitchFull(&hndl->boxcarOutBuf, &hndl->boxcarCurBufInfo, 1, 0, hndl->boxcarCount);

    if (hndl->boxcarCurBufInfo.id != CSL_BUF_ID_INVALID) {
      CSL_ipipeBoxcarSetOutAddr(hndl, (Uint8*)hndl->boxcarCurBufInfo.addr);    
    }
  }

  return status;
}
