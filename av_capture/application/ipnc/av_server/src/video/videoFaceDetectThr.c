#include <avserver.h>

#include "alg_face.h"  /* ANR */
#include "alg_ti_facetrack.h"

int VIDEO_fdCopyRun(int tskId, int streamId, OSA_BufInfo *pBufInfo)
{
  VIDEO_CaptureStream *pCaptureStream;
  OSA_BufInfo *pFdBufInfo;
  VIDEO_BufHeader *pInBufHeader, *pOutBufHeader;

  DRV_FrameCopyPrm frameCopy;

  int status = OSA_SOK, fdBufId;

  if(streamId<0 || streamId >= AVSERVER_MAX_STREAMS) {
    OSA_ERROR("Invalid streamId %d\n", streamId);
    return OSA_EFAIL;
  }

  if(streamId != gAVSERVER_config.faceDetectConfig.captureStreamId)
    return OSA_SOK;

  pCaptureStream = &gVIDEO_ctrl.captureStream[streamId];

  if(pCaptureStream->faceDetectCopyTsk != tskId)
    return OSA_SOK;

  status = OSA_bufGetEmpty(&gVIDEO_ctrl.faceDetectStream.bufFdIn, &fdBufId, OSA_TIMEOUT_NONE);

  if(status==OSA_SOK) {

    pFdBufInfo = OSA_bufGetBufInfo(&gVIDEO_ctrl.faceDetectStream.bufFdIn, fdBufId);

    OSA_assert(pFdBufInfo!=NULL);

    if(pFdBufInfo!=NULL) {

      pInBufHeader = (VIDEO_BufHeader *)pBufInfo->virtAddr;
      pOutBufHeader = (VIDEO_BufHeader *)pFdBufInfo->virtAddr;

      OSA_assert(pInBufHeader!=NULL);

      #ifdef AVSERVER_DEBUG_VIDEO_FD_THR
        #ifdef AVSERVER_DEBUG_RUNNING
        OSA_printf(" FD: Copying to Buf %d \n", fdBufId);
        #endif
      #endif

      frameCopy.srcPhysAddr = pBufInfo->physAddr + VIDEO_BUF_HEADER_SIZE;
      frameCopy.srcVirtAddr = pBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;
      frameCopy.dstPhysAddr = pFdBufInfo->physAddr + VIDEO_BUF_HEADER_SIZE;
      frameCopy.dstVirtAddr = pFdBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;
      frameCopy.srcOffsetH  = pInBufHeader->offsetH;
      frameCopy.srcOffsetV  = pInBufHeader->offsetV;
      frameCopy.dstOffsetH  = gVIDEO_ctrl.faceDetectStream.fdOffsetH;
      frameCopy.dstOffsetV  = gVIDEO_ctrl.faceDetectStream.fdHeight;
      frameCopy.copyWidth   = gVIDEO_ctrl.faceDetectStream.fdWidth;
      frameCopy.copyHeight  = gVIDEO_ctrl.faceDetectStream.fdHeight;
      frameCopy.dataFormat  = gAVSERVER_config.captureYuvFormat;
      frameCopy.srcStartX   = pInBufHeader->startX;
      frameCopy.srcStartY   = pInBufHeader->startY;
      frameCopy.dstStartX   = 0;
      frameCopy.dstStartY   = 0;

	  if(gAVSERVER_config.faceDetectConfig.fdEnable || gAVSERVER_config.faceDetectConfig.frIdentify||
		gAVSERVER_config.faceDetectConfig.frRegUsr || gAVSERVER_config.faceDetectConfig.frDelUsr)
	  {
		  status = DRV_frameCopy(NULL, &frameCopy);
		  if(status!=OSA_SOK) {
			OSA_ERROR("DRV_frameCopy(srcOffset:%dx%d dstOffset:%dx%d wxh:%dx%d)\n",
			  frameCopy.srcOffsetH, frameCopy.srcOffsetV,
			  frameCopy.dstOffsetH, frameCopy.dstOffsetV,
			  frameCopy.copyWidth, frameCopy.copyHeight
				);
		  }
      }

      memcpy(pOutBufHeader, pInBufHeader, sizeof(VIDEO_BufHeader));
    }

    OSA_bufPutFull(&gVIDEO_ctrl.faceDetectStream.bufFdIn, fdBufId);
    OSA_tskSendMsg(&gVIDEO_ctrl.fdTsk, NULL, AVSERVER_CMD_NEW_DATA, NULL, 0);
  }

  return status;
}

