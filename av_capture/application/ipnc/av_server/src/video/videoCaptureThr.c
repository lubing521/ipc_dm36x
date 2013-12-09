
#include <avserver.h>
#include <osa_sem.h>

extern OSA_SemHndl g_saldreSemStart;
extern OSA_SemHndl g_saldreSemDone;

int VIDEO_captureTskCreate()
{
  DRV_CaptureConfig     captureConfig;
  int status, i;
  Uint16 width, height;

  captureConfig.imgsConfig.sensorMode = gAVSERVER_config.sensorMode;
  captureConfig.imgsConfig.fps        = gAVSERVER_config.sensorFps;
  captureConfig.imgsConfig.binEnable  = gAVSERVER_config.aewb_config.aewbBinEnable;

  captureConfig.isifConfig.sensorMode 	= captureConfig.imgsConfig.sensorMode;
  captureConfig.isifConfig.vnfDemoCfg 	= gAVSERVER_config.vnfDemoEnable;
  captureConfig.isifConfig.alawEnable 	= gVIDEO_ctrl.alawEnable;
  captureConfig.isifConfig.dpcmEnable 	= FALSE;
  captureConfig.isifConfig.numBuf     	= VIDEO_NUM_BUF;
  captureConfig.isifConfig.numLscBuf    = 0;

  captureConfig.h3aConfig.sensorMode  = captureConfig.imgsConfig.sensorMode;
  captureConfig.h3aConfig.numBufAewb  = VIDEO_NUM_BUF;
  captureConfig.h3aConfig.numBufAf    = VIDEO_NUM_BUF;
  captureConfig.h3aConfig.aewbVendor  = gAVSERVER_config.aewb_config.aewbVendor;

  captureConfig.ipipeConfig.sensorMode = captureConfig.imgsConfig.sensorMode;
  captureConfig.ipipeConfig.vnfDemoCfg = gAVSERVER_config.vnfDemoEnable;

  if( gAVSERVER_config.captureRawInMode == AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN )
    captureConfig.ipipeConfig.inputSrc = DRV_IPIPE_INPUT_SRC_ISIF;
  else
    captureConfig.ipipeConfig.inputSrc = DRV_IPIPE_INPUT_SRC_DDR;

  captureConfig.ipipeConfig.boxcarBlockSize  = DRV_IPIPE_BOXCAR_BLOCK_SIZE_NONE;
  captureConfig.ipipeConfig.histogramMode    = DRV_IPIPE_HISTO_MODE_1_REGION; //ANR-Hist

  captureConfig.ipipeConfig.bscNumVectorsRow = 0;

  if(gAVSERVER_config.vstabTskEnable)
    captureConfig.ipipeConfig.bscNumVectorsRow = 3;

  captureConfig.ipipeConfig.bscNumVectorsCol        = captureConfig.ipipeConfig.bscNumVectorsRow;
  captureConfig.ipipeConfig.rszValidDataStartOffset = VIDEO_BUF_HEADER_SIZE;

  captureConfig.ipipeConfig.rszOutConfig[0].enable    = FALSE;
  captureConfig.ipipeConfig.rszOutConfig[0].numBuf    = 0;

  captureConfig.ipipeConfig.rszOutConfig[1].enable    = FALSE;
  captureConfig.ipipeConfig.rszOutConfig[1].numBuf    = 0;

  if( gAVSERVER_config.captureRawInMode == AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN ) {

    captureConfig.ipipeConfig.rszOutConfig[0].enable    = TRUE;
    captureConfig.ipipeConfig.rszOutConfig[0].outFormat = gAVSERVER_config.captureYuvFormat;
    captureConfig.ipipeConfig.rszOutConfig[0].width     = gAVSERVER_config.captureConfig[0].width;
    captureConfig.ipipeConfig.rszOutConfig[0].height    = gAVSERVER_config.captureConfig[0].height;
    captureConfig.ipipeConfig.rszOutConfig[0].flipH     = gAVSERVER_config.captureConfig[0].flipH;
    captureConfig.ipipeConfig.rszOutConfig[0].flipV     = gAVSERVER_config.captureConfig[0].flipV;
    captureConfig.ipipeConfig.rszOutConfig[0].numBuf    = VIDEO_NUM_BUF;

    if(gAVSERVER_config.numCaptureStream>1) {
      captureConfig.ipipeConfig.rszOutConfig[1].enable    = TRUE;
      captureConfig.ipipeConfig.rszOutConfig[1].outFormat = captureConfig.ipipeConfig.rszOutConfig[0].outFormat;
      captureConfig.ipipeConfig.rszOutConfig[1].width     = gAVSERVER_config.captureConfig[1].width;
      captureConfig.ipipeConfig.rszOutConfig[1].height    = gAVSERVER_config.captureConfig[1].height;
      captureConfig.ipipeConfig.rszOutConfig[1].flipH     = gAVSERVER_config.captureConfig[1].flipH;
      captureConfig.ipipeConfig.rszOutConfig[1].flipV     = gAVSERVER_config.captureConfig[1].flipV;
      captureConfig.ipipeConfig.rszOutConfig[1].numBuf    = captureConfig.ipipeConfig.rszOutConfig[0].numBuf;
    }
  }

  #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
  OSA_printf(" CAPTURE: Opening capture on imager %s.\n", drvImgsFunc->imgsGetImagerName());
  #endif

  status = DRV_captureOpen(&captureConfig, &gVIDEO_ctrl.captureInfo);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_captureOpen()\n");
    return status;
  }

  if( gAVSERVER_config.captureRawInMode == AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN ) {

    for(i=0; i<gAVSERVER_config.numCaptureStream; i++) {
      gVIDEO_ctrl.captureStream[i].captureOutOffsetH = gVIDEO_ctrl.captureInfo.ipipeInfo.rszInfo[i].offsetH;
      gVIDEO_ctrl.captureStream[i].captureOutOffsetV = gVIDEO_ctrl.captureInfo.ipipeInfo.rszInfo[i].offsetV;
      gVIDEO_ctrl.captureStream[i].captureOutWidth   = gVIDEO_ctrl.captureInfo.ipipeInfo.rszInfo[i].width;
      gVIDEO_ctrl.captureStream[i].captureOutHeight  = gVIDEO_ctrl.captureInfo.ipipeInfo.rszInfo[i].height;
      gVIDEO_ctrl.captureStream[i].captureCount      = 0;
    }

  } else {

    for(i=0; i<gAVSERVER_config.numCaptureStream; i++) {

      width = gAVSERVER_config.captureConfig[i].width;
      height = gAVSERVER_config.captureConfig[i].height;

      if(gAVSERVER_config.captureSingleResize && i>0) {

      } else {

        DRV_imgWidthHeightPad(gAVSERVER_config.sensorMode, &width, &height);
      }

      gVIDEO_ctrl.captureStream[i].captureOutWidth   = width;
      gVIDEO_ctrl.captureStream[i].captureOutHeight  = height;

      gVIDEO_ctrl.captureStream[i].captureOutOffsetH = OSA_align(gVIDEO_ctrl.captureStream[i].captureOutWidth, 32);
      gVIDEO_ctrl.captureStream[i].captureOutOffsetV = gVIDEO_ctrl.captureStream[i].captureOutHeight;//+16;
      gVIDEO_ctrl.captureStream[i].captureCount      = 0;
    }

    if(gAVSERVER_config.aewb_config.saldreEnable) {
    	gAVSERVER_config.aewb_config.saldreState = gAVSERVER_config.aewb_config.saldreEnable;
    	VIDEO_saldreCreate();
	}
  }

  return OSA_SOK;
}

