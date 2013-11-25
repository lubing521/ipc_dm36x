
#include <drv_captureTestPriv.h>

static DRV_CaptureTestCtrl gDRV_testCtrl;

int DRV_captureTestOpen()
{
  int status;

  gDRV_testCtrl.captureConfig.imgsConfig.sensorMode = DRV_CAPTURE_TEST_SENSOR_MODE;
  gDRV_testCtrl.captureConfig.imgsConfig.binEnable  = FALSE;
  gDRV_testCtrl.captureConfig.imgsConfig.fps        = 30;

  gDRV_testCtrl.captureConfig.isifConfig.sensorMode = gDRV_testCtrl.captureConfig.imgsConfig.sensorMode;
  gDRV_testCtrl.captureConfig.isifConfig.alawEnable = FALSE;
  gDRV_testCtrl.captureConfig.isifConfig.dpcmEnable = FALSE;
  gDRV_testCtrl.captureConfig.isifConfig.numBuf     = DRV_CAPTURE_TEST_NUM_BUF;
  gDRV_testCtrl.captureConfig.isifConfig.numLscBuf  = 0;

  #ifdef DRV_CAPTURE_TEST_ENABLE_LSC
  gDRV_testCtrl.captureConfig.isifConfig.numLscBuf = DRV_CAPTURE_TEST_NUM_BUF;
  #endif

  gDRV_testCtrl.captureConfig.isifConfig.lscConfig.paxelWidth   = 16;
  gDRV_testCtrl.captureConfig.isifConfig.lscConfig.paxelHeight  = 8;
  gDRV_testCtrl.captureConfig.isifConfig.lscConfig.gainFormat   = 0;
  gDRV_testCtrl.captureConfig.isifConfig.lscConfig.offsetEnable = TRUE;
  gDRV_testCtrl.captureConfig.isifConfig.lscConfig.offsetScale  = 0x80;
  gDRV_testCtrl.captureConfig.isifConfig.lscConfig.offsetShift  = 0;
  gDRV_testCtrl.captureConfig.isifConfig.lscConfig.initX        = 0;
  gDRV_testCtrl.captureConfig.isifConfig.lscConfig.initY        = 0;

  gDRV_testCtrl.captureConfig.h3aConfig.sensorMode  = gDRV_testCtrl.captureConfig.imgsConfig.sensorMode;
  gDRV_testCtrl.captureConfig.h3aConfig.numBufAewb  = DRV_CAPTURE_TEST_NUM_BUF;
  gDRV_testCtrl.captureConfig.h3aConfig.numBufAf    = DRV_CAPTURE_TEST_NUM_BUF;
  gDRV_testCtrl.captureConfig.h3aConfig.aewbVendor  = APPRO_2A;

  gDRV_testCtrl.captureConfig.ipipeConfig.sensorMode              = gDRV_testCtrl.captureConfig.imgsConfig.sensorMode;
  gDRV_testCtrl.captureConfig.ipipeConfig.inputSrc                = DRV_IPIPE_INPUT_SRC_ISIF;
  gDRV_testCtrl.captureConfig.ipipeConfig.boxcarBlockSize         = DRV_IPIPE_BOXCAR_BLOCK_SIZE_NONE;
  gDRV_testCtrl.captureConfig.ipipeConfig.boxcarInShift           = 0;
  gDRV_testCtrl.captureConfig.ipipeConfig.histogramMode           = DRV_IPIPE_HISTO_MODE_NONE;

  gDRV_testCtrl.captureConfig.ipipeConfig.bscNumVectorsRow        = 0;

  #ifdef DRV_CAPTURE_TEST_ENABLE_BSC
  gDRV_testCtrl.captureConfig.ipipeConfig.bscNumVectorsRow        = 3;
  #endif

  #ifdef DRV_CAPTURE_TEST_ENABLE_BOXCAR
  gDRV_testCtrl.captureConfig.ipipeConfig.boxcarBlockSize         = DRV_IPIPE_BOXCAR_BLOCK_SIZE_8x8;
  #endif

  gDRV_testCtrl.captureConfig.ipipeConfig.bscNumVectorsCol        = gDRV_testCtrl.captureConfig.ipipeConfig.bscNumVectorsRow;
  gDRV_testCtrl.captureConfig.ipipeConfig.rszValidDataStartOffset = 0;

  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[0].enable    = TRUE;
  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[0].outFormat = DRV_CAPTURE_TEST_DATA_FORMAT;
  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[0].width     = 1280;
  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[0].height    = 720;
  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[0].flipH     = TRUE;
  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[0].flipV     = TRUE;
  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[0].numBuf    = DRV_CAPTURE_TEST_NUM_BUF;

  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[1].enable    = TRUE;
  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[1].outFormat = gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[0].outFormat;
  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[1].width     = 640;
  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[1].height    = 480;
  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[1].flipH     = gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[0].flipH;
  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[1].flipV     = gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[0].flipV;
  gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[1].numBuf    = gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[0].numBuf;

  gDRV_testCtrl.displayConfig.winId       = DRV_DISPLAY_VID_WIN_0;
  gDRV_testCtrl.displayConfig.numBuf      = DRV_CAPTURE_TEST_NUM_BUF;
  gDRV_testCtrl.displayConfig.dataFormat  = gDRV_testCtrl.captureConfig.ipipeConfig.rszOutConfig[0].outFormat;
  gDRV_testCtrl.displayConfig.startX      = 0;
  gDRV_testCtrl.displayConfig.startY      = 0;
  gDRV_testCtrl.displayConfig.width       = 640;
  gDRV_testCtrl.displayConfig.height      = 480;
  gDRV_testCtrl.displayConfig.offsetH     = OSA_align(gDRV_testCtrl.displayConfig.width, 32);
  gDRV_testCtrl.displayConfig.offsetV     = gDRV_testCtrl.displayConfig.height;
  gDRV_testCtrl.displayConfig.zoomH       = 0;
  gDRV_testCtrl.displayConfig.zoomV       = 0;
  gDRV_testCtrl.displayConfig.bufFlag     = 0;

  gDRV_testCtrl.displayBufId=0;

  OSA_printf(" CAPTURE: Opening display.\n");

  status = DRV_displaySetMode(DRV_DISPLAY_MODE_NTSC);
  if(status!=OSA_SOK)
    goto error_exit;

  status = DRV_displayOpen(&gDRV_testCtrl.displayHndl, &gDRV_testCtrl.displayConfig);
  if(status!=OSA_SOK)
    goto error_exit;

  DRV_displayGetBufInfo(&gDRV_testCtrl.displayHndl, &gDRV_testCtrl.displayInfo);

  OSA_printf(" CAPTURE: Opening capture on imager %s.\n", DRV_imgsGetImagerName());

  status = DRV_captureOpen(&gDRV_testCtrl.captureConfig, &gDRV_testCtrl.captureInfo);
  if(status!=OSA_SOK)
    goto capture_exit;

  #ifdef DRV_CAPTURE_TEST_ENABLE_FD
  status = DRV_faceDetectOpen();
  if(status!=OSA_SOK)
    goto fd_exit;
  #endif

  DRV_captureTestLscTableInit(gDRV_testCtrl.captureConfig.isifConfig.numLscBuf);

  #ifdef DRV_CAPTURE_TEST_ENABLE_VS
  {
    ALG_VstabCreate vsCreatePrm;
    DRV_ImgsModeConfig *pImgsMode;

    pImgsMode = DRV_imgsGetModeConfig(gDRV_testCtrl.captureConfig.imgsConfig.sensorMode);

    if(pImgsMode==NULL)
      goto alloc_exit;

    vsCreatePrm.totalFrameWidth  = pImgsMode->validWidth;
    vsCreatePrm.totalFrameHeight = pImgsMode->validHeight;
    vsCreatePrm.stabFrameWidth   = (vsCreatePrm.totalFrameWidth*100)/120;
    vsCreatePrm.stabFrameHeight  = (vsCreatePrm.totalFrameHeight*100)/120;
    vsCreatePrm.pBscConfig       = &gDRV_testCtrl.captureInfo.ipipeInfo.bscInfo;

    gDRV_testCtrl.vsHndl = ALG_vstabCreate(&vsCreatePrm);
    if(gDRV_testCtrl.vsHndl==NULL)
      goto alloc_exit;
  }
  #endif


  if(gDRV_testCtrl.tmpBufSize) {
    gDRV_testCtrl.tmpBufCurSize = 0;

    gDRV_testCtrl.tmpBufVirtAddr = OSA_cmemAlloc(gDRV_testCtrl.tmpBufSize, 32);
    gDRV_testCtrl.tmpBufPhysAddr = OSA_cmemGetPhysAddr(gDRV_testCtrl.tmpBufVirtAddr);

    if(gDRV_testCtrl.tmpBufVirtAddr==NULL || gDRV_testCtrl.tmpBufPhysAddr==NULL)
      goto alloc_exit;
  }

  return OSA_SOK;

alloc_exit:
  #ifdef DRV_CAPTURE_TEST_ENABLE_FD
  DRV_faceDetectClose();
  #endif

fd_exit:
  DRV_captureClose();

capture_exit:
  DRV_displayClose(&gDRV_testCtrl.displayHndl);

error_exit:

  OSA_ERROR("Open failed\n");

  return OSA_EFAIL;
}

