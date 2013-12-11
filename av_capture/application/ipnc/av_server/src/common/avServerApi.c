
#include <avserver.h>
#include <drv_display.h>
#include <stream.h>

AVSERVER_Config gAVSERVER_config;
VIDEO_Ctrl      gVIDEO_ctrl;
AUDIO_Ctrl      gAUDIO_ctrl;
AUDIO_Ctrl  	gAUDIOPLAY_ctrl;
AVSERVER_Ctrl   gAVSERVER_ctrl;

int AVSERVER_lock()
{
  return OSA_mutexLock(&gAVSERVER_ctrl.lockMutex);
}

int AVSERVER_unlock()
{
  return OSA_mutexUnlock(&gAVSERVER_ctrl.lockMutex);
}

int AVSERVER_setMotion(int MotioneEnable, int MotioneCEnable, int MotioneCValue, int MotioneLevel, int block)
{
	if( MotioneEnable<0 || MotioneEnable > 1 )
	{
		OSA_printf("MotioneEnable = %d out of range!!!\n", MotioneEnable);
		MotioneEnable = 0;
	}

	AVSERVER_lock();

	gVIDEO_ctrl.motionStream.motionBlock 	= block;
	gVIDEO_ctrl.motionStream.motionLevel	= MotioneLevel;
	gVIDEO_ctrl.motionStream.motionCValue 	= MotioneCValue;
	gVIDEO_ctrl.motionStream.motionCEnable 	= MotioneCEnable;

	gVIDEO_ctrl.encodeStream[0].newMotionUpdate= 1;

	/* Set enable only for first stream */
	if((gAVSERVER_config.encodeConfig[0].codecType == ALG_VID_CODEC_MJPEG)||(block==0))
	{
		/* Mask if it is MegaPixel JPEG or no blocks Selected */
		gVIDEO_ctrl.encodeStream[0].newMotionStatus	= 0;
		gVIDEO_ctrl.motionStream.motionEnable 		= 0;
	}
	else {
		gVIDEO_ctrl.encodeStream[0].newMotionStatus	= MotioneEnable;
		gVIDEO_ctrl.motionStream.motionEnable 		= MotioneEnable;
	}

	/* Switch off motion detect for 2nd and 3rd streams */
	gVIDEO_ctrl.encodeStream[1].newMotionUpdate	= 1;
	gVIDEO_ctrl.encodeStream[1].newMotionStatus	= 0;
	gVIDEO_ctrl.encodeStream[2].newMotionUpdate	= 1;
	gVIDEO_ctrl.encodeStream[2].newMotionStatus	= 0;

	AVSERVER_unlock();

	return OSA_SOK;
}

int AVSERVER_setAACBitrate(int bitrate)
{
  gAVSERVER_config.audioConfig.bitRate 		= bitrate;
  gAVSERVER_config.audioConfig.audioUpdate	= TRUE;

  return OSA_SOK;
}

int AVSERVER_setEncBitrate(int streamId, int bitrate)
{
  if(streamId < 0 || streamId >= gAVSERVER_config.numEncodeStream)
    return OSA_EFAIL;

  AVSERVER_lock();

  if(gAVSERVER_config.encodeConfig[streamId].codecBitrate != bitrate) {
	gAVSERVER_config.encodeConfig[streamId].codecBitrate = bitrate;
  	gVIDEO_ctrl.encodeStream[streamId].newBitrate = bitrate;
  }

  AVSERVER_unlock();

  return OSA_SOK;
}
// next func add by zeng
int AVSERVER_setEncRcType(int streamId, int RcType)
{
  if(streamId < 0 || streamId >= gAVSERVER_config.numEncodeStream)
    return OSA_EFAIL;

  AVSERVER_lock();

  if(gAVSERVER_config.encodeConfig[streamId].rateControlType != RcType) {
	gAVSERVER_config.encodeConfig[streamId].rateControlType = RcType;
  	gVIDEO_ctrl.encodeStream[streamId].newRcUpdate = 1;
	gVIDEO_ctrl.encodeStream[streamId].newRcType = RcType;

  }

  AVSERVER_unlock();

  return OSA_SOK;
}



