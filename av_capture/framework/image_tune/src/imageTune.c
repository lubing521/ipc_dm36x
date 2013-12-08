#include <osa_file.h>
#include <osa_cmem.h>

#include <imageTunePriv.h>
#include <imageTuneCmdHandler.h>

IMAGE_TUNE_Ctrl gIMAGE_TUNE_ctrl;

static int flgIsRawDataSend = 0;
static int flgIsYuvDataSend = 0;

#ifdef IMAGE_TUNE_MULTI_PARAM
IMAGE_TUNE_ParamSetList *gImageTunePrmList;
int gCurPrmSet = 0;       // to keep current paramset id...
#endif

int IMAGE_TUNE_Init()
{
    gIMAGE_TUNE_ctrl.enable = 0;
    gIMAGE_TUNE_ctrl.serverSocketId = -1;
    gIMAGE_TUNE_ctrl.clientSocketId = -1;
    gIMAGE_TUNE_ctrl.connectedSocketId = -1;
    gIMAGE_TUNE_ctrl.serverPort = IMAGE_TUNE_SERVER_DEFAULT_PORT;

    if (sizeof(IMAGE_TUNE_ParamSet) > (IMAGE_TUNE_PACKET_SIZE_MAX-IMAGE_TUNE_PACKET_HEADER_SIZE-IMAGE_TUNE_PACKET_FOOTER_SIZE))
    {
        OSA_printf(" IMAGE TUNE: sizeof(IMAGE_TUNE_ParamSet) = %d bytes\n", sizeof(IMAGE_TUNE_ParamSet));
        OSA_printf(" IMAGE TUNE: Max packet parameter size = %d bytes\n", 
                   (IMAGE_TUNE_PACKET_SIZE_MAX-IMAGE_TUNE_PACKET_HEADER_SIZE-IMAGE_TUNE_PACKET_FOOTER_SIZE));
        return OSA_EFAIL;
    }

    memset(gIMAGE_TUNE_ctrl.cmdHandlerList, 0, sizeof(gIMAGE_TUNE_ctrl.cmdHandlerList));
    memset(&gIMAGE_TUNE_ctrl.curPrm, 0, sizeof(IMAGE_TUNE_ParamSet));

#ifdef IMAGE_TUNE_MULTI_PARAM
    /* Allocate memory for paramset list */
    if (gImageTunePrmList != NULL)
    {
        OSA_cmemFree(gImageTunePrmList);
    }

    gImageTunePrmList = OSA_cmemAlloc(sizeof(IMAGE_TUNE_ParamSetList), 32);
    if (gImageTunePrmList != NULL)
    {
        memset(gImageTunePrmList, 0, sizeof(IMAGE_TUNE_ParamSetList));
    }
    else
    {
        gImageTunePrmList = NULL;
    }

#endif

    // attach default command handlers
    // Sys handlers
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_SYS_GET_VERSION,     IMAGE_TUNE_CmdExecuteGetVersion);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_SYS_ECHO_STRING,     IMAGE_TUNE_CmdExecuteEchoString);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_SYS_SAVE_PARAM_SET,  IMAGE_TUNE_CmdExecuteSaveParamSet);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_SYS_LOAD_PARAM_SET,  IMAGE_TUNE_CmdExecuteLoadParamSet);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_SYS_GET_PARAMS,      IMAGE_TUNE_CmdExecuteGetParamSet);

    // Ipipe handlers
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_COL_PAT,       IMAGE_TUNE_CmdExecuteIpipeColPat);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_LSC,           IMAGE_TUNE_CmdExecuteIpipeLsc);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_DPC,           IMAGE_TUNE_CmdExecuteIpipeDpc);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_NF1,           IMAGE_TUNE_CmdExecuteIpipeNf1);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_NF2,           IMAGE_TUNE_CmdExecuteIpipeNf2);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_GIC,           IMAGE_TUNE_CmdExecuteIpipeGic);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_WB,            IMAGE_TUNE_CmdExecuteIpipeWb);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_CFA,           IMAGE_TUNE_CmdExecuteIpipeCfa);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_RGB2RGB1,      IMAGE_TUNE_CmdExecuteIpipeRgb2Rgb1);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_GAMMA,         IMAGE_TUNE_CmdExecuteIpipeGamma);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_RGB2RGB2,      IMAGE_TUNE_CmdExecuteIpipeRgb2Rgb2);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_LUT3D,         IMAGE_TUNE_CmdExecuteIpipeLut3d);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_RGB2YUV,       IMAGE_TUNE_CmdExecuteIpipeRgb2Yuv);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_CNT_BRT,       IMAGE_TUNE_CmdExecuteIpipeCntBrt);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_GBCE,          IMAGE_TUNE_CmdExecuteIpipeGbce);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_EDGE_ENHANCE,  IMAGE_TUNE_CmdExecuteIpipeEdgeEnhance);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_CAR,           IMAGE_TUNE_CmdExecuteIpipeCromaArtifactReduce);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPE_CGS,           IMAGE_TUNE_CmdExecuteIpipeChromaGainSupress);

    // Ccdc handlers
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_CCDC_HLPF_ENABLE,           IMAGE_TUNE_CmdExecuteCcdcHLpf);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_CCDC_IN_DATA_MSB_POSITION,  IMAGE_TUNE_CmdExecuteCcdcInDataMsbPosition);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_CCDC_LINEARIZATION,         IMAGE_TUNE_CmdExecuteCcdcLinerization);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_CCDC_COL_PAT,               IMAGE_TUNE_CmdExecuteCcdcColPat);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_CCDC_GAIN_OFFSET,           IMAGE_TUNE_CmdExecuteCcdcGainOffset);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_CCDC_DFC,                   IMAGE_TUNE_CmdExecuteCcdcDfc);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_CCDC_CLAMP,                 IMAGE_TUNE_CmdExecuteCcdcClamp);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_CCDC_LSC,                   IMAGE_TUNE_CmdExecuteCcdcLsc);

    // Ipipeif handlers
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPEIF_VPI_ISIF_IN_DPC,    IMAGE_TUNE_CmdExecuteIpipeifVpiIsifInDpc);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPEIF_DDR_IN_DPC,         IMAGE_TUNE_CmdExecuteIpipeifDdrInDpc);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPEIF_GAIN,               IMAGE_TUNE_CmdExecuteIpipeifGain);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPEIF_OUTCLIP,            IMAGE_TUNE_CmdExecuteIpipeifOutClip);
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_IPIPEIF_AVG_FILTER_ENABLE,  IMAGE_TUNE_CmdExecuteIpipeifAvgFilter);

    // LDC handler's
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_LDC_PRM,   IMAGE_TUNE_CmdExecuteLdcPrm );

    // VNF handler's
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_VNF_PRM,   IMAGE_TUNE_CmdExecuteVnfPrm );

    // AWB handler's
    IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_AWB_PRM,   IMAGE_TUNE_CmdExecuteAwbPrm );

    return OSA_SOK;
}

