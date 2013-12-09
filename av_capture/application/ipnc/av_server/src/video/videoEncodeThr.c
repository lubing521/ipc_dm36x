#include <stdlib.h>
#include <string.h>

#include <avserver.h>

int VIDEO_encodeTskCreate()
{
  int i, captureStreamId;
  ALG_VidEncCreate createPrm;

#if 1 // this logic is done to get best performance.
  createPrm.resetHDVICP = (gAVSERVER_config.vnfTskEnable || gAVSERVER_config.aewb_config.saldreEnable)?2:0;
  for(i=0; i<gAVSERVER_config.numEncodeStream; i++)
  {
  	if(createPrm.resetHDVICP!=1){
		if((gAVSERVER_config.encodeConfig[i].encodePreset&ALG_VID_ENC_PRESET_MASK) == ALG_VID_ENC_PRESET_CUSTOM)
  			createPrm.resetHDVICP = (gAVSERVER_config.numEncodeStream>1)?1:createPrm.resetHDVICP;
		else if((gAVSERVER_config.encodeConfig[i].cropWidth < 320)||(gAVSERVER_config.encodeConfig[i].cropHeight < 128))
  			createPrm.resetHDVICP = 1;
  		else if(gAVSERVER_config.encodeConfig[i].codecType!=ALG_VID_CODEC_H264)
  			createPrm.resetHDVICP = 2;
	}
  }
#else
  createPrm.resetHDVICP	= (gAVSERVER_config.numEncodeStream==1)?0:1;
#endif

  for(i=0; i<gAVSERVER_config.numEncodeStream; i++)
  {
    captureStreamId = gAVSERVER_config.encodeConfig[i].captureStreamId;

    createPrm.codec       		= gAVSERVER_config.encodeConfig[i].codecType;
    createPrm.dataFormat  		= gAVSERVER_config.captureYuvFormat;
    createPrm.width       		= gAVSERVER_config.encodeConfig[i].cropWidth;
    createPrm.height      		= gAVSERVER_config.encodeConfig[i].cropHeight;
    createPrm.offsetH     		= gVIDEO_ctrl.captureStream[captureStreamId].captureOutOffsetH;
    createPrm.offsetV     		= gVIDEO_ctrl.captureStream[captureStreamId].captureOutOffsetV;
    createPrm.framerate    		= gAVSERVER_config.encodeConfig[i].frameRateBase;
    createPrm.bitrate     		= gAVSERVER_config.encodeConfig[i].codecBitrate;
    createPrm.keyFrameInterval 	= gAVSERVER_config.sensorFps;
    createPrm.mbMvOutEnable 	= gAVSERVER_config.encodeConfig[i].motionVectorOutputEnable;
    createPrm.qValue      		= gAVSERVER_config.encodeConfig[i].qValue;
    createPrm.rateControlType 	= gAVSERVER_config.encodeConfig[i].rateControlType;
	createPrm.encodePreset		= gAVSERVER_config.encodeConfig[i].encodePreset;
    createPrm.ddrEnable 		= (gAVSERVER_config.vnfTskEnable | gAVSERVER_config.aewb_config.saldreEnable);
	createPrm.enableROI			= gAVSERVER_config.faceDetectConfig.fdROIEnable|gAVSERVER_config.fxdROIEnable;
	createPrm.qpinit			= (createPrm.codec==ALG_VID_CODEC_H264)?28:4;
	createPrm.qpmin				= 1;
	createPrm.qpmax				= (createPrm.codec==ALG_VID_CODEC_H264)?51:31;
	createPrm.packetsize		= 100; //set to 100%
	createPrm.mode        		= ALG_VID_ENC_MODE_FAST_FPS;
	////zeng add
	createPrm.profileIdc		= gAVSERVER_config.encodeConfig[i].profileIdc;
	
	if((gAVSERVER_config.encodeConfig[0].cropWidth > 1280)||(gAVSERVER_config.encodeConfig[0].cropHeight > 960))
    	createPrm.mode        	=  (gAVSERVER_config.numEncodeStream==1)?ALG_VID_ENC_MODE_STANDARD:ALG_VID_ENC_MODE_FAST_FPS;

    gVIDEO_ctrl.encodeStream[i].algEncHndl = ALG_vidEncCreate(&createPrm);

    gVIDEO_ctrl.encodeStream[i].curFrameCount=0;

    if(gVIDEO_ctrl.encodeStream[i].algEncHndl==NULL) {
      OSA_ERROR("ALG_vidEncCreate(%d)", i);
      return OSA_EFAIL;
    }
  }

  return OSA_SOK;
}