int AVSERVER_setEncFramerate(int streamId, unsigned int fps)
{
  int mask_fps = ((fps/1000)*AVSERVER_MAX_FRAMERATE)/gAVSERVER_config.sensorFps;

  if(mask_fps>AVSERVER_MAX_FRAMERATE)
  	mask_fps = AVSERVER_MAX_FRAMERATE;

  if(gAVSERVER_config.encodeConfig[streamId].frameRateBase != fps) {
	  gAVSERVER_config.encodeConfig[streamId].frameRateBase = fps;
	  gAVSERVER_config.encodeConfig[streamId].frameSkipMask = AVSERVER_getFrameSkipMask(mask_fps);
	  gVIDEO_ctrl.encodeStream[streamId].newFps = TRUE;
  }

  return OSA_SOK;
}

int AVSERVER_faceDetectEnable(Bool enable)
{
  gAVSERVER_config.faceDetectConfig.fdEnable 			= enable?TRUE:FALSE;
  gAVSERVER_config.faceDetectConfig.privacyMaskEnable	= FALSE;

  return OSA_SOK;
}

int AVSERVER_pMaskEnable(Bool enable)
{
  gAVSERVER_config.faceDetectConfig.fdEnable 			= enable?TRUE:FALSE;
  gAVSERVER_config.faceDetectConfig.privacyMaskEnable	= enable?TRUE:FALSE;

  return OSA_SOK;
}

int AVSERVER_faceRecogIdentifyEnable(Bool enable)
{
   gAVSERVER_config.faceDetectConfig.frIdentify 		= enable?TRUE:FALSE;
   gAVSERVER_config.faceDetectConfig.frRegUsr   		= FALSE;
   gAVSERVER_config.faceDetectConfig.frDelUsr			= FALSE;
   gAVSERVER_config.faceDetectConfig.fdEnable   		= enable?TRUE:FALSE;

   return OSA_SOK;
}

int AVSERVER_faceRegUsrEnable(Bool enable)
{
   gAVSERVER_config.faceDetectConfig.fdEnable   		= enable?TRUE:FALSE;
   gAVSERVER_config.faceDetectConfig.frRegUsr   		= enable?TRUE:FALSE;
   gAVSERVER_config.faceDetectConfig.frDelUsr			= FALSE;
   gAVSERVER_config.faceDetectConfig.frIdentify 		= FALSE;

   return OSA_SOK;
}

int AVSERVER_faceRegUsrDelete(Bool enable)
{
   gAVSERVER_config.faceDetectConfig.frRegUsr   		= FALSE;
   gAVSERVER_config.faceDetectConfig.frDelUsr			= enable?TRUE:FALSE;
   gAVSERVER_config.faceDetectConfig.frIdentify 		= FALSE;

   return OSA_SOK;
}

int AVSERVER_faceRecogClear(void)
{
   gAVSERVER_config.faceDetectConfig.frIdentify 		= FALSE;
   gAVSERVER_config.faceDetectConfig.frRegUsr   		= FALSE;
   gAVSERVER_config.faceDetectConfig.frDelUsr			= FALSE;

   return OSA_SOK;
}

int AVSERVER_faceTrackerEnable(Bool enable)
{
#if 0
  int count = 0, flag = FALSE;

  /* Switch off Face Tracker if MPEG4 or MJPEG */
  for(count = 0; count<gAVSERVER_config.numEncodeStream;count++) {
  	if(gAVSERVER_config.encodeConfig[count].codecType != ALG_VID_CODEC_H264)
  		flag = TRUE;
  }

  if(flag)
  	gAVSERVER_config.faceDetectConfig.fdTracker			= FALSE;
  else
  	gAVSERVER_config.faceDetectConfig.fdTracker			= enable?TRUE:FALSE;

#else
	/* switched off face tracker as more tuning needs to be done for the algorithm to have better tracking */
	/* will be addressed in next release */
  	gAVSERVER_config.faceDetectConfig.fdTracker			= enable?TRUE:FALSE;

#endif

  return OSA_SOK;
}