int VIDEO_captureTskStart()
{
  int status;
  Bool rawOutEnable, yuvOutEnable;

  if( gAVSERVER_config.captureRawInMode == AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN ) {
    rawOutEnable = FALSE;
    yuvOutEnable = TRUE;
  } else {
    rawOutEnable = TRUE;
    yuvOutEnable = FALSE;
  }

  status = DRV_captureStart(rawOutEnable, yuvOutEnable);

  return status;
}

int VIDEO_captureTskDelete()
{
  int status;

  #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
  OSA_printf(" CAPTURE: Stopping...\n");
  #endif

  status = DRV_captureStop();

  if(gAVSERVER_config.aewb_config.saldreEnable)
  	VIDEO_saldreDelete();

  return status;
}

int VIDEO_captureMemFree()
{
  int status;

  #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
  OSA_printf(" CAPTURE: Closing...\n");
  #endif

  status = DRV_captureClose();

  return status;
}

inline void VIDEO_captureFrameRateCount(const Bool enable, const int num)
{
#if 0
    struct timeval pts;
    static int bak = 0;
    static int cntbak = 0;
    static int count = 0;
    static int frame_big = 0;
    static int frame_small = 0;
    
    if (enable && count<num)
    {
        gettimeofday(&pts, NULL);
        //OSA_printf("capture one frame(%ld, %ld) rawcnt:%d\n", pts.tv_sec, pts.tv_usec, gVIDEO_ctrl.rawCaptureCount);
        cntbak++;
        if (pts.tv_sec != bak)
        {
            if (cntbak > 25)
            {
                frame_big++;
            }
            else if (cntbak < 25)
            {
                frame_small++;
            }

            OSA_printf("fps=%d, c=%04d, >:%04d, <:%04d\n", cntbak, count, frame_big, frame_small);
            
            cntbak = 0;
            count++;
            bak = pts.tv_sec;
        }
    }
 #endif
}

