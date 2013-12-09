
#include <avserver.h>
#include <osa_cmem.h>
#include <sys/prctl.h>


//////zeng add 2012-06-28
OSDBASE			g_OSDBase = {TRUE, TRUE};
OSDPARAM11		g_OSD11 = {0};
OSDPARAM16		g_OSD16 = {0};
OSDPARAM32		g_OSD32 = {0};
MASK_CFG		g_MaskCfg = {0};

//MASKAREAS		MaskAreas = {{{0,0,50,50}, {50,50,100,100}, {60,0,70,20}, {0,0,0,0}}};

static int AVSERVER_OSDStart();
static void* OSD_ThreadOSD(void *param);


Uint32 FrameSkipMask[AVSERVER_MAX_FRAMERATE+1] =
{
	0x00000000,
	0x00000001,0x20004000,0x20080200,0x20404040,0x2082082,
	0x21084210,0x01111111,0x11111111,0x24892488,0x24924924,
	0x24924949,0x2949494A,0x2949A94A,0x29A9A94A,0x2AAAAAAA,
	0x2AAAEAAA,0x2EAAEAAA,0x2B5AD6B5,0x35AD6B5B,0x1B6DB6DB,
	0x1BDDB6DB,0x1BDDBDDB,0x3BDDBDDB,0x1EF7BDEF,0x1F7DF7DF,
	0x1F7FF7DF,0x1F7FF7FF,0x1FFFF7FF,0x1FFFFFFF,0x3FFFFFFF
};

Uint32 AVSERVER_getFrameSkipMask(int fps)
{

  if(fps>30)
  {
  	fps = 30;
  }

  return FrameSkipMask[fps];

}