int AVSERVER_snapName(char *strText, int nLength)
{
	if(nLength >= MAX_STRING_LENGTH)
		return OSA_EFAIL;

	memcpy((char*)gAVSERVER_config.snap_config.snapName, (char*)strText, nLength);
	gAVSERVER_config.snap_config.snapName[nLength] = '\0';

	gAVSERVER_config.snap_config.snapEnable |= 0x1;

	return OSA_SOK;
}

int AVSERVER_snapLocation(int value)
{
	gAVSERVER_config.snap_config.snapLocation = value;
	gAVSERVER_config.snap_config.snapEnable |= 0x1<<1;

	return OSA_SOK;
}

int AVSERVER_fxdROIEnable(Bool enable)
{
  gAVSERVER_config.fxdROIEnable = enable;

  return OSA_SOK;
}

int AVSERVER_fdROIEnable(Bool enable)
{
  gAVSERVER_config.faceDetectConfig.fdROIEnable = enable;

  return OSA_SOK;
}

int AVSERVER_winmodeEnable(Bool enable)
{
  gAVSERVER_config.winmodeEnable = enable;

  return OSA_SOK;
}

int AVSERVER_ldcEnable(int streamId, Bool enable)
{
  if(streamId < 0 || streamId >= gAVSERVER_config.numCaptureStream)
    return OSA_EFAIL;

  gAVSERVER_config.captureConfig[streamId].ldcEnable = enable;

  return OSA_SOK;
}

int AVSERVER_snfEnable(int streamId, Bool enable)
{
  if(streamId < 0 || streamId >= gAVSERVER_config.numCaptureStream)
    return OSA_EFAIL;

  gAVSERVER_config.captureConfig[streamId].snfEnable = enable;

  return OSA_SOK;
}

int AVSERVER_tnfEnable(int streamId, Bool enable)
{
  if(streamId < 0 || streamId >= gAVSERVER_config.numCaptureStream)
    return OSA_EFAIL;

  gAVSERVER_config.captureConfig[streamId].tnfEnable = enable;

  return OSA_SOK;
}

int AVSERVER_fileSaveEnable(int streamId, Bool enable)
{
  if(streamId < 0 || streamId >= gAVSERVER_config.numEncodeStream)
    return OSA_EFAIL;

  gAVSERVER_config.encodeConfig[streamId].fileSaveEnable = enable;

  return OSA_SOK;
}

int AVSERVER_vsEnable(int streamId, Bool enable)
{
  if(streamId < 0 || streamId >= gAVSERVER_config.numCaptureStream)
    return OSA_EFAIL;

  gAVSERVER_config.captureConfig[streamId].vsEnable = enable;

  return OSA_SOK;
}

int AVSERVER_swosdEnable(int streamId, Bool enable)
{
	if(streamId < 0 || streamId >= gAVSERVER_config.numCaptureStream)
		return OSA_EFAIL;

	gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdEnable = enable;

	return OSA_SOK;

}

int AVSERVER_swosdEnableDateTimeStamp(int enable)
{
	int count = 0;
	for(count = 0; count<AVSERVER_MAX_STREAMS;count++)
	{
		if(enable > 0)
		{
			gAVSERVER_config.captureConfig[count].swosdConfig.swosdType = SWOSD_BASIC;
			gAVSERVER_config.captureConfig[count].swosdConfig.swosdTimeEnable = 1;
		}
		else
		{
			gAVSERVER_config.captureConfig[count].swosdConfig.swosdType = -1;
			gAVSERVER_config.captureConfig[count].swosdConfig.swosdTimeEnable = 0;
		}
	}
	return OSA_SOK;
}

