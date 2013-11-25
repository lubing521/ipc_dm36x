/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_rszIoctl.h>

CSL_IntRet_t CSL_rszIsr(int intId, void *prm, void *reserved);

CSL_Status CSL_rszOpen(CSL_RszHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  hndl->regs = CSL_sysGetBaseAddr(CSL_MODULE_RSZ);

  if (hndl->regs == NULL)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_rszClose(CSL_RszHandle hndl)
{
  return CSL_SOK;
}

CSL_Status CSL_rszInit(CSL_RszHandle hndl)
{
  int     i;
  CSL_Status status = CSL_SOK;

  if (hndl == NULL)
    return CSL_EINVPARAMS;

  for (i = 0; i < CSL_RSZ_CH_MAX; i++) {
    hndl->outBufSwitchEnable[i] = FALSE;
    status |= CSL_bufCreate(&hndl->outBuf[i]);
    hndl->yuv420BufCoffset[i]=0;
    hndl->flipH[i]=0;
    hndl->flipV[i]=0;    
    hndl->flipVOffsetY[i]=0;
    hndl->flipVOffsetC[i]=0;
    hndl->flipHOffsetYC[i]=0; 
	hndl->rszRuntimeConfigFlag[i] = FALSE;
  }

  status |= CSL_sysFlagCreate(&hndl->intFlag);

  status |= CSL_rszOpen(hndl);

  CSL_sysIntAttachIsr(CSL_SYS_INT_RSZ, CSL_rszIsr, hndl);

  CSL_sysIntEnable(CSL_SYS_INT_RSZ, FALSE);

  return status;
}

CSL_Status CSL_rszExit(CSL_RszHandle hndl)
{
  int     i;

  if (hndl == NULL)
    return CSL_EINVPARAMS;

  CSL_sysIntEnable(CSL_SYS_INT_RSZ, FALSE);

  CSL_sysIntAttachIsr(CSL_SYS_INT_RSZ, NULL, hndl);

  CSL_rszClose(hndl);

  for (i = 0; i < CSL_RSZ_CH_MAX; i++) {
    hndl->outBufSwitchEnable[i] = FALSE;
    CSL_bufDelete(&hndl->outBuf[i]);
  }

  CSL_sysFlagDelete(&hndl->intFlag);

  return CSL_SOK;
}
