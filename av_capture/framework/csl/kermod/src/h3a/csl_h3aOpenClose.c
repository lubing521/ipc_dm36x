/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_h3aIoctl.h>

CSL_Status CSL_h3aOpen(CSL_H3aHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  hndl->regs = CSL_sysGetBaseAddr(CSL_MODULE_H3A);

  if (hndl->regs == NULL)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_h3aClose(CSL_H3aHandle hndl)
{
  return CSL_SOK;
}

CSL_Status CSL_h3aInit(CSL_H3aHandle hndl)
{
  int i;
  CSL_Status status = CSL_SOK;

  if (hndl == NULL)
    return CSL_EINVPARAMS;

  for (i = 0; i < CSL_H3A_MOD_MAX; i++) {
    hndl->outBufSwitchEnable[i] = FALSE;
    status |= CSL_bufCreate(&hndl->outBuf[i]);
  }

  status |= CSL_h3aOpen(hndl);

  return status;
}

CSL_Status CSL_h3aExit(CSL_H3aHandle hndl)
{
  int i;
  
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  CSL_h3aClose(hndl);

  for (i = 0; i < CSL_H3A_MOD_MAX; i++) {
    hndl->outBufSwitchEnable[i] = FALSE;
    CSL_bufDelete(&hndl->outBuf[i]);
  }

  return CSL_SOK;
}
