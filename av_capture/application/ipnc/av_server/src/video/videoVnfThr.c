#include <avserver.h>

int VIDEO_vnfTskCreate(Int32 sysRegBase)
{
  int streamId = 0;
  ALG_VnfCreate createPrm;
  createPrm.sysBaseAddr = sysRegBase;
  int paramSetId = 0, mode = 0, status = OSA_EFAIL;

  createPrm.dataFormat      = DRV_DATA_FORMAT_YUV420;
  createPrm.pVnfParams      = NULL;
  /* Get NF enble status & pass as paramSetId - ITT */
  if(  gAVSERVER_config.captureConfig[streamId].snfEnable
	  || gAVSERVER_config.captureConfig[streamId].tnfEnable){
    paramSetId = 2;
  }else{
	paramSetId = 1;
  }

  OSA_printf("\n NF enable status for ITT Paramset Boot... %d \n", paramSetId);
  status = DRV_imageTuneLoadParamSet(mode, paramSetId);

  for(streamId=0; streamId<gAVSERVER_config.numCaptureStream; streamId++)
  {
    createPrm.width           = gAVSERVER_config.captureConfig[streamId].width+ALG_VNF_PIXEL_PAD;
    createPrm.height          = gAVSERVER_config.captureConfig[streamId].height+ALG_VNF_PIXEL_PAD;
    createPrm.offsetH         = gVIDEO_ctrl.captureStream[streamId].captureOutOffsetH;
    createPrm.offsetV         = gVIDEO_ctrl.captureStream[streamId].captureOutOffsetV;

#if 0
	if(  gAVSERVER_config.captureConfig[streamId].snfEnable
	  && gAVSERVER_config.captureConfig[streamId].tnfEnable
	  ) {
	  createPrm.mode            = ALG_VNF_MODE_KTNF;
	}
	else if(gAVSERVER_config.captureConfig[streamId].tnfEnable ) {
	  createPrm.mode            = ALG_VNF_MODE_ONLY_TNF;
	}
	else if(gAVSERVER_config.captureConfig[streamId].snfEnable ) {
	  createPrm.mode            = ALG_VNF_MODE_ONLY_KNF;
	}
	else {
	  createPrm.mode            = ALG_VNF_MODE_NO_NF;
	}
#else
	if(  gAVSERVER_config.captureConfig[streamId].snfEnable
	  || gAVSERVER_config.captureConfig[streamId].tnfEnable) {
	  createPrm.mode            = ALG_VNF_MODE_KTNF;
	  createPrm.pVnfParams		= ImageTune_imgsGetVnfConfig(0);
	}
	else {
	  createPrm.mode            = ALG_VNF_MODE_NO_NF;
	}

#endif

  	createPrm.snfStrength = gAVSERVER_config.vnfDemoEnable ? SNF_CUSTOM:SNF_DEFAULT;

    gVIDEO_ctrl.captureStream[streamId].algVnfHndl = ALG_vnfCreate(&createPrm);

    if(gVIDEO_ctrl.captureStream[streamId].algVnfHndl==NULL) {
      OSA_ERROR("ALG_vnfCreate()\n");
      return OSA_EFAIL;
    }
  }


  return OSA_SOK;
}

int VIDEO_vnfTskDelete()
{
  int i;

  for(i=0; i<gAVSERVER_config.numCaptureStream; i++)
  {
    ALG_vnfDelete(gVIDEO_ctrl.captureStream[i].algVnfHndl);
  }

  return OSA_SOK;
}

