
#include <avserver.h>
#include <aewb_xdm.h>

int VIDEO_aewbSetPrm(ALG_AewbStatus *pPrm)
{
  OSA_mutexLock(&gVIDEO_ctrl.aewbStream.aewbLock);

  memcpy(&gVIDEO_ctrl.aewbStream.aewbStatus, pPrm, sizeof(gVIDEO_ctrl.aewbStream.aewbStatus));

  OSA_mutexUnlock(&gVIDEO_ctrl.aewbStream.aewbLock);

  return OSA_SOK;
}

int VIDEO_aewbGetPrm(ALG_AewbStatus *pPrm)
{
  OSA_mutexLock(&gVIDEO_ctrl.aewbStream.aewbLock);

  memcpy(pPrm, &gVIDEO_ctrl.aewbStream.aewbStatus, sizeof(gVIDEO_ctrl.aewbStream.aewbStatus));

  OSA_mutexUnlock(&gVIDEO_ctrl.aewbStream.aewbLock);

  return OSA_SOK;
}

int VIDEO_aewbApplyPrm()
{
  ALG_AewbStatus prm;

  DRV_IpipeWb wb;

  VIDEO_aewbGetPrm(&prm);

  wb.gainGr = prm.gainGr;
  wb.gainGb = prm.gainGb;
  wb.gainR  = prm.gainR;
  wb.gainB  = prm.gainB;

  if(prm.gainGr) {
    DRV_ipipeSetWb(&wb);

    #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" 2A: Setting WB Gr=%d R=%d B=%d Gb=%d\n", wb.gainGr, wb.gainR, wb.gainB, wb.gainGb);
      #endif
    #endif
  }

  if(prm.ipipeGain) {
    DRV_isifSetDgain(prm.ipipeGain, prm.ipipeGain, prm.ipipeGain, prm.ipipeGain, 0);

    #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" 2A: Setting DGAIN=%d\n", prm.ipipeGain);
      #endif
    #endif
  }

  if(prm.sensorGain) {
    drvImgsFunc->imgsSetAgain(prm.sensorGain, 1231);

    #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" 2A: Setting AGAIN=%d\n", prm.sensorGain);
      #endif
    #endif
  }

  if(prm.exposureTimeInUsec) {
    drvImgsFunc->imgsSetEshutter(prm.exposureTimeInUsec, 33333);

    #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" 2A: Setting exposure=%d usec\n", prm.exposureTimeInUsec);
      #endif
    #endif
  }

  return OSA_SOK;
}

int VIDEO_aewbTskCreate()
{
  ALG_AewbCreate createPrm;



  createPrm.algId 			= gAVSERVER_config.aewb_config.aewbVendor;
  createPrm.sensorMode 		= gAVSERVER_config.sensorMode;
  createPrm.pH3aInfo 		= &gVIDEO_ctrl.captureInfo.h3aInfo;
  createPrm.vnfDemoCfg 		= gAVSERVER_config.vnfDemoEnable;
  createPrm.aewbType   		= gAVSERVER_config.aewb_config.aewbType;
  createPrm.aewbVendor  	= gAVSERVER_config.aewb_config.aewbVendor;
  createPrm.aewbPriority   	= gAVSERVER_config.aewb_config.aewbPriority;
  createPrm.aewbBinEnable   = gAVSERVER_config.aewb_config.aewbBinEnable;
  createPrm.reduceShutter   = gAVSERVER_config.aewb_config.reduceShutter;
  createPrm.saldre   		= gAVSERVER_config.aewb_config.saldreEnable;
  createPrm.flickerType   	= (gAVSERVER_config.aewb_config.flickerType==FLICKER_NTSC)?VIDEO_NTSC:VIDEO_PAL;
  createPrm.flickerType   	= gAVSERVER_config.aewb_config.flickerType; //Gang: for FLICKER_NONE
  createPrm.shiftValue 		= (gAVSERVER_config.sensorMode >= DRV_IMGS_SENSOR_MODE_1280x720) ? 3:4;
  								/* shift=3 for 1080P/720P, shift=4 for D1 */

  gVIDEO_ctrl.aewbStream.algAewbHndl = ALG_aewbCreate(&createPrm);
  if(gVIDEO_ctrl.aewbStream.algAewbHndl==NULL) {
    OSA_ERROR("ALG_aewbCreate(0x%04x)\n", gAVSERVER_config.sensorMode);
    return OSA_EFAIL;
  }

  return OSA_SOK;
}

