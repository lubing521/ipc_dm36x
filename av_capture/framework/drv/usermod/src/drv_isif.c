
#include <drv_isifPriv.h>
#include <osa_cmem.h>
#include <osa_file.h>

static DRV_IsifObj gDRV_isifObj;

OSA_MutexHndl isifLock; //ANR - ISIF

int DRV_isifOpen(DRV_IsifConfig *config)
{
    int bufSize, i;
    int status = OSA_mutexCreate(&isifLock);  //ANR - ISIF

    CSL_ccdcEnable(&gCSL_ccdcHndl, FALSE);

    memset(&gDRV_isifObj, 0, sizeof(gDRV_isifObj));

    gDRV_isifObj.pImgsIsifConfig = drvImgsFunc->imgsGetIsifConfig(config->sensorMode);
    if (gDRV_isifObj.pImgsIsifConfig == NULL)
    {
        OSA_ERROR("DRV_imgsGetIsifConfig(%d)\n", config->sensorMode);
        return OSA_EFAIL;
    }

    if (config->numBuf > CSL_BUF_NUM_MAX)
    {
        OSA_ERROR("exceeded max buf limit of %d\n", CSL_BUF_NUM_MAX);
        return OSA_EFAIL;
    }

    if (drvImgsFunc->imgsGetModeConfig(config->sensorMode)==NULL)
    {
        OSA_ERROR("DRV_imgsGetModeConfig()\n");
        return OSA_EFAIL;
    }

    if (config->alawEnable)
        config->dpcmEnable = FALSE;

    memcpy(&gDRV_isifObj.imgsModeInfo, drvImgsFunc->imgsGetModeConfig(config->sensorMode), sizeof(gDRV_isifObj.imgsModeInfo));
    memcpy(&gDRV_isifObj.config, config, sizeof(gDRV_isifObj.config));

    gDRV_isifObj.info.ddrOutDataOffsetH = gDRV_isifObj.imgsModeInfo.validWidth;

    if (config->alawEnable)
    {

    }
    else
        if (config->dpcmEnable)
    {
        gDRV_isifObj.info.ddrOutDataOffsetH = (gDRV_isifObj.info.ddrOutDataOffsetH*3)/2;
    }
    else
    {
        gDRV_isifObj.info.ddrOutDataOffsetH = (gDRV_isifObj.info.ddrOutDataOffsetH*2);
    }

    gDRV_isifObj.info.ddrOutDataOffsetH = OSA_align(gDRV_isifObj.info.ddrOutDataOffsetH, 32);
    gDRV_isifObj.info.ddrOutDataWidth  = gDRV_isifObj.imgsModeInfo.validWidth;
#ifdef YUV_MODE_INTERLACED
    gDRV_isifObj.info.ddrOutDataHeight = gDRV_isifObj.imgsModeInfo.validHeight * 2;
#else
    gDRV_isifObj.info.ddrOutDataHeight = gDRV_isifObj.imgsModeInfo.validHeight;
#endif

    gDRV_isifObj.numBuf = config->numBuf;
    gDRV_isifObj.numLscBuf = config->numLscBuf;

    gDRV_isifObj.lscGainTableVirtAddr = OSA_cmemAlloc(IMAGE_TUNE_LSC_BUFFER_MAX, 32);
    gDRV_isifObj.lscOffsetTableVirtAddr = OSA_cmemAlloc(IMAGE_TUNE_LSC_BUFFER_MAX, 32);

    if (gDRV_isifObj.lscGainTableVirtAddr)
        gDRV_isifObj.lscGainTablePhysAddr = OSA_cmemGetPhysAddr(gDRV_isifObj.lscGainTableVirtAddr);

    if (gDRV_isifObj.lscOffsetTableVirtAddr)
        gDRV_isifObj.lscOffsetTablePhysAddr = OSA_cmemGetPhysAddr(gDRV_isifObj.lscOffsetTableVirtAddr);

    if (     gDRV_isifObj.lscGainTableVirtAddr==NULL
             ||  gDRV_isifObj.lscOffsetTableVirtAddr==NULL
             ||  gDRV_isifObj.lscGainTablePhysAddr==NULL
             ||  gDRV_isifObj.lscOffsetTablePhysAddr==NULL
       )
    {
        OSA_ERROR("OSA_cmemAlloc()\n");
        goto error_exit;
    }

#ifdef YUV_MODE_INTERLACED
    bufSize = gDRV_isifObj.info.ddrOutDataOffsetH*(gDRV_isifObj.info.ddrOutDataHeight) * 2 ;//+16);
#else
    bufSize = gDRV_isifObj.info.ddrOutDataOffsetH*(gDRV_isifObj.info.ddrOutDataHeight);//+16);
#endif

    for (i=0; i<gDRV_isifObj.numBuf; i++)
    {
        gDRV_isifObj.bufInfo[i].virtAddr = OSA_cmemAlloc(bufSize, 32);
        if (gDRV_isifObj.bufInfo[i].virtAddr==NULL)
        {
            OSA_ERROR("OSA_cmemAlloc()\n");
            goto error_exit;
        }

#ifdef DRV_BUF_CLEAR
        memset(gDRV_isifObj.bufInfo[i].virtAddr, 0xAA, bufSize);
#endif

        gDRV_isifObj.bufInfo[i].physAddr = OSA_cmemGetPhysAddr((Uint8*)gDRV_isifObj.bufInfo[i].virtAddr);
        if (gDRV_isifObj.bufInfo[i].physAddr==NULL)
        {
            OSA_ERROR("OSA_cmemGetPhysAddr()\n");
            goto error_exit;
        }

        gDRV_isifObj.bufInfo[i].size  = bufSize;
    }

    bufSize = IMAGE_TUNE_LSC_BUFFER_MAX*2*4;

    for (i=0; i<gDRV_isifObj.numLscBuf; i++)
    {
        gDRV_isifObj.lscBufInfo[i].virtAddr = OSA_cmemAlloc(bufSize, 32);
        if (gDRV_isifObj.lscBufInfo[i].virtAddr==NULL)
        {
            OSA_ERROR("OSA_cmemAlloc()\n");
            goto error_exit;
        }

        memset(gDRV_isifObj.lscBufInfo[i].virtAddr, 0x00, bufSize);

        gDRV_isifObj.lscBufInfo[i].physAddr = OSA_cmemGetPhysAddr((Uint8*)gDRV_isifObj.lscBufInfo[i].virtAddr);
        if (gDRV_isifObj.lscBufInfo[i].physAddr==NULL)
        {
            OSA_ERROR("OSA_cmemGetPhysAddr()\n");
            goto error_exit;
        }

        gDRV_isifObj.lscBufInfo[i].size  = bufSize;
    }

    status = DRV_isifSetParams();
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_isifSetParams()\n");
        goto error_exit;
    }

    return OSA_SOK;

    error_exit:

    if (gDRV_isifObj.lscGainTableVirtAddr)
        OSA_cmemFree(gDRV_isifObj.lscGainTableVirtAddr);

    if (gDRV_isifObj.lscOffsetTableVirtAddr)
        OSA_cmemFree(gDRV_isifObj.lscOffsetTableVirtAddr);

    for (i = 0; i < gDRV_isifObj.numBuf; i++)
    {
        if (gDRV_isifObj.bufInfo[i].virtAddr)
            OSA_cmemFree((Uint8*)gDRV_isifObj.bufInfo[i].virtAddr);
    }

    for (i = 0; i < gDRV_isifObj.numLscBuf; i++)
    {
        if (gDRV_isifObj.lscBufInfo[i].virtAddr)
            OSA_cmemFree((Uint8*)gDRV_isifObj.lscBufInfo[i].virtAddr);
    }

    return OSA_EFAIL;
}

