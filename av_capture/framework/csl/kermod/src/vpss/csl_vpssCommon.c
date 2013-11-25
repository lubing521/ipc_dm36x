/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/


#include <csl_vpss.h>
#include <csl_sysDrv.h>

CSL_Status CSL_vpssSetClkConfig(CSL_VpssHandle hndl, CSL_VpssClkConfig * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  if(data->pclkSelect!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->PCCR, ISP5_PCCR_PSYNC_CLK_SEL, data->pclkSelect);

  if(data->pclkSyncClkEnable!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->PCCR, ISP5_PCCR_SYNC_ENABLE, data->pclkSyncClkEnable);

  if(data->blClkEnable!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->PCCR, ISP5_PCCR_BL_CLK_ENABLE, data->blClkEnable);

  if(data->isifClkEnable!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->PCCR, ISP5_PCCR_ISIF_CLK_ENABLE, data->isifClkEnable);

  if(data->h3aClkEnable!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->PCCR, ISP5_PCCR_H3A_CLK_ENABLE, data->h3aClkEnable);

  if(data->ipipeifClkEnable!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->PCCR, ISP5_PCCR_IPIPEIF_CLK_ENABLE, data->ipipeifClkEnable);

  if(data->ipipeClkEnable!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->PCCR, ISP5_PCCR_IPIPE_CLK_ENABLE, data->ipipeClkEnable);

  if(data->rszClkEnable!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->PCCR, ISP5_PCCR_RSZ_CLK_ENABLE, data->rszClkEnable);

  if(data->ldcClkEnable!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->regs->PCCR, VPSS_PCCR_LDC_CLK_ENABLE, data->ldcClkEnable);

  if(data->fdifClkEnable!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->regs->PCCR, VPSS_PCCR_FDIF_CLK_ENABLE, data->fdifClkEnable);

  if(data->ldcClkSelect!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->regs->PCCR, VPSS_PCCR_LDC_CLK_SEL, data->ldcClkSelect);

  return status;
}

CSL_Status CSL_vpssSetIntEvtConfig(CSL_VpssHandle hndl, CSL_VpssIntEvtConfig * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  if(data->intSel[0]!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->INTSEL1, ISP5_INTSEL1_INTSEL0, data->intSel[0]);

  if(data->intSel[1]!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->INTSEL1, ISP5_INTSEL1_INTSEL1, data->intSel[1]);

  if(data->intSel[2]!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->INTSEL1, ISP5_INTSEL1_INTSEL2, data->intSel[2]);

  if(data->intSel[3]!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->INTSEL1, ISP5_INTSEL1_INTSEL3, data->intSel[3]);

  if(data->intSel[4]!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->INTSEL2, ISP5_INTSEL2_INTSEL4, data->intSel[4]);

  if(data->intSel[5]!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->INTSEL2, ISP5_INTSEL2_INTSEL5, data->intSel[5]);

  if(data->intSel[6]!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->INTSEL2, ISP5_INTSEL2_INTSEL6, data->intSel[6]);

  if(data->intSel[7]!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->INTSEL2, ISP5_INTSEL2_INTSEL7, data->intSel[7]);

  if(data->intSel[8]!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->INTSEL3, ISP5_INTSEL3_INTSEL8, data->intSel[8]);

  if(data->evtSel[0]!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->EVTSEL, ISP5_EVTSEL_EVTSEL0, data->evtSel[0]);

  if(data->evtSel[1]!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->EVTSEL, ISP5_EVTSEL_EVTSEL1, data->evtSel[1]);

  if(data->evtSel[2]!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->EVTSEL, ISP5_EVTSEL_EVTSEL2, data->evtSel[2]);

  if(data->evtSel[3]!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->EVTSEL, ISP5_EVTSEL_EVTSEL3, data->evtSel[3]);

  return status;
}

CSL_Status CSL_vpssSetPriConfig(CSL_VpssHandle hndl, CSL_VpssPriConfig * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;


  if(data->priVpssWr!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->BCR, ISP5_BCR_CPRIORITY_W, data->priVpssWr);

  if(data->priVpssRd!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->BCR, ISP5_BCR_CPRIORITY_R, data->priVpssRd);

  return status;
}

CSL_Status CSL_vpssSetWblSelConfig(CSL_VpssHandle hndl, CSL_VpssWblSelConfig * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  if(data->wblSelIsifIpipe!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->BCR, ISP5_BCR_SRC_SEL_ISIF_IPIPE, data->wblSelIsifIpipe);

  if(data->wblSelIpipeLdc!=CSL_VPSS_NO_UPDATE)
    CSL_FINS(hndl->ispRegs->BCR, ISP5_BCR_SRC_SEL_IPIPE_LDC, data->wblSelIpipeLdc);

  return status;
}