int VIDEO_fdGetFaceStatus(DRV_FaceDetectRunStatus *faceStatus)
{
  OSA_mutexLock(&gVIDEO_ctrl.faceDetectStream.statusLock);

  faceStatus->numFaces = gVIDEO_ctrl.faceDetectStream.faceStatus.numFaces;
  faceStatus->type = gVIDEO_ctrl.faceDetectStream.faceStatus.type;

  if(faceStatus->numFaces)
    memcpy(faceStatus->info, &gVIDEO_ctrl.faceDetectStream.faceStatus.info, sizeof(CSL_FaceDetectFaceStatus)*faceStatus->numFaces);

  faceStatus->frFaces = gVIDEO_ctrl.faceDetectStream.faceStatus.frFaces;
  if(faceStatus->frFaces)
    memcpy(faceStatus->frInfo, &gVIDEO_ctrl.faceDetectStream.faceStatus.frInfo, sizeof(DRV_FaceRecogStatus)*faceStatus->numFaces);

  OSA_mutexUnlock(&gVIDEO_ctrl.faceDetectStream.statusLock);

  return OSA_SOK;
}

static Bool fdStabilizeStatus;  /* ANR */
Bool VIDEO_getFdStabilize()
{
	return fdStabilizeStatus;
}

Bool VIDEO_fdStabilize(DRV_FaceDetectRunStatus *faceStatus)
{
	static int fdHysterisis=0;
	static int prevFaceCount=0;

	if((faceStatus->numFaces > prevFaceCount) || (fdHysterisis > 10)) {
		fdHysterisis = 0;
		fdStabilizeStatus = TRUE;
	}
	else {
		fdHysterisis ++;
		fdStabilizeStatus = FALSE;
	}

	prevFaceCount = faceStatus->numFaces;
	return fdStabilizeStatus;
}

int VIDEO_fdSetFaceStatus(DRV_FaceDetectRunStatus *faceStatus, OSA_BufInfo *pBufInfo)
{
  int i;
  VIDEO_BufHeader *pBufHeader;

  OSA_mutexLock(&gVIDEO_ctrl.faceDetectStream.statusLock);

  if(!gAVSERVER_config.faceDetectConfig.fdTracker) {
	if(VIDEO_getFdStabilize() == FALSE)
  	{
		OSA_mutexUnlock(&gVIDEO_ctrl.faceDetectStream.statusLock);
		return OSA_SOK;
  	}
  }

  gVIDEO_ctrl.faceDetectStream.faceStatus.numFaces = faceStatus->numFaces;

  if(faceStatus->numFaces) {

    #ifdef AVSERVER_DEBUG_VIDEO_FD_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" FD: NumFaces %d \n", faceStatus->numFaces);
      #endif
    #endif

    pBufHeader = (VIDEO_BufHeader *)pBufInfo->virtAddr;

    memcpy(&gVIDEO_ctrl.faceDetectStream.faceStatus.info, faceStatus->info, sizeof(CSL_FaceDetectFaceStatus)*faceStatus->numFaces);

    gVIDEO_ctrl.faceDetectStream.faceStatus.frFaces = faceStatus->frFaces;
    if(faceStatus->frFaces)
        memcpy(&gVIDEO_ctrl.faceDetectStream.faceStatus.frInfo, faceStatus->frInfo, sizeof(DRV_FaceRecogStatus)*faceStatus->numFaces);

    for(i=0; i<faceStatus->numFaces; i++) {
      gVIDEO_ctrl.faceDetectStream.faceStatus.info[i].centerX += pBufHeader->startX;
      gVIDEO_ctrl.faceDetectStream.faceStatus.info[i].centerY += pBufHeader->startY;
    }
  }

  OSA_mutexUnlock(&gVIDEO_ctrl.faceDetectStream.statusLock);

  return OSA_SOK;
}

int VIDEO_fdSetFaceType(DRV_FaceDetectRunType *faceType)
{
	OSA_mutexLock(&gVIDEO_ctrl.faceDetectStream.statusLock);

	gVIDEO_ctrl.faceDetectStream.faceStatus.type = faceType->type;

	OSA_mutexUnlock(&gVIDEO_ctrl.faceDetectStream.statusLock);

	return OSA_SOK;
}