int DRV_isifClose()
{
    int i;

    OSA_mutexDelete(&isifLock);  //ANR - ISIF

    DRV_isifOutEnable(FALSE);
    DRV_isifLscEnable(FALSE);
    DRV_isifEnable(FALSE);

    CSL_ccdcIntEnable(&gCSL_ccdcHndl, DRV_ISIF_INT_VD0, FALSE);
    CSL_ccdcIntEnable(&gCSL_ccdcHndl, DRV_ISIF_INT_VD1, FALSE);
    CSL_ccdcIntClear(&gCSL_ccdcHndl, DRV_ISIF_INT_VD0);
    CSL_ccdcIntClear(&gCSL_ccdcHndl, DRV_ISIF_INT_VD1);

    if (gDRV_isifObj.lscGainTableVirtAddr)
    {
        OSA_cmemFree(gDRV_isifObj.lscGainTableVirtAddr);
        gDRV_isifObj.lscGainTableVirtAddr = NULL;
        gDRV_isifObj.lscGainTablePhysAddr = NULL;
    }

    if (gDRV_isifObj.lscOffsetTableVirtAddr)
    {
        OSA_cmemFree(gDRV_isifObj.lscOffsetTableVirtAddr);
        gDRV_isifObj.lscOffsetTableVirtAddr = NULL;
        gDRV_isifObj.lscOffsetTablePhysAddr = NULL;
    }

    for (i = 0; i < gDRV_isifObj.numBuf; i++)
    {
        if (gDRV_isifObj.bufInfo[i].virtAddr)
            OSA_cmemFree((Uint8*)gDRV_isifObj.bufInfo[i].virtAddr);
    }

    for (i = 0; i < gDRV_isifObj.numLscBuf; i++)
    {
        if (gDRV_isifObj.lscBufInfo[i].virtAddr)
            OSA_cmemFree((Uint8*)gDRV_isifObj.lscBufInfo[i].virtAddr);
    }

    return OSA_SOK;
}

int DRV_isifGetInfo(DRV_IsifInfo *info)
{
    memcpy(info, &gDRV_isifObj.info, sizeof(gDRV_isifObj.info));

    return OSA_SOK;
}

