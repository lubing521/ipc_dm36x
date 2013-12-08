
#include <drv_ipipe.h>
#include <osa_cmem.h>
#include <drv_imageTune.h>

#define DRV_IPIPE_BSC_NUM_BUF     (3)
#define DRV_IPIPE_HISTO_NUM_BUF   (3)
#define DRV_IPIPE_BOXCAR_NUM_BUF  (3)

#define DRV_IPIPE_BOXCAR_BUFFER_MAX (((2048/8)*(1536/8))*4*2)

typedef struct
{
    DRV_IpipeConfig config;
    OSA_MutexHndl lock;

    DRV_IpipeRszInfo         rszInfo[CSL_RSZ_CH_MAX];
    CSL_IpipeBscConfig       bscInfo;
    CSL_IpipeHistogramConfig histoInfo;

    DRV_IpipeBoxcarInfo      boxcarInfo;
    CSL_IpipeBoxcarConfig    boxcarConfig;

    int numBufRsz[CSL_RSZ_CH_MAX];
    int numBufBsc;
    int numBufHisto;
    int numBufBoxcar;

    OSA_BufInfo bufInfoRsz[CSL_RSZ_CH_MAX][CSL_BUF_NUM_MAX];
    OSA_BufInfo bufInfoBsc[CSL_BUF_NUM_MAX];
    OSA_BufInfo bufInfoHisto[CSL_BUF_NUM_MAX];
    OSA_BufInfo bufInfoBoxcar[CSL_BUF_NUM_MAX];

    DRV_ImgsModeConfig imgsModeInfo;

    DRV_ImgsIpipeConfig *pImgsIpipeConfig;

    Uint8 *bscBufBaseVirtAddr;
    Uint8 *bscBufBasePhysAddr;
    Uint8 *histoBufBaseVirtAddr;
    Uint8 *histoBufBasePhysAddr;
    Uint8 *boxcarBufBaseVirtAddr;
    Uint8 *boxcarBufBasePhysAddr;

    CSL_IpipeHwSetup        ipipeSetup;
    CSL_IpipeInFrameConfig  ipipeInFrameConfig;
    CSL_IpipeClkConfig      ipipeClkConfig;

    CSL_IpipeifClkConfig        ipipeifClk;
    CSL_IpipeifDpcmConfig       ipipeifDpcmConfig;
    CSL_IpipeifSdramInConfig    ipipeifDdrInConfig;
    CSL_IpipeifInSource2Config  ipipeifInSrcConfig;
    CSL_IpipeifVpifIsifInConfig ipipeifVpifIsifInConfig;

    CSL_RszHwSetup        rszSetup;
    CSL_RszInConfig       rszInConfig;
    CSL_RszInFrameConfig  rszInFrameConfig;
    CSL_RszClkConfig      rszClkConfig;
    CSL_RszMinMaxConfig   rszMinMaxConfig;
    CSL_RszOutConfig      rszOutConfig[CSL_RSZ_CH_MAX];

    Uint32* histResultBuf; /* Hist - ANR */
    Uint8 histStatus;
    OSA_MutexHndl statusLock;
} DRV_IpipeObj;

DRV_IpipeObj gDRV_ipipeObj;

int DRV_ipipeOpen(DRV_IpipeConfig *config)
{
    int i, rszId, bufSize, status;

    memset(&gDRV_ipipeObj, 0, sizeof(gDRV_ipipeObj));

    CSL_ipipeEnable(&gCSL_ipipeHndl, FALSE);

    status = OSA_mutexCreate(&gDRV_ipipeObj.lock);
    if (status != OSA_SOK)
    {
        OSA_ERROR("OSA_mutexCreate()\n");
        return status;
    }

    gDRV_ipipeObj.pImgsIpipeConfig = drvImgsFunc->imgsGetIpipeConfig(config->sensorMode, config->vnfDemoCfg);
    if (gDRV_ipipeObj.pImgsIpipeConfig == NULL)
    {
        OSA_ERROR("DRV_imgsGetIpipeConfig(%d %d)\n", config->sensorMode, config->vnfDemoCfg);
        return OSA_EFAIL;
    }

    config->rszValidDataStartOffset = OSA_align(config->rszValidDataStartOffset, 32);

    if (!config->rszOutConfig[DRV_IPIPE_RSZ_A].enable)
    {
        config->rszOutConfig[DRV_IPIPE_RSZ_A].numBuf = 0;
    }

    if (!config->rszOutConfig[DRV_IPIPE_RSZ_B].enable)
    {
        config->rszOutConfig[DRV_IPIPE_RSZ_B].numBuf = 0;
    }

    if (config->rszOutConfig[DRV_IPIPE_RSZ_A].numBuf > CSL_BUF_NUM_MAX)
    {
        OSA_ERROR("exceeded max buf limit of %d\n", CSL_BUF_NUM_MAX);
        return OSA_EFAIL;
    }

    if (config->rszOutConfig[DRV_IPIPE_RSZ_B].numBuf > CSL_BUF_NUM_MAX)
    {
        OSA_ERROR("exceeded max buf limit of %d\n", CSL_BUF_NUM_MAX);
        return OSA_EFAIL;
    }

    if (drvImgsFunc->imgsGetModeConfig(config->sensorMode) == NULL)
    {
        OSA_ERROR("DRV_imgsGetModeConfig()\n");
        return OSA_EFAIL;
    }

    memcpy(&gDRV_ipipeObj.imgsModeInfo, drvImgsFunc->imgsGetModeConfig(config->sensorMode), sizeof(gDRV_ipipeObj.imgsModeInfo) );
    memcpy(&gDRV_ipipeObj.config, config, sizeof(gDRV_ipipeObj.config));

    if (config->bscNumVectorsRow > 0 && config->bscNumVectorsCol > 0)
    {
        gDRV_ipipeObj.numBufBsc = DRV_IPIPE_BSC_NUM_BUF;
        gDRV_ipipeObj.bscBufBaseVirtAddr = OSA_cmemAlloc(DRV_IPIPE_BSC_BUFFER_MAX*gDRV_ipipeObj.numBufBsc, 32);

        if (gDRV_ipipeObj.bscBufBaseVirtAddr)
        {
            gDRV_ipipeObj.bscBufBasePhysAddr = OSA_cmemGetPhysAddr(gDRV_ipipeObj.bscBufBaseVirtAddr);
        }

        if (gDRV_ipipeObj.bscBufBaseVirtAddr == NULL || 
            gDRV_ipipeObj.bscBufBasePhysAddr == NULL)
        {
            OSA_ERROR("OSA_cmemAlloc()\n");
            goto error_exit;
        }

#ifdef DRV_BUF_CLEAR
        memset(gDRV_ipipeObj.bscBufBaseVirtAddr, 0xAA, DRV_IPIPE_BSC_BUFFER_MAX * gDRV_ipipeObj.numBufBsc);
#endif

        for (i = 0; i < gDRV_ipipeObj.numBufBsc; i++)
        {
            gDRV_ipipeObj.bufInfoBsc[i].virtAddr = gDRV_ipipeObj.bscBufBaseVirtAddr + i * DRV_IPIPE_BSC_BUFFER_MAX;
            gDRV_ipipeObj.bufInfoBsc[i].physAddr = gDRV_ipipeObj.bscBufBasePhysAddr + i * DRV_IPIPE_BSC_BUFFER_MAX;
            gDRV_ipipeObj.bufInfoBsc[i].size     = DRV_IPIPE_BSC_BUFFER_MAX;
        }
    }

    if (config->boxcarBlockSize != DRV_IPIPE_BOXCAR_BLOCK_SIZE_NONE)
    {
        DRV_vpssEnableBoxcar(TRUE); // when boxcar is enabled, LDC MUST be disabled

        gDRV_ipipeObj.numBufBoxcar = DRV_IPIPE_BOXCAR_NUM_BUF;
        gDRV_ipipeObj.boxcarBufBaseVirtAddr = OSA_cmemAlloc(DRV_IPIPE_BOXCAR_BUFFER_MAX*gDRV_ipipeObj.numBufBoxcar, 32);
        if (gDRV_ipipeObj.boxcarBufBaseVirtAddr)
        {
            gDRV_ipipeObj.boxcarBufBasePhysAddr = OSA_cmemGetPhysAddr(gDRV_ipipeObj.boxcarBufBaseVirtAddr);
        }

        if (gDRV_ipipeObj.boxcarBufBaseVirtAddr == NULL || 
            gDRV_ipipeObj.boxcarBufBasePhysAddr == NULL)
        {
            OSA_ERROR("OSA_cmemAlloc()\n");
            goto error_exit;
        }

#ifdef DRV_BUF_CLEAR
        memset(gDRV_ipipeObj.boxcarBufBaseVirtAddr, 0xAA, DRV_IPIPE_BOXCAR_BUFFER_MAX*gDRV_ipipeObj.numBufHisto);
#endif

        for (i = 0; i < gDRV_ipipeObj.numBufBoxcar; i++)
        {
            gDRV_ipipeObj.bufInfoBoxcar[i].virtAddr = gDRV_ipipeObj.boxcarBufBaseVirtAddr + i * DRV_IPIPE_BOXCAR_BUFFER_MAX;
            gDRV_ipipeObj.bufInfoBoxcar[i].physAddr = gDRV_ipipeObj.boxcarBufBasePhysAddr + i * DRV_IPIPE_BOXCAR_BUFFER_MAX;
            gDRV_ipipeObj.bufInfoBoxcar[i].size     = DRV_IPIPE_BOXCAR_BUFFER_MAX;
        }
    }


    if (config->histogramMode != DRV_IPIPE_HISTO_MODE_NONE)
    {
        gDRV_ipipeObj.numBufHisto = DRV_IPIPE_BSC_NUM_BUF;
        gDRV_ipipeObj.histoBufBaseVirtAddr = OSA_cmemAlloc(DRV_IPIPE_HISTO_BUFFER_MAX*gDRV_ipipeObj.numBufHisto, 32);
        if (gDRV_ipipeObj.histoBufBaseVirtAddr)
        {
            gDRV_ipipeObj.histoBufBasePhysAddr = OSA_cmemGetPhysAddr(gDRV_ipipeObj.histoBufBaseVirtAddr);
        }

        if (gDRV_ipipeObj.histoBufBaseVirtAddr == NULL || 
            gDRV_ipipeObj.histoBufBasePhysAddr == NULL)
        {
            OSA_ERROR("OSA_cmemAlloc()\n");
            goto error_exit;
        }

#ifdef DRV_BUF_CLEAR
        memset(gDRV_ipipeObj.histoBufBaseVirtAddr, 0xAA, DRV_IPIPE_HISTO_BUFFER_MAX * gDRV_ipipeObj.numBufHisto);
#endif

        for (i = 0; i < gDRV_ipipeObj.numBufHisto; i++)
        {
            gDRV_ipipeObj.bufInfoHisto[i].virtAddr = gDRV_ipipeObj.histoBufBaseVirtAddr + i * DRV_IPIPE_HISTO_BUFFER_MAX;
            gDRV_ipipeObj.bufInfoHisto[i].physAddr = gDRV_ipipeObj.histoBufBasePhysAddr + i * DRV_IPIPE_HISTO_BUFFER_MAX;
            gDRV_ipipeObj.bufInfoHisto[i].size     = DRV_IPIPE_HISTO_BUFFER_MAX;
        }
    }

    for (rszId = DRV_IPIPE_RSZ_A; rszId <= DRV_IPIPE_RSZ_B; rszId++)
    {
        gDRV_ipipeObj.rszInfo[rszId].enable = config->rszOutConfig[rszId].enable;
        if (!gDRV_ipipeObj.rszInfo[rszId].enable)
        {
            continue;
        }

        gDRV_ipipeObj.rszInfo[rszId].outFormat   = config->rszOutConfig[rszId].outFormat;
        gDRV_ipipeObj.rszInfo[rszId].width       = config->rszOutConfig[rszId].width;
        gDRV_ipipeObj.rszInfo[rszId].height      = config->rszOutConfig[rszId].height;
        gDRV_ipipeObj.rszInfo[rszId].dmaInterval = config->rszOutConfig[rszId].dmaInterval;
#if 0
        if (config->sensorMode & DRV_IMGS_SENSOR_MODE_PIXEL_PAD)
        {
            /* Done here to keep the same Field of View */
            gDRV_ipipeObj.rszInfo[rszId].width  += DRV_IMGS_VNF_PAD_VALUE;
            gDRV_ipipeObj.rszInfo[rszId].height += DRV_IMGS_VNF_PAD_VALUE;
        }
#endif
        DRV_imgWidthHeightPad(config->sensorMode, &gDRV_ipipeObj.rszInfo[rszId].width, &gDRV_ipipeObj.rszInfo[rszId].height);

        gDRV_ipipeObj.rszInfo[rszId].offsetH = OSA_align(gDRV_ipipeObj.rszInfo[rszId].width, 32);
        gDRV_ipipeObj.rszInfo[rszId].offsetV = gDRV_ipipeObj.rszInfo[rszId].height;//+16;

        bufSize = gDRV_ipipeObj.rszInfo[rszId].offsetH * gDRV_ipipeObj.rszInfo[rszId].offsetV + config->rszValidDataStartOffset;

        if (gDRV_ipipeObj.rszInfo[rszId].outFormat == DRV_DATA_FORMAT_YUV422)
        {
            bufSize = bufSize * 2;
        }
        else
        {
            bufSize = (bufSize * 3) / 2;
        }

        gDRV_ipipeObj.numBufRsz[rszId] = config->rszOutConfig[rszId].numBuf;

        for (i = 0; i < gDRV_ipipeObj.numBufRsz[rszId]; i++)
        {
            gDRV_ipipeObj.bufInfoRsz[rszId][i].virtAddr = OSA_cmemAlloc(bufSize, 32);
            if (gDRV_ipipeObj.bufInfoRsz[rszId][i].virtAddr == NULL)
            {
                OSA_ERROR("OSA_cmemAlloc()\n");
                goto error_exit;
            }

#ifdef DRV_BUF_CLEAR
            memset(gDRV_ipipeObj.bufInfoRsz[rszId][i].virtAddr, 0x80, bufSize);
#endif

            gDRV_ipipeObj.bufInfoRsz[rszId][i].physAddr = OSA_cmemGetPhysAddr((Uint8*)gDRV_ipipeObj.bufInfoRsz[rszId][i].virtAddr);
            if (gDRV_ipipeObj.bufInfoRsz[rszId][i].physAddr == NULL)
            {
                OSA_ERROR("OSA_cmemGetPhysAddr()\n");
                goto error_exit;
            }
            gDRV_ipipeObj.bufInfoRsz[rszId][i].size  = bufSize;
        }
    }

    status = DRV_ipipeSetParams();
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_ipipeSetParams()\n");
        goto error_exit;
    }

    /* ANR-Hist */
    if (gDRV_ipipeObj.histResultBuf != NULL)
    {
        OSA_cmemFree((Uint8 *)gDRV_ipipeObj.histResultBuf);
    }

    gDRV_ipipeObj.histResultBuf = (Uint32*)OSA_cmemAlloc(CSL_IPIPE_HISTOGRAM_RESULTS_MAX_ENTRIES * 4 , 32);
    if (gDRV_ipipeObj.histResultBuf == NULL)
    {
        OSA_ERROR("OSA_cmemAlloc()\n");
        goto error_exit;
    }

    return OSA_SOK;

