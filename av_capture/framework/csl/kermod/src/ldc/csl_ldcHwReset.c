/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_ldc.h>


CSL_Status CSL_ldcHwReset(CSL_LdcHandle hndl)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  hndl->regs->PID = CSL_LDC_PID_RESETVAL;
  hndl->regs->PCR = CSL_LDC_PCR_RESETVAL;
  hndl->regs->RD_BASE = CSL_LDC_RD_BASE_RESETVAL;
  hndl->regs->RD_OFST = CSL_LDC_RD_OFST_RESETVAL;
  hndl->regs->FRAME_SIZE = CSL_LDC_FRAME_SIZE_RESETVAL;
  hndl->regs->INITXY = CSL_LDC_INITXY_RESETVAL;
  hndl->regs->WR_BASE = CSL_LDC_WR_BASE_RESETVAL;
  hndl->regs->WR_OFST = CSL_LDC_WR_OFST_RESETVAL;
  hndl->regs->C420_RD_BASE = CSL_LDC_C420_RD_BASE_RESETVAL;
  hndl->regs->C420_WR_BASE = CSL_LDC_C420_WR_BASE_RESETVAL;
  hndl->regs->CONFIG = CSL_LDC_CONFIG_RESETVAL;
  hndl->regs->CENTER = CSL_LDC_CENTER_RESETVAL;
  hndl->regs->KHV = CSL_LDC_KHV_RESETVAL;
  hndl->regs->BLOCK = CSL_LDC_BLOCK_RESETVAL;
  hndl->regs->LUT_ADDR = CSL_LDC_LUT_ADDR_RESETVAL;
  hndl->regs->LUT_WDATA = CSL_LDC_LUT_WDATA_RESETVAL;
  hndl->regs->LUT_RDATA = CSL_LDC_LUT_RDATA_RESETVAL;
  hndl->regs->AFFINE_AB = CSL_LDC_AFFINE_AB_RESETVAL;
  hndl->regs->AFFINE_CD = CSL_LDC_AFFINE_CD_RESETVAL;
  hndl->regs->AFFINE_EF = CSL_LDC_AFFINE_EF_RESETVAL;

  return CSL_SOK;
}
