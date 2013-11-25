
#include <alg_priv.h>
#include <alg_vidDec.h>
#include <alg_jpgDec.h>
#include <osa_cmem.h>
#include <osa_file.h>
#include <drv_display.h>
#include <drv_frameCopy.h>

//#define ALG_VID_DEC_DEBUG
//#define ALG_VID_DEC_TEST_DEBUG

#define ALG_VID_DEC_MAX_BITRATE (4*1024*1024)

typedef struct {

  VIDDEC2_Handle          hDecode;
  VIDDEC2_Status          decStatus;
  VIDDEC2_Params          params;
  VIDDEC2_DynamicParams   dynamicParams;
  char                    algName[20];

  ALG_VidDecCreate        createPrm;

  int curFrameNum;
  int outStartX;
  int outStartY;

  void *hJpgDecode;

} ALG_VidDecObj;

void *ALG_vidDecCreate(ALG_VidDecCreate *create)
{
  ALG_VidDecObj *pObj;

  pObj = OSA_memAlloc(sizeof(ALG_VidDecObj));

  if(pObj==NULL)
    return NULL;

  memset(pObj, 0, sizeof(*pObj));

  memcpy(&pObj->createPrm, create, sizeof(pObj->createPrm));

  switch(create->codec)
  {
    case ALG_VID_CODEC_H264:
      strcpy(pObj->algName, ALG_VID_CODEC_H264_DEC_NAME);
      break;
    case ALG_VID_CODEC_MPEG4:
      strcpy(pObj->algName, ALG_VID_CODEC_MPEG4_DEC_NAME);
      break;
    case ALG_VID_CODEC_MJPEG:
    {
      ALG_JpgDecCreate jpgDecCreate;

      jpgDecCreate.dataFormat = create->dataFormat;
      jpgDecCreate.maxWidth = create->maxWidth;
      jpgDecCreate.maxHeight = create->maxHeight;

      pObj->hJpgDecode = ALG_jpgDecCreate(&jpgDecCreate);

      if(pObj->hJpgDecode==NULL)
        return NULL;

      return pObj;
    }
      break;
    default:
      OSA_memFree(pObj);
      return NULL;
  }

  pObj->params.size               = sizeof(VIDDEC2_Params);
  pObj->params.maxHeight          = create->maxHeight;
  pObj->params.maxWidth           = create->maxWidth;
  pObj->params.maxFrameRate       = 0;
  pObj->params.maxBitRate         = ALG_VID_DEC_MAX_BITRATE;
  pObj->params.dataEndianness     = XDM_BYTE;
  pObj->params.forceChromaFormat  = 9;

  /* Create video decoder instance */
  pObj->hDecode = VIDDEC2_create(gALG_hEngine, pObj->algName, &pObj->params);
  if (pObj->hDecode == NULL) {
    OSA_ERROR("Failed to open video decode algorithm (%s)\n", pObj->algName);
    OSA_memFree(pObj);
    return NULL;
  }

  return (void*)pObj;
}

