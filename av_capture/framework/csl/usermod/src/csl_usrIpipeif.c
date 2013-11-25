#include <csl_usrDrv.h>
#include <csl_ipipeifIoctl.h>
#include <csl_hndl.h>

CSL_Status CSL_ipipeifOpen(CSL_IpipeifHandle hndl)
{
  hndl->fd = CSL_usrDrvOpen(CSL_MODULE_IPIPEIF);

  if (hndl->fd < 0)
    return CSL_EFAIL;
    
  hndl->regs = gCSL_vpssHndl.ipipeifRegs;    

  return CSL_SOK;
}

CSL_Status CSL_ipipeifClose(CSL_IpipeifHandle hndl)
{
  return CSL_usrDrvClose(hndl->fd);
}

CSL_Status CSL_ipipeifLock(CSL_IpipeifHandle hndl, Uint32 timeout)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_LOCK, (void *) timeout);
}

CSL_Status CSL_ipipeifUnlock(CSL_IpipeifHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_UNLOCK, NULL);
}