int VIDEO_fdTskRun()
{
  DRV_FaceDetectRunPrm  facePrm;
  DRV_FaceDetectRunStatus faceStatus;
  int status=OSA_SOK, inBufId;
  OSA_BufInfo *pBufInfo;
  Uint32 ffProcess = 0;
  ALG_FaceOpenPrm faceOpenPrm;
  ALG_FaceDetectRunPrm  *alg_facePrm;
  ALG_FaceDetectRunStatus *alg_faceStatus;
  static unsigned short frconflevel = 0;
  static unsigned short frdatabase = 1;

  status = OSA_bufGetFull(&gVIDEO_ctrl.faceDetectStream.bufFdIn, &inBufId, OSA_TIMEOUT_FOREVER);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_bufGetFull()\n");
    return status;
  }

  pBufInfo = OSA_bufGetBufInfo(&gVIDEO_ctrl.faceDetectStream.bufFdIn, inBufId);

  OSA_assert(pBufInfo!=NULL);

  faceStatus.numFaces = 0;
  faceStatus.frFaces  = 0;

  if((gAVSERVER_config.faceDetectConfig.fdEnable || gAVSERVER_config.faceDetectConfig.frIdentify||
  	gAVSERVER_config.faceDetectConfig.frRegUsr || gAVSERVER_config.faceDetectConfig.frDelUsr)
  	&& gVIDEO_ctrl.faceDetectStream.fdWidth<=gVIDEO_ctrl.faceDetectStream.fdOffsetH) {

    OSA_assert(pBufInfo->physAddr!=NULL);

    #ifdef AVSERVER_DEBUG_VIDEO_FD_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      	OSA_printf(" FD: InBuf %d \n", inBufId);
      #endif
    #endif

   	facePrm.inPhysAddr      = pBufInfo->physAddr + VIDEO_BUF_HEADER_SIZE;
    facePrm.inWidth         = gVIDEO_ctrl.faceDetectStream.fdWidth;
    facePrm.inHeight        = gVIDEO_ctrl.faceDetectStream.fdHeight;

    facePrm.detectThres     = ((gAVSERVER_config.faceDetectConfig.fdconflevel*10)/100)-1; //8; Earlier value
	if(facePrm.detectThres > DRV_FACE_MAX_THRESHOLD)
		facePrm.detectThres = DRV_FACE_MAX_THRESHOLD;

    if(gAVSERVER_config.faceDetectConfig.fddirection == 2)
    	facePrm.detectCondition = DRV_FACE_DETECT_CONDITION_RIGHT;
    else if(gAVSERVER_config.faceDetectConfig.fddirection == 1)
    	facePrm.detectCondition = DRV_FACE_DETECT_CONDITION_LEFT;
    else
       	facePrm.detectCondition = DRV_FACE_DETECT_CONDITION_UP;

    OSA_prfBegin(&gAVSERVER_ctrl.fdPrf);

    status = DRV_faceDetectRun(&facePrm, &faceStatus);

    OSA_prfEnd(&gAVSERVER_ctrl.fdPrf, 1);

    if(status!=OSA_SOK) {
      OSA_ERROR("DRV_faceDetectRun()\n");
      gVIDEO_ctrl.faceDetectStream.fdError=TRUE;
      gAVSERVER_config.faceDetectConfig.fdEnable = FALSE;
    }

	if(gAVSERVER_config.faceDetectConfig.fdTracker)
	{
    	OSA_prfBegin(&gAVSERVER_ctrl.fdtrkPrf);

		/* Face Tracker Run Command */
		Uint8* bufptr = pBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;
		ALG_faceTrackerRun(bufptr, gVIDEO_ctrl.faceDetectStream.fdOffsetH, facePrm.inHeight, &faceStatus);

    	OSA_prfEnd(&gAVSERVER_ctrl.fdtrkPrf, 1);
	}
  }

  if((VIDEO_fdStabilize(&faceStatus) == TRUE) && ( faceStatus.numFaces > 0) &&
  	(gAVSERVER_config.faceDetectConfig.frRegUsr || gAVSERVER_config.faceDetectConfig.frIdentify)){

	facePrm.inVirtAddr = pBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;
    alg_facePrm = &facePrm;
    alg_faceStatus = &faceStatus;

    if(frdatabase != gAVSERVER_config.faceDetectConfig.frdatabase){
  		status = ALG_facialFeatureClose();
  		status = ALG_facialFeatureRelMem();
		frconflevel = 0;
		frdatabase = gAVSERVER_config.faceDetectConfig.frdatabase;
	  	/* Set path for Face album */
	  	if(gAVSERVER_config.faceDetectConfig.frdatabase==0)
	  		strcpy(faceOpenPrm.AlbumPath, FR_ALBUM_MMC_SD_PATH);
	  	else
	  		strcpy(faceOpenPrm.AlbumPath, FR_ALBUM_NAND_PATH);

	  	status = ALG_facialFeatureGetMem();
	  	status = ALG_facialFeatureOpen(&faceOpenPrm);
	}

    if(frconflevel != gAVSERVER_config.faceDetectConfig.frconflevel) {
   	  ALG_fffr_SetIdentifyConfig(gAVSERVER_config.faceDetectConfig.frconflevel*10);
  	  ALG_fffr_SetRegConfig(gAVSERVER_config.faceDetectConfig.frconflevel*10);
  	  frconflevel = gAVSERVER_config.faceDetectConfig.frconflevel;
    }

    if(gAVSERVER_config.faceDetectConfig.frIdentify){  //identify user
	  ffProcess = FR_IDENTIFY; /* for Face Recognition */
  	  status = ALG_facialFeatureRun(alg_faceStatus, alg_facePrm, ffProcess);
    }
    else if(gAVSERVER_config.faceDetectConfig.frRegUsr){  //Register new user
	  ffProcess = FR_REG_USER; /* for Face Recognition */
  	  status = ALG_facialFeatureRun(alg_faceStatus, alg_facePrm, ffProcess);
	  if(status == FF_SUCCESS){
         gAVSERVER_config.faceDetectConfig.frRegUsr = FALSE;
	  }
    }
  }

  if(gAVSERVER_config.faceDetectConfig.frDelUsr) {  //Delete All User
  	status = ALG_facialFeature_DeleteAllUser();
	if(status == FF_SUCCESS){
       gAVSERVER_config.faceDetectConfig.frDelUsr = FALSE;
       OSA_printf("DELETED ALL USER ALBUM\n");
	}
  }

  VIDEO_fdSetFaceStatus(&faceStatus, pBufInfo);

  OSA_bufPutEmpty(&gVIDEO_ctrl.faceDetectStream.bufFdIn, inBufId);

  return status;
}

