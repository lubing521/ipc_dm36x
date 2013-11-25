#include <avserver.h>
#include <alarm_msg_drv.h>

int VIDEO_motionCopyRun( OSA_BufInfo *pBufInfo )
{
  int status, motionBufId;
  OSA_BufInfo *pMotionBufInfo;

  if(pBufInfo->size > gVIDEO_ctrl.motionStream.maxbufsize) {
		OSA_ERROR("Buffer size over limit \n");
		return OSA_EFAIL;
  }

  status = OSA_bufGetEmpty(&gVIDEO_ctrl.motionStream.bufMotionIn, &motionBufId, OSA_TIMEOUT_NONE);

  if(status==OSA_SOK) {

    pMotionBufInfo = OSA_bufGetBufInfo(&gVIDEO_ctrl.motionStream.bufMotionIn, motionBufId);

    OSA_assert(pMotionBufInfo!=NULL);

    if(pMotionBufInfo!=NULL) {

      #ifdef AVSERVER_DEBUG_VIDEO_MOTION_THR
        OSA_printf(" MOTION: Copying to pBufInfo->size = %d \n", pBufInfo->size );
        OSA_printf(" MOTION: Copying to pBufInfo->width = %d \n", pBufInfo->width );
        OSA_printf(" MOTION: Copying to pBufInfo->height = %d \n", pBufInfo->height );
        OSA_printf(" MOTION: Copying to pBufInfo->isKeyFrame = %d \n", pBufInfo->isKeyFrame );
        OSA_printf(" MOTION: Copying to pBufInfo->codecType = %d \n", pBufInfo->codecType);
      #endif

		pMotionBufInfo->size 		= pBufInfo->size;
		pMotionBufInfo->width 		= pBufInfo->width;
		pMotionBufInfo->height 		= pBufInfo->height;
		pMotionBufInfo->isKeyFrame	= pBufInfo->isKeyFrame;
		pMotionBufInfo->codecType	= pBufInfo->codecType;

		memcpy( pMotionBufInfo->virtAddr, pBufInfo->virtAddr, pBufInfo->size);
    }

    OSA_bufPutFull(&gVIDEO_ctrl.motionStream.bufMotionIn, motionBufId);
    OSA_tskSendMsg(&gVIDEO_ctrl.motionTsk, NULL, AVSERVER_CMD_NEW_DATA, NULL, 0);
  }

  return status;
}

int VIDEO_motionTskCreate()
{
  int status = OSA_SOK;

  gVIDEO_ctrl.motionStream.algMotionHndl = ALG_motionDetectCreate(NULL,NULL);

  if( gVIDEO_ctrl.motionStream.algMotionHndl == NULL )
  {
		return OSA_EFAIL;
  }

  return status;
}

int VIDEO_motionTskDelete()
{
  int status = OSA_SOK;

  ALG_motionDetectDelete(gVIDEO_ctrl.motionStream.algMotionHndl);

  return status;
}


