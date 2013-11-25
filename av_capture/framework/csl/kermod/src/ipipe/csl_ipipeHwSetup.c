/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_ipipe.h>

CSL_Status CSL_ipipeHwSetup(CSL_IpipeHandle hndl, CSL_IpipeHwSetup * data)
{
  Uint8   count;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->SRC_MODE, IPIPE_SRC_MODE_OST, data->oneShotEnable);
  CSL_FINS(hndl->regs->SRC_MODE, IPIPE_SRC_MODE_WRT, data->wenUseEnable);
  hndl->regs->SRC_FMT = data->dataPath;


  if ((CSL_ipipeSetColPatConfig(hndl, data->colpatConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->SRC_COL = CSL_IPIPE_SRC_COL_RESETVAL;
  }
  if ((CSL_ipipeSetClkConfig(hndl, data->clkConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->GCK_PIX = CSL_IPIPE_GCK_PIX_RESETVAL;
  }
  if ((CSL_ipipeSetInFrameConfig(hndl, data->inFrameConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->SRC_VPS = CSL_IPIPE_SRC_VPS_RESETVAL;
    hndl->regs->SRC_VSZ = CSL_IPIPE_SRC_VSZ_RESETVAL;
    hndl->regs->SRC_HPS = CSL_IPIPE_SRC_HPS_RESETVAL;
    hndl->regs->SRC_HSZ = CSL_IPIPE_SRC_HSZ_RESETVAL;
  }
  if ((CSL_ipipeSetLscConfig(hndl, data->lscConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->LSC_VOFT = CSL_IPIPE_LSC_VOFT_RESETVAL;
    hndl->regs->LSC_VA2 = CSL_IPIPE_LSC_VA2_RESETVAL;
    hndl->regs->LSC_VA1 = CSL_IPIPE_LSC_VA1_RESETVAL;
    hndl->regs->LSC_VS = CSL_IPIPE_LSC_VS_RESETVAL;
    hndl->regs->LSC_HOFT = CSL_IPIPE_LSC_HOFT_RESETVAL;
    hndl->regs->LSC_HA2 = CSL_IPIPE_LSC_HA2_RESETVAL;
    hndl->regs->LSC_HA1 = CSL_IPIPE_LSC_HA1_RESETVAL;
    hndl->regs->LSC_HS = CSL_IPIPE_LSC_HS_RESETVAL;
    hndl->regs->LSC_GAN_R = CSL_IPIPE_LSC_GAN_R_RESETVAL;
    hndl->regs->LSC_GAN_GR = CSL_IPIPE_LSC_GAN_GR_RESETVAL;
    hndl->regs->LSC_GAN_GB = CSL_IPIPE_LSC_GAN_GB_RESETVAL;
    hndl->regs->LSC_GAN_B = CSL_IPIPE_LSC_GAN_B_RESETVAL;
    hndl->regs->LSC_OFT_R = CSL_IPIPE_LSC_OFT_R_RESETVAL;
    hndl->regs->LSC_OFT_GR = CSL_IPIPE_LSC_OFT_GR_RESETVAL;
    hndl->regs->LSC_OFT_GB = CSL_IPIPE_LSC_OFT_GB_RESETVAL;
    hndl->regs->LSC_OFT_B = CSL_IPIPE_LSC_OFT_B_RESETVAL;
    hndl->regs->LSC_SHF = CSL_IPIPE_LSC_SHF_RESETVAL;
    hndl->regs->LSC_MAX = CSL_IPIPE_LSC_MAX_RESETVAL;
  }
  if ((CSL_ipipeSetDpcConfig(hndl, data->dpcConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->DPC_LUT_EN = CSL_IPIPE_DPC_LUT_EN_RESETVAL;
    hndl->regs->DPC_LUT_SEL = CSL_IPIPE_DPC_LUT_SEL_RESETVAL;
    hndl->regs->DPC_LUT_ADR = CSL_IPIPE_DPC_LUT_ADR_RESETVAL;
    hndl->regs->DPC_LUT_SIZ = CSL_IPIPE_DPC_LUT_SIZ_RESETVAL;
    hndl->regs->DPC_OTF_EN = CSL_IPIPE_DPC_OTF_EN_RESETVAL;
    hndl->regs->DPC_OTF_TYP = CSL_IPIPE_DPC_OTF_TYP_RESETVAL;
    hndl->regs->DPC_OTF_2_D_THR_R = CSL_IPIPE_DPC_OTF_2_D_THR_R_RESETVAL;
    hndl->regs->DPC_OTF_2_D_THR_GR = CSL_IPIPE_DPC_OTF_2_D_THR_GR_RESETVAL;
    hndl->regs->DPC_OTF_2_D_THR_GB = CSL_IPIPE_DPC_OTF_2_D_THR_GB_RESETVAL;
    hndl->regs->DPC_OTF_2_D_THR_B = CSL_IPIPE_DPC_OTF_2_D_THR_B_RESETVAL;
    hndl->regs->DPC_OTF_2_C_THR_R = CSL_IPIPE_DPC_OTF_2_C_THR_R_RESETVAL;
    hndl->regs->DPC_OTF_2_C_THR_GR = CSL_IPIPE_DPC_OTF_2_C_THR_GR_RESETVAL;
    hndl->regs->DPC_OTF_2_C_THR_GB = CSL_IPIPE_DPC_OTF_2_C_THR_GB_RESETVAL;
    hndl->regs->DPC_OTF_2_C_THR_B = CSL_IPIPE_DPC_OTF_2_C_THR_B_RESETVAL;
    hndl->regs->DPC_OTF_3_SHF = CSL_IPIPE_DPC_OTF_3_SHF_RESETVAL;
    hndl->regs->DPC_OTF_3_D_THR = CSL_IPIPE_DPC_OTF_3_D_THR_RESETVAL;
    hndl->regs->DPC_OTF_3_D_SLP = CSL_IPIPE_DPC_OTF_3_D_SLP_RESETVAL;
    hndl->regs->DPC_OTF_3_D_MIN = CSL_IPIPE_DPC_OTF_3_D_MIN_RESETVAL;
    hndl->regs->DPC_OTF_3_D_MAX = CSL_IPIPE_DPC_OTF_3_D_MAX_RESETVAL;
    hndl->regs->DPC_OTF_3_C_THR = CSL_IPIPE_DPC_OTF_3_C_THR_RESETVAL;
    hndl->regs->DPC_OTF_3_C_SLP = CSL_IPIPE_DPC_OTF_3_C_SLP_RESETVAL;
    hndl->regs->DPC_OTF_3_C_MIN = CSL_IPIPE_DPC_OTF_3_C_MIN_RESETVAL;
    hndl->regs->DPC_OTF_3_C_MAX = CSL_IPIPE_DPC_OTF_3_C_MAX_RESETVAL;
  }
  if ((CSL_ipipeSetNf1Config(hndl, data->nf1Config) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->D2F_1ST_TYP = CSL_IPIPE_D2F_1ST_TYP_RESETVAL;
    hndl->regs->D2F_1ST_EN = CSL_IPIPE_D2F_1ST_EN_RESETVAL;
    hndl->regs->D2F_1ST_TYP = CSL_IPIPE_D2F_1ST_TYP_RESETVAL;
    for (count = 0; count < 8; count++) {
      hndl->regs->D2F_1ST_THR[count] = CSL_IPIPE_D2F_1ST_THR_RESETVAL;
      hndl->regs->D2F_1ST_STR[count] = CSL_IPIPE_D2F_1ST_STR_RESETVAL;
      hndl->regs->D2F_1ST_SPR[count] = CSL_IPIPE_D2F_1ST_SPR_RESETVAL;
    }
    hndl->regs->D2F_1ST_EDG_MIN = CSL_IPIPE_D2F_1ST_EDG_MIN_RESETVAL;
    hndl->regs->D2F_1ST_EDG_MAX = CSL_IPIPE_D2F_1ST_EDG_MAX_RESETVAL;
  }
  if ((CSL_ipipeSetNf2Config(hndl, data->nf2Config) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->D2F_2ND_EN = CSL_IPIPE_D2F_2ND_EN_RESETVAL;
    hndl->regs->D2F_2ND_TYP = CSL_IPIPE_D2F_2ND_TYP_RESETVAL;
    for (count = 0; count < 8; count++) {
      hndl->regs->D2F_2ND_THR[0] = CSL_IPIPE_D2F_2ND_THR_RESETVAL;
      hndl->regs->D2F_2ND_STR[0] = CSL_IPIPE_D2F_2ND_STR_RESETVAL;
      hndl->regs->D2F_2ND_SPR[0] = CSL_IPIPE_D2F_2ND_SPR_RESETVAL;
    }
    hndl->regs->D2F_2ND_EDG_MIN = CSL_IPIPE_D2F_2ND_EDG_MIN_RESETVAL;
    hndl->regs->D2F_2ND_EDG_MAX = CSL_IPIPE_D2F_2ND_EDG_MAX_RESETVAL;;
  }
  if ((CSL_ipipeSetGicConfig(hndl, data->gicConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->GIC_EN = CSL_IPIPE_GIC_EN_RESETVAL;
  }
  if ((CSL_ipipeSetWbConfig(hndl, data->wbConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->WB2_OFT_R = CSL_IPIPE_WB2_OFT_R_RESETVAL;
    hndl->regs->WB2_OFT_GR = CSL_IPIPE_WB2_OFT_GR_RESETVAL;
    hndl->regs->WB2_OFT_GB = CSL_IPIPE_WB2_OFT_GB_RESETVAL;
    hndl->regs->WB2_OFT_B = CSL_IPIPE_WB2_OFT_B_RESETVAL;
    hndl->regs->WB2_WGN_R = CSL_IPIPE_WB2_WGN_R_RESETVAL;
    hndl->regs->WB2_WGN_GR = CSL_IPIPE_WB2_WGN_GR_RESETVAL;
    hndl->regs->WB2_WGN_GB = CSL_IPIPE_WB2_WGN_GB_RESETVAL;;
    hndl->regs->WB2_WGN_B = CSL_IPIPE_WB2_WGN_B_RESETVAL;;
  }
  if ((CSL_ipipeSetCfaConfig(hndl, data->cfaConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->CFA_MODE = CSL_IPIPE_CFA_MODE_RESETVAL;;
    hndl->regs->CFA_2DIR_HPF_THR = CSL_IPIPE_CFA_2DIR_HPF_THR_RESETVAL;
    hndl->regs->CFA_2DIR_HPF_SLP = CSL_IPIPE_CFA_2DIR_HPF_SLP_RESETVAL;
    hndl->regs->CFA_2DIR_MIX_THR = CSL_IPIPE_CFA_2DIR_MIX_THR_RESETVAL;
    hndl->regs->CFA_2DIR_MIX_SLP = CSL_IPIPE_CFA_2DIR_MIX_SLP_RESETVAL;
    hndl->regs->CFA_2DIR_DIR_THR = CSL_IPIPE_CFA_2DIR_DIR_THR_RESETVAL;
    hndl->regs->CFA_2DIR_DIR_SLP = CSL_IPIPE_CFA_2DIR_DIR_SLP_RESETVAL;
    hndl->regs->CFA_2DIR_NDWT = CSL_IPIPE_CFA_2DIR_NDWT_RESETVAL;
    hndl->regs->CFA_MONO_HUE_FRA = CSL_IPIPE_CFA_MONO_HUE_FRA_RESETVAL;
    hndl->regs->CFA_MONO_EDG_THR = CSL_IPIPE_CFA_MONO_EDG_THR_RESETVAL;
    hndl->regs->CFA_MONO_THR_MIN = CSL_IPIPE_CFA_MONO_THR_MIN_RESETVAL;
    hndl->regs->CFA_MONO_THR_SLP = CSL_IPIPE_CFA_MONO_THR_SLP_RESETVAL;
    hndl->regs->CFA_MONO_SLP_MIN = CSL_IPIPE_CFA_MONO_SLP_MIN_RESETVAL;
    hndl->regs->CFA_MONO_SLP_SLP = CSL_IPIPE_CFA_MONO_SLP_SLP_RESETVAL;
    hndl->regs->CFA_MONO_LPWT = CSL_IPIPE_CFA_MONO_LPWT_RESETVAL;;
  }
  if ((CSL_ipipeSetRgb2Rgb1Config(hndl, data->rgb2Rgb1Config) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->RGB1_MUL_RR = CSL_IPIPE_RGB1_MUL_RR_RESETVAL;
    hndl->regs->RGB1_MUL_GR = CSL_IPIPE_RGB1_MUL_GR_RESETVAL;
    hndl->regs->RGB1_MUL_BR = CSL_IPIPE_RGB1_MUL_BR_RESETVAL;
    hndl->regs->RGB1_MUL_RB = CSL_IPIPE_RGB1_MUL_RG_RESETVAL;
    hndl->regs->RGB1_MUL_GB = CSL_IPIPE_RGB1_MUL_GG_RESETVAL;
    hndl->regs->RGB1_MUL_BB = CSL_IPIPE_RGB1_MUL_BG_RESETVAL;
    hndl->regs->RGB1_MUL_RB = CSL_IPIPE_RGB1_MUL_RB_RESETVAL;
    hndl->regs->RGB1_MUL_GB = CSL_IPIPE_RGB1_MUL_GB_RESETVAL;
    hndl->regs->RGB1_MUL_BB = CSL_IPIPE_RGB1_MUL_BB_RESETVAL;
    hndl->regs->RGB1_OFT_OR = CSL_IPIPE_RGB1_OFT_OR_RESETVAL;
    hndl->regs->RGB1_OFT_OG = CSL_IPIPE_RGB1_OFT_OG_RESETVAL;
    hndl->regs->RGB1_OFT_OB = CSL_IPIPE_RGB1_OFT_OB_RESETVAL;
  }
  if ((CSL_ipipeSetGammaConfig(hndl, data->gammaConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->GMM_CFG = CSL_IPIPE_GMM_CFG_RESETVAL;
  }
  if ((CSL_ipipeSetRgb2Rgb2Config(hndl, data->rgb2Rgb2Config) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->RGB2_MUL_RR = CSL_IPIPE_RGB2_MUL_RR_RESETVAL;
    hndl->regs->RGB2_MUL_GR = CSL_IPIPE_RGB2_MUL_GR_RESETVAL;
    hndl->regs->RGB2_MUL_BR = CSL_IPIPE_RGB2_MUL_BR_RESETVAL;
    hndl->regs->RGB2_MUL_RG = CSL_IPIPE_RGB2_MUL_RG_RESETVAL;
    hndl->regs->RGB2_MUL_GG = CSL_IPIPE_RGB2_MUL_GG_RESETVAL;
    hndl->regs->RGB2_MUL_BG = CSL_IPIPE_RGB2_MUL_BG_RESETVAL;
    hndl->regs->RGB2_MUL_RB = CSL_IPIPE_RGB2_MUL_RB_RESETVAL;
    hndl->regs->RGB2_MUL_GB = CSL_IPIPE_RGB2_MUL_GB_RESETVAL;
    hndl->regs->RGB2_MUL_BB = CSL_IPIPE_RGB2_MUL_BB_RESETVAL;
    hndl->regs->RGB2_OFT_OR = CSL_IPIPE_RGB2_OFT_OR_RESETVAL;
    hndl->regs->RGB2_OFT_OG = CSL_IPIPE_RGB2_OFT_OG_RESETVAL;
    hndl->regs->RGB2_OFT_OB = CSL_IPIPE_RGB2_OFT_OB_RESETVAL;
  }
  if ((CSL_ipipeSet3dLutConfig(hndl, data->lut3DConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->D3LUT_EN = CSL_IPIPE_D3LUT_EN_RESETVAL;
  }
  if ((CSL_ipipeSetRgb2YuvConfig(hndl, data->rgb2YuvConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->YUV_MUL_RY = CSL_IPIPE_YUV_MUL_RY_RESETVAL;
    hndl->regs->YUV_MUL_GY = CSL_IPIPE_YUV_MUL_GY_RESETVAL;
    hndl->regs->YUV_MUL_BY = CSL_IPIPE_YUV_MUL_BY_RESETVAL;
    hndl->regs->YUV_MUL_RCB = CSL_IPIPE_YUV_MUL_RCB_RESETVAL;
    hndl->regs->YUV_MUL_GCB = CSL_IPIPE_YUV_MUL_GCB_RESETVAL;
    hndl->regs->YUV_MUL_BCB = CSL_IPIPE_YUV_MUL_BCB_RESETVAL;
    hndl->regs->YUV_MUL_RCR = CSL_IPIPE_YUV_MUL_RCR_RESETVAL;
    hndl->regs->YUV_MUL_GCR = CSL_IPIPE_YUV_MUL_GCR_RESETVAL;
    hndl->regs->YUV_MUL_BCR = CSL_IPIPE_YUV_MUL_BCR_RESETVAL;
    hndl->regs->YUV_OFT_Y = CSL_IPIPE_YUV_OFT_Y_RESETVAL;
    hndl->regs->YUV_OFT_CB = CSL_IPIPE_YUV_OFT_CB_RESETVAL;
    hndl->regs->YUV_OFT_CR = CSL_IPIPE_YUV_OFT_CR_RESETVAL;
    hndl->regs->YUV_PHS = CSL_IPIPE_YUV_PHS_RESETVAL;
  }
  if ((CSL_ipipeSetCntBrtConfig(hndl, data->cntBrtConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->YUV_ADJ = CSL_IPIPE_YUV_ADJ_RESETVAL;
  }
  if ((CSL_ipipeSetGbceConfig(hndl, data->gbceConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->GBCE_EN = CSL_IPIPE_GBCE_EN_RESETVAL;
    hndl->regs->GBCE_TYP = CSL_IPIPE_GBCE_TYP_RESETVAL;
  }
  if ((CSL_ipipeSetEdgeEnhanceConfig(hndl, data->edgeEnhanceConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->YEE_EN = CSL_IPIPE_YEE_EN_RESETVAL;
  }
  if ((CSL_ipipeSetChromaArtifactReduceConfig(hndl, data->chromaArtifactReduceConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->CAR_EN = CSL_IPIPE_CAR_EN_RESETVAL;
  }
  if ((CSL_ipipeSetChromaGainSupressConfig(hndl, data->chromaGainSupressConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->CGS_EN = CSL_IPIPE_CGS_EN_RESETVAL;
  }
  if ((CSL_ipipeSetBoxcarConfig(hndl, data->boxcarConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->BOX_EN = CSL_IPIPE_BOX_EN_RESETVAL;
  }
  if ((CSL_ipipeSetBscConfig(hndl, data->bscConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->BSC_EN = CSL_IPIPE_BSC_EN_RESETVAL;
  }
  if ((CSL_ipipeSetHistogramConfig(hndl, data->histogramConfig) != CSL_SOK) && (data->bypassModuleIfNullConfig == TRUE)) {
    hndl->regs->HST_EN = CSL_IPIPE_BSC_EN_RESETVAL;
  }

  hndl->regs->SRC_EN = data->enable;

  return CSL_SOK;
}