int AVSERVER_swosdText(char *strText, int nLength)
{
	int count = 0;
	if(nLength >= MAX_STRING_LENGTH)
		return OSA_EFAIL;

	for(count = 0; count<AVSERVER_MAX_STREAMS; count++)
	{
		memcpy(gAVSERVER_config.captureConfig[count].swosdConfig.swosdDispString,
				strText, nLength);
		gAVSERVER_config.captureConfig[count].swosdConfig.swosdDispString[nLength] = '\0';
		gAVSERVER_config.captureConfig[count].swosdConfig.swosdLogoStringUpdate = OSD_YES;
	}
	return OSA_SOK;
}

int AVSERVER_swosdEnableText(int enable)
{
	int count = 0;
	for(count = 0; count<AVSERVER_MAX_STREAMS;count++)
	{
		if(enable)
		{
			gAVSERVER_config.captureConfig[count].swosdConfig.swosdTextEnable = 1;
		}
		else
		{
			gAVSERVER_config.captureConfig[count].swosdConfig.swosdTextEnable = 0;
		}
	}
	return OSA_SOK;
}

int AVSERVER_swosdEnableLogo(int enable)
{
	int count = 0;
	for(count = 0; count<AVSERVER_MAX_STREAMS;count++)
	{
		if(enable)
		{
			gAVSERVER_config.captureConfig[count].swosdConfig.swosdLogoEnable = 1;
		}
		else
		{
			gAVSERVER_config.captureConfig[count].swosdConfig.swosdLogoEnable = 0;
		}
	}
	return OSA_SOK;
}

int AVSERVER_histEnable(int enable)
{
	int count = 0;

	for(count = 0; count<AVSERVER_MAX_STREAMS;count++)
		gAVSERVER_config.captureConfig[count].swosdConfig.swosdHistEnable = enable;

	return OSA_SOK;
}

int AVSERVER_saldreEnable(int enable)
{
	gAVSERVER_config.aewb_config.saldreEnable = enable;

	return OSA_SOK;
}

int AVSERVER_saldreMode(int mode)
{
	gAVSERVER_config.aewb_config.saldreMode = mode;

	return OSA_SOK;
}

int AVSERVER_saldreLevel(int level)
{
	gAVSERVER_config.aewb_config.saldreLevel = level;

	return OSA_SOK;
}

int SetColPatConfig(const int mirrValue)
{
    static CSL_IpipeColPatConfig ColPatConfig;

    if (mirrValue == 3)
    {
        ColPatConfig.colPat[0][0] = CSL_IPIPE_SOURCE_COLOUR_GB;
        ColPatConfig.colPat[0][1] = CSL_IPIPE_SOURCE_COLOUR_B;
        ColPatConfig.colPat[1][0] = CSL_IPIPE_SOURCE_COLOUR_R;
        ColPatConfig.colPat[1][1] = CSL_IPIPE_SOURCE_COLOUR_GR;
    }
    else if (mirrValue == 2)
    {
        ColPatConfig.colPat[0][0] = CSL_IPIPE_SOURCE_COLOUR_R;
        ColPatConfig.colPat[0][1] = CSL_IPIPE_SOURCE_COLOUR_GR;
        ColPatConfig.colPat[1][0] = CSL_IPIPE_SOURCE_COLOUR_GB;
        ColPatConfig.colPat[1][1] = CSL_IPIPE_SOURCE_COLOUR_B;
    }
    else if (mirrValue == 1)
    {
        ColPatConfig.colPat[0][0] = CSL_IPIPE_SOURCE_COLOUR_B;
        ColPatConfig.colPat[0][1] = CSL_IPIPE_SOURCE_COLOUR_GB;
        ColPatConfig.colPat[1][0] = CSL_IPIPE_SOURCE_COLOUR_GR;
        ColPatConfig.colPat[1][1] = CSL_IPIPE_SOURCE_COLOUR_R;
    }
    else
    {
        ColPatConfig.colPat[0][0] = CSL_IPIPE_SOURCE_COLOUR_GR;
        ColPatConfig.colPat[0][1] = CSL_IPIPE_SOURCE_COLOUR_R;
        ColPatConfig.colPat[1][0] = CSL_IPIPE_SOURCE_COLOUR_B;
        ColPatConfig.colPat[1][1] = CSL_IPIPE_SOURCE_COLOUR_GB;
    }

    int status = CSL_ipipeSetColPatConfig(&gCSL_ipipeHndl, &ColPatConfig);

    return status;
}