int DRV_isifOutEnable(Bool enable)
{
    int status = CSL_ccdcSdramOutEnable(&gCSL_ccdcHndl, enable);
    status |= CSL_ccdcBufSwitchEnable(&gCSL_ccdcHndl, enable);

    return status;
}

int DRV_isifEnable(Bool enable)
{
    int status;

    if (gDRV_isifObj.numLscBuf)
    {
        DRV_isifLscEnable(enable);
    }

    status = CSL_ccdcEnable(&gCSL_ccdcHndl, enable);

    return status;
}


int DRV_isifGetOutBuf(int *bufId, int timeout)
{
    int status;
    CSL_BufInfo buf;

    *bufId = -1;

    status = CSL_ccdcBufGetFull(&gCSL_ccdcHndl, &buf, 1, timeout);
    if (status != OSA_SOK)
    {
        return status;
    }

    if (buf.id < 0 || buf.id >= gDRV_isifObj.numBuf )
    {
        return OSA_EFAIL;
    }

    gDRV_isifObj.bufInfo[buf.id].timestamp = buf.timestamp;
    gDRV_isifObj.bufInfo[buf.id].count = buf.count;

    *bufId = buf.id;

    return status;
}

int DRV_isifPutOutBuf(int bufId)
{
    CSL_BufInfo buf;
    int status;

    if (bufId < 0 || bufId >= gDRV_isifObj.numBuf)
    {
        return OSA_EFAIL;
    }

    buf.id = bufId;

    status = CSL_ccdcBufPutEmpty(&gCSL_ccdcHndl, &buf);

    return status;
}

OSA_BufInfo *DRV_isifGetOutBufInfo(int bufId)
{
    if (bufId < 0 || bufId >= gDRV_isifObj.numBuf)
    {
        return NULL;
    }

    return &gDRV_isifObj.bufInfo[bufId];
}

int DRV_isifLscGetBuf(int *bufId, int timeout)
{
    int status;
    CSL_BufInfo buf;

    *bufId = -1;

    status = CSL_ccdcLscBufGetEmpty(&gCSL_ccdcHndl, &buf, 1, timeout);
    if (status != OSA_SOK)
    {
        return status;
    }

    if (buf.id < 0 || buf.id >= gDRV_isifObj.numLscBuf )
    {
        return OSA_EFAIL;
    }

    gDRV_isifObj.lscBufInfo[buf.id].timestamp = buf.timestamp;
    gDRV_isifObj.lscBufInfo[buf.id].count = buf.count;

    *bufId = buf.id;

    return status;
}

int DRV_isifLscPutBuf(int bufId)
{
    CSL_BufInfo buf;
    int status;

    if (bufId < 0 || bufId >= gDRV_isifObj.numLscBuf)
    {
        return OSA_EFAIL;
    }

    buf.id = bufId;

    status = CSL_ccdcLscBufPutFull(&gCSL_ccdcHndl, &buf);

    return status;
}

OSA_BufInfo *DRV_isifLscGetBufInfo(int bufId)
{
    if (bufId < 0 || bufId >= gDRV_isifObj.numLscBuf)
    {
        return NULL;
    }

    return &gDRV_isifObj.lscBufInfo[bufId];
}

int DRV_isifLscEnable(Bool enable)
{
    int status = CSL_ccdcLscBufSwitchEnable(&gCSL_ccdcHndl, enable);
    status |= CSL_ccdcLscEnable(&gCSL_ccdcHndl, enable);

    // for > 1 VD for this to take effect
    OSA_waitMsecs(50);

    return OSA_SOK;
}

int DRV_isifSetDcSub(Int16 dcSub)
{
    DRV_isifLock();  //ANR - ISIF
    gCSL_vpssHndl.isifRegs->CLDCOFST = dcSub;

    IMAGE_TUNE_SetIsifDcsub(dcSub);

    DRV_isifUnlock();  //ANR - ISIF

    return 0;
}

int DRV_isifSetDgain(Uint16 gainR, Uint16 gainGb, Uint16 gainGr, Uint16 gainB, Uint16 offset)
{
    int status;
    CSL_CcdcGainOffsetConfig prm;

    DRV_isifLock();  //ANR - ISIF

    prm.gainR  = gainR;
    prm.gainGr = gainGr;
    prm.gainGb = gainGb;
    prm.gainB  = gainB;
    prm.offset = offset;
    prm.ipipeGainEnable        = TRUE;
    prm.ipipeOffsetEnable      = TRUE;
    prm.h3aGainEnable          = TRUE;
    prm.h3aOffsetEnable        = TRUE;
    prm.sdramOutGainEnable     = TRUE;
    prm.sdramOutOffsetEnable   = TRUE;

    status = CSL_ccdcSetGainOffsetConfig(&gCSL_ccdcHndl, &prm);

    IMAGE_TUNE_SetIsifGainOffset(&prm);

    DRV_isifUnlock();  //ANR - ISIF

    return status;
}

int DRV_isifWaitInt(Uint16 intId, Uint16 numIntsToWait, Uint32 timeout)
{
    int status;

    if (intId > DRV_ISIF_INT_VD1)
    {
        return OSA_EFAIL;
    }

    status = CSL_ccdcIntWait(&gCSL_ccdcHndl, intId, numIntsToWait, timeout);

    return status;
}