int DRV_captureTestBoxcarRun(int count)
{
  int status, i, j, numBlocks;
  int bufId=-1;
  OSA_BufInfo *pBufInfo;

  Uint16 *curLineAddr;
  Uint8  *curAddr;
  int accValue[3];

  status = DRV_ipipeGetBoxcarBuf(&bufId, OSA_TIMEOUT_FOREVER);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_ipipeGetBoxcarBuf()\n");
    return status;
  }

  pBufInfo =   DRV_ipipeGetBoxcarBufInfo(bufId);
  if(pBufInfo) {

    #ifdef DRV_CAPTURE_TEST_SAVE_BOXCAR_DATA
    if(count>1) {
      static char filename[80];

      if(count%(30*10)==0) {
        sprintf(filename, "BOX_%04d_%dx%d.BIN", count, gDRV_testCtrl.captureInfo.ipipeInfo.boxcarInfo.offsetH, gDRV_testCtrl.captureInfo.ipipeInfo.boxcarInfo.height);
        OSA_fileWriteFile(filename, pBufInfo->virtAddr, pBufInfo->size);
      }
    }
    #endif

    if(count%(30*5)==0) {
      curAddr = pBufInfo->virtAddr;

      accValue[0] = 0;
      accValue[1] = 0;
      accValue[2] = 0;

      for(i=0; i<gDRV_testCtrl.captureInfo.ipipeInfo.boxcarInfo.width; i++) {

        curLineAddr = (Uint16*)curAddr;

        for(j=0; j<gDRV_testCtrl.captureInfo.ipipeInfo.boxcarInfo.height; j++) {

          accValue[0] += (curLineAddr[0] & 0xFFFF);
          accValue[1] += (curLineAddr[1] & 0xFFFF);
          accValue[2] += (curLineAddr[2] & 0xFFFF);

          curLineAddr+=4;
        }
        curAddr += gDRV_testCtrl.captureInfo.ipipeInfo.boxcarInfo.offsetH;
      }

      numBlocks = gDRV_testCtrl.captureInfo.ipipeInfo.boxcarInfo.width*gDRV_testCtrl.captureInfo.ipipeInfo.boxcarInfo.height;

      accValue[0] /= numBlocks;
      accValue[1] /= numBlocks;
      accValue[2] /= numBlocks;

      #ifdef DRV_CAPTURE_TEST_PRINT_BOXCAR_INFO
      OSA_printf(" Boxcar Avg Color: R=%5d, G=%5d, B=%5d\n", accValue[2], accValue[1], accValue[0]);
      #endif
    }
  }

  DRV_ipipePutBoxcarBuf(bufId);

  return status;
}