int AVSERVER_bufAlloc()
{
  int status=OSA_SOK;
  OSA_BufHndl *pBufHndl;
  OSA_BufInfo *pBufInfo;
  OSA_BufCreate *pBufCreatePrm;
  int bufId, i, k;
  VIDEO_EncodeStream *pEncodeStream;
  VIDEO_CaptureStream *pCaptureStream;
  Uint32 maxbufSize = 0;
  Uint32 bufSize;

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Allocating buffers ...\n");
  #endif

  for(i=0; i<gAVSERVER_config.numCaptureStream; i++) {

    pCaptureStream = &gVIDEO_ctrl.captureStream[i];

    bufSize = pCaptureStream->captureOutOffsetH * pCaptureStream->captureOutOffsetV;

    if(gAVSERVER_config.captureYuvFormat == DRV_DATA_FORMAT_YUV420)
      bufSize += bufSize/2;
    else
      bufSize += bufSize;

    bufSize += VIDEO_BUF_HEADER_SIZE;

    pBufHndl      = &pCaptureStream->bufLdcIn;
    pBufCreatePrm = &pCaptureStream->bufLdcInCreatePrm;

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Stream %d: Allocating LDC buffers: %d of size %d bytes\n", i, pBufCreatePrm->numBuf, bufSize);
    #endif

    if(   gAVSERVER_config.captureRawInMode==AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN
      &&  pCaptureStream->captureNextTsk == VIDEO_TSK_LDC
      ) {

      for(k=0; k<pBufCreatePrm->numBuf; k++) {

        pBufInfo = DRV_ipipeGetRszBufInfo(i, k);

        pBufCreatePrm->bufVirtAddr[k] = pBufInfo->virtAddr;
        pBufCreatePrm->bufPhysAddr[k] = pBufInfo->physAddr;
      }

    } else {

      for(k=0; k<pBufCreatePrm->numBuf; k++) {
        pBufCreatePrm->bufVirtAddr[k] = OSA_cmemAlloc(bufSize, 32);
        pBufCreatePrm->bufPhysAddr[k] = OSA_cmemGetPhysAddr(pBufCreatePrm->bufVirtAddr[k]);

        if(pBufCreatePrm->bufVirtAddr[k]==NULL||pBufCreatePrm->bufPhysAddr[k]==NULL) {
          OSA_ERROR("OSA_cmemAlloc()\n");
          return OSA_EFAIL;
        }
      }
    }

    if(pBufCreatePrm->numBuf)
      status |= OSA_bufCreate(pBufHndl, pBufCreatePrm);

    pBufHndl      = &pCaptureStream->bufVnfIn;
    pBufCreatePrm = &pCaptureStream->bufVnfInCreatePrm;

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Stream %d: Allocating VNF buffers: %d of size %d bytes\n", i, pBufCreatePrm->numBuf, bufSize);
    #endif

    if(   gAVSERVER_config.captureRawInMode==AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN
      &&  pCaptureStream->captureNextTsk == VIDEO_TSK_VNF
      ) {

      for(k=0; k<pBufCreatePrm->numBuf; k++) {

        pBufInfo = DRV_ipipeGetRszBufInfo(i, k);

        pBufCreatePrm->bufVirtAddr[k] = pBufInfo->virtAddr;
        pBufCreatePrm->bufPhysAddr[k] = pBufInfo->physAddr;
      }

    } else {

      for(k=0; k<pBufCreatePrm->numBuf; k++) {
        pBufCreatePrm->bufVirtAddr[k] = OSA_cmemAlloc(bufSize, 32);
        pBufCreatePrm->bufPhysAddr[k] = OSA_cmemGetPhysAddr(pBufCreatePrm->bufVirtAddr[k]);

        if(pBufCreatePrm->bufVirtAddr[k]==NULL||pBufCreatePrm->bufPhysAddr[k]==NULL) {
          OSA_ERROR("OSA_cmemAlloc()\n");
          return OSA_EFAIL;
        }
      }
    }
    if(pBufCreatePrm->numBuf)
      status |= OSA_bufCreate(pBufHndl, pBufCreatePrm);

    pBufHndl      = &pCaptureStream->bufEncodeIn;
    pBufCreatePrm = &pCaptureStream->bufEncodeInCreatePrm;

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Stream %d: Allocating Encode IN buffers: %d of size %d bytes\n", i, pBufCreatePrm->numBuf, bufSize);
    #endif

    if(   gAVSERVER_config.captureRawInMode==AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN
      &&  pCaptureStream->captureNextTsk == VIDEO_TSK_ENCODE
      ) {

      for(k=0; k<pBufCreatePrm->numBuf; k++) {

        pBufInfo = DRV_ipipeGetRszBufInfo(i, k);

        pBufCreatePrm->bufVirtAddr[k] = pBufInfo->virtAddr;
        pBufCreatePrm->bufPhysAddr[k] = pBufInfo->physAddr;
      }

    } else {

      for(k=0; k<pBufCreatePrm->numBuf; k++) {
        pBufCreatePrm->bufVirtAddr[k] = OSA_cmemAlloc(bufSize, 32);
        pBufCreatePrm->bufPhysAddr[k] = OSA_cmemGetPhysAddr(pBufCreatePrm->bufVirtAddr[k]);

        if(pBufCreatePrm->bufVirtAddr[k]==NULL||pBufCreatePrm->bufPhysAddr[k]==NULL) {
          OSA_ERROR("OSA_cmemAlloc()\n");
          return OSA_EFAIL;
        }
      }
    }
    if(pBufCreatePrm->numBuf)
      status |= OSA_bufCreate(pBufHndl, pBufCreatePrm);

    pBufHndl      = &pCaptureStream->bufResizeIn;
    pBufCreatePrm = &pCaptureStream->bufResizeInCreatePrm;

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Stream %d: Assigning Resize IN buffers: %d of size %d bytes\n", i, pBufCreatePrm->numBuf, bufSize);
    #endif

    for(k=0; k<pBufCreatePrm->numBuf; k++) {
      pBufCreatePrm->bufVirtAddr[k] = pCaptureStream->bufEncodeInCreatePrm.bufVirtAddr[k];
      pBufCreatePrm->bufPhysAddr[k] = pCaptureStream->bufEncodeInCreatePrm.bufPhysAddr[k];
    }

    if(pBufCreatePrm->numBuf) {
      status |= OSA_bufCreate(pBufHndl, pBufCreatePrm);
    }

    if(pCaptureStream->bufResizeIn.numBuf) {

      #ifdef AVSERVER_DEBUG_MAIN_THR
      OSA_printf(" AVSERVER MAIN: Stream %d: Getting Encode IN buffers\n", i);
      #endif

      while( OSA_bufGetEmpty(&pCaptureStream->bufEncodeIn, &bufId, OSA_TIMEOUT_NONE) == OSA_SOK) {
        #ifdef AVSERVER_DEBUG_MAIN_THR
        OSA_printf(" AVSERVER MAIN: Stream %d: Encode IN buffers %d\n", i, bufId);
        #endif
      }
    }

    if(gAVSERVER_config.captureRawInMode==AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN)
    {
      #ifdef AVSERVER_DEBUG_MAIN_THR
      OSA_printf(" AVSERVER MAIN: Stream %d: Getting Capture OUT buffers\n", i);
      #endif

      while( AVSERVER_bufGetEmpty( VIDEO_TSK_CAPTURE, i, &bufId, OSA_TIMEOUT_NONE) != NULL) {
        #ifdef AVSERVER_DEBUG_MAIN_THR
        OSA_printf(" AVSERVER MAIN: Stream %d: Capture OUT buffers %d\n", i, bufId);
        #endif
      }
    }
  }

#if 0
  bufSize = OSA_align(gVIDEO_ctrl.faceDetectStream.fdOffsetH, 32)*gVIDEO_ctrl.faceDetectStream.fdHeight*2;
  bufSize += VIDEO_BUF_HEADER_SIZE;

  pBufHndl      = &gVIDEO_ctrl.faceDetectStream.bufFdIn;
  pBufCreatePrm = &gVIDEO_ctrl.faceDetectStream.bufFdInCreatePrm;

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Allocating FD buffers: %d of size %d bytes\n", pBufCreatePrm->numBuf, bufSize);
  #endif

  for(k=0; k<pBufCreatePrm->numBuf; k++) {
    pBufCreatePrm->bufVirtAddr[k] = OSA_cmemAlloc(bufSize, 32);
    pBufCreatePrm->bufPhysAddr[k] = OSA_cmemGetPhysAddr(pBufCreatePrm->bufVirtAddr[k]);

    if(pBufCreatePrm->bufVirtAddr[k]==NULL||pBufCreatePrm->bufPhysAddr[k]==NULL) {
      OSA_ERROR("OSA_cmemAlloc()\n");
      return OSA_EFAIL;
    }
  }

  if(pBufCreatePrm->numBuf)
    status |= OSA_bufCreate(pBufHndl, pBufCreatePrm);
#endif

  pBufHndl      = &gVIDEO_ctrl.displayStream.bufDisplayIn;
  pBufCreatePrm = &gVIDEO_ctrl.displayStream.bufDisplayInCreatePrm;

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Assigning Display buffers: %d of size %d bytes\n", pBufCreatePrm->numBuf, bufSize);
  #endif

  for(k=0; k<pBufCreatePrm->numBuf; k++) {
    pBufCreatePrm->bufVirtAddr[k] = gVIDEO_ctrl.displayStream.displayInfo.virtAddr[k];
    pBufCreatePrm->bufPhysAddr[k] = gVIDEO_ctrl.displayStream.displayInfo.physAddr[k];
  }

  if(pBufCreatePrm->numBuf)
    status |= OSA_bufCreate(pBufHndl, pBufCreatePrm);

  for(i=0; i<gAVSERVER_config.numEncodeStream; i++) {
    pEncodeStream = &gVIDEO_ctrl.encodeStream[i];

    bufSize = gAVSERVER_config.encodeConfig[i].cropWidth*gAVSERVER_config.encodeConfig[i].cropHeight;

    switch(gAVSERVER_config.encodeConfig[i].codecType) {
      default:
      case ALG_VID_CODEC_MJPEG:
        bufSize /= 1;
        break;

      case ALG_VID_CODEC_H264:
      case ALG_VID_CODEC_MPEG4:
        bufSize /= 2;
        break;
    }

    bufSize += VIDEO_BUF_HEADER_SIZE;

    bufSize = OSA_align(bufSize, KB);

    pBufHndl      = &pEncodeStream->bufEncryptIn;
    pBufCreatePrm = &pEncodeStream->bufEncryptInCreatePrm;

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Stream %d: Allocating Encrypt buffers: %d of size %d bytes\n", i, pBufCreatePrm->numBuf, bufSize);
    #endif

    for(k=0; k<pBufCreatePrm->numBuf; k++) {
      pBufCreatePrm->bufVirtAddr[k] = OSA_cmemAlloc(bufSize, 32);
      pBufCreatePrm->bufPhysAddr[k] = OSA_cmemGetPhysAddr(pBufCreatePrm->bufVirtAddr[k]);

      if(pBufCreatePrm->bufVirtAddr[k]==NULL||pBufCreatePrm->bufPhysAddr[k]==NULL) {
        OSA_ERROR("OSA_cmemAlloc()\n");
        return OSA_EFAIL;
      }
    }

    if(pBufCreatePrm->numBuf)
      status |= OSA_bufCreate(pBufHndl, pBufCreatePrm);

    pBufHndl      = &pEncodeStream->bufStreamIn;
    pBufCreatePrm = &pEncodeStream->bufStreamInCreatePrm;

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Stream %d: Allocating Stream buffers: %d of size %d bytes\n", i, pBufCreatePrm->numBuf, bufSize);
    #endif

    for(k=0; k<pBufCreatePrm->numBuf; k++) {
      pBufCreatePrm->bufVirtAddr[k] = OSA_cmemAlloc(bufSize, 32);
      pBufCreatePrm->bufPhysAddr[k] = OSA_cmemGetPhysAddr(pBufCreatePrm->bufVirtAddr[k]);

      if(pBufCreatePrm->bufVirtAddr[k]==NULL||pBufCreatePrm->bufPhysAddr[k]==NULL) {
        OSA_ERROR("OSA_cmemAlloc()\n");
        return OSA_EFAIL;
      }
    }

    if(pBufCreatePrm->numBuf)
      status |= OSA_bufCreate(pBufHndl, pBufCreatePrm);
  }

	maxbufSize = 32;
	for(i=0; i<gAVSERVER_config.numEncodeStream; i++)
	{
		pEncodeStream = &gVIDEO_ctrl.encodeStream[i];
		bufSize = ALG_vidEncGetMVdataSize( pEncodeStream->algEncHndl );
		if( maxbufSize < bufSize )
		{
			maxbufSize = bufSize;
		}
	}
	gVIDEO_ctrl.motionStream.maxbufsize = maxbufSize;
	pBufHndl      = &gVIDEO_ctrl.motionStream.bufMotionIn;
    pBufCreatePrm = &gVIDEO_ctrl.motionStream.bufMotionInCreatePrm;

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Allocating MotionDetect buffers: %d of size %d bytes\n", pBufCreatePrm->numBuf, bufSize);
    #endif

	for(k=0; k<pBufCreatePrm->numBuf; k++)
	{
    pBufCreatePrm->bufVirtAddr[k] = OSA_cmemAlloc(maxbufSize, 32);
    pBufCreatePrm->bufPhysAddr[k] = OSA_cmemGetPhysAddr(pBufCreatePrm->bufVirtAddr[k]);

    if(pBufCreatePrm->bufVirtAddr[k]==NULL)
    {
      OSA_ERROR("OSA_cmemAlloc()\n");
      return OSA_EFAIL;
    }
  }
  if(pBufCreatePrm->numBuf)
  	status |= OSA_bufCreate(pBufHndl, pBufCreatePrm);

  if(status!=OSA_SOK)
    OSA_ERROR("\n");

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Allocating buffers ...DONE\n");
  #endif

  return status;
}