int VIDEO_captureTskRunIsifIn()
{
  int status = OSA_SOK, streamId;
  int rszBufId[2];
  IMAGE_TUNE_SaveDataInfo saveDataInfo;

  VIDEO_BufHeader *pBufHeader;
  OSA_BufInfo *pBufInfo;
  int count = 0;

  VIDEO_captureFrameRateCount(1, 600000);
  
  for(streamId=0; streamId<gAVSERVER_config.numCaptureStream; streamId++) {
    status = DRV_ipipeGetRszBuf(streamId, &rszBufId[streamId], OSA_TIMEOUT_FOREVER);
    if(status!=OSA_SOK) {
      OSA_ERROR("DRV_ipipeGetRszBuf(%d)\n", streamId);
      return status;
    }

    #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" CAPTURE: Get Rsz %d Buf %d (count=%d)\n", streamId, rszBufId[streamId], gVIDEO_ctrl.captureStream[streamId].captureCount);
      #endif
    #endif

    pBufInfo = DRV_ipipeGetRszBufInfo(streamId, rszBufId[streamId]);

    OSA_assert(pBufInfo!=NULL);

    pBufHeader = (VIDEO_BufHeader*)pBufInfo->virtAddr;

    OSA_assert(pBufHeader!=NULL);

    memset(pBufHeader, 0, sizeof(*pBufHeader));

	if(gAVSERVER_config.captureConfig[streamId].mirrUpdate) {
		DRV_ipipeEnableFlip(gAVSERVER_config.captureConfig[streamId].flipH, gAVSERVER_config.captureConfig[streamId].flipV);
		gAVSERVER_config.captureConfig[streamId].mirrUpdate = FALSE;
	}

    pBufHeader->timestamp = pBufInfo->timestamp;
    pBufHeader->frameNum  = pBufInfo->count;
    pBufHeader->startX    = 0;
    pBufHeader->startY    = 0;
    pBufHeader->width     = gVIDEO_ctrl.captureInfo.ipipeInfo.rszInfo[streamId].width;
    pBufHeader->height    = gVIDEO_ctrl.captureInfo.ipipeInfo.rszInfo[streamId].height;
    pBufHeader->offsetH   = gVIDEO_ctrl.captureInfo.ipipeInfo.rszInfo[streamId].offsetH;
    pBufHeader->offsetV   = gVIDEO_ctrl.captureInfo.ipipeInfo.rszInfo[streamId].offsetV;

    gVIDEO_ctrl.rawCaptureCount++;

    if(streamId==DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID) {
      if(DRV_imageTuneIsSaveYuvData() || DRV_imageTuneIsSendYuvData()) {

        saveDataInfo.dataFormat    = gAVSERVER_config.captureYuvFormat;
        saveDataInfo.frameWidth    = pBufHeader->width;
        saveDataInfo.frameHeight   = pBufHeader->height;
        saveDataInfo.frameOffsetH  = pBufHeader->offsetH;
        saveDataInfo.frameOffsetV  = pBufHeader->offsetV;
        saveDataInfo.frameVirtAddr = pBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;

        DRV_imageTuneSaveData(&saveDataInfo);
      }
      if(DRV_imageTuneIsSaveRawData() ||DRV_imageTuneIsSendRawData()) {

        int rawBufId, numRawSkipBuf=2;
        OSA_BufInfo *pRawBufInfo;

        DRV_isifOutEnable(TRUE);

        while(numRawSkipBuf) {
          DRV_isifGetOutBuf(&rawBufId, OSA_TIMEOUT_FOREVER);
          DRV_isifPutOutBuf(rawBufId);
          numRawSkipBuf--;
        }

        DRV_isifGetOutBuf(&rawBufId, OSA_TIMEOUT_FOREVER);

        pRawBufInfo = DRV_isifGetOutBufInfo(rawBufId);

        if(pRawBufInfo!=NULL) {
          saveDataInfo.dataFormat    = DRV_DATA_FORMAT_RAW;
          saveDataInfo.frameWidth    = gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataWidth;
          saveDataInfo.frameHeight   = gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataHeight;
          saveDataInfo.frameOffsetH  = gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataOffsetH;
          saveDataInfo.frameOffsetV  = saveDataInfo.frameHeight;
          saveDataInfo.frameVirtAddr = pRawBufInfo->virtAddr;

          DRV_imageTuneSaveData(&saveDataInfo);
        }

        DRV_isifPutOutBuf(rawBufId);
        DRV_isifOutEnable(FALSE);
      }

    }

#if 0
    VIDEO_vsGetResult(VIDEO_TSK_CAPTURE, streamId, pBufInfo);

    status = DRV_ipipeGetHistogram(); //ANR-Hist
    if (status){ // != OSA_EFAIL){
        OSA_ERROR("\r\n Histogram IsIfIn %d ", status);
    }

    VIDEO_fdCopyRun(VIDEO_TSK_CAPTURE, streamId, pBufInfo);
#endif

    VIDEO_swosdRun(VIDEO_TSK_CAPTURE, streamId, pBufInfo);

    pBufInfo = AVSERVER_bufPutFull( VIDEO_TSK_CAPTURE, streamId, rszBufId[streamId]);
    if(pBufInfo==NULL) {
      OSA_ERROR("AVSERVER_bufPutFull(%d, %d)\n", streamId, rszBufId[streamId]);
      return OSA_EFAIL;
    }
    
#if 0
    VIDEO_displayCopyRun(VIDEO_TSK_CAPTURE, streamId, pBufInfo);
#endif

    if(gVIDEO_ctrl.captureStream[streamId].captureCount>0) {

      #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
        #ifdef AVSERVER_DEBUG_RUNNING
        OSA_printf(" CAPTURE: GetEmpty Rsz %d\n", streamId);
        #endif
      #endif

      pBufInfo = AVSERVER_bufGetEmpty( VIDEO_TSK_CAPTURE, streamId, &rszBufId[streamId], OSA_TIMEOUT_FOREVER);
      if(pBufInfo!=NULL) {
        DRV_ipipePutRszBuf(streamId, rszBufId[streamId]);

        #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
          #ifdef AVSERVER_DEBUG_RUNNING
          OSA_printf(" CAPTURE: Put Rsz %d Buf %d\n", streamId, rszBufId[streamId]);
          #endif
        #endif
      }
    }
    gVIDEO_ctrl.captureStream[streamId].captureCount++;
  }

  return status;
}