int VIDEO_vnfTskRun(int streamId)
{
  int status=OSA_EFAIL, inBufId, outBufId;
  OSA_BufInfo *pInBufInfo, *pOutBufInfo;
  VIDEO_BufHeader *pInBufHeader, *pOutBufHeader;
  ALG_VnfRunPrm vnfPrm;
  ALG_VnfStatus vnfStatus;
  DRV_FrameCopyPrm frameCopy;

  pInBufInfo = AVSERVER_bufGetFull( VIDEO_TSK_VNF, streamId, &inBufId, OSA_TIMEOUT_FOREVER);

  OSA_assert(pInBufInfo!=NULL);

  if(pInBufInfo!=NULL) {

    pOutBufInfo = AVSERVER_bufGetEmpty( VIDEO_TSK_VNF, streamId, &outBufId, OSA_TIMEOUT_FOREVER);

    OSA_assert(pOutBufInfo!=NULL);

    if(pOutBufInfo!=NULL) {

      VIDEO_vsGetResult(VIDEO_TSK_VNF, streamId, pInBufInfo);

      pInBufHeader  = (VIDEO_BufHeader*)pInBufInfo->virtAddr;
      pOutBufHeader = (VIDEO_BufHeader*)pOutBufInfo->virtAddr;

      OSA_assert(pInBufHeader!=NULL);
      OSA_assert(pOutBufHeader!=NULL);

      vnfPrm.inAddr   	  = pInBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;
      vnfPrm.outAddr      = pOutBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;
      vnfPrm.inStartX     = pInBufHeader->startX;
      vnfPrm.inStartY     = pInBufHeader->startY;
      vnfPrm.pVnfParams   = NULL;

      #ifdef AVSERVER_DEBUG_VIDEO_VNF_THR
        #ifdef AVSERVER_DEBUG_RUNNING
        OSA_printf(" VNF: Stream %d InBuf %d OutBuf %d (%d,%d)\n",
            streamId, inBufId, outBufId,
            vnfPrm.inStartX,
            vnfPrm.inStartY
          );
        #endif
      #endif

   	  if(  gAVSERVER_config.captureConfig[streamId].snfEnable
    	 && gAVSERVER_config.captureConfig[streamId].tnfEnable
    	 ) {
    	  vnfPrm.mode            = ALG_VNF_MODE_KTNF;
      }
      else if(gAVSERVER_config.captureConfig[streamId].tnfEnable ) {
    	  vnfPrm.mode            = ALG_VNF_MODE_ONLY_TNF;
      }
      else if(gAVSERVER_config.captureConfig[streamId].snfEnable ) {
        vnfPrm.mode            = ALG_VNF_MODE_ONLY_KNF;
      }
      else {
        vnfPrm.mode            = ALG_VNF_MODE_NO_NF;
      }

      if(   gAVSERVER_config.captureConfig[streamId].tnfEnable
         || gAVSERVER_config.captureConfig[streamId].snfEnable
         ) {

		if((vnfPrm.mode==ALG_VNF_MODE_ONLY_TNF)||(vnfPrm.mode==ALG_VNF_MODE_NO_NF)) {
			vnfPrm.inStartX = 16;
			vnfPrm.inStartY = 9;
		}

        OSA_prfBegin(&gAVSERVER_ctrl.vnfPrf[streamId]);

      	status = ALG_vnfRun(gVIDEO_ctrl.captureStream[streamId].algVnfHndl, &vnfPrm, &vnfStatus);
    		if(status!=OSA_SOK)
    			OSA_ERROR("ALG_vnfRun()\n");

        OSA_prfEnd(&gAVSERVER_ctrl.vnfPrf[streamId], 1);

        memcpy(pOutBufHeader, pInBufHeader, sizeof(*pOutBufHeader));

        pOutBufHeader->startX = vnfStatus.outStartX;
        pOutBufHeader->startY = vnfStatus.outStartY;
        pOutBufHeader->width  = vnfStatus.outWidth;
      	pOutBufHeader->height = vnfStatus.outHeight;
        if((vnfPrm.mode==ALG_VNF_MODE_ONLY_TNF)||(vnfPrm.mode==ALG_VNF_MODE_NO_NF))
        {
        	pOutBufHeader->width  -= ALG_VNF_PIXEL_PAD;
		}//Hack , need to figure out a proper fix
	  }
	  else {
        frameCopy.srcPhysAddr = pInBufInfo->physAddr + VIDEO_BUF_HEADER_SIZE;
        frameCopy.srcVirtAddr = pInBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;
        frameCopy.dstPhysAddr = pOutBufInfo->physAddr + VIDEO_BUF_HEADER_SIZE;
        frameCopy.dstVirtAddr = pOutBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;
        frameCopy.srcOffsetH  = pInBufHeader->offsetH;
        frameCopy.srcOffsetV  = pInBufHeader->offsetV;
        frameCopy.dstOffsetH  = pInBufHeader->offsetH;
        frameCopy.dstOffsetV  = pInBufHeader->offsetV;
        frameCopy.copyWidth   = pInBufHeader->width;
        frameCopy.copyHeight  = pInBufHeader->height;
        frameCopy.srcStartX   = pInBufHeader->startX;
        frameCopy.srcStartY   = pInBufHeader->startY;
        frameCopy.dstStartX   = 0;
        frameCopy.dstStartY   = 0;
        frameCopy.dataFormat  = gAVSERVER_config.captureYuvFormat;

        status = DRV_frameCopy(NULL, &frameCopy);
        if(status!=OSA_SOK) {
          OSA_ERROR("DRV_frameCopy()\n");
        }

        memcpy(pOutBufHeader, pInBufHeader, sizeof(*pOutBufHeader));

        pOutBufHeader->startX = 0;
        pOutBufHeader->startY = ALG_VNF_PIXEL_PAD/2;
        pOutBufHeader->width  -= ALG_VNF_PIXEL_PAD;
        pOutBufHeader->height -= ALG_VNF_PIXEL_PAD;
      }

      #ifdef AVSERVER_DEBUG_VIDEO_VNF_THR
        #ifdef AVSERVER_DEBUG_RUNNING
        OSA_printf(" VNF: Stream %d InBuf %d OutBuf %d (%d,%d %dx%d)\n",
            streamId, inBufId, outBufId,
            pOutBufHeader->startX,
            pOutBufHeader->startY,
            pOutBufHeader->width,
            pOutBufHeader->height
          );
        #endif
      #endif

      VIDEO_fdCopyRun(VIDEO_TSK_VNF, streamId, pOutBufInfo);
      VIDEO_swosdRun(VIDEO_TSK_VNF, streamId, pOutBufInfo);

      AVSERVER_bufPutFull( VIDEO_TSK_VNF, streamId, outBufId);

      VIDEO_displayCopyRun(VIDEO_TSK_VNF, streamId, pOutBufInfo);
    }

    AVSERVER_bufPutEmpty( VIDEO_TSK_VNF, streamId, inBufId);
  }

  return status;
}

