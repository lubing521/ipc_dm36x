/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/


#include <csl_h3a.h>


CSL_Status CSL_h3aInFrameConfig(CSL_H3aHandle hndl, CSL_H3aInFrameConfig * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->LINE_START, H3A_LINE_START_SLV, data->startV);

  CSL_FINS(hndl->regs->LINE_START, H3A_LINE_START_LINE_START, data->line_start);

  CSL_FINS(hndl->regs->PCR, H3A_PCR_MED_TH, data->medFiltThreshold);

  return status;
}

CSL_Status CSL_h3aAfSetConfig(CSL_H3aHandle hndl, CSL_H3aAfConfig * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  if (   ((data->paxWidth  % 2) || (data->paxWidth < 8) )
      || ((data->paxHeight % 2) || (data->paxHeight < 2) )//|| (data->paxHeight > 51))
      || ((data->paxIncH   % 2) || (data->paxIncH > 30))
      || ((data->paxIncV   % 2) || (data->paxIncV > 30))
      || ((data->paxNumH < 1)   || (data->paxNumH > 36))
      || ((data->paxNumV < 1)   || (data->paxNumV > 128))
    )
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->PCR, H3A_PCR_AF_EN, data->enable);

  CSL_FINS(hndl->regs->PCR, H3A_PCR_AF_ALAW_EN, data->alawEnable);

  CSL_FINS(hndl->regs->PCR, H3A_PCR_AF_VF_EN, data->vfEnable);

  CSL_FINS(hndl->regs->PCR, H3A_PCR_AF_MED_EN, data->medFiltEnable);

  CSL_FINS(hndl->regs->PCR, H3A_PCR_RGBPOS, data->rgbPos);

  CSL_FINS(hndl->regs->PCR, H3A_PCR_FVMODE, data->fvAccMode);

  CSL_FINS(hndl->regs->AFPAX1, H3A_AFPAX1_PAXW, (data->paxWidth/2) - 1);

  CSL_FINS(hndl->regs->AFPAX1, H3A_AFPAX1_PAXH, (data->paxHeight/2) - 1 );

  CSL_FINS(hndl->regs->AFPAX2, H3A_AFPAX2_AFINCH, (data->paxIncH/2) - 1);

  CSL_FINS(hndl->regs->AFPAX2, H3A_AFPAX2_AFINCV, (data->paxIncV/2) - 1);

  CSL_FINS(hndl->regs->AFPAX2, H3A_AFPAX2_PAXHC, data->paxNumH-1);

  CSL_FINS(hndl->regs->AFPAX2, H3A_AFPAX2_PAXVC, data->paxNumV-1);

  CSL_FINS(hndl->regs->AFPAXSTART, H3A_AFPAXSTART_PAXSH, data->paxStartH);

  CSL_FINS(hndl->regs->AFPAXSTART, H3A_AFPAXSTART_PAXSV, data->paxStartV);

  CSL_FINS(hndl->regs->AFIIRSH, H3A_AFIIRSH_IIRSH, data->iirStartH);

  if (((Uint32) data->outputAddr) % 64)
    status = CSL_EFAIL;
  hndl->regs->AFBUFST = (Uint32) data->outputAddr;

  CSL_FINS(hndl->regs->AFCOEF010, H3A_AFCOEF010_COEFF0, data->iirCoeff0[0]);
  CSL_FINS(hndl->regs->AFCOEF010, H3A_AFCOEF010_COEFF1, data->iirCoeff0[1]);
  CSL_FINS(hndl->regs->AFCOEF032, H3A_AFCOEF032_COEFF2, data->iirCoeff0[2]);
  CSL_FINS(hndl->regs->AFCOEF032, H3A_AFCOEF032_COEFF3, data->iirCoeff0[3]);
  CSL_FINS(hndl->regs->AFCOEFF054, H3A_AFCOEFF054_COEFF4, data->iirCoeff0[4]);
  CSL_FINS(hndl->regs->AFCOEFF054, H3A_AFCOEFF054_COEFF5, data->iirCoeff0[5]);
  CSL_FINS(hndl->regs->AFCOEFF076, H3A_AFCOEFF076_COEFF6, data->iirCoeff0[6]);
  CSL_FINS(hndl->regs->AFCOEFF076, H3A_AFCOEFF076_COEFF7, data->iirCoeff0[7]);
  CSL_FINS(hndl->regs->AFCOEFF098, H3A_AFCOEFF098_COEFF8, data->iirCoeff0[8]);
  CSL_FINS(hndl->regs->AFCOEFF098, H3A_AFCOEFF098_COEFF9, data->iirCoeff0[9]);
  CSL_FINS(hndl->regs->AFCOEFF0010, H3A_AFCOEFF0010_COEFF10, data->iirCoeff0[10]);

  CSL_FINS(hndl->regs->AFCOEF110, H3A_AFCOEF110_COEFF0, data->iirCoeff1[0]);
  CSL_FINS(hndl->regs->AFCOEF110, H3A_AFCOEF110_COEFF1, data->iirCoeff1[1]);
  CSL_FINS(hndl->regs->AFCOEF132, H3A_AFCOEF132_COEFF2, data->iirCoeff1[2]);
  CSL_FINS(hndl->regs->AFCOEF132, H3A_AFCOEF132_COEFF3, data->iirCoeff1[3]);
  CSL_FINS(hndl->regs->AFCOEFF154, H3A_AFCOEFF154_COEFF4, data->iirCoeff1[4]);
  CSL_FINS(hndl->regs->AFCOEFF154, H3A_AFCOEFF154_COEFF5, data->iirCoeff1[5]);
  CSL_FINS(hndl->regs->AFCOEFF176, H3A_AFCOEFF176_COEFF6, data->iirCoeff1[6]);
  CSL_FINS(hndl->regs->AFCOEFF176, H3A_AFCOEFF176_COEFF7, data->iirCoeff1[7]);
  CSL_FINS(hndl->regs->AFCOEFF198, H3A_AFCOEFF198_COEFF8, data->iirCoeff1[8]);
  CSL_FINS(hndl->regs->AFCOEFF198, H3A_AFCOEFF198_COEFF9, data->iirCoeff1[9]);
  CSL_FINS(hndl->regs->AFCOEFF1010, H3A_AFCOEFF1010_COEFF10, data->iirCoeff1[10]);

  CSL_FINS(hndl->regs->VFV_CFG1, H3A_VFV_CFG1_VCOEF1_0, data->vfvFir1Coeff[0]);
  CSL_FINS(hndl->regs->VFV_CFG1, H3A_VFV_CFG1_VCOEF1_1, data->vfvFir1Coeff[1]);
  CSL_FINS(hndl->regs->VFV_CFG1, H3A_VFV_CFG1_VCOEF1_2, data->vfvFir1Coeff[2]);
  CSL_FINS(hndl->regs->VFV_CFG1, H3A_VFV_CFG1_VCOEF1_3, data->vfvFir1Coeff[3]);
  CSL_FINS(hndl->regs->VFV_CFG2, H3A_VFV_CFG2_VCOEF1_4, data->vfvFir1Coeff[4]);

  CSL_FINS(hndl->regs->VFV_CFG2, H3A_VFV_CFG2_VTHR1, data->vfvFir1Threshold);

  CSL_FINS(hndl->regs->HFV_THR, H3A_HFV_THR_HTHR1, data->hfvFir1Threshold);

  CSL_FINS(hndl->regs->VFV_CFG3, H3A_VFV_CFG3_VCOEF2_0, data->vfvFir2Coeff[0]);
  CSL_FINS(hndl->regs->VFV_CFG3, H3A_VFV_CFG3_VCOEF2_1, data->vfvFir2Coeff[1]);
  CSL_FINS(hndl->regs->VFV_CFG3, H3A_VFV_CFG3_VCOEF2_2, data->vfvFir2Coeff[2]);
  CSL_FINS(hndl->regs->VFV_CFG3, H3A_VFV_CFG3_VCOEF2_3, data->vfvFir2Coeff[3]);
  CSL_FINS(hndl->regs->VFV_CFG4, H3A_VFV_CFG4_VCOEF2_4, data->vfvFir2Coeff[4]);

  CSL_FINS(hndl->regs->VFV_CFG4, H3A_VFV_CFG4_VTHR2, data->vfvFir2Threshold);

  CSL_FINS(hndl->regs->HFV_THR, H3A_HFV_THR_HTHR2, data->hfvFir2Threshold);

  return status;
}