error_exit:
    for (rszId = DRV_IPIPE_RSZ_A; rszId <= DRV_IPIPE_RSZ_B; rszId++)
    {
        for (i = 0; i < gDRV_ipipeObj.numBufRsz[rszId]; i++)
        {
            if (gDRV_ipipeObj.bufInfoRsz[rszId][i].virtAddr)
            {
                OSA_cmemFree((Uint8*)gDRV_ipipeObj.bufInfoRsz[rszId][i].virtAddr);
            }
        }
    }

    if (gDRV_ipipeObj.bscBufBaseVirtAddr)
    {
        OSA_cmemFree((Uint8*)gDRV_ipipeObj.bscBufBaseVirtAddr);
    }

    if (gDRV_ipipeObj.histoBufBaseVirtAddr)
    {
        OSA_cmemFree((Uint8*)gDRV_ipipeObj.histoBufBaseVirtAddr);
    }

    if (gDRV_ipipeObj.boxcarBufBaseVirtAddr)
    {
        OSA_cmemFree((Uint8*)gDRV_ipipeObj.boxcarBufBaseVirtAddr);
    }

    /* ANR-Hist  */
    if (gDRV_ipipeObj.histResultBuf)
    {
        OSA_cmemFree((Uint8 *)gDRV_ipipeObj.histResultBuf);
    }

    return OSA_EFAIL;
}

int DRV_ipipeClose()
{
    int i, rszId;

    OSA_mutexDelete(&gDRV_ipipeObj.lock);

    DRV_ipipeEnable(FALSE, FALSE, FALSE, FALSE);

    for (rszId = DRV_IPIPE_RSZ_A; rszId <= DRV_IPIPE_RSZ_B; rszId++)
    {
        for (i = 0; i < gDRV_ipipeObj.numBufRsz[rszId]; i++)
        {
            if (gDRV_ipipeObj.bufInfoRsz[rszId][i].virtAddr)
                OSA_cmemFree((Uint8*)gDRV_ipipeObj.bufInfoRsz[rszId][i].virtAddr);
        }
    }

    if (gDRV_ipipeObj.bscBufBaseVirtAddr)
        OSA_cmemFree((Uint8*)gDRV_ipipeObj.bscBufBaseVirtAddr);

    if (gDRV_ipipeObj.histoBufBaseVirtAddr)
        OSA_cmemFree((Uint8*)gDRV_ipipeObj.histoBufBaseVirtAddr);

    if (gDRV_ipipeObj.boxcarBufBaseVirtAddr)
    {
        OSA_cmemFree((Uint8*)gDRV_ipipeObj.boxcarBufBaseVirtAddr);
        DRV_vpssEnableBoxcar(FALSE); // when boxcar is disabled, LDC can be enabled
    }

    /* ANR-Hist */
    if (gDRV_ipipeObj.histResultBuf)
        OSA_cmemFree((Uint8 *)gDRV_ipipeObj.histResultBuf);

    return OSA_SOK;
}