int IMAGE_TUNE_Exit()
{
#ifdef IMAGE_TUNE_MULTI_PARAM
    /* Release the memory allocated for IT paramset list */
    if (gImageTunePrmList != NULL)
    {
        OSA_cmemFree(gImageTunePrmList);
    }
#endif
    return OSA_SOK;

}

int IMAGE_TUNE_GetParams(IMAGE_TUNE_Params *prm)
{
    if (prm->pCcdcPrm != NULL)
    {
        memcpy(prm->pCcdcPrm, &gIMAGE_TUNE_ctrl.curPrm.ccdcPrm, sizeof(gIMAGE_TUNE_ctrl.curPrm.ccdcPrm));
    }
    if (prm->pIpipeifPrm != NULL)
    {
        //printf("XXXXXXXXXXXXXXXX get ipipeif\n");
        memcpy(prm->pIpipeifPrm, &gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm, sizeof(gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm));
    }
    if (prm->pLdcPrm != NULL)
    {
        memcpy(prm->pLdcPrm, &gIMAGE_TUNE_ctrl.curPrm.ldcPrm, sizeof(gIMAGE_TUNE_ctrl.curPrm.ldcPrm));
    }
    if (prm->pIpipePrm != NULL)
    {
       // printf("XXXXXXXXXXXXXXXX get ipipe\n");
        memcpy(prm->pIpipePrm, &gIMAGE_TUNE_ctrl.curPrm.ipipePrm, sizeof(gIMAGE_TUNE_ctrl.curPrm.ipipePrm));
    }
    if (prm->pAwbPrm != NULL)
    {
        memcpy(prm->pAwbPrm, &gIMAGE_TUNE_ctrl.curPrm.awbPrm, sizeof(gIMAGE_TUNE_ctrl.curPrm.awbPrm));
    }
    if (prm->pVnfPrm != NULL)
    {
        memcpy(prm->pVnfPrm, &gIMAGE_TUNE_ctrl.curPrm.vnfPrm, sizeof(gIMAGE_TUNE_ctrl.curPrm.vnfPrm));
    }

    return OSA_SOK;
}