int VIDEO_fdTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status;
  Bool done=FALSE, ackMsg = FALSE;
  ALG_FaceOpenPrm faceOpenPrm;
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  #ifdef AVSERVER_DEBUG_VIDEO_FD_THR
  OSA_printf(" FD: Received CMD = 0x%04x\n", cmd);
  #endif

  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_FD_THR
  OSA_printf(" FD: Create...\n");
  #endif

  status = DRV_faceDetectOpen();

  /* Set default path for Face album */
  strcpy(faceOpenPrm.AlbumPath, FR_ALBUM_NAND_PATH);
  status =  ALG_facialFeatureGetMem();
  status = ALG_facialFeatureOpen(&faceOpenPrm);

  status = ALG_faceTrackerOpen();

  OSA_tskAckOrFreeMsg(pMsg, status);

  if(status!=OSA_SOK)
    return OSA_SOK;

  #ifdef AVSERVER_DEBUG_VIDEO_FD_THR
  OSA_printf(" FD: Create...DONE\n");
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

        VIDEO_fdTskRun();

        break;
      default:

        #ifdef AVSERVER_DEBUG_VIDEO_FD_THR
        OSA_printf(" FD: Unknown CMD = 0x%04x\n", cmd);
        #endif

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
    }
  }

  #ifdef AVSERVER_DEBUG_VIDEO_FD_THR
  OSA_printf(" FD: Delete...\n");
  #endif

  status = DRV_faceDetectClose();

  status = ALG_facialFeatureClose();
  status = ALG_facialFeatureRelMem();

  ALG_faceTrackerClose();

  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  #ifdef AVSERVER_DEBUG_VIDEO_FD_THR
  OSA_printf(" FD: Delete...DONE\n");
  #endif

  if(gVIDEO_ctrl.faceDetectStream.fdError) {
    OSA_printf(" FD: FD timeout happened while running !!!\n");
  }

  return OSA_SOK;
}

int VIDEO_fdCreate()
{
  int status;

  status = OSA_mutexCreate(&gVIDEO_ctrl.faceDetectStream.statusLock);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_mutexCreate()\n");
    return status;
  }

  status = OSA_tskCreate( &gVIDEO_ctrl.fdTsk, VIDEO_fdTskMain, VIDEO_FD_THR_PRI, VIDEO_FD_STACK_SIZE, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }

  return status;
}

int VIDEO_fdDelete()
{
  int status;

  status = OSA_tskDelete( &gVIDEO_ctrl.fdTsk );
  status |= OSA_mutexDelete(&gVIDEO_ctrl.faceDetectStream.statusLock);

  return status;
}
