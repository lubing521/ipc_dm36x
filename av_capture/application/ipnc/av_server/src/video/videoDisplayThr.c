
#include <avserver.h>
#include <drv_frameCopy.h>

#define DISBUF_MEMCPY

int VIDEO_displayCopyRun(int tskId, int streamId, OSA_BufInfo *pBufInfo)
{
  VIDEO_CaptureStream *pCaptureStream;
  OSA_BufInfo *pDisplayBufInfo;
  VIDEO_BufHeader *pBufHeader;
  DRV_FrameCopyPrm frameCopy;

  int status, displayBufId;

  if(streamId<0 || streamId >= AVSERVER_MAX_STREAMS) {
    OSA_ERROR("Invalid streamId %d\n", streamId);
    return OSA_EFAIL;
  }

  if(streamId != gAVSERVER_config.displayConfig.captureStreamId)
    return OSA_SOK;

  pCaptureStream = &gVIDEO_ctrl.captureStream[streamId];

  if(pCaptureStream->displayCopyTsk != tskId)
    return OSA_SOK;

#ifdef DISBUF_MEMCPY
  status = OSA_bufGetEmpty(&gVIDEO_ctrl.displayStream.bufDisplayIn, &displayBufId, OSA_TIMEOUT_NONE);

  if(status==OSA_SOK) {

    pDisplayBufInfo = OSA_bufGetBufInfo(&gVIDEO_ctrl.displayStream.bufDisplayIn, displayBufId);

    OSA_assert(pDisplayBufInfo!=NULL);

    if(pDisplayBufInfo!=NULL) {

      pBufHeader = (VIDEO_BufHeader *)pBufInfo->virtAddr;

      OSA_assert(pBufHeader!=NULL);

      #ifdef AVSERVER_DEBUG_VIDEO_DISPLAY_THR
        #ifdef AVSERVER_DEBUG_RUNNING
        OSA_printf(" DISPLAY: Copying to Buf %d (%d, %d)\n", displayBufId, pBufHeader->startX, pBufHeader->startY);
        #endif
      #endif

      frameCopy.srcPhysAddr = pBufInfo->physAddr + VIDEO_BUF_HEADER_SIZE;
      frameCopy.srcVirtAddr = pBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;
      frameCopy.dstPhysAddr = pDisplayBufInfo->physAddr;
      frameCopy.dstVirtAddr = pDisplayBufInfo->virtAddr;
      frameCopy.srcOffsetH  = pBufHeader->offsetH;
      frameCopy.srcOffsetV  = pBufHeader->offsetV;
      frameCopy.dstOffsetH  = gVIDEO_ctrl.displayStream.displayInfo.offsetH;
      frameCopy.dstOffsetV  = gVIDEO_ctrl.displayStream.displayInfo.offsetV;
      frameCopy.dataFormat  = gAVSERVER_config.captureYuvFormat;

      if(pBufHeader->width > gAVSERVER_config.displayConfig.width) {

        frameCopy.copyWidth   = gAVSERVER_config.displayConfig.width;
        frameCopy.copyHeight  = gAVSERVER_config.displayConfig.height;
        frameCopy.srcStartX   = pBufHeader->startX + (pBufHeader->width-gAVSERVER_config.displayConfig.width)/2;
        frameCopy.srcStartY   = pBufHeader->startY + (pBufHeader->height-gAVSERVER_config.displayConfig.height)/2;
        frameCopy.dstStartX   = 0;
        frameCopy.dstStartY   = 0;
      } else {

        frameCopy.copyWidth   = pBufHeader->width;
        frameCopy.copyHeight  = pBufHeader->height;
        frameCopy.srcStartX   = pBufHeader->startX;
        frameCopy.srcStartY   = pBufHeader->startY;
        frameCopy.dstStartX   = (gAVSERVER_config.displayConfig.width-pBufHeader->width)/2;
        frameCopy.dstStartY   = (gAVSERVER_config.displayConfig.height-pBufHeader->height)/2;
      }
	  if(gAVSERVER_config.displayEnable) {
		  status = DRV_frameCopy(NULL, &frameCopy);
		  if(status!=OSA_SOK) {
			OSA_ERROR("DRV_frameCopy()\n");
		  }
  		}
    }

    OSA_bufPutFull(&gVIDEO_ctrl.displayStream.bufDisplayIn, displayBufId);
    OSA_tskSendMsg(&gVIDEO_ctrl.displayTsk, NULL, AVSERVER_CMD_NEW_DATA, NULL, 0);
  }

  return status;

#else

  OSA_tskSendMsg(&gVIDEO_ctrl.displayTsk, NULL, AVSERVER_CMD_NEW_DATA, (void*)pBufInfo, 0);
  return OSA_SOK;

#endif // #ifdef DISBUF_MEMCPY
}