int VIDEO_aewbTskDelete()
{
  int status;

  status = ALG_aewbDelete(gVIDEO_ctrl.aewbStream.algAewbHndl);

  return status;
}

int VIDEO_aewbTskRun()
{
  int status, aewbBufId, afBufId;
  OSA_BufInfo *pBufInfo_aewb, *pBufInfo_af;
  ALG_AewbRunPrm runPrm;
  ALG_AfRunPrm runPrm_af;
  ALG_AewbStatus aewbStatus;
  static int af_loop = 0;

  status = DRV_h3aGetBuf(DRV_H3A_MOD_AEWB, &aewbBufId, OSA_TIMEOUT_FOREVER);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_h3aGetBuf()\n");
    return status;
  }

  pBufInfo_aewb = DRV_h3aGetBufInfo(DRV_H3A_MOD_AEWB, aewbBufId);

  OSA_assert(pBufInfo_aewb!=NULL);

  if(pBufInfo_aewb!=NULL) {

    #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" 2A: H3a Buf %d\n", aewbBufId);
      #endif
    #endif

    OSA_assert(pBufInfo_aewb->virtAddr!=NULL);

    runPrm.awbMode          = ALG_AWB_MODE_INDOOR;
    runPrm.aeMode           = ALG_AE_MODE_DAY;
    runPrm.videoMode        = ALG_VIDEO_MODE_NTSC;
    runPrm.binMode          = TRUE;
    runPrm.sharpness        = 0;
    runPrm.contrast         = 0;
    runPrm.brightness       = 0;
    runPrm.backLight        = 0;
    runPrm.saturation       = 0;
    runPrm.h3aDataVirtAddr  = pBufInfo_aewb->virtAddr;

    runPrm.vnfDemoCfg 		= gAVSERVER_config.vnfDemoEnable;
	runPrm.aewbType   		= gAVSERVER_config.aewb_config.aewbType;
    runPrm.aewbVendor   	= gAVSERVER_config.aewb_config.aewbVendor;
    runPrm.aewbPriority   	= gAVSERVER_config.aewb_config.aewbPriority;
  	runPrm.reduceShutter    = gAVSERVER_config.aewb_config.reduceShutter;
  	runPrm.saldre   		= gAVSERVER_config.aewb_config.saldreEnable;
	runPrm.sensorMode		= gAVSERVER_config.sensorMode;

    if(gAVSERVER_config.aewb_config.aewbEnable) {
      OSA_prfBegin(&gAVSERVER_ctrl.aewbPrf);

      status = ALG_aewbRun(gVIDEO_ctrl.aewbStream.algAewbHndl, &runPrm, &aewbStatus);

      OSA_prfEnd(&gAVSERVER_ctrl.aewbPrf, 1);
    } else {
      memset(&aewbStatus, 0, sizeof(aewbStatus));
    }

    #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" 2A: H3a Buf %d ... DONE\n", aewbBufId);
      #endif
    #endif

    if(status==OSA_SOK)
      VIDEO_aewbSetPrm(&aewbStatus);
  }

  status = DRV_h3aGetBuf(DRV_H3A_MOD_AF, &afBufId, OSA_TIMEOUT_FOREVER);
  if(status !=OSA_SOK) {
    OSA_ERROR("DRV_h3aGetBuf()\n");
    return status;
  }

  pBufInfo_af = DRV_h3aGetBufInfo(DRV_H3A_MOD_AF, afBufId);

  OSA_assert(pBufInfo_af!=NULL);

  if(pBufInfo_af!=NULL) {
	 #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
		 #ifdef AVSERVER_DEBUG_RUNNING
			 OSA_printf(" 2A: H3a Buf %d\n", afBufId);
		 #endif
	 #endif

	 OSA_assert(pBufInfo_af->virtAddr!=NULL);

	 runPrm_af.h3aDataVirtAddr  = pBufInfo_af->virtAddr;
	 runPrm_af.pH3aInfo 		= &gVIDEO_ctrl.captureInfo.h3aInfo;

	 if(gAVSERVER_config.aewb_config.afEnable) {
   		OSA_prfBegin(&gAVSERVER_ctrl.afPrf);

		if(af_loop==30) {
			int focus_val = 0;
			status = ALG_afRun(&runPrm_af, &focus_val);
			af_loop = 0;
			gAVSERVER_config.aewb_config.autoFocusVal = focus_val;
			//OSA_printf("AutoFocusValue : %d \n", gAVSERVER_config.aewb_config.autoFocusVal);
		}
		else
			af_loop++;

	    OSA_prfEnd(&gAVSERVER_ctrl.afPrf, 1);
	 }

	 #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
	   #ifdef AVSERVER_DEBUG_RUNNING
	   OSA_printf(" 2A: H3a Buf %d ... DONE\n", afBufId);
	   #endif
	 #endif

  }

  DRV_h3aPutBuf(DRV_H3A_MOD_AF, afBufId);
  DRV_h3aPutBuf(DRV_H3A_MOD_AEWB, aewbBufId);

  return status;
}

