/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_ipipe.h>
#include <csl_dma.h>


CSL_Status CSL_ipipeSetColPatConfig(CSL_IpipeHandle hndl, CSL_IpipeColPatConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    CSL_FINS(hndl->regs->SRC_COL, IPIPE_SRC_COL_OO, data->colPat[1][1]);
    CSL_FINS(hndl->regs->SRC_COL, IPIPE_SRC_COL_OE, data->colPat[1][0]);
    CSL_FINS(hndl->regs->SRC_COL, IPIPE_SRC_COL_EO, data->colPat[0][1]);
    CSL_FINS(hndl->regs->SRC_COL, IPIPE_SRC_COL_EE, data->colPat[0][0]);

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetClkConfig(CSL_IpipeHandle hndl, CSL_IpipeClkConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->GCK_MMR = data->mmrClkEnable;
    CSL_FINS(hndl->regs->GCK_PIX, IPIPE_GCK_PIX_G3, data->yeeCarClkEnable);
    CSL_FINS(hndl->regs->GCK_PIX, IPIPE_GCK_PIX_G2, data->cfa2yuvHistoBscClkEnable);
    CSL_FINS(hndl->regs->GCK_PIX, IPIPE_GCK_PIX_G1, data->dpc2WbClkEnable);
    CSL_FINS(hndl->regs->GCK_PIX, IPIPE_GCK_PIX_G0, data->boxcarClkEnable);

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetInFrameConfig(CSL_IpipeHandle hndl, CSL_IpipeInFrameConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->SRC_HPS = data->inStartX;
    hndl->regs->SRC_VPS = data->inStartY;
    hndl->regs->SRC_HSZ = data->inWidth - 1;
    hndl->regs->SRC_VSZ = data->inHeight - 1;

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetLscConfig(CSL_IpipeHandle hndl, CSL_IpipeLscConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->LSC_VOFT = data->vertOffset;
    hndl->regs->LSC_VA2 = data->vertQuadCoeff;
    hndl->regs->LSC_VA1 = data->vertLinCoeff;
    CSL_FINS(hndl->regs->LSC_VS, IPIPE_LSC_VS_VS1, data->vertQuadShiftLen);
    CSL_FINS(hndl->regs->LSC_VS, IPIPE_LSC_VS_VS2, data->vertLinShiftLen);

    hndl->regs->LSC_HOFT = data->horzOffset;
    hndl->regs->LSC_HA2 = data->horzQuadCoeff;
    hndl->regs->LSC_HA1 = data->horzLinCoeff;
    CSL_FINS(hndl->regs->LSC_HS, IPIPE_LSC_HS_HS1, data->horzQuadShiftLen);
    CSL_FINS(hndl->regs->LSC_HS, IPIPE_LSC_HS_HS2, data->horzLinShiftLen);

    hndl->regs->LSC_GAN_R = data->gain[0];
    hndl->regs->LSC_GAN_GR = data->gain[1];
    hndl->regs->LSC_GAN_GB = data->gain[2];
    hndl->regs->LSC_GAN_B = data->gain[3];

    hndl->regs->LSC_OFT_R = data->offset[0];
    hndl->regs->LSC_OFT_GR = data->offset[1];
    hndl->regs->LSC_OFT_GB = data->offset[2];
    hndl->regs->LSC_OFT_B = data->offset[3];

    hndl->regs->LSC_SHF = data->gainShiftLen;
    hndl->regs->LSC_MAX = data->gainMax;

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetDpcConfig(CSL_IpipeHandle hndl, CSL_IpipeDpcConfig * data)
{
    Uint32 i;
    volatile Uint32 *tableMemAddr[2];  

    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    if (data->lutNumEntries>CSL_IPIPE_DPC_LUT_MAX_ENTRIES)
        data->lutNumEntries = CSL_IPIPE_DPC_LUT_MAX_ENTRIES;

    CSL_FINS(hndl->regs->DPC_LUT_SEL, IPIPE_DPC_LUT_SEL_TBL, data->lutType);
    CSL_FINS(hndl->regs->DPC_LUT_SEL, IPIPE_DPC_LUT_SEL_SEL, data->lutOption0CorMethod);
    hndl->regs->DPC_LUT_ADR = data->lutStartAddr;
    hndl->regs->DPC_LUT_SIZ = data->lutNumEntries - 1;

    if (data->lutAddr)
    {

        tableMemAddr[0] = (Uint32*)CSL_IPIPE_DPC_TBL_0_ADDR;
        tableMemAddr[1] = (Uint32*)CSL_IPIPE_DPC_TBL_1_ADDR;    

        for (i=0; i<data->lutNumEntries; i+=2)
        {
            *tableMemAddr[0]++ = data->lutAddr[i];
            *tableMemAddr[1]++ = data->lutAddr[i+1];
        }
    }

    hndl->regs->DPC_OTF_EN = data->otfEnable;
    CSL_FINS(hndl->regs->DPC_OTF_TYP, IPIPE_DPC_OTF_TYP_TYP, data->otfType);
    CSL_FINS(hndl->regs->DPC_OTF_TYP, IPIPE_DPC_OTF_TYP_ALG, data->otfAlg);

    hndl->regs->DPC_OTF_2_D_THR_R = data->otf2DetThres[0];
    hndl->regs->DPC_OTF_2_D_THR_GR = data->otf2DetThres[1];
    hndl->regs->DPC_OTF_2_D_THR_GB = data->otf2DetThres[2];
    hndl->regs->DPC_OTF_2_D_THR_B = data->otf2DetThres[3];

    hndl->regs->DPC_OTF_2_C_THR_R = data->otf2CorThres[0];
    hndl->regs->DPC_OTF_2_C_THR_GR = data->otf2CorThres[1];
    hndl->regs->DPC_OTF_2_C_THR_GB = data->otf2CorThres[2];
    hndl->regs->DPC_OTF_2_C_THR_B = data->otf2CorThres[3];

    hndl->regs->DPC_OTF_3_SHF = data->otf3ActAdjust;
    hndl->regs->DPC_OTF_3_D_THR = data->otf3DetThres;
    hndl->regs->DPC_OTF_3_D_SLP = data->otf3DetThresSlope;
    hndl->regs->DPC_OTF_3_D_MIN = data->otf3DetThresMin;
    hndl->regs->DPC_OTF_3_D_MAX = data->otf3DetThresMax;
    hndl->regs->DPC_OTF_3_C_THR = data->otf3CorThres;
    hndl->regs->DPC_OTF_3_C_SLP = data->otf3CorThresSlope;
    hndl->regs->DPC_OTF_3_C_MIN = data->otf3CorThresMin;
    hndl->regs->DPC_OTF_3_C_MAX = data->otf3CorThresMax;

    hndl->regs->DPC_LUT_EN = data->lutEnable;

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetNf1Config(CSL_IpipeHandle hndl, CSL_IpipeNfConfig * data)
{
    Uint32 i;

    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    CSL_FINS(hndl->regs->D2F_1ST_TYP, IPIPE_D2F_1ST_TYP_SPR, data->spreadVal);
    CSL_FINS(hndl->regs->D2F_1ST_TYP, IPIPE_D2F_1ST_TYP_SEL, data->spreadValSrc);
    CSL_FINS(hndl->regs->D2F_1ST_TYP, IPIPE_D2F_1ST_TYP_SHF, data->lutAddrShift);
    CSL_FINS(hndl->regs->D2F_1ST_TYP, IPIPE_D2F_1ST_TYP_TYP, data->greenSampleMethod);
    CSL_FINS(hndl->regs->D2F_1ST_TYP, IPIPE_D2F_1ST_TYP_LSC, data->lscGainEnable);

    hndl->regs->D2F_1ST_EDG_MIN = data->edgeDetectThresMin;
    hndl->regs->D2F_1ST_EDG_MAX = data->edgeDetectThresMax;

    for (i=0; i<8; i++)
    {
        hndl->regs->D2F_1ST_THR[i] = (Uint32) data->lutThresTable[i];
        hndl->regs->D2F_1ST_STR[i] = (Uint32) data->lutIntensityTable[i];
        hndl->regs->D2F_1ST_SPR[i] = (Uint32) data->lutSpreadTable[i];
    }

    hndl->regs->D2F_1ST_EN = data->enable;

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetNf2Config(CSL_IpipeHandle hndl, CSL_IpipeNfConfig * data)
{
    Uint32 i;

    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    CSL_FINS(hndl->regs->D2F_2ND_TYP, IPIPE_D2F_2ND_TYP_SPR, data->spreadVal);
    CSL_FINS(hndl->regs->D2F_2ND_TYP, IPIPE_D2F_2ND_TYP_SEL, data->spreadValSrc);
    CSL_FINS(hndl->regs->D2F_2ND_TYP, IPIPE_D2F_2ND_TYP_SHF, data->lutAddrShift);
    CSL_FINS(hndl->regs->D2F_2ND_TYP, IPIPE_D2F_2ND_TYP_TYP, data->greenSampleMethod);
    CSL_FINS(hndl->regs->D2F_2ND_TYP, IPIPE_D2F_2ND_TYP_LSC, data->lscGainEnable);

    hndl->regs->D2F_2ND_EDG_MIN = data->edgeDetectThresMin;
    hndl->regs->D2F_2ND_EDG_MAX = data->edgeDetectThresMax;

    for (i=0; i<8; i++)
    {
        hndl->regs->D2F_2ND_THR[i] = (Uint32) data->lutThresTable[i];
        hndl->regs->D2F_2ND_STR[i] = (Uint32) data->lutIntensityTable[i];
        hndl->regs->D2F_2ND_SPR[i] = (Uint32) data->lutSpreadTable[i];
    }

    hndl->regs->D2F_2ND_EN = data->enable;

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetGicConfig(CSL_IpipeHandle hndl, CSL_IpipeGicConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    CSL_FINS(hndl->regs->GIC_TYP, IPIPE_GIC_TYP_TYP, data->type);
    CSL_FINS(hndl->regs->GIC_TYP, IPIPE_GIC_TYP_SEL, data->thresSel);
    CSL_FINS(hndl->regs->GIC_TYP, IPIPE_GIC_TYP_LSC, data->lscGainEnable);

    hndl->regs->GIC_GAN = data->gain;
    hndl->regs->GIC_NFGAN = data->nf2ThresGainScale;
    hndl->regs->GIC_THR = data->thresVal;
    hndl->regs->GIC_SLP = data->thresSlope;

    hndl->regs->GIC_EN = data->enable;

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetWbConfig(CSL_IpipeHandle hndl, CSL_IpipeWbConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->WB2_OFT_R = data->offset[0];
    hndl->regs->WB2_OFT_GR = data->offset[1];
    hndl->regs->WB2_OFT_GB = data->offset[2];
    hndl->regs->WB2_OFT_B = data->offset[3];

    hndl->regs->WB2_WGN_R = data->gain[0];
    hndl->regs->WB2_WGN_GR = data->gain[1];
    hndl->regs->WB2_WGN_GB = data->gain[2];
    hndl->regs->WB2_WGN_B = data->gain[3];

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetCfaConfig(CSL_IpipeHandle hndl, CSL_IpipeCfaConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->CFA_MODE = data->mode;

    hndl->regs->CFA_2DIR_HPF_THR = data->twoDirHpValLowThres;
    hndl->regs->CFA_2DIR_HPF_SLP = data->twoDirHpValSlope;
    hndl->regs->CFA_2DIR_MIX_THR = data->twoDirHpMixThres;
    hndl->regs->CFA_2DIR_MIX_SLP = data->twoDirHpMixSlope;
    hndl->regs->CFA_2DIR_DIR_THR = data->twoDirDirThres;
    hndl->regs->CFA_2DIR_DIR_SLP = data->twoDirDirSlope;
    hndl->regs->CFA_2DIR_NDWT = data->twoDirNonDirWeight;

    hndl->regs->CFA_MONO_HUE_FRA = data->daaHueFrac;
    hndl->regs->CFA_MONO_EDG_THR = data->daaEdgeThres;
    hndl->regs->CFA_MONO_THR_MIN = data->daaThresMin;
    hndl->regs->CFA_MONO_THR_SLP = data->daaThresSlope;
    hndl->regs->CFA_MONO_SLP_MIN = data->daaSlopeMin;
    hndl->regs->CFA_MONO_SLP_SLP = data->daaSlopeSlope;
    hndl->regs->CFA_MONO_LPWT = data->daaLpWeight;

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetRgb2Rgb1Config(CSL_IpipeHandle hndl, CSL_IpipeRgb2RgbConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->RGB1_MUL_RR = data->matrix[0][0];
    hndl->regs->RGB1_MUL_GR = data->matrix[0][1];
    hndl->regs->RGB1_MUL_BR = data->matrix[0][2];
    hndl->regs->RGB1_MUL_RG = data->matrix[1][0];
    hndl->regs->RGB1_MUL_GG = data->matrix[1][1];
    hndl->regs->RGB1_MUL_BG = data->matrix[1][2];
    hndl->regs->RGB1_MUL_RB = data->matrix[2][0];
    hndl->regs->RGB1_MUL_GB = data->matrix[2][1];
    hndl->regs->RGB1_MUL_BB = data->matrix[2][2];

    hndl->regs->RGB1_OFT_OR = data->offset[0];
    hndl->regs->RGB1_OFT_OG = data->offset[1];
    hndl->regs->RGB1_OFT_OB = data->offset[2];

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetGammaConfig(CSL_IpipeHandle hndl, CSL_IpipeGammaConfig * data)
{
    Uint32 i, tableSize;
    volatile Uint32 *tableMemAddr;

    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    CSL_FINS(hndl->regs->GMM_CFG, IPIPE_GMM_CFG_SIZ, data->tableSize);
    CSL_FINS(hndl->regs->GMM_CFG, IPIPE_GMM_CFG_TBL, data->tableSrc);
    CSL_FINS(hndl->regs->GMM_CFG, IPIPE_GMM_CFG_BYPR, data->bypassR);
    CSL_FINS(hndl->regs->GMM_CFG, IPIPE_GMM_CFG_BYPG, data->bypassG);
    CSL_FINS(hndl->regs->GMM_CFG, IPIPE_GMM_CFG_BYPB, data->bypassB);

    switch (data->tableSize)
    {
    case CSL_IPIPE_GAMMA_CORRECTION_TABLE_SIZE_64:
        tableSize = 64;
        break;
    case CSL_IPIPE_GAMMA_CORRECTION_TABLE_SIZE_128:
        tableSize = 128;
        break;
    case CSL_IPIPE_GAMMA_CORRECTION_TABLE_SIZE_256:
        tableSize = 256;
        break;
    case CSL_IPIPE_GAMMA_CORRECTION_TABLE_SIZE_512:
        tableSize = 512;
        break;
    default:
        return CSL_EFAIL;
    }

    if (data->tableR)
    {

        tableMemAddr = (Uint32*)CSL_IPIPE_GAMMA_R_TBL_3_ADDR;

        for (i=0; i<tableSize; i++)
        {
            *tableMemAddr++ = data->tableR[i];
        }
    }

    if (data->tableG)
    {
        tableMemAddr = (Uint32*)CSL_IPIPE_GAMMA_G_TBL_3_ADDR;

        for (i=0; i<tableSize; i++)
        {
            *tableMemAddr++ = data->tableG[i];
        }
    }

    if (data->tableB)
    {
        tableMemAddr = (Uint32*)CSL_IPIPE_GAMMA_B_TBL_3_ADDR;

        for (i=0; i<tableSize; i++)
        {
            *tableMemAddr++ = data->tableB[i];
        }
    }

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetRgb2Rgb2Config(CSL_IpipeHandle hndl, CSL_IpipeRgb2RgbConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->RGB2_MUL_RR = data->matrix[0][0];
    hndl->regs->RGB2_MUL_GR = data->matrix[0][1];
    hndl->regs->RGB2_MUL_BR = data->matrix[0][2];
    hndl->regs->RGB2_MUL_RG = data->matrix[1][0];
    hndl->regs->RGB2_MUL_GG = data->matrix[1][1];
    hndl->regs->RGB2_MUL_BG = data->matrix[1][2];
    hndl->regs->RGB2_MUL_RB = data->matrix[2][0];
    hndl->regs->RGB2_MUL_GB = data->matrix[2][1];
    hndl->regs->RGB2_MUL_BB = data->matrix[2][2];

    hndl->regs->RGB2_OFT_OR = data->offset[0];
    hndl->regs->RGB2_OFT_OG = data->offset[1];
    hndl->regs->RGB2_OFT_OB = data->offset[2];

    return CSL_SOK;
}

CSL_Status CSL_ipipeSet3dLutConfig(CSL_IpipeHandle hndl, CSL_Ipipe3dLutConfig * data)
{
    Uint32 i;
    volatile Uint32 *tableMemAddr[4];

    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    if (data->table)
    {
        tableMemAddr[0] = (Uint32*)CSL_IPIPE_3DLUT_TBL_0_ADDR;
        tableMemAddr[1] = (Uint32*)CSL_IPIPE_3DLUT_TBL_1_ADDR;
        tableMemAddr[2] = (Uint32*)CSL_IPIPE_3DLUT_TBL_2_ADDR;
        tableMemAddr[3] = (Uint32*)CSL_IPIPE_3DLUT_TBL_3_ADDR;

        for ( i=0; i< (CSL_IPIPE_3D_LUT_MAX_ENTRIES) ; i+=4)
        {
            *tableMemAddr[0]++ = data->table[i];
            *tableMemAddr[1]++ = data->table[i+1];
            *tableMemAddr[2]++ = data->table[i+2];
            *tableMemAddr[3]++ = data->table[i+3];                  
        }
    }

    hndl->regs->D3LUT_EN = data->enable;

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetRgb2YuvConfig(CSL_IpipeHandle hndl, CSL_IpipeRgb2YuvConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->YUV_MUL_RY = data->matrix[0][0];
    hndl->regs->YUV_MUL_GY = data->matrix[0][1];
    hndl->regs->YUV_MUL_BY = data->matrix[0][2];
    hndl->regs->YUV_MUL_RCB = data->matrix[1][0];
    hndl->regs->YUV_MUL_GCB = data->matrix[1][1];
    hndl->regs->YUV_MUL_BCB = data->matrix[1][2];
    hndl->regs->YUV_MUL_RCR = data->matrix[2][0];
    hndl->regs->YUV_MUL_GCR = data->matrix[2][1];
    hndl->regs->YUV_MUL_BCR = data->matrix[2][2];

    hndl->regs->YUV_OFT_Y = data->offset[0];
    hndl->regs->YUV_OFT_CB = data->offset[1];
    hndl->regs->YUV_OFT_CR = data->offset[2];

    CSL_FINS(hndl->regs->YUV_PHS, IPIPE_YUV_PHS_LPF, data->cLpfEnable);
    CSL_FINS(hndl->regs->YUV_PHS, IPIPE_YUV_PHS_POS, data->cPos);

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetCntBrtConfig(CSL_IpipeHandle hndl, CSL_IpipeCntBrtConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    CSL_FINS(hndl->regs->YUV_ADJ, IPIPE_YUV_ADJ_BRT, data->brightness);
    CSL_FINS(hndl->regs->YUV_ADJ, IPIPE_YUV_ADJ_CTR, data->contrast);

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetGbceConfig(CSL_IpipeHandle hndl, CSL_IpipeGbceConfig * data)
{
    Uint32 i, val32;
    volatile Uint32 *tableMemAddr;

    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->GBCE_TYP = data->type;

    if (data->gainTable)
    {
        tableMemAddr = (Uint32*)CSL_IPIPE_GBCE_TBL_ADDR;

        for (i=0; i<(CSL_IPIPE_GBCE_GAIN_TABLE_MAX_ENTRIES); i+=2)
        {
            val32 = ( data->gainTable[i] ) | ( data->gainTable[i+1] << 10 );

            *tableMemAddr++ = val32;
        }
    }

    hndl->regs->GBCE_EN = data->enable;

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetEdgeEnhanceConfig(CSL_IpipeHandle hndl, CSL_IpipeEdgeEnhanceConfig * data)
{
    Uint32 i, val32;
    volatile Uint32 *tableMemAddr;

    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    CSL_FINS(hndl->regs->YEE_TYP, IPIPE_YEE_TYP_HAL, data->haloReduceEnable);
    CSL_FINS(hndl->regs->YEE_TYP, IPIPE_YEE_TYP_SEL, data->mergeMethod);

    hndl->regs->YEE_SHF    = data->hpfShift;
    hndl->regs->YEE_MUL_00 = data->hpfCoeff[0][0];
    hndl->regs->YEE_MUL_01 = data->hpfCoeff[0][1];
    hndl->regs->YEE_MUL_02 = data->hpfCoeff[0][2];
    hndl->regs->YEE_MUL_10 = data->hpfCoeff[1][0];
    hndl->regs->YEE_MUL_11 = data->hpfCoeff[1][1];
    hndl->regs->YEE_MUL_12 = data->hpfCoeff[1][2];
    hndl->regs->YEE_MUL_20 = data->hpfCoeff[2][0];
    hndl->regs->YEE_MUL_21 = data->hpfCoeff[2][1];
    hndl->regs->YEE_MUL_22 = data->hpfCoeff[2][2];

    hndl->regs->YEE_THR   = data->lowerThres;

    hndl->regs->YEE_E_GAN   = data->edgeSharpGain;
    hndl->regs->YEE_E_THR_1 = data->edgeSharpHpValLowThres;
    hndl->regs->YEE_E_THR_2 = data->edgeSharpHpValUpLimit;
    hndl->regs->YEE_G_GAN   = data->edgeSharpGainGradient;
    hndl->regs->YEE_G_OFT   = data->edgeSharpOffsetGradient;

    if (data->table)
    {
        tableMemAddr = (Uint32*)CSL_IPIPE_YEE_TBL_ADDR;

        for (i=0; i<(CSL_IPIPE_EDGE_ENHANCE_TABLE_MAX_ENTRIES); i+=2)
        {
            val32 = ( data->table[i] ) | (data->table[i+1] << 9);

            *tableMemAddr++ = val32;
        }
    }

    hndl->regs->YEE_EN = data->enable;

    return CSL_SOK;
}


CSL_Status CSL_ipipeSetChromaArtifactReduceConfig(CSL_IpipeHandle hndl, CSL_IpipeChromaArtifactReduceConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->CAR_TYP = data->type;

    CSL_FINS(hndl->regs->CAR_SW, IPIPE_CAR_SW_SW0, data->sw0MedFiltThres);
    CSL_FINS(hndl->regs->CAR_SW, IPIPE_CAR_SW_SW1, data->sw1GainCtrlThres);

    hndl->regs->CAR_HPF_TYP = data->hpfType;
    hndl->regs->CAR_HPF_SHF = data->hpfShift;
    hndl->regs->CAR_HPF_THR = data->hpfValThres;

    hndl->regs->CAR_GN1_GAN = data->gain1Intensity;
    hndl->regs->CAR_GN1_SHF = data->gain1Shift;
    hndl->regs->CAR_GN1_MIN = data->gain1Min;

    hndl->regs->CAR_GN2_GAN = data->gain2Intensity;
    hndl->regs->CAR_GN2_SHF = data->gain2Shift;
    hndl->regs->CAR_GN2_MIN = data->gain2Min;

    hndl->regs->CAR_EN = data->enable;

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetChromaGainSupressConfig(CSL_IpipeHandle hndl, CSL_IpipeChromaGainSupressConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->CGS_EN = data->enable;

    hndl->regs->CGS_GN1_H_THR = data->gain1HThres;
    hndl->regs->CGS_GN1_H_GAN = data->gain1HSlope;
    hndl->regs->CGS_GN1_H_SHF = data->gain1HShift;
    hndl->regs->CGS_GN1_H_MIN = data->gain1HMin;

    hndl->regs->CGS_GN1_L_THR = data->gain1LThres;
    hndl->regs->CGS_GN1_L_GAN = data->gain1LSlope;
    hndl->regs->CGS_GN1_L_SHF = data->gain1LShift;
    hndl->regs->CGS_GN1_L_MIN = data->gain1LMin;

    hndl->regs->CGS_GN2_L_THR = data->gain2LThres;
    hndl->regs->CGS_GN2_L_GAN = data->gain2LSlope;
    hndl->regs->CGS_GN2_L_SHF = data->gain2LShift;
    hndl->regs->CGS_GN2_L_MIN = data->gain2LMin;

    return CSL_SOK;
}

CSL_Status CSL_ipipeEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
    if (hndl == NULL)
        return CSL_EFAIL;

    hndl->regs->SRC_EN = enable;

    return CSL_SOK;
}

CSL_Status CSL_ipipeOneShotEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
    if (hndl == NULL)
        return CSL_EFAIL;

    CSL_FINS(hndl->regs->SRC_MODE, IPIPE_SRC_MODE_OST, enable);

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetDataPath(CSL_IpipeHandle hndl, Uint8 dataPath)
{
    if (hndl == NULL)
        return CSL_EFAIL;

    hndl->regs->SRC_FMT = dataPath;

    return CSL_SOK;
}

CSL_Status CSL_ipipeSetBoxcarConfig(CSL_IpipeHandle hndl, CSL_IpipeBoxcarConfig * data)
{
    CSL_Status status = CSL_SOK;

    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->BOX_MODE = data->oneShotEnable;
    hndl->regs->BOX_TYP = data->blockSize;
    hndl->regs->BOX_SHF = data->inShift;

    status |= CSL_ipipeBoxcarSetOutAddr(hndl, data->outAddr);
    status |= CSL_ipipeBoxcarEnable(hndl, data->enable);

    return status;
}

CSL_Status CSL_ipipeBoxcarEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
    if (hndl == NULL)
        return CSL_EFAIL;

    hndl->regs->BOX_EN = enable;

    return CSL_SOK;
}

CSL_Status CSL_ipipeBoxcarSetOutAddr(CSL_IpipeHandle hndl, Uint8 *outAddr)
{
    CSL_Status status = CSL_SOK;
    Uint32 addr;

    if (hndl == NULL || outAddr == NULL)
        return CSL_EFAIL;

    addr = (Uint32) outAddr;

    if (addr % 32)
        status = CSL_EFAIL;

    hndl->regs->BOX_SDR_SAD_L = (Uint16) addr;
    hndl->regs->BOX_SDR_SAD_H = (Uint16) (addr >> 16);

    return status;    
}

CSL_Status CSL_ipipeSetBscConfig(CSL_IpipeHandle hndl, CSL_IpipeBscConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->BSC_MODE = data->oneShotEnable;

    CSL_FINS(hndl->regs->BSC_TYP, IPIPE_BSC_TYP_COL, data->colorSelect);

    CSL_FINS(hndl->regs->BSC_TYP, IPIPE_BSC_TYP_REN, data->rowEnable);
    hndl->regs->BSC_ROW_VCT = data->rowNumVectors - 1;
    hndl->regs->BSC_ROW_SHF = data->rowInShift;
    hndl->regs->BSC_ROW_VPOS = data->rowStartV;
    hndl->regs->BSC_ROW_VNUM = data->rowNumV - 1;
    hndl->regs->BSC_ROW_VSKIP = data->rowSkipV - 1;
    hndl->regs->BSC_ROW_HPOS = data->rowStartH;
    hndl->regs->BSC_ROW_HNUM = data->rowNumH - 1;
    hndl->regs->BSC_ROW_HSKIP = data->rowSkipH - 1;

    CSL_FINS(hndl->regs->BSC_TYP, IPIPE_BSC_TYP_CEN, data->colEnable);
    hndl->regs->BSC_COL_VCT = data->colNumVectors - 1;
    hndl->regs->BSC_COL_SHF = data->colInShift;
    hndl->regs->BSC_COL_VPOS = data->colStartV;
    hndl->regs->BSC_COL_VNUM = data->colNumV - 1;
    hndl->regs->BSC_COL_VSKIP = data->colSkipV - 1;
    hndl->regs->BSC_COL_HPOS = data->colStartH;
    hndl->regs->BSC_COL_HNUM = data->colNumH - 1;
    hndl->regs->BSC_COL_HSKIP = data->colSkipH - 1;

    hndl->regs->BSC_EN = data->enable;

    return CSL_SOK;
}

CSL_Status CSL_ipipeBscEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
    if (hndl == NULL)
        return CSL_EFAIL;

    if (!enable)
        CSL_dmaStop(CSL_DMA_IPIPE_BSC);

    hndl->regs->BSC_EN = enable;

    return CSL_SOK;
}


CSL_Status CSL_ipipeBscReadResults(CSL_IpipeHandle hndl, CSL_IpipeBscResults *data)
{
    Uint32 i, idx, val32;
    volatile Uint32 *tableMemAddr;

    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    if (data->rowSumTable==NULL || data->colSumTable==NULL)
        return CSL_EFAIL;

    data->rowNumV =  (hndl->regs->BSC_ROW_VNUM+1);
    data->rowNumVectors = (hndl->regs->BSC_ROW_VCT+1);

    tableMemAddr = (Uint32*)CSL_IPIPE_BSC_TBL_0_ADDR;

    idx=0;  

    for (i=0; i<(data->rowNumVectors*data->rowNumV)/2; i++)
    {
        val32 = *tableMemAddr++;
        data->rowSumTable[idx]   = (Uint16)(val32);
        data->rowSumTable[idx+1] = (Uint16)(val32>>16);      
        idx+=2;
    }

    data->colNumH =  (hndl->regs->BSC_COL_HNUM+1);
    data->colNumVectors = (hndl->regs->BSC_COL_VCT+1);

    tableMemAddr = (Uint32*)CSL_IPIPE_BSC_TBL_1_ADDR;

    idx=0;  

    for (i=0; i<(data->colNumVectors*data->colNumH)/2; i++)
    {
        val32 = *tableMemAddr++;
        data->colSumTable[idx]   = (Uint16)(val32);
        data->colSumTable[idx+1] = (Uint16)(val32>>16);      
        idx+=2;
    }


    return CSL_SOK;
}

CSL_Status CSL_ipipeSetHistogramConfig(CSL_IpipeHandle hndl, CSL_IpipeHistogramConfig * data)
{
    if (hndl == NULL || data == NULL)
        return CSL_EFAIL;

    hndl->regs->HST_MODE = data->oneShotEnable;

    CSL_FINS(hndl->regs->HST_SEL, IPIPE_HST_SEL_SEL, data->source);
    CSL_FINS(hndl->regs->HST_SEL, IPIPE_HST_SEL_TYP, data->greenType);
#if 1 //Gang: bug fix for setup BIN register
    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_BIN, data->numBins);
#endif

    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_SHF, data->inShift);

    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_RGN0, data->regionEnable[0]);
    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_RGN1, data->regionEnable[1]);
    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_RGN2, data->regionEnable[2]);
    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_RGN3, data->regionEnable[3]);

    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_COL0, data->colorEnable[0]);
    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_COL1, data->colorEnable[1]);
    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_COL2, data->colorEnable[2]);
    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_COL3, data->colorEnable[3]);

    hndl->regs->HST_MUL_R = data->matrixCoeff[0];
    hndl->regs->HST_MUL_GR = data->matrixCoeff[1];
    hndl->regs->HST_MUL_GB = data->matrixCoeff[2];
    hndl->regs->HST_MUL_B = data->matrixCoeff[3];

    hndl->regs->HST_0_HPS = data->regionConfig[0].startX;
    hndl->regs->HST_0_VPS = data->regionConfig[0].startY;
    hndl->regs->HST_0_HSZ = data->regionConfig[0].width - 1;
    hndl->regs->HST_0_VSZ = data->regionConfig[0].height - 1;

    hndl->regs->HST_1_HPS = data->regionConfig[1].startX;
    hndl->regs->HST_1_VPS = data->regionConfig[1].startY;
    hndl->regs->HST_1_HSZ = data->regionConfig[1].width - 1;
    hndl->regs->HST_1_VSZ = data->regionConfig[1].height - 1;

    hndl->regs->HST_2_HPS = data->regionConfig[2].startX;
    hndl->regs->HST_2_VPS = data->regionConfig[2].startY;
    hndl->regs->HST_2_HSZ = data->regionConfig[2].width - 1;
    hndl->regs->HST_2_VSZ = data->regionConfig[2].height - 1;

    hndl->regs->HST_3_HPS = data->regionConfig[3].startX;
    hndl->regs->HST_3_VPS = data->regionConfig[3].startY;
    hndl->regs->HST_3_HSZ = data->regionConfig[3].width - 1;
    hndl->regs->HST_3_VSZ = data->regionConfig[3].height - 1;

    CSL_FINS(hndl->regs->HST_TBL, IPIPE_HST_TBL_SEL, data->outTableSel);
    CSL_FINS(hndl->regs->HST_TBL, IPIPE_HST_TBL_CLR, data->clearOnVd);

    hndl->regs->HST_EN = data->enable;

    return CSL_SOK;
}

CSL_Status CSL_ipipeHistogramEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
    if (hndl == NULL)
        return CSL_EFAIL;

    hndl->regs->HST_EN = enable;

    return CSL_SOK;
}

CSL_Status CSL_ipipeHistogramRegionEnable(CSL_IpipeHandle hndl, Bool32 * enable)
{
    if (hndl == NULL || enable == NULL)
        return CSL_EFAIL;

    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_RGN0, enable[0]);
    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_RGN1, enable[1]);
    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_RGN2, enable[2]);
    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_RGN3, enable[3]);

    return CSL_SOK;
}