int VIDEO_displayTskCreate()
{
  int status,disStreamId;
  DRV_DisplayWinConfig  displayConfig;

  displayConfig.winId       = DRV_DISPLAY_VID_WIN_0;
  displayConfig.numBuf      = gVIDEO_ctrl.displayStream.bufDisplayInCreatePrm.numBuf;
  displayConfig.dataFormat  = gAVSERVER_config.captureYuvFormat;

  displayConfig.width  = gAVSERVER_config.displayConfig.width;
  displayConfig.height = gAVSERVER_config.displayConfig.height;

#ifdef DISBUF_MEMCPY
  displayConfig.startX     = 0;
  displayConfig.startY     = 0;
  displayConfig.offsetH    = OSA_align(displayConfig.width, 32);
  displayConfig.offsetV    = displayConfig.height;
  displayConfig.cropWidth  = gAVSERVER_config.displayConfig.width;
  displayConfig.cropHeight = gAVSERVER_config.displayConfig.height;
  displayConfig.bufFlag    = 0;
#else
  disStreamId = gAVSERVER_config.displayConfig.captureStreamId;

  if(gAVSERVER_config.captureConfig[disStreamId].width < gAVSERVER_config.displayConfig.width)
  {
      displayConfig.startX    = (gAVSERVER_config.displayConfig.width - gAVSERVER_config.captureConfig[disStreamId].width)/2;
      displayConfig.cropWidth = gAVSERVER_config.captureConfig[disStreamId].width;
  }
  else
  {
      displayConfig.startX    = 0;
      displayConfig.cropWidth = gAVSERVER_config.displayConfig.width;
  }

  if(gAVSERVER_config.captureConfig[disStreamId].height < gAVSERVER_config.displayConfig.height)
  {
      displayConfig.startY     = (gAVSERVER_config.displayConfig.height - gAVSERVER_config.captureConfig[disStreamId].height)/2;
      displayConfig.cropHeight = gAVSERVER_config.captureConfig[disStreamId].height;
  }
  else
  {
      displayConfig.startY     = 0;
      displayConfig.cropHeight = gAVSERVER_config.displayConfig.height;
  }

  displayConfig.offsetH = OSA_align(gAVSERVER_config.captureConfig[disStreamId].width, 32);
  displayConfig.offsetV = gAVSERVER_config.captureConfig[disStreamId].height;
  displayConfig.bufFlag = 1;
#endif // #ifdef DISBUF_MEMCPY

  displayConfig.zoomH    = 0;
  displayConfig.zoomV    = 0;
  displayConfig.expandH  = gAVSERVER_config.displayConfig.expandH;
  displayConfig.expandV  = gAVSERVER_config.displayConfig.expandV;

  status = DRV_displaySetMode(DRV_DISPLAY_MODE_NTSC);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_displaySetMode()\n");
    return status;
  }

  status = DRV_displayOpen(&gVIDEO_ctrl.displayStream.displayHndl, &displayConfig);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_displayOpen()\n");
    return status;
  }

  DRV_displayGetBufInfo(&gVIDEO_ctrl.displayStream.displayHndl, &gVIDEO_ctrl.displayStream.displayInfo);

  return status;
}

int VIDEO_displayTskDelete()
{
  int status;

  status = DRV_displayClose(&gVIDEO_ctrl.displayStream.displayHndl);

  return status;
}