int DRV_isifLscSetParams()
{
    Uint16 paxelSizeH, paxelSizeV, inWidth, inHeight, offsetH;

    if (gDRV_isifObj.numLscBuf)
    {
        paxelSizeH = gDRV_isifObj.config.lscConfig.paxelWidth;
        paxelSizeV = gDRV_isifObj.config.lscConfig.paxelHeight;

        inWidth = gDRV_isifObj.imgsModeInfo.validWidth;
#ifdef YUV_MODE_INTERLACED
        inHeight= gDRV_isifObj.imgsModeInfo.validHeight * 2;
#else
        inHeight= gDRV_isifObj.imgsModeInfo.validHeight;
#endif
        offsetH = OSA_align((inWidth*4)/paxelSizeH, 4);

        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.paxelWidth  = paxelSizeH;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.paxelHeight = paxelSizeV;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.inWidth = inWidth;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.inHeight= inHeight;

        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.enable=FALSE;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.intSofEnable=FALSE;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.intPreFetchErrorEnable=FALSE;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.intDoneEnable=FALSE;

        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.gainTableAddr = (Uint8*)gDRV_isifObj.bufInfo[0].physAddr;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.offsetTableAddr = (Uint8*)gDRV_isifObj.bufInfo[0].physAddr+gDRV_isifObj.lscBufInfo[0].size/2;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.gainTableLineOffset = offsetH;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.offsetTableLineOffset = offsetH;

        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.initX=gDRV_isifObj.config.lscConfig.initX;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.initY=gDRV_isifObj.config.lscConfig.initY;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.gainFormat = gDRV_isifObj.config.lscConfig.gainFormat;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.offsetEnable = gDRV_isifObj.config.lscConfig.offsetEnable;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.offsetScale = gDRV_isifObj.config.lscConfig.offsetScale;
        gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.offsetShift = gDRV_isifObj.config.lscConfig.offsetShift;

        gDRV_isifObj.info.lscDataWidth = (inWidth/paxelSizeH);
        gDRV_isifObj.info.lscDataHeight = (inHeight/paxelSizeV);
        gDRV_isifObj.info.lscDataOffsetH = offsetH;
        gDRV_isifObj.info.lscOffsetTableOffsetInBuf = gDRV_isifObj.lscBufInfo[0].size/2;
    }

    return OSA_SOK;
}

int DRV_isifSetParams()
{
    int status, i;
    CSL_CcdcHwSetup             setup;
    CSL_CcdcInDataConfig        inDataConfig;
    CSL_CcdcMiscConfig          miscConfig;
    CSL_CcdcDfcLscOffsetConfig  dfcLscOffsetConfig;
    CSL_CcdcSdramOutConfig      sdrOutConfig;
    CSL_CcdcDpcmConfig          dpcmConfig;
    CSL_CcdcVdIntConfig         vdIntConfig;
    CSL_IpipeifVpifIsifInConfig isifInConfig;
    CSL_IpipeifH3aConfig        h3aConfig;
    CSL_BufInit                 bufInit;
    DRV_ImgsIpipeConfig         *pIpipeConfig;

    Uint16 vdInt;

    DRV_isifLock();  //ANR - ISIF

    status = CSL_ccdcHwReset(&gCSL_ccdcHndl);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_ccdcHwReset()\n");
        return status;
    }

    inDataConfig.hLpfEnable         = gDRV_isifObj.pImgsIsifConfig->ccdcParams.hLpfEnable;
    inDataConfig.inDataMsbPosition  = gDRV_isifObj.pImgsIsifConfig->ccdcParams.inDataMsbPosition;
    inDataConfig.dataPolarity       = CSL_CCDC_DATA_POLARITY_NO_CHANGE;

    if (SensorIsYuvMode() > 0)
        inDataConfig.inDataType     = CSL_CCDC_IN_DATA_TYPE_YUV16;
    else
        inDataConfig.inDataType     = CSL_CCDC_IN_DATA_TYPE_RAW;

    if (SensorIsYuvModeProgressive() > 0)
        inDataConfig.yPos           = CSL_CCDC_Y_POS_EVEN;
    else
        inDataConfig.yPos           = CSL_CCDC_Y_POS_ODD;
 
    miscConfig.cfaPattern                     = CSL_CCDC_CFA_PATTERN_MOSIAC;
    miscConfig.vdLatchDisable                 = FALSE;
    miscConfig.inverseMsbCout                 = FALSE;
    miscConfig.inverseMsbCin                  = FALSE;
    miscConfig.sdramAddrInitExtTiggerEnable   = FALSE;
    miscConfig.sdramAddrInitTiggerSource      = CSL_CCDC_SDR_ADDR_INIT_DWEN;
    miscConfig.fidLatchAtVdDisable            = FALSE;
    miscConfig.ycOutSwap                      = FALSE;

    if (SensorIsYuvModeProgressive() > 0)
        miscConfig.orWenIntExt                = FALSE;
    else
        miscConfig.orWenIntExt                = TRUE;

    if (SensorIsYuvMode() > 0)
        miscConfig.ycInSwap                   = FALSE;//TRUE;
    else
        miscConfig.ycInSwap                   = FALSE;
    