CSL_Status CSL_ipipeHistogramTableSelect(CSL_IpipeHandle hndl, Uint32 tableID)
{
    if (hndl == NULL)
        return CSL_EFAIL;

    CSL_FINS(hndl->regs->HST_TBL, IPIPE_HST_TBL_SEL, tableID);    

    return CSL_SOK;  
}

#if 1 //Gang: Histogram Table Switch
CSL_Status CSL_ipipeHistogramTableSwitch(CSL_IpipeHandle hndl)
{
    Uint32 tableID;
    if (hndl == NULL)
        return CSL_EFAIL;

    tableID = CSL_FEXT(hndl->regs->HST_TBL, IPIPE_HST_TBL_SEL);
    tableID = 1 - tableID;
    CSL_FINS(hndl->regs->HST_TBL, IPIPE_HST_TBL_SEL, tableID);    

    return CSL_SOK;  
}

CSL_Status CSL_ipipeHistogramReadIdleY(CSL_IpipeHandle hndl, Uint32 * table)
{
    Uint32 tableID, i;
    volatile Uint32 *tableMemAddr;

    if (hndl == NULL || table == NULL)
        return CSL_EFAIL;

    tableID = CSL_FEXT(hndl->regs->HST_TBL, IPIPE_HST_TBL_SEL);
    if (tableID==CSL_IPIPE_HISTOGRAM_OUT_TABLE_2_3)
    {
        tableMemAddr = (Uint32*)CSL_IPIPE_HST_TBL_1_ADDR;    
    } else
    {
        tableMemAddr = (Uint32*)CSL_IPIPE_HST_TBL_3_ADDR;    
    }

    for (i=0; i<256; i++)
    {
        table[i] = tableMemAddr[256+i];
    }

    return CSL_SOK;
}
#endif

