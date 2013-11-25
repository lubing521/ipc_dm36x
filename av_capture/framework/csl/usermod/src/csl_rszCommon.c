/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_rsz.h>

CSL_Status CSL_rszSetInConfig(CSL_RszHandle hndl, CSL_RszInConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->SRC_MODE, RSZ_SRC_MODE_WRT, data->wenUseEnable);
  CSL_FINS(hndl->regs->SRC_FMT0, RSZ_SRC_FMT0_BYPASS, data->passThruEnable);
  CSL_FINS(hndl->regs->SRC_FMT1, RSZ_SRC_FMT1_RAW, data->passThruInFormat);

  CSL_FINS(hndl->regs->SRC_FMT0, RSZ_SRC_FMT0_SRC, data->inputDataPath);

  CSL_FINS(hndl->regs->SRC_FMT1, RSZ_SRC_FMT1_420, data->chormaFormatType);
  CSL_FINS(hndl->regs->SRC_FMT1, RSZ_SRC_FMT1_COL, data->yuv420Color);

  hndl->regs->YUV_PHS = data->chormaPos;
  CSL_FINS(hndl->regs->SEQ, RSZ_SEQ_CRV, data->chormaFlip);

  return CSL_SOK;
}

CSL_Status CSL_rszSetInFrameConfig(CSL_RszHandle hndl, CSL_RszInFrameConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  hndl->regs->SRC_HPS = data->inStartX;
  hndl->regs->SRC_VPS = data->inStartY;

  hndl->regs->SRC_HSZ = data->inWidth - 1;
  hndl->regs->SRC_VSZ = data->inHeight - 1;

  return CSL_SOK;
}

CSL_Status CSL_rszGetInFrameConfig(CSL_RszHandle hndl, CSL_RszInFrameConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  data->inStartX = hndl->regs->SRC_HPS;
  data->inStartY = hndl->regs->SRC_VPS;

  data->inWidth = hndl->regs->SRC_HSZ + 1;
  data->inHeight = hndl->regs->SRC_VSZ + 1;

  return CSL_SOK;
}

CSL_Status CSL_rszSetClkConfig(CSL_RszHandle hndl, CSL_RszClkConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  hndl->regs->GCK_MMR = data->mmrClkEnable;
  hndl->regs->GCK_SDR = data->coreClkEnable;

  return CSL_SOK;
}

CSL_Status CSL_rszSetMinMaxConfig(CSL_RszHandle hndl, CSL_RszMinMaxConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  hndl->regs->YUV_Y_MIN = data->minY;
  hndl->regs->YUV_Y_MAX = data->maxY;
  hndl->regs->YUV_C_MIN = data->minC;
  hndl->regs->YUV_C_MAX = data->maxC;

  return CSL_SOK;
}