int VIDEO_captureRszFunc(Uint8 *inVirtAddr, OSA_BufInfo **pOutBufInfo, int *rszBufId, int numStreams)
{
    int status=OSA_SOK, streamId;
    IMAGE_TUNE_SaveDataInfo saveDataInfo;
    VIDEO_BufHeader *pBufHeader;
    DRV_ReszRunPrm reszPrm;
    DRV_ReszOutPrm reszOutPrm[2];

    #ifdef YUV_MODE
    reszPrm.inType = DRV_DATA_FORMAT_YUV422;
    #else
    reszPrm.inType = DRV_DATA_FORMAT_RAW;
    #endif

    reszPrm.inVirtAddr = inVirtAddr;
    reszPrm.inPhysAddr = OSA_cmemGetPhysAddr(inVirtAddr);

    reszPrm.inStartX = 0;
    reszPrm.inStartY = 1;
    reszPrm.inWidth  = gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataWidth;
    reszPrm.inHeight = gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataHeight;

    if(gVIDEO_ctrl.alawEnable)
      reszPrm.inOffsetH= gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataOffsetH;
    else
      reszPrm.inOffsetH= gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataOffsetH/2;

    if (gAVSERVER_config.aewb_config.saldreEnable== TRUE){
      reszPrm.inWidth  = (reszPrm.inWidth < SALDRE_MAX_IMGWIDTH  ? reszPrm.inWidth : SALDRE_MAX_IMGWIDTH);
      reszPrm.inHeight  = (reszPrm.inHeight < SALDRE_MAX_IMGHEIGHT  ? reszPrm.inHeight : SALDRE_MAX_IMGHEIGHT);
    }

    if (gAVSERVER_config.aewb_config.saldreEnable== FALSE) {
      Uint32 offset;
      offset= (gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataWidth - reszPrm.inWidth) + reszPrm.inOffsetH*(gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataHeight - reszPrm.inHeight);

      reszPrm.inVirtAddr+= offset;
      reszPrm.inPhysAddr+= offset;

    }

    reszPrm.inOffsetV= reszPrm.inHeight;
    reszPrm.enableInvAlaw = gVIDEO_ctrl.alawEnable;
    reszPrm.enableInvDpcm = FALSE;
    reszPrm.clkDivM = 10;
    reszPrm.clkDivN = gAVSERVER_config.resizerClkdivN;

    streamId = 0;
    while(streamId < numStreams) {
      if(pOutBufInfo[streamId]!=NULL)
        break;
      streamId++;
    }

    if(streamId < numStreams) {

      reszPrm.pOut[0] = NULL;
      reszPrm.pOut[1] = NULL;

      reszPrm.pOut[0] = &reszOutPrm[0];

      reszOutPrm[0].outType = gAVSERVER_config.captureYuvFormat;
      reszOutPrm[0].flipH = gAVSERVER_config.captureConfig[streamId].flipH;
      reszOutPrm[0].flipV = gAVSERVER_config.captureConfig[streamId].flipV;
      reszOutPrm[0].outVirtAddr = pOutBufInfo[streamId]->virtAddr + VIDEO_BUF_HEADER_SIZE;
      reszOutPrm[0].outPhysAddr = pOutBufInfo[streamId]->physAddr + VIDEO_BUF_HEADER_SIZE;
      reszOutPrm[0].outWidth    = gVIDEO_ctrl.captureStream[streamId].captureOutWidth;
      reszOutPrm[0].outHeight   = gVIDEO_ctrl.captureStream[streamId].captureOutHeight;
      reszOutPrm[0].outOffsetH  = gVIDEO_ctrl.captureStream[streamId].captureOutOffsetH;
      reszOutPrm[0].outOffsetV  = gVIDEO_ctrl.captureStream[streamId].captureOutOffsetV;

      streamId++;
      while(streamId < numStreams) {
        if(pOutBufInfo[streamId]!=NULL)
          break;
        streamId++;
      }

      if(streamId < numStreams) {

        reszPrm.pOut[1] = &reszOutPrm[1];

        reszOutPrm[1].outType = reszOutPrm[0].outType;
        reszOutPrm[1].flipH = gAVSERVER_config.captureConfig[streamId].flipH;
        reszOutPrm[1].flipV = gAVSERVER_config.captureConfig[streamId].flipV;
        reszOutPrm[1].outVirtAddr = pOutBufInfo[streamId]->virtAddr + VIDEO_BUF_HEADER_SIZE;
        reszOutPrm[1].outPhysAddr = pOutBufInfo[streamId]->physAddr + VIDEO_BUF_HEADER_SIZE;
        reszOutPrm[1].outWidth    = gVIDEO_ctrl.captureStream[streamId].captureOutWidth;
        reszOutPrm[1].outHeight   = gVIDEO_ctrl.captureStream[streamId].captureOutHeight;
        reszOutPrm[1].outOffsetH  = gVIDEO_ctrl.captureStream[streamId].captureOutOffsetH;
        reszOutPrm[1].outOffsetV  = gVIDEO_ctrl.captureStream[streamId].captureOutOffsetV;
      }

      if(gAVSERVER_config.sensorMode == DRV_IMGS_SENSOR_MODE_2592x1920)
      {
        reszPrm.inWidth = gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataWidth/2;
        reszOutPrm[0].outWidth = gVIDEO_ctrl.captureStream[0].captureOutWidth/2;

        if(gAVSERVER_config.captureConfig[0].flipH){
          reszOutPrm[0].outVirtAddr = pOutBufInfo[0]->virtAddr + VIDEO_BUF_HEADER_SIZE
            + gVIDEO_ctrl.captureStream[0].captureOutOffsetH/2;
          reszOutPrm[0].outPhysAddr = pOutBufInfo[0]->physAddr + VIDEO_BUF_HEADER_SIZE
            + gVIDEO_ctrl.captureStream[0].captureOutOffsetH/2;
        }

        OSA_prfBegin(&gAVSERVER_ctrl.ipipePrf);
        status = DRV_reszRun(&reszPrm);
        OSA_prfEnd(&gAVSERVER_ctrl.ipipePrf, 1);

        reszPrm.inType = DRV_DATA_FORMAT_RAW;
        reszPrm.inVirtAddr = inVirtAddr;
        reszPrm.inPhysAddr = OSA_cmemGetPhysAddr(inVirtAddr);
        reszPrm.inStartX = gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataWidth/2;
        reszPrm.inStartY = 1;
        reszPrm.inWidth  = gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataWidth/2;
        reszPrm.inHeight = gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataHeight;

        if(gVIDEO_ctrl.alawEnable)
          reszPrm.inOffsetH= gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataOffsetH;
        else
          reszPrm.inOffsetH= gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataOffsetH/2;

        reszPrm.inOffsetV= reszPrm.inHeight;
        reszPrm.enableInvAlaw = gVIDEO_ctrl.alawEnable;
        reszPrm.enableInvDpcm = FALSE;
        reszPrm.clkDivM = 10;
        reszPrm.clkDivN = gAVSERVER_config.resizerClkdivN;

        reszPrm.pOut[0] = NULL;
        reszPrm.pOut[1] = NULL;

        reszPrm.pOut[0] = &reszOutPrm[0];

        reszOutPrm[0].outType = gAVSERVER_config.captureYuvFormat;
        reszOutPrm[0].flipH = gAVSERVER_config.captureConfig[0].flipH;
        reszOutPrm[0].flipV = gAVSERVER_config.captureConfig[0].flipV;
        if(gAVSERVER_config.captureConfig[0].flipH){
          reszOutPrm[0].outVirtAddr = pOutBufInfo[0]->virtAddr + VIDEO_BUF_HEADER_SIZE;
          reszOutPrm[0].outPhysAddr = pOutBufInfo[0]->physAddr + VIDEO_BUF_HEADER_SIZE;
        }
        else{
          reszOutPrm[0].outVirtAddr = pOutBufInfo[0]->virtAddr + VIDEO_BUF_HEADER_SIZE
            + gVIDEO_ctrl.captureStream[0].captureOutOffsetH/2;
          reszOutPrm[0].outPhysAddr = pOutBufInfo[0]->physAddr + VIDEO_BUF_HEADER_SIZE
            + gVIDEO_ctrl.captureStream[0].captureOutOffsetH/2;
        }
        reszOutPrm[0].outWidth	  = gVIDEO_ctrl.captureStream[0].captureOutWidth/2;
        reszOutPrm[0].outHeight   = gVIDEO_ctrl.captureStream[0].captureOutHeight;
        reszOutPrm[0].outOffsetH  = gVIDEO_ctrl.captureStream[0].captureOutOffsetH;
        reszOutPrm[0].outOffsetV  = gVIDEO_ctrl.captureStream[0].captureOutOffsetV;
      }

      OSA_prfBegin(&gAVSERVER_ctrl.ipipePrf);

      status = DRV_reszRun(&reszPrm);

      OSA_prfEnd(&gAVSERVER_ctrl.ipipePrf, 1);

      if(status!=OSA_SOK)
        OSA_ERROR("DRV_reszRun()\n");


      if(DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID < 2) {
        if(DRV_imageTuneIsSaveYuvData() || DRV_imageTuneIsSendYuvData()) {

          saveDataInfo.dataFormat    = reszOutPrm[DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID].outType;
          saveDataInfo.frameWidth    = reszOutPrm[DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID].outWidth;
          saveDataInfo.frameHeight   = reszOutPrm[DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID].outHeight;
          saveDataInfo.frameOffsetH  = reszOutPrm[DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID].outOffsetH;
          saveDataInfo.frameOffsetV  = reszOutPrm[DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID].outOffsetV;
          saveDataInfo.frameVirtAddr = pOutBufInfo[DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID]->virtAddr + VIDEO_BUF_HEADER_SIZE;

          DRV_imageTuneSaveData(&saveDataInfo);
        }
      }

      streamId++;
      while(streamId < numStreams) {
        if(pOutBufInfo[streamId]!=NULL)
          break;
        streamId++;
      }

      if(streamId < numStreams) {

        reszPrm.inType = reszOutPrm[1].outType;
        reszPrm.inVirtAddr = reszOutPrm[1].outVirtAddr;
        reszPrm.inPhysAddr = reszOutPrm[1].outPhysAddr;
        reszPrm.inStartX = 0;
        reszPrm.inStartY = 0;
        reszPrm.inWidth  = reszOutPrm[1].outWidth;
        reszPrm.inHeight = reszOutPrm[1].outHeight;
        reszPrm.inOffsetH= reszOutPrm[1].outOffsetH;
        reszPrm.inOffsetV= reszOutPrm[1].outOffsetV;
        reszPrm.enableInvAlaw = FALSE;
        reszPrm.enableInvDpcm = FALSE;
        reszPrm.clkDivM = 10;
        reszPrm.clkDivN = gAVSERVER_config.resizerClkdivN;

        reszPrm.pOut[0] = NULL;
        reszPrm.pOut[1] = NULL;

        reszPrm.pOut[0] = &reszOutPrm[0];

        reszOutPrm[0].outType 	  = reszPrm.inType;
        reszOutPrm[0].flipH = FALSE;
        reszOutPrm[0].flipV = FALSE;
        reszOutPrm[0].outVirtAddr = pOutBufInfo[streamId]->virtAddr + VIDEO_BUF_HEADER_SIZE;
        reszOutPrm[0].outPhysAddr = pOutBufInfo[streamId]->physAddr + VIDEO_BUF_HEADER_SIZE;
        reszOutPrm[0].outWidth    = gVIDEO_ctrl.captureStream[streamId].captureOutWidth;
        reszOutPrm[0].outHeight   = gVIDEO_ctrl.captureStream[streamId].captureOutHeight;
        reszOutPrm[0].outOffsetH  = gVIDEO_ctrl.captureStream[streamId].captureOutOffsetH;
        reszOutPrm[0].outOffsetV  = gVIDEO_ctrl.captureStream[streamId].captureOutOffsetV;

        streamId++;
        while(streamId < numStreams) {
          if(pOutBufInfo[streamId]!=NULL)
            break;
          streamId++;
        }

        if(streamId < numStreams) {

          reszPrm.pOut[1] = &reszOutPrm[1];

          reszOutPrm[1].outType = reszOutPrm[0].outType;
          reszOutPrm[1].flipH = FALSE;
          reszOutPrm[1].flipV = FALSE;
          reszOutPrm[1].outVirtAddr = pOutBufInfo[streamId]->virtAddr + VIDEO_BUF_HEADER_SIZE;
          reszOutPrm[1].outPhysAddr = pOutBufInfo[streamId]->physAddr + VIDEO_BUF_HEADER_SIZE;
          reszOutPrm[1].outWidth    = gVIDEO_ctrl.captureStream[streamId].captureOutWidth;
          reszOutPrm[1].outHeight   = gVIDEO_ctrl.captureStream[streamId].captureOutHeight;
          reszOutPrm[1].outOffsetH  = gVIDEO_ctrl.captureStream[streamId].captureOutOffsetH;
          reszOutPrm[1].outOffsetV  = gVIDEO_ctrl.captureStream[streamId].captureOutOffsetV;
        }

        OSA_prfBegin(&gAVSERVER_ctrl.resizePrf);

        status = DRV_reszRun(&reszPrm);

        OSA_prfEnd(&gAVSERVER_ctrl.resizePrf, 1);

        if(DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID >= 2) {
          if(DRV_imageTuneIsSaveYuvData() || DRV_imageTuneIsSendYuvData()) {

            saveDataInfo.dataFormat    = reszOutPrm[DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID-2].outType;
            saveDataInfo.frameWidth    = reszOutPrm[DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID-2].outWidth;
            saveDataInfo.frameHeight   = reszOutPrm[DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID-2].outHeight;
            saveDataInfo.frameOffsetH  = reszOutPrm[DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID-2].outOffsetH;
            saveDataInfo.frameOffsetV  = reszOutPrm[DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID-2].outOffsetV;
            saveDataInfo.frameVirtAddr = pOutBufInfo[DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID]->virtAddr + VIDEO_BUF_HEADER_SIZE;

            DRV_imageTuneSaveData(&saveDataInfo);

            OSA_printf(" CAPTURE: %dx%d %dx%d\n",
              saveDataInfo.frameOffsetH, saveDataInfo.frameOffsetV,
              saveDataInfo.frameWidth, saveDataInfo.frameHeight
              );
          }
        }

        if(status!=OSA_SOK)
          OSA_ERROR("DRV_reszRun()\n");
      }
    }

    for(streamId=0; streamId<numStreams; streamId++) {

      if(pOutBufInfo[streamId]!=NULL) {

        pBufHeader = (VIDEO_BufHeader*)pOutBufInfo[streamId]->virtAddr;

        status = DRV_ipipeGetHistogram();
        if (status){ // != OSA_EFAIL){
          OSA_ERROR("\r\n Histogram DdrIn %d ", status);
        }

        VIDEO_vsGetResult(VIDEO_TSK_CAPTURE, streamId, pOutBufInfo[streamId]);

        VIDEO_fdCopyRun(VIDEO_TSK_CAPTURE, streamId, pOutBufInfo[streamId]);

        VIDEO_swosdRun(VIDEO_TSK_CAPTURE, streamId, pOutBufInfo[streamId]);

        AVSERVER_bufPutFull( VIDEO_TSK_CAPTURE, streamId, rszBufId[streamId]);

        VIDEO_displayCopyRun(VIDEO_TSK_CAPTURE, streamId, pOutBufInfo[streamId]);
      }
    }

    return 0;
}