int AVSERVER_bufFree()
{
  int i, k, status=OSA_SOK;
  VIDEO_EncodeStream *pEncodeStream;
  VIDEO_CaptureStream *pCaptureStream;

  for(i=0; i<gAVSERVER_config.numCaptureStream; i++) {

    pCaptureStream = &gVIDEO_ctrl.captureStream[i];

    if(   gAVSERVER_config.captureRawInMode==AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN
      &&  pCaptureStream->captureNextTsk == VIDEO_TSK_LDC
      ) {

    } else {
      for(k=0; k<pCaptureStream->bufLdcInCreatePrm.numBuf; k++)
        OSA_cmemFree(pCaptureStream->bufLdcInCreatePrm.bufVirtAddr[k]);
    }

    if(   gAVSERVER_config.captureRawInMode==AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN
      &&  pCaptureStream->captureNextTsk == VIDEO_TSK_VNF
      ) {

    } else {
      for(k=0; k<pCaptureStream->bufVnfInCreatePrm.numBuf; k++)
        OSA_cmemFree(pCaptureStream->bufVnfInCreatePrm.bufVirtAddr[k]);
    }

    if(   gAVSERVER_config.captureRawInMode==AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN
      &&  pCaptureStream->captureNextTsk == VIDEO_TSK_ENCODE
      ) {

    } else {
      for(k=0; k<pCaptureStream->bufEncodeInCreatePrm.numBuf; k++)
        OSA_cmemFree(pCaptureStream->bufEncodeInCreatePrm.bufVirtAddr[k]);
    }

    if(pCaptureStream->bufLdcInCreatePrm.numBuf)
      OSA_bufDelete(&pCaptureStream->bufLdcIn);

    if(pCaptureStream->bufVnfInCreatePrm.numBuf)
      OSA_bufDelete(&pCaptureStream->bufVnfIn);

    if(pCaptureStream->bufResizeInCreatePrm.numBuf)
      OSA_bufDelete(&pCaptureStream->bufResizeIn);

    if(pCaptureStream->bufEncodeInCreatePrm.numBuf)
      OSA_bufDelete(&pCaptureStream->bufEncodeIn);
  }

  for(k=0; k<gVIDEO_ctrl.faceDetectStream.bufFdInCreatePrm.numBuf; k++)
    OSA_cmemFree(gVIDEO_ctrl.faceDetectStream.bufFdInCreatePrm.bufVirtAddr[k]);

  if(gVIDEO_ctrl.faceDetectStream.bufFdInCreatePrm.numBuf)
    OSA_bufDelete(&gVIDEO_ctrl.faceDetectStream.bufFdIn);

  if(gVIDEO_ctrl.displayStream.bufDisplayInCreatePrm.numBuf)
    OSA_bufDelete(&gVIDEO_ctrl.displayStream.bufDisplayIn);

  for(i=0; i<gAVSERVER_config.numEncodeStream; i++) {
    pEncodeStream = &gVIDEO_ctrl.encodeStream[i];

    for(k=0; k<pEncodeStream->bufEncryptInCreatePrm.numBuf; k++)
      OSA_cmemFree(pEncodeStream->bufEncryptInCreatePrm.bufVirtAddr[k]);

    for(k=0; k<pEncodeStream->bufStreamInCreatePrm.numBuf; k++)
      OSA_cmemFree(pEncodeStream->bufStreamInCreatePrm.bufVirtAddr[k]);

    if(pEncodeStream->bufEncryptInCreatePrm.numBuf)
      OSA_bufDelete(&pEncodeStream->bufEncryptIn);

    if(pEncodeStream->bufStreamInCreatePrm.numBuf)
      OSA_bufDelete(&pEncodeStream->bufStreamIn);
  }

	for(k=0; k<gVIDEO_ctrl.motionStream.bufMotionInCreatePrm.numBuf; k++)
	{
	  if( gVIDEO_ctrl.motionStream.bufMotionInCreatePrm.bufVirtAddr[k])
	  {
			OSA_cmemFree(gVIDEO_ctrl.motionStream.bufMotionInCreatePrm.bufVirtAddr[k]);
	  }
	}

	if( gVIDEO_ctrl.motionStream.bufMotionInCreatePrm.numBuf )
		OSA_bufDelete(&(gVIDEO_ctrl.motionStream.bufMotionIn));

  return status;
}


