
#include <drv_h3a.h>
#include <osa_cmem.h>

typedef struct
{
    DRV_H3aConfig config;
    DRV_H3aInfo   info;

    OSA_BufInfo   afBufInfo[CSL_BUF_NUM_MAX];
    OSA_BufInfo   aewbBufInfo[CSL_BUF_NUM_MAX];

    CSL_H3aAewbConfig    aewbConfig;
    CSL_H3aAfConfig      afConfig;
    CSL_H3aInFrameConfig inFrameConfig;

    DRV_ImgsModeConfig imgsModeConfig;
    DRV_ImgsH3aConfig  *pImgsH3aConfig;
} DRV_H3aObj;


DRV_H3aObj gDRV_h3aObj;

const int iircoeff_0[11] = {8, -95, -49, 38, 76, 38, -111, -54, 17, -34, 17};
const int iircoeff_1[11] = {11, -72, -50, 26, 51, 26, -92, -53, 19, -38, 19};

int DRV_h3aOpen(DRV_H3aConfig *config)
{
    int afBufSize, aewbBufSize, i, status;

    CSL_h3aAfEnable(&gCSL_h3aHndl, FALSE);
    CSL_h3aAewbEnable(&gCSL_h3aHndl, FALSE);

    memset(&gDRV_h3aObj, 0, sizeof(gDRV_h3aObj));

    if (config->numBufAewb > CSL_BUF_NUM_MAX ||
        config->numBufAf   > CSL_BUF_NUM_MAX)
    {
        OSA_ERROR("exceeded max buf limit of %d\n", CSL_BUF_NUM_MAX);
        return OSA_EFAIL;
    }

    gDRV_h3aObj.pImgsH3aConfig = drvImgsFunc->imgsGetH3aConfig(config->sensorMode, config->aewbVendor);
    if (gDRV_h3aObj.pImgsH3aConfig == NULL)
    {
        OSA_ERROR("DRV_imgsGetH3aConfig(%d , %d)\n", config->sensorMode, config->aewbVendor);
        return OSA_EFAIL;
    }

    /*AF DRIVER CHANGE*/
    gDRV_h3aObj.pImgsH3aConfig->medFiltThreshold  =  0;
    gDRV_h3aObj.pImgsH3aConfig->aewbMedFiltEnable = FALSE;
    gDRV_h3aObj.pImgsH3aConfig->afVfEnable        = FALSE;
    gDRV_h3aObj.pImgsH3aConfig->afMedFiltEnable   = FALSE;
    gDRV_h3aObj.pImgsH3aConfig->afRgbPos          = 1;
    gDRV_h3aObj.pImgsH3aConfig->afFvAccMode       = 1;
    gDRV_h3aObj.pImgsH3aConfig->afPaxStartX       = 32;
    gDRV_h3aObj.pImgsH3aConfig->afPaxStartY       = 32;
    gDRV_h3aObj.pImgsH3aConfig->afPaxNumH         = 6;
    gDRV_h3aObj.pImgsH3aConfig->afPaxNumV         = 16;
    for (i = 0; i < 11; i++)
    {
        gDRV_h3aObj.pImgsH3aConfig->afIirCoeff0[i] = iircoeff_0[i];
    }
    for (i = 0;i < 11; i++)
    {
        gDRV_h3aObj.pImgsH3aConfig->afIirCoeff1[i] = iircoeff_1[i];
    }
    for (i = 0; i < 5; i++)
    {
        gDRV_h3aObj.pImgsH3aConfig->afVfvFir1Coeff[i] = 2;
    }
    for (i = 0; i < 5; i++)
    {
        gDRV_h3aObj.pImgsH3aConfig->afVfvFir2Coeff[i] = 2;
    }
    gDRV_h3aObj.pImgsH3aConfig->afVfvFir1Threshold = 500;
    gDRV_h3aObj.pImgsH3aConfig->afHfvFir1Threshold = 100;
    gDRV_h3aObj.pImgsH3aConfig->afVfvFir2Threshold = 500;
    gDRV_h3aObj.pImgsH3aConfig->afHfvFir2Threshold = 100;

    if (drvImgsFunc->imgsGetModeConfig(config->sensorMode) == NULL)
    {
        OSA_ERROR("DRV_imgsGetModeConfig(%d)\n", config->sensorMode);
        return OSA_EFAIL;
    }

    memcpy(&gDRV_h3aObj.config, config, sizeof(gDRV_h3aObj.config));
    memcpy(&gDRV_h3aObj.imgsModeConfig, drvImgsFunc->imgsGetModeConfig(config->sensorMode), sizeof(gDRV_h3aObj.imgsModeConfig));

    status = DRV_h3aCalcParams();
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_h3aCalcParams()\n");
        return status;
    }

    afBufSize = gDRV_h3aObj.afConfig.paxNumH * gDRV_h3aObj.afConfig.paxNumV * 4 * 16;

    // +1 to accomodate black pixel row of windows
    aewbBufSize = gDRV_h3aObj.aewbConfig.winNumH * (gDRV_h3aObj.aewbConfig.winNumV + 1 ) * 4 * 9;
    afBufSize   = OSA_align(afBufSize, KB);
    aewbBufSize = OSA_align(aewbBufSize, KB) * 2;

    for (i = 0; i < gDRV_h3aObj.config.numBufAf; i++)
    {
        gDRV_h3aObj.afBufInfo[i].virtAddr = OSA_cmemAlloc(afBufSize, 64);
        if (gDRV_h3aObj.afBufInfo[i].virtAddr == NULL)
        {
            OSA_ERROR("OSA_cmemAlloc()\n");
            goto error_exit;
        }
        gDRV_h3aObj.afBufInfo[i].physAddr = OSA_cmemGetPhysAddr((Uint8*)gDRV_h3aObj.afBufInfo[i].virtAddr);
        if (gDRV_h3aObj.afBufInfo[i].physAddr == NULL)
        {
            OSA_ERROR("OSA_cmemGetPhysAddr()\n");
            goto error_exit;
        }

        memset(gDRV_h3aObj.afBufInfo[i].virtAddr, 0x00, afBufSize);

        gDRV_h3aObj.afBufInfo[i].size = afBufSize;
    }

    for (i = 0; i < gDRV_h3aObj.config.numBufAewb; i++)
    {
        gDRV_h3aObj.aewbBufInfo[i].virtAddr = OSA_cmemAlloc(aewbBufSize, 64);
        if (gDRV_h3aObj.aewbBufInfo[i].virtAddr == NULL)
        {
            OSA_ERROR("OSA_cmemAlloc()\n");
            goto error_exit;
        }
        gDRV_h3aObj.aewbBufInfo[i].physAddr = OSA_cmemGetPhysAddr((Uint8*)gDRV_h3aObj.aewbBufInfo[i].virtAddr);
        if (gDRV_h3aObj.aewbBufInfo[i].physAddr == NULL)
        {
            OSA_ERROR("OSA_cmemGetPhysAddr()\n");
            goto error_exit;
        }

        memset(gDRV_h3aObj.aewbBufInfo[i].virtAddr, 0x00, aewbBufSize);

        gDRV_h3aObj.aewbBufInfo[i].size = aewbBufSize;
    }

    gDRV_h3aObj.info.afVfEnable    = gDRV_h3aObj.afConfig.vfEnable;
    gDRV_h3aObj.info.afFvAccMode   = gDRV_h3aObj.afConfig.fvAccMode;
    gDRV_h3aObj.info.afNumWinH     = gDRV_h3aObj.afConfig.paxNumH;
    gDRV_h3aObj.info.afNumWinV     = gDRV_h3aObj.afConfig.paxNumV;

    gDRV_h3aObj.info.aewbOutFormat = gDRV_h3aObj.aewbConfig.outFormat;
    gDRV_h3aObj.info.aewbNumWinH   = gDRV_h3aObj.aewbConfig.winNumH;
    gDRV_h3aObj.info.aewbNumWinV   = gDRV_h3aObj.aewbConfig.winNumV;

    gDRV_h3aObj.info.aewbNumSamplesPerColorInWin =
    ((gDRV_h3aObj.aewbConfig.winWidth  + gDRV_h3aObj.aewbConfig.winIncH - 1) / gDRV_h3aObj.aewbConfig.winIncH) *
    ((gDRV_h3aObj.aewbConfig.winHeight + gDRV_h3aObj.aewbConfig.winIncV - 1) / gDRV_h3aObj.aewbConfig.winIncV);

    status = DRV_h3aSetParams();
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_h3aSetParams()\n");
        goto error_exit;
    }

    return OSA_SOK;