int ALG_vidDecRun(void *hndl, ALG_VidDecRunPrm *prm, ALG_VidDecRunStatus *runStatus)
{
  VIDDEC2_InArgs  inArgs;
  VIDDEC2_OutArgs outArgs;
  XDM1_BufDesc    inBufDesc;
  XDM_BufDesc     outBufDesc;
  XDAS_Int32      outBufSizeArray[2];
  XDAS_Int32      status;
  XDAS_Int8		    *outBufPtrs[2];
  ALG_VidDecObj   *pObj;

  pObj = (ALG_VidDecObj *)hndl;

  if(pObj==NULL)
    return OSA_EFAIL;

  if(pObj->createPrm.codec == ALG_VID_CODEC_MJPEG) {

    ALG_JpgDecRunPrm jpgDecRun;
    ALG_JpgDecRunStatus jpgDecStatus;

    jpgDecRun.inAddr = prm->inAddr;
    jpgDecRun.inDataSize = prm->inDataSize;
    jpgDecRun.outAddr = prm->outAddr;
    jpgDecRun.outOffsetH = prm->outOffsetH;
    jpgDecRun.outOffsetV = prm->outOffsetV;
    jpgDecRun.outStartX = 0;
    jpgDecRun.outStartY = 0;

    status = ALG_jpgDecRun(pObj->hJpgDecode, &jpgDecRun, &jpgDecStatus);
    if(status!=OSA_SOK)
      return OSA_EFAIL;

    runStatus->bytesUsed  = jpgDecStatus.bytesUsed;
    runStatus->isKeyFrame = TRUE;
    runStatus->frameWidth = jpgDecStatus.frameWidth;
    runStatus->frameHeight= jpgDecStatus.frameHeight;
    runStatus->outputBufId= prm->inputBufId;
    runStatus->freeBufId  = prm->inputBufId;
    runStatus->outStartX  = jpgDecRun.outStartX;
    runStatus->outStartY  = jpgDecRun.outStartY;
    runStatus->outOffsetH = jpgDecRun.outOffsetH;
    runStatus->outOffsetV = jpgDecRun.outOffsetV;

    return OSA_SOK;
  }

  if(pObj->hDecode==NULL)
    return OSA_EFAIL;

  if(pObj->curFrameNum==0)
  {
    pObj->dynamicParams.size          = sizeof(VIDDEC2_DynamicParams);
    pObj->dynamicParams.decodeHeader  = XDM_DECODE_AU;
    pObj->dynamicParams.displayWidth  = prm->outOffsetH;
    pObj->dynamicParams.frameSkipMode = IVIDEO_NO_SKIP;

    pObj->decStatus.size     = sizeof(VIDDEC2_Status);
    pObj->decStatus.data.buf = NULL;

    status = VIDDEC2_control(pObj->hDecode, XDM_SETPARAMS, &pObj->dynamicParams, &pObj->decStatus);
    if (status != VIDDEC2_EOK) {
      OSA_ERROR("XDM_SETPARAMS failed, status=%ld\n", status);
      return OSA_EFAIL;
    }

    /* Get buffer information from video decoder */
    pObj->decStatus.size     = sizeof(VIDDEC2_Status);
    pObj->decStatus.data.buf = NULL;

    pObj->dynamicParams.size = sizeof(VIDDEC2_DynamicParams);
    status = VIDDEC2_control(pObj->hDecode, XDM_GETBUFINFO, &pObj->dynamicParams, &pObj->decStatus);
    if (status != VIDDEC2_EOK) {
      OSA_ERROR("XDM_GETBUFINFO failed, status=%ld\n", status);
      return OSA_EFAIL;
    }

    #ifdef ALG_VID_DEC_DEBUG
    OSA_printf(" ALG: VidDec: XDM_GETBUFINFO: min out bufs:%ld,size:%ld %ld\n",pObj->decStatus.bufInfo.minNumOutBufs,pObj->decStatus.bufInfo.minOutBufSize[0], pObj->decStatus.bufInfo.minOutBufSize[1]);
    #endif
  }

  pObj->dynamicParams.size      = sizeof(VIDDEC2_DynamicParams);

  pObj->decStatus.size          = sizeof(VIDDEC2_Status);
  pObj->decStatus.data.buf      = NULL;

  outBufSizeArray[0] = (prm->outOffsetH)*(prm->outOffsetV);
  outBufSizeArray[1] = outBufSizeArray[0]/2;

  inBufDesc.descs[0].bufSize = prm->inDataSize;
  inBufDesc.descs[0].buf  = (XDAS_Int8 *)prm->inAddr;
  inBufDesc.numBufs       = 1;

  outBufPtrs[0] = (XDAS_Int8 *)prm->outAddr;
  outBufPtrs[1] = (XDAS_Int8 *)(prm->outAddr + outBufSizeArray[0]) ;

  outBufDesc.bufSizes     = outBufSizeArray;
  outBufDesc.bufs         = (XDAS_Int8 **) &outBufPtrs;
  outBufDesc.numBufs      = 2;

  inArgs.size             = sizeof(VIDDEC2_InArgs);
  inArgs.numBytes         = prm->inDataSize;
  inArgs.inputID          = prm->inputBufId+1; // must be greater than 0

  outArgs.size            = sizeof(VIDDEC2_OutArgs);

  status = VIDDEC2_process(pObj->hDecode, &inBufDesc, &outBufDesc, &inArgs, &outArgs);

  runStatus->bytesUsed = outArgs.bytesConsumed;

  if (status != VIDDEC2_EOK) {
    OSA_ERROR("status=%ld\n", status);
    return OSA_EFAIL;
  }

  switch (outArgs.displayBufs[0].frameType) {
    case IVIDEO_I_FRAME:
      runStatus->isKeyFrame = TRUE;
      break;
    case IVIDEO_P_FRAME:
      runStatus->isKeyFrame = FALSE;
      break;
    case IVIDEO_B_FRAME:
      runStatus->isKeyFrame = FALSE;
      break;
    case IVIDEO_IDR_FRAME:
      runStatus->isKeyFrame = TRUE;
      break;
  }

  status = VIDDEC2_control(pObj->hDecode, XDM_GETSTATUS, &pObj->dynamicParams, &pObj->decStatus);
  if (status != VIDDEC2_EOK) {
    OSA_ERROR("XDM_GETSTATUS failed, status=%ld\n", status);
    return OSA_EFAIL;
  }

  runStatus->frameWidth = pObj->decStatus.outputWidth;
  runStatus->frameHeight = pObj->decStatus.outputHeight;

  if(outArgs.displayBufs[0].bufDesc[0].buf == NULL) {
    return OSA_EFAIL;
  }
  else {
    runStatus->outputBufId = outArgs.outputID[0]-1; // to adjust for +1 done in inputID
    runStatus->freeBufId = outArgs.freeBufID[0]-1;
    if(pObj->curFrameNum==0) {
      pObj->outStartY = ((Uint32)outArgs.displayBufs[0].bufDesc[0].buf - (Uint32)prm->outAddr)/prm->outOffsetH;
      pObj->outStartX = ((Uint32)outArgs.displayBufs[0].bufDesc[0].buf - (Uint32)prm->outAddr)%prm->outOffsetH;
    }
    runStatus->outStartX = pObj->outStartX;
    runStatus->outStartY = pObj->outStartY;
    runStatus->outOffsetH= prm->outOffsetH;
    runStatus->outOffsetV= prm->outOffsetV;
  }

  pObj->curFrameNum++;

  return OSA_SOK;
}

