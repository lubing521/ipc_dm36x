/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_rsz.h>


CSL_Status CSL_rszHwSetup(CSL_RszHandle hndl, CSL_RszHwSetup * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  hndl->regs->SRC_EN = CSL_RSZ_SRC_EN_RESETVAL;
  hndl->regs->SRC_MODE = CSL_RSZ_SRC_MODE_RESETVAL;

  if (data->inConfig != NULL) {
    if (CSL_rszSetInConfig(hndl, data->inConfig) != CSL_SOK)
      status = CSL_EFAIL;
  }

  if (data->inFrameConfig != NULL) {
    if (CSL_rszSetInFrameConfig(hndl, data->inFrameConfig) != CSL_SOK)
      status = CSL_EFAIL;
  }

  if (data->clkConfig != NULL) {
    if (CSL_rszSetClkConfig(hndl, data->clkConfig) != CSL_SOK)
      status = CSL_EFAIL;
  }

  if (data->minMaxConfig != NULL) {
    if (CSL_rszSetMinMaxConfig(hndl, data->minMaxConfig) != CSL_SOK)
      status = CSL_EFAIL;
  }

  if (data->rszAConfig != NULL) {
    if (CSL_rszSetOutConfig(hndl, CSL_RSZ_A, data->rszAConfig) != CSL_SOK)
      status = CSL_EFAIL;
  } else {
    hndl->regs->RZA_EN = 0;  
  }

  if (data->rszBConfig != NULL) {
    if (CSL_rszSetOutConfig(hndl, CSL_RSZ_B, data->rszBConfig) != CSL_SOK)
      status = CSL_EFAIL;
  } else {
    hndl->regs->RZB_EN = 0;
  }

  CSL_FINS(hndl->regs->SRC_MODE, RSZ_SRC_MODE_OST, data->oneShotEnable) ;
    
  hndl->regs->SRC_EN = data->enable;
  
  return status;
}
