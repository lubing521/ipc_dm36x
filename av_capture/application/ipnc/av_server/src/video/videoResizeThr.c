#include <avserver.h>

int VIDEO_resizeTskRun(int streamId)
{
  int status=OSA_SOK, numStreams, rszStreamId, inBufId, rszBufId[AVSERVER_MAX_STREAMS];

  OSA_BufInfo *pInBufInfo, *pOutBufInfo[AVSERVER_MAX_STREAMS];

  VIDEO_BufHeader *pInBufHeader, *pOutBufHeader;

  DRV_ReszRunPrm reszPrm;
  DRV_ReszOutPrm reszOutPrm[2];

  Uint32 frameSkip;


  if(streamId > 1 || !gAVSERVER_config.captureSingleResize) {
    OSA_ERROR(" RESIZE: Illegal parameters\n");
    return OSA_EFAIL;
  }

  pInBufInfo = AVSERVER_bufGetFull( VIDEO_TSK_RESIZE, streamId, &inBufId, OSA_TIMEOUT_FOREVER);

  OSA_assert(pInBufInfo!=NULL);

  if(pInBufInfo!=NULL) {

    // send for compression
    VIDEO_vsGetResult(VIDEO_TSK_RESIZE, streamId, pInBufInfo);

    AVSERVER_bufPutFull( VIDEO_TSK_RESIZE, streamId, inBufId);

    pInBufHeader = (VIDEO_BufHeader*)pInBufInfo->virtAddr;

    numStreams = gAVSERVER_config.numCaptureStream-1;

    for(rszStreamId=1; rszStreamId<gAVSERVER_config.numCaptureStream; rszStreamId++) {

      frameSkip = pInBufHeader->frameNum%30;

      if(! (gAVSERVER_config.captureConfig[rszStreamId].frameSkipMask & (1<<frameSkip)) )
      {
        pOutBufInfo[rszStreamId] = NULL;
        continue;
      }

      pOutBufInfo[rszStreamId] = AVSERVER_bufGetEmpty( VIDEO_TSK_RESIZE, rszStreamId, &rszBufId[rszStreamId], OSA_TIMEOUT_FOREVER);

      OSA_assert(pOutBufInfo[rszStreamId]!=NULL);

      pOutBufHeader = (VIDEO_BufHeader*)pOutBufInfo[rszStreamId]->virtAddr;

      OSA_assert(pOutBufHeader!=NULL);

      memset(pOutBufHeader, 0, sizeof(*pOutBufHeader));

      pOutBufHeader->timestamp = pInBufHeader->timestamp;
      pOutBufHeader->frameNum  = pInBufHeader->frameNum;
      pOutBufHeader->startX    = 0;
      pOutBufHeader->startY    = 0;
      pOutBufHeader->width     = gVIDEO_ctrl.captureStream[rszStreamId].captureOutWidth;
      pOutBufHeader->height    = gVIDEO_ctrl.captureStream[rszStreamId].captureOutHeight;
      pOutBufHeader->offsetH   = gVIDEO_ctrl.captureStream[rszStreamId].captureOutOffsetH;
      pOutBufHeader->offsetV   = gVIDEO_ctrl.captureStream[rszStreamId].captureOutOffsetV;
    }

    #ifdef AVSERVER_DEBUG_VIDEO_RESIZE_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" RESIZE: Stream %d InBuf %d OutBuf %d\n", streamId, inBufId, rszBufId[1]);
      #endif
    #endif

    reszPrm.inType     = gAVSERVER_config.captureYuvFormat;
    reszPrm.inVirtAddr = pInBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;
    reszPrm.inPhysAddr = pInBufInfo->physAddr + VIDEO_BUF_HEADER_SIZE;
    reszPrm.inStartX   = pInBufHeader->startX;
    reszPrm.inStartY   = pInBufHeader->startY+1;
    reszPrm.inWidth    = pInBufHeader->width;
    reszPrm.inHeight   = pInBufHeader->height;
    reszPrm.inOffsetH  = pInBufHeader->offsetH;
    reszPrm.inOffsetV  = pInBufHeader->offsetV;
    reszPrm.enableInvAlaw = FALSE;
    reszPrm.enableInvDpcm = FALSE;
    reszPrm.clkDivM = 0;
    reszPrm.clkDivN = 0;

    rszStreamId = 1;
    while(rszStreamId < gAVSERVER_config.numCaptureStream) {
      if(pOutBufInfo[rszStreamId]!=NULL)
        break;
      rszStreamId++;
    }

    if(rszStreamId < gAVSERVER_config.numCaptureStream) {

      OSA_prfBegin(&gAVSERVER_ctrl.resizePrf);

      reszPrm.pOut[0] = NULL;
      reszPrm.pOut[1] = NULL;

      reszPrm.pOut[0] = &reszOutPrm[0];

      reszOutPrm[0].outType = gAVSERVER_config.captureYuvFormat;
      reszOutPrm[0].flipH = FALSE;
      reszOutPrm[0].flipV = FALSE;
      reszOutPrm[0].outVirtAddr = pOutBufInfo[rszStreamId]->virtAddr + VIDEO_BUF_HEADER_SIZE;
      reszOutPrm[0].outPhysAddr = pOutBufInfo[rszStreamId]->physAddr + VIDEO_BUF_HEADER_SIZE;
      reszOutPrm[0].outWidth    = gVIDEO_ctrl.captureStream[rszStreamId].captureOutWidth;
      reszOutPrm[0].outHeight   = gVIDEO_ctrl.captureStream[rszStreamId].captureOutHeight;
      reszOutPrm[0].outOffsetH  = gVIDEO_ctrl.captureStream[rszStreamId].captureOutOffsetH;
      reszOutPrm[0].outOffsetV  = gVIDEO_ctrl.captureStream[rszStreamId].captureOutOffsetV;

      rszStreamId++;
      while(rszStreamId < gAVSERVER_config.numCaptureStream) {
        if(pOutBufInfo[rszStreamId]!=NULL)
          break;
        rszStreamId++;
      }

      if(rszStreamId < gAVSERVER_config.numCaptureStream) {

        reszPrm.pOut[1] = &reszOutPrm[1];

        reszOutPrm[1].outType = reszOutPrm[0].outType;
        reszOutPrm[1].flipH = reszOutPrm[0].flipH;
        reszOutPrm[1].flipV = reszOutPrm[0].flipV;
        reszOutPrm[1].outVirtAddr = pOutBufInfo[2]->virtAddr + VIDEO_BUF_HEADER_SIZE;
        reszOutPrm[1].outPhysAddr = pOutBufInfo[2]->physAddr + VIDEO_BUF_HEADER_SIZE;
        reszOutPrm[1].outWidth    = gVIDEO_ctrl.captureStream[rszStreamId].captureOutWidth;
        reszOutPrm[1].outHeight   = gVIDEO_ctrl.captureStream[rszStreamId].captureOutHeight;
        reszOutPrm[1].outOffsetH  = gVIDEO_ctrl.captureStream[rszStreamId].captureOutOffsetH;
        reszOutPrm[1].outOffsetV  = gVIDEO_ctrl.captureStream[rszStreamId].captureOutOffsetV;
      }

      OSA_prfBegin(&gAVSERVER_ctrl.resizePrf);
      status = DRV_reszRun(&reszPrm);
      OSA_prfEnd(&gAVSERVER_ctrl.resizePrf, 1);

      if(status!=OSA_SOK)
        OSA_ERROR("DRV_reszRun()\n");

      rszStreamId++;
      while(rszStreamId < gAVSERVER_config.numCaptureStream) {
        if(pOutBufInfo[rszStreamId]!=NULL)
          break;
        rszStreamId++;
      }

      if(rszStreamId < gAVSERVER_config.numCaptureStream) {

        reszPrm.inType = reszOutPrm[1].outType;
        reszPrm.inVirtAddr = reszOutPrm[1].outVirtAddr;
        reszPrm.inPhysAddr = reszOutPrm[1].outPhysAddr;
        reszPrm.inStartX = 0;
        reszPrm.inStartY = 1;
        reszPrm.inWidth  = reszOutPrm[1].outWidth;
        reszPrm.inHeight = reszOutPrm[1].outHeight;
        reszPrm.inOffsetH= reszOutPrm[1].outOffsetH;
        reszPrm.inOffsetV= reszOutPrm[1].outOffsetV;
        reszPrm.enableInvAlaw = FALSE;
        reszPrm.enableInvDpcm = FALSE;
        reszPrm.clkDivM = 0;
        reszPrm.clkDivN = 0;

        reszPrm.pOut[0] = NULL;
        reszPrm.pOut[1] = NULL;

        reszPrm.pOut[0] = &reszOutPrm[0];

        reszOutPrm[0].outType = reszPrm.inType;
        reszOutPrm[0].flipH = FALSE;
        reszOutPrm[0].flipV = FALSE;
        reszOutPrm[0].outVirtAddr = pOutBufInfo[3]->virtAddr + VIDEO_BUF_HEADER_SIZE;
        reszOutPrm[0].outPhysAddr = pOutBufInfo[3]->physAddr + VIDEO_BUF_HEADER_SIZE;
        reszOutPrm[0].outWidth    = gVIDEO_ctrl.captureStream[rszStreamId].captureOutWidth;
        reszOutPrm[0].outHeight   = gVIDEO_ctrl.captureStream[rszStreamId].captureOutHeight;
        reszOutPrm[0].outOffsetH  = gVIDEO_ctrl.captureStream[rszStreamId].captureOutOffsetH;
        reszOutPrm[0].outOffsetV  = gVIDEO_ctrl.captureStream[rszStreamId].captureOutOffsetV;

        OSA_prfBegin(&gAVSERVER_ctrl.resizePrf);
        status = DRV_reszRun(&reszPrm);
        OSA_prfEnd(&gAVSERVER_ctrl.resizePrf, 1);

        if(status!=OSA_SOK)
          OSA_ERROR("DRV_reszRun()\n");
      }

      OSA_prfEnd(&gAVSERVER_ctrl.resizePrf, 1);
    }


    for(rszStreamId=1; rszStreamId<gAVSERVER_config.numCaptureStream; rszStreamId++) {
      if(pOutBufInfo[rszStreamId]!=NULL) {
        AVSERVER_bufPutFull( VIDEO_TSK_RESIZE, rszStreamId, rszBufId[rszStreamId]);

        VIDEO_fdCopyRun(VIDEO_TSK_RESIZE, rszStreamId, pOutBufInfo[rszStreamId]);
        VIDEO_swosdRun(VIDEO_TSK_RESIZE, rszStreamId, pOutBufInfo[rszStreamId]);
        VIDEO_displayCopyRun(VIDEO_TSK_RESIZE, rszStreamId, pOutBufInfo[rszStreamId]);
      }
    }

    pInBufInfo = AVSERVER_bufGetEmpty( VIDEO_TSK_RESIZE, streamId, &inBufId, OSA_TIMEOUT_FOREVER);
    if(pInBufInfo!=NULL) {
      AVSERVER_bufPutEmpty( VIDEO_TSK_RESIZE, streamId, inBufId);

      #ifdef AVSERVER_DEBUG_VIDEO_RESIZE_THR
        #ifdef AVSERVER_DEBUG_RUNNING
        OSA_printf(" RESIZE: Put Stream %d Buf %d\n", streamId, inBufId);
        #endif
      #endif
    }
  }

  return status;
}

