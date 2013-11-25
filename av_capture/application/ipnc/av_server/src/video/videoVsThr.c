
#include <avserver.h>

int VIDEO_vsGetResult(int tskId, int streamId, OSA_BufInfo *pInBufInfo)
{
  int i;
  Uint16 startX, startY, padH=0, padV=0;
  Bool done=FALSE;
  VIDEO_CaptureConfig *pCaptureConfig;
  VIDEO_CaptureStream *pCaptureStream;
  VIDEO_BufHeader *pBufHeader = (VIDEO_BufHeader *)pInBufInfo->virtAddr;

  if(!gAVSERVER_config.vstabTskEnable)
    return OSA_SOK;

  if(streamId < 0 || streamId >= gAVSERVER_config.numCaptureStream)
    return OSA_EFAIL;

  pCaptureConfig = &gAVSERVER_config.captureConfig[streamId];
  pCaptureStream = &gVIDEO_ctrl.captureStream[streamId];

  if(tskId != pCaptureStream->vsApplyTsk)
    return OSA_SOK;

  if( tskId <= VIDEO_TSK_VNF ) {
    if(gAVSERVER_config.sensorMode & DRV_IMGS_SENSOR_MODE_PIXEL_PAD) {
      DRV_imgWidthHeightPad(DRV_IMGS_SENSOR_MODE_PIXEL_PAD, &padH, &padV);
    }
  }

  pBufHeader->startX = (pBufHeader->width  - pCaptureConfig->width  + padH)/2;
  pBufHeader->startY = (pBufHeader->height - pCaptureConfig->height + padV)/2;

  if(!pCaptureConfig->vsEnable || pBufHeader->frameNum<=2) {

    pBufHeader->width  = pCaptureConfig->width  + padH;
    pBufHeader->height = pCaptureConfig->height + padV;

    return OSA_SOK;
  }


  while(!done) {

    if(OSA_flgIsSet(&gVIDEO_ctrl.vsStream.vsFlag, VIDEO_VS_FLAG_EXIT)) {

      pBufHeader->startX = (pBufHeader->width  - pCaptureConfig->width  + padH)/2;
      pBufHeader->startY = (pBufHeader->height - pCaptureConfig->height + padV)/2;

      pBufHeader->width  = pCaptureConfig->width  + padH;
      pBufHeader->height = pCaptureConfig->height + padV;

      return OSA_SOK;
    }

    OSA_flgClear(&gVIDEO_ctrl.vsStream.vsFlag, tskId);

    OSA_mutexLock(&gVIDEO_ctrl.vsStream.vsLock);

    i = pBufHeader->frameNum%VIDEO_VS_NUM_STATUS;

    if(pBufHeader->frameNum == gVIDEO_ctrl.vsStream.vsStatus[i].frameNum)
    {
      startX = gVIDEO_ctrl.vsStream.vsStatus[i].startX;
      startY = gVIDEO_ctrl.vsStream.vsStatus[i].startY;
      done = TRUE;
    }

    OSA_mutexUnlock(&gVIDEO_ctrl.vsStream.vsLock);

    if(!done) {

      #ifdef AVSERVER_DEBUG_VIDEO_VS_THR
         #ifdef AVSERVER_DEBUG_RUNNING
         OSA_printf(" VS: Waiting ... Stream %d Frame %d\n",
            streamId, pBufHeader->frameNum
            );
         #endif
      #endif

      OSA_flgWait(&gVIDEO_ctrl.vsStream.vsFlag, tskId | VIDEO_VS_FLAG_EXIT, OSA_FLG_MODE_OR, OSA_TIMEOUT_FOREVER);
    }
  }

  startX = ( startX * pBufHeader->width  ) / gVIDEO_ctrl.vsStream.vsCreatePrm.totalFrameWidth;
  startY = ( startY * pBufHeader->height ) / gVIDEO_ctrl.vsStream.vsCreatePrm.totalFrameHeight;

  startX = OSA_floor(startX, 2);

  pBufHeader->startX = startX;
  pBufHeader->startY = startY;

  pBufHeader->width  = pCaptureConfig->width  + padH;
  pBufHeader->height = pCaptureConfig->height + padV;

  #ifdef AVSERVER_DEBUG_VIDEO_VS_THR
     #ifdef AVSERVER_DEBUG_RUNNING
     OSA_printf(" VS: Stream %d Frame %d (%d, %d) (%dx%d)\n",
        streamId, pBufHeader->frameNum,
        pBufHeader->startX, pBufHeader->startY,
        pBufHeader->width, pBufHeader->height
        );
     #endif
  #endif

  return OSA_SOK;
}