CSL_Status CSL_vpssSetReg(CSL_VpssHandle hndl, Uint32 regStartAddr, Uint32 *regValues, Uint32 numRegs)
{
  volatile Uint32 *regVirtAddr = (Uint32*)CSL_PHYS_TO_VIRT(regStartAddr);
  
  if(regVirtAddr==NULL)
    return CSL_EFAIL;
    
  while(numRegs--) {
    *regVirtAddr++ = *regValues++;
  }
  
  return CSL_SOK;  
}

CSL_Status CSL_vpssGetReg(CSL_VpssHandle hndl, Uint32 regStartAddr, Uint32 *regValues, Uint32 numRegs)
{
  volatile Uint32 *regVirtAddr = (Uint32*)CSL_PHYS_TO_VIRT(regStartAddr);
  
  if(regVirtAddr==NULL)
    return CSL_EFAIL;
    
  while(numRegs--) {
    *regValues++ = *regVirtAddr++;
  }
  
  return CSL_SOK;
}

CSL_Status CSL_vpssGetClkDiv(CSL_VpssHandle hndl, CSL_VpssClkDiv *prm)
{
  volatile Uint32 pllBaseAddr = CSL_PHYS_TO_VIRT(CSL_PLL1_BASE_PHYS_ADDR);
  
  prm->PLL1_M         = *(volatile Uint32*)(pllBaseAddr+0x110) & 0xFF;
  prm->PLL1_PREDIV    = *(volatile Uint32*)(pllBaseAddr+0x114) & 0xFF;  
  prm->PLL1_POSTDIV   = *(volatile Uint32*)(pllBaseAddr+0x128) & 0xFF;
  
  prm->PLL1_PLLDIV[0] = 0;
  prm->PLL1_PLLDIV[1] = *(volatile Uint32*)(pllBaseAddr+0x118) & 0xFF;  
  prm->PLL1_PLLDIV[2] = *(volatile Uint32*)(pllBaseAddr+0x11c) & 0xFF;  
  prm->PLL1_PLLDIV[3] = *(volatile Uint32*)(pllBaseAddr+0x120) & 0xFF;  
  prm->PLL1_PLLDIV[4] = *(volatile Uint32*)(pllBaseAddr+0x160) & 0xFF;  
  prm->PLL1_PLLDIV[5] = *(volatile Uint32*)(pllBaseAddr+0x164) & 0xFF;    
  prm->PLL1_PLLDIV[6] = *(volatile Uint32*)(pllBaseAddr+0x168) & 0xFF;    
  prm->PLL1_PLLDIV[7] = *(volatile Uint32*)(pllBaseAddr+0x16c) & 0xFF;    
  prm->PLL1_PLLDIV[8] = *(volatile Uint32*)(pllBaseAddr+0x170) & 0xFF;    
  prm->PLL1_PLLDIV[9] = *(volatile Uint32*)(pllBaseAddr+0x174) & 0xFF;    

  pllBaseAddr = CSL_PHYS_TO_VIRT(CSL_PLL2_BASE_PHYS_ADDR);
  
  prm->PLL2_M         = *(volatile Uint32*)(pllBaseAddr+0x110) & 0xFF;
  prm->PLL2_PREDIV    = *(volatile Uint32*)(pllBaseAddr+0x114) & 0xFF;  
  prm->PLL2_POSTDIV   = *(volatile Uint32*)(pllBaseAddr+0x128) & 0xFF;
  
  prm->PLL2_PLLDIV[0] = 0;
  prm->PLL2_PLLDIV[1] = *(volatile Uint32*)(pllBaseAddr+0x118) & 0xFF;  
  prm->PLL2_PLLDIV[2] = *(volatile Uint32*)(pllBaseAddr+0x11c) & 0xFF;  
  prm->PLL2_PLLDIV[3] = *(volatile Uint32*)(pllBaseAddr+0x120) & 0xFF;  
  prm->PLL2_PLLDIV[4] = *(volatile Uint32*)(pllBaseAddr+0x160) & 0xFF;  
  prm->PLL2_PLLDIV[5] = *(volatile Uint32*)(pllBaseAddr+0x164) & 0xFF;    
  prm->PLL2_PLLDIV[6] = *(volatile Uint32*)(pllBaseAddr+0x168) & 0xFF;    
  prm->PLL2_PLLDIV[7] = *(volatile Uint32*)(pllBaseAddr+0x16c) & 0xFF;    
  prm->PLL2_PLLDIV[8] = *(volatile Uint32*)(pllBaseAddr+0x170) & 0xFF;    
  prm->PLL2_PLLDIV[9] = *(volatile Uint32*)(pllBaseAddr+0x174) & 0xFF;    
  
  prm->PERICLKCTL     = gCSL_drvGpioObj.sysRegs->PERI_CLK_CTRL;
  prm->VPSSCLKCTL     = gCSL_drvGpioObj.sysRegs->VPSS_CLK_CTRL;
  
  return CSL_SOK;
}

CSL_Status CSL_vpssSetClkDiv(CSL_VpssHandle hndl, CSL_VpssClkDiv *prm)
{


  return CSL_SOK;
}
