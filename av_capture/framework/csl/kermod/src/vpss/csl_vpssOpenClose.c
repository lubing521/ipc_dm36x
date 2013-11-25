/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_vpssIoctl.h>

CSL_Status CSL_vpssOpen(CSL_VpssHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  hndl->regs = CSL_sysGetBaseAddr(CSL_MODULE_VPSS);
  hndl->ispRegs = CSL_sysGetBaseAddr(CSL_MODULE_ISP5);

  if (hndl->regs == NULL || hndl->ispRegs==NULL)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_vpssClose(CSL_VpssHandle hndl)
{
  return CSL_SOK;
}

CSL_Status CSL_vpssInit(CSL_VpssHandle hndl)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL)
    return CSL_EINVPARAMS;

  status |= CSL_vpssOpen(hndl);

  return status;
}

CSL_Status CSL_vpssExit(CSL_VpssHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  CSL_vpssClose(hndl);

  return CSL_SOK;
}