int ALG_vidDecDelete(void *hndl)
{
  ALG_VidDecObj *pObj;

  pObj = (ALG_VidDecObj *)hndl;

  if(pObj==NULL)
    return OSA_EFAIL;

  if(pObj->createPrm.codec == ALG_VID_CODEC_MJPEG) {

    ALG_jpgDecDelete(pObj->hJpgDecode);

  } else {

    if(pObj->hDecode==NULL)
      return OSA_EFAIL;

    VIDDEC2_delete(pObj->hDecode);
  }

  OSA_memFree(pObj);

  return OSA_SOK;
}


static DRV_DisplayWinHndl gDRV_displayHndl;
static DRV_DisplayBufInfo gDRV_displayInfo;
static int                gDRV_displayBufId;

static int ALG_vidDecTestDisplayCreate() {

  DRV_DisplayWinConfig  displayConfig;
  int status;

  displayConfig.winId       = DRV_DISPLAY_VID_WIN_0;
  displayConfig.numBuf      = 3;
  displayConfig.dataFormat  = DRV_DATA_FORMAT_YUV420;
  displayConfig.startX      = 0;
  displayConfig.startY      = 0;
  displayConfig.width       = 720;
  displayConfig.height      = 480;
  displayConfig.offsetH     = OSA_align(displayConfig.width, 32);
  displayConfig.offsetV     = displayConfig.height;
  displayConfig.zoomH       = 0;
  displayConfig.zoomV       = 0;
  displayConfig.bufFlag     = 0;

  gDRV_displayBufId=0;

  OSA_printf(" ALG: VidDec: Opening display.\n");

  status = DRV_displaySetMode(DRV_DISPLAY_MODE_NTSC);
  if(status!=OSA_SOK)
    goto error_exit;

  status = DRV_displayOpen(&gDRV_displayHndl, &displayConfig);
  if(status!=OSA_SOK)
    goto error_exit;

  DRV_displayGetBufInfo(&gDRV_displayHndl, &gDRV_displayInfo);

error_exit:
  if(status!=OSA_SOK)
    OSA_ERROR("Display Init!!!\n");

  return status;
}