int IMAGE_TUNE_SetParams(IMAGE_TUNE_Params *prm)
{
    if (prm->pCcdcPrm != NULL)
    {
        memcpy(&gIMAGE_TUNE_ctrl.curPrm.ccdcPrm, prm->pCcdcPrm, sizeof(gIMAGE_TUNE_ctrl.curPrm.ccdcPrm));
    }
    if (prm->pIpipeifPrm != NULL)
    {
       // printf("XXXXXXXXXXXXXXXX set ipipeif\n");
        memcpy(&gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm, prm->pIpipeifPrm, sizeof(gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm));
    }
    if (prm->pLdcPrm != NULL)
    {
        memcpy(&gIMAGE_TUNE_ctrl.curPrm.ldcPrm, prm->pLdcPrm, sizeof(gIMAGE_TUNE_ctrl.curPrm.ldcPrm));
    }
    if (prm->pIpipePrm != NULL)
    {
       // printf("XXXXXXXXXXXXXXXX set ipipe\n");
        memcpy(&gIMAGE_TUNE_ctrl.curPrm.ipipePrm, prm->pIpipePrm, sizeof(gIMAGE_TUNE_ctrl.curPrm.ipipePrm));
    }
    if (prm->pAwbPrm != NULL)
    {
        memcpy(&gIMAGE_TUNE_ctrl.curPrm.awbPrm, prm->pAwbPrm, sizeof(gIMAGE_TUNE_ctrl.curPrm.awbPrm));
    }
    if (prm->pVnfPrm != NULL)
    {
        memcpy(&gIMAGE_TUNE_ctrl.curPrm.vnfPrm, prm->pVnfPrm, sizeof(gIMAGE_TUNE_ctrl.curPrm.vnfPrm));
    }
    return OSA_SOK;
}

int IMAGE_TUNE_SetSendRawData(int val)
{
    flgIsRawDataSend = val;
    return flgIsRawDataSend;
}

int IMAGE_TUNE_SetSendYuvData(int val)
{
    flgIsYuvDataSend = val;
    return flgIsRawDataSend;
}