int DRV_captureTestLscTableInit(Uint16 numLscBuf)
{
  int i;
  Uint8 curGain, curOffset;
  int bufId;
  OSA_BufInfo *pBufInfo;
  Uint8 *curAddr;

  for(bufId=0; bufId<numLscBuf; bufId++)
  {
    OSA_printf(" CAPTURE: LSC: Buffer init %d, %dx%d, (%d, %d)\n",
      bufId,
      gDRV_testCtrl.captureInfo.isifInfo.lscDataWidth,
      gDRV_testCtrl.captureInfo.isifInfo.lscDataHeight,
      gDRV_testCtrl.captureInfo.isifInfo.lscDataOffsetH,
      gDRV_testCtrl.captureInfo.isifInfo.lscOffsetTableOffsetInBuf
      );

    pBufInfo = DRV_isifLscGetBufInfo(bufId);

    if(pBufInfo!=NULL)
    {
      curAddr = pBufInfo->virtAddr;
      curGain = 0;
      curOffset=0;

      OSA_cmemCacheWbInv(pBufInfo->virtAddr, gDRV_testCtrl.captureInfo.isifInfo.lscOffsetTableOffsetInBuf*2);

      for(i=0; i<gDRV_testCtrl.captureInfo.isifInfo.lscDataHeight+1; i++)
      {
        memset(curAddr, curGain, gDRV_testCtrl.captureInfo.isifInfo.lscDataWidth*4);
        memset(curAddr+gDRV_testCtrl.captureInfo.isifInfo.lscOffsetTableOffsetInBuf, curOffset, gDRV_testCtrl.captureInfo.isifInfo.lscDataWidth*4);

        curAddr+=gDRV_testCtrl.captureInfo.isifInfo.lscDataOffsetH;

        #if 1
        curGain += 8;
        curOffset += 8;
        #endif
      }

      OSA_cmemCacheWbInv(pBufInfo->virtAddr, gDRV_testCtrl.captureInfo.isifInfo.lscOffsetTableOffsetInBuf*2);
    }
  }

  return OSA_SOK;
}