error_exit:

    for (i = 0; i < gDRV_h3aObj.config.numBufAf; i++)
    {
        if (gDRV_h3aObj.afBufInfo[i].virtAddr)
        {
            OSA_cmemFree((Uint8*)gDRV_h3aObj.afBufInfo[i].virtAddr);
        }
    }

    for (i = 0; i < gDRV_h3aObj.config.numBufAewb; i++)
    {
        if (gDRV_h3aObj.aewbBufInfo[i].virtAddr)
        {
            OSA_cmemFree((Uint8*)gDRV_h3aObj.aewbBufInfo[i].virtAddr);
        }
    }

    return OSA_EFAIL;
}

int DRV_h3aClose()
{
    int i = 0;

    DRV_h3aEnable(DRV_H3A_MOD_AF, FALSE);
    DRV_h3aEnable(DRV_H3A_MOD_AEWB, FALSE);

    for (i = 0; i < gDRV_h3aObj.config.numBufAf; i++)
    {
        if (gDRV_h3aObj.afBufInfo[i].virtAddr)
        {
            OSA_cmemFree((Uint8*)gDRV_h3aObj.afBufInfo[i].virtAddr);
        }
    }

    for (i = 0; i < gDRV_h3aObj.config.numBufAewb; i++)
    {
        if (gDRV_h3aObj.aewbBufInfo[i].virtAddr)
        {
            OSA_cmemFree((Uint8*)gDRV_h3aObj.aewbBufInfo[i].virtAddr);
        }
    }

    return OSA_SOK;
}