int AVSERVER_tskConnectInit()
{
  int i, status;
  VIDEO_EncodeStream *pEncodeStream;
  VIDEO_CaptureStream *pCaptureStream;

  gVIDEO_ctrl.alawEnable = FALSE;

  if(gAVSERVER_config.captureRawInMode==AVSERVER_CAPTURE_RAW_IN_MODE_ISIF_IN)
  {
    gAVSERVER_config.captureSingleResize = FALSE;

    if(gAVSERVER_config.numCaptureStream > 2)
      gAVSERVER_config.numCaptureStream = 2;
  }

  if(!gAVSERVER_config.aewb_config.saldreEnable) {
	if(gAVSERVER_config.captureRawInMode==AVSERVER_CAPTURE_RAW_IN_MODE_DDR_IN)
    	gVIDEO_ctrl.alawEnable = TRUE;
  }
  
  #ifdef YUV_MODE
  gVIDEO_ctrl.alawEnable = FALSE;
  #endif

  if(gAVSERVER_config.vstabTskEnable)
  {
    gAVSERVER_config.sensorMode |= DRV_IMGS_SENSOR_MODE_VSTAB;
  }

  if(gAVSERVER_config.vnfTskEnable)
  {
    gAVSERVER_config.sensorMode |= DRV_IMGS_SENSOR_MODE_PIXEL_PAD;
  }

  i = gAVSERVER_config.faceDetectConfig.captureStreamId;

  gVIDEO_ctrl.faceDetectStream.fdWidth  = gAVSERVER_config.captureConfig[i].width;
  gVIDEO_ctrl.faceDetectStream.fdHeight = gAVSERVER_config.captureConfig[i].height;
  gVIDEO_ctrl.faceDetectStream.fdOffsetH = 320;

  if(gVIDEO_ctrl.faceDetectStream.fdWidth>gVIDEO_ctrl.faceDetectStream.fdOffsetH) {
    OSA_printf(" *** WARNING  FD: exceeded max input size for FD, disabling FD (current input %dx%d)!!!\n",
      gVIDEO_ctrl.faceDetectStream.fdWidth, gVIDEO_ctrl.faceDetectStream.fdHeight
      );
    gAVSERVER_config.faceDetectConfig.fdEnable = FALSE;
  }

  memset(&gVIDEO_ctrl.faceDetectStream.faceStatus, 0, sizeof(gVIDEO_ctrl.faceDetectStream.faceStatus));
  memset(&gVIDEO_ctrl.vsStream.vsStatus, 0xFF, sizeof(gVIDEO_ctrl.vsStream.vsStatus));
  memset(&gVIDEO_ctrl.aewbStream.aewbStatus, 0, sizeof(gVIDEO_ctrl.aewbStream.aewbStatus));

  for(i=0; i<gAVSERVER_config.numCaptureStream; i++) {
    pCaptureStream = &gVIDEO_ctrl.captureStream[i];

    pCaptureStream->bufLdcInCreatePrm.numBuf = 0;
    pCaptureStream->bufVnfInCreatePrm.numBuf = 0;
    pCaptureStream->bufResizeInCreatePrm.numBuf = 0;

    pCaptureStream->bufEncodeInCreatePrm.numBuf = VIDEO_NUM_BUF;

    if(gAVSERVER_config.captureSingleResize && i > 0) {

    } else {

      if(gAVSERVER_config.ldcTskEnable)
        pCaptureStream->bufLdcInCreatePrm.numBuf = VIDEO_NUM_BUF;

      if(gAVSERVER_config.vnfTskEnable)
        pCaptureStream->bufVnfInCreatePrm.numBuf = VIDEO_NUM_BUF;

      if(gAVSERVER_config.captureSingleResize) {
        pCaptureStream->bufResizeInCreatePrm.numBuf = pCaptureStream->bufEncodeInCreatePrm.numBuf;
      }
    }
  }

  gVIDEO_ctrl.displayStream.bufDisplayInCreatePrm.numBuf = VIDEO_NUM_BUF;

  gVIDEO_ctrl.faceDetectStream.bufFdInCreatePrm.numBuf = VIDEO_NUM_BUF;

  gVIDEO_ctrl.motionStream.bufMotionInCreatePrm.numBuf = VIDEO_NUM_BUF;

  for(i=0; i<gAVSERVER_config.numEncodeStream; i++) {
    pEncodeStream = &gVIDEO_ctrl.encodeStream[i];

    pEncodeStream->fileSaveState = VIDEO_STREAM_FILE_SAVE_STATE_IDLE;
    pEncodeStream->fileSaveIndex = 0;
    pEncodeStream->fileSaveHndl  = NULL;

    pEncodeStream->bufEncryptInCreatePrm.numBuf = 0;

    if(gAVSERVER_config.encryptTskEnable)
      pEncodeStream->bufEncryptInCreatePrm.numBuf = VIDEO_NUM_BUF;

    pEncodeStream->bufStreamInCreatePrm.numBuf = VIDEO_NUM_BUF;
  }

  status = AVSERVER_tskConnectReset();

	/////zeng 2012-06-28
	if(FALSE == AVSERVER_OSDStart())
		printf("[AVSERVER_tskConnectInit]AVSERVER_OSDStart-----------FALSE\n");
	else
		printf("[AVSERVER_tskConnectInit]AVSERVER_OSDStart-----------ok\n");

  return status;
}

int AVSERVER_tskConnectExit()
{
  int status;

  status = AVSERVER_bufFree();

  return status;
}


int AVSERVER_tskConnectReset()
{
  int i, status=OSA_SOK;
  VIDEO_CaptureStream *pCaptureStream;
  VIDEO_CaptureConfig *pCaptureConfig;
  VIDEO_EncodeStream *pEncodeStream;
  VIDEO_EncodeConfig *pEncodeConfig;

  for(i=0; i<gAVSERVER_config.numCaptureStream; i++) {

    pCaptureStream = &gVIDEO_ctrl.captureStream[i];
    pCaptureConfig = &gAVSERVER_config.captureConfig[i];

    pCaptureStream->captureNextTsk = VIDEO_TSK_NONE;
    pCaptureStream->ldcNextTsk = VIDEO_TSK_NONE;
    pCaptureStream->vnfNextTsk = VIDEO_TSK_NONE;

    pCaptureStream->resizeNextTsk = VIDEO_TSK_ENCODE;

    pCaptureStream->displayCopyTsk = VIDEO_TSK_NONE;
    pCaptureStream->faceDetectCopyTsk = VIDEO_TSK_NONE;
    pCaptureStream->swosdRunTsk = VIDEO_TSK_NONE;
    pCaptureStream->vsApplyTsk = VIDEO_TSK_NONE;

    if( gAVSERVER_config.captureSingleResize && i>0 )
    {
      pCaptureStream->swosdRunTsk = VIDEO_TSK_RESIZE;
      pCaptureStream->displayCopyTsk = VIDEO_TSK_RESIZE;
      pCaptureStream->faceDetectCopyTsk = VIDEO_TSK_RESIZE;

    } else
    {

      if(gAVSERVER_config.ldcTskEnable)
        pCaptureStream->captureNextTsk = VIDEO_TSK_LDC;
      else
      if(gAVSERVER_config.vnfTskEnable)
        pCaptureStream->captureNextTsk = VIDEO_TSK_VNF;
      else
      if(gAVSERVER_config.captureSingleResize)
        pCaptureStream->captureNextTsk = VIDEO_TSK_RESIZE;
      else
        pCaptureStream->captureNextTsk = VIDEO_TSK_ENCODE;

      if(gAVSERVER_config.ldcTskEnable) {
        if(gAVSERVER_config.vnfTskEnable)
          pCaptureStream->ldcNextTsk = VIDEO_TSK_VNF;
        else
        if(gAVSERVER_config.captureSingleResize)
          pCaptureStream->ldcNextTsk = VIDEO_TSK_RESIZE;
        else
          pCaptureStream->ldcNextTsk = VIDEO_TSK_ENCODE;
      }

      if(gAVSERVER_config.vnfTskEnable) {
        if(gAVSERVER_config.captureSingleResize)
          pCaptureStream->vnfNextTsk = VIDEO_TSK_RESIZE;
        else
          pCaptureStream->vnfNextTsk = VIDEO_TSK_ENCODE;
      }

      pCaptureStream->swosdRunTsk = VIDEO_TSK_CAPTURE;
      pCaptureStream->displayCopyTsk = VIDEO_TSK_CAPTURE;
      pCaptureStream->faceDetectCopyTsk = VIDEO_TSK_CAPTURE;
      pCaptureStream->vsApplyTsk = VIDEO_TSK_CAPTURE;

      if(gAVSERVER_config.ldcTskEnable) {
        pCaptureStream->swosdRunTsk = VIDEO_TSK_LDC;
        pCaptureStream->displayCopyTsk = VIDEO_TSK_LDC;
        pCaptureStream->faceDetectCopyTsk = VIDEO_TSK_LDC;
        pCaptureStream->vsApplyTsk = VIDEO_TSK_LDC;
      }
      if(gAVSERVER_config.vnfTskEnable) {
        pCaptureStream->vsApplyTsk = VIDEO_TSK_VNF;
        pCaptureStream->swosdRunTsk = VIDEO_TSK_VNF;
        pCaptureStream->displayCopyTsk = VIDEO_TSK_VNF;
        pCaptureStream->faceDetectCopyTsk = VIDEO_TSK_VNF;
      }

    }
  }

  for(i=0; i<gAVSERVER_config.numEncodeStream; i++) {
    pEncodeStream = &gVIDEO_ctrl.encodeStream[i];
    pEncodeConfig = &gAVSERVER_config.encodeConfig[i];

    pEncodeStream->encodeNextTsk = VIDEO_TSK_STREAM;

    if(gAVSERVER_config.encryptTskEnable)
      pEncodeStream->encodeNextTsk = VIDEO_TSK_ENCRYPT;

  }

  return status;
}