int VIDEO_captureTskRunDdrIn()
{
  int status=OSA_SOK, streamId, rawBufId, rszBufId[AVSERVER_MAX_STREAMS], numStreams;
  OSA_BufInfo *pRawBufInfo, *pOutBufInfo[AVSERVER_MAX_STREAMS];
  IMAGE_TUNE_SaveDataInfo saveDataInfo;
  VIDEO_BufHeader *pBufHeader;
  Uint32 frameSkip;
  Uint8 *saldreOutputBufAddr;

  VIDEO_captureFrameRateCount(1, 600000);
  
  status = DRV_isifGetOutBuf(&rawBufId, OSA_TIMEOUT_FOREVER);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_isifGetOutBuf()\n");
    return status;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
    #ifdef AVSERVER_DEBUG_RUNNING
    OSA_printf(" CAPTURE: Isif Buf %d\n", rawBufId);
    #endif
  #endif

  pRawBufInfo = DRV_isifGetOutBufInfo(rawBufId);

  if(gAVSERVER_config.aewb_config.saldreEnable)
  	VIDEO_saldreSetBufInfo(rawBufId, (Uint8*)pRawBufInfo->virtAddr);

  OSA_assert(pRawBufInfo!=NULL);

  if(pRawBufInfo!=NULL) {

    if(!gAVSERVER_config.captureSingleResize)
      numStreams = gAVSERVER_config.numCaptureStream;
    else
      numStreams = 1;

    gVIDEO_ctrl.rawCaptureCount++;

    for(streamId=0; streamId<numStreams; streamId++) {

      frameSkip = (gVIDEO_ctrl.rawCaptureCount-1)%30;

      if(! (gAVSERVER_config.captureConfig[streamId].frameSkipMask & (1<<frameSkip)) )
      {
        pOutBufInfo[streamId] = NULL;
        continue;
      }

      pOutBufInfo[streamId] = AVSERVER_bufGetEmpty( VIDEO_TSK_CAPTURE, streamId, &rszBufId[streamId], OSA_TIMEOUT_FOREVER);

      OSA_assert(pOutBufInfo[streamId]!=NULL);

      pBufHeader = (VIDEO_BufHeader*)pOutBufInfo[streamId]->virtAddr;

      OSA_assert(pBufHeader!=NULL);

      memset(pBufHeader, 0, sizeof(*pBufHeader));

	  if(gAVSERVER_config.captureConfig[streamId].mirrUpdate) {
		DRV_ipipeEnableFlip(gAVSERVER_config.captureConfig[streamId].flipH, gAVSERVER_config.captureConfig[streamId].flipV);
		gAVSERVER_config.captureConfig[streamId].mirrUpdate = FALSE;
	  }

      pBufHeader->timestamp = pRawBufInfo->timestamp;
      pBufHeader->frameNum  = gVIDEO_ctrl.rawCaptureCount;
      pBufHeader->startX    = 0;
      pBufHeader->startY    = 0;
      pBufHeader->width     = gVIDEO_ctrl.captureStream[streamId].captureOutWidth;
      pBufHeader->height    = gVIDEO_ctrl.captureStream[streamId].captureOutHeight;
      pBufHeader->offsetH   = gVIDEO_ctrl.captureStream[streamId].captureOutOffsetH;
      pBufHeader->offsetV   = gVIDEO_ctrl.captureStream[streamId].captureOutOffsetV;
    }

	if (gAVSERVER_config.aewb_config.saldreEnable == FALSE){
		VIDEO_captureRszFunc(pRawBufInfo->virtAddr, pOutBufInfo, rszBufId, numStreams);
	}
	else {
		saldreOutputBufAddr= VIDEO_saldreGetOutputBufAddr();
		OSA_semSignal(&g_saldreSemStart);
		VIDEO_captureRszFunc(saldreOutputBufAddr, pOutBufInfo, rszBufId, numStreams);
		OSA_semWait(&g_saldreSemDone, OSA_TIMEOUT_FOREVER);
	}
  }

  if(DRV_imageTuneIsSaveRawData() || DRV_imageTuneIsSendRawData()) {

      if(pRawBufInfo!=NULL) {
        saveDataInfo.dataFormat    = DRV_DATA_FORMAT_RAW;
        saveDataInfo.frameWidth    = gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataWidth;
        saveDataInfo.frameHeight   = gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataHeight;
        saveDataInfo.frameOffsetH  = gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataOffsetH;
        saveDataInfo.frameOffsetV  = saveDataInfo.frameHeight;
        saveDataInfo.frameVirtAddr = pRawBufInfo->virtAddr;

        DRV_imageTuneSaveData(&saveDataInfo);
      }
    }

	if (gAVSERVER_config.aewb_config.saldreEnable) {
		VIDEO_saldreIsifPutOutBuf();
	}
	else {
		if (gAVSERVER_config.aewb_config.saldreState) {
			VIDEO_saldreStop();
		}
		DRV_isifPutOutBuf(rawBufId);
	}

    gAVSERVER_config.aewb_config.saldreState = gAVSERVER_config.aewb_config.saldreEnable;

	return status;
}