int DRV_ipipeEnable(Bool rszOutEnable, Bool bscEnable, Bool histoEnable, Bool boxcarEnable)
{
    int i;

    if (gDRV_ipipeObj.config.bscNumVectorsRow == 0 ||
        gDRV_ipipeObj.config.bscNumVectorsCol == 0)
    {
        CSL_ipipeIntEnable(&gCSL_ipipeHndl, CSL_IPIPE_INT_TYPE_BSC, FALSE);
        CSL_ipipeBscEnable(&gCSL_ipipeHndl, FALSE);
        CSL_ipipeBscBufSwitchEnable(&gCSL_ipipeHndl, FALSE);
    }
    else
    {
        CSL_ipipeIntClear(&gCSL_ipipeHndl, CSL_IPIPE_INT_TYPE_BSC);
        CSL_ipipeIntEnable(&gCSL_ipipeHndl, CSL_IPIPE_INT_TYPE_BSC, bscEnable);
        CSL_ipipeBscEnable(&gCSL_ipipeHndl, bscEnable);
        CSL_ipipeBscBufSwitchEnable(&gCSL_ipipeHndl, bscEnable);
    }

    if (gDRV_ipipeObj.config.histogramMode > DRV_IPIPE_HISTO_MODE_2x2_REGION)
    {
        CSL_ipipeHistogramEnable(&gCSL_ipipeHndl, FALSE);
    }
    else
    {
        CSL_ipipeHistogramEnable(&gCSL_ipipeHndl, histoEnable);
    }

    if (gDRV_ipipeObj.config.boxcarBlockSize == DRV_IPIPE_BOXCAR_BLOCK_SIZE_NONE)
    {
        CSL_ipipeBoxcarEnable(&gCSL_ipipeHndl, FALSE);
    }
    else
    {
        CSL_ipipeIntClear(&gCSL_ipipeHndl, CSL_IPIPE_INT_TYPE_BOXCAR);
        CSL_ipipeIntEnable(&gCSL_ipipeHndl, CSL_IPIPE_INT_TYPE_BOXCAR, boxcarEnable);
        CSL_ipipeBoxcarEnable(&gCSL_ipipeHndl, boxcarEnable);
        CSL_ipipeBoxcarBufSwitchEnable(&gCSL_ipipeHndl, boxcarEnable);
    }

    for (i=0; i<CSL_RSZ_CH_MAX; i++)
    {
        if (rszOutEnable)
        {
            CSL_rszOutEnable(&gCSL_rszHndl, i, gDRV_ipipeObj.config.rszOutConfig[i].enable);
            CSL_rszBufSwitchEnable(&gCSL_rszHndl, i, gDRV_ipipeObj.config.rszOutConfig[i].enable);
        }
        else
        {
            CSL_rszOutEnable(&gCSL_rszHndl, i, FALSE);
            CSL_rszBufSwitchEnable(&gCSL_rszHndl, i, FALSE);
        }

    }

    OSA_waitMsecs(100);

    CSL_rszEnable(&gCSL_rszHndl, rszOutEnable);

    CSL_ipipeEnable(&gCSL_ipipeHndl, rszOutEnable);

    return OSA_SOK;
}

int DRV_ipipeGetInfo(DRV_IpipeInfo *info)
{
    memcpy(&info->rszInfo, &gDRV_ipipeObj.rszInfo, sizeof(info->rszInfo));
    memcpy(&info->bscInfo, &gDRV_ipipeObj.bscInfo, sizeof(info->bscInfo));
    memcpy(&info->histoInfo, &gDRV_ipipeObj.histoInfo, sizeof(info->histoInfo));
    memcpy(&info->boxcarInfo, &gDRV_ipipeObj.boxcarInfo, sizeof(info->boxcarInfo));

    return OSA_SOK;
}

int DRV_ipipeGetRszBuf(int rszId, int *bufId, int timeout)
{
    CSL_BufInfo buf;
    int status;

    if (rszId<0 || rszId >= CSL_RSZ_CH_MAX)
        return OSA_EFAIL;

    *bufId = -1;

    status = CSL_rszBufGetFull(&gCSL_rszHndl, rszId, &buf, 1, timeout);
    if (status!=OSA_SOK)
        return status;

    if (buf.id < 0 || buf.id >= gDRV_ipipeObj.numBufRsz[rszId] )
        return OSA_EFAIL;

    gDRV_ipipeObj.bufInfoRsz[rszId][buf.id].timestamp = buf.timestamp;
    gDRV_ipipeObj.bufInfoRsz[rszId][buf.id].count = buf.count;
    gDRV_ipipeObj.bufInfoRsz[rszId][buf.id].width = buf.width;
    gDRV_ipipeObj.bufInfoRsz[rszId][buf.id].height= buf.height;

    *bufId = buf.id;

    return OSA_SOK;
}

int DRV_ipipePutRszBuf(int rszId, int bufId)
{
    CSL_BufInfo buf;
    int status;

    if (rszId<0 || rszId >= CSL_RSZ_CH_MAX)
        return OSA_EFAIL;

    if (bufId < 0 || bufId >= gDRV_ipipeObj.numBufRsz[rszId])
        return OSA_EFAIL;

    buf.id = bufId;

    status = CSL_rszBufPutEmpty(&gCSL_rszHndl, rszId, &buf);

    return status;
}

OSA_BufInfo *DRV_ipipeGetRszBufInfo(int rszId, int bufId)
{
    if (rszId<0 || rszId >= CSL_RSZ_CH_MAX)
        return NULL;

    if (bufId < 0 || bufId >= gDRV_ipipeObj.numBufRsz[rszId])
        return NULL;

    return &gDRV_ipipeObj.bufInfoRsz[rszId][bufId];
}

int DRV_ipipeGetBscBuf(int *bufId, int timeout)
{
    CSL_BufInfo buf;
    int status;

    *bufId = -1;

    status = CSL_ipipeBscBufGetFull(&gCSL_ipipeHndl, &buf, 1, timeout);
    if (status!=OSA_SOK)
        return status;

    if (buf.id < 0 || buf.id >= gDRV_ipipeObj.numBufBsc )
        return OSA_EFAIL;

    gDRV_ipipeObj.bufInfoBsc[buf.id].timestamp = buf.timestamp;
    gDRV_ipipeObj.bufInfoBsc[buf.id].count = buf.count;

    *bufId = buf.id;

    return OSA_SOK;
}

int DRV_ipipePutBscBuf(int bufId)
{
    CSL_BufInfo buf;
    int status;

    if (bufId < 0 || bufId >= gDRV_ipipeObj.numBufBsc)
        return OSA_EFAIL;

    buf.id = bufId;

    status = CSL_ipipeBscBufPutEmpty(&gCSL_ipipeHndl, &buf);

    return status;
}

OSA_BufInfo *DRV_ipipeGetBscBufInfo(int bufId)
{
    if (bufId < 0 || bufId >= gDRV_ipipeObj.numBufBsc)
        return NULL;

    return &gDRV_ipipeObj.bufInfoBsc[bufId];
}

int DRV_ipipeGetBoxcarBuf(int *bufId, int timeout)
{
    CSL_BufInfo buf;
    int status;

    *bufId = -1;

    status = CSL_ipipeBoxcarBufGetFull(&gCSL_ipipeHndl, &buf, 1, timeout);
    if (status!=OSA_SOK)
        return status;

    if (buf.id < 0 || buf.id >= gDRV_ipipeObj.numBufBoxcar )
        return OSA_EFAIL;

    gDRV_ipipeObj.bufInfoBoxcar[buf.id].timestamp = buf.timestamp;
    gDRV_ipipeObj.bufInfoBoxcar[buf.id].count = buf.count;

    *bufId = buf.id;

    return OSA_SOK;
}

int DRV_ipipePutBoxcarBuf(int bufId)
{
    CSL_BufInfo buf;
    int status;

    if (bufId < 0 || bufId >= gDRV_ipipeObj.numBufBoxcar)
        return OSA_EFAIL;

    buf.id = bufId;

    status = CSL_ipipeBoxcarBufPutEmpty(&gCSL_ipipeHndl, &buf);

    return status;
}

OSA_BufInfo *DRV_ipipeGetBoxcarBufInfo(int bufId)
{
    if (bufId < 0 || bufId >= gDRV_ipipeObj.numBufBoxcar)
        return NULL;

    return &gDRV_ipipeObj.bufInfoBoxcar[bufId];
}


int DRV_ipipeLock()
{
    return OSA_mutexLock(&gDRV_ipipeObj.lock);
}

int DRV_ipipeUnlock()
{
    return OSA_mutexUnlock(&gDRV_ipipeObj.lock);
}

int DRV_ipipeSetWb(DRV_IpipeWb *wbData)
{
    if (wbData==NULL)
        return OSA_EFAIL;

    DRV_ipipeLock();

    gCSL_vpssHndl.ipipeRegs->WB2_WGN_R  = wbData->gainR;
    gCSL_vpssHndl.ipipeRegs->WB2_WGN_GR = wbData->gainGr;
    gCSL_vpssHndl.ipipeRegs->WB2_WGN_GB = wbData->gainGb;
    gCSL_vpssHndl.ipipeRegs->WB2_WGN_B  = wbData->gainB;

    DRV_ipipeUnlock();

    IMAGE_TUNE_SetIpipeWb(wbData->gainR, wbData->gainGr, wbData->gainGb, wbData->gainB);

    return OSA_SOK;
}

int DRV_ipipeSetRgb2Rgb(CSL_IpipeRgb2RgbConfig *rgb2rgb)
{
    int status = CSL_ipipeSetRgb2Rgb1Config(&gCSL_ipipeHndl, rgb2rgb);

    IMAGE_TUNE_SetIpipeRgb2Rgb1(rgb2rgb);

    return status;
}

int DRV_ipipeSetRgb2Rgb2(CSL_IpipeRgb2RgbConfig *rgb2rgb)
{
    int status = CSL_ipipeSetRgb2Rgb2Config(&gCSL_ipipeHndl, rgb2rgb);

    IMAGE_TUNE_SetIpipeRgb2Rgb2(rgb2rgb);

    return status;
}

