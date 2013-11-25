
#include <csl_rsz.h>
#include <csl_sysDrv.h>

CSL_IntRet_t CSL_rszIsr(int intId, void *prm, void *reserved)
{
  CSL_RszHandle hndl = &gCSL_drvRszObj;
  
  CSL_sysFlagSet(&hndl->intFlag);

  return CSL_INT_RET_SOK;
}

CSL_Status CSL_rszIntEnable(CSL_RszHandle hndl, Bool32 enable)
{
  CSL_Status status;

  if (hndl == NULL)
    return CSL_EFAIL;

  status = CSL_sysIntEnable(CSL_SYS_INT_RSZ, enable);

  return status;
}

CSL_Status CSL_rszIntClear(CSL_RszHandle hndl)
{
  CSL_Status status;

  if (hndl == NULL)
    return CSL_EFAIL;
    
  CSL_sysIntClear(CSL_SYS_INT_RSZ);    

  status = CSL_sysFlagClear(&hndl->intFlag);

  return status;
}

CSL_Status CSL_rszIntWait(CSL_RszHandle hndl)
{
  CSL_Status status;

  if (hndl == NULL)
    return CSL_EFAIL;

  status = CSL_sysFlagWait(&hndl->intFlag, CSL_SYS_TIMEOUT_FOREVER);

  return status;
}