CSL_Status CSL_rszASetOutConfig(CSL_RszHandle hndl, CSL_RszOutConfig * data)
{
  Uint32  val, addr, inWidth, inHeight;
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->SEQ, RSZ_SEQ_HRVA, data->flipH);
  CSL_FINS(hndl->regs->SEQ, RSZ_SEQ_VRVA, data->flipV);

  hndl->regs->IRQ_RZA = data->irqInterval - 1;
  hndl->regs->DMA_RZA = data->dmaInterval;

  hndl->regs->RZA_EN = data->enable;
  hndl->regs->RZA_MODE = data->oneShotEnable;

  CSL_FINS(hndl->regs->RZA_420, RSZ_RZA_420_YEN, data->yuv420YOutEnable);
  CSL_FINS(hndl->regs->RZA_420, RSZ_RZA_420_CEN, data->yuv420COutEnable);

  hndl->regs->RZA_I_HPS = data->inStartX;
  hndl->regs->RZA_I_VPS = data->inStartY;

  hndl->regs->RZA_O_HSZ = data->outWidth - 1;
  hndl->regs->RZA_O_VSZ = data->outHeight - 1;

  hndl->regs->RZA_V_PHS_Y = data->vertStartPhaseY;
  hndl->regs->RZA_V_PHS_C = data->vertStartPhaseC;

  CSL_FINS(hndl->regs->RZA_V_TYP, RSZ_RZA_V_TYP_Y, data->vertReszTypeY);
  CSL_FINS(hndl->regs->RZA_V_TYP, RSZ_RZA_V_TYP_C, data->vertReszTypeC);

  CSL_FINS(hndl->regs->RZA_V_LPF, RSZ_RZA_V_LPF_C, data->vertLpfIntensityY);
  CSL_FINS(hndl->regs->RZA_V_LPF, RSZ_RZA_V_LPF_C, data->vertLpfIntensityC);

  hndl->regs->RZA_H_PHS = data->horzStartPhase;

  if(data->vertReszRatio==0) {
    inHeight = hndl->regs->SRC_VSZ+1;

    data->vertReszRatio = (inHeight*256)/(data->outHeight);
  }

  if(data->vertReszRatio < 16 || data->vertReszRatio > 4096)
    status = CSL_EFAIL;

  hndl->regs->RZA_V_DIF = data->vertReszRatio;

  if(data->horzReszRatio==0) {
    inWidth = hndl->regs->SRC_HSZ+1;

    data->horzReszRatio = (inWidth*256)/(data->outWidth);
  }

  if(data->horzReszRatio < 16 || data->horzReszRatio > 4096)
    status = CSL_EFAIL;

  hndl->regs->RZA_H_DIF = data->horzReszRatio;

  CSL_FINS(hndl->regs->RZA_H_TYP, RSZ_RZA_H_TYP_Y, data->vertReszTypeY);
  CSL_FINS(hndl->regs->RZA_H_TYP, RSZ_RZA_H_TYP_C, data->vertReszTypeC);

  CSL_FINS(hndl->regs->RZA_H_LPF, RSZ_RZA_H_LPF_C, data->vertLpfIntensityY);
  CSL_FINS(hndl->regs->RZA_H_LPF, RSZ_RZA_H_LPF_C, data->vertLpfIntensityC);

  hndl->regs->RZA_DWN_EN = data->downScaleModeEnable;
  CSL_FINS(hndl->regs->RZA_DWN_AV, RSZ_RZA_DWN_AV_V, data->downScaleModeAvgSizeV);
  CSL_FINS(hndl->regs->RZA_DWN_AV, RSZ_RZA_DWN_AV_H, data->downScaleModeAvgSizeH);

  hndl->regs->RZA_RGB_EN = data->rgbOutEnable;

  val = CSL_FMK(RSZ_RZA_RGB_TYP_MSK0, data->rgbOutMaskFirstEnable)
    | CSL_FMK(RSZ_RZA_RGB_TYP_MSK1, data->rgbOutMaskFirstEnable)
    | CSL_FMK(RSZ_RZA_RGB_TYP_TYP, data->rgbOutMaskFirstEnable);

  hndl->regs->RZA_RGB_TYP = val;
  hndl->regs->RZB_RGB_BLD = data->rgbOutAlphaVal;

  addr = (Uint32) data->outStartAddr;
  hndl->regs->RZA_SDR_Y_SAD_L = (Uint16) addr;
  hndl->regs->RZA_SDR_Y_SAD_H = (Uint16) (addr >> 16);

  addr = (Uint32) data->outBaseAddr;
  hndl->regs->RZA_SDR_Y_BAD_L = (Uint16) addr;
  hndl->regs->RZA_SDR_Y_BAD_H = (Uint16) (addr >> 16);

  if(data->outLineOffset%32)
    status = CSL_EFAIL;

  hndl->regs->RZA_SDR_Y_OFT = data->outLineOffset;

  hndl->regs->RZA_SDR_Y_PTR_S = data->outStartLine;
  hndl->regs->RZA_SDR_Y_PTR_E = data->outEndLine;

  addr = (Uint32) data->yuv420CoutStartAddr;
  hndl->regs->RZA_SDR_C_SAD_L = (Uint16) addr;
  hndl->regs->RZA_SDR_C_SAD_H = (Uint16) (addr >> 16);

  addr = (Uint32)data->yuv420CoutBaseAddr;
  hndl->regs->RZA_SDR_C_BAD_L = (Uint16) addr;
  hndl->regs->RZA_SDR_C_BAD_H = (Uint16) (addr >> 16);

  hndl->regs->RZA_SDR_C_OFT = data->yuv420CoutLineOffset;

  hndl->regs->RZA_SDR_C_PTR_S = data->yuv420CoutStartLine;
  hndl->regs->RZA_SDR_C_PTR_E = data->yuv420CoutEndLine;

  return status;
}