int VIDEO_motionTskRun()
{
	int status, motionVal, inBufId;
	OSA_BufInfo *pMotionBufInfo;
	ALG_MotionDetectRunPrm tMotionDetectPrm;
	ALG_MotionDetectRunStatus  tMotionDetectStatus;
	static int motionDetectCnt = 0;

	status = OSA_bufGetFull(&gVIDEO_ctrl.motionStream.bufMotionIn, &inBufId, OSA_TIMEOUT_FOREVER);
	if(status!=OSA_SOK) {
		OSA_ERROR("OSA_bufGetFull()\n");
		return status;
	}

	#ifdef AVSERVER_DEBUG_VIDEO_MOTION_THR
	 OSA_printf(" MOTION: Get Buf %d \n", inBufId);
	#endif

	pMotionBufInfo = OSA_bufGetBufInfo(&gVIDEO_ctrl.motionStream.bufMotionIn, inBufId);
	#ifdef AVSERVER_DEBUG_VIDEO_MOTION_THR
	OSA_printf(" MOTION: pMotionBufInfo->width = %d \n", pMotionBufInfo->width);
	OSA_printf(" MOTION: pMotionBufInfo->height =  %d \n", pMotionBufInfo->height);
	#endif

	/*Do detect and alarm here */
	AVSERVER_lock();

	tMotionDetectPrm.mbMvInfo 		= pMotionBufInfo->virtAddr;
	tMotionDetectPrm.ImageWidth 	= pMotionBufInfo->width;
	tMotionDetectPrm.ImageHeight	= pMotionBufInfo->height;
	tMotionDetectPrm.isKeyFrame 	= pMotionBufInfo->isKeyFrame;
	tMotionDetectPrm.codecType		= pMotionBufInfo->codecType;
	tMotionDetectPrm.isDateTimeDraw = 0;
	tMotionDetectPrm.motionenable	= gVIDEO_ctrl.motionStream.motionEnable;
	tMotionDetectPrm.motioncenable	= gVIDEO_ctrl.motionStream.motionCEnable;
	tMotionDetectPrm.motioncvalue	= gVIDEO_ctrl.motionStream.motionCValue;
	tMotionDetectPrm.motionlevel	= gVIDEO_ctrl.motionStream.motionLevel;
	tMotionDetectPrm.motionsens		= gVIDEO_ctrl.motionStream.motionCValue;
    tMotionDetectPrm.windowWidth 	= (tMotionDetectPrm.ImageWidth / ALG_MOTION_DETECT_HORZ_REGIONS);
    tMotionDetectPrm.windowHeight	= (tMotionDetectPrm.ImageHeight / ALG_MOTION_DETECT_VERT_REGIONS);
	tMotionDetectPrm.blockNum 		= gVIDEO_ctrl.motionStream.motionBlock;

	AVSERVER_unlock();
	//tMotionDetectStatus structure will have the status for motion detected for each selected window.

	if(tMotionDetectPrm.motionenable == 1)
	{
    	OSA_prfBegin(&gAVSERVER_ctrl.motionPrf);

		motionVal = ALG_motionDetectRun(gVIDEO_ctrl.motionStream.algMotionHndl, &tMotionDetectPrm,&tMotionDetectStatus);

    	OSA_prfEnd(&gAVSERVER_ctrl.motionPrf, 1);

		if(motionVal == ALG_MOTION_S_DETECT)
		{
#ifdef AVSERVER_DEBUG_VIDEO_MOTION_THR
			OSA_printf(" MOTION: DETECTED \n");
#endif
			/* send motion dectect once in 5 frames to avoid too many messages */
			if(motionDetectCnt>5)
				motionDetectCnt=0;
			if((gAVSERVER_config.alarmEnable)&&(motionDetectCnt==0)) {
				SendAlarmMotionrDetect(VIDEO_streamGetJPGSerialNum());
			}
			motionDetectCnt++;
		}
		else
		{
#ifdef AVSERVER_DEBUG_VIDEO_MOTION_THR
			OSA_printf(" MOTION: NOT DETECTED \n");
#endif
			motionDetectCnt=0;
		}
	}

	OSA_bufPutEmpty(&gVIDEO_ctrl.motionStream.bufMotionIn, inBufId);

	#ifdef AVSERVER_DEBUG_VIDEO_MOTION_THR
	OSA_printf(" MOTION: Put Buf %d \n", inBufId);
	#endif

	return status;
}

int VIDEO_motionTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  #ifdef AVSERVER_DEBUG_VIDEO_MOTION_THR
  OSA_printf(" MOTION: Recevied CMD = 0x%04x\n", cmd);
  #endif

  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_MOTION_THR
  OSA_printf(" MOTION: Create...\n");
  #endif

  status = VIDEO_motionTskCreate();

  OSA_tskAckOrFreeMsg(pMsg, status);

  if(status!=OSA_SOK) {
    OSA_ERROR("VIDEO_motionTskCreate()\n");
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_MOTION_THR
  OSA_printf(" MOTION: Create...DONE\n");
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
        VIDEO_motionTskRun();
        break;

      default:

        #ifdef AVSERVER_DEBUG_VIDEO_MOTION_THR
        OSA_printf(" MOTION: Unknown CMD = 0x%04x\n", cmd);
        #endif

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
    }
  }

  #ifdef AVSERVER_DEBUG_VIDEO_MOTION_THR
  OSA_printf(" MOTION: Delete...\n");
  #endif

  VIDEO_motionTskDelete();

  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  #ifdef AVSERVER_DEBUG_VIDEO_MOTION_THR
  OSA_printf(" MOTION: Delete...DONE\n");
  #endif

  return OSA_SOK;
}

int VIDEO_motionCreate()
{
  int status;

  status = OSA_tskCreate( &gVIDEO_ctrl.motionTsk, VIDEO_motionTskMain, VIDEO_MOTION_THR_PRI, VIDEO_MOTION_STACK_SIZE, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }

  return status;
}

int VIDEO_motionDelete()
{
  int status;

  status = OSA_tskDelete( &gVIDEO_ctrl.motionTsk );

  return status;
}