static int ALG_vidDecTestDisplayCopy(Uint8 *outVirtAddr, Uint8 *outPhysAddr, ALG_VidDecRunStatus *vidDecStatus) {

  int status;
  DRV_FrameCopyPrm frameCopy;

  frameCopy.srcPhysAddr = outPhysAddr;
  frameCopy.srcVirtAddr = outVirtAddr;
  frameCopy.dstPhysAddr = gDRV_displayInfo.physAddr[gDRV_displayBufId];
  frameCopy.dstVirtAddr = gDRV_displayInfo.virtAddr[gDRV_displayBufId];
  frameCopy.srcOffsetH  = vidDecStatus->outOffsetH;
  frameCopy.srcOffsetV  = vidDecStatus->outOffsetV;
  frameCopy.dstOffsetH  = gDRV_displayInfo.offsetH;
  frameCopy.dstOffsetV  = gDRV_displayInfo.offsetV;
  frameCopy.copyWidth   = vidDecStatus->frameWidth;
  frameCopy.copyHeight  = vidDecStatus->frameHeight;
  frameCopy.dataFormat  = gDRV_displayInfo.dataFormat;
  frameCopy.srcStartX   = vidDecStatus->outStartX;
  frameCopy.srcStartY   = vidDecStatus->outStartY;
  frameCopy.dstStartX   = 0;
  frameCopy.dstStartY   = 0;

  status = DRV_frameCopy(NULL, &frameCopy);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_frameCopy(A)\n");
  }

  DRV_displaySwitchBuf( &gDRV_displayHndl, &gDRV_displayBufId, 0, NULL);

  return status;
}

static int ALG_vidDecTestDisplayDelete() {

  DRV_displayClose(&gDRV_displayHndl);

  return OSA_SOK;
}

int ALG_vidDecTestMain(int argc, char **argv)
{
  int status, bufId, count, whOffset, frameWidth, frameHeight;
  Uint8 *inVirtAddr=NULL, *inPhysAddr, *outVirtAddr[2], *outPhysAddr[2], *curInAddr;
  OSA_PrfHndl prfVidDec;
  Uint32 size=10*MB, fileSize, totalDecSize;
  void *algVidDecHndl;
  ALG_VidDecCreate createPrm;
  ALG_VidDecRunPrm runPrm;
  ALG_VidDecRunStatus runStatus;
  char filename[100];
  int runCount=0;

  if(argc>0) {
    status = DRV_init();
    if(status!=OSA_SOK) {
      OSA_ERROR("DRV_init()\n");
      return status;
    }

    status = ALG_sysInit();
    if(status!=OSA_SOK)  {
      DRV_exit();
      return status;
    }
  }

  strcpy(filename, "test.264");
  frameWidth = 384;
  frameHeight = 240;

  if(argc > 2) {
    strcpy(filename, argv[2]);

    if(argc>3)
      frameWidth = atoi(argv[3]);

    if(argc>4)
      frameHeight = atoi(argv[4]);
  }

  if(frameWidth==0||frameHeight==0) {
    OSA_ERROR("Illegal parameters!!!\n");
  }

  inVirtAddr     = OSA_cmemAlloc(size, 32);
  outVirtAddr[0] = OSA_cmemAlloc(2*MB, 32);
  outVirtAddr[1] = OSA_cmemAlloc(2*MB, 32);

  inPhysAddr     = OSA_cmemGetPhysAddr(inVirtAddr);
  outPhysAddr[0] = OSA_cmemGetPhysAddr(outVirtAddr[0]);
  outPhysAddr[1] = OSA_cmemGetPhysAddr(outVirtAddr[1]);

  memset(outVirtAddr[0], 0xFF, 2*MB);
  memset(outVirtAddr[1], 0xFF, 2*MB);

  if(   inVirtAddr==NULL || outVirtAddr[0] == NULL || outVirtAddr[1] == NULL
     || inPhysAddr==NULL || outPhysAddr[0] == NULL || outPhysAddr[1] == NULL
    ) {
    OSA_ERROR("OSA_cmemAlloc()\n");
    goto error_exit;
  }

  status = OSA_fileReadFile(filename, inVirtAddr, size, &fileSize);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_fileReadFile()\n");
    goto error_exit;
  }