int DRV_ipipeBscSetParams(void)
{
    int hSkip, vSkip, compHeight, compWidth, status, i, shift, startX, startY, inWidth, inHeight;
    CSL_BufInit bufInit;

    if (gDRV_ipipeObj.config.bscNumVectorsRow == 0||
        gDRV_ipipeObj.config.bscNumVectorsCol == 0)
    {
        gDRV_ipipeObj.bscInfo.enable = FALSE;
        return OSA_SOK;
    }

    gDRV_ipipeObj.bscInfo.enable          = FALSE;

    gDRV_ipipeObj.bscInfo.oneShotEnable   = FALSE;

    gDRV_ipipeObj.bscInfo.colorSelect    = CSL_IPIPE_BSC_COLOUR_SELECT_Y;

    shift    = 0;
    startX   = 0;
    startY   = 0;
    inWidth  = gDRV_ipipeObj.ipipeInFrameConfig.inWidth  - startX*2;
    inHeight = gDRV_ipipeObj.ipipeInFrameConfig.inHeight - startY*2;


    if (inWidth <= 960)
    {
        hSkip = 2;
        vSkip = 2;
    }
    else if (inWidth <= 1536)
    {
        hSkip = 4;
        vSkip = 4;
    }
    else
    {
        hSkip = 8;
        vSkip = 8;
    }

    compWidth  = inWidth/hSkip;
    compHeight = inHeight/vSkip;
    compWidth  = OSA_floor(compWidth, gDRV_ipipeObj.config.bscNumVectorsRow);
    compHeight = OSA_floor(compHeight, gDRV_ipipeObj.config.bscNumVectorsCol);

    gDRV_ipipeObj.bscInfo.rowEnable      = TRUE;
    gDRV_ipipeObj.bscInfo.rowNumVectors  = gDRV_ipipeObj.config.bscNumVectorsRow;
    gDRV_ipipeObj.bscInfo.rowInShift     = shift;
    gDRV_ipipeObj.bscInfo.rowStartV      = startY;
    gDRV_ipipeObj.bscInfo.rowNumV        = compHeight;
    gDRV_ipipeObj.bscInfo.rowSkipV       = vSkip;
    gDRV_ipipeObj.bscInfo.rowStartH      = startX;
    gDRV_ipipeObj.bscInfo.rowNumH        = compWidth/gDRV_ipipeObj.config.bscNumVectorsRow;
    gDRV_ipipeObj.bscInfo.rowSkipH       = hSkip;

    gDRV_ipipeObj.bscInfo.colEnable      = TRUE;
    gDRV_ipipeObj.bscInfo.colNumVectors  = gDRV_ipipeObj.config.bscNumVectorsCol;
    gDRV_ipipeObj.bscInfo.colInShift     = shift;
    gDRV_ipipeObj.bscInfo.colStartV      = startY;
    gDRV_ipipeObj.bscInfo.colNumV        = compHeight/gDRV_ipipeObj.config.bscNumVectorsCol;
    gDRV_ipipeObj.bscInfo.colSkipV       = vSkip;
    gDRV_ipipeObj.bscInfo.colStartH      = startX;
    gDRV_ipipeObj.bscInfo.colNumH        = compWidth;
    gDRV_ipipeObj.bscInfo.colSkipH       = hSkip;

#ifdef DRV_SHOW_INFO
    OSA_printf(" \n");
    OSA_printf(" IPIPE BSC Info,\n");
    OSA_printf(" rowNumVectors  = %d,\n", gDRV_ipipeObj.bscInfo.rowNumVectors);
    OSA_printf(" rowNumV        = %d,\n", gDRV_ipipeObj.bscInfo.rowNumV);
    OSA_printf(" rowSkipV       = %d,\n", gDRV_ipipeObj.bscInfo.rowSkipV);
    OSA_printf(" rowNumH        = %d,\n", gDRV_ipipeObj.bscInfo.rowNumH);
    OSA_printf(" rowSkipH       = %d,\n", gDRV_ipipeObj.bscInfo.rowSkipH);
    OSA_printf(" colNumVectors  = %d,\n", gDRV_ipipeObj.bscInfo.colNumVectors);
    OSA_printf(" colNumV        = %d,\n", gDRV_ipipeObj.bscInfo.colNumV);
    OSA_printf(" colSkipV       = %d,\n", gDRV_ipipeObj.bscInfo.colSkipV);
    OSA_printf(" colNumH        = %d,\n", gDRV_ipipeObj.bscInfo.colNumH);
    OSA_printf(" colSkipH       = %d,\n", gDRV_ipipeObj.bscInfo.colSkipH);
    OSA_printf(" \n");
#endif


    bufInit.numBuf = gDRV_ipipeObj.numBufBsc;
    bufInit.curBuf = 0;
    for (i=0; i<bufInit.numBuf; i++)
    {

        bufInit.bufAddr[i] = (Uint32)gDRV_ipipeObj.bufInfoBsc[i].physAddr;
    }

    status = CSL_ipipeBscBufInit(&gCSL_ipipeHndl, &bufInit);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("CSL_ipipeBscBufInit()\n");
        return status;
    }

    return status;
}

int DRV_ipipeBscUpdateParams(DRV_IpipeBscConfig *pBscConfig)
{
    gDRV_ipipeObj.bscInfo.enable        = TRUE;
    gDRV_ipipeObj.bscInfo.oneShotEnable = FALSE;
    gDRV_ipipeObj.bscInfo.colorSelect   = CSL_IPIPE_BSC_COLOUR_SELECT_Y;

    gDRV_ipipeObj.bscInfo.rowEnable     = TRUE;
    gDRV_ipipeObj.bscInfo.rowNumVectors = pBscConfig->bsc_row_vct;
    gDRV_ipipeObj.bscInfo.rowInShift    = pBscConfig->bsc_row_shf;
    gDRV_ipipeObj.bscInfo.rowStartV     = pBscConfig->bsc_row_vpos;
    gDRV_ipipeObj.bscInfo.rowNumV       = pBscConfig->bsc_row_vnum;
    gDRV_ipipeObj.bscInfo.rowSkipV      = pBscConfig->bsc_row_vskip;
    gDRV_ipipeObj.bscInfo.rowStartH     = pBscConfig->bsc_row_hpos;
    gDRV_ipipeObj.bscInfo.rowNumH       = pBscConfig->bsc_row_hnum;
    gDRV_ipipeObj.bscInfo.rowSkipH      = pBscConfig->bsc_row_hskip;

    gDRV_ipipeObj.bscInfo.colEnable     = TRUE;
    gDRV_ipipeObj.bscInfo.colNumVectors = pBscConfig->bsc_col_vct;
    gDRV_ipipeObj.bscInfo.colInShift    = pBscConfig->bsc_col_shf;
    gDRV_ipipeObj.bscInfo.colStartV     = pBscConfig->bsc_col_vpos;
    gDRV_ipipeObj.bscInfo.colNumV       = pBscConfig->bsc_col_vnum;
    gDRV_ipipeObj.bscInfo.colSkipV      = pBscConfig->bsc_col_vskip;
    gDRV_ipipeObj.bscInfo.colStartH     = pBscConfig->bsc_col_hpos;
    gDRV_ipipeObj.bscInfo.colNumH       = pBscConfig->bsc_col_hnum;
    gDRV_ipipeObj.bscInfo.colSkipH      = pBscConfig->bsc_col_hskip;

    if (CSL_ipipeSetBscConfig(&gCSL_ipipeHndl,&gDRV_ipipeObj.bscInfo) != OSA_SOK)
    {
        return OSA_EFAIL;
    }

    return OSA_SOK;
}

