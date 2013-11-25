#include <avserver.h>

int VIDEO_ldcTskCreate()
{
  int status=OSA_SOK, streamId;

  DRV_LdcConfig ldcConfig;

  for(streamId=0; streamId<gAVSERVER_config.numCaptureStream; streamId++) {

    ldcConfig.sensorMode = gAVSERVER_config.sensorMode;
    ldcConfig.inWidth    = gVIDEO_ctrl.captureStream[streamId].captureOutWidth;
    ldcConfig.inHeight   = gVIDEO_ctrl.captureStream[streamId].captureOutHeight;
    ldcConfig.pLdcConfig = NULL;

    status = DRV_ldcOpen(streamId, &ldcConfig);

    if(status!=OSA_SOK)
      OSA_ERROR("DRV_ldcOpen(%04x:%dx%d)\n", ldcConfig.sensorMode, ldcConfig.inWidth, ldcConfig.inHeight);
  }

  return status;
}

int VIDEO_ldcTskDelete()
{
  int streamId;

  for(streamId=0; streamId<gAVSERVER_config.numCaptureStream; streamId++) {
    DRV_ldcClose(streamId);
  }

  return OSA_SOK;
}

int VIDEO_ldcTskRun(int streamId)
{
  int status=OSA_EFAIL, inBufId, outBufId;
  OSA_BufInfo *pInBufInfo, *pOutBufInfo;
  VIDEO_BufHeader *pInBufHeader, *pOutBufHeader;
  DRV_LdcRunPrm ldcPrm;
  DRV_FrameCopyPrm frameCopy;

  pInBufInfo = AVSERVER_bufGetFull( VIDEO_TSK_LDC, streamId, &inBufId, OSA_TIMEOUT_FOREVER);

  OSA_assert(pInBufInfo!=NULL);

  if(pInBufInfo!=NULL) {

    pOutBufInfo = AVSERVER_bufGetEmpty( VIDEO_TSK_LDC, streamId, &outBufId, OSA_TIMEOUT_FOREVER);

    OSA_assert(pOutBufInfo!=NULL);

    #ifdef AVSERVER_DEBUG_VIDEO_LDC_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" LDC: Stream %d InBuf %d OutBuf %d\n", streamId, inBufId, outBufId);
      #endif
    #endif

    if(pOutBufInfo!=NULL) {

      pInBufHeader  = (VIDEO_BufHeader*)pInBufInfo->virtAddr;
      pOutBufHeader = (VIDEO_BufHeader*)pOutBufInfo->virtAddr;

      OSA_assert(pInBufHeader!=NULL);
      OSA_assert(pOutBufHeader!=NULL);

      ldcPrm.inFormat     = gAVSERVER_config.captureYuvFormat;
      ldcPrm.inStartX     = 0;
      ldcPrm.inStartY     = 0;
      ldcPrm.inPhysAddr   = pInBufInfo->physAddr + VIDEO_BUF_HEADER_SIZE;
      ldcPrm.inOffsetH    = pInBufHeader->offsetH;
      ldcPrm.inOffsetV    = pInBufHeader->offsetV;
      ldcPrm.inWidth      = pInBufHeader->width;
      ldcPrm.inHeight     = pInBufHeader->height;
      ldcPrm.outPhysAddr  = pOutBufInfo->physAddr + VIDEO_BUF_HEADER_SIZE;
      ldcPrm.outOffsetH   = pInBufHeader->offsetH;
      ldcPrm.outOffsetV   = pInBufHeader->offsetV;
      ldcPrm.OBH          = 0;
      ldcPrm.OBV          = 0;
      ldcPrm.PIXEL_PAD    = 0;

      if(gAVSERVER_config.captureConfig[streamId].ldcEnable) {

          OSA_prfBegin(&gAVSERVER_ctrl.ldcPrf[streamId]);

          status = DRV_ldcRun(streamId, &ldcPrm);

          OSA_prfEnd(&gAVSERVER_ctrl.ldcPrf[streamId], 1);

          if(status!=OSA_SOK)
            OSA_ERROR("DRV_ldcRun(%dx%d)\n", ldcPrm.inWidth, ldcPrm.inHeight);
      }

      if(!gAVSERVER_config.captureConfig[streamId].ldcEnable) {
        frameCopy.srcPhysAddr = ldcPrm.inPhysAddr;
        frameCopy.srcVirtAddr = pInBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;
        frameCopy.dstPhysAddr = ldcPrm.outPhysAddr;
        frameCopy.dstVirtAddr = pOutBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;
        frameCopy.srcOffsetH  = ldcPrm.inOffsetH;
        frameCopy.srcOffsetV  = ldcPrm.inOffsetV;
        frameCopy.dstOffsetH  = ldcPrm.outOffsetH;
        frameCopy.dstOffsetV  = ldcPrm.outOffsetV;
        frameCopy.copyWidth   = ldcPrm.inWidth;
        frameCopy.copyHeight  = ldcPrm.inHeight;
        frameCopy.dataFormat  = ldcPrm.inFormat;
        frameCopy.srcStartX   = ldcPrm.inStartX;
        frameCopy.srcStartY   = ldcPrm.inStartY;
        frameCopy.dstStartX   = 0;
        frameCopy.dstStartY   = 0;

        status = DRV_frameCopy(NULL, &frameCopy);
        if(status!=OSA_SOK) {
          OSA_ERROR("DRV_frameCopy()\n");
        }

      }

      memcpy(pOutBufHeader, pInBufHeader, sizeof(*pOutBufHeader));

      pOutBufHeader->startX = 0;
      pOutBufHeader->startY = 0;
      pOutBufHeader->width  = ldcPrm.inWidth;
      pOutBufHeader->height = ldcPrm.inHeight;

      #ifdef AVSERVER_DEBUG_VIDEO_LDC_THR
        #ifdef AVSERVER_DEBUG_RUNNING
        OSA_printf(" LDC: DONE Stream %d InBuf %d OutBuf %d\n", streamId, inBufId, outBufId);
        #endif
      #endif

      VIDEO_vsGetResult(VIDEO_TSK_LDC, streamId, pOutBufInfo);

      VIDEO_fdCopyRun(VIDEO_TSK_LDC, streamId, pOutBufInfo);
      VIDEO_swosdRun(VIDEO_TSK_LDC, streamId, pOutBufInfo);

      AVSERVER_bufPutFull( VIDEO_TSK_LDC, streamId, outBufId);

      VIDEO_displayCopyRun(VIDEO_TSK_LDC, streamId, pOutBufInfo);
    }


    AVSERVER_bufPutEmpty( VIDEO_TSK_LDC, streamId, inBufId);
  }

  return status;
}