int AVSERVER_bufGetNextTskInfo(int tskId, int streamId, OSA_BufHndl **pBufHndl, OSA_TskHndl **pTskHndl)
{
  VIDEO_CaptureStream *pCaptureStream=NULL;
  VIDEO_EncodeStream  *pEncodeStream=NULL;
  int nextTskId;

  *pTskHndl=NULL;
  *pBufHndl=NULL;

  if(streamId<0 || streamId >= AVSERVER_MAX_STREAMS) {
    OSA_ERROR("Incorrect streamId (%d)\n", streamId);
    return OSA_EFAIL;
  }

  if(tskId <=VIDEO_TSK_NONE || tskId >= VIDEO_TSK_STREAM) {
    OSA_ERROR("Incorrect tskId (%d)\n", tskId);
    return OSA_EFAIL;
  }

  if(tskId <= VIDEO_TSK_RESIZE) {
    pCaptureStream = &gVIDEO_ctrl.captureStream[streamId];

    nextTskId = VIDEO_TSK_NONE;

    if(tskId==VIDEO_TSK_CAPTURE) {
      nextTskId = pCaptureStream->captureNextTsk;
    } else
    if(tskId==VIDEO_TSK_LDC) {
      nextTskId = pCaptureStream->ldcNextTsk;
    } else
    if(tskId==VIDEO_TSK_VNF) {
      nextTskId = pCaptureStream->vnfNextTsk;
    } else
    if(tskId==VIDEO_TSK_RESIZE) {
      nextTskId = pCaptureStream->resizeNextTsk;
    }

    if(nextTskId==VIDEO_TSK_NONE) {
      OSA_ERROR("Incorrect nextTskId (%d)\n", nextTskId);
      return OSA_EFAIL;
    }

    if(nextTskId==VIDEO_TSK_LDC) {
      *pBufHndl = &pCaptureStream->bufLdcIn;
      *pTskHndl = &gVIDEO_ctrl.ldcTsk;
    } else
    if(nextTskId==VIDEO_TSK_VNF) {
      *pBufHndl = &pCaptureStream->bufVnfIn;
      *pTskHndl = &gVIDEO_ctrl.vnfTsk;
    } else
    if(nextTskId==VIDEO_TSK_RESIZE) {
      *pBufHndl = &pCaptureStream->bufResizeIn;
      *pTskHndl = &gVIDEO_ctrl.resizeTsk;
    } else
    if(nextTskId==VIDEO_TSK_ENCODE) {
      *pBufHndl = &pCaptureStream->bufEncodeIn;
      *pTskHndl = &gVIDEO_ctrl.encodeTsk;
    } else {
      OSA_ERROR("Incorrect nextTskId (%d)\n", nextTskId);
      return OSA_EFAIL;
    }

  } else {

    pEncodeStream  = &gVIDEO_ctrl.encodeStream[streamId];

    nextTskId = VIDEO_TSK_NONE;

    if(tskId==VIDEO_TSK_ENCODE) {
      nextTskId = pEncodeStream->encodeNextTsk;
    } else
    if(tskId==VIDEO_TSK_ENCRYPT) {
      nextTskId = VIDEO_TSK_STREAM;
    }

    if(nextTskId==VIDEO_TSK_NONE) {
      OSA_ERROR("Incorrect nextTskId (%d)\n", nextTskId);
      return OSA_EFAIL;
    }

    if(nextTskId==VIDEO_TSK_ENCRYPT) {
      *pBufHndl = &pEncodeStream->bufEncryptIn;
      *pTskHndl = &gVIDEO_ctrl.encryptTsk;
    } else
    if(nextTskId==VIDEO_TSK_STREAM) {
      *pBufHndl = &pEncodeStream->bufStreamIn;
      *pTskHndl = &gVIDEO_ctrl.streamTsk;
    } else {
      OSA_ERROR("Incorrect nextTskId (%d)\n", nextTskId);
      return OSA_EFAIL;
    }
  }

  return OSA_SOK;
}

int AVSERVER_bufGetCurTskInfo(int tskId, int streamId, OSA_BufHndl **pBufHndl)
{
  VIDEO_CaptureStream *pCaptureStream=NULL;
  VIDEO_EncodeStream  *pEncodeStream=NULL;

  *pBufHndl=NULL;

  if(streamId<0 || streamId >= AVSERVER_MAX_STREAMS) {
    OSA_ERROR("Incorrect streamId (%d)\n", streamId);
    return OSA_EFAIL;
  }

  if(tskId <= VIDEO_TSK_CAPTURE || tskId > VIDEO_TSK_STREAM) {
    OSA_ERROR("Incorrect tskId (%d)\n", tskId);
    return OSA_EFAIL;
  }

  if(tskId <= VIDEO_TSK_ENCODE) {
    pCaptureStream = &gVIDEO_ctrl.captureStream[streamId];

    if(tskId==VIDEO_TSK_LDC) {
      *pBufHndl = &pCaptureStream->bufLdcIn;
    } else
    if(tskId==VIDEO_TSK_VNF) {
      *pBufHndl = &pCaptureStream->bufVnfIn;
    } else
    if(tskId==VIDEO_TSK_RESIZE) {
      *pBufHndl = &pCaptureStream->bufResizeIn;
    } else
    if(tskId==VIDEO_TSK_ENCODE) {
      *pBufHndl = &pCaptureStream->bufEncodeIn;
    } else {
      OSA_ERROR("Incorrect tskId (%d)\n", tskId);
      return OSA_EFAIL;
    }

  } else {

    pEncodeStream  = &gVIDEO_ctrl.encodeStream[streamId];

    if(tskId==VIDEO_TSK_ENCRYPT) {
      *pBufHndl = &pEncodeStream->bufEncryptIn;
    } else
    if(tskId==VIDEO_TSK_STREAM) {
      *pBufHndl = &pEncodeStream->bufStreamIn;
    } else {
      OSA_ERROR("Incorrect tskId (%d)\n", tskId);
      return OSA_EFAIL;
    }
  }

  return OSA_SOK;
}

OSA_BufInfo *AVSERVER_bufGetEmpty(int tskId, int streamId, int *bufId, int timeout)
{
  OSA_BufHndl *pBufHndl;
  OSA_TskHndl *pTskHndl;
  int status;

  *bufId = -1;

  status = AVSERVER_bufGetNextTskInfo(tskId, streamId, &pBufHndl, &pTskHndl);
  if(status!=OSA_SOK) {
    OSA_ERROR("AVSERVER_bufGetNextTskInfo(%d, %d)\n", tskId, streamId);
    return NULL;
  }

  status = OSA_bufGetEmpty(pBufHndl, bufId, timeout);
  if(status!=OSA_SOK) {
    //OSA_ERROR("OSA_bufGetEmpty(%d, %d, %d)\n", tskId, streamId, *bufId);
    return NULL;
  }

  return OSA_bufGetBufInfo(pBufHndl, *bufId);
}