int DRV_ipipeHistoSetParams(void)
{
    int i, numRegions, inWidth, inHeight, numBins, numCols;

    if (gDRV_ipipeObj.config.histogramMode==DRV_IPIPE_HISTO_MODE_NONE)
    {
        gDRV_ipipeObj.histoInfo.enable          = FALSE;
        return OSA_SOK;
    }

    inWidth  = gDRV_ipipeObj.ipipeInFrameConfig.inWidth;
    inHeight = gDRV_ipipeObj.ipipeInFrameConfig.inHeight;

    gDRV_ipipeObj.histoInfo.enable        = FALSE;

    gDRV_ipipeObj.histoInfo.oneShotEnable = FALSE;

    gDRV_ipipeObj.histoInfo.source          = CSL_IPIPE_HISTOGRAM_SOURCE_RGB2YUV; //Gang: for GBCE
    gDRV_ipipeObj.histoInfo.greenType       = CSL_IPIPE_HISTOGRAM_GREEN_SAMPLING_GAVG;
    gDRV_ipipeObj.histoInfo.inShift         = 4; //Gang: 8bit Y is on 8msbs for GBCE

    for (i=0; i<4; i++)
    {
        gDRV_ipipeObj.histoInfo.regionEnable[i] = FALSE;
        gDRV_ipipeObj.histoInfo.colorEnable[i]  = FALSE; //Gang: for GBCE
        gDRV_ipipeObj.histoInfo.matrixCoeff[i]  = 0;
        gDRV_ipipeObj.histoInfo.regionConfig[i].startX = 0;
        gDRV_ipipeObj.histoInfo.regionConfig[i].startY = 0;
        gDRV_ipipeObj.histoInfo.regionConfig[i].width  = 0;
        gDRV_ipipeObj.histoInfo.regionConfig[i].height = 0;
    }

    numCols = 1; //Gang: for GBCE
    gDRV_ipipeObj.histoInfo.colorEnable[3] = TRUE; //Gang: for GBCE

    switch (gDRV_ipipeObj.config.histogramMode)
    {
    
    case DRV_IPIPE_HISTO_MODE_1_REGION:

        numRegions = 1;

        gDRV_ipipeObj.histoInfo.regionConfig[0].startX = 0;
        gDRV_ipipeObj.histoInfo.regionConfig[0].startY = 0;
        gDRV_ipipeObj.histoInfo.regionConfig[0].width  = inWidth;
        gDRV_ipipeObj.histoInfo.regionConfig[0].height = inHeight;

        break;

    case DRV_IPIPE_HISTO_MODE_2_REGION:

        numRegions = 2;

        gDRV_ipipeObj.histoInfo.regionConfig[0].startX = 0;
        gDRV_ipipeObj.histoInfo.regionConfig[0].startY = 0;
        gDRV_ipipeObj.histoInfo.regionConfig[0].width  = inWidth;
        gDRV_ipipeObj.histoInfo.regionConfig[0].height = inHeight;

        gDRV_ipipeObj.histoInfo.regionConfig[1].width  = inWidth/2;
        gDRV_ipipeObj.histoInfo.regionConfig[1].height = inHeight/2;
        gDRV_ipipeObj.histoInfo.regionConfig[1].startX = (inWidth - gDRV_ipipeObj.histoInfo.regionConfig[1].width)/2;
        gDRV_ipipeObj.histoInfo.regionConfig[1].startY = (inHeight - gDRV_ipipeObj.histoInfo.regionConfig[1].height)/2;

        break;

    case DRV_IPIPE_HISTO_MODE_2x2_REGION:
        numRegions = 4;

        gDRV_ipipeObj.histoInfo.regionConfig[0].startX = 0;
        gDRV_ipipeObj.histoInfo.regionConfig[0].startY = 0;
        gDRV_ipipeObj.histoInfo.regionConfig[0].width  = inWidth/2;
        gDRV_ipipeObj.histoInfo.regionConfig[0].height = inHeight/2;

        gDRV_ipipeObj.histoInfo.regionConfig[1].startX = inWidth/2;
        gDRV_ipipeObj.histoInfo.regionConfig[1].startY = 0;
        gDRV_ipipeObj.histoInfo.regionConfig[1].width  = inWidth/2;
        gDRV_ipipeObj.histoInfo.regionConfig[1].height = inHeight/2;

        gDRV_ipipeObj.histoInfo.regionConfig[2].startX = 0;              /* Changed the num Region -1 ANR */
        gDRV_ipipeObj.histoInfo.regionConfig[2].startY = inHeight/2;
        gDRV_ipipeObj.histoInfo.regionConfig[2].width  = inWidth/2;
        gDRV_ipipeObj.histoInfo.regionConfig[2].height = inHeight/2;

        gDRV_ipipeObj.histoInfo.regionConfig[3].startX = inWidth/2;  /* Changed the num Region -1 ANR */
        gDRV_ipipeObj.histoInfo.regionConfig[3].startY = inHeight/2;
        gDRV_ipipeObj.histoInfo.regionConfig[3].width  = inWidth/2;
        gDRV_ipipeObj.histoInfo.regionConfig[3].height = inHeight/2;

        break;
    default:
        return OSA_EFAIL;
        break;
    }

    for (i=0; i<numRegions; i++)
    {
        gDRV_ipipeObj.histoInfo.regionEnable[i] = TRUE;

        gDRV_ipipeObj.histoInfo.regionConfig[i].startX = OSA_floor(gDRV_ipipeObj.histoInfo.regionConfig[i].startX, 2);
        gDRV_ipipeObj.histoInfo.regionConfig[i].startY = OSA_floor(gDRV_ipipeObj.histoInfo.regionConfig[i].startY, 2);
        gDRV_ipipeObj.histoInfo.regionConfig[i].width  = OSA_floor(gDRV_ipipeObj.histoInfo.regionConfig[i].width , 2);
        gDRV_ipipeObj.histoInfo.regionConfig[i].height = OSA_floor(gDRV_ipipeObj.histoInfo.regionConfig[i].height, 2);
    }

    numBins = 512/(numRegions*numCols);

    if (numBins < 64)
        gDRV_ipipeObj.histoInfo.numBins         = CSL_IPIPE_HISTOGRAM_BIN_NUM_32;
    else
        if (numBins < 128)
        gDRV_ipipeObj.histoInfo.numBins         = CSL_IPIPE_HISTOGRAM_BIN_NUM_64;
    else
        if (numBins < 256)
        gDRV_ipipeObj.histoInfo.numBins         = CSL_IPIPE_HISTOGRAM_BIN_NUM_128;
    else
        gDRV_ipipeObj.histoInfo.numBins         = CSL_IPIPE_HISTOGRAM_BIN_NUM_256;

    gDRV_ipipeObj.histoInfo.outTableSel = CSL_IPIPE_HISTOGRAM_OUT_TABLE_0_1;
    gDRV_ipipeObj.histoInfo.clearOnVd   = TRUE;   //FALSE ANR - Hist

    return OSA_SOK;
}