int DRV_h3aEnable(int h3aMod, Bool enable)
{
    int status = OSA_EFAIL;

    if (h3aMod >= DRV_H3A_MOD_MAX)
    {
        return OSA_EFAIL;
    }

    if (h3aMod == DRV_H3A_MOD_AF)
    {
        status = CSL_h3aAfEnable(&gCSL_h3aHndl, enable);
    }

    if (h3aMod == DRV_H3A_MOD_AEWB)
    {
        status = CSL_h3aAewbEnable(&gCSL_h3aHndl, enable);
    }

    status |= CSL_h3aBufSwitchEnable(&gCSL_h3aHndl, h3aMod, enable);

    return status;
}

int DRV_h3aGetInfo(DRV_H3aInfo *info)
{
    memcpy(info, &gDRV_h3aObj.info, sizeof(gDRV_h3aObj.info));
    return OSA_SOK;
}

int DRV_h3aGetBuf(int h3aMod, int *bufId, int timeout)
{
    int status;
    CSL_BufInfo buf;

    *bufId = -1;

    if (h3aMod >= DRV_H3A_MOD_MAX)
    {
        return OSA_EFAIL;
    }

    status = CSL_h3aBufGetFull(&gCSL_h3aHndl, h3aMod, &buf, 1, timeout);
    if (status != OSA_SOK)
    {
        return status;
    }

    if (h3aMod == DRV_H3A_MOD_AF)
    {
        if (buf.id < 0 || buf.id >= gDRV_h3aObj.config.numBufAf )
        {
            return OSA_EFAIL;
        }

        gDRV_h3aObj.afBufInfo[buf.id].timestamp = buf.timestamp;
        gDRV_h3aObj.afBufInfo[buf.id].count     = buf.count;

        OSA_cmemCacheInv(gDRV_h3aObj.afBufInfo[buf.id].virtAddr,
                         gDRV_h3aObj.afBufInfo[buf.id].size);

    }
    else
    {
        if (buf.id < 0 || buf.id >= gDRV_h3aObj.config.numBufAewb )
        {
            return OSA_EFAIL;
        }

        gDRV_h3aObj.aewbBufInfo[buf.id].timestamp = buf.timestamp;
        gDRV_h3aObj.aewbBufInfo[buf.id].count = buf.count;

        OSA_cmemCacheInv(gDRV_h3aObj.aewbBufInfo[buf.id].virtAddr,
                         gDRV_h3aObj.aewbBufInfo[buf.id].size);
    }

    *bufId = buf.id;
    return status;
}