int VIDEO_encodeTskDelete()
{
  int i;

  for(i=0; i<gAVSERVER_config.numEncodeStream; i++)
  {
    ALG_vidEncDelete(gVIDEO_ctrl.encodeStream[i].algEncHndl);
  }

  return OSA_SOK;
}

int VIDEO_encodeTskRun(int streamId)
{
  int status=OSA_EFAIL, inBufId, outBufId, encodeId, i;
  OSA_BufInfo *pInBufInfo, *pOutBufInfo, MvBufInfo;
  VIDEO_BufHeader *pInBufHeader, *pOutBufHeader;
  ALG_VidEncRunPrm encPrm;
  ALG_VidEncRunStatus encStatus;
  DRV_FaceDetectRunStatus faceStatus;
  Bool skipEncode;
  Uint32 frameCount, counter = 0;
  Int32 centerX, centerY,size,videoWidth,videoHeight,fdWidth,fdHeight;

  pInBufInfo = AVSERVER_bufGetFull(VIDEO_TSK_ENCODE, streamId, &inBufId, OSA_TIMEOUT_FOREVER);

  OSA_assert(pInBufInfo!=NULL);

  if(pInBufInfo!=NULL) {

    for(i=0; i<gAVSERVER_config.captureConfig[streamId].numEncodes; i++) 
    {
      encodeId = gAVSERVER_config.captureConfig[streamId].encodeStreamId[i];
      skipEncode = FALSE;
      frameCount = (gVIDEO_ctrl.encodeStream[encodeId].curFrameCount+1)%25;
      if( (gAVSERVER_config.encodeConfig[encodeId].frameSkipMask & ((Uint32)1<<frameCount)) == 0)
      {
          skipEncode = TRUE;
          //OSA_printf(" ENCODE: skip streamId=%d,encodeId=%d,frameCount=%d\n", streamId, encodeId,frameCount); 
      }

      gVIDEO_ctrl.encodeStream[encodeId].curFrameCount++;

	  ///zeng mode 
      if(skipEncode)
      {
          continue;
      }

      pOutBufInfo = AVSERVER_bufGetEmpty( VIDEO_TSK_ENCODE, encodeId, &outBufId, OSA_TIMEOUT_FOREVER);

      OSA_assert(pOutBufInfo!=NULL);

      pInBufHeader  = (VIDEO_BufHeader*)pInBufInfo->virtAddr;
      pOutBufHeader = (VIDEO_BufHeader*)pOutBufInfo->virtAddr;

      OSA_assert(pInBufHeader!=NULL);
      OSA_assert(pOutBufHeader!=NULL);

#ifdef AVSERVER_DEBUG_VIDEO_ENCODE_THR
#ifdef AVSERVER_DEBUG_RUNNING
        OSA_printf(" ENCODE: Stream %d InBuf %d OutBuf %d (%d, %d)\n", streamId, inBufId, outBufId, pInBufHeader->startX, pInBufHeader->startY);
#endif
#endif

      if(pOutBufInfo!=NULL) {

        encPrm.inStartX = pInBufHeader->startX + (pInBufHeader->width  - gAVSERVER_config.encodeConfig[encodeId].cropWidth)/2;
        encPrm.inStartY = pInBufHeader->startY + (pInBufHeader->height - gAVSERVER_config.encodeConfig[encodeId].cropHeight)/2;

        encPrm.inStartX = OSA_floor(encPrm.inStartX, 2);

        encPrm.inAddr   = pInBufInfo->virtAddr  + VIDEO_BUF_HEADER_SIZE;
        encPrm.outAddr  = pOutBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;

        encPrm.mbMvInfo 		= NULL;
        encPrm.outDataMaxSize 	= 0;

		if(gAVSERVER_config.faceDetectConfig.fdROIEnable) {
			VIDEO_fdGetFaceStatus(&faceStatus);
			videoWidth 	= gVIDEO_ctrl.captureStream[streamId].captureOutWidth;
			videoHeight = gVIDEO_ctrl.captureStream[streamId].captureOutHeight;
			fdWidth 	= gVIDEO_ctrl.captureStream[gAVSERVER_config.faceDetectConfig.captureStreamId].captureOutWidth;
			fdHeight 	= gVIDEO_ctrl.captureStream[gAVSERVER_config.faceDetectConfig.captureStreamId].captureOutHeight;

			encPrm.roiPrm.numROI = faceStatus.numFaces;

			for(counter = 0;((counter < faceStatus.numFaces) && (counter < ALG_VID_ENC_ROI_MAX_ROI));counter ++)
			{
				centerX = ((faceStatus.info[counter].centerX) * videoWidth ) / (fdWidth);
				centerY = ((faceStatus.info[counter].centerY) * videoHeight ) / (fdHeight);
				size    = (CSL_FACE_DETECT_GET_SIZE(faceStatus.info[counter].sizeConfidence)*videoWidth) / fdWidth;
#if 0
				printf("ROI DEBUG VALUE: %d %d %d %d %d %d %d %d\n", faceStatus.numFaces, faceStatus.info[counter].centerX, faceStatus.info[counter].centerY,
				CSL_FACE_DETECT_GET_SIZE(faceStatus.info[counter].sizeConfidence), videoWidth,videoHeight,fdWidth,fdHeight);
#endif
				encPrm.roiPrm.roiInfo[counter].topLeft_x = centerX - size/2;
				encPrm.roiPrm.roiInfo[counter].topLeft_y = centerY - size/2;
				encPrm.roiPrm.roiInfo[counter].bottomRight_x = centerX + size/2;
				encPrm.roiPrm.roiInfo[counter].bottomRight_y = centerY + size/2;

				if(gAVSERVER_config.faceDetectConfig.privacyMaskEnable){
					encPrm.roiPrm.roiInfo[counter].roiType 		= ALG_VID_ENC_ROI_TYPE_PRIVACYMASK;
					encPrm.roiPrm.roiInfo[counter].roiPriority 	= ALG_VID_ENC_ROI_PRIORITY_LOW;
				}
				else if((faceStatus.type == FACE_T_DETECT)||(faceStatus.type == FACE_T_RECOGIZE)){
					encPrm.roiPrm.roiInfo[counter].roiType 		= ALG_VID_ENC_ROI_TYPE_FACE;
					encPrm.roiPrm.roiInfo[counter].roiPriority 	= ALG_VID_ENC_ROI_PRIORITY_HIGH;
				}
			}
		}
		else if(gAVSERVER_config.fxdROIEnable) {
			videoWidth 	= gVIDEO_ctrl.captureStream[streamId].captureOutWidth;
			videoHeight = gVIDEO_ctrl.captureStream[streamId].captureOutHeight;

			encPrm.roiPrm.numROI = 1;

			encPrm.roiPrm.roiInfo[0].topLeft_x = videoWidth>>2;
			encPrm.roiPrm.roiInfo[0].topLeft_y = videoHeight>>2;
			encPrm.roiPrm.roiInfo[0].bottomRight_x = (videoWidth>>2)*3;
			encPrm.roiPrm.roiInfo[0].bottomRight_y = (videoHeight>>2)*3;

			encPrm.roiPrm.roiInfo[0].roiType 		= ALG_VID_ENC_ROI_TYPE_FOREGROUND;
			encPrm.roiPrm.roiInfo[0].roiPriority 	= ALG_VID_ENC_ROI_PRIORITY_HIGH;
		}
		else if(gAVSERVER_config.encodeConfig[encodeId].numROI){
			encPrm.roiPrm.numROI = gAVSERVER_config.encodeConfig[encodeId].numROI;

			for(counter = 0;((counter < encPrm.roiPrm.numROI) && (counter < ALG_VID_ENC_ROI_MAX_ROI));counter ++) {
				encPrm.roiPrm.roiInfo[counter].topLeft_x = gAVSERVER_config.encodeConfig[streamId].prmROI[counter].startx;
				encPrm.roiPrm.roiInfo[counter].topLeft_y = gAVSERVER_config.encodeConfig[streamId].prmROI[counter].starty;
				encPrm.roiPrm.roiInfo[counter].bottomRight_x =
					gAVSERVER_config.encodeConfig[streamId].prmROI[counter].startx + gAVSERVER_config.encodeConfig[streamId].prmROI[counter].width;
				encPrm.roiPrm.roiInfo[counter].bottomRight_y =
					gAVSERVER_config.encodeConfig[streamId].prmROI[counter].starty + gAVSERVER_config.encodeConfig[streamId].prmROI[counter].height;

				encPrm.roiPrm.roiInfo[counter].roiType 		= ALG_VID_ENC_ROI_TYPE_FOREGROUND;
				encPrm.roiPrm.roiInfo[counter].roiPriority 	= ALG_VID_ENC_ROI_PRIORITY_HIGH;
			}
		}
		else {
			encPrm.roiPrm.numROI = 0;
		}

        AVSERVER_lock();

        if(gVIDEO_ctrl.encodeStream[encodeId].newBitrate) {
          ALG_vidEncSetBitrate(gVIDEO_ctrl.encodeStream[encodeId].algEncHndl, gVIDEO_ctrl.encodeStream[encodeId].newBitrate);
          gVIDEO_ctrl.encodeStream[encodeId].newBitrate = 0;
          //OSA_printf("BITRATE UPDATE: %d \n", encodeId);
        }

		///////zeng add RC		
		if(gVIDEO_ctrl.encodeStream[encodeId].newRcUpdate) {			
			ALG_vidEncSetRCType(gVIDEO_ctrl.encodeStream[encodeId].algEncHndl, gVIDEO_ctrl.encodeStream[encodeId].newRcType);			
			gVIDEO_ctrl.encodeStream[encodeId].newRcUpdate = 0;
		}

		
        if(gVIDEO_ctrl.encodeStream[encodeId].newMotionUpdate) {
        	ALG_vidEncSetMVdataOut(gVIDEO_ctrl.encodeStream[encodeId].algEncHndl, gVIDEO_ctrl.encodeStream[encodeId].newMotionStatus);
            gVIDEO_ctrl.encodeStream[encodeId].newMotionUpdate = 0;
          	//OSA_printf("MOTION UPDATE: %d \n", encodeId);
        }

        if(gVIDEO_ctrl.encodeStream[encodeId].newFps) {
            ALG_vidEncSetFramerate(gVIDEO_ctrl.encodeStream[encodeId].algEncHndl, gAVSERVER_config.encodeConfig[encodeId].frameRateBase);
            gVIDEO_ctrl.encodeStream[encodeId].newFps = FALSE;
          	//OSA_printf("FRAMERATE UPDATE: %d \n", encodeId);
        }

		if(gAVSERVER_config.encodeConfig[encodeId].newCodecPrm) {
			ALG_vidEncSetQPSize(gVIDEO_ctrl.encodeStream[encodeId].algEncHndl, gAVSERVER_config.encodeConfig[encodeId].qpInit,
				gAVSERVER_config.encodeConfig[encodeId].qpMin, gAVSERVER_config.encodeConfig[encodeId].qpMax,
				gAVSERVER_config.encodeConfig[encodeId].packetSize );
			ALG_vidEncSetKeyFrameInterval(gVIDEO_ctrl.encodeStream[encodeId].algEncHndl, gAVSERVER_config.encodeConfig[encodeId].ipRatio,
				gAVSERVER_config.encodeConfig[encodeId].fIframe);

			gAVSERVER_config.encodeConfig[encodeId].newCodecPrm	= 0;
          	//OSA_printf("NEW CODEC PARAM UPDATE: %d \n", encodeId);
		}

		if(gAVSERVER_config.encodeConfig[encodeId].resetCodec) {
			ALG_vidEncSetKeyFrameInterval(gVIDEO_ctrl.encodeStream[encodeId].algEncHndl, gAVSERVER_config.encodeConfig[encodeId].ipRatio,
				gAVSERVER_config.encodeConfig[encodeId].fIframe);
			gAVSERVER_config.encodeConfig[encodeId].resetCodec	= 0;
          	//OSA_printf("RESET CODEC PARAM UPDATE: %d \n", encodeId);
		}

        AVSERVER_unlock();

		/************** SNAP ENABLE ************/
		if((gAVSERVER_config.snap_config.snapEnable == 0x3)&&(streamId==0)){
			gAVSERVER_config.snap_config.snapEnable = 0;

			if(gAVSERVER_config.snap_config.snapLocation==0) {
				strcpy(encPrm.snapFilename,"/mnt/mmc/ipnc/");
				strcat(encPrm.snapFilename, gAVSERVER_config.snap_config.snapName);
				strcat(encPrm.snapFilename, ".yuv");
				encPrm.snapEnable = 1;
			}
			else if(gAVSERVER_config.snap_config.snapLocation==2) {
				strcpy(encPrm.snapFilename,gAVSERVER_config.snap_config.snapName);
				strcat(encPrm.snapFilename, ".yuv");
				encPrm.snapEnable = 1;
			}
			else {
				OSA_printf("SNAP STORAGE FORMAT NOT SUPPORTED\n");
				encPrm.snapEnable = 0;
			}
		}
		else
			encPrm.snapEnable = 0;
		/***************************************/

        OSA_prfBegin(&gAVSERVER_ctrl.encodePrf[encodeId]);

        encStatus.bytesGenerated = 0;
				//next line changed by zeng
                status = ALG_vidEncRun(gVIDEO_ctrl.encodeStream[encodeId].algEncHndl, &encPrm, &encStatus, AVSERVER_SetOSDPixel);

        OSA_prfEnd(&gAVSERVER_ctrl.encodePrf[encodeId], 1);

        #ifdef AVSERVER_DEBUG_VIDEO_ENCODE_THR
          #ifdef AVSERVER_DEBUG_RUNNING
  		  	/* Profile print just to know only encode fps */
  		  	AVSERVER_profilePrint("ALGVIDENC ", &gAVSERVER_ctrl.encodePrf[encodeId]);
          #endif
        #endif

        if(status!=OSA_SOK) {
          OSA_ERROR("ALG_vidEncRun()\n");
          encStatus.bytesGenerated = 0;
           //OSA_printf("CODEC RUN FAIL STREAM ID: %d \n", encodeId);
       }

        memcpy(pOutBufHeader, pInBufHeader, sizeof(*pOutBufHeader));

        pOutBufHeader->encFrameSize = encStatus.bytesGenerated;
        pOutBufHeader->encFrameType = encStatus.isKeyFrame;
        pOutBufHeader->temporalId   = encStatus.temporalId;

        #ifdef AVSERVER_DEBUG_VIDEO_ENCODE_THR
          #ifdef AVSERVER_DEBUG_RUNNING
          static int cntfrm[3]={0,0,0};
          OSA_printf(" ENCODE FRAME %d: Stream %d Size %d bytes Keyframe %d\n", cntfrm[encodeId], encodeId, encStatus.bytesGenerated, encStatus.isKeyFrame);
          cntfrm[encodeId]++;
          #endif
        #endif

        /* check if motion enable and run only for first stream */

		if( ALG_vidEncGetMVdataStatus(gVIDEO_ctrl.encodeStream[encodeId].algEncHndl) )
		{
			MvBufInfo.size 			= ALG_vidEncGetMVdataSize(gVIDEO_ctrl.encodeStream[encodeId].algEncHndl);
			MvBufInfo.virtAddr 		= ALG_vidEncGetMVdataAddr(gVIDEO_ctrl.encodeStream[encodeId].algEncHndl);
			MvBufInfo.width 		= gAVSERVER_config.encodeConfig[encodeId].cropWidth;
			MvBufInfo.height 		= gAVSERVER_config.encodeConfig[encodeId].cropHeight;
			MvBufInfo.isKeyFrame	= encStatus.isKeyFrame;
			MvBufInfo.temporalId	= encStatus.temporalId;
			MvBufInfo.codecType		= gAVSERVER_config.encodeConfig[encodeId].codecType;
			if( MvBufInfo.size ) {
				VIDEO_motionCopyRun(&MvBufInfo);
			}
		}
      }

      AVSERVER_bufPutFull( VIDEO_TSK_ENCODE, encodeId, outBufId);
    }

   	AVSERVER_bufPutEmpty( VIDEO_TSK_ENCODE, streamId, inBufId);
  }

  return status;
}