OSA_BufInfo *AVSERVER_bufPutFull(int tskId, int streamId, int bufId)
{
  OSA_BufHndl *pBufHndl;
  OSA_TskHndl *pTskHndl;
  int status;

  status = AVSERVER_bufGetNextTskInfo(tskId, streamId, &pBufHndl, &pTskHndl);
  if(status!=OSA_SOK) {
    OSA_ERROR("AVSERVER_bufGetNextTskInfo(%d, %d)\n", tskId, streamId);
    return NULL;
  }

  status = OSA_bufPutFull(pBufHndl, bufId);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_bufPutFull(%d, %d, %d)\n", tskId, streamId, bufId);
    return NULL;
  }

  status = OSA_tskSendMsg(pTskHndl, NULL, AVSERVER_CMD_NEW_DATA, (void*)streamId, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskSendMsg(AVSERVER_CMD_NEW_DATA, %d, %d)\n", tskId, streamId);
    return NULL;
  }

  return OSA_bufGetBufInfo(pBufHndl, bufId);
}

OSA_BufInfo *AVSERVER_bufGetFull(int tskId, int streamId, int *bufId, int timeout)
{
  OSA_BufHndl *pBufHndl;
  int status;

  *bufId = -1;

  status = AVSERVER_bufGetCurTskInfo(tskId, streamId, &pBufHndl);
  if(status!=OSA_SOK) {
    OSA_ERROR("AVSERVER_bufGetCurTskInfo(%d, %d)\n", tskId, streamId);
    return NULL;
  }

  status = OSA_bufGetFull(pBufHndl, bufId, timeout);
  if(status!=OSA_SOK) {
    //OSA_ERROR("OSA_bufGetFull(%d, %d, %d)\n", tskId, streamId, *bufId);
    return NULL;
  }

  return OSA_bufGetBufInfo(pBufHndl, *bufId);
}

OSA_BufInfo *AVSERVER_bufPutEmpty(int tskId, int streamId, int bufId)
{
  OSA_BufHndl *pBufHndl;
  int status;

  status = AVSERVER_bufGetCurTskInfo(tskId, streamId, &pBufHndl);
  if(status!=OSA_SOK) {
    OSA_ERROR("AVSERVER_bufGetCurTskInfo(%d, %d)\n", tskId, streamId);
    return NULL;
  }

  status = OSA_bufPutEmpty(pBufHndl, bufId);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_bufPutEmpty(%d, %d, %d)\n", tskId, streamId, bufId);
    return NULL;
  }

  return OSA_bufGetBufInfo(pBufHndl, bufId);
}


//////zeng add 2012-06-28

#define OFFSET_12	4
#define OFFSET_16	8
#define OFFSET_32	8


#define		ZH32_FILE_PATH	"GB2312_32.DZK"
#define		ZH16_FILE_PATH	"ct_libhz16x16.so"
#define		ZH11_FILE_PATH  "GB2312_12.DZK"

char		*g_pZH32			= NULL;
char		*g_pZH16			= NULL;
char		*g_pZH11			= NULL;

void AVSERVER_SetOsdEnable(int nType , int bEnable)
{
	if(nType == 0 )
	{
		g_OSDBase.bEnglish = bEnable;
	}
	if(nType == 1 )
	{
		g_OSDBase.bTimeEn = bEnable;
	}
	if(nType == 2)
	{
		g_OSDBase.bTextEn = bEnable;
	}
}

void AVSERVER_SetOsdTextStr(char* pStr ,int nLen)
{
	if(!g_OSDBase.bTextEn) return ;
	if(nLen>32) return ;
	printf("[AVSERVER_SetOsdTextStr]pStr=%s,len=%d\n" ,pStr ,strlen(pStr));
	memset(&g_OSDBase.szText , 0 , sizeof(g_OSDBase.szText));
	sprintf(g_OSDBase.szText , "%s" , pStr );	
}


int AVSERVER_OSDStart()
{
	int		iLen	= 0;
	FILE*	pFile	= NULL;

	pFile = fopen(ZH16_FILE_PATH, "rb");
	if( pFile == NULL )
	{
		return FALSE;
	}
	fseek(pFile, 0, SEEK_END);
	iLen = ftell(pFile);
	fseek(pFile,0,SEEK_SET);	
	
	g_pZH16 = (char*)calloc(sizeof(char), iLen);	
	if( g_pZH16 == NULL )
	{
		fclose(pFile);
		pFile = NULL;
		return FALSE;
	}
	fread(g_pZH16, 1, iLen, pFile);
	fclose(pFile);
	
	pFile = NULL;
	pFile = fopen(ZH32_FILE_PATH, "rb");
	if( pFile == NULL )
	{
		return FALSE;
	}
	fseek(pFile, 0, SEEK_END);
	iLen = ftell(pFile);
	fseek(pFile,0,SEEK_SET);
	g_pZH32 = (char*)calloc(sizeof(char), iLen);
	if( g_pZH32 == NULL )
	{
		if(g_pZH16) free(g_pZH16);
		fclose(pFile);
		return FALSE;
	}
	fread(g_pZH32, 1, iLen, pFile);
	fclose(pFile);
	
	pFile = NULL;
	pFile = fopen(ZH11_FILE_PATH, "rb");
	if( pFile == NULL ){
		return FALSE;
	}
	fseek(pFile, 0, SEEK_END);
	iLen = ftell(pFile);
	fseek(pFile,0,SEEK_SET);
	g_pZH11 = (char*)calloc(sizeof(char), iLen);
	if( g_pZH11 == NULL )
	{
		if(g_pZH16)	free(g_pZH16);
		if(g_pZH32) free(g_pZH32);	
		fclose(pFile);
		return FALSE;
	}
	fread(g_pZH11, 1, iLen, pFile);
	fclose(pFile);	
	pFile = NULL;

	pthread_t		__pth__;
	pthread_create(&__pth__, NULL, (void *)OSD_ThreadOSD, (void *)NULL);
	
	return TRUE;
}