int DRV_captureTestLscRun(int count)
{
  int status;
  int bufId;
  OSA_BufInfo *pBufInfo;

  status = DRV_isifLscGetBuf(&bufId, OSA_TIMEOUT_FOREVER);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_isifLscGetBuf()\n");
    return status;
  }

  pBufInfo =   DRV_isifLscGetBufInfo(bufId);
  if(pBufInfo) {

    #ifdef DRV_CAPTURE_TEST_SAVE_LSC_DATA
    if(count>1) {
      static char filename[80];

      if(count%(30*10)==0) {
        sprintf(filename, "LSC_%04d_%dx%d.BIN", count, gDRV_testCtrl.captureInfo.isifInfo.lscDataOffsetH, gDRV_testCtrl.captureInfo.isifInfo.lscDataHeight);
        OSA_fileWriteFile(filename, pBufInfo->virtAddr, pBufInfo->size);
      }
    }
    #endif

    if(count%(30*5)==0) {

    }
  }

  DRV_isifLscPutBuf(bufId);

  return status;
}

int DRV_captureTestH3aRun(int count)
{
  int status;
  int bufIdAewb;
  OSA_BufInfo *pBufInfo;

  int accValue[4];

  CSL_H3aAewbOutSumModeOverlay *pAewbWinData;
  Uint8 *curAewbAddr;
  int numWin, i, j, avgG, aGain;

  DRV_IpipeWb wb;

  status = DRV_h3aGetBuf(DRV_H3A_MOD_AEWB, &bufIdAewb, OSA_TIMEOUT_FOREVER);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_h3aGetBuf()\n");
    return status;
  }

  pBufInfo =   DRV_h3aGetBufInfo(DRV_H3A_MOD_AEWB, bufIdAewb);
  if(pBufInfo) {

    #ifdef DRV_CAPTURE_TEST_SAVE_H3A_DATA
    if(count>1) {
      static char filename[80];

      if(count%100==0) {
        sprintf(filename, "H3A_%04d_%dx%d.BIN", count, gDRV_testCtrl.captureInfo.h3aInfo.aewbNumWinH, gDRV_testCtrl.captureInfo.h3aInfo.aewbNumWinV);
        OSA_fileWriteFile(filename, pBufInfo->virtAddr, pBufInfo->size);
      }
    }
    #endif


    if(count%(30*5)==0) {
      curAewbAddr = pBufInfo->virtAddr;
      numWin=0;

      accValue[0]=accValue[1]=accValue[2]=accValue[3]=0;

      for(i=0; i<gDRV_testCtrl.captureInfo.h3aInfo.aewbNumWinV; i++) {
        for(j=0; j<gDRV_testCtrl.captureInfo.h3aInfo.aewbNumWinH; j++) {

          pAewbWinData = (CSL_H3aAewbOutSumModeOverlay *)curAewbAddr;

          accValue[0] += pAewbWinData->subSampleAcc[0];
          accValue[1] += pAewbWinData->subSampleAcc[1];
          accValue[2] += pAewbWinData->subSampleAcc[2];
          accValue[3] += pAewbWinData->subSampleAcc[3];

          #if 0
          OSA_printf(" (%2d:%2d) %5d, %5d, %5d, %5d\n", i, j,
                pAewbWinData->subSampleAcc[0], pAewbWinData->subSampleAcc[1],
                pAewbWinData->subSampleAcc[2], pAewbWinData->subSampleAcc[3]
                );
          #endif

          curAewbAddr += sizeof(CSL_H3aAewbOutSumModeOverlay);

          numWin++;

          if(numWin%8==0)
            curAewbAddr += sizeof(CSL_H3aAewbOutUnsatBlkCntOverlay);
        }
        curAewbAddr = (Uint8*)OSA_align( (Uint32)curAewbAddr, 32);
      }

      accValue[0] /= numWin*gDRV_testCtrl.captureInfo.h3aInfo.aewbNumSamplesPerColorInWin;
      accValue[1] /= numWin*gDRV_testCtrl.captureInfo.h3aInfo.aewbNumSamplesPerColorInWin;
      accValue[2] /= numWin*gDRV_testCtrl.captureInfo.h3aInfo.aewbNumSamplesPerColorInWin;
      accValue[3] /= numWin*gDRV_testCtrl.captureInfo.h3aInfo.aewbNumSamplesPerColorInWin;

      #ifdef DRV_CAPTURE_TEST_APPLY_2A
      // do simple AWB
      wb.gainGr = 0x200;
      wb.gainGb = 0x200;

      wb.gainR  = (wb.gainGr*accValue[0])/accValue[1];
      wb.gainB  = (wb.gainGb*accValue[3])/accValue[2];

      DRV_ipipeSetWb(&wb);

      // do simple AE
      avgG = (accValue[0]+accValue[3])/2;

      aGain = 8000 - (avgG*4000)/512;

      DRV_imgsSetAgain(aGain, 0);

      #ifdef DRV_CAPTURE_TEST_PRINT_H3A_INFO
      OSA_printf(" AEWB Avg Color: %5d, %5d, %5d, %5d (AE aGain = %d)\n", accValue[0], accValue[1], accValue[2], accValue[3], aGain);
      #endif

      #endif

    }
  }

  DRV_h3aPutBuf(DRV_H3A_MOD_AEWB, bufIdAewb);

  return status;
}