int SetMirrValue(const int mirrValue)
{
    int i;
    int flipH, flipV;
    int status = OSA_EFAIL;
    static int mirrValuebak = -1;

    if (mirrValuebak != mirrValue)
    {
        mirrValuebak = mirrValue;
        if (mirrValue == 3)
        {
            flipH = TRUE;
            flipV = TRUE;
        }
        else if (mirrValue == 2)
        {
            flipH = TRUE;
            flipV = FALSE;
        }
        else if (mirrValue == 1)
        {
            flipH = FALSE;
            flipV = TRUE;
        }
        else
        {
            flipH = FALSE;
            flipV = FALSE;
        }

        for (i = 0; i < AVSERVER_MAX_STREAMS; i++)
        {
            gAVSERVER_config.captureConfig[i].flipH      = flipH;
            gAVSERVER_config.captureConfig[i].flipV      = flipV;
            gAVSERVER_config.captureConfig[i].mirrUpdate = TRUE;
        }

        eSensorType sensorId = GetSensorId();
        switch (sensorId)
        {
        case AR0130:
        case MT9M034:
        case AR0330:
        case MT9P031:
            for (i = 0; i < AVSERVER_MAX_STREAMS; i++)
            {
                gAVSERVER_config.captureConfig[i].flipH      = FALSE;
                gAVSERVER_config.captureConfig[i].flipV      = FALSE;
                gAVSERVER_config.captureConfig[i].mirrUpdate = TRUE;
            }
            status = drvImgsFunc->imgsSetMirror(flipH, flipV);
            if (status == 0 && sensorId == MT9P031)
            {
                status = SetColPatConfig(mirrValue);
                printf("SetColPatConfig mirrValue = %d\n", mirrValue);
            }
            return status;
        case OV9712:
        case AR0331:
        case IMX122:
        default:
            break;
        }
    }

    return 0;
}

int AVSERVER_setMirrValue(int value)
{
#if 0
	int i, flipH, flipV;

	if(value == 3) {
		flipH = TRUE; flipV = TRUE;
	}
	else if(value == 2) {
		flipH = FALSE; flipV = TRUE;
	}
	else if(value == 1) {
		flipH = TRUE; flipV = FALSE;
	}
	else {
		flipH = FALSE; flipV = FALSE;
	}

  	for(i=0;i<AVSERVER_MAX_STREAMS;i++) {
		gAVSERVER_config.captureConfig[i].flipH 		= FALSE;//flipH;
		gAVSERVER_config.captureConfig[i].flipV 		= FALSE;//flipV;
		gAVSERVER_config.captureConfig[i].mirrUpdate 	= TRUE;
	}
#else

    SetMirrValue(value);

#endif

	return OSA_SOK;
}

int AVSERVER_aewbEnable(Bool enable)
{
  gAVSERVER_config.aewb_config.aewbEnable = enable;

  return 0;
}

int AVSERVER_aewbSetType(int type)
{
  gAVSERVER_config.aewb_config.aewbType 	= type;
  gAVSERVER_config.aewb_config.aewbEnable 	= (type==0)?FALSE:TRUE;

  return 0;
}

int AVSERVER_aewbPriority(int value)
{
  gAVSERVER_config.aewb_config.aewbPriority = value;

  return 0;
}

int AVSERVER_afEnable(Bool enable)
{
  gAVSERVER_config.aewb_config.afEnable = enable;

  return 0;
}

int AVSERVER_audioAlarmLevel(int value)
{
	gAVSERVER_config.audioConfig.alarmLevel = value;

	return OSA_SOK;
}

