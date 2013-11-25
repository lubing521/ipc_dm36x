/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_vpss.h>


CSL_Status CSL_vpssHwReset(CSL_VpssHandle hndl)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  hndl->ispRegs->PCCR = CSL_ISP5_PCCR_RESETVAL;
  hndl->ispRegs->BCR = CSL_ISP5_BCR_RESETVAL;
  hndl->ispRegs->INTSTAT = CSL_ISP5_INTSTAT_RESETVAL;
  hndl->ispRegs->INTSEL1 = CSL_ISP5_INTSEL1_RESETVAL;
  hndl->ispRegs->INTSEL2 = CSL_ISP5_INTSEL2_RESETVAL;
  hndl->ispRegs->INTSEL3 = CSL_ISP5_INTSEL3_RESETVAL;
  hndl->ispRegs->EVTSEL = CSL_ISP5_EVTSEL_RESETVAL;

  return CSL_SOK;
}