int DRV_h3aPutBuf(int h3aMod, int bufId)
{
    CSL_BufInfo buf;
    int status;

    if (h3aMod >= DRV_H3A_MOD_MAX)
    {
        return OSA_EFAIL;
    }

    if (h3aMod == DRV_H3A_MOD_AF)
    {
        if (bufId < 0 || bufId >= gDRV_h3aObj.config.numBufAf )
        {
            return OSA_EFAIL;
        }
    }
    else
    {
        if (bufId < 0 || bufId >= gDRV_h3aObj.config.numBufAewb)
        {
            return OSA_EFAIL;
        }
    }

    buf.id = bufId;

    status = CSL_h3aBufPutEmpty(&gCSL_h3aHndl, h3aMod, &buf);

    return status;
}

OSA_BufInfo *DRV_h3aGetBufInfo(int h3aMod, int bufId)
{
    if (h3aMod == DRV_H3A_MOD_AF)
    {
        if (bufId < 0 || bufId >= gDRV_h3aObj.config.numBufAf)
        {
            return NULL;
        }
        return &gDRV_h3aObj.afBufInfo[bufId];
    }
    if (h3aMod == DRV_H3A_MOD_AEWB)
    {
        if (bufId < 0 || bufId >= gDRV_h3aObj.config.numBufAewb)
        {
            return NULL;
        }

        return &gDRV_h3aObj.aewbBufInfo[bufId];
    }

    return NULL;
}