int IMAGE_TUNE_SaveData(char *filename, IMAGE_TUNE_SaveDataInfo *info)
{
    IMAGE_TUNE_SaveDataFileHeader fileHeader;
    Uint32 dataSize, writeDataSize;
    FILE *fp;
    Uint8 *tmpBuf;
    Uint32 size =0;

    memset(&fileHeader, 0, sizeof(fileHeader));

    fileHeader.dataFormat = info->dataFormat;
    fileHeader.dataWidth  = info->frameWidth;
    fileHeader.dataHeight = info->frameHeight;

    switch (info->dataFormat)
    {
    case 0:
        fileHeader.dataBytesPerLines   = info->frameOffsetH;
        fileHeader.yuv420ChormaOffset  = 0;
        fileHeader.rawDataStartPhase   = info->rawDataStartPhase;
        fileHeader.rawDataBitsPerPixel = info->rawDataBitsPerPixel;
        fileHeader.rawDataFormat       = info->rawDataFormat;
        dataSize = fileHeader.dataBytesPerLines*fileHeader.dataHeight;
        break;
    case 1:
        fileHeader.dataBytesPerLines    = info->frameOffsetH*2;
        fileHeader.yuv420ChormaOffset   = 0;
        fileHeader.rawDataStartPhase    = 0;
        fileHeader.rawDataBitsPerPixel  = 0;
        fileHeader.rawDataFormat        = 0;
        dataSize = fileHeader.dataBytesPerLines*fileHeader.dataHeight;
        break;
    case 2:
        fileHeader.dataBytesPerLines    = info->frameOffsetH;
        fileHeader.yuv420ChormaOffset   = info->frameOffsetH*info->frameOffsetV;
        fileHeader.rawDataStartPhase    = 0;
        fileHeader.rawDataBitsPerPixel  = 0;
        fileHeader.rawDataFormat        = 0;
        dataSize = fileHeader.yuv420ChormaOffset + fileHeader.dataBytesPerLines*info->frameHeight/2;
        break;
    default:
        return OSA_EFAIL;
    }
    //fileHeader.validDataStartOffset = fileHeader.dataBytesPerLines;
    fileHeader.magicNum             = IMAGE_TUNE_HEADER_MAGIC_NUM;
    fileHeader.version              = IMAGE_TUNE_VERSION;

    /* AWB */
    size = sizeof(fileHeader);
    size += (fileHeader.dataBytesPerLines - (size % fileHeader.dataBytesPerLines));
    fileHeader.validDataStartOffset = size;

    //fileHeader.H3aRegs
    memcpy(fileHeader.H3aRegs, info->awbData.h3aRegs, sizeof(fileHeader.H3aRegs));
    fileHeader.AwbNumWinH = info->awbData.awbNumWinH;
    fileHeader.AwbNumWinV = info->awbData.awbNumWinV;
    fileHeader.AwbRgbDataOffset = ((Uint32) &fileHeader.AwbRgbData -(Uint32) &fileHeader.validDataStartOffset);
    fileHeader.AwbYuvDataOffset = ((Uint32) &fileHeader.AwbYuvData -(Uint32) &fileHeader.validDataStartOffset);
    //fileHeader.AwbMiscData
    memcpy(fileHeader.AwbMiscData, info->awbData.awbMiscData, sizeof(fileHeader.AwbMiscData));
    //fileHeader.AwbRgbData
    memcpy(fileHeader.AwbRgbData, info->awbData.awbRgbData, sizeof(fileHeader.AwbRgbData));
    //fileHeader.AwbYuvData
    memcpy(fileHeader.AwbYuvData, info->awbData.awbYuvData, sizeof(fileHeader.AwbYuvData));


    tmpBuf = malloc(fileHeader.validDataStartOffset);
    if (tmpBuf == NULL)
    {
        return OSA_EFAIL;
    }

    memset(tmpBuf, 0, fileHeader.validDataStartOffset);
    memcpy(tmpBuf, &fileHeader, sizeof(fileHeader));

    /* Check for File sending through Eathernet */
    if (flgIsRawDataSend || flgIsYuvDataSend)
    {
        IMAGE_TUNE_CopyImageData(fileHeader.validDataStartOffset, dataSize, tmpBuf, info->frameVirtAddr);
        if (flgIsRawDataSend)
        {
            flgIsRawDataSend = 0;
        }
        else
        {
            flgIsYuvDataSend = 0;
        }
        return OSA_SOK;
    }

    fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        free(tmpBuf);
        return OSA_EFAIL;
    }

    writeDataSize = fwrite(tmpBuf, 1, fileHeader.validDataStartOffset, fp);
    writeDataSize += fwrite(info->frameVirtAddr, 1, dataSize, fp);
    fclose(fp);
    free(tmpBuf);

    if (writeDataSize != dataSize + fileHeader.validDataStartOffset)
    {
        return OSA_EFAIL;
    }

    return OSA_SOK;
}

int IMAGE_TUNE_SaveParams(int paramSetId)
{
    int retVal;
    if (paramSetId < IMAGE_TUNE_DEFAULT_PARAMSET || paramSetId >= IMAGE_TUNE_MAX_PARAMSET)
    {
        return OSA_EFAIL;
    }

#ifdef IMAGE_TUNE_MULTI_PARAM
    gImageTunePrmList->curParamset = gCurPrmSet; //Update with current id
    retVal = OSA_fileWriteFile(IMAGE_TUNE_PARAMSET_DIR, (Uint8*)gImageTunePrmList, sizeof(IMAGE_TUNE_ParamSetList));
#else
    char filename[64];
    sprintf(filename, "%s/IMGPRM%02d.BIN", IMAGE_TUNE_PARAM_STORAGE_FILE_PATH, paramSetId);
    gIMAGE_TUNE_ctrl.curPrm.headerMagicNum = IMAGE_TUNE_HEADER_MAGIC_NUM;
    gIMAGE_TUNE_ctrl.curPrm.checkSum = IMAGE_TUNE_CalcParamSetChecksum(&gIMAGE_TUNE_ctrl.curPrm);
    retVal = OSA_fileWriteFile(filename, (Uint8*)&gIMAGE_TUNE_ctrl.curPrm, sizeof(IMAGE_TUNE_ParamSet));
#endif

    return retVal;
}