int VIDEO_vsSetPrm(ALG_VstabStatus *vstabStatus, int frameNum)
{
  int i;
  static int prevStartX=0, prevStartY=0;

  OSA_mutexLock(&gVIDEO_ctrl.vsStream.vsLock);

  i = (frameNum)%VIDEO_VS_NUM_STATUS;

  gVIDEO_ctrl.vsStream.curFrame             = frameNum;
  gVIDEO_ctrl.vsStream.vsStatus[i].frameNum = frameNum;
  gVIDEO_ctrl.vsStream.vsStatus[i].startX   = vstabStatus->startX;
  gVIDEO_ctrl.vsStream.vsStatus[i].startY   = vstabStatus->startY;

  if(vstabStatus->startX!=prevStartX || vstabStatus->startY != prevStartY)
  {
     #ifdef AVSERVER_DEBUG_RUNNING
     OSA_printf(" VS: Frame %d (%d, %d) \n", frameNum, vstabStatus->startX, vstabStatus->startY);
     #endif
  }

  prevStartX = vstabStatus->startX;
  prevStartY = vstabStatus->startY;

  OSA_mutexUnlock(&gVIDEO_ctrl.vsStream.vsLock);

  OSA_flgSet(&gVIDEO_ctrl.vsStream.vsFlag, VIDEO_VS_FLAG_NEW_DATA);

  return OSA_SOK;
}

int VIDEO_vsTskCreate()
{
  DRV_IpipeBscConfig bscConfig;

  DRV_ImgsModeConfig *pImgsConfig = DRV_imgsGetModeConfig(gAVSERVER_config.sensorMode);

  if(pImgsConfig==NULL) {
    OSA_ERROR("DRV_imgsGetModeConfig(0x%04x)\n", gAVSERVER_config.sensorMode);
    return OSA_EFAIL;
  }

  gVIDEO_ctrl.vsStream.vsCreatePrm.totalFrameWidth  = pImgsConfig->validWidth;
  gVIDEO_ctrl.vsStream.vsCreatePrm.totalFrameHeight = pImgsConfig->validHeight;

  gVIDEO_ctrl.vsStream.vsCreatePrm.stabFrameWidth   = (gVIDEO_ctrl.vsStream.vsCreatePrm.totalFrameWidth*1000)/1200;
  gVIDEO_ctrl.vsStream.vsCreatePrm.stabFrameHeight  = (gVIDEO_ctrl.vsStream.vsCreatePrm.totalFrameHeight*1000)/1200;

  gVIDEO_ctrl.vsStream.vsCreatePrm.pBscConfig       = &gVIDEO_ctrl.captureInfo.ipipeInfo.bscInfo;

  gVIDEO_ctrl.vsStream.algVsHndl = ALG_vstabCreate(&gVIDEO_ctrl.vsStream.vsCreatePrm);

  // Configure BSC with the parameters returned by the VS alg create
  bscConfig.bsc_row_vct   = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_row_vct;
  bscConfig.bsc_row_vpos  = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_row_vpos;
  bscConfig.bsc_row_vnum  = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_row_vnum;
  bscConfig.bsc_row_vskip = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_row_vskip;
  bscConfig.bsc_row_hpos  = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_row_hpos;
  bscConfig.bsc_row_hnum  = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_row_hnum;
  bscConfig.bsc_row_hskip = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_row_hskip;
  bscConfig.bsc_row_shf   = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_row_shf;

  bscConfig.bsc_col_vct   = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_col_vct;
  bscConfig.bsc_col_vpos  = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_col_vpos;
  bscConfig.bsc_col_vnum  = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_col_vnum;
  bscConfig.bsc_col_vskip = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_col_vskip;
  bscConfig.bsc_col_hpos  = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_col_hpos;
  bscConfig.bsc_col_hnum  = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_col_hnum;
  bscConfig.bsc_col_hskip = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_col_hskip;
  bscConfig.bsc_col_shf   = gVIDEO_ctrl.vsStream.vsCreatePrm.bsc_col_shf;

  if(DRV_ipipeBscUpdateParams(&bscConfig) != OSA_SOK)
  {
      OSA_ERROR("DRV_ipipeBscUpdateParams()\n");
      return OSA_EFAIL;
  }

  if(gVIDEO_ctrl.vsStream.algVsHndl==NULL) {
    OSA_ERROR("ALG_vstabCreate(%dx%d:%dx%d)\n",
      gVIDEO_ctrl.vsStream.vsCreatePrm.totalFrameWidth,
      gVIDEO_ctrl.vsStream.vsCreatePrm.totalFrameHeight,
      gVIDEO_ctrl.vsStream.vsCreatePrm.stabFrameWidth,
      gVIDEO_ctrl.vsStream.vsCreatePrm.stabFrameHeight
    );
    return OSA_EFAIL;
  }

  OSA_flgClear(&gVIDEO_ctrl.vsStream.vsFlag, VIDEO_VS_FLAG_EXIT|VIDEO_VS_FLAG_NEW_DATA);

  return OSA_SOK;
}

int VIDEO_vsTskDelete()
{
  int status;

  status = ALG_vstabDelete(gVIDEO_ctrl.vsStream.algVsHndl);

  OSA_flgSet(&gVIDEO_ctrl.vsStream.vsFlag, VIDEO_VS_FLAG_EXIT);

  return status;
}