int DRV_h3aCalcParams()
{
    int status=OSA_SOK, i;
    Uint16 inc;
    Uint32 inWidth, inHeight;

    gDRV_h3aObj.inFrameConfig.startV            = 0;
    gDRV_h3aObj.inFrameConfig.line_start        = 0;
    gDRV_h3aObj.inFrameConfig.medFiltThreshold  = gDRV_h3aObj.pImgsH3aConfig->medFiltThreshold;

    gDRV_h3aObj.aewbConfig.enable           = FALSE;
    gDRV_h3aObj.aewbConfig.alawEnable       = FALSE;
    gDRV_h3aObj.aewbConfig.medFiltEnable    = gDRV_h3aObj.pImgsH3aConfig->aewbMedFiltEnable;
    gDRV_h3aObj.aewbConfig.satLimit         = gDRV_h3aObj.pImgsH3aConfig->aewbSatLimit;

    inWidth  = (gDRV_h3aObj.imgsModeConfig.validWidth  - gDRV_h3aObj.pImgsH3aConfig->aewbWinStartX*2);
    inHeight = (gDRV_h3aObj.imgsModeConfig.validHeight - gDRV_h3aObj.pImgsH3aConfig->aewbWinStartY*2);

    if (gDRV_h3aObj.pImgsH3aConfig->aewbWinNumH)
    {
        gDRV_h3aObj.aewbConfig.winWidth     = inWidth/gDRV_h3aObj.pImgsH3aConfig->aewbWinNumH;
    }

    if (gDRV_h3aObj.pImgsH3aConfig->aewbWinNumV)
    {
        gDRV_h3aObj.aewbConfig.winHeight    = inHeight/gDRV_h3aObj.pImgsH3aConfig->aewbWinNumV;
    }

    gDRV_h3aObj.aewbConfig.winWidth         = OSA_floor(gDRV_h3aObj.aewbConfig.winWidth, 2);
    gDRV_h3aObj.aewbConfig.winHeight        = OSA_floor(gDRV_h3aObj.aewbConfig.winHeight, 2);

    gDRV_h3aObj.aewbConfig.winNumH          = gDRV_h3aObj.pImgsH3aConfig->aewbWinNumH;
    gDRV_h3aObj.aewbConfig.winNumV          = gDRV_h3aObj.pImgsH3aConfig->aewbWinNumV;
    gDRV_h3aObj.aewbConfig.winStartH        = gDRV_h3aObj.imgsModeConfig.validStartX + gDRV_h3aObj.pImgsH3aConfig->aewbWinStartX;
    gDRV_h3aObj.aewbConfig.winStartV        = gDRV_h3aObj.imgsModeConfig.validStartY + gDRV_h3aObj.pImgsH3aConfig->aewbWinStartY;

    inc = 2 + OSA_align( (gDRV_h3aObj.aewbConfig.winWidth*gDRV_h3aObj.aewbConfig.winHeight)/(256*2), 2);

    if (inc > 32)
    {
        inc = 32; /* hack to get 5MP working, need to have proper tune fix in 2A */
    }

    gDRV_h3aObj.aewbConfig.winIncH          = inc;
    gDRV_h3aObj.aewbConfig.winIncV          = inc;
    gDRV_h3aObj.aewbConfig.winBlackStartV   = gDRV_h3aObj.aewbConfig.winStartV + inHeight;
    gDRV_h3aObj.aewbConfig.winBlackHeight   = 4;
    gDRV_h3aObj.aewbConfig.outputAddr       = NULL;
    gDRV_h3aObj.aewbConfig.outFormat        = gDRV_h3aObj.pImgsH3aConfig->aewbOutFormat;
    gDRV_h3aObj.aewbConfig.shift            = gDRV_h3aObj.pImgsH3aConfig->aewbShift;

    gDRV_h3aObj.afConfig.enable             = FALSE;
    gDRV_h3aObj.afConfig.alawEnable         = FALSE;
    gDRV_h3aObj.afConfig.vfEnable           = gDRV_h3aObj.pImgsH3aConfig->afVfEnable;
    gDRV_h3aObj.afConfig.medFiltEnable      = gDRV_h3aObj.pImgsH3aConfig->afMedFiltEnable;
    gDRV_h3aObj.afConfig.rgbPos             = gDRV_h3aObj.pImgsH3aConfig->afRgbPos;
    gDRV_h3aObj.afConfig.fvAccMode          = gDRV_h3aObj.pImgsH3aConfig->afFvAccMode;

    inWidth  = (gDRV_h3aObj.imgsModeConfig.validWidth  - gDRV_h3aObj.pImgsH3aConfig->afPaxStartX*2);
    inHeight = (gDRV_h3aObj.imgsModeConfig.validHeight - gDRV_h3aObj.pImgsH3aConfig->afPaxStartY*2);

    if (gDRV_h3aObj.pImgsH3aConfig->afPaxNumH)
        gDRV_h3aObj.afConfig.paxWidth           = inWidth/gDRV_h3aObj.pImgsH3aConfig->afPaxNumH;

    if (gDRV_h3aObj.pImgsH3aConfig->afPaxNumV)
        gDRV_h3aObj.afConfig.paxHeight          = inHeight/gDRV_h3aObj.pImgsH3aConfig->afPaxNumV;

    gDRV_h3aObj.afConfig.paxWidth         = OSA_floor(gDRV_h3aObj.afConfig.paxWidth, 2);
    gDRV_h3aObj.afConfig.paxHeight        = OSA_floor(gDRV_h3aObj.afConfig.paxHeight, 2);

    inc = 2 + OSA_align( (gDRV_h3aObj.afConfig.paxWidth*gDRV_h3aObj.afConfig.paxHeight)/(256*2), 2);

    gDRV_h3aObj.afConfig.paxIncH            = 8;//inc;
    gDRV_h3aObj.afConfig.paxIncV            = 8;//inc;
    gDRV_h3aObj.afConfig.paxNumH            = gDRV_h3aObj.pImgsH3aConfig->afPaxNumH;
    gDRV_h3aObj.afConfig.paxNumV            = gDRV_h3aObj.pImgsH3aConfig->afPaxNumV;
    gDRV_h3aObj.afConfig.paxStartH          = gDRV_h3aObj.imgsModeConfig.validStartX + gDRV_h3aObj.pImgsH3aConfig->afPaxStartX;
    gDRV_h3aObj.afConfig.paxStartV          = gDRV_h3aObj.imgsModeConfig.validStartY + gDRV_h3aObj.pImgsH3aConfig->afPaxStartY;
    gDRV_h3aObj.afConfig.iirStartH          = gDRV_h3aObj.afConfig.paxStartH - 2;
    //gDRV_h3aObj.afConfig.iirStartH          = 0;
    gDRV_h3aObj.afConfig.outputAddr         = NULL;

    for (i = 0; i < 11; i++)
    {
        gDRV_h3aObj.afConfig.iirCoeff0[i]    = gDRV_h3aObj.pImgsH3aConfig->afIirCoeff0[i];
        gDRV_h3aObj.afConfig.iirCoeff1[i]    = gDRV_h3aObj.pImgsH3aConfig->afIirCoeff1[i];
    }

    for (i = 0; i < 11; i++)
    {
        gDRV_h3aObj.afConfig.vfvFir1Coeff[i] = gDRV_h3aObj.pImgsH3aConfig->afVfvFir1Coeff[i];
        gDRV_h3aObj.afConfig.vfvFir2Coeff[i] = gDRV_h3aObj.pImgsH3aConfig->afVfvFir2Coeff[i];
    }

    gDRV_h3aObj.afConfig.vfvFir1Threshold   = gDRV_h3aObj.pImgsH3aConfig->afVfvFir1Threshold;
    gDRV_h3aObj.afConfig.hfvFir1Threshold   = gDRV_h3aObj.pImgsH3aConfig->afHfvFir1Threshold;
    gDRV_h3aObj.afConfig.vfvFir2Threshold   = gDRV_h3aObj.pImgsH3aConfig->afVfvFir2Threshold;
    gDRV_h3aObj.afConfig.hfvFir2Threshold   = gDRV_h3aObj.pImgsH3aConfig->afHfvFir2Threshold;

    return status;
}

