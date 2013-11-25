/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_vpss.h>


CSL_Status CSL_vpssHwSetup(CSL_VpssHandle hndl, CSL_VpssHwSetup * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  if (data->wblSelConfig != NULL) {
    if (CSL_vpssSetWblSelConfig(hndl, data->wblSelConfig) != CSL_SOK)
      status = CSL_EFAIL;
  }

  if (data->priConfig != NULL) {
    if (CSL_vpssSetPriConfig(hndl, data->priConfig) != CSL_SOK)
      status = CSL_EFAIL;
  }

  if (data->intEvtConfig != NULL) {
    if (CSL_vpssSetIntEvtConfig(hndl, data->intEvtConfig) != CSL_SOK)
      status = CSL_EFAIL;
  }

  if (data->clkConfig != NULL) {
    if (CSL_vpssSetClkConfig(hndl, data->clkConfig) != CSL_SOK)
      status = CSL_EFAIL;
  }
  return status;
}