int VIDEO_vsTskRun()
{
  int status, vsBufId;
  OSA_BufInfo *pBufInfo;
  ALG_VstabRunPrm runPrm;
  ALG_VstabStatus vstabStatus;

  status = DRV_ipipeGetBscBuf(&vsBufId, OSA_TIMEOUT_FOREVER);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_ipipeGetBscBuf()\n");
    return status;
  }

  pBufInfo = DRV_ipipeGetBscBufInfo(vsBufId);

  OSA_assert(pBufInfo!=NULL);

  if(pBufInfo!=NULL) {

    OSA_assert(pBufInfo->virtAddr!=NULL);

    #ifdef AVSERVER_DEBUG_VIDEO_VS_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" VS: IPIPE:BSC Buf %d\n", vsBufId);
      #endif
    #endif

    runPrm.bscDataVirtAddr = pBufInfo->virtAddr;
    runPrm.bscDataPhysAddr = pBufInfo->physAddr;

    OSA_prfBegin(&gAVSERVER_ctrl.vsPrf);

    status = ALG_vstabRun(gVIDEO_ctrl.vsStream.algVsHndl, &runPrm, &vstabStatus);

    OSA_prfEnd(&gAVSERVER_ctrl.vsPrf, 1);

    #ifdef AVSERVER_DEBUG_VIDEO_VS_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" VS: IPIPE:BSC Buf %d DONE\n", vsBufId);
      #endif
    #endif


    if(status==OSA_SOK) {
      VIDEO_vsSetPrm(&vstabStatus, pBufInfo->count);
    }

  }

  DRV_ipipePutBscBuf(vsBufId);

  return status;
}

int VIDEO_vsTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  #ifdef AVSERVER_DEBUG_VIDEO_VS_THR
  OSA_printf(" VS: Recevied CMD = 0x%04x\n", cmd);
  #endif

  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_VS_THR
  OSA_printf(" VS: Create...\n");
  #endif

  status = VIDEO_vsTskCreate();

  OSA_tskAckOrFreeMsg(pMsg, status);

  if(status!=OSA_SOK) {
    OSA_ERROR("VIDEO_vsTskCreate()\n");
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_VS_THR
  OSA_printf(" VS: Create...DONE\n");
  #endif

  status = OSA_tskWaitMsg(pTsk, &pMsg);
  if(status!=OSA_SOK)
    return OSA_SOK;

  cmd = OSA_msgGetCmd(pMsg);

  if(cmd==AVSERVER_CMD_DELETE) {

    done = TRUE;
    ackMsg = TRUE;

  } else {

    #ifdef AVSERVER_DEBUG_VIDEO_VS_THR
    OSA_printf(" VS: Start...\n");
    #endif

    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
  }

  #ifdef AVSERVER_DEBUG_VIDEO_VS_THR
  OSA_printf(" VS: Start...DONE\n");
  #endif

  while(!done) {

    status = VIDEO_vsTskRun();

    if(status!=OSA_SOK) {
      OSA_ERROR("VIDEO_vsTskRun()\n");
      break;
    }

    status = OSA_tskCheckMsg(pTsk, &pMsg);

    if(status!=OSA_SOK)
      continue;

    #ifdef AVSERVER_DEBUG_VIDEO_VS_THR
    OSA_printf(" VS: Recevied CMD = 0x%04x\n", cmd);
    #endif

    cmd = OSA_msgGetCmd(pMsg);

    switch(cmd) {
      case AVSERVER_CMD_DELETE:
        done = TRUE;
        ackMsg = TRUE;
        break;

      default:

        #ifdef AVSERVER_DEBUG_VIDEO_VS_THR
        OSA_printf(" VS: Unknown CMD = 0x%04x\n", cmd);
        #endif

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
    }
  }

  #ifdef AVSERVER_DEBUG_VIDEO_VS_THR
  OSA_printf(" VS: Delete...\n");
  #endif

  VIDEO_vsTskDelete();

  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  #ifdef AVSERVER_DEBUG_VIDEO_VS_THR
  OSA_printf(" VS: Delete...DONE\n");
  #endif

  return OSA_SOK;
}

int VIDEO_vsCreate()
{
  int status;

  status = OSA_mutexCreate(&gVIDEO_ctrl.vsStream.vsLock);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_mutexCreate()\n");
    return status;
  }

  status = OSA_flgCreate(&gVIDEO_ctrl.vsStream.vsFlag, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_mutexCreate()\n");
    return status;
  }

  status = OSA_tskCreate( &gVIDEO_ctrl.vsTsk, VIDEO_vsTskMain, VIDEO_VS_THR_PRI, VIDEO_VS_STACK_SIZE, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }

  return status;
}

int VIDEO_vsDelete()
{
  int status;

  status = OSA_tskDelete( &gVIDEO_ctrl.vsTsk );
  status |= OSA_mutexDelete(&gVIDEO_ctrl.vsStream.vsLock);
  status |= OSA_flgDelete(&gVIDEO_ctrl.vsStream.vsFlag);

  return status;
}