#ifdef OV10630
    miscConfig.ycInSwap                       = FALSE;
#endif

    dfcLscOffsetConfig.hOffset                = gDRV_isifObj.imgsModeInfo.validStartX;
    dfcLscOffsetConfig.vOffset                = gDRV_isifObj.imgsModeInfo.validStartY;

    if (SensorIsYuvModeProgressive() == 0)
    {
        dfcLscOffsetConfig.vOffset            = 1;
    }

    sdrOutConfig.outDataShift         = 0;
    sdrOutConfig.outStartH            = gDRV_isifObj.imgsModeInfo.validStartX;
    sdrOutConfig.outStartV0           = gDRV_isifObj.imgsModeInfo.validStartY;
    sdrOutConfig.outStartV1           = gDRV_isifObj.imgsModeInfo.validStartY;
#ifdef YUV_MODE_INTERLACED
    sdrOutConfig.outStartV0           = 1;
    sdrOutConfig.outStartV1           = 1;
#endif
    if (inDataConfig.inDataType == CSL_CCDC_IN_DATA_TYPE_YUV8)
        sdrOutConfig.outWidth           = gDRV_isifObj.imgsModeInfo.validWidth*2; // Needed to change the outWidth when YUV8 bit mode is selected
    else
        sdrOutConfig.outWidth           = gDRV_isifObj.imgsModeInfo.validWidth; // Needed to change the outWidth when YUV8 bit mode is selected

#ifdef YUV_MODE_INTERLACED
    sdrOutConfig.outHeight            = gDRV_isifObj.imgsModeInfo.validHeight * 2;
#else
    sdrOutConfig.outHeight            = gDRV_isifObj.imgsModeInfo.validHeight;
#endif

    sdrOutConfig.culHEven             = 0xFF;
    sdrOutConfig.culHOdd              = 0xFF;
    sdrOutConfig.culV                 = 0xFF;
    sdrOutConfig.outLineOffset        = gDRV_isifObj.info.ddrOutDataOffsetH;
    sdrOutConfig.outAddrDecrement     = FALSE;

    sdrOutConfig.sdramOffsetConfig.fidInv               = FALSE;
    sdrOutConfig.sdramOffsetConfig.fidOffsetOdd         = CSL_CCDC_SDR_OFFSET_PLUS_1LINE;
    sdrOutConfig.sdramOffsetConfig.lineOffsetEvenEven   = CSL_CCDC_SDR_OFFSET_PLUS_1LINE;
    sdrOutConfig.sdramOffsetConfig.lineOffsetOddEven    = CSL_CCDC_SDR_OFFSET_PLUS_1LINE;
    sdrOutConfig.sdramOffsetConfig.lineOffsetEvenOdd    = CSL_CCDC_SDR_OFFSET_PLUS_1LINE;
    sdrOutConfig.sdramOffsetConfig.lineOffsetOddOdd     = CSL_CCDC_SDR_OFFSET_PLUS_1LINE;
#ifdef YUV_MODE_INTERLACED
    sdrOutConfig.sdramOffsetConfig.fidOffsetOdd         = CSL_CCDC_SDR_OFFSET_PLUS_1LINE;
    sdrOutConfig.sdramOffsetConfig.lineOffsetEvenEven   = CSL_CCDC_SDR_OFFSET_PLUS_2LINE;
    sdrOutConfig.sdramOffsetConfig.lineOffsetOddEven    = CSL_CCDC_SDR_OFFSET_PLUS_2LINE;
    sdrOutConfig.sdramOffsetConfig.lineOffsetEvenOdd    = CSL_CCDC_SDR_OFFSET_PLUS_2LINE;
    sdrOutConfig.sdramOffsetConfig.lineOffsetOddOdd     = CSL_CCDC_SDR_OFFSET_PLUS_2LINE;
#endif

    sdrOutConfig.outAddr              = (Uint8*)gDRV_isifObj.bufInfo[0].physAddr;
    sdrOutConfig.alawEnable           = gDRV_isifObj.config.alawEnable;
#ifdef OV10630
    sdrOutConfig.byteSwapEnable       = TRUE;
#else
    sdrOutConfig.byteSwapEnable       = FALSE;
