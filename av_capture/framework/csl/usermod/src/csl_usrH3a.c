#include <csl_usrDrv.h>
#include <csl_h3aIoctl.h>

CSL_Status CSL_h3aOpen(CSL_H3aHandle hndl)
{
  hndl->fd = CSL_usrDrvOpen(CSL_MODULE_H3A);

  if (hndl->fd < 0)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_h3aClose(CSL_H3aHandle hndl)
{
  return CSL_usrDrvClose(hndl->fd);
}

CSL_Status CSL_h3aHwSetup(CSL_H3aHandle hndl, CSL_H3aHwSetup * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_HW_SETUP, data);
}

CSL_Status CSL_h3aHwReset(CSL_H3aHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_HW_RESET, NULL);
}

CSL_Status CSL_h3aAfFrameConfig(CSL_H3aHandle hndl, CSL_H3aInFrameConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_INFRAME_CONFIG, data);
}

CSL_Status CSL_h3aAfEnable(CSL_H3aHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_AF_ENABLE, (void *) enable);
}

CSL_Status CSL_h3aAfVfEnable(CSL_H3aHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_AF_VF_ENABLE, (void *) enable);
}

CSL_Status CSL_h3aAfAlawEnable(CSL_H3aHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_AF_ALAW_ENABLE, (void *) enable);
}

CSL_Status CSL_h3aAfSetOutAddr(CSL_H3aHandle hndl, Uint8 * addr)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_AF_SET_OUTADDR, (Uint8 *) addr);
}

CSL_Status CSL_h3aAfSetConfig(CSL_H3aHandle hndl, CSL_H3aAfConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_AF_SET_CONFIG, data);
}

CSL_Status CSL_h3aAewbEnable(CSL_H3aHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_AEWB_ENABLE, (void *) enable);
}

CSL_Status CSL_h3aAewbAlawEnable(CSL_H3aHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_AEWB_ALAW_ENABLE, (void *) enable);
}

CSL_Status CSL_h3aAewbSetOutAddr(CSL_H3aHandle hndl, Uint8 * addr)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_AEWB_SET_OUTADDR, (Uint8 *) addr);
}

CSL_Status CSL_h3aAewbSetConfig(CSL_H3aHandle hndl, CSL_H3aAewbConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_AEWB_SET_CONFIG, data);
}

CSL_Status CSL_h3aBufInit(CSL_H3aHandle hndl, Uint8 h3aModId, CSL_BufInit * bufInit)
{
  CSL_H3aBufInitPrm bufInitPrm;

  bufInitPrm.h3aModId = h3aModId;
  bufInitPrm.bufInit = bufInit;

  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_BUF_INIT, &bufInitPrm);
}

CSL_Status CSL_h3aBufSwitchEnable(CSL_H3aHandle hndl, Uint8 h3aModId, Bool32 enable)
{
  CSL_H3aBufSwitchEnablePrm bufSwitchEnablePrm;

  bufSwitchEnablePrm.h3aModId = h3aModId;
  bufSwitchEnablePrm.enable = enable;

  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_BUF_SWITCH_ENABLE, &bufSwitchEnablePrm);
}

CSL_Status CSL_h3aBufGetFull(CSL_H3aHandle hndl, Uint8 h3aModId, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout)
{
  CSL_H3aBufGetFullPrm bufGetFullPrm;

  bufGetFullPrm.h3aModId = h3aModId;
  bufGetFullPrm.buf = buf;
  bufGetFullPrm.minBuf = minBuf;
  bufGetFullPrm.timeout = timeout;

  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_BUF_GET_FULL, &bufGetFullPrm);
}

CSL_Status CSL_h3aBufPutEmpty(CSL_H3aHandle hndl, Uint8 h3aModId, CSL_BufInfo * buf)
{
  CSL_H3aBufPutEmptyPrm bufPutEmptyPrm;

  bufPutEmptyPrm.h3aModId = h3aModId;
  bufPutEmptyPrm.buf = buf;

  return CSL_usrDrvIoctl(hndl->fd, CSL_H3A_CMD_BUF_PUT_EMPTY, &bufPutEmptyPrm);
}

CSL_Status CSL_h3aLock(CSL_H3aHandle hndl, Uint32 timeout)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_LOCK, (void *) timeout);
}

CSL_Status CSL_h3aUnlock(CSL_H3aHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_UNLOCK, NULL);
}