int VIDEO_encodeTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status, streamId;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  OSA_setTskName("encode");
  
  #ifdef AVSERVER_DEBUG_VIDEO_ENCODE_THR
  OSA_printf(" ENCODE: Recevied CMD = 0x%04x\n", cmd);
  #endif

  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_ENCODE_THR
  OSA_printf(" ENCODE: Create...\n");
  #endif

  status = VIDEO_encodeTskCreate();

  OSA_tskAckOrFreeMsg(pMsg, status);

  if(status!=OSA_SOK) {
    OSA_ERROR("VIDEO_encodeTskCreate()\n");
    return status;
  }

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

        VIDEO_encodeTskRun(streamId);

        break;
      default:

        #ifdef AVSERVER_DEBUG_VIDEO_ENCODE_THR
        OSA_printf(" ENCODE: Unknown CMD = 0x%04x\n", cmd);
        #endif

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
    }
  }

  #ifdef AVSERVER_DEBUG_VIDEO_ENCODE_THR
  OSA_printf(" ENCODE: Delete...\n");
  #endif

  VIDEO_encodeTskDelete();

  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  #ifdef AVSERVER_DEBUG_VIDEO_ENCODE_THR
  OSA_printf(" ENCODE: Delete...DONE\n");
  #endif

  return OSA_SOK;
}

int VIDEO_encodeCreate()
{
  int status;

  status = OSA_tskCreate( &gVIDEO_ctrl.encodeTsk, VIDEO_encodeTskMain, VIDEO_ENCODE_THR_PRI, VIDEO_ENCODE_STACK_SIZE, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }

  return status;
}

int VIDEO_encodeDelete()
{
  int status;

  status = OSA_tskDelete( &gVIDEO_ctrl.encodeTsk );

  return status;
}