#endif

    if (gDRV_isifObj.config.alawEnable)
        sdrOutConfig.packMode             = CSL_CCDC_SDR_OUT_TYPE_8BITS_PER_PIXEL;
    else
        if (gDRV_isifObj.config.dpcmEnable)
        sdrOutConfig.packMode             = CSL_CCDC_SDR_OUT_TYPE_12BITS_PER_PIXEL;
    else
        sdrOutConfig.packMode             = CSL_CCDC_SDR_OUT_TYPE_16BITS_PER_PIXEL;

    if (inDataConfig.inDataType == CSL_CCDC_IN_DATA_TYPE_YUV8)
    {
        sdrOutConfig.packMode             = CSL_CCDC_SDR_OUT_TYPE_8BITS_PER_PIXEL;
    }
    dpcmConfig.enable       = gDRV_isifObj.config.dpcmEnable;
    dpcmConfig.predictor    = CSL_CCDC_DPCM_PREDICTOR_1;

    if (SensorIsYuvMode() > 0)
    {
        vdInt = ((Uint32)gDRV_isifObj.imgsModeInfo.sensorDataHeight-32);
    }
    else
    {
        vdInt = ((Uint32)gDRV_isifObj.imgsModeInfo.sensorDataHeight-1);
    }

    vdIntConfig.vdInt0  = vdInt;
    vdIntConfig.vdInt1  = vdInt;
    vdIntConfig.vdInt2  = vdInt;

    gDRV_isifObj.pImgsIsifConfig->ccdcParams.lin.lutTable        = gDRV_isifObj.pImgsIsifConfig->ccdcParams.linTable;
    gDRV_isifObj.pImgsIsifConfig->ccdcParams.dfc.vdfcTable       = &gDRV_isifObj.pImgsIsifConfig->ccdcParams.vdfcTable;
    gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.gainTableAddr   =  gDRV_isifObj.lscGainTablePhysAddr;
    gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.offsetTableAddr =  gDRV_isifObj.lscOffsetTablePhysAddr;

    if (gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc.enable)
    {
        memcpy(gDRV_isifObj.lscGainTableVirtAddr  , gDRV_isifObj.pImgsIsifConfig->ccdcParams.lscGainTable, IMAGE_TUNE_LSC_BUFFER_MAX);
        memcpy(gDRV_isifObj.lscOffsetTableVirtAddr, gDRV_isifObj.pImgsIsifConfig->ccdcParams.lscOffsetTable, IMAGE_TUNE_LSC_BUFFER_MAX);
    }

#ifdef YUV_MODE_INTERLACED
    CSL_CcdcRec656Config  rec656Config;
    rec656Config.enable             = TRUE;
    rec656Config.errorCorrectEnable = FALSE;
    rec656Config.dataWidth          = CSL_CCDC_REC656_DATA_WIDTH_8BIT;
#endif
#ifdef YUV_MODE_INTERLACED
    setup.rec656Config              = &rec656Config;
#else
    setup.rec656Config              = NULL;
