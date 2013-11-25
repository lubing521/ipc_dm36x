/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_ipipeifIoctl.h>


CSL_Status CSL_ipipeifOpen(CSL_IpipeifHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  hndl->regs = CSL_sysGetBaseAddr(CSL_MODULE_IPIPEIF);

  if (hndl->regs == NULL)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_ipipeifClose(CSL_IpipeifHandle hndl)
{
  return CSL_SOK;
}

CSL_Status CSL_ipipeifInit(CSL_IpipeifHandle hndl)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL)
    return CSL_EINVPARAMS;

  status |= CSL_ipipeifOpen(hndl);

  return status;
}

CSL_Status CSL_ipipeifExit(CSL_IpipeifHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  CSL_ipipeifClose(hndl);

  return CSL_SOK;
}