int DRV_captureTestBscRun(int count)
{
  int status, i, j;
  int bufIdBsc;
  OSA_BufInfo *pBufInfo;
  int rowSum[10], colSum[10];
  Uint16 *curAddr;

  status = DRV_ipipeGetBscBuf(&bufIdBsc, OSA_TIMEOUT_FOREVER);
  if(status!=OSA_SOK) {
    return status;
  }

  pBufInfo =   DRV_ipipeGetBscBufInfo(bufIdBsc);
  if(pBufInfo) {

    #ifdef DRV_CAPTURE_TEST_SAVE_BSC_DATA
    if(count>1) {
      static char filename[80];

      if(count%100==0) {
        sprintf(filename, "BSC_%04d.BIN", count);
        OSA_fileWriteFile(filename, pBufInfo->virtAddr, pBufInfo->size);
      }
    }
    #endif

    #ifdef DRV_CAPTURE_TEST_ENABLE_VS
    {
      ALG_VstabRunPrm vsPrm;

      vsPrm.bscDataVirtAddr = pBufInfo->virtAddr;
      vsPrm.bscDataPhysAddr = pBufInfo->physAddr;

      OSA_prfBegin(&gDRV_testCtrl.vsPrf);

      status = ALG_vstabRun(gDRV_testCtrl.vsHndl, &vsPrm, &gDRV_testCtrl.vsStatus);

      OSA_prfEnd(&gDRV_testCtrl.vsPrf, 1);

      if(status!=OSA_SOK) {
        OSA_ERROR("ALG_vstabRun()\n");
      }

      if(count%(30*5)==0) {
        OSA_prfPrint(&gDRV_testCtrl.vsPrf, "VS", 0);
      }
    }
    #endif

    if(count%(30*3)==0) {
      curAddr = (Uint16*)pBufInfo->virtAddr;

      for(i=0; i<gDRV_testCtrl.captureInfo.ipipeInfo.bscInfo.rowNumVectors; i++) {
        rowSum[i] = 0;

        for(j=0; j<gDRV_testCtrl.captureInfo.ipipeInfo.bscInfo.rowNumV; j++)  {
          rowSum[i] += *curAddr;
          curAddr++;
        }

        rowSum[i] /= gDRV_testCtrl.captureInfo.ipipeInfo.bscInfo.rowNumV;
      }

      curAddr = (Uint16*)(pBufInfo->virtAddr + DRV_IPIPE_BSC_BUFFER_MAX/2);

      for(i=0; i<gDRV_testCtrl.captureInfo.ipipeInfo.bscInfo.colNumVectors; i++) {
        colSum[i] = 0;

        for(j=0; j<gDRV_testCtrl.captureInfo.ipipeInfo.bscInfo.colNumH; j++)  {
          colSum[i] += *curAddr;
          curAddr++;
        }

        colSum[i] /= gDRV_testCtrl.captureInfo.ipipeInfo.bscInfo.colNumH;
      }

      #ifdef DRV_CAPTURE_TEST_PRINT_BSC_INFO
      OSA_printf(" BSC: Row [%8d, %8d, %8d] Col [%8d, %8d, %8d]\n",
          rowSum[0], rowSum[1], rowSum[2],
          colSum[0], colSum[1], colSum[2]
          );
      #endif
    }
  }

  DRV_ipipePutBscBuf(bufIdBsc);

  return status;
}