int VIDEO_vnfTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status, streamId;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);
  Int32 sysRegBase;

  #ifdef AVSERVER_DEBUG_VIDEO_VNF_THR
  OSA_printf(" VNF: Recevied CMD = 0x%04x\n", cmd);
  #endif

  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_VNF_THR
  OSA_printf(" VNF: Create...\n");
  #endif

  DM365MM_init();
  sysRegBase = DM365MM_mmap(0x01C40000,0x4000);

  status = VIDEO_vnfTskCreate(sysRegBase);

  OSA_tskAckOrFreeMsg(pMsg, status);

  if(status!=OSA_SOK) {
    OSA_ERROR("VIDEO_vnfTskCreate()\n");
    return status;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_VNF_THR
  OSA_printf(" VNF: Create...DONE\n");
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
        streamId = (int)OSA_msgGetPrm(pMsg);

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

        VIDEO_vnfTskRun(streamId);

        break;
      default:

        #ifdef AVSERVER_DEBUG_VIDEO_VNF_THR
        OSA_printf(" VNF: Unknown CMD = 0x%04x\n", cmd);
        #endif

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
    }
  }

  #ifdef AVSERVER_DEBUG_VIDEO_VNF_THR
  OSA_printf(" VNF: Delete...\n");
  #endif

  VIDEO_vnfTskDelete();

  DM365MM_ummap(sysRegBase,0x4000);
  DM365MM_exit();
  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  #ifdef AVSERVER_DEBUG_VIDEO_VNF_THR
  OSA_printf(" VNF: Delete...DONE\n");
  #endif

  return OSA_SOK;
}

int VIDEO_vnfCreate()
{
  int status;

  status = OSA_tskCreate( &gVIDEO_ctrl.vnfTsk, VIDEO_vnfTskMain, VIDEO_VNF_THR_PRI, VIDEO_VNF_STACK_SIZE, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }

  return status;
}

int VIDEO_vnfDelete()
{
  int status;

  status = OSA_tskDelete( &gVIDEO_ctrl.vnfTsk );

  return status;
}