int DRV_ipipeRszSetParams()
{
    int status, i, k;
    CSL_RszOutConfig *pOutConfig;
    CSL_BufInit bufInit[CSL_RSZ_CH_MAX];
    CSL_RszBufConfig bufConfig[CSL_RSZ_CH_MAX];

    status = CSL_rszHwReset(&gCSL_rszHndl);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_rszHwReset()\n");
        return status;
    }

    gDRV_ipipeObj.rszSetup.enable         = TRUE;
    gDRV_ipipeObj.rszSetup.oneShotEnable  = FALSE;
    gDRV_ipipeObj.rszSetup.inConfig       = &gDRV_ipipeObj.rszInConfig;
    gDRV_ipipeObj.rszSetup.inFrameConfig  = &gDRV_ipipeObj.rszInFrameConfig;
    gDRV_ipipeObj.rszSetup.clkConfig      = &gDRV_ipipeObj.rszClkConfig;
    gDRV_ipipeObj.rszSetup.minMaxConfig   = &gDRV_ipipeObj.rszMinMaxConfig;
    gDRV_ipipeObj.rszSetup.rszAConfig     = NULL;
    gDRV_ipipeObj.rszSetup.rszBConfig     = NULL;

    if (gDRV_ipipeObj.config.rszOutConfig[0].enable)
    {
        gDRV_ipipeObj.rszSetup.rszAConfig     = &gDRV_ipipeObj.rszOutConfig[0];
    }

    if (gDRV_ipipeObj.config.rszOutConfig[1].enable)
    {
        gDRV_ipipeObj.rszSetup.rszBConfig     = &gDRV_ipipeObj.rszOutConfig[1];
    }

    gDRV_ipipeObj.rszInConfig.wenUseEnable       = FALSE;
    gDRV_ipipeObj.rszInConfig.passThruEnable     = FALSE;
    gDRV_ipipeObj.rszInConfig.passThruInFormat   = 0;

    if (SensorIsYuvMode() > 0)
        gDRV_ipipeObj.rszInConfig.inputDataPath  = CSL_RSZ_INPUT_DATA_PATH_IPIPEIF;
    else
        gDRV_ipipeObj.rszInConfig.inputDataPath  = CSL_RSZ_INPUT_DATA_PATH_IPIPE;

    gDRV_ipipeObj.rszInConfig.chormaFormatType   = CSL_RSZ_CHROMA_FORMAT_422;
    gDRV_ipipeObj.rszInConfig.yuv420Color        = 0;
    gDRV_ipipeObj.rszInConfig.chormaPos          = CSL_RSZ_CHROMA_POS_SAME_AS_Y;
    gDRV_ipipeObj.rszInConfig.chormaFlip         = FALSE;

    gDRV_ipipeObj.rszInFrameConfig.inStartX     = 0;
    gDRV_ipipeObj.rszInFrameConfig.inStartY     = 0;
    gDRV_ipipeObj.rszInFrameConfig.inWidth      = gDRV_ipipeObj.ipipeInFrameConfig.inWidth ;
    gDRV_ipipeObj.rszInFrameConfig.inHeight     = gDRV_ipipeObj.ipipeInFrameConfig.inHeight;

    gDRV_ipipeObj.rszClkConfig.mmrClkEnable  = TRUE;
    gDRV_ipipeObj.rszClkConfig.coreClkEnable = TRUE;

    gDRV_ipipeObj.rszMinMaxConfig.minY       = 0;
    gDRV_ipipeObj.rszMinMaxConfig.maxY       = 0xFF;
    gDRV_ipipeObj.rszMinMaxConfig.minC       = 0;
    gDRV_ipipeObj.rszMinMaxConfig.maxC       = 0xFF;

    for (i = 0; i < CSL_RSZ_CH_MAX; i++)
    {
        if (!gDRV_ipipeObj.config.rszOutConfig[i].enable)
        {
            continue;
        }

        pOutConfig = &gDRV_ipipeObj.rszOutConfig[i];

        pOutConfig->flipH                  = gDRV_ipipeObj.config.rszOutConfig[i].flipH;
        pOutConfig->flipV                  = gDRV_ipipeObj.config.rszOutConfig[i].flipV;
        pOutConfig->irqInterval            = 0;
        pOutConfig->dmaInterval            = 10;//gDRV_ipipeObj.config.rszOutConfig[i].dmaInterval < 0x10 ? 0x10 : gDRV_ipipeObj.config.rszOutConfig[i].dmaInterval;
#if 0
        OSA_printf("\n------pOutConfig->dmaInterval = 0x%x\n", pOutConfig->dmaInterval);
#endif        
        pOutConfig->enable                 = FALSE;
        pOutConfig->oneShotEnable          = FALSE;
        pOutConfig->yuv420YOutEnable       = FALSE;
        pOutConfig->yuv420COutEnable       = FALSE;

        if (gDRV_ipipeObj.config.rszOutConfig[i].outFormat == DRV_DATA_FORMAT_YUV420)
        {
            pOutConfig->yuv420YOutEnable       = TRUE;
            pOutConfig->yuv420COutEnable       = TRUE;
        }

        pOutConfig->inStartX               = 0;
        pOutConfig->inStartY               = 0;
        pOutConfig->outWidth               = gDRV_ipipeObj.rszInfo[i].width;
        pOutConfig->outHeight              = gDRV_ipipeObj.rszInfo[i].height;
        pOutConfig->vertStartPhaseY        = 0;
        pOutConfig->vertStartPhaseC        = 0;
        pOutConfig->vertReszRatio          = ((Uint32)gDRV_ipipeObj.ipipeInFrameConfig.inHeight * 256) / pOutConfig->outHeight;
        pOutConfig->vertReszTypeY          = CSL_RSZ_TYPE_4_TAP_CUBIC_CONVOLUTION;
        pOutConfig->vertReszTypeC          = CSL_RSZ_TYPE_4_TAP_CUBIC_CONVOLUTION;
        pOutConfig->vertLpfIntensityY      = 0;
        pOutConfig->vertLpfIntensityC      = 0;
        pOutConfig->horzStartPhase         = 0;
        pOutConfig->horzStartPhaseAdd      = 0;
        pOutConfig->horzReszRatio          = ((Uint32)gDRV_ipipeObj.ipipeInFrameConfig.inWidth * 256) / pOutConfig->outWidth;
        pOutConfig->horzReszTypeY          = CSL_RSZ_TYPE_4_TAP_CUBIC_CONVOLUTION;
        pOutConfig->horzReszTypeC          = CSL_RSZ_TYPE_4_TAP_CUBIC_CONVOLUTION;
        pOutConfig->horzLpfIntensityY      = 0;
        pOutConfig->horzLpfIntensityC      = 0;
        pOutConfig->downScaleModeEnable    = FALSE;
        pOutConfig->downScaleModeAvgSizeV  = 0;
        pOutConfig->downScaleModeAvgSizeH  = 0;
        pOutConfig->rgbOutEnable           = FALSE;
        pOutConfig->rgbOutMaskFirstEnable  = FALSE;
        pOutConfig->rgbOutMaskLastEnable   = FALSE;
        pOutConfig->rgbOutType             = 0;
        pOutConfig->rgbOutAlphaVal         = 0;
        pOutConfig->outStartAddr           = (Uint8*)gDRV_ipipeObj.bufInfoRsz[i][0].physAddr + gDRV_ipipeObj.config.rszValidDataStartOffset;

        if (gDRV_ipipeObj.rszInfo[i].outFormat == DRV_DATA_FORMAT_YUV422)
        {
            pOutConfig->outLineOffset          = gDRV_ipipeObj.rszInfo[i].offsetH*2;
        }
        else
        {
            pOutConfig->outLineOffset          = gDRV_ipipeObj.rszInfo[i].offsetH;
        }

        pOutConfig->yuv420CoutStartAddr    = pOutConfig->outStartAddr+gDRV_ipipeObj.rszInfo[i].offsetH*gDRV_ipipeObj.rszInfo[i].offsetV;
        pOutConfig->yuv420CoutLineOffset   = pOutConfig->outLineOffset;

        pOutConfig->outBaseAddr            = pOutConfig->outStartAddr;
        pOutConfig->outStartLine           = 0;
        pOutConfig->outEndLine             = pOutConfig->outHeight;

        pOutConfig->yuv420CoutBaseAddr     = pOutConfig->yuv420CoutStartAddr;
        pOutConfig->yuv420CoutStartLine    = 0;
        pOutConfig->yuv420CoutEndLine      = pOutConfig->outHeight/2;

        bufInit[i].numBuf = gDRV_ipipeObj.numBufRsz[i];
        bufInit[i].curBuf = 0;
        for (k = 0; k < bufInit[i].numBuf; k++)
        {
            bufInit[i].bufAddr[k] = (Uint32)gDRV_ipipeObj.bufInfoRsz[i][k].physAddr + gDRV_ipipeObj.config.rszValidDataStartOffset;
        }

        bufConfig[i].width = gDRV_ipipeObj.rszInfo[i].width;
        bufConfig[i].height = gDRV_ipipeObj.rszInfo[i].height;
        bufConfig[i].offsetH = gDRV_ipipeObj.rszInfo[i].offsetH;
        bufConfig[i].offsetV = gDRV_ipipeObj.rszInfo[i].offsetV;

        if (gDRV_ipipeObj.config.rszOutConfig[i].outFormat == DRV_DATA_FORMAT_YUV420)
        {
            bufConfig[i].format = CSL_RSZ_CHROMA_FORMAT_420;
        }
        else
        {
            bufConfig[i].format = CSL_RSZ_CHROMA_FORMAT_422;
        }
    }

    gDRV_ipipeObj.ipipeifClk.clkSel        = CSL_IPIPEIF_IPIPEIF_CLOCK_SELECT_PCLK;
    gDRV_ipipeObj.ipipeifClk.clkDivM       = 0;
    gDRV_ipipeObj.ipipeifClk.clkDivN       = 0;
    gDRV_ipipeObj.ipipeifClk.pixelsPerLine = gDRV_ipipeObj.ipipeInFrameConfig.inWidth;
    gDRV_ipipeObj.ipipeifClk.linesPerFrame = gDRV_ipipeObj.ipipeInFrameConfig.inHeight;

    gDRV_ipipeObj.ipipeifDpcmConfig.enable        = FALSE;
    gDRV_ipipeObj.ipipeifDpcmConfig.bits          = 0;
    gDRV_ipipeObj.ipipeifDpcmConfig.predType      = 0;

    gDRV_ipipeObj.ipipeifDdrInConfig.enable          = FALSE;
    gDRV_ipipeObj.ipipeifDdrInConfig.oneShotEnable   = FALSE;
    gDRV_ipipeObj.ipipeifDdrInConfig.rawDataShift    = 0;
    gDRV_ipipeObj.ipipeifDdrInConfig.rawUnpackMode   = 0;
    gDRV_ipipeObj.ipipeifDdrInConfig.inputAddr       = 0;
    gDRV_ipipeObj.ipipeifDdrInConfig.inputWidth      = 0;
    gDRV_ipipeObj.ipipeifDdrInConfig.inputHeight     = 0;
    gDRV_ipipeObj.ipipeifDdrInConfig.inputLineOffset = 0;
    gDRV_ipipeObj.ipipeifDdrInConfig.dpcEnable       = gDRV_ipipeObj.pImgsIpipeConfig->ipipeifParams.ddrInDpcEnable;
    gDRV_ipipeObj.ipipeifDdrInConfig.dpcThreshold    = gDRV_ipipeObj.pImgsIpipeConfig->ipipeifParams.ddrInDpcThreshold;
    gDRV_ipipeObj.ipipeifDdrInConfig.dpcmConfig      = &gDRV_ipipeObj.ipipeifDpcmConfig;
    gDRV_ipipeObj.ipipeifDdrInConfig.clkConfig       = &gDRV_ipipeObj.ipipeifClk;

    gDRV_ipipeObj.ipipeifInSrcConfig.inputSource2           = CSL_IPIPEIF_INPUT_SOURCE_PARALLEL_PORT_RAW;
    gDRV_ipipeObj.ipipeifInSrcConfig.pixelDecimationEnable  = FALSE;
    gDRV_ipipeObj.ipipeifInSrcConfig.pixelDecimationRatio   = 16;
    gDRV_ipipeObj.ipipeifInSrcConfig.avgFilterEnable        = gDRV_ipipeObj.pImgsIpipeConfig->ipipeifParams.avgFilterEnable;
    gDRV_ipipeObj.ipipeifInSrcConfig.alignHsyncVsync        = FALSE;
    gDRV_ipipeObj.ipipeifInSrcConfig.initReszPosH           = 0;
    gDRV_ipipeObj.ipipeifInSrcConfig.outClip                = gDRV_ipipeObj.pImgsIpipeConfig->ipipeifParams.outClip;
    gDRV_ipipeObj.ipipeifInSrcConfig.gain                   = gDRV_ipipeObj.pImgsIpipeConfig->ipipeifParams.gain;
    gDRV_ipipeObj.ipipeifInSrcConfig.isifInType             = CSL_IPIPEIF_ISIF_INPUT_TYPE_RAW;
    gDRV_ipipeObj.ipipeifInSrcConfig.yuvConvertEnable       = FALSE;
    gDRV_ipipeObj.ipipeifInSrcConfig.vsyncMaskEnable        = FALSE;

    gDRV_ipipeObj.ipipeifVpifIsifInConfig.dpcEnable         = gDRV_ipipeObj.pImgsIpipeConfig->ipipeifParams.vpiIsifInDpcEnable;
    gDRV_ipipeObj.ipipeifVpifIsifInConfig.dpcThreshold      = gDRV_ipipeObj.pImgsIpipeConfig->ipipeifParams.vpiIsifInDpcThreshold;
    gDRV_ipipeObj.ipipeifVpifIsifInConfig.wenUseEnable      = FALSE;
    gDRV_ipipeObj.ipipeifVpifIsifInConfig.vdPol             = CSL_IPIPEIF_HDPOLARITY_POSITIVE;
    gDRV_ipipeObj.ipipeifVpifIsifInConfig.hdPol             = CSL_IPIPEIF_HDPOLARITY_POSITIVE;

    status = CSL_rszHwSetup(&gCSL_rszHndl, &gDRV_ipipeObj.rszSetup);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_ipipeHwSetup()\n");
        return status;
    }

    for (i = 0; i < CSL_RSZ_CH_MAX; i++)
    {
        if (!gDRV_ipipeObj.config.rszOutConfig[i].enable)
        {
            continue;
        }

        status = CSL_rszBufInit(&gCSL_rszHndl, i, &bufInit[i], &bufConfig[i]);
        if (status != OSA_SOK)
        {
            OSA_ERROR("CSL_ipipeBufInit(%d)\n", i);
            return status;
        }
    }

    status = CSL_ipipeifSetInSource2Config(&gCSL_ipipeifHndl, &gDRV_ipipeObj.ipipeifInSrcConfig);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_ipipeifSetInSource2Config()\n");
        return status;
    }

    status = CSL_ipipeifSetVpifIsifInConfig(&gCSL_ipipeifHndl, &gDRV_ipipeObj.ipipeifVpifIsifInConfig);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_ipipeifSetVpifIsifInConfig()\n");
        return status;
    }

    status = CSL_ipipeifSetSdramInConfig(&gCSL_ipipeifHndl, &gDRV_ipipeObj.ipipeifDdrInConfig);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_ipipeifSetSdramInConfig()\n");
        return status;
    }

    return status;
}

