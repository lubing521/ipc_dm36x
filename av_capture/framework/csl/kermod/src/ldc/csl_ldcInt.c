
#include <csl_ldc.h>
#include <csl_sysDrv.h>

CSL_IntRet_t CSL_ldcIsr(int intId, void *prm, void *reserved)
{
  CSL_LdcHandle hndl = &gCSL_drvLdcObj;

  CSL_sysFlagSet(&hndl->intFlag);

  return CSL_INT_RET_SOK;
}

CSL_Status CSL_ldcIntEnable(CSL_LdcHandle hndl, Bool32 enable)
{
  CSL_Status status;

  if (hndl == NULL)
    return CSL_EFAIL;

  status = CSL_sysIntEnable(CSL_SYS_INT_LDC, enable);

  return status;
}

CSL_Status CSL_ldcIntClear(CSL_LdcHandle hndl)
{
  CSL_Status status;

  if (hndl == NULL)
    return CSL_EFAIL;

  CSL_sysIntClear(CSL_SYS_INT_LDC);    

  status = CSL_sysFlagClear(&hndl->intFlag);

  return status;
}

CSL_Status CSL_ldcIntWait(CSL_LdcHandle hndl)
{
  CSL_Status status;

  if (hndl == NULL)
    return CSL_EFAIL;

  status = CSL_sysFlagWait(&hndl->intFlag, CSL_SYS_TIMEOUT_FOREVER);

  return status;
}
