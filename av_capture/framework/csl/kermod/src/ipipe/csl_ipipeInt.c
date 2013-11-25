
#include <csl_ipipe.h>
#include <csl_dma.h>
#include <csl_sysDrv.h>

CSL_Status CSL_ipipeBoxcarBufSwitch(CSL_IpipeHandle hndl);

CSL_IntRet_t CSL_ipipeBscIsr(int intId, void *prm, void *reserved)
{
  CSL_dmaStart(CSL_DMA_IPIPE_BSC);

  return CSL_INT_RET_SOK;
}

CSL_IntRet_t CSL_ipipeBoxcarIsr(int intId, void *prm, void *reserved)
{
  CSL_ipipeBoxcarBufSwitch(&gCSL_drvIpipeObj);

  return CSL_INT_RET_SOK;
}

CSL_IntRet_t CSL_ipipeIsr(int intId, void *prm, void *reserved)
{
  CSL_IpipeHandle hndl = &gCSL_drvIpipeObj;
  int     intType;

  if (intId == CSL_SYS_INT_IPIPE_BOXCAR)
    intType = CSL_IPIPE_INT_TYPE_BOXCAR;
  else if (intId == CSL_SYS_INT_IPIPE_BSC)
    intType = CSL_IPIPE_INT_TYPE_BSC;
  else
    return CSL_INT_RET_SOK;

  if (intType == CSL_IPIPE_INT_TYPE_BOXCAR) {
    CSL_ipipeBoxcarIsr(intId, prm, reserved);
  } else if (intType == CSL_IPIPE_INT_TYPE_BSC) {
    CSL_ipipeBscIsr(intId, prm, reserved);
  }

  CSL_sysFlagSet(&hndl->intFlag[intType]);

  return CSL_INT_RET_SOK;
}

CSL_Status CSL_ipipeIntEnable(CSL_IpipeHandle hndl, Uint8 intType, Bool32 enable)
{
  CSL_Status status;
  int     intId;

  if (hndl == NULL)
    return CSL_EFAIL;

  if (intType == CSL_IPIPE_INT_TYPE_BOXCAR)
    intId = CSL_SYS_INT_IPIPE_BOXCAR;
  else if (intType == CSL_IPIPE_INT_TYPE_BSC) {
    intId = CSL_SYS_INT_IPIPE_BSC;
    hndl->bscCount = 0;
  }
  else
    return CSL_EFAIL;

  status = CSL_sysIntEnable(intId, enable);

  return status;
}

CSL_Status CSL_ipipeIntClear(CSL_IpipeHandle hndl, Uint8 intType)
{
  CSL_Status status;
  int intId;

  if (hndl == NULL)
    return CSL_EFAIL;

  if (intType == CSL_IPIPE_INT_TYPE_BOXCAR)
    intId = CSL_SYS_INT_IPIPE_BOXCAR;
  else if (intType == CSL_IPIPE_INT_TYPE_BSC)
    intId = CSL_SYS_INT_IPIPE_BSC;
  else
    return CSL_EFAIL;
    
  CSL_sysIntClear(intId);    

  status = CSL_sysFlagClear(&hndl->intFlag[intType]);

  return status;
}

CSL_Status CSL_ipipeIntWait(CSL_IpipeHandle hndl, Uint8 intType)
{
  CSL_Status status;

  if (hndl == NULL)
    return CSL_EFAIL;

  if (intType >= CSL_IPIPE_INT_TYPE_MAX)
    return CSL_EFAIL;

  status = CSL_sysFlagWait(&hndl->intFlag[intType], CSL_SYS_TIMEOUT_FOREVER);

  return status;
}