CSL_Status CSL_h3aAewbSetConfig(CSL_H3aHandle hndl, CSL_H3aAewbConfig * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  if (   ( (data->winHeight % 2) || (data->winWidth<2) || (data->winWidth > 512))
      || ( (data->winWidth  % 2) || (data->winWidth<8) )
      || ( (data->winNumH < 1)   || (data->winNumH > 36))
      || ( (data->winNumV < 1)   || (data->winNumV > 128))
      || ( (data->winIncH < 2)   || (data->winIncH > 32))
      || ( (data->winIncV < 2)   || (data->winIncV > 32))
      || ( (data->winBlackHeight % 2) || (data->winBlackHeight < 2 ) || (data->winBlackHeight > 256 ))
      || (data->shift > 15)
     )

    return CSL_EFAIL;

  CSL_FINS(hndl->regs->PCR, H3A_PCR_AEW_EN, data->enable);

  CSL_FINS(hndl->regs->PCR, H3A_PCR_AEW_ALAW_EN, data->alawEnable);

  CSL_FINS(hndl->regs->PCR, H3A_PCR_AEW_MED_EN, data->medFiltEnable);

  CSL_FINS(hndl->regs->PCR, H3A_PCR_AVE2LMT, data->satLimit);

  CSL_FINS(hndl->regs->AEWWIN1, H3A_AEWWIN1_WINW, (data->winWidth/2) - 1);

  CSL_FINS(hndl->regs->AEWWIN1, H3A_AEWWIN1_WINH, (data->winHeight/2) - 1);

  CSL_FINS(hndl->regs->AEWWIN1, H3A_AEWWIN1_WINHC, data->winNumH-1);

  CSL_FINS(hndl->regs->AEWWIN1, H3A_AEWWIN1_WINVC, data->winNumV-1);

  CSL_FINS(hndl->regs->AEWINSTART, H3A_AEWINSTART_WINSH, data->winStartH);

  CSL_FINS(hndl->regs->AEWINSTART, H3A_AEWINSTART_WINSV, data->winStartV);

  CSL_FINS(hndl->regs->AEWSUBWIN, H3A_AEWSUBWIN_AEWINCH, (data->winIncH/2) - 1);

  CSL_FINS(hndl->regs->AEWSUBWIN, H3A_AEWSUBWIN_AEWINCV, (data->winIncV/2) - 1);

  CSL_FINS(hndl->regs->AEWINBLK, H3A_AEWINBLK_WINSV, data->winBlackStartV);

  CSL_FINS(hndl->regs->AEWINBLK, H3A_AEWINBLK_WINH, (data->winBlackHeight/2) - 1);

  if (((Uint32) data->outputAddr) % 64)
    status = CSL_EFAIL;
  hndl->regs->AEWBUFST = (Uint32) data->outputAddr;

  CSL_FINS(hndl->regs->RSDR_ADDR, H3A_RSDR_ADDR_AEFMT, data->outFormat);

  CSL_FINS(hndl->regs->RSDR_ADDR, H3A_RSDR_ADDR_SUMSFT, data->shift);

  return status;
}