void mSleep(const int mseconds)
{
    struct timeval tv;
    tv.tv_sec  = mseconds / 1000;
    tv.tv_usec = (mseconds % 1000) * 1000;
    select(0, NULL, NULL, NULL, &tv);
}

int VIDEO_aewbTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  OSA_setTskName("lhy_aewb");
  
  #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
  OSA_printf(" 2A: Recevied CMD = 0x%04x\n", cmd);
  #endif

  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
  OSA_printf(" 2A: Create...\n");
  #endif
   // test by fan
    OSA_printf(" ***** fan1:2A therad start run .....*****\n");
  status = VIDEO_aewbTskCreate();

  OSA_tskAckOrFreeMsg(pMsg, status);

  if(status!=OSA_SOK) {
    OSA_ERROR("VIDEO_aewbTskCreate()\n");
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
  OSA_printf(" 2A: Create...DONE\n");
  #endif

  status = OSA_tskWaitMsg(pTsk, &pMsg);
  if(status!=OSA_SOK)
    return OSA_SOK;

  cmd = OSA_msgGetCmd(pMsg);

  if(cmd==AVSERVER_CMD_DELETE) {

    done = TRUE;
    ackMsg = TRUE;

  } else {

    #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
    OSA_printf(" 2A: Start...\n");
    #endif

    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
  }

  #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
  OSA_printf(" 2A: Start...DONE\n");
  #endif

  while(!done) 
  {
    mSleep(40);
    status = VIDEO_aewbTskRun();

    if(status!=OSA_SOK) {
      OSA_ERROR("VIDEO_aewbTskRun()\n");
      break;
    }

    status = OSA_tskCheckMsg(pTsk, &pMsg);

    if(status!=OSA_SOK)
      continue;

    #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
    OSA_printf(" 2A: Received CMD = 0x%04x\n", cmd);
    #endif

    cmd = OSA_msgGetCmd(pMsg);

    switch(cmd) {
      case AVSERVER_CMD_DELETE:
        done = TRUE;
        ackMsg = TRUE;
        break;

      default:

        #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
        OSA_printf(" 2A: Unknown CMD = 0x%04x\n", cmd);
        #endif

        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
    }
  }

  #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
  OSA_printf(" 2A: Delete...\n");
  #endif

  VIDEO_aewbTskDelete();

  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  #ifdef AVSERVER_DEBUG_VIDEO_2A_THR
  OSA_printf(" 2A: Delete...DONE\n");
  #endif

  return OSA_SOK;
}

int VIDEO_aewbImageTuneCmdExecuteAewbEnable(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  Uint32 enable;

  if( cmdInfo->prmSize > sizeof(Uint32) ) {
    return OSA_EFAIL;
  }

  enable = cmdInfo->prm[0];

  AVSERVER_aewbEnable(enable);

  return OSA_SOK;
}

int VIDEO_aewbCreate()
{
  int status;

  IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_SYS_AEWB_ENABLE  , VIDEO_aewbImageTuneCmdExecuteAewbEnable );

  status = OSA_tskCreate( &gVIDEO_ctrl.aewbTsk, VIDEO_aewbTskMain, VIDEO_2A_THR_PRI, VIDEO_2A_STACK_SIZE, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }

  return status;
}

int VIDEO_aewbDelete()
{
  int status;

  status = OSA_tskDelete( &gVIDEO_ctrl.aewbTsk );

  return status;
}
