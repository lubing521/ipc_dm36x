
#include <csl_usrDrv.h>
#include <csl_ldcIoctl.h>

CSL_Status CSL_ldcOpen(CSL_LdcHandle hndl)
{
  hndl->fd = CSL_usrDrvOpen(CSL_MODULE_LDC);

  if (hndl->fd < 0)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_ldcClose(CSL_LdcHandle hndl)
{
  return CSL_usrDrvClose(hndl->fd);
}

CSL_Status CSL_ldcHwSetup(CSL_LdcHandle hndl, CSL_LdcHwSetup * setup)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_LDC_CMD_HW_SETUP, setup);
}

CSL_Status CSL_ldcHwReset(CSL_LdcHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_LDC_CMD_HW_RESET, NULL);
}

CSL_Status CSL_ldcFrameConfig(CSL_LdcHandle hndl, CSL_LdcFrameConfig * frameConfig)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_LDC_CMD_FRAME_CONFIG, frameConfig);
}

CSL_Status CSL_ldcEnable(CSL_LdcHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_LDC_CMD_ENABLE, (void *) enable);
}

CSL_Status CSL_ldcIsBusy(CSL_LdcHandle hndl, Bool32 * isBusy)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_LDC_CMD_IS_BUSY, isBusy);
}

CSL_Status CSL_ldcWriteTable(CSL_LdcHandle hndl, Uint32 * table)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_LDC_CMD_WRITE_TABLE, table);
}

CSL_Status CSL_ldcReadTable(CSL_LdcHandle hndl, Uint32 * table)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_LDC_CMD_READ_TABLE, table);
}

CSL_Status CSL_ldcIntEnable(CSL_LdcHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_LDC_CMD_INT_ENABLE, (void *) enable);
}

CSL_Status CSL_ldcIntClear(CSL_LdcHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_LDC_CMD_INT_CLEAR, NULL);
}

CSL_Status CSL_ldcIntWait(CSL_LdcHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_LDC_CMD_INT_WAIT, NULL);
}

CSL_Status CSL_ldcLock(CSL_LdcHandle hndl, Uint32 timeout)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_LOCK, (void *) timeout);
}

CSL_Status CSL_ldcUnlock(CSL_LdcHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_UNLOCK, NULL);
}