CSL_Status CSL_rszBSetOutConfig(CSL_RszHandle hndl, CSL_RszOutConfig * data)
{
  Uint32  val, addr, inWidth, inHeight;
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->SEQ, RSZ_SEQ_HRVB, data->flipH);
  CSL_FINS(hndl->regs->SEQ, RSZ_SEQ_VRVB, data->flipV);

  hndl->regs->IRQ_RZB = data->irqInterval - 1;
  hndl->regs->DMA_RZB = data->dmaInterval;

  hndl->regs->RZB_EN = data->enable;
  hndl->regs->RZB_MODE = data->oneShotEnable;

  CSL_FINS(hndl->regs->RZB_420, RSZ_RZB_420_YEN, data->yuv420YOutEnable);
  CSL_FINS(hndl->regs->RZB_420, RSZ_RZB_420_CEN, data->yuv420COutEnable);

  hndl->regs->RZB_I_HPS = data->inStartX;
  hndl->regs->RZB_I_VPS = data->inStartY;

  hndl->regs->RZB_O_HSZ = data->outWidth - 1;
  hndl->regs->RZB_O_VSZ = data->outHeight - 1;

  hndl->regs->RZB_V_PHS_Y = data->vertStartPhaseY;
  hndl->regs->RZB_V_PHS_C = data->vertStartPhaseC;

  CSL_FINS(hndl->regs->RZB_V_TYP, RSZ_RZB_V_TYP_Y, data->vertReszTypeY);
  CSL_FINS(hndl->regs->RZB_V_TYP, RSZ_RZB_V_TYP_C, data->vertReszTypeC);

  CSL_FINS(hndl->regs->RZB_V_LPF, RSZ_RZB_V_LPF_C, data->vertLpfIntensityY);
  CSL_FINS(hndl->regs->RZB_V_LPF, RSZ_RZB_V_LPF_C, data->vertLpfIntensityC);

  hndl->regs->RZB_H_PHS = data->horzStartPhase;

  if(data->vertReszRatio==0) {
    inHeight = hndl->regs->SRC_VSZ+1;

    data->vertReszRatio = (inHeight*256)/(data->outHeight);
  }

  if(data->vertReszRatio < 16 || data->vertReszRatio > 4096)
    status = CSL_EFAIL;

  hndl->regs->RZB_V_DIF = data->vertReszRatio;

  if(data->horzReszRatio==0) {
    inWidth = hndl->regs->SRC_HSZ+1;

    data->horzReszRatio = (inWidth*256)/(data->outWidth);
  }

  if(data->horzReszRatio < 16 || data->horzReszRatio > 4096)
    status = CSL_EFAIL;

  hndl->regs->RZB_H_DIF = data->horzReszRatio;


  CSL_FINS(hndl->regs->RZB_H_TYP, RSZ_RZB_H_TYP_Y, data->vertReszTypeY);
  CSL_FINS(hndl->regs->RZB_H_TYP, RSZ_RZB_H_TYP_C, data->vertReszTypeC);

  CSL_FINS(hndl->regs->RZB_H_LPF, RSZ_RZB_H_LPF_C, data->vertLpfIntensityY);
  CSL_FINS(hndl->regs->RZB_H_LPF, RSZ_RZB_H_LPF_C, data->vertLpfIntensityC);

  hndl->regs->RZB_DWN_EN = data->downScaleModeEnable;
  CSL_FINS(hndl->regs->RZB_DWN_AV, RSZ_RZB_DWN_AV_V, data->downScaleModeAvgSizeV);
  CSL_FINS(hndl->regs->RZB_DWN_AV, RSZ_RZB_DWN_AV_H, data->downScaleModeAvgSizeH);

  hndl->regs->RZA_RGB_EN = data->rgbOutEnable;

  val = CSL_FMK(RSZ_RZB_RGB_TYP_MSK1, data->rgbOutMaskFirstEnable)
    | CSL_FMK(RSZ_RZB_RGB_TYP_MSK0, data->rgbOutMaskFirstEnable)
    | CSL_FMK(RSZ_RZB_RGB_TYP_TYP, data->rgbOutMaskFirstEnable);

  hndl->regs->RZB_RGB_TYP = val;
  hndl->regs->RZB_RGB_BLD = data->rgbOutAlphaVal;

  addr = (Uint32) data->outStartAddr;
  hndl->regs->RZB_SDR_Y_SAD_L = (Uint16) addr;
  hndl->regs->RZB_SDR_Y_SAD_H = (Uint16) (addr >> 16);

  addr = (Uint32) data->outBaseAddr;
  hndl->regs->RZB_SDR_Y_BAD_L = (Uint16) addr;
  hndl->regs->RZB_SDR_Y_BAD_H = (Uint16) (addr >> 16);

  if(data->outLineOffset%32)
    status = CSL_EFAIL;
  hndl->regs->RZB_SDR_Y_OFT = data->outLineOffset;

  hndl->regs->RZB_SDR_Y_PTR_S = data->outStartLine;
  hndl->regs->RZB_SDR_Y_PTR_E = data->outEndLine;

  addr = (Uint32) data->yuv420CoutStartAddr;
  hndl->regs->RZB_SDR_C_SAD_L = (Uint16) addr;
  hndl->regs->RZB_SDR_C_SAD_H = (Uint16) (addr >> 16);

  addr = (Uint32) data->yuv420CoutBaseAddr;
  hndl->regs->RZB_SDR_C_BAD_L = (Uint16) addr;
  hndl->regs->RZB_SDR_C_BAD_H = (Uint16) (addr >> 16);

  hndl->regs->RZB_SDR_C_OFT = data->yuv420CoutLineOffset;

  hndl->regs->RZB_SDR_C_PTR_S = data->yuv420CoutStartLine;
  hndl->regs->RZB_SDR_C_PTR_E = data->yuv420CoutEndLine;

  return status;
}

