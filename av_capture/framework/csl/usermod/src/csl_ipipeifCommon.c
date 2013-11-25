/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_ipipeif.h>



CSL_Status CSL_ipipeifSetSdramInConfig(CSL_IpipeifHandle hndl, CSL_IpipeifSdramInConfig * data)
{
  CSL_Status status = CSL_SOK;

  CSL_IpipeifSdramInFrameConfig inFrameConfig;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->ENABLE, IPIPEIF_ENABLE_ENABLE, 0);
  
  CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_ONESHOT, data->oneShotEnable);
  CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_DATASFT, data->rawDataShift);
  CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_PACK8IN, data->rawUnpackMode);

  inFrameConfig.inputAddr = data->inputAddr;
  inFrameConfig.inputWidth = data->inputWidth;
  inFrameConfig.inputHeight = data->inputHeight;
  inFrameConfig.inputLineOffset = data->inputLineOffset;

  if (CSL_ipipeifSetSdramInFrameConfig(hndl, &inFrameConfig) != CSL_SOK)
    status = CSL_EFAIL;

  CSL_FINS(hndl->regs->DPC2, IPIPEIF_DPC2_DPC2ENA, data->dpcEnable);
  CSL_FINS(hndl->regs->DPC2, IPIPEIF_DPC2_DPC2TH, data->dpcThreshold);

  if (CSL_ipipeifSetDpcmConfig(hndl, data->dpcmConfig) != CSL_SOK)
    status = CSL_EFAIL;

  if (CSL_ipipeifSetClkConfig(hndl, data->clkConfig) != CSL_SOK)
    status = CSL_EFAIL;

  CSL_FINS(hndl->regs->ENABLE, IPIPEIF_ENABLE_ENABLE, data->enable);

  return status;
}

CSL_Status CSL_ipipeifSetSdramInFrameConfig(CSL_IpipeifHandle hndl, CSL_IpipeifSdramInFrameConfig * data)
{
  CSL_Status status = CSL_SOK;
  Uint32 addrOffset;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  if (CSL_ipipeifSetSdramInAddr(hndl, data->inputAddr) != CSL_SOK)
    status = CSL_EFAIL;

  if (data->inputLineOffset % 32)
    status = CSL_EFAIL;
    
  addrOffset = data->inputLineOffset / 32;

  hndl->regs->HNUM = data->inputWidth;
  hndl->regs->VNUM = data->inputHeight;
  
  hndl->regs->ADOFS = addrOffset;
  CSL_FINS(hndl->regs->ADDRU, IPIPEIF_ADDRU_ADOFS9, addrOffset >> 9);

  return status;
}

CSL_Status CSL_ipipeifSetInSource2Config(CSL_IpipeifHandle hndl, CSL_IpipeifInSource2Config * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_INPSRC2, data->inputSource2);
  CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_DECM, data->pixelDecimationEnable);

  hndl->regs->RSZ = data->pixelDecimationRatio;
  CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_AVGFILT, data->avgFilterEnable);

  CSL_FINS(hndl->regs->INIRSZ, IPIPEIF_INIRSZ_SYNCAL, data->alignHsyncVsync);
  CSL_FINS(hndl->regs->INIRSZ, IPIPEIF_INIRSZ_INIRSZ, data->initReszPosH);

  hndl->regs->OCLIP = data->outClip;

  hndl->regs->GAIN = data->gain;

  CSL_FINS(hndl->regs->CFG2, IPIPEIF_CFG2_INPTYP, data->isifInType);
  CSL_FINS(hndl->regs->CFG2, IPIPEIF_CFG2_YUV8, data->yuvConvertEnable);
  CSL_FINS(hndl->regs->ENABLE, IPIPEIF_ENABLE_SYNCOFF, data->vsyncMaskEnable);

  return CSL_SOK;
}


CSL_Status CSL_ipipeifSetVpifIsifInConfig(CSL_IpipeifHandle hndl, CSL_IpipeifVpifIsifInConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->DPC1, IPIPEIF_DPC1_DPC1ENA, data->dpcEnable);
  CSL_FINS(hndl->regs->DPC1, IPIPEIF_DPC1_DPC1TH, data->dpcThreshold);
  CSL_FINS(hndl->regs->CFG2, IPIPEIF_CFG2_EXWEN, data->wenUseEnable);
  CSL_FINS(hndl->regs->CFG2, IPIPEIF_CFG2_VDPOL, data->vdPol);
  CSL_FINS(hndl->regs->CFG2, IPIPEIF_CFG2_HDPOL, data->hdPol);

  return CSL_SOK;
}

