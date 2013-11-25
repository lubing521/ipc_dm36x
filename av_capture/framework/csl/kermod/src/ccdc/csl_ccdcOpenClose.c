/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/
#include <csl_ccdcIoctl.h>

CSL_IntRet_t CSL_ccdcIsr(int intId, void *prm, void *reserved);

CSL_Status CSL_ccdcOpen(CSL_CcdcHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  hndl->regs = CSL_sysGetBaseAddr(CSL_MODULE_ISIF);

  if (hndl->regs == NULL)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_ccdcClose(CSL_CcdcHandle hndl)
{
  return CSL_SOK;
}

CSL_Status CSL_ccdcInit(CSL_CcdcHandle hndl)
{
  int     i;
  CSL_Status status = CSL_SOK;

  if (hndl == NULL)
    return CSL_EINVPARAMS;

  hndl->count = 0;
  hndl->dataOutBufSwitchEnable = FALSE;
  status |= CSL_bufCreate(&hndl->dataOutBuf);

  hndl->lscBufSwitchEnable = FALSE;
  hndl->lscGainTableBufSize = 0;
  status |= CSL_bufCreate(&hndl->lscBuf);

  for (i = 0; i < CSL_CCDC_INT_TYPE_MAX; i++) {
    status |= CSL_sysFlagCreate(&hndl->intFlag[i]);
  }

  status |= CSL_ccdcOpen(hndl);
  
  CSL_sysIntAttachIsr(CSL_SYS_INT_CCDC_VD0, CSL_ccdcIsr, hndl);
  CSL_sysIntAttachIsr(CSL_SYS_INT_CCDC_VD1, CSL_ccdcIsr, hndl);      

  CSL_ccdcIntEnable(hndl, CSL_SYS_INT_CCDC_VD0, FALSE);
  CSL_ccdcIntEnable(hndl, CSL_SYS_INT_CCDC_VD1, FALSE);  

  return status;
}

CSL_Status CSL_ccdcExit(CSL_CcdcHandle hndl)
{
  int     i;

  if (hndl == NULL)
    return CSL_EINVPARAMS;

  CSL_ccdcClose(hndl);

  CSL_ccdcIntEnable(hndl, CSL_SYS_INT_CCDC_VD0, FALSE);
  CSL_ccdcIntEnable(hndl, CSL_SYS_INT_CCDC_VD1, FALSE);  

  CSL_sysIntAttachIsr(CSL_SYS_INT_CCDC_VD0, NULL, hndl);
  CSL_sysIntAttachIsr(CSL_SYS_INT_CCDC_VD1, NULL, hndl);      

  hndl->dataOutBufSwitchEnable = FALSE;
  CSL_bufDelete(&hndl->dataOutBuf);

  for (i = 0; i < CSL_CCDC_INT_TYPE_MAX; i++) {
    CSL_sysFlagDelete(&hndl->intFlag[i]);
  }


  return CSL_SOK;
}