int DRV_ipipeBoxcarSetParams()
{
    int status;
    Uint16 inWidth, inHeight, blockSize, k;
    CSL_BufInit bufInit;

    gDRV_ipipeObj.boxcarConfig.enable = FALSE;
    gDRV_ipipeObj.boxcarConfig.oneShotEnable = FALSE;
    gDRV_ipipeObj.boxcarConfig.blockSize = CSL_IPIPE_BOXCAR_BLOCK_SIZE_8_8;
    gDRV_ipipeObj.boxcarConfig.inShift = 0;
    gDRV_ipipeObj.boxcarConfig.outAddr = NULL;

    if (gDRV_ipipeObj.config.boxcarBlockSize==DRV_IPIPE_BOXCAR_BLOCK_SIZE_NONE)
    {
        return OSA_SOK;
    }

    gDRV_ipipeObj.boxcarConfig.blockSize = gDRV_ipipeObj.config.boxcarBlockSize;
    gDRV_ipipeObj.boxcarConfig.inShift   = gDRV_ipipeObj.config.boxcarInShift;
    gDRV_ipipeObj.boxcarConfig.outAddr   = (Uint8*)gDRV_ipipeObj.bufInfoBoxcar[0].physAddr;

    if (gDRV_ipipeObj.boxcarConfig.blockSize == CSL_IPIPE_BOXCAR_BLOCK_SIZE_8_8)
    {
        blockSize = 8;
    }
    else
    {
        blockSize = 16;
    }

    inWidth  = gDRV_ipipeObj.imgsModeInfo.validWidth;
    inHeight = gDRV_ipipeObj.imgsModeInfo.validHeight;

    if (inWidth % blockSize)
    {
        OSA_printf(" DRV: IPIPE: Warning input width (%d) not multiple of boxcar block size (%d)\n",
                   inWidth, blockSize);
    }
    if (inHeight%blockSize)
    {
        OSA_printf(" DRV: IPIPE: Warning input height (%d) not multiple of boxcar block size (%d)\n",
                   inHeight, blockSize);
    }

    gDRV_ipipeObj.boxcarInfo.width = inWidth/blockSize;
    gDRV_ipipeObj.boxcarInfo.height= inHeight/blockSize;
    gDRV_ipipeObj.boxcarInfo.offsetH = gDRV_ipipeObj.boxcarInfo.width*4*2;

    OSA_printf(" DRV: IPIPE: Boxcar: Original = %dx%d, Boxcar = %dx%d, (%d)\n",
               inWidth, inHeight,
               gDRV_ipipeObj.boxcarInfo.width,
               gDRV_ipipeObj.boxcarInfo.height,
               blockSize);


    bufInit.numBuf = gDRV_ipipeObj.numBufBoxcar;
    bufInit.curBuf = 0;
    for (k = 0; k < bufInit.numBuf; k++)
    {
        bufInit.bufAddr[k] = (Uint32)gDRV_ipipeObj.bufInfoBoxcar[k].physAddr;
    }

    status = CSL_ipipeBoxcarBufInit(&gCSL_ipipeHndl, &bufInit);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_ipipeBoxcarBufInit()\n");
        return status;
    }

    return status;
}

int DRV_ipipeSetParams(void)
{
    int status = CSL_ipipeHwReset(&gCSL_ipipeHndl);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_ipipeHwReset()\n");
        return status;
    }

    gDRV_ipipeObj.ipipeSetup.bypassModuleIfNullConfig   = TRUE;
    gDRV_ipipeObj.ipipeSetup.enable                     = FALSE;
    gDRV_ipipeObj.ipipeSetup.oneShotEnable              = FALSE;
    gDRV_ipipeObj.ipipeSetup.wenUseEnable               = FALSE;
    gDRV_ipipeObj.ipipeSetup.dataPath                   = CSL_IPIPE_IPIPE_DATA_PATH_BAYER_IN_YCBCR_RGB_OUT;
    gDRV_ipipeObj.ipipeSetup.colpatConfig               = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.colPat;
    gDRV_ipipeObj.ipipeSetup.clkConfig                  = &gDRV_ipipeObj.ipipeClkConfig;
    gDRV_ipipeObj.ipipeSetup.inFrameConfig              = &gDRV_ipipeObj.ipipeInFrameConfig;
    gDRV_ipipeObj.ipipeSetup.lscConfig                  = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.lsc;
    gDRV_ipipeObj.ipipeSetup.dpcConfig                  = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.dpc;
    gDRV_ipipeObj.ipipeSetup.nf1Config                  = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.nf1;
    gDRV_ipipeObj.ipipeSetup.nf2Config                  = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.nf2;
    gDRV_ipipeObj.ipipeSetup.gicConfig                  = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.gic;
    gDRV_ipipeObj.ipipeSetup.wbConfig                   = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.wb;
    gDRV_ipipeObj.ipipeSetup.cfaConfig                  = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.cfa;
    gDRV_ipipeObj.ipipeSetup.rgb2Rgb1Config             = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.rgb2rgb1;
    gDRV_ipipeObj.ipipeSetup.gammaConfig                = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.gamma;
    gDRV_ipipeObj.ipipeSetup.rgb2Rgb2Config             = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.rgb2rgb2;
    gDRV_ipipeObj.ipipeSetup.lut3DConfig                = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.lut3d;
    gDRV_ipipeObj.ipipeSetup.rgb2YuvConfig              = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.rgb2yuv;
    gDRV_ipipeObj.ipipeSetup.cntBrtConfig               = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.cntBrt;
    gDRV_ipipeObj.ipipeSetup.gbceConfig                 = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.gbce;
    gDRV_ipipeObj.ipipeSetup.edgeEnhanceConfig          = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.yee;
    gDRV_ipipeObj.ipipeSetup.chromaArtifactReduceConfig = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.car;
    gDRV_ipipeObj.ipipeSetup.chromaGainSupressConfig    = &gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.cgs;
    gDRV_ipipeObj.ipipeSetup.boxcarConfig               = &gDRV_ipipeObj.boxcarConfig;
    gDRV_ipipeObj.ipipeSetup.histogramConfig            = &gDRV_ipipeObj.histoInfo;
    gDRV_ipipeObj.ipipeSetup.bscConfig                  = &gDRV_ipipeObj.bscInfo;

    gDRV_ipipeObj.ipipeSetup.dpcConfig->lutAddr         = gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.dpcTable;
    gDRV_ipipeObj.ipipeSetup.gammaConfig->tableR        = gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.gammaTableR;
    gDRV_ipipeObj.ipipeSetup.gammaConfig->tableG        = gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.gammaTableG;
    gDRV_ipipeObj.ipipeSetup.gammaConfig->tableB        = gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.gammaTableB;
    gDRV_ipipeObj.ipipeSetup.lut3DConfig->table         = gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.lut3dTable;
    gDRV_ipipeObj.ipipeSetup.gbceConfig->gainTable      = gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.gbceTable;
    gDRV_ipipeObj.ipipeSetup.edgeEnhanceConfig->table   = gDRV_ipipeObj.pImgsIpipeConfig->ipipeParams.yeeTable;

    gDRV_ipipeObj.ipipeClkConfig.mmrClkEnable             = TRUE;
    gDRV_ipipeObj.ipipeClkConfig.yeeCarClkEnable          = TRUE;
    gDRV_ipipeObj.ipipeClkConfig.cfa2yuvHistoBscClkEnable = TRUE;
    gDRV_ipipeObj.ipipeClkConfig.dpc2WbClkEnable          = TRUE;
    gDRV_ipipeObj.ipipeClkConfig.boxcarClkEnable          = TRUE;

    gDRV_ipipeObj.ipipeInFrameConfig.inStartX  = gDRV_ipipeObj.imgsModeInfo.validStartX;
    gDRV_ipipeObj.ipipeInFrameConfig.inStartY  = gDRV_ipipeObj.imgsModeInfo.validStartY;
    gDRV_ipipeObj.ipipeInFrameConfig.inWidth   = gDRV_ipipeObj.imgsModeInfo.validWidth;
    gDRV_ipipeObj.ipipeInFrameConfig.inHeight  = gDRV_ipipeObj.imgsModeInfo.validHeight;

    status = DRV_ipipeBoxcarSetParams();
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_ipipeBoxcarSetParams()\n");
        return status;
    }

    status = DRV_ipipeBscSetParams();
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_ipipeBscSetParams()\n");
        return status;
    }

    status = DRV_ipipeHistoSetParams();
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_ipipeHistoSetParams()\n");
        return status;
    }

    status = CSL_ipipeHwSetup(&gCSL_ipipeHndl, &gDRV_ipipeObj.ipipeSetup);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_ipipeHwSetup()\n");
        return status;
    }
    
    status = DRV_ipipeRszSetParams();
    if (status != OSA_SOK)
    {
        return status;
    }

    return status;
}