void DRV_CaptureTestInitRtPrms(DRV_IpipeReszRtPrm *rszRtPrm1, DRV_IpipeReszRtPrm *rszRtPrm2)
{
  DRV_IpipeReszRtPrm *rszRtPrm;
  int rszIndx = CSL_RSZ_B;

  rszRtPrm = rszRtPrm1;
  rszRtPrm->inWidth   = 1280;
  rszRtPrm->inHeight  = 720;
  rszRtPrm->inStartX  = 0;
  rszRtPrm->inStartY  = 0;

  rszRtPrm->pOut[0].enableRtChange = FALSE;
  rszRtPrm->pOut[rszIndx].enableRtChange = TRUE;
  rszRtPrm->pOut[rszIndx].outHeight  = 198;
  rszRtPrm->pOut[rszIndx].outWidth   = 352;

//  rszIndx++;

  rszRtPrm = rszRtPrm2;
  rszRtPrm->inWidth	    = 960;
  rszRtPrm->inHeight	= 720;
  rszRtPrm->inStartX	= 0; //(1280-960)/2; //image will get centered automatically??
  rszRtPrm->inStartY	= 0;
  rszRtPrm->pOut[0].enableRtChange = FALSE;
  rszRtPrm->pOut[rszIndx].enableRtChange = TRUE;
  rszRtPrm->pOut[rszIndx].outHeight  = 480;
  rszRtPrm->pOut[rszIndx].outWidth   = 640;

}

int DRV_captureTestRun()
{
  int status;
  int bufIdRszA, bufIdRszB;
  DRV_FrameCopyPrm frameCopy;
  OSA_BufInfo *pBufInfo;
  static int count=0;
  Uint32 copySize;
  DRV_IpipeReszRtPrm rszRtPrm1, rszRtPrm2;
  static int rszRtPrmFlag=1;

  DRV_CaptureTestInitRtPrms(&rszRtPrm1, &rszRtPrm2);

  status = DRV_ipipeGetRszBuf(DRV_IPIPE_RSZ_A, &bufIdRszA, OSA_TIMEOUT_FOREVER);
  if(status!=OSA_SOK) {
    return status;
  }
  status = DRV_ipipeGetRszBuf(DRV_IPIPE_RSZ_B, &bufIdRszB, OSA_TIMEOUT_FOREVER);
  if(status!=OSA_SOK) {
    return status;
  }

  status = DRV_captureTestH3aRun(count);
  if(status!=OSA_SOK) {
    return status;
  }

  #ifdef DRV_CAPTURE_TEST_ENABLE_BSC
  status = DRV_captureTestBscRun(count);
  if(status!=OSA_SOK) {
    return status;
  }
  #endif

  #ifdef DRV_CAPTURE_TEST_ENABLE_BOXCAR
  status =   DRV_captureTestBoxcarRun(count);
  if(status!=OSA_SOK) {
    return status;
  }
  #endif

  #ifdef DRV_CAPTURE_TEST_ENABLE_LSC
  status =   DRV_captureTestLscRun(count);
  if(status!=OSA_SOK) {
    return status;
  }
  #endif

  count++;

  /* update runtime change of resizer parameters */
  #if 1
  if(count%90==0) {
	  OSA_printf(" Dynamic update of Resizer Params to %d \n",
						 rszRtPrmFlag );

	if(rszRtPrmFlag == 0){
		DRV_ipipeSetReszRtPrms(&rszRtPrm1);
		rszRtPrmFlag = 1;
	} else {
		DRV_ipipeSetReszRtPrms(&rszRtPrm2);
		rszRtPrmFlag = 0;
	}
  }
  #endif
  // copy rszA buffer to display

  pBufInfo =   DRV_ipipeGetRszBufInfo(DRV_IPIPE_RSZ_A, bufIdRszA);
  if(pBufInfo) {

    #ifdef DRV_CAPTURE_TEST_SAVE_RSZ_A_DATA
    if(count>1) {
      static char filename[80];

      if(count%100==0) {
        sprintf(filename, "IMG_%04d_%dx%d.YUV", count, gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].offsetH, gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].offsetV);
        OSA_fileWriteFile(filename, pBufInfo->virtAddr, gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].offsetH*gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].offsetV*2);
      }
    }
    #endif