//test_again:
  OSA_prfReset(&prfVidDec);

  createPrm.codec           = ALG_VID_CODEC_H264;
  createPrm.dataFormat      = DRV_DATA_FORMAT_YUV420;
  createPrm.maxWidth        = frameWidth;
  createPrm.maxHeight       = frameHeight;

  algVidDecHndl = ALG_vidDecCreate(&createPrm);
  if(algVidDecHndl==NULL) {
    OSA_ERROR("ALG_vidDecCreate()\n");
    goto error_exit;
  }

  curInAddr = inVirtAddr;
  totalDecSize = 0;
  bufId=0;
  count=0;

  ALG_vidDecTestDisplayCreate();

  whOffset = 0;
  if(createPrm.codec==ALG_VID_CODEC_H264)
    whOffset = 48;

  OSA_printf(" ALG: VidDec: Decode in progress!!!\n");

  while(totalDecSize < fileSize) {

    runPrm.inAddr         = curInAddr;
    runPrm.outAddr        = outVirtAddr[bufId];
    runPrm.inDataSize     = 100*KB;
    runPrm.outOffsetH     = OSA_align(createPrm.maxWidth+whOffset, 32);
    runPrm.outOffsetV     = createPrm.maxHeight+whOffset;
    runPrm.inputBufId     = bufId;

    OSA_prfBegin(&prfVidDec);
    status = ALG_vidDecRun(algVidDecHndl, &runPrm, &runStatus);
    OSA_prfEnd(&prfVidDec, 1);

    #ifdef ALG_VID_DEC_TEST_DEBUG
    OSA_printf(" ALG: VidDec: %ld:%ld:%ld:%dx%d:%ld: %ld bytes (%ld, %ld)\n",
          runStatus.freeBufId,
          runPrm.inputBufId,
          runStatus.outputBufId,
          runStatus.frameWidth, runStatus.frameHeight,
          runStatus.isKeyFrame,
          runStatus.bytesUsed,
          runStatus.outStartX,
          runStatus.outStartY
          );
    #endif

    if(status==OSA_SOK) {
      //runStatus.bytesUsed += 3;
      curInAddr += runStatus.bytesUsed;
      totalDecSize += runStatus.bytesUsed;
    } else {
      OSA_printf(" ALG: VidDec: ALG_vidDecRun() ERROR !!!\n");
      break;
    }

    ALG_vidDecTestDisplayCopy(
            outVirtAddr[runStatus.outputBufId],
            outPhysAddr[runStatus.outputBufId],
            &runStatus
            );

    bufId ^= 1;

    count++;
    if(count>10000)
      break;
  }

  OSA_printf(" ALG: VidDec: Decode DONE!!!\n");

  ALG_vidDecTestDisplayDelete();

  ALG_vidDecDelete(algVidDecHndl);

  OSA_prfPrint(&prfVidDec, "VID DEC", 0);

  runCount++;
  //if(runCount<10)
  //  goto test_again;

error_exit:
  if(inVirtAddr)
    OSA_cmemFree(inVirtAddr);

  if(outVirtAddr[0])
    OSA_cmemFree(outVirtAddr[0]);

  if(outVirtAddr[1])
    OSA_cmemFree(outVirtAddr[1]);

  if(argc>0) {
    ALG_sysExit();
    DRV_exit();
  }

  return status;
}