int DRV_h3aSetParams()
{
    int status, i;
    CSL_H3aHwSetup setup;
    CSL_BufInit afBufInit, aewbBufInit;

    status = CSL_h3aHwReset(&gCSL_h3aHndl);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_h3aHwReset()\n");
        return status;
    }

    setup.inFrameConfig = &gDRV_h3aObj.inFrameConfig;

    if (gDRV_h3aObj.config.numBufAf)
    {
        setup.afConfig      = &gDRV_h3aObj.afConfig;
    }
    else
    {
        setup.afConfig      = NULL;
    }

    if (gDRV_h3aObj.config.numBufAewb)
    {
        setup.aewbConfig    = &gDRV_h3aObj.aewbConfig;
    }
    else
    {
        setup.aewbConfig    = NULL;
    }

#ifdef DRV_SHOW_INFO
    OSA_printf(" \n");
    OSA_printf(" H3A Settings,\n");
    OSA_printf(" AEWB Win Start H       = %d\n", gDRV_h3aObj.aewbConfig.winStartH);
    OSA_printf(" AEWB Win Start V       = %d\n", gDRV_h3aObj.aewbConfig.winStartV);
    OSA_printf(" AEWB Win Num H         = %d\n", gDRV_h3aObj.aewbConfig.winNumH);
    OSA_printf(" AEWB Win Num V         = %d\n", gDRV_h3aObj.aewbConfig.winNumV);
    OSA_printf(" AEWB Win Width         = %d\n", gDRV_h3aObj.aewbConfig.winWidth);
    OSA_printf(" AEWB Win Height        = %d\n", gDRV_h3aObj.aewbConfig.winHeight);
    OSA_printf(" AEWB Win Inc H         = %d\n", gDRV_h3aObj.aewbConfig.winIncH);
    OSA_printf(" AEWB Win Inc V         = %d\n", gDRV_h3aObj.aewbConfig.winIncV);
    OSA_printf(" AEWB Win Pixels/Color  = %d\n", gDRV_h3aObj.info.aewbNumSamplesPerColorInWin);
    OSA_printf(" \n");
#endif