int VIDEO_displayTskRun(OSA_BufInfo *pBufInfo)
{
  int status, inBufId;
  static int disFrameNum = 0;

#ifdef DISBUF_MEMCPY
  status = OSA_bufGetFull(&gVIDEO_ctrl.displayStream.bufDisplayIn, &inBufId, OSA_TIMEOUT_FOREVER);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_bufGetFull()\n");
    return status;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_DISPLAY_THR
     #ifdef AVSERVER_DEBUG_RUNNING
     OSA_printf(" DISPLAY: Get Buf %d \n", inBufId);
     #endif
  #endif

  status = DRV_displaySwitchBuf(&gVIDEO_ctrl.displayStream.displayHndl, &inBufId, 0, NULL);
#else
  if(gAVSERVER_config.displayEnable)
  {
      inBufId = (disFrameNum++ % gVIDEO_ctrl.displayStream.bufDisplayInCreatePrm.numBuf);
      status = DRV_displaySwitchBuf(&gVIDEO_ctrl.displayStream.displayHndl, &inBufId, 1, pBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE);
  }
  else
  {
      inBufId = (disFrameNum++ % gVIDEO_ctrl.displayStream.bufDisplayInCreatePrm.numBuf);
      status = DRV_displaySwitchBuf(&gVIDEO_ctrl.displayStream.displayHndl, &inBufId, 2, NULL);
  }
#endif //#ifdef DISBUF_MEMCPY

  //if(!gAVSERVER_config.displayEnable)
  //	DRV_displayEnable(&gVIDEO_ctrl.displayStream.displayHndl, FALSE);

  if(status==OSA_SOK) {
#ifdef DISBUF_MEMCPY
    OSA_bufPutEmpty(&gVIDEO_ctrl.displayStream.bufDisplayIn, inBufId);

    #ifdef AVSERVER_DEBUG_VIDEO_DISPLAY_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" DISPLAY: Put Buf %d \n", inBufId);
      #endif
    #endif
#endif // #ifdef DISBUF_MEMCPY
  }

  return status;
}

int VIDEO_displayTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  #ifdef AVSERVER_DEBUG_VIDEO_DISPLAY_THR
  OSA_printf(" DISPLAY: Recevied CMD = 0x%04x\n", cmd);
  #endif

  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_DISPLAY_THR
  OSA_printf(" DISPLAY: Create...\n");
  #endif

  status = VIDEO_displayTskCreate();

  OSA_tskAckOrFreeMsg(pMsg, status);

  if(status!=OSA_SOK) {
    OSA_ERROR("VIDEO_displayTskCreate()\n");
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_DISPLAY_THR
  OSA_printf(" DISPLAY: Create...DONE\n");
  #endif

  while(!done) {

    status = OSA_tskWaitMsg(pTsk, &pMsg);

    if(status!=OSA_SOK)
      break;

    cmd = OSA_msgGetCmd(pMsg);

    switch(cmd) {
      case AVSERVER_CMD_DELETE:
        done = TRUE;
        ackMsg = TRUE;
        break;

      case AVSERVER_CMD_NEW_DATA:

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

#ifdef DISBUF_MEMCPY
        VIDEO_displayTskRun(NULL);
#else
        VIDEO_displayTskRun((OSA_BufInfo*)OSA_msgGetPrm(pMsg));
#endif

        break;
      default:

        #ifdef AVSERVER_DEBUG_VIDEO_DISPLAY_THR
        OSA_printf(" DISPLAY: Unknown CMD = 0x%04x\n", cmd);
        #endif

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
    }
  }

  #ifdef AVSERVER_DEBUG_VIDEO_DISPLAY_THR
  OSA_printf(" DISPLAY: Delete...\n");
  #endif

  VIDEO_displayTskDelete();

  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  #ifdef AVSERVER_DEBUG_VIDEO_DISPLAY_THR
  OSA_printf(" DISPLAY: Delete...DONE\n");
  #endif

  return OSA_SOK;
}

int VIDEO_displayCreate()
{
  int status;

  status = OSA_tskCreate( &gVIDEO_ctrl.displayTsk, VIDEO_displayTskMain, VIDEO_DISPLAY_THR_PRI, VIDEO_DISPLAY_STACK_SIZE, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }

  return status;
}

int VIDEO_displayDelete()
{
  int status;

  status = OSA_tskDelete( &gVIDEO_ctrl.displayTsk );

  return status;
}
