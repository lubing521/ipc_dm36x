#include <csl_usrDrv.h>
#include <csl_rszIoctl.h>
#include <csl_hndl.h>

CSL_Status CSL_rszOpen(CSL_RszHandle hndl)
{
  hndl->fd = CSL_usrDrvOpen(CSL_MODULE_RSZ);

  if (hndl->fd < 0)
    return CSL_EFAIL;

  hndl->regs = gCSL_vpssHndl.rszRegs;

  return CSL_SOK;
}

CSL_Status CSL_rszClose(CSL_RszHandle hndl)
{
  return CSL_usrDrvClose(hndl->fd);
}

CSL_Status CSL_rszBufInit(CSL_RszHandle hndl, Uint8 rszMod, CSL_BufInit * bufInit, CSL_RszBufConfig *bufConfig)
{
  CSL_RszBufInitPrm bufInitPrm;

  if(bufInit==NULL || bufConfig==NULL)
    return CSL_EFAIL;

  bufInitPrm.rszMod = rszMod;
  bufInitPrm.bufInit = bufInit;
  bufInitPrm.bufConfig = bufConfig;

  return CSL_usrDrvIoctl(hndl->fd, CSL_RSZ_CMD_BUF_INIT, &bufInitPrm);
}

CSL_Status CSL_rszBufSwitchEnable(CSL_RszHandle hndl, Uint8 rszMod, Bool32 enable)
{
  CSL_RszBufSwitchEnablePrm bufSwitchEnablePrm;

  bufSwitchEnablePrm.rszMod = rszMod;
  bufSwitchEnablePrm.enable = enable;

  return CSL_usrDrvIoctl(hndl->fd, CSL_RSZ_CMD_BUF_SWITCH_ENABLE, &bufSwitchEnablePrm);
}

CSL_Status CSL_rszBufGetFull(CSL_RszHandle hndl, Uint8 rszMod, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout)
{
  CSL_RszBufGetFullPrm bufGetFullPrm;

  bufGetFullPrm.rszMod = rszMod;
  bufGetFullPrm.buf = buf;
  bufGetFullPrm.minBuf = minBuf;
  bufGetFullPrm.timeout = timeout;

  return CSL_usrDrvIoctl(hndl->fd, CSL_RSZ_CMD_BUF_GET_FULL, &bufGetFullPrm);
}

CSL_Status CSL_rszBufPutEmpty(CSL_RszHandle hndl, Uint8 rszMod, CSL_BufInfo * buf)
{
  CSL_RszBufPutEmptyPrm bufPutEmptyPrm;

  if(buf==NULL)
    return CSL_EFAIL;

  bufPutEmptyPrm.rszMod = rszMod;
  bufPutEmptyPrm.buf = buf;

  return CSL_usrDrvIoctl(hndl->fd, CSL_RSZ_CMD_BUF_PUT_EMPTY, &bufPutEmptyPrm);
}

CSL_Status CSL_rszIntEnable(CSL_RszHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_RSZ_CMD_INT_ENABLE, (void *) enable);
}

CSL_Status CSL_rszIntClear(CSL_RszHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_RSZ_CMD_INT_CLEAR, NULL);
}

CSL_Status CSL_rszIntWait(CSL_RszHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_RSZ_CMD_INT_WAIT, NULL);
}

CSL_Status CSL_rszLock(CSL_RszHandle hndl, Uint32 timeout)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_LOCK, (void *) timeout);
}

CSL_Status CSL_rszUnlock(CSL_RszHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_UNLOCK, NULL);
}

CSL_Status CSL_rszSetFlip(CSL_RszHandle hndl, Uint8 rszMod, Bool32 flipH, Bool32 flipV)
{
  CSL_RszSetFlipPrm prm;

  prm.rszMod = rszMod;
  prm.flipH = flipH;
  prm.flipV = flipV;

  return CSL_usrDrvIoctl(hndl->fd, CSL_RSZ_CMD_SET_FLIP, &prm);
}

CSL_Status CSL_rszSetRuntimeConfig(CSL_RszHandle hndl, Uint8 rszMod, CSL_RszRuntimeConfig *rtCfg)
{
  CSL_RszSetRtPrm prm;

  prm.rszMod   = rszMod;
  prm.rtConfig = rtCfg;

  return CSL_usrDrvIoctl(hndl->fd, CSL_RSZ_CMD_SET_RT_PRMS, &prm);
}