#ifdef AF_DEBUG_INFO
    OSA_printf("setup.aewbConfig->enable: %d\n",setup.aewbConfig->enable);
    OSA_printf("setup.afConfig->enable: %d\n",setup.afConfig->enable);
    OSA_printf("setup.afConfig->alawEnable: %d\n",setup.afConfig->alawEnable);
    OSA_printf("setup.afConfig->vfEnable: %d\n",setup.afConfig->vfEnable);
    OSA_printf("setup.afConfig->medFiltEnable: %d\n",setup.afConfig->medFiltEnable);
    OSA_printf("setup.afConfig->rgbPos: %d\n",setup.afConfig->rgbPos);
    OSA_printf("setup.afConfig->fvAccMode: %d\n",setup.afConfig->fvAccMode);
    OSA_printf("setup.afConfig->paxWidth: %d\n",setup.afConfig->paxWidth);
    OSA_printf("setup.afConfig->paxHeight: %d\n",setup.afConfig->paxHeight);
    OSA_printf("setup.afConfig->paxIncH: %d\n",setup.afConfig->paxIncH);
    OSA_printf("setup.afConfig->paxIncV: %d\n",setup.afConfig->paxIncV);
    OSA_printf("setup.afConfig->paxNumH: %d\n",setup.afConfig->paxNumH);
    OSA_printf("setup.afConfig->paxNumV: %d\n",setup.afConfig->paxNumV);
    OSA_printf("setup.afConfig->paxStartH: %d\n",setup.afConfig->paxStartH);
    OSA_printf("setup.afConfig->paxStartV: %d\n",setup.afConfig->paxStartV);
    OSA_printf("setup.afConfig->iirStartH: %d\n",setup.afConfig->iirStartH);
    OSA_printf("setup.afConfig->outputAddr: %d\n",setup.afConfig->outputAddr);
    for (i = 0; i < 11; i++)
    {
        OSA_printf("setup.afConfig->iirCoeff0[%d]: %d\n", i, setup.afConfig->iirCoeff0[i]);
    }
    for (i = 0; i < 11; i++)
    {
        OSA_printf("setup.afConfig->iirCoeff1[%d]: %d\n",i, setup.afConfig->iirCoeff1[i]);
    }
    for (i = 0; i < 5; i++)
    {
        OSA_printf("setup.afConfig->vfvFir1Coeff[%d]: %d\n",i, setup.afConfig->vfvFir1Coeff[i]);
    }
    for (i = 0; i < 5; i++)
    {
        OSA_printf("setup.afConfig->vfvFir2Coeff[%d]: %d\n",i, setup.afConfig->vfvFir2Coeff[i]);
    }
    OSA_printf("setup.afConfig->vfvFir1Threshold: %d\n",setup.afConfig->vfvFir1Threshold);
    OSA_printf("setup.afConfig->hfvFir1Threshold: %d\n",setup.afConfig->hfvFir1Threshold);
    OSA_printf("setup.afConfig->vfvFir2Threshold: %d\n",setup.afConfig->vfvFir2Threshold);
    OSA_printf("setup.afConfig->hfvFir2Threshold: %d\n",setup.afConfig->hfvFir2Threshold);
#endif

    status = CSL_h3aHwSetup(&gCSL_h3aHndl, &setup);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_h3aHwSetup()\n");
        return status;
    }

    CSL_h3aAewbSetOutAddr(&gCSL_h3aHndl, (Uint8*)gDRV_h3aObj.aewbBufInfo[0].physAddr);
    CSL_h3aAfSetOutAddr(&gCSL_h3aHndl, (Uint8*)gDRV_h3aObj.afBufInfo[0].physAddr);

    if (gDRV_h3aObj.config.numBufAf)
    {
        afBufInit.numBuf = gDRV_h3aObj.config.numBufAf;
        afBufInit.curBuf = 0;
        for (i = 0; i < gDRV_h3aObj.config.numBufAf; i++)
        {
            afBufInit.bufAddr[i] = (Uint32)gDRV_h3aObj.afBufInfo[i].physAddr;
        }

        status = CSL_h3aBufInit(&gCSL_h3aHndl, DRV_H3A_MOD_AF, &afBufInit);
        if (status != OSA_SOK)
        {
            OSA_ERROR("CSL_h3aBufInit()\n");
            return status;
        }
    }

    if (gDRV_h3aObj.config.numBufAewb)
    {
        aewbBufInit.numBuf = gDRV_h3aObj.config.numBufAewb;
        aewbBufInit.curBuf = 0;

        for (i = 0; i < gDRV_h3aObj.config.numBufAewb; i++)
        {
            aewbBufInit.bufAddr[i] = (Uint32)gDRV_h3aObj.aewbBufInfo[i].physAddr;
        }

        status = CSL_h3aBufInit(&gCSL_h3aHndl, DRV_H3A_MOD_AEWB, &aewbBufInit);
        if (status != OSA_SOK)
        {
            OSA_ERROR("CSL_h3aBufInit()\n");
            return status;
        }
    }

    return status;
}