int VIDEO_captureTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  OSA_setTskName("capture");
  
  #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
  OSA_printf(" CAPTURE: Recevied CMD = 0x%04x\n", cmd);
  #endif

  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
  OSA_printf(" CAPTURE: Create...\n");
  #endif

  status = VIDEO_captureTskCreate();

  OSA_tskAckOrFreeMsg(pMsg, status);

  if(status!=OSA_SOK) {
    OSA_ERROR("VIDEO_captureTskCreate()");
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
  OSA_printf(" CAPTURE: Create...DONE\n");
  #endif

  status = OSA_tskWaitMsg(pTsk, &pMsg);
  if(status!=OSA_SOK)
    return OSA_SOK;

  cmd = OSA_msgGetCmd(pMsg);

  if(cmd==AVSERVER_CMD_DELETE) {

    done = TRUE;
    ackMsg = TRUE;

  } else {

    #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
    OSA_printf(" CAPTURE: Start...\n");
    #endif

    VIDEO_captureTskStart();

    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

    #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
    OSA_printf(" CAPTURE: Start...DONE\n");
    #endif
  }

    while(!done) 
    {
        //VIDEO_aewbApplyPrm();
        OSA_prfBegin(&gAVSERVER_ctrl.capturePrf);

        if( gAVSERVER_config.captureRawInMode == AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN ) 
        {
            status = VIDEO_captureTskRunIsifIn();
        } 
        else if( gAVSERVER_config.captureRawInMode == AVSERVER_CAPTURE_RAW_IN_MODE_DDR_IN ) 
        {
            status = VIDEO_captureTskRunDdrIn();
        }

        OSA_prfEnd(&gAVSERVER_ctrl.capturePrf, 1);

        if(status!=OSA_SOK)
            break;

        if(gVIDEO_ctrl.rawCaptureCount % (gAVSERVER_config.sensorFps*3) == 0)
        {
            AVSERVER_profileInfoShow();
 	    }

        status = OSA_tskCheckMsg(pTsk, &pMsg);

        if(status!=OSA_SOK)
            continue;

        cmd = OSA_msgGetCmd(pMsg);

        #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
        OSA_printf(" CAPTURE: Recevied CMD = 0x%04x\n", cmd);
        #endif

        switch(cmd) 
        {
        case AVSERVER_CMD_DELETE:
            done = TRUE;
            ackMsg = TRUE;
            break;
        default:
            OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
        }
    }

    #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
    OSA_printf(" CAPTURE: Delete...\n");
    #endif

    VIDEO_captureTskDelete();

    if(ackMsg)
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

    #ifdef AVSERVER_DEBUG_VIDEO_CAPTURE_THR
        OSA_printf(" CAPTURE: Delete...DONE\n");
    #endif
    return OSA_SOK;
}

int VIDEO_captureCreate()
{
    int status;

    status = OSA_tskCreate(&gVIDEO_ctrl.captureTsk, VIDEO_captureTskMain, VIDEO_CAPTURE_THR_PRI, VIDEO_CAPTURE_STACK_SIZE, 0);
    if(status!=OSA_SOK) 
    {
        OSA_ERROR("OSA_tskCreate()\n");
        return status;
    }

    return status;
}

int VIDEO_captureDelete()
{
    int status;

    status = OSA_tskDelete(&gVIDEO_ctrl.captureTsk);

    if(status!=OSA_SOK) 
    {
        OSA_ERROR("OSA_tskDelete()\n");
    }
    return status;
}

