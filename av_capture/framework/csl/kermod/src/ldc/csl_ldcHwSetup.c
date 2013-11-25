/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_ldc.h>



CSL_Status CSL_ldcHwSetup(CSL_LdcHandle hndl, CSL_LdcHwSetup * setup)
{
  CSL_Status status = CSL_SOK;
  Uint32  val;

  if (hndl == NULL || setup == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->PCR, LDC_PCR_EN, 0);

  CSL_FINS(hndl->regs->PCR, LDC_PCR_LDMAPEN, setup->ldMapEnable);

  CSL_FINS(hndl->regs->PCR, LDC_PCR_MODE, setup->inputType);

  CSL_FINS(hndl->regs->PCR, LDC_PCR_BMODE, setup->bayerType);

  CSL_FINS(hndl->regs->CONFIG, LDC_CONFIG_INITC, setup->bayerStartPhase);

  CSL_FINS(hndl->regs->CONFIG, LDC_CONFIG_YINT_TYPE, setup->interpolationTypeY);

  CSL_FINS(hndl->regs->CONFIG, LDC_CONFIG_T, setup->backMapRightShift);

  CSL_FINS(hndl->regs->CONFIG, LDC_CONFIG_RTH, setup->backMapThreshold);

  if(setup->table) {
    if( CSL_ldcWriteTable(hndl, setup->table) != CSL_SOK )
      status = CSL_EFAIL; 
  }

  CSL_FINS(hndl->regs->CENTER, LDC_CENTER_H0, setup->lensCenterX);

  CSL_FINS(hndl->regs->CENTER, LDC_CENTER_V0, setup->lensCenterY);

  val = CSL_FMK(LDC_KHV_KVU, setup->kvh)
    | CSL_FMK(LDC_KHV_KVL, setup->kvl)
    | CSL_FMK(LDC_KHV_KHR, setup->khr)
    | CSL_FMK(LDC_KHV_KHL, setup->khl);

  hndl->regs->KHV = val;

  CSL_FINS(hndl->regs->AFFINE_AB, LDC_AFFINE_AB_A, setup->affineTranswrap[0]);

  CSL_FINS(hndl->regs->AFFINE_AB, LDC_AFFINE_AB_B, setup->affineTranswrap[1]);

  CSL_FINS(hndl->regs->AFFINE_CD, LDC_AFFINE_CD_C, setup->affineTranswrap[2]);

  CSL_FINS(hndl->regs->AFFINE_CD, LDC_AFFINE_CD_D, setup->affineTranswrap[3]);

  CSL_FINS(hndl->regs->AFFINE_EF, LDC_AFFINE_EF_E, setup->affineTranswrap[4]);

  CSL_FINS(hndl->regs->AFFINE_EF, LDC_AFFINE_EF_F, setup->affineTranswrap[5]);

  if (setup->frameConfig != NULL) {
    if (CSL_ldcFrameConfig(hndl, setup->frameConfig) != CSL_SOK)
      status = CSL_EFAIL;
  }

  CSL_FINS(hndl->regs->PCR, LDC_PCR_EN, setup->enable);
  
  return status;
}