int AVSERVER_audioAlarmEnable(int enable)
{
	gAVSERVER_config.audioConfig.audioAlarm = enable;

	return OSA_SOK;
}

int AVSERVER_SetDisplay(int type)
{
	DRV_DisplayWinConfig  displayConfig;

	gAVSERVER_config.displayEnable = (type>0)?1:0;

	displayConfig.winId       = DRV_DISPLAY_VID_WIN_0;
	displayConfig.numBuf      = gVIDEO_ctrl.displayStream.bufDisplayInCreatePrm.numBuf;
	displayConfig.dataFormat  = gAVSERVER_config.captureYuvFormat;
	displayConfig.startX      = 0;
	displayConfig.startY      = 0;
	displayConfig.width       = gAVSERVER_config.displayConfig.width;
	displayConfig.height      = gAVSERVER_config.displayConfig.height;
	displayConfig.offsetH     = OSA_align(displayConfig.width, 32);
	displayConfig.offsetV     = displayConfig.height;
	displayConfig.zoomH       = 0;
	displayConfig.zoomV       = 0;
	displayConfig.expandH 	  = gAVSERVER_config.displayConfig.expandH;
	displayConfig.expandV 	  = gAVSERVER_config.displayConfig.expandV;

	if( type == 2 /*PAL*/ ) {
		DRV_displaySetMode(DRV_DISPLAY_MODE_PAL);
		displayConfig.expandV = 1;
		gAVSERVER_config.displayEnable = 1;
	}
	else if( type == 1 /*NTSC*/ ) {
		DRV_displaySetMode(DRV_DISPLAY_MODE_NTSC);
		displayConfig.expandV = 0;
		gAVSERVER_config.displayEnable = 1;
	}
	else {
		gAVSERVER_config.displayEnable = 0;
	}

	DRV_displayExpandEx(&gVIDEO_ctrl.displayStream.displayHndl, &displayConfig);

    return 0;
}

int AVSERVER_getCaptureFrameRate(int encodeId, float *fr)
{
   //*fr = (float)(gAVSERVER_ctrl.capturePrf.count*1000)/gAVSERVER_ctrl.capturePrf.totalTime;
   *fr = (float)gAVSERVER_config.encodeConfig[encodeId].frameRateBase/1000;
    return 0;
}


void AVSERVER_profilePrint(char *name, OSA_PrfHndl *hndl)
{
  #ifdef OSA_PRF_ENABLE
  if(hndl->count==0 || hndl->totalTime<10)
    return;

  OSA_printf(" %s | %14.2f | %10.2f | %10d | %12d |\n",
    name,
    (float)hndl->totalTime/hndl->count,
    (float)(hndl->count*1000)/hndl->totalTime,
    hndl->totalTime,
    hndl->count
    );
  hndl->totalTime = 0;
  hndl->count = 0;

  #endif
}