#endif
    setup.enable                    = FALSE;
    setup.sdramOutEnable            = FALSE;
    setup.bypassModuleIfNullConfig  = TRUE;
    setup.inDataConfig              = &inDataConfig;
    setup.syncConfig                = &gDRV_isifObj.pImgsIsifConfig->syncConfig;
    setup.miscConfig                = &miscConfig;
    setup.colPatConfig              = &gDRV_isifObj.pImgsIsifConfig->ccdcParams.colPat;
    setup.linerizationConfig        = &gDRV_isifObj.pImgsIsifConfig->ccdcParams.lin;
    setup.fmtInFrameConfig          = NULL;
    setup.fmtConfig                 = NULL;
    setup.cscConfig                 = NULL;
    setup.clampConfig               = &gDRV_isifObj.pImgsIsifConfig->ccdcParams.clamp;
    setup.dfcLscOffsetConfig        = &dfcLscOffsetConfig;
    setup.dfcConfig                 = &gDRV_isifObj.pImgsIsifConfig->ccdcParams.dfc;
    setup.lscConfig                 = &gDRV_isifObj.pImgsIsifConfig->ccdcParams.lsc;
    setup.gainOffsetConfig          = &gDRV_isifObj.pImgsIsifConfig->ccdcParams.gainOffset;
    setup.sdramOutSizeConfig        = NULL;
    setup.sdramOutConfig            = &sdrOutConfig;
    setup.dpcmConfig                = &dpcmConfig;

    setup.flashConfig               = NULL;
    setup.vdIntConfig               = &vdIntConfig;

    isifInConfig.dpcEnable      = FALSE;
    isifInConfig.dpcThreshold   = 0;

    pIpipeConfig = drvImgsFunc->imgsGetIpipeConfig(gDRV_isifObj.config.sensorMode, gDRV_isifObj.config.vnfDemoCfg);
    if (pIpipeConfig)
    {
        isifInConfig.dpcEnable      = pIpipeConfig->ipipeifParams.vpiIsifInDpcEnable;
        isifInConfig.dpcThreshold   = pIpipeConfig->ipipeifParams.vpiIsifInDpcThreshold;
    }

    isifInConfig.wenUseEnable   = FALSE;
    isifInConfig.vdPol          = CSL_IPIPEIF_HDPOLARITY_POSITIVE;
    isifInConfig.hdPol          = CSL_IPIPEIF_VDPOLARITY_POSITIVE;

    h3aConfig.pixelDecimationEnable = FALSE;
    h3aConfig.pixelDecimationRatio  = 16;
    h3aConfig.avgFilterEnable       = FALSE;
    h3aConfig.alignHsyncVsync       = FALSE;
    h3aConfig.initReszPosH          = 0;

    DRV_isifLscSetParams();

    status = CSL_ccdcHwSetup(&gCSL_ccdcHndl, &setup);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_ccdcHwSetup()\n");
        return status;
    }

    status = CSL_ipipeifSetVpifIsifInConfig(&gCSL_ipipeifHndl, &isifInConfig);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_ipipeifSetVpifIsifInConfig()\n");
        return status;
    }

    status = CSL_ipipeifSetH3aConfig(&gCSL_ipipeifHndl, &h3aConfig);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_ipipeifSetH3aConfig()\n");
        return status;
    }

    status = CSL_ipipeifSetInputSource1(&gCSL_ipipeifHndl, CSL_IPIPEIF_INPUT_SOURCE_PARALLEL_PORT_RAW);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("CSL_ipipeifSetInputSource1()\n");
        return status;
    }

    if (gDRV_isifObj.numBuf)
    {
        bufInit.numBuf = gDRV_isifObj.numBuf;
        bufInit.curBuf = 0;

        for (i = 0; i < gDRV_isifObj.numBuf; i++)
        {
            bufInit.bufAddr[i] = (Uint32)gDRV_isifObj.bufInfo[i].physAddr;
        }

        status = CSL_ccdcBufInit(&gCSL_ccdcHndl, &bufInit);
        if (status != OSA_SOK)
        {
            OSA_ERROR("CSL_ccdcBufInit()\n");
            return status;
        }
    }

    if (gDRV_isifObj.numLscBuf)
    {
        bufInit.numBuf = gDRV_isifObj.numLscBuf;
        bufInit.curBuf = 0;

        for (i = 0; i < gDRV_isifObj.numLscBuf; i++)
        {
            bufInit.bufAddr[i] = (Uint32)gDRV_isifObj.lscBufInfo[i].physAddr;
        }

        status = CSL_ccdcLscBufInit(&gCSL_ccdcHndl, &bufInit);
        if (status != OSA_SOK)
        {
            OSA_ERROR("CSL_ccdcBufInit()\n");
            return status;
        }

        CSL_ccdcLscBufSetGainTableSize(&gCSL_ccdcHndl, gDRV_isifObj.info.lscOffsetTableOffsetInBuf);
    }

    CSL_ccdcIntClear(&gCSL_ccdcHndl, DRV_ISIF_INT_VD0);
    CSL_ccdcIntClear(&gCSL_ccdcHndl, DRV_ISIF_INT_VD1);

    CSL_ccdcIntEnable(&gCSL_ccdcHndl, DRV_ISIF_INT_VD0, TRUE);
    //CSL_ccdcIntEnable(&gCSL_ccdcHndl, DRV_ISIF_INT_VD1, TRUE);

    DRV_isifUnlock();  //ANR - ISIF

    return status;
}

int DRV_isifImageTuneCmdExecuteLsc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
    CSL_CcdcLscConfig lscConfig;
    int retVal = IMAGE_TUNE_CmdExecuteCcdcLsc(cmdInfo, statusInfo);
    if (gDRV_isifObj.lscGainTableVirtAddr == NULL || gDRV_isifObj.lscOffsetTableVirtAddr == NULL)
    {
        return retVal;
    }

    if (retVal == OSA_SOK)
    {
        memset(&lscConfig, 0, sizeof(lscConfig));

        IMAGE_TUNE_GetIsifLscParams(&lscConfig);

        if (lscConfig.enable)
        {
            // copy to contigous memory buffer
            if (lscConfig.gainTableAddr)
            {
                memcpy(gDRV_isifObj.lscGainTableVirtAddr  , lscConfig.gainTableAddr, IMAGE_TUNE_LSC_BUFFER_MAX);
            }

            if (lscConfig.offsetTableAddr)
            {
                memcpy(gDRV_isifObj.lscOffsetTableVirtAddr, lscConfig.offsetTableAddr, IMAGE_TUNE_LSC_BUFFER_MAX);
            }
        }

        // need to give physical address to CSL
        lscConfig.gainTableAddr = gDRV_isifObj.lscGainTablePhysAddr;
        lscConfig.offsetTableAddr = gDRV_isifObj.lscOffsetTablePhysAddr;

        if (lscConfig.enable)
        {
            CSL_ccdcLscEnable(&gCSL_ccdcHndl, FALSE);
            OSA_waitMsecs(60); // wait for atleast one VD for LSC to be disabled
        }

        retVal = CSL_ccdcSetLscConfig(&gCSL_ccdcHndl, &lscConfig);
    }

    return retVal;
}


static int gDRV_isifTestQuit = 0;

#define DRV_ISIF_TEST_NUM_BUF   (3)

DRV_IsifInfo gDRV_isifInfo;