CSL_Status CSL_ipipeHistogramColorEnable(CSL_IpipeHandle hndl, Bool32 * enable)
{
    if (hndl == NULL || enable == NULL)
        return CSL_EFAIL;

    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_COL0, enable[0]);
    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_COL1, enable[1]);
    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_COL2, enable[2]);
    CSL_FINS(hndl->regs->HST_PARA, IPIPE_HST_PARA_COL3, enable[3]);

    return CSL_SOK;
}

CSL_Status CSL_ipipeHistogramReadResults(CSL_IpipeHandle hndl, Uint32 * table, Uint32 offset, Uint32 size)  //ANR-Hist
{
    Uint32 tableID, i =0;
    volatile Uint32 *tableMemAddr[2];

    if (hndl == NULL || table == NULL)
        return CSL_EFAIL;

    tableID = CSL_FEXT(hndl->regs->HST_TBL, IPIPE_HST_TBL_SEL);
    if (tableID==CSL_IPIPE_HISTOGRAM_OUT_TABLE_0_1)
    {
        tableMemAddr[0] = (Uint32*)CSL_IPIPE_HST_TBL_0_ADDR;
        tableMemAddr[1] = (Uint32*)CSL_IPIPE_HST_TBL_1_ADDR;    
    } else
    {
        tableMemAddr[0] = (Uint32*)CSL_IPIPE_HST_TBL_2_ADDR;
        tableMemAddr[1] = (Uint32*)CSL_IPIPE_HST_TBL_3_ADDR;    
    }

    //ANR - Copy only the required data.
    if ( size != 0 )
    {

        if ( offset <= (512*4))
        {

            tableMemAddr[0] = (tableMemAddr[0] + offset ); // * sizeof(Uint32*)

            for (i=0; i<CSL_IPIPE_HISTOGRAM_RESULTS_MAX_ENTRIES/2, i < size; i++)
            {
                *table++ = *tableMemAddr[0]++;
            }
            offset = 0;
        }

        if ( offset > (512*4) || i < size )
        {

            tableMemAddr[1] = (tableMemAddr[1] + offset); 

            for ( ; i<CSL_IPIPE_HISTOGRAM_RESULTS_MAX_ENTRIES/2, i < size; i++)
            {
                *table++ = *tableMemAddr[1]++;
            }      
        }

    } else
    {

        for (i=0; i<CSL_IPIPE_HISTOGRAM_RESULTS_MAX_ENTRIES/2; i++)
        {
            *table++ = *tableMemAddr[0]++;
        }

        for (i=0; i<CSL_IPIPE_HISTOGRAM_RESULTS_MAX_ENTRIES/2; i++)
        {
            *table++ = *tableMemAddr[1]++;
        }
    }

    return CSL_SOK;
}