int IMAGE_TUNE_ReadParamset(int value)
{
    int retVal = OSA_EFAIL;
    if (gImageTunePrmList != NULL)
    {
        int readSize = 0;
        IMAGE_TUNE_ParamSetList *pPrmList = gImageTunePrmList;
        retVal = OSA_fileReadFile(IMAGE_TUNE_PARAMSET_DIR, (Uint8*)pPrmList, sizeof(IMAGE_TUNE_ParamSetList), &readSize);
        if ((retVal != OSA_SOK) || (readSize!=sizeof(IMAGE_TUNE_ParamSetList)))
        {
#ifdef DEBUG
            if (readSize == 0)
            {
                OSA_printf("IMAGE TUNE: Paramset File is not available..... Setting DEFAULT parameter\n");
            }
            else
            {
                OSA_printf("IMAGE TUNE: Error Reading Paramset File(File Size %d)..... Setting DEFAULT parameter \n", readSize);
            }
#endif
            memset(gImageTunePrmList, 0, sizeof(IMAGE_TUNE_ParamSetList));
            retVal = OSA_EFAIL;
        }
        else
        {
            retVal = OSA_SOK;
        }
    }

    return retVal;
}


int IMAGE_TUNE_SaveParamSetList(IMAGE_TUNE_ParamSet *prm, int pID)
{
    int retVal = OSA_EFAIL;

#ifdef IMAGE_TUNE_MULTI_PARAM
    IMAGE_TUNE_ParamSetList *pPrmList;
    if (gImageTunePrmList != NULL)
    {
        if (pID == IMAGE_TUNE_DEFAULT_PARAMSET)
        {
            prm = &gIMAGE_TUNE_ctrl.curPrm;
        }
        else if (prm == NULL)
        {
            return retVal;
        }

        /* Copy the parameter in to its paramset ID place in the lisit*/
        /* Make sure to save the param after this  */
        pPrmList = gImageTunePrmList;

        memcpy(&pPrmList->paramSet[pID].ccdcPrm, &prm->ccdcPrm, sizeof(IMAGE_TUNE_CcdcParams_m));
        memcpy(&pPrmList->ccdcTable.lscGainTable,  &prm->ccdcPrm.lscGainTable, sizeof(prm->ccdcPrm.lscGainTable));
        memcpy(&pPrmList->ccdcTable.lscOffsetTable,  &prm->ccdcPrm.lscOffsetTable, sizeof(prm->ccdcPrm.lscOffsetTable));
        memcpy(&pPrmList->paramSet[pID].ipipeifPrm, &prm->ipipeifPrm, sizeof(prm->ipipeifPrm));
        memcpy(&pPrmList->paramSet[pID].ipipePrm, &prm->ipipePrm, sizeof(prm->ipipePrm));
        memcpy(&pPrmList->paramSet[pID].ldcPrm, &prm->ldcPrm, sizeof(prm->ldcPrm));
        memcpy(&pPrmList->paramSet[pID].vnfPrm, &prm->vnfPrm, sizeof(prm->vnfPrm));
        memcpy(&pPrmList->paramSet[pID].awbPrm, &prm->awbPrm, sizeof(prm->awbPrm));
        retVal = OSA_SOK;
    }

#endif

    return retVal;

}

int IMAGE_TUNE_SaveDefaultParamToList(IMAGE_TUNE_Params *prm, int pID)
{
    int retVal = OSA_EFAIL;

#ifdef IMAGE_TUNE_MULTI_PARAM
    IMAGE_TUNE_ParamSetList *pPrmList;
    if (gImageTunePrmList != NULL)
    {
        if (prm == NULL)
        {
            return retVal;
        }

        pPrmList = gImageTunePrmList;

        memcpy(&pPrmList->paramSet[pID].ccdcPrm, prm->pCcdcPrm, sizeof(IMAGE_TUNE_CcdcParams_m));
        memcpy(&pPrmList->ccdcTable.lscGainTable,  prm->pCcdcPrm->lscGainTable, sizeof(prm->pCcdcPrm->lscGainTable));
        memcpy(&pPrmList->ccdcTable.lscOffsetTable,  prm->pCcdcPrm->lscOffsetTable, sizeof(prm->pCcdcPrm->lscOffsetTable));
        memcpy(&pPrmList->paramSet[pID].ipipeifPrm, prm->pIpipeifPrm, sizeof(IMAGE_TUNE_IpipeifParams));
        memcpy(&pPrmList->paramSet[pID].ipipePrm, prm->pIpipePrm, sizeof(IMAGE_TUNE_IpipeParams));
        memcpy(&pPrmList->paramSet[pID].ldcPrm, prm->pLdcPrm, sizeof(IMAGE_TUNE_LdcParams));
        memcpy(&pPrmList->paramSet[pID].vnfPrm,prm->pVnfPrm, sizeof(ALG_vnfParams));
        memcpy(&pPrmList->paramSet[pID].awbPrm, prm->pAwbPrm, sizeof(awb_calc_data_t));
        retVal = OSA_SOK;
    }
#endif

    return retVal;

}