#ifdef DRV_CAPTURE_TEST_ENABLE_RSZ_A_COPY
    frameCopy.srcPhysAddr = pBufInfo->physAddr;
    frameCopy.srcVirtAddr = pBufInfo->virtAddr;
    frameCopy.dstPhysAddr = gDRV_testCtrl.displayInfo.physAddr[gDRV_testCtrl.displayBufId];
    frameCopy.dstVirtAddr = gDRV_testCtrl.displayInfo.virtAddr[gDRV_testCtrl.displayBufId];
    frameCopy.srcOffsetH  = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].offsetH;
    frameCopy.srcOffsetV  = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].offsetV;
    frameCopy.dstOffsetH  = gDRV_testCtrl.displayInfo.offsetH;
    frameCopy.dstOffsetV  = gDRV_testCtrl.displayInfo.offsetV;
    frameCopy.copyWidth   = gDRV_testCtrl.displayInfo.width;
    frameCopy.copyHeight  = gDRV_testCtrl.displayInfo.height;
    frameCopy.dataFormat  = gDRV_testCtrl.displayInfo.dataFormat;
    frameCopy.srcStartX   = 0;//(gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].width-gDRV_testCtrl.displayInfo.width)/2;
    frameCopy.srcStartY   = 0;//(gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].height-gDRV_testCtrl.displayInfo.height)/2;

    #ifdef DRV_CAPTURE_TEST_ENABLE_VS
    frameCopy.srcStartX   += gDRV_testCtrl.vsStatus.startX;
    frameCopy.srcStartY   += gDRV_testCtrl.vsStatus.startY;
    #endif

    frameCopy.dstStartX   = 0;
    frameCopy.dstStartY   = 0;

    status = DRV_frameCopy(NULL, &frameCopy);
    if(status!=OSA_SOK) {
      OSA_ERROR("DRV_frameCopy(A)\n");
    }

    if(gDRV_testCtrl.tmpBufSize) {
      copySize = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].height * gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].width;

      if(gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].outFormat==DRV_DATA_FORMAT_YUV420)
        copySize += copySize/2;
      else
        copySize *= 2;

      if((gDRV_testCtrl.tmpBufCurSize + copySize) > gDRV_testCtrl.tmpBufSize)
        gDRV_testCtrl.tmpBufCurSize = 0;

      frameCopy.srcPhysAddr = pBufInfo->physAddr;
      frameCopy.srcVirtAddr = pBufInfo->virtAddr;
      frameCopy.dstPhysAddr = gDRV_testCtrl.tmpBufPhysAddr + gDRV_testCtrl.tmpBufCurSize;
      frameCopy.dstVirtAddr = gDRV_testCtrl.tmpBufVirtAddr + gDRV_testCtrl.tmpBufCurSize;
      frameCopy.srcOffsetH  = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].offsetH;
      frameCopy.srcOffsetV  = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].offsetV;
      frameCopy.dstOffsetH  = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].width;
      frameCopy.dstOffsetV  = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].height;
      frameCopy.copyWidth   = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].width;
      frameCopy.copyHeight  = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].height;
      frameCopy.dataFormat  = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_A].outFormat;
      frameCopy.srcStartX   = 0;
      frameCopy.srcStartY   = 0;
      frameCopy.dstStartX   = 0;
      frameCopy.dstStartY   = 0;

      status = DRV_frameCopy(NULL, &frameCopy);
      if(status!=OSA_SOK) {
        OSA_ERROR("DRV_frameCopy(A)\n");
      }

      gDRV_testCtrl.tmpBufCurSize += copySize;
    }
