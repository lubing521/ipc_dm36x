/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_ldcIoctl.h>

CSL_IntRet_t CSL_ldcIsr(int intId, void *prm, void *reserved);

CSL_Status CSL_ldcOpen(CSL_LdcHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  hndl->regs = CSL_sysGetBaseAddr(CSL_MODULE_LDC);

  if (hndl->regs == NULL)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_ldcClose(CSL_LdcHandle hndl)
{
  return CSL_SOK;
}

CSL_Status CSL_ldcInit(CSL_LdcHandle hndl)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL)
    return CSL_EINVPARAMS;

  status |= CSL_sysFlagCreate(&hndl->intFlag);
  status |= CSL_ldcOpen(hndl);

  CSL_sysIntAttachIsr(CSL_SYS_INT_LDC, CSL_ldcIsr, hndl);

  CSL_sysIntEnable(CSL_SYS_INT_LDC, FALSE);

  return status;
}

CSL_Status CSL_ldcExit(CSL_LdcHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  CSL_sysIntEnable(CSL_SYS_INT_LDC, FALSE);

  CSL_sysIntAttachIsr(CSL_SYS_INT_LDC, NULL, hndl);

  CSL_ldcClose(hndl);
  CSL_sysFlagDelete(&hndl->intFlag);

  return CSL_SOK;
}