CSL_Status CSL_h3aAfEnable(CSL_H3aHandle hndl, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->PCR, H3A_PCR_AF_EN, enable);

  return CSL_SOK;
}

CSL_Status CSL_h3aAfVfEnable(CSL_H3aHandle hndl, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->PCR, H3A_PCR_AF_VF_EN, enable);

  return CSL_SOK;
}

CSL_Status CSL_h3aAfAlawEnable(CSL_H3aHandle hndl, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->PCR, H3A_PCR_AF_ALAW_EN, enable);

  return CSL_SOK;
}

CSL_Status CSL_h3aAfSetOutAddr(CSL_H3aHandle hndl, Uint8 * addr)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || addr == NULL)
    return CSL_EFAIL;

  if( (Uint32)addr%64)
    status = CSL_EFAIL;

  hndl->regs->AFBUFST = (Uint32) addr;

  return status;
}

CSL_Status CSL_h3aAewbEnable(CSL_H3aHandle hndl, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->PCR, H3A_PCR_AEW_EN, enable);

  return CSL_SOK;
}

CSL_Status CSL_h3aAewbAlawEnable(CSL_H3aHandle hndl, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->PCR, H3A_PCR_AEW_ALAW_EN, enable);

  return CSL_SOK;
}

CSL_Status CSL_h3aAewbSetOutAddr(CSL_H3aHandle hndl, Uint8 * addr)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || addr == NULL)
    return CSL_EFAIL;

  if( (Uint32)addr%64)
    status = CSL_EFAIL;

  hndl->regs->AEWBUFST = (Uint32) addr;

  return status;
}