int DRV_ipipeSetDigitalPtz(Uint16 dzoom, Int16 pan, Int16 tilt, DRV_IpipeDigitalPtzStatus *pStatus)
{
    CSL_RszInFrameConfig inFrameConfig;
    CSL_RszDzoomConfig dzoomConfig;
    int hrsz[2], vrsz[2], inStartX, inStartY, inWidth, inHeight, dx, dy, status, i;

    if (dzoom < 100 )
    {
        dzoom = 100;
    }

    if (dzoom > 400 )
    {
        dzoom = 400;
    }

    if (pan < -10)
    {
        pan = -10;
    }

    if (pan > 10)
    {
        pan = 10;
    }

    if (tilt < -10)
    {
        tilt = -10;
    }

    if (tilt > 10)
    {
        tilt = 10;
    }

    memcpy(&inFrameConfig, &gDRV_ipipeObj.rszInFrameConfig, sizeof(gDRV_ipipeObj.rszInFrameConfig));

    inWidth  = (inFrameConfig.inWidth  * 100) / dzoom;
    inHeight = (inFrameConfig.inHeight * 100) / dzoom;
    inWidth  = OSA_floor(inWidth, 2);
    inHeight = OSA_floor(inHeight, 2);
    inStartX = (inFrameConfig.inWidth  - inWidth) / 2;
    inStartY = (inFrameConfig.inHeight - inHeight) / 2;

    dx = OSA_align(inFrameConfig.inWidth / 10, 2);
    dy = OSA_align(inFrameConfig.inHeight / 10, 2);

    // increment or decrement X, Y by dx, dt amount
    inStartX = pan * dx;
    inStartY = tilt * dy;

    pStatus->isLeftPanDone  = FALSE;
    pStatus->isRightPanDone = FALSE;
    pStatus->isUpTiltDone   = FALSE;
    pStatus->isDownTiltDone = FALSE;

    // bound X, Y within the frame
    if (inStartX < 0)
    {
        inStartX = 0;
        pStatus->isLeftPanDone = TRUE;
    }

    if (inStartX + inWidth > inFrameConfig.inWidth)
    {
        inStartX = (inFrameConfig.inWidth - inWidth);
        pStatus->isRightPanDone = TRUE;
    }

    if (inStartY < 0)
    {
        inStartY = 0;
        pStatus->isUpTiltDone   = TRUE;
    }

    if (inStartY + inHeight > inFrameConfig.inHeight)
    {
        inStartY = (inFrameConfig.inHeight - inHeight);
        pStatus->isDownTiltDone   = TRUE;
    }

    for (i=0; i<CSL_RSZ_CH_MAX; i++)
    {
        hrsz[i] = (inWidth*256)/gDRV_ipipeObj.rszOutConfig[i].outWidth;
        vrsz[i] = (inHeight*256)/gDRV_ipipeObj.rszOutConfig[i].outHeight;
    }

    inStartX = OSA_floor(inStartX, 2);
    inStartY = OSA_floor(inStartY, 2);

    dzoomConfig.inStartX = inStartX;
    dzoomConfig.inStartX = inStartY;
    dzoomConfig.inWidth  = inWidth;
    dzoomConfig.inHeight = inHeight;
    dzoomConfig.hrsz[0]  = hrsz[0];
    dzoomConfig.vrsz[0]  = vrsz[0];
    dzoomConfig.hrsz[1]  = hrsz[1];
    dzoomConfig.vrsz[1]  = vrsz[1];

    pStatus->totalInWidth  = inFrameConfig.inWidth;
    pStatus->totalInHeight = inFrameConfig.inHeight;
    pStatus->curInStartX   = inStartX;
    pStatus->curInStartY   = inStartY;
    pStatus->curInWidth    = inWidth ;
    pStatus->curInHeight   = inHeight;

    status = CSL_rszSetDzoomConfig(&gCSL_rszHndl, &dzoomConfig);
    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_rszSetDzoomConfig()\n");
        return status;
    }

    return status;
}

/* LK: 07/10: contineous twin resize mode: support for runtime change of resizer input crop and output sizes.
    Can be used for input aspect ratio change and output resolution change dynamically */
int DRV_ipipeSetReszRtPrms(DRV_IpipeReszRtPrm *rszPrm)
{
    CSL_RszRuntimeConfig runtimeCfg;
    CSL_RszInFrameConfig inFrameConfig;
    int hrsz[2], vrsz[2], inStartX, inStartY, inWidth, inHeight, i;
    int rszMod;
    /* Check parameters */
    memcpy(&inFrameConfig, &gDRV_ipipeObj.rszInFrameConfig, sizeof(gDRV_ipipeObj.rszInFrameConfig));

    inWidth  = rszPrm->inWidth;
    inHeight = rszPrm->inHeight;
    inWidth  = OSA_floor(inWidth, 2);
    inHeight = OSA_floor(inHeight, 2);
    inStartX = rszPrm->inStartX;
    inStartY = rszPrm->inStartY;

    // bound X, Y within the frame
    if (inStartX < 0)
    {
        inStartX = 0;
    }

    if (inStartX + inWidth > inFrameConfig.inWidth)
    {
        inStartX = (inFrameConfig.inWidth - inWidth);
    }

    if (inStartY < 0)
    {
        inStartY = 0;
    }

    if (inStartY + inHeight > inFrameConfig.inHeight)
    {
        inStartY = (inFrameConfig.inHeight - inHeight);
    }

    inStartX = OSA_floor(inStartX, 2);
    inStartY = OSA_floor(inStartY, 2);

    for (i = 0; i < CSL_RSZ_CH_MAX; i++)
    {
        if (rszPrm->pOut[i].enableRtChange == TRUE)
        {
            hrsz[i] = (inWidth*256)/rszPrm->pOut[i].outWidth;
            vrsz[i] = (inHeight*256)/rszPrm->pOut[i].outHeight;
            rszMod   = i;
            runtimeCfg.inStartX = inStartX;
            runtimeCfg.inStartY = inStartY;
            runtimeCfg.hrsz     = hrsz[rszMod];
            runtimeCfg.vrsz     = vrsz[rszMod];
            runtimeCfg.outWidth = rszPrm->pOut[rszMod].outWidth;
            runtimeCfg.outHeight= rszPrm->pOut[rszMod].outHeight;
            CSL_rszSetRuntimeConfig(&gCSL_rszHndl, rszMod, &runtimeCfg);
        }
    }

    return OSA_SOK;
}

int DRV_ipipeEnableFlip(Bool flipH, Bool flipV)
{
    int status = CSL_rszSetFlip(&gCSL_rszHndl, CSL_RSZ_A, flipH, flipV);
    status |= CSL_rszSetFlip(&gCSL_rszHndl, CSL_RSZ_B, flipH, flipV);

    return status;
}

int DRV_ipipeSetContrastBrightness(Uint16 contrast, Uint16 brightness)
{
    CSL_IpipeCntBrtConfig cntBrt;
    int status;

    cntBrt.contrast = contrast;
    cntBrt.brightness = brightness;

    status = CSL_ipipeSetCntBrtConfig(&gCSL_ipipeHndl, &cntBrt);

    IMAGE_TUNE_SetIpipeCntBrt(&cntBrt);

    return status;
}

int DRV_ipipeSetYoffet(int Yoffset)
{
    int status;
    CSL_IpipeRgb2YuvConfig *data = gDRV_ipipeObj.ipipeSetup.rgb2YuvConfig;

    data->offset[0] = Yoffset;

    status = CSL_ipipeSetRgb2YuvConfig(&gCSL_ipipeHndl, data);

    IMAGE_TUNE_SetIpipeRgb2Yuv(data);

    return status;

}

int DRV_ipipeSetRgb2Yuv(CSL_IpipeRgb2YuvConfig *config)
{
    int status = CSL_ipipeSetRgb2YuvConfig(&gCSL_ipipeHndl, config);
    IMAGE_TUNE_SetIpipeRgb2Yuv(config);
    return status;
}

int DRV_ipipeSetEdgeEnhance(CSL_IpipeEdgeEnhanceConfig *config)
{
    int status = CSL_ipipeSetEdgeEnhanceConfig(&gCSL_ipipeHndl, config);

    IMAGE_TUNE_SetIpipeEdgeEnhance(config);

    return status;
}

int DRV_ipipeSetNf2(CSL_IpipeNfConfig *config)
{
    int status = CSL_ipipeSetNf2Config(&gCSL_ipipeHndl, config);

    IMAGE_TUNE_SetIpipeNf2(config);

    return status;
}

int DRV_ipipeSetDpcConfig(CSL_IpipeDpcConfig *config)
{
    int status = CSL_ipipeSetDpcConfig(&gCSL_ipipeHndl, config);
    return status;
}

int DRV_ipipeGetFrameSize(int *width, int *height)
{
    *width  = gDRV_ipipeObj.ipipeInFrameConfig.inWidth;
    *height = gDRV_ipipeObj.ipipeInFrameConfig.inHeight;
    return OSA_SOK;
}


/* DRV_ipipeGetHistogram - Get histogram data */
int DRV_ipipeGetHistogram(void)
{
    int status = OSA_SOK;
    Uint32 * table = gDRV_ipipeObj.histResultBuf;
    Uint32 offset;
    Uint32 size;

    if (table == NULL)
    {
        OSA_ERROR("\nGetHistogram mem error %d \n", status);
        gDRV_ipipeObj.histStatus = 0;
        return OSA_EFAIL;
    }

    if (status == OSA_SOK)
    {
        memset(table,0, CSL_IPIPE_HISTOGRAM_RESULTS_MAX_ENTRIES);
        offset = 0; //(128* 4 )* 1;       // no of bin * size of data (Uint32) * colour RGBY = 0 1 2 3;
        size = 0; //128;   //128;         // no of bin of data (Uint32)
        status = CSL_ipipeHistogramReadResults(&gCSL_ipipeHndl, table, offset, size);
    }

    if (status != OSA_SOK)
    {
        OSA_ERROR("CSL_ipipeHistogramReadResults -%d \n", status);
        gDRV_ipipeObj.histStatus = 0;
        return OSA_EFAIL;
    }

    gDRV_ipipeObj.histStatus = 1;
    return status;
}

/* DRV_ipipeCpyHistData - Copy required histogram data */
int DRV_ipipeCpyHistData(Uint32* histData, Uint8* histStatus)
{
    *histStatus = gDRV_ipipeObj.histStatus;
    if (*histStatus)
    {
        OSA_mutexLock(&gDRV_ipipeObj.statusLock);
        memcpy(&histData[0], &gDRV_ipipeObj.histResultBuf[3*256], 256*sizeof(unsigned int));
        OSA_mutexUnlock(&gDRV_ipipeObj.statusLock);
    }
    return OSA_SOK;
}

