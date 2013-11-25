/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_ipipeIoctl.h>

CSL_IntRet_t CSL_ipipeIsr(int intId, void *prm, void *reserved);

CSL_Status CSL_ipipeOpen(CSL_IpipeHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  hndl->regs = CSL_sysGetBaseAddr(CSL_MODULE_IPIPE);

  if (hndl->regs == NULL)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_ipipeClose(CSL_IpipeHandle hndl)
{
  return CSL_SOK;
}

CSL_Status CSL_ipipeInit(CSL_IpipeHandle hndl)
{
  int     i;
  CSL_Status status = CSL_SOK;

  if (hndl == NULL)
    return CSL_EINVPARAMS;

  hndl->bscOutBufSwitchEnable = FALSE;
  status |= CSL_bufCreate(&hndl->bscOutBuf);

  hndl->boxcarOutBufSwitchEnable = FALSE;
  status |= CSL_bufCreate(&hndl->boxcarOutBuf);

  for (i = 0; i < CSL_IPIPE_INT_TYPE_MAX; i++) {
    status |= CSL_sysFlagCreate(&hndl->intFlag[i]);
  }

  status |= CSL_ipipeOpen(hndl);

  CSL_sysIntAttachIsr(CSL_SYS_INT_IPIPE_BSC, CSL_ipipeIsr, hndl);
  CSL_sysIntAttachIsr(CSL_SYS_INT_IPIPE_BOXCAR, CSL_ipipeIsr, hndl);  

  return status;
}

CSL_Status CSL_ipipeExit(CSL_IpipeHandle hndl)
{
  int     i;

  if (hndl == NULL)
    return CSL_EINVPARAMS;

  CSL_sysIntAttachIsr(CSL_SYS_INT_IPIPE_BSC, NULL, hndl);
  CSL_sysIntAttachIsr(CSL_SYS_INT_IPIPE_BOXCAR, NULL, hndl);  

  CSL_ipipeClose(hndl);

  hndl->bscOutBufSwitchEnable = FALSE;
  CSL_bufDelete(&hndl->bscOutBuf);

  for (i = 0; i < CSL_IPIPE_INT_TYPE_MAX; i++) {
    CSL_sysFlagDelete(&hndl->intFlag[i]);
  }

  return CSL_SOK;
}