CSL_Status CSL_rszSetOutConfig(CSL_RszHandle hndl, Uint8 rszMod, CSL_RszOutConfig * data)
{
  CSL_Status status = CSL_EFAIL;

  if(rszMod >=CSL_RSZ_CH_MAX)
    return status;

  if (rszMod == CSL_RSZ_A)
    status = CSL_rszASetOutConfig(hndl, data);

  if (rszMod == CSL_RSZ_B)
    status = CSL_rszBSetOutConfig(hndl, data);

  CSL_rszSetFlip(hndl, rszMod, data->flipH, data->flipV);

  return status;
}

CSL_Status CSL_rszEnable(CSL_RszHandle hndl, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  hndl->regs->SRC_EN = enable;

  return CSL_SOK;
}

CSL_Status CSL_rszOneShotEnable(CSL_RszHandle hndl, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->SRC_MODE, RSZ_SRC_MODE_OST, enable);

  return CSL_SOK;
}

CSL_Status CSL_rszOutEnable(CSL_RszHandle hndl, Uint8 rszMod, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  if (rszMod == CSL_RSZ_A)
    hndl->regs->RZA_EN = enable;
  else if (rszMod == CSL_RSZ_B)
    hndl->regs->RZB_EN = enable;

  return CSL_SOK;
}

CSL_Status CSL_rszOutOneShotEnable(CSL_RszHandle hndl, Uint8 rszMod, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  if (rszMod == CSL_RSZ_A)
    hndl->regs->RZA_MODE = enable;
  else if (rszMod == CSL_RSZ_B)
    hndl->regs->RZB_MODE = enable;

  return CSL_SOK;
}

CSL_Status CSL_rszSetOutAddr(CSL_RszHandle hndl, Uint8 rszMod, Uint8 * addr, Uint8 *yuv420CoutAddr)
{
  Uint32  addr32, caddr32;

  if (hndl == NULL)
    return CSL_EFAIL;

  addr32 = (Uint32) addr;
  caddr32 = (Uint32) yuv420CoutAddr;

  if (rszMod == CSL_RSZ_A) {

    hndl->regs->RZA_SDR_Y_SAD_L = (Uint16) addr32;
    hndl->regs->RZA_SDR_Y_SAD_H = (Uint16) (addr32 >> 16);
    hndl->regs->RZA_SDR_Y_BAD_L = (Uint16) addr32;
    hndl->regs->RZA_SDR_Y_BAD_H = (Uint16) (addr32 >> 16);

    hndl->regs->RZA_SDR_C_SAD_L = (Uint16) caddr32;
    hndl->regs->RZA_SDR_C_SAD_H = (Uint16) (caddr32 >> 16);
    hndl->regs->RZA_SDR_C_BAD_L = (Uint16) caddr32;
    hndl->regs->RZA_SDR_C_BAD_H = (Uint16) (caddr32 >> 16);

  } else if (rszMod == CSL_RSZ_B) {

    hndl->regs->RZB_SDR_Y_SAD_L = (Uint16) addr32;
    hndl->regs->RZB_SDR_Y_SAD_H = (Uint16) (addr32 >> 16);
    hndl->regs->RZB_SDR_Y_BAD_L = (Uint16) addr32;
    hndl->regs->RZB_SDR_Y_BAD_H = (Uint16) (addr32 >> 16);

    hndl->regs->RZB_SDR_C_SAD_L = (Uint16) caddr32;
    hndl->regs->RZB_SDR_C_SAD_H = (Uint16) (caddr32 >> 16);
    hndl->regs->RZB_SDR_C_BAD_L = (Uint16) caddr32;
    hndl->regs->RZB_SDR_C_BAD_H = (Uint16) (caddr32 >> 16);
  }

  return CSL_SOK;
}