int IMAGE_TUNE_LoadParams(int paramSetId)
{
    int retVal = OSA_EFAIL;
    Uint32 readSize = 0, pID = 0;

#ifdef IMAGE_TUNE_MULTI_PARAM
    IMAGE_TUNE_ParamSetList *pPrmList;

#if 0
    if (gImageTunePrmList != NULL)
    {
        pPrmList = gImageTunePrmList;
        retVal = OSA_fileReadFile(IMAGE_TUNE_PARAMSET_DIR, (Uint8*)pPrmList, sizeof(IMAGE_TUNE_ParamSetList), &readSize);

        if (retVal != OSA_SOK || readSize!=sizeof(IMAGE_TUNE_ParamSetList))
        {
            OSA_printf(" IMAGE TUNE:PId %d; ParamSet Size: %d, Size %d;  Set default paramset !!!\n", paramSetId, sizeof(IMAGE_TUNE_ParamSetList), readSize);
            retVal = OSA_EFAIL;
            return retVal;
        }
    }
#endif

    if (gImageTunePrmList != NULL)
    {
        pPrmList = gImageTunePrmList;
        //pID = pPrmList->curParamset;  /* Getting Parameter set ID from NAND */
        //OSA_printf(" IMAGE TUNE: param set ID: %d read from NAND!\n", pID);
        pID = paramSetId;

        if ( pID <= IMAGE_TUNE_DEFAULT_PARAMSET || pID >= IMAGE_TUNE_MAX_PARAMSET || pPrmList->paramFlag[pID]!= pID)
        {
            OSA_printf(" IMAGE TUNE: Param set ID: %d, ParamFlag: %d ; default(0) / wrong param set!!!\n", pID, pPrmList->paramFlag[pID]);
            //if( pID <= IMAGE_TUNE_DEFAULT_PARAMSET || pID >= IMAGE_TUNE_MAX_PARAMSET ){
            //OSA_printf(" IMAGE TUNE: Param set ID: %d ; default(0) / pPrmList->paramFlag[pID] %d wrong param set!!!\n", pID,0);
            //gCurPrmSet = IMAGE_TUNE_DEFAULT_PARAMSET;
            retVal = OSA_EFAIL;
            return retVal;
        }

        memcpy(&gIMAGE_TUNE_ctrl.curPrm.ccdcPrm, &pPrmList->paramSet[pID].ccdcPrm, sizeof(IMAGE_TUNE_CcdcParams_m));
        memcpy(&gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.lscGainTable, &pPrmList->ccdcTable.lscGainTable, sizeof(pPrmList->ccdcTable.lscGainTable));
        memcpy(&gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.lscOffsetTable, &pPrmList->ccdcTable.lscOffsetTable, sizeof(pPrmList->ccdcTable.lscOffsetTable));
        memcpy(&gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm, &pPrmList->paramSet[pID].ipipeifPrm, sizeof(IMAGE_TUNE_IpipeifParams));
        memcpy(&gIMAGE_TUNE_ctrl.curPrm.ipipePrm, &pPrmList->paramSet[pID].ipipePrm, sizeof(IMAGE_TUNE_IpipeParams));
        memcpy(&gIMAGE_TUNE_ctrl.curPrm.ldcPrm, &pPrmList->paramSet[pID].ldcPrm, sizeof(IMAGE_TUNE_LdcParams));
        memcpy(&gIMAGE_TUNE_ctrl.curPrm.vnfPrm, &pPrmList->paramSet[pID].vnfPrm, sizeof(ALG_vnfParams));
        memcpy(&gIMAGE_TUNE_ctrl.curPrm.awbPrm, &pPrmList->paramSet[pID].awbPrm, sizeof(awb_calc_data_t));

        OSA_printf(" IMAGE TUNE: param set ID:%d updated!!!\n", pID);
#if 0  //debug
        OSA_printf(" gIMAGE_TUNE_ctrl.curPrm.ccdcPrm %d, %p \n", sizeof(gIMAGE_TUNE_ctrl.curPrm.ccdcPrm), &gIMAGE_TUNE_ctrl.curPrm.ccdcPrm);
        OSA_printf(" gIMAGE_TUNE_ctrl.curPrm.ccdcPrm %d, %p \n", sizeof(gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.lscGainTable), &gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.lscGainTable);
        OSA_printf(" gIMAGE_TUNE_ctrl.curPrm.ccdcPrm %d, %p \n", sizeof(gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.lscOffsetTable), &gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.lscOffsetTable);
        OSA_printf(" gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm %d, %p \n", sizeof(gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm), &gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm);
        OSA_printf(" gIMAGE_TUNE_ctrl.curPrm.ipipePrm %d, %p \n", sizeof(gIMAGE_TUNE_ctrl.curPrm.ipipePrm), &gIMAGE_TUNE_ctrl.curPrm.ipipePrm);
        OSA_printf(" gIMAGE_TUNE_ctrl.curPrm.ldcPrm %d, %p \n", sizeof(gIMAGE_TUNE_ctrl.curPrm.ldcPrm), &gIMAGE_TUNE_ctrl.curPrm.ldcPrm);
        OSA_printf(" gIMAGE_TUNE_ctrl.curPrm.vnfPrm %d, %p \n", sizeof(gIMAGE_TUNE_ctrl.curPrm.vnfPrm), &gIMAGE_TUNE_ctrl.curPrm.vnfPrm);
        OSA_printf(" gIMAGE_TUNE_ctrl.curPrm.awbPrm %d, %p \n", sizeof(gIMAGE_TUNE_ctrl.curPrm.awbPrm), &gIMAGE_TUNE_ctrl.curPrm.awbPrm);
        OSA_printf(" \r\n gIMAGE_TUNE_ctrl.curPrm.ipipePrm %d, %d, %d ", gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2yuv.matrix[0][0],gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2yuv.matrix[1][1], gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2yuv.offset[0]);

#endif
        retVal = OSA_SOK;
    }

#else

    char filename[64];
    IMAGE_TUNE_ParamSet *pTmpPrmSet;
    int checkSum = 0 ;

    if (paramSetId>99)
    {
        return OSA_SOK;
    }

    pTmpPrmSet = (IMAGE_TUNE_ParamSet *)OSA_cmemAlloc(sizeof(IMAGE_TUNE_ParamSet), 32);

    sprintf(filename, "%s/IMGPRM%02d.BIN", IMAGE_TUNE_PARAM_STORAGE_FILE_PATH, paramSetId);

    retVal = OSA_fileReadFile(filename, (Uint8*)pTmpPrmSet, sizeof(IMAGE_TUNE_ParamSet), &readSize);
    if (retVal != OSA_SOK)
    {
        goto error_exit;
    }
    if (readSize != sizeof(IMAGE_TUNE_ParamSet))
    {
        OSA_ERROR("Invalid Param Set : Incorrect Size !!!\n");
        retVal = OSA_EFAIL;
        goto error_exit;
    }
    if (pTmpPrmSet->headerMagicNum != IMAGE_TUNE_HEADER_MAGIC_NUM)
    {
        OSA_ERROR("Invalid Param Set : Header Magic Num Invalid !!!\n");
        retVal = OSA_EFAIL;
        goto error_exit;
    }

    checkSum = IMAGE_TUNE_CalcParamSetChecksum(pTmpPrmSet);
    if (checkSum != pTmpPrmSet->checkSum)
    {
        OSA_ERROR("Invalid Param Set : Check Sum Failed !!!\n");
        retVal = OSA_EFAIL;
        goto error_exit;
    }

    memcpy(&gIMAGE_TUNE_ctrl.curPrm, pTmpPrmSet, sizeof(IMAGE_TUNE_ParamSet));

error_exit:
    if (pTmpPrmSet != NULL)
    {
        OSA_cmemFree((void*)pTmpPrmSet);
    }

#endif
    return retVal;
}