int DRV_isifTestRun()
{
    int status, bufId;
    OSA_BufInfo *pBufInfo;
    static int count=0;
    static char filename[80];

    status = DRV_isifGetOutBuf(&bufId, OSA_TIMEOUT_FOREVER);
    if (status!=OSA_SOK)
        OSA_ERROR("DRV_isifGetOutBuf()\n");

    if (status==OSA_SOK)
    {
        pBufInfo = DRV_isifGetOutBufInfo(bufId);

        if (pBufInfo!=NULL)
        {
            if (count%30==0)
            {
                OSA_printf("%d: buf ID:%d ADDR:%08x\n", count, bufId, (Uint32)pBufInfo->physAddr);
            }
#if 1
            if (count>1)
            {
                if (count%100==0)
                {
                    sprintf(filename, "IMG_%04d_%dx%d.RAW", count, gDRV_isifInfo.ddrOutDataOffsetH, gDRV_isifInfo.ddrOutDataHeight);
                    OSA_fileWriteFile(filename, (Uint8*)pBufInfo->virtAddr, gDRV_isifInfo.ddrOutDataOffsetH*gDRV_isifInfo.ddrOutDataHeight);
                }
            }
#endif
        }

        DRV_isifPutOutBuf(bufId);
    }

    count++;

    return status;
}

void DRV_isifTestSignalHandler(int signum)
{
    gDRV_isifTestQuit = 1;
}

int DRV_isifTestMain(int argc, char **argv)
{
    int status;
    DRV_ImgsConfig        imgsConfig;
    DRV_IsifConfig        isifConfig;

    OSA_attachSignalHandler(SIGINT, DRV_isifTestSignalHandler);

    status = DRV_init();
    if (status != OSA_SOK)
    {
        return status;
    }

    imgsConfig.sensorMode = DRV_IMGS_SENSOR_MODE_640x480;
    imgsConfig.binEnable  = FALSE;
    imgsConfig.fps        = 30;

    isifConfig.sensorMode = imgsConfig.sensorMode;
    isifConfig.alawEnable = FALSE;
    isifConfig.dpcmEnable = FALSE;
    isifConfig.numBuf     = DRV_ISIF_TEST_NUM_BUF;

    OSA_printf(" ISIF: Opening imager.\n");
    status = drvImgsFunc->imgsOpen(&imgsConfig);
    if (status != OSA_SOK)
    {
        goto error_exit;
    }

    OSA_printf("---ISIF: Opening ISIF.\n");
    status = DRV_isifOpen(&isifConfig);
    if (status != OSA_SOK)
    {
        goto isif_exit;
    }

    DRV_isifGetInfo(&gDRV_isifInfo);

    OSA_printf("Isif Info,\n");
    OSA_printf("Width         = %d\n", gDRV_isifInfo.ddrOutDataWidth);
    OSA_printf("Height        = %d\n", gDRV_isifInfo.ddrOutDataHeight);
    OSA_printf("Line Offset H = %d\n", gDRV_isifInfo.ddrOutDataOffsetH);
    OSA_printf("\n");

    DRV_isifOutEnable(TRUE);

    OSA_printf(" ISIF: Starting ISIF.\n");
    DRV_isifEnable(TRUE);

    OSA_printf(" ISIF: Starting imager.\n");
    drvImgsFunc->imgsEnable(TRUE);

    DRV_isifWaitInt(DRV_ISIF_INT_VD0, 2, 3000);

    OSA_printf(" ISIF: Running.\n");

    status = OSA_SOK;

    while (!gDRV_isifTestQuit && status==OSA_SOK)
    {
        status = DRV_isifTestRun();
    }

    OSA_printf(" ISIF: Stoping ISIF.\n");
    DRV_isifOutEnable(FALSE);
    DRV_isifWaitInt(DRV_ISIF_INT_VD0, 2, 3000);
    DRV_isifEnable(FALSE);

    OSA_printf(" ISIF: Stoping imager.\n");
    drvImgsFunc->imgsEnable(FALSE);

    OSA_printf(" ISIF: Closing ISIF.\n");
    DRV_isifClose();

isif_exit:

    OSA_printf(" ISIF: Closing imager.\n");
    drvImgsFunc->imgsClose();

error_exit:
    OSA_printf(" ISIF: Exiting.\n");
    DRV_exit();

    return status;
}

int DRV_isifLock()
{
    return OSA_mutexLock(&isifLock);
}

int DRV_isifUnlock()
{
    return OSA_mutexUnlock(&isifLock);
}

int DRV_isifGetBayerPhase(int sensorMode)
{
    DRV_ImgsIsifConfig   *isifConfig = drvImgsFunc->imgsGetIsifConfig(sensorMode);
    CSL_CcdcColPatConfig *colPat     = &isifConfig->ccdcParams.colPat;

    switch (colPat->colPat0[0][0])
    {
    case CSL_CCDC_COL_PAT_Gr:
        return 0;
        break;
    case CSL_CCDC_COL_PAT_R:
        return 1;
        break;
    case CSL_CCDC_COL_PAT_Gb:
        return 2;
        break;
    case CSL_CCDC_COL_PAT_B:
        return 3;
        break;
    default:
        OSA_ERROR("Undefined Bayer format\n");
        return 0;
        break;
    }
}