int AVSERVER_profileInfoShow()
{
  OSA_printf("\n");
  OSA_printf(" Module   | Avg Time/Frame | Frame-rate | Total time | Total Frames |\n")
  AVSERVER_profilePrint("CAPTURE ", &gAVSERVER_ctrl.capturePrf);
  AVSERVER_profilePrint("IPIPE   ", &gAVSERVER_ctrl.ipipePrf);
  AVSERVER_profilePrint("RESIZE  ", &gAVSERVER_ctrl.resizePrf);
  AVSERVER_profilePrint("LDC0    ", &gAVSERVER_ctrl.ldcPrf[0]);
  AVSERVER_profilePrint("LDC1    ", &gAVSERVER_ctrl.ldcPrf[1]);
  AVSERVER_profilePrint("LDC2    ", &gAVSERVER_ctrl.ldcPrf[2]);
  AVSERVER_profilePrint("LDC3    ", &gAVSERVER_ctrl.ldcPrf[3]);
  AVSERVER_profilePrint("VNF0    ", &gAVSERVER_ctrl.vnfPrf[0]);
  AVSERVER_profilePrint("VNF1    ", &gAVSERVER_ctrl.vnfPrf[1]);
  AVSERVER_profilePrint("VNF2    ", &gAVSERVER_ctrl.vnfPrf[2]);
  AVSERVER_profilePrint("VNF3    ", &gAVSERVER_ctrl.vnfPrf[3]);
  AVSERVER_profilePrint("ENCODE0 ", &gAVSERVER_ctrl.encodePrf[0]);
  AVSERVER_profilePrint("ENCODE1 ", &gAVSERVER_ctrl.encodePrf[1]);
  AVSERVER_profilePrint("ENCODE2 ", &gAVSERVER_ctrl.encodePrf[2]);
  AVSERVER_profilePrint("ENCODE3 ", &gAVSERVER_ctrl.encodePrf[3]);
  AVSERVER_profilePrint("STREAM  ", &gAVSERVER_ctrl.streamPrf);
  AVSERVER_profilePrint("MISC0   ", &gAVSERVER_ctrl.miscPrf[0]);
  AVSERVER_profilePrint("MISC1   ", &gAVSERVER_ctrl.miscPrf[1]);
  AVSERVER_profilePrint("MISC2   ", &gAVSERVER_ctrl.miscPrf[2]);
  AVSERVER_profilePrint("MISC3   ", &gAVSERVER_ctrl.miscPrf[3]);
  AVSERVER_profilePrint("OSD0    ", &gAVSERVER_ctrl.swosdPrf[0]);
  AVSERVER_profilePrint("OSD1    ", &gAVSERVER_ctrl.swosdPrf[1]);
  AVSERVER_profilePrint("OSD2    ", &gAVSERVER_ctrl.swosdPrf[2]);
  AVSERVER_profilePrint("OSD3    ", &gAVSERVER_ctrl.swosdPrf[3]);
  AVSERVER_profilePrint("VSTAB   ", &gAVSERVER_ctrl.vsPrf);
  AVSERVER_profilePrint("FACE.D  ", &gAVSERVER_ctrl.fdPrf);
  AVSERVER_profilePrint("FACE.T  ", &gAVSERVER_ctrl.fdtrkPrf);
  AVSERVER_profilePrint("AEWB    ", &gAVSERVER_ctrl.aewbPrf);
  AVSERVER_profilePrint("AUDIO   ", &gAVSERVER_ctrl.audioPrf);
  AVSERVER_profilePrint("MOTION  ", &gAVSERVER_ctrl.motionPrf);
  AVSERVER_profilePrint("SALDRE  ", &gAVSERVER_ctrl.saldrePrf);
  OSA_printf("\n");

  return OSA_SOK;
}

int AVSERVER_init()
{
  int status;

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Creating TSKs.\n");
  #endif

  status = DRV_init();
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_init()\n");
    return status;
  }

  status = ALG_sysInit();
  if(status!=OSA_SOK) {
    OSA_ERROR("ALG_sysInit()\n");
    DRV_exit();
    return status;
  }

  memset(&gAVSERVER_config, 0, sizeof(gAVSERVER_config));
  memset(&gVIDEO_ctrl, 0, sizeof(gVIDEO_ctrl));
  memset(&gAVSERVER_ctrl, 0, sizeof(gAVSERVER_ctrl));

  status |= OSA_mutexCreate(&gAVSERVER_ctrl.lockMutex);

  status |= VIDEO_captureCreate();
  //status |= VIDEO_resizeCreate();
  status |= VIDEO_streamCreate();
  status |= VIDEO_displayCreate();
  //status |= VIDEO_ldcCreate();
  //status |= VIDEO_vnfCreate();
  //status |= VIDEO_vsCreate();
  status |= VIDEO_aewbCreate();
  //status |= VIDEO_fdCreate();
  status |= VIDEO_encodeCreate();
  status |= VIDEO_swosdCreate();
  //status |= VIDEO_encryptCreate();
  status |= VIDEO_motionCreate();
  //status |= AUDIO_audioCreate();
  //status |= AUDIOPLAY_audioCreate();
  status |= AVSERVER_mainCreate();

  if(status!=OSA_SOK) {
    OSA_ERROR("AVSERVER_init()\n");
    ALG_sysExit();
    DRV_exit();
    return status;
  }

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Creating TSKs...DONE\n");
  #endif

  return status;
}