int IMAGE_TUNE_CalcParamSetChecksum(IMAGE_TUNE_ParamSet *prm)
{
    Uint32 *tmpAddr = &prm->paramFlags;
    Uint32 size = sizeof(IMAGE_TUNE_ParamSet) - 2*sizeof(Uint32);
    Uint32 i, checkSum=0;
    for (i = 0; i < size / 4; i++)
    {
        checkSum += *tmpAddr++;
    }

    return checkSum;
}

#ifdef  IMAGE_TUNE_MULTI_PARAM
int IMAGE_TUNE_CalcParamSetChecksum2(IMAGE_TUNE_Pkt_ParamSet *prm)
{
    Uint32 *tmpAddr = &prm->paramFlags;
    Uint32 size = sizeof(IMAGE_TUNE_Pkt_ParamSet) - 2*sizeof(Uint32);
    Uint32 i, checkSum=0;
    for (i = 0; i < size / 4; i++)
    {
        checkSum += *tmpAddr++;
    }
    return checkSum;
}
#endif

int IMAGE_TUNE_GetIsifLscParams(CSL_CcdcLscConfig *lscPrm)
{
    if (lscPrm != NULL)
    {
        memcpy(lscPrm, &gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.lsc, sizeof(*lscPrm));
    }
    return OSA_SOK;
}