int OSD_GetBitMap(char* pText, int iType)		//iType:0 Text, 1:time
{
	char		*pSrc		= pText;
	int			iIndex		= 0;
	if( pSrc == NULL || g_pZH16 == NULL || g_pZH32 == NULL || g_pZH11 == NULL ){
		return -1;
	}

	if( 0 == iType ){
		memset(g_OSD11.szText, 0, sizeof(g_OSD11.szText));
		memset(g_OSD16.szText, 0, sizeof(g_OSD16.szText));
		memset(g_OSD32.szText, 0, sizeof(g_OSD32.szText));
		g_OSD11.iTextLen = 0;
		g_OSD16.iTextLen = 0;
		g_OSD32.iTextLen = 0;
	}else{
		memset(g_OSD11.szTime, 0, sizeof(g_OSD11.szTime));
		memset(g_OSD16.szTime, 0, sizeof(g_OSD16.szTime));
		memset(g_OSD32.szTime, 0, sizeof(g_OSD32.szTime));
		g_OSD11.iTimeLen = 0;
		g_OSD16.iTimeLen = 0;
		g_OSD32.iTimeLen = 0;
	}
	
	while( *pSrc != 0 ){
		if( *pSrc & 0x80 ){
			if( *(pSrc+1) & 0x80 ){
				iIndex = (*pSrc - 0xa1)*94 + (*(pSrc+1) - 0xa1);
				pSrc += 2;
			}else{
				pSrc += 1;
				continue;
			}
		}else{
			iIndex = 188 + (*pSrc-33);
			pSrc += 1;
		}

		if( 0 == iType ){
			if( g_OSD11.iTextLen + 24 > sizeof(g_OSD11.szText) )
			{
				printf("OSD_GetBitMap[]\n");
				return -1;
			}
			memcpy(g_OSD11.szText+g_OSD11.iTextLen, g_pZH11+iIndex*24, 24);
			g_OSD11.iTextLen += 24;

			if( g_OSD16.iTextLen + 32 > sizeof(g_OSD16.szText) ){
				printf("OSD_GetBitMap[]\n");
				return -1;
			}
			memcpy(g_OSD16.szText+g_OSD16.iTextLen, g_pZH16+iIndex*32, 32);
			g_OSD16.iTextLen += 32;

			if( g_OSD32.iTextLen + 128 > sizeof(g_OSD32.szText) ){
				printf("OSD_GetBitMap[]");
				return -1;
			}
			memcpy(g_OSD32.szText+g_OSD32.iTextLen, g_pZH32+iIndex*128, 128);
			g_OSD32.iTextLen += 128;

		}else if( 1 == iType ){
			if( g_OSD11.iTimeLen + 24 > sizeof(g_OSD11.szTime) ){
				printf("OSD_GetBitMap[], %d", g_OSD11.iTimeLen);
				return -1;
			}
			memcpy(g_OSD11.szTime+g_OSD11.iTimeLen, g_pZH11+iIndex*24, 24);
			g_OSD11.iTimeLen += 24;

			if( g_OSD16.iTimeLen + 32 > sizeof(g_OSD16.szTime) ){
				printf("OSD_GetBitMap[], %d", g_OSD16.iTimeLen);
				return -1;
			}
			memcpy(g_OSD16.szTime+g_OSD16.iTimeLen, g_pZH16+iIndex*32, 32);
			g_OSD16.iTimeLen += 32;

			if( g_OSD32.iTimeLen + 128 > sizeof(g_OSD32.szTime) ){
				printf("OSD_GetBitMap[]");
				return -1;
			}
			memcpy(g_OSD32.szTime+g_OSD32.iTimeLen, g_pZH32+iIndex*128, 128);
			g_OSD32.iTimeLen += 128;

		}
		
	}
	//PrintPixel(g_OSD32.szText, g_OSD32.iTextLen);
	return 0;
}


void* OSD_ThreadOSD(void *param)
{
	time_t           timep = {0};
	g_OSDBase.bTimeEn = TRUE;
	prctl(PR_SET_NAME, "osdthr");
	while( TRUE )
	{
		timep = time(NULL);
		if(	!g_OSDBase.bEnglish )
		{
			strftime(g_OSDBase.szTime, sizeof(g_OSDBase.szTime), "%Y-%m-%d %H:%M:%S", localtime( &timep));
		}
		else
		{
			strftime(g_OSDBase.szTime, sizeof(g_OSDBase.szTime), "%Y-%m-%d %H:%M:%S", localtime( &timep));
		}
		if(g_OSDBase.bTimeEn)
		{
			if(strlen(g_OSDBase.szTime)>0)
				OSD_GetBitMap(g_OSDBase.szTime, 1);
		}
		if(g_OSDBase.bTextEn)
		{
			if(strlen(g_OSDBase.szText)>0)
				OSD_GetBitMap(g_OSDBase.szText, 0);	
		}
		usleep(100*1000);
	}
	return 0;
}


void OSD_SetColor(char *c)
{
	if( *c > 128 ){
		*c = 0;
	}else{
		*c = 255;
	}
}

int OSD_GetColor(char c)
{
	if( c > 128 ){
		return 0;
	}else{
		return 255;
	}
}

int PrintPixelTime(char* pBitMap, int iMapLen, char* pBmp, int iWidth)
{
	int			i			= 0,
				iLineMask	= 0,
				iCount		= 0,
				iBaseOffset = 0,
				iOffset		= 0,
				iMask		= 0;
	char*		pPos		= NULL;
	int			iColor		= 0;


	if( iWidth >= 1280 ){
		iOffset = OFFSET_32;
		iMask = 128;
		iBaseOffset = 32;
		iLineMask = 4;

	}else if( iWidth >= 640 ){
		iOffset = OFFSET_16;
		iMask = 32;
		iBaseOffset = 16;
		iLineMask = 2;
	}else{
		iOffset = OFFSET_12;
		iMask = 24;
		iBaseOffset = 12;
		iLineMask = 2;
	}
	
	
	for(i=0;i<iMapLen;i++){
		if( i % iLineMask == 0 ){
			pPos = pBmp + iOffset + (iCount+10)*iWidth;
			iCount++;
			if( i % iMask == 0 ){
				iOffset += iBaseOffset;
				iCount = 0;
				iColor = OSD_GetColor(*pPos);
			}
		}

		if( pBitMap[i] & 0x80 ){
			//OSD_SetColor(pPos);
			*pPos = iColor;
		}
		++pPos;

		if( pBitMap[i] & 0x40 ){
			*pPos = iColor;
			//OSD_SetColor(pPos);
		}
		++pPos;

		if( pBitMap[i] & 0x20 ){
			*pPos = iColor;
			//OSD_SetColor(pPos);
		}
		++pPos;

		if( pBitMap[i] & 0x10 ){
			*pPos = iColor;
			//OSD_SetColor(pPos);
		}
		++pPos;

		if( pBitMap[i] & 0x08 ){
			*pPos = iColor;
			//OSD_SetColor(pPos);
		}
		++pPos;

		if( pBitMap[i] & 0x04 ){
			*pPos = iColor;
			//OSD_SetColor(pPos);
		}
		++pPos;

		if( pBitMap[i] & 0x02 ){
			*pPos = iColor;
			//OSD_SetColor(pPos);
		}
		++pPos;

		if( pBitMap[i] & 0x01 ){
			*pPos = iColor;
			//OSD_SetColor(pPos);
		}
		++pPos;
	}
	return 0;
}

int PrintPixelText(char* pBitMap, int iMapLen, char* pBmp, int iWidth, int iHeight)
{
	int			i			= 0,
				iLineMask	= 0,
				iCount		= 0,
				iBaseOffset = 0,
				iOffset		= 0,
				iMask		= 0,
				iOffsetLine = 0;
	char*		pPos		= NULL;
	int			iColor		= 0;


	if( iWidth >= 1280 ){
		iOffset = iWidth - iMapLen/4 - 50;
		iMask = 128;
		iBaseOffset = 32;
		iLineMask = 4;
		iOffsetLine = iHeight - 50;

	}else if(iWidth >= 640){
		iOffset = iWidth - iMapLen/2 - 25;
		iMask = 32;
		iBaseOffset = 16;
		iLineMask = 2;
		iOffsetLine = iHeight - 25;
	}else{
		iOffset = iWidth - iMapLen/2 - 15;
		iMask = 24;
		iBaseOffset = 12;
		iLineMask = 2;
		iOffsetLine = iHeight - 20;
	}


	for(i=0;i<iMapLen;i++){
		if( i % iLineMask == 0 ){
			pPos = pBmp + iOffset + (iCount+iOffsetLine)*iWidth;
			iCount++;
			if( i % iMask == 0 ){
				iOffset += iBaseOffset;
				iCount = 0;
				iColor = OSD_GetColor(*pPos);
			}
		}

		if( pBitMap[i] & 0x80 ){
			//OSD_SetColor(pPos);
			*pPos = iColor;
		}
		++pPos;

		if( pBitMap[i] & 0x40 ){
			//OSD_SetColor(pPos);
			*pPos = iColor;
		}
		++pPos;

		if( pBitMap[i] & 0x20 ){
			//OSD_SetColor(pPos);
			*pPos = iColor;
		}
		++pPos;

		if( pBitMap[i] & 0x10 ){
			//OSD_SetColor(pPos);
			*pPos = iColor;
		}
		++pPos;

		if( pBitMap[i] & 0x08 ){
			//OSD_SetColor(pPos);
			*pPos = iColor;
		}
		++pPos;

		if( pBitMap[i] & 0x04 ){
			//OSD_SetColor(pPos);
			*pPos = iColor;
		}
		++pPos;

		if( pBitMap[i] & 0x02 ){
			//OSD_SetColor(pPos);
			*pPos = iColor;
		}
		++pPos;

		if( pBitMap[i] & 0x01 ){
			//OSD_SetColor(pPos);
			*pPos = iColor;
		}
		++pPos;
	}
	return 0;
}