int AVSERVER_exit()
{
  int status = OSA_SOK;

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Deleting TSKs.\n");
  #endif

  status |= VIDEO_captureDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Resize Delete ...\n");
  #endif

  status |= VIDEO_resizeDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: LDC Delete ...\n");
  #endif

  status |= VIDEO_ldcDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: VNF Delete ...\n");
  #endif

  status |= VIDEO_vnfDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: VS Delete ...\n");
  #endif

  status |= VIDEO_vsDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: AEWB Delete ...\n");
  #endif

  status |= VIDEO_aewbDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: FD Delete ...\n");
  #endif

  status |= VIDEO_fdDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Display Delete ...\n");
  #endif

  status |= VIDEO_displayDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Encode Delete ...\n");
  #endif

  status |= VIDEO_encodeDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: SWOSD Delete ...\n");
  #endif

  status |= VIDEO_swosdDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Encrypt Delete ...\n");
  #endif

  status |= VIDEO_encryptDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Motion Delete ...\n");
  #endif

  status |= VIDEO_motionDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Stream Delete ...\n");
  #endif

  status |= VIDEO_streamDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Audio Delete ...\n");
  #endif

//  status |= AUDIO_audioDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Audio Play Delete ...\n");
  #endif

//  status |= AUDIOPLAY_audioDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Main Delete ...\n");
  #endif

  status |= AVSERVER_mainDelete();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: ALG Exit...\n");
  #endif

  status |= ALG_sysExit();

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: DRV Exit...\n");
  #endif

  status |= DRV_exit();

  if(status!=OSA_SOK) {
    OSA_ERROR("AVSERVER_exit()\n");
  }

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Deleting TSKs...DONE\n");
  #endif

  return status;
}

int AVSERVER_start(AVSERVER_Config *config)
{
  int status;

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Sending START.\n");
  #endif

  memcpy(&gAVSERVER_config, config, sizeof(gAVSERVER_config));

#if 0 //ITT shuld set from here - revisit
  if(  gAVSERVER_config.captureConfig[0].snfEnable
		 || gAVSERVER_config.captureConfig[0].tnfEnable){

		 OSA_printf("\n NF enable status for ITT Paramset Boot... %d \n", 2);
	 }else{
	   OSA_printf("\n NF enable status for ITT Paramset Boot... %d \n", 1);
	 }

   // Get NF enble status & pass as paramSetId - ITT - Revisit
   OSA_printf("\nNF enable status for ITT PS- boot... %d \n", 2);
   status = DRV_imageTuneLoadParamSet(0, 2);  //ANR
#endif


  status = OSA_mbxSendMsg(&gAVSERVER_ctrl.mainTsk.mbxHndl, &gAVSERVER_ctrl.uiMbx, AVSERVER_MAIN_CMD_START, NULL, OSA_MBX_WAIT_ACK);

  if(status!=OSA_SOK) {
    OSA_ERROR("AVSERVER_start()\n");
  }

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Sending START...DONE\n");
  #endif

  return status;
}

int AVSERVER_stop()
{
  int status;

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Sending STOP.\n");
  #endif

  status = OSA_mbxSendMsg(&gAVSERVER_ctrl.mainTsk.mbxHndl, &gAVSERVER_ctrl.uiMbx, AVSERVER_MAIN_CMD_STOP, NULL, OSA_MBX_WAIT_ACK);

  if(status!=OSA_SOK) {
    OSA_ERROR("AVSERVER_stop()\n");
  }

  #ifdef AVSERVER_DEBUG_API
  OSA_printf(" AVSERVER API: Sending STOP...DONE\n");
  #endif

  return status;
}