int IMAGE_TUNE_SetIpipeRgb2Rgb1(CSL_IpipeRgb2RgbConfig *prm)
{
    memcpy(&gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2rgb1, prm, sizeof(*prm));
    return OSA_SOK;
}

int IMAGE_TUNE_SetIpipeRgb2Rgb2(CSL_IpipeRgb2RgbConfig *prm)
{
    memcpy(&gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2rgb2, prm, sizeof(*prm));
    return OSA_SOK;
}

int IMAGE_TUNE_SetIpipeRgb2Yuv(CSL_IpipeRgb2YuvConfig *prm)
{
    memcpy(&gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2yuv, prm, sizeof(*prm));
    return OSA_SOK;
}

int IMAGE_TUNE_SetIpipeWb(Uint16 gainR, Uint16 gainGr, Uint16 gainGb, Uint16 gainB)
{
    gIMAGE_TUNE_ctrl.curPrm.ipipePrm.wb.gain[0] = gainR;
    gIMAGE_TUNE_ctrl.curPrm.ipipePrm.wb.gain[1] = gainGr;
    gIMAGE_TUNE_ctrl.curPrm.ipipePrm.wb.gain[2] = gainGb;
    gIMAGE_TUNE_ctrl.curPrm.ipipePrm.wb.gain[3] = gainB;
    return OSA_SOK;
}

int IMAGE_TUNE_SetIpipeCntBrt(CSL_IpipeCntBrtConfig *prm)
{
    memcpy(&gIMAGE_TUNE_ctrl.curPrm.ipipePrm.cntBrt, prm, sizeof(*prm));
    return OSA_SOK;
}

int IMAGE_TUNE_SetIpipeEdgeEnhance(CSL_IpipeEdgeEnhanceConfig *prm)
{
    memcpy(&gIMAGE_TUNE_ctrl.curPrm.ipipePrm.yee, prm, sizeof(*prm));
    if (prm->table!=NULL)
    {
        memcpy(gIMAGE_TUNE_ctrl.curPrm.ipipePrm.yeeTable, prm->table, sizeof(gIMAGE_TUNE_ctrl.curPrm.ipipePrm.yeeTable));
    }
    gIMAGE_TUNE_ctrl.curPrm.ipipePrm.yee.table = gIMAGE_TUNE_ctrl.curPrm.ipipePrm.yeeTable;
    return OSA_SOK;
}

int IMAGE_TUNE_SetIpipeNf2(CSL_IpipeNfConfig *prm)
{
    memcpy(&gIMAGE_TUNE_ctrl.curPrm.ipipePrm.nf2, prm, sizeof(*prm));
    return OSA_SOK;
}
int IMAGE_TUNE_SetIsifGainOffset(CSL_CcdcGainOffsetConfig *prm)
{
    memcpy(&gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.gainOffset, prm, sizeof(*prm));
    return OSA_SOK;
}

int IMAGE_TUNE_SetIsifDcsub(Int16 dcSub)
{
    gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.clamp.dcOffset = dcSub;
    return OSA_SOK;
}

int IMAGE_TUNE_GetVnfParams(ALG_vnfParams *vnfParams)
{
    memcpy(vnfParams, &gIMAGE_TUNE_ctrl.curPrm.vnfPrm, sizeof(*vnfParams));
    return OSA_SOK;
}

int IMAGE_TUNE_SetVnfParams(ALG_vnfParams *vnfParams)
{
    memcpy(&gIMAGE_TUNE_ctrl.curPrm.vnfPrm, vnfParams, sizeof(*vnfParams));
    return OSA_SOK;
}

int IMAGE_TUNE_GetAwbParams(awb_calc_data_t *awbParams)
{
    memcpy(awbParams, &gIMAGE_TUNE_ctrl.curPrm.awbPrm, sizeof(awb_calc_data_t));
    return OSA_SOK;
}

int IMAGE_TUNE_SetAwbParams(awb_calc_data_t *awbParams)
{
    memcpy(&gIMAGE_TUNE_ctrl.curPrm.awbPrm, awbParams, sizeof(awb_calc_data_t));
    return OSA_SOK;
}