int OSDMaskText(short* szMask, int iMaskLen, short* pVideo, int iWidth, int iHeight)
{
	int			i				= 0,
				j				= 0,
				iMatrixWidth	= 0,
				iOffsetLine		= 0,
				iOffsetPicWidth	= 0,
				iCountLine		= 0;
	short		*pM				= NULL,
				*pV				= NULL;

	if( szMask == NULL || iMaskLen == 0 || pVideo == NULL ){
		return -1;
	}

	if( iWidth >= 1280 ){
		iMatrixWidth	= 32;
		iOffsetLine		= iHeight - 50;
		iOffsetPicWidth = iWidth - iMaskLen/32 - 50;

	}else if(iWidth >= 640){
		iMatrixWidth = 16;
	}else{
		iMatrixWidth = 12;
	}
	
	pM = (short*)szMask;
	pV = (short*)pVideo;
	for(i=0; i<iMatrixWidth;i++){
		*pV++ &= *pM++;
		*pV++ &= *pM++;
		*pV++ &= *pM++;
		*pV++ &= *pM++;
		*pV++ &= *pM++;
		*pV++ &= *pM++;
		*pV++ &= *pM++;
		*pV++ &= *pM++;

		*pV++ &= *pM++;
		*pV++ &= *pM++;
		*pV++ &= *pM++;
		*pV++ &= *pM++;
		*pV++ &= *pM++;
		*pV++ &= *pM++;
		*pV++ &= *pM++;
		*pV++ &= *pM++;

		pV = pVideo + iOffsetPicWidth + (i+iOffsetLine)*iWidth;
	}

	return 0;
}

int AVSERVER_SetVideoMask(int nNumber , int bEnable ,int nx0,int ny0,int nx1,int ny1)
{

	if(nNumber>4 || nNumber<0)
	{
		return FALSE;
	}
	g_MaskCfg.rcMask[nNumber].bMaskEn = bEnable;
	g_MaskCfg.rcMask[nNumber].iX0 = nx0;
	g_MaskCfg.rcMask[nNumber].iY0 = ny0;
	g_MaskCfg.rcMask[nNumber].iX1 = nx1;
	g_MaskCfg.rcMask[nNumber].iY1 = ny1;
	printf("[AVSERVER_SetVideoMask]rcMask[%d]ben=%d--------\n",nNumber ,g_MaskCfg.rcMask[nNumber].bMaskEn);
	printf("x0=%d,y0=%d,x1=%d,y1=%d\n",
		g_MaskCfg.rcMask[nNumber].iX0,
		g_MaskCfg.rcMask[nNumber].iY0,
		g_MaskCfg.rcMask[nNumber].iX1,
		g_MaskCfg.rcMask[nNumber].iY1);
	return TRUE;
}

int Mask_SetDisp(int iWidth, int iHeight, char* pVideoBuf0, char* pVideoBuf1)
{
	char*	pBuf = NULL;

	int		x0 = 0, y0 = 0, x1 = 0, y1 = 0;
	int		i = 0,
			j = 0,
			iLen = 0,
			iLenUV = iWidth*iHeight/4;

	for(i=0; i<4; i++)
	{
		if(FALSE == g_MaskCfg.rcMask[i].bMaskEn)
		{
			//printf("[%d]bMaskEn=%d\n" ,i ,g_MaskCfg.rcMask[i].bMaskEn);
			continue;
		}
		x0 = (g_MaskCfg.rcMask[i].iX0*iWidth)/100;
		y0 = (g_MaskCfg.rcMask[i].iY0*iHeight)/100;
		x1 = (g_MaskCfg.rcMask[i].iX1*iWidth)/100;
		y1 = (g_MaskCfg.rcMask[i].iY1*iHeight)/100;
		
		x0 = (x0>iWidth ? iWidth:x0);
		x1 = (x1>iWidth ? iWidth:x1);
		y0 = (y0>iHeight ? iHeight:y0);
		y1 = (y1>iHeight ? iHeight:y1);

		iLen = x1 - x0;
		if( iLen <= 0 ){
			continue;
		}
		if( y0 >= y1 ){
			continue;
		}
		//printf("x0=%d,y0=%d,x1=%d,y1=%d\n" ,x0,y0,x1,y1);
		for(j=y0; j<y1; j++)
		{
			pBuf = pVideoBuf0 + j*iWidth + x0;
			memset(pBuf, 0,iLen);
			if( j%2 == 0 )
			{
				pBuf = pVideoBuf1 + j*iWidth/2 + x0;
				memset(pBuf, 128, iLen);
			}
		}
		
	}


	return 0;
}

int AVSERVER_SetOSDPixel(int iWidth, int iHeight, char* pVideoBuf0, char* pVideoBuf1)	//pVideoBuf0¨º?Y¡¤?¨¢?,pVideoBuf1¨º?Uo¨ªV¡¤?¨¢?
{
	int			i = 0;
	char*		pPos = NULL;
	
	Mask_SetDisp(iWidth, iHeight, pVideoBuf0, pVideoBuf1);

	if( iWidth >= 1280 )
	{
		if( g_OSDBase.bTextEn ){
			PrintPixelText(g_OSD32.szText, g_OSD32.iTextLen, pVideoBuf0, iWidth, iHeight);
		}
		if( g_OSDBase.bTimeEn ){
			PrintPixelTime(g_OSD32.szTime, g_OSD32.iTimeLen, pVideoBuf0, iWidth);
		}
	}else if( iWidth >= 640 ){
		if( g_OSDBase.bTextEn ){
			PrintPixelText(g_OSD16.szText, g_OSD16.iTextLen, pVideoBuf0, iWidth, iHeight);
		}
		if( g_OSDBase.bTimeEn ){
			PrintPixelTime(g_OSD16.szTime, g_OSD16.iTimeLen, pVideoBuf0, iWidth);
		}
	}else{
		if( g_OSDBase.bTextEn ){
			PrintPixelText(g_OSD11.szText, g_OSD11.iTextLen, pVideoBuf0, iWidth, iHeight);
		}
		if( g_OSDBase.bTimeEn ){
			PrintPixelTime(g_OSD11.szTime, g_OSD11.iTimeLen, pVideoBuf0, iWidth);
		}
	}

	return 0;
}