int VIDEO_resizeTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status, streamId;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  #ifdef AVSERVER_DEBUG_VIDEO_RESIZE_THR
  OSA_printf(" RESIZE: Recevied CMD = 0x%04x\n", cmd);
  #endif

  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

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
        streamId = (int)OSA_msgGetPrm(pMsg);

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

        VIDEO_resizeTskRun(streamId);

        break;
      default:

        #ifdef AVSERVER_DEBUG_VIDEO_RESIZE_THR
        OSA_printf(" RESIZE: Unknown CMD = 0x%04x\n", cmd);
        #endif

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
    }
  }

  #ifdef AVSERVER_DEBUG_VIDEO_RESIZE_THR
  OSA_printf(" RESIZE: Delete...\n");
  #endif

  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  #ifdef AVSERVER_DEBUG_VIDEO_RESIZE_THR
  OSA_printf(" RESIZE: Delete...DONE\n");
  #endif

  return OSA_SOK;
}

int VIDEO_resizeCreate()
{
  int status;

  status = OSA_tskCreate( &gVIDEO_ctrl.resizeTsk, VIDEO_resizeTskMain, VIDEO_RESIZE_THR_PRI, VIDEO_RESIZE_STACK_SIZE, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }

  return status;
}

int VIDEO_resizeDelete()
{
  int status;

  status = OSA_tskDelete( &gVIDEO_ctrl.resizeTsk );

  return status;
}