CSL_Status CSL_ipipeifSetDarkFrameConfig(CSL_IpipeifHandle hndl, CSL_IpipeifDarkFrameConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->DFSGVL, IPIPEIF_DFSGVL_DFSGEN, data->gainEnable);
  CSL_FINS(hndl->regs->DFSGVL, IPIPEIF_DFSGVL_DFSGVL, data->gain);

  hndl->regs->DFSGTH = data->gainThreshold;

  CSL_FINS(hndl->regs->CFG2, IPIPEIF_CFG2_DFSDIR, data->subtractDir);

  return CSL_SOK;
}



CSL_Status CSL_ipipeifSetH3aConfig(CSL_IpipeifHandle hndl, CSL_IpipeifH3aConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->RSZ2, IPIPEIF_RSZ2_DECM2, data->pixelDecimationEnable);
  CSL_FINS(hndl->regs->RSZ2, IPIPEIF_RSZ2_RSZ2, data->pixelDecimationRatio);
  CSL_FINS(hndl->regs->RSZ2, IPIPEIF_RSZ2_AVGFILT2, data->avgFilterEnable);

  CSL_FINS(hndl->regs->INIRSZ2, IPIPEIF_INIRSZ2_SYNCAL2, data->alignHsyncVsync);
  CSL_FINS(hndl->regs->INIRSZ2, IPIPEIF_INIRSZ2_INIRSZ2, data->initReszPosH);

  return CSL_SOK;
}


CSL_Status CSL_ipipeifSetClkConfig(CSL_IpipeifHandle hndl, CSL_IpipeifClkConfig * data)
{
  Uint16  clkDiv;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_CLKSEL, data->clkSel);

  clkDiv = (Uint8) (data->clkDivN - 1) | ( (Uint16)(data->clkDivM - 1) << 8);

  hndl->regs->CLKDIV = clkDiv;
  hndl->regs->PPLN = data->pixelsPerLine;
  hndl->regs->LPFR = data->linesPerFrame;

  return CSL_SOK;
}

CSL_Status CSL_ipipeifSetDpcmConfig(CSL_IpipeifHandle hndl, CSL_IpipeifDpcmConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->DPCM, IPIPEIF_DPCM_ENA, data->enable);
  CSL_FINS(hndl->regs->DPCM, IPIPEIF_DPCM_BITS, data->bits);
  CSL_FINS(hndl->regs->DPCM, IPIPEIF_DPCM_PRED, data->predType);

  return CSL_SOK;
}

CSL_Status CSL_ipipeifSetInputSource1(CSL_IpipeifHandle hndl, Uint8 inputSource1)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_INPSRC1, inputSource1);

  return CSL_SOK;
}

CSL_Status CSL_ipipeifSetSdramInAddr(CSL_IpipeifHandle hndl, Uint8 * addr)
{
  CSL_Status status = CSL_SOK;
  Uint32  sdramAddr;

  if (hndl == NULL)
    return CSL_EFAIL;

  sdramAddr = (Uint32) addr;
  if (sdramAddr % 32)
    status = CSL_EFAIL;

  hndl->regs->ADDRL = (Uint16)(sdramAddr >> 5);
  CSL_FINS(hndl->regs->ADDRU, IPIPEIF_ADDRU_ADDRU, (Uint16) (sdramAddr >> 21));
  CSL_FINS(hndl->regs->ADDRU, IPIPEIF_ADDRU_ADDRMSB, (Uint16) (sdramAddr >> 31));

  return status;
}

CSL_Status CSL_ipipeifSdramInEnable(CSL_IpipeifHandle hndl, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->ENABLE, IPIPEIF_ENABLE_ENABLE, enable);
  return CSL_SOK;
}

CSL_Status CSL_ipipeifSdramInOneShotEnable(CSL_IpipeifHandle hndl, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_ONESHOT, enable);

  return CSL_SOK;
}

CSL_Status CSL_ipipeifUnderflowDetectClear(CSL_IpipeifHandle hndl)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  hndl->regs->UFERR = 1;

  return CSL_SOK;
}

CSL_Status CSL_ipipeifIsUnderflowDetect(CSL_IpipeifHandle hndl, Bool32 * underflowDetect)
{
  if (hndl == NULL || underflowDetect == NULL)
    return CSL_EFAIL;

  *underflowDetect = hndl->regs->UFERR;

  return CSL_SOK;
}