CSL_Status CSL_rszGetStatus(CSL_RszHandle hndl, Uint8 rszMod, CSL_RszStatus * status)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  if (rszMod == CSL_RSZ_A) {
    status->valLastPhase = hndl->regs->RZA_V_PHS_C;
    status->numLastLines = hndl->regs->RZA_O_VSZ;
    status->vLastSdr = hndl->regs->RZA_SDR_Y_PTR_E;
  } else if (rszMod == CSL_RSZ_B) {
    status->valLastPhase = hndl->regs->RZB_V_PHS_C;
    status->numLastLines = hndl->regs->RZB_O_VSZ;
    status->vLastSdr = hndl->regs->RZB_SDR_Y_PTR_E;
  }

  return CSL_SOK;
}

CSL_Status CSL_rszSetOutFrameConfig(CSL_RszHandle hndl, Uint8 rszMod, CSL_RszInFrameConfig * inFrameConfig, CSL_RszOutFrameConfig * outConfig)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  if (rszMod == CSL_RSZ_A) {
    hndl->regs->RZA_O_HSZ = outConfig->outWidth - 1;
    hndl->regs->RZA_O_VSZ = outConfig->outHeight - 1;
    hndl->regs->RZA_SDR_Y_OFT = outConfig->outLineOffset;
    hndl->regs->RZA_SDR_Y_PTR_E = outConfig->outBufHeight;
    hndl->regs->RZA_H_DIF = outConfig->horzReszRatio;
    hndl->regs->RZA_V_DIF = outConfig->vertReszRatio;
  } else if (rszMod == CSL_RSZ_B) {
    hndl->regs->RZB_O_HSZ = outConfig->outWidth - 1;
    hndl->regs->RZB_O_VSZ = outConfig->outHeight - 1;
    hndl->regs->RZB_SDR_Y_OFT = outConfig->outLineOffset;
    hndl->regs->RZB_SDR_Y_PTR_E = outConfig->outBufHeight;
    hndl->regs->RZB_H_DIF = outConfig->horzReszRatio;
    hndl->regs->RZB_V_DIF = outConfig->vertReszRatio;
  }
  return CSL_SOK;
}

CSL_Status CSL_rszGetOutFrameConfig(CSL_RszHandle hndl, Uint8 rszMod, CSL_RszOutFrameConfig * outConfig)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  if (rszMod == CSL_RSZ_A) {
    outConfig->outWidth = hndl->regs->RZA_O_HSZ + 1;
    outConfig->outHeight = hndl->regs->RZA_O_VSZ + 1;
    outConfig->outLineOffset = hndl->regs->RZA_SDR_Y_OFT;
    outConfig->outBufHeight = hndl->regs->RZA_SDR_Y_PTR_E;
    outConfig->horzReszRatio = hndl->regs->RZA_H_DIF;
    outConfig->vertReszRatio = hndl->regs->RZA_V_DIF;
  } else if (rszMod == CSL_RSZ_B) {
    outConfig->outWidth = hndl->regs->RZB_O_HSZ + 1;
    outConfig->outHeight = hndl->regs->RZB_O_VSZ + 1;
    outConfig->outLineOffset = hndl->regs->RZB_SDR_Y_OFT;
    outConfig->outBufHeight = hndl->regs->RZB_SDR_Y_PTR_E;
    outConfig->horzReszRatio = hndl->regs->RZB_H_DIF;
    outConfig->vertReszRatio = hndl->regs->RZB_V_DIF;
  }
  return CSL_SOK;
}

CSL_Status CSL_rszSetDzoomConfig(CSL_RszHandle hndl, CSL_RszDzoomConfig *dzoomConfig)
{
  if (hndl == NULL || dzoomConfig==NULL)
    return CSL_EFAIL;

  hndl->regs->RZA_V_DIF = dzoomConfig->vrsz[0];
  hndl->regs->RZA_H_DIF = dzoomConfig->hrsz[0];
  hndl->regs->RZB_V_DIF = dzoomConfig->vrsz[1];
  hndl->regs->RZB_H_DIF = dzoomConfig->hrsz[1];
  hndl->regs->SRC_HPS = dzoomConfig->inStartX;
  hndl->regs->SRC_VPS = dzoomConfig->inStartY;
  hndl->regs->SRC_HSZ = dzoomConfig->inWidth - 1;
  hndl->regs->SRC_VSZ = dzoomConfig->inHeight - 1;

  return CSL_SOK;
}