#endif
  }


  // copy rszB buffer to display
  pBufInfo =   DRV_ipipeGetRszBufInfo(DRV_IPIPE_RSZ_B, bufIdRszB);
  if(pBufInfo) {

    #ifdef DRV_CAPTURE_TEST_ENABLE_FD
    if(count%(1)==0)
    {
      DRV_FaceDetectRunPrm  facePrm;
      DRV_FaceDetectRunStatus faceStatus;
      int i;

      facePrm.inPhysAddr = pBufInfo->physAddr;
      facePrm.inWidth    = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_B].offsetH;
      facePrm.inHeight   = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_B].offsetV;
      facePrm.detectThres= 8;
      facePrm.detectCondition = DRV_FACE_DETECT_CONDITION_DEFAULT;

      status = DRV_faceDetectRun(&facePrm, &faceStatus);
      if(status==OSA_SOK) {
        if(faceStatus.numFaces) {
          for(i=0; i<faceStatus.numFaces; i++) {
            #if 0
            OSA_printf(" Face %2d of %2d : centre(X,Y) = (%3d,%3d),  angle = %3d, size = %d, confidence = %d \n",
                   1+i,
                   faceStatus.numFaces,
                   faceStatus.info[i].centerX,
                   faceStatus.info[i].centerY,
                   faceStatus.info[i].angle,
                   CSL_FACE_DETECT_GET_SIZE(faceStatus.info[i].sizeConfidence),
                   CSL_FACE_DETECT_GET_CONFIDENCE(faceStatus.info[i].sizeConfidence)
                   );
            #endif
          }
        }
      }
    }
    #endif

    #ifdef DRV_CAPTURE_TEST_ENABLE_RSZ_B_COPY
    frameCopy.srcPhysAddr = pBufInfo->physAddr;
    frameCopy.srcVirtAddr = pBufInfo->virtAddr;
    frameCopy.dstPhysAddr = gDRV_testCtrl.displayInfo.physAddr[gDRV_testCtrl.displayBufId];
    frameCopy.dstVirtAddr = gDRV_testCtrl.displayInfo.virtAddr[gDRV_testCtrl.displayBufId];
    frameCopy.srcOffsetH  = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_B].offsetH;
    frameCopy.srcOffsetV  = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_B].offsetV;
    frameCopy.dstOffsetH  = gDRV_testCtrl.displayInfo.offsetH;
    frameCopy.dstOffsetV  = gDRV_testCtrl.displayInfo.offsetV;
    frameCopy.copyWidth   = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_B].width;
    frameCopy.copyHeight  = gDRV_testCtrl.captureInfo.ipipeInfo.rszInfo[DRV_IPIPE_RSZ_B].height;
    frameCopy.dataFormat  = gDRV_testCtrl.displayInfo.dataFormat;
    frameCopy.srcStartX   = 0;
    frameCopy.srcStartY   = 0;
    frameCopy.dstStartX   = 0;
    frameCopy.dstStartY   = 0;

    status = DRV_frameCopy(NULL, &frameCopy);
    if(status!=OSA_SOK) {
      OSA_ERROR("DRV_frameCopy(B)\n");
    }
    #endif
  }


  DRV_ipipePutRszBuf(DRV_IPIPE_RSZ_A, bufIdRszA);
  DRV_ipipePutRszBuf(DRV_IPIPE_RSZ_B, bufIdRszB);
  DRV_displaySwitchBuf( &gDRV_testCtrl.displayHndl, &gDRV_testCtrl.displayBufId, 0, NULL);

  return status;
}

void DRV_captureTestSignalHandler(int signum)
{
  gDRV_testCtrl.testQuit=1;
}

int DRV_captureTestMain(int argc, char **argv)
{
  int status;

  memset(&gDRV_testCtrl, 0, sizeof(gDRV_testCtrl));

  gDRV_testCtrl.testQuit = 0;

  OSA_attachSignalHandler(SIGINT, DRV_captureTestSignalHandler);

  status = DRV_init();
  if(status!=OSA_SOK)
    return status;

  status = DRV_captureTestOpen();
  if(status!=OSA_SOK)
    goto open_exit;

  status = DRV_captureStart(FALSE, TRUE);
  if(status!=OSA_SOK)
    goto start_exit;

  OSA_printf(" CAPTURE: Running.\n");

  status = OSA_SOK;
  while(!gDRV_testCtrl.testQuit && status==OSA_SOK)
    status = DRV_captureTestRun();

  DRV_captureStop();

start_exit:

  OSA_printf(" CAPTURE: Closing capture.\n");
  DRV_captureClose();

  OSA_printf(" CAPTURE: Closing display.\n");
  DRV_displayClose(&gDRV_testCtrl.displayHndl);

  #ifdef DRV_CAPTURE_TEST_ENABLE_FD
  DRV_faceDetectClose();
  #endif

  #ifdef DRV_CAPTURE_TEST_ENABLE_VS
  ALG_vstabDelete(gDRV_testCtrl.vsHndl);
  #endif

  if(gDRV_testCtrl.tmpBufSize) {
//    OSA_fileWriteFile("DATA_DUMP.YUV", gDRV_testCtrl.tmpBufVirtAddr, gDRV_testCtrl.tmpBufSize);
    OSA_cmemFree(gDRV_testCtrl.tmpBufVirtAddr);
  }

open_exit:
  OSA_printf(" CAPTURE: Exiting.\n");
  DRV_exit();
  OSA_printf(" CAPTURE: Bye Bye !!!\n");

  return status;
}