int VIDEO_ldcTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status, streamId;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  #ifdef AVSERVER_DEBUG_VIDEO_LDC_THR
  OSA_printf(" LDC: Recevied CMD = 0x%04x\n", cmd);
  #endif

  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  status = VIDEO_ldcTskCreate();

  OSA_tskAckOrFreeMsg(pMsg, status);

  if(status!=OSA_SOK)
    return OSA_SOK;

  while(!done) {

    status = OSA_tskWaitMsg(pTsk, &pMsg);

    if(status!=OSA_SOK) {
      break;
    }

    cmd = OSA_msgGetCmd(pMsg);

    switch(cmd) {
      case AVSERVER_CMD_DELETE:
        done = TRUE;
        ackMsg = TRUE;
        break;

      case AVSERVER_CMD_NEW_DATA:
        streamId = (int)OSA_msgGetPrm(pMsg);

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

        VIDEO_ldcTskRun(streamId);

        break;
      default:

        #ifdef AVSERVER_DEBUG_VIDEO_LDC_THR
        OSA_printf(" LDC: Unknown CMD = 0x%04x\n", cmd);
        #endif

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
    }
  }

  #ifdef AVSERVER_DEBUG_VIDEO_LDC_THR
  OSA_printf(" LDC: Delete...\n");
  #endif

  VIDEO_ldcTskDelete();

  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);



  #ifdef AVSERVER_DEBUG_VIDEO_LDC_THR
  OSA_printf(" LDC: Delete...DONE\n");
  #endif

  return OSA_SOK;
}

int VIDEO_ldcCreate()
{
  int status;

  status = OSA_tskCreate( &gVIDEO_ctrl.ldcTsk, VIDEO_ldcTskMain, VIDEO_LDC_THR_PRI, VIDEO_LDC_STACK_SIZE, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }

  return status;
}

int VIDEO_ldcDelete()
{
  int status;

  status = OSA_tskDelete( &gVIDEO_ctrl.ldcTsk );

  return status;
}
