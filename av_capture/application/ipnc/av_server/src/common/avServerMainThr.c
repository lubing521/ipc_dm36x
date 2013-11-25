#include <avserver.h>
#include <sys/msg.h>

int AVSERVER_tskStart(OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg)
{
  int status;

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: AVSERVER_tskConnectInit() ...\n");
  #endif

  status = AVSERVER_tskConnectInit();

  if(status!=OSA_SOK)
    return status;

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Create captureTsk ...\n");
  #endif

  status = OSA_tskSendMsg(&gVIDEO_ctrl.captureTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

  if(status!=OSA_SOK) {
    OSA_ERROR("\n");
    return status;
  }

  if(gAVSERVER_config.ldcTskEnable) {

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Create ldcTsk ...\n");
    #endif

    status = OSA_tskSendMsg(&gVIDEO_ctrl.ldcTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

    if(status!=OSA_SOK) {
      OSA_ERROR("\n");
      return status;
    }
  }

  if(gAVSERVER_config.vnfTskEnable) {

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Create vnfTsk ...\n");
    #endif

    status = OSA_tskSendMsg(&gVIDEO_ctrl.vnfTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

    if(status!=OSA_SOK) {
      OSA_ERROR("\n");
      return status;
    }
  }

  if(gAVSERVER_config.captureSingleResize) {

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Create resizeTsk ...\n");
    #endif

    status = OSA_tskSendMsg(&gVIDEO_ctrl.resizeTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

    if(status!=OSA_SOK) {
      OSA_ERROR("\n");
      return status;
    }
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Create encodeTsk ...\n");
  #endif

  status = OSA_tskSendMsg(&gVIDEO_ctrl.encodeTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

  if(status!=OSA_SOK) {
    OSA_ERROR("\n");
    return status;
  }

  if(gAVSERVER_config.encryptTskEnable) {

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Create encryptTsk ...\n");
    #endif

    status = OSA_tskSendMsg(&gVIDEO_ctrl.encryptTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

    if(status!=OSA_SOK) {
      OSA_ERROR("\n");
      return status;
    }
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Create streamTsk ...\n");
  #endif

  status = OSA_tskSendMsg(&gVIDEO_ctrl.streamTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

  if(status!=OSA_SOK) {
    OSA_ERROR("\n");
    return status;
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Create fdTsk ...\n");
  #endif

  status = OSA_tskSendMsg(&gVIDEO_ctrl.fdTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

  if(status!=OSA_SOK) {
    OSA_ERROR("\n");
    return status;
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Create displayTsk ...\n");
  #endif

  status = OSA_tskSendMsg(&gVIDEO_ctrl.displayTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

  if(status!=OSA_SOK) {
    OSA_ERROR("\n");
    return status;
  }

  if(gAVSERVER_config.vstabTskEnable) {

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Create vsTsk ...\n");
    #endif

    status = OSA_tskSendMsg(&gVIDEO_ctrl.vsTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

    if(status!=OSA_SOK) {
      OSA_ERROR("\n");
      return status;
    }
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Create aewbTsk ...\n");
  #endif

  status = OSA_tskSendMsg(&gVIDEO_ctrl.aewbTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

  if(status!=OSA_SOK) {
    OSA_ERROR("\n");
    return status;
  }

#if 0
  if(gAVSERVER_config.audioTskEnable) {
	  #ifdef AVSERVER_DEBUG_MAIN_THR
	  OSA_printf(" AVSERVER MAIN: Create audioTsk ...\n");
	  #endif

	  status = OSA_tskSendMsg(&gAUDIO_ctrl.audioTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

	  if(status!=OSA_SOK) {
		OSA_ERROR("\n");
		return status;
	  }
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Create audioPlayTsk ...\n");
  #endif

  status = OSA_tskSendMsg(&gAUDIOPLAY_ctrl.audioTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

  if(status!=OSA_SOK) {
	OSA_ERROR("\n");
	return status;
  }
#endif


  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Create motionTsk ...\n");
  #endif
  status = OSA_tskSendMsg(&gVIDEO_ctrl.motionTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

  if(status!=OSA_SOK) {
    OSA_ERROR("\n");
    return status;
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Create swosdTsk ...\n");
  #endif
  status = OSA_tskSendMsg(&gVIDEO_ctrl.swosdTsk, pTsk, AVSERVER_CMD_CREATE, NULL, OSA_MBX_WAIT_ACK);

  if(status!=OSA_SOK) {
    OSA_ERROR("\n");
    return status;
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Allocing Buffers ...\n");
  #endif

  status = AVSERVER_bufAlloc();

  if(status!=OSA_SOK) {
    OSA_ERROR("\n");
    return status;
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Start aewbTsk ...\n");
  #endif

  OSA_tskSendMsg(&gVIDEO_ctrl.aewbTsk, pTsk, AVSERVER_CMD_START, NULL, OSA_MBX_WAIT_ACK);

  if(gAVSERVER_config.vstabTskEnable) {

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Start vsTsk ...");
    #endif

    OSA_tskSendMsg(&gVIDEO_ctrl.vsTsk, pTsk, AVSERVER_CMD_START, NULL, OSA_MBX_WAIT_ACK);
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Start captureTsk ...\n");
  #endif

  OSA_tskSendMsg(&gVIDEO_ctrl.captureTsk, pTsk, AVSERVER_CMD_START, NULL, OSA_MBX_WAIT_ACK);
// if 0 add by zeng
#if 0
  if(gAVSERVER_config.audioTskEnable) {
	  #ifdef AVSERVER_DEBUG_MAIN_THR
		OSA_printf(" AVSERVER MAIN: Start audioTsk ...\n");
	  #endif

	  OSA_tskSendMsg(&gAUDIO_ctrl.audioTsk, pTsk, AVSERVER_CMD_START, NULL, OSA_MBX_WAIT_ACK);
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Start audioPlayTsk ...\n");
  #endif

  OSA_tskSendMsg(&gAUDIOPLAY_ctrl.audioTsk, pTsk, AVSERVER_CMD_START, NULL, OSA_MBX_WAIT_ACK);

#endif

  OSA_tskSetState(pTsk, AVSERVER_MAIN_STATE_RUNNING);

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Start DONE\n");
  #endif

  return status;
}

int AVSERVER_tskStop(OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg)
{
  int status=OSA_SOK;

  if(gAVSERVER_config.audioTskEnable) {
	#ifdef AVSERVER_DEBUG_AUDIO_THR
	OSA_printf(" AVSERVER MAIN: Delete audioTsk ...\n");
	#endif

	status |= OSA_tskSendMsg(&gAUDIO_ctrl.audioTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);
  }

#if 0
  {
      // send DEL cmd to the audio receiver mbx
      sbuf.mtype = 1;
      sbuf.msg   = AVSERVER_CMD_DELETE;
      msgsnd(snd_msg_qid, &sbuf, sizeof(sbuf) - sizeof(long), IPC_NOWAIT);
      msgrcv(snd_msg_qid, &sbuf, sizeof(sbuf) - sizeof(long), 2, 0);
      //status |= OSA_tskSendMsg(&gAUDIOPLAY_ctrl.audioTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);
  }
#endif

  if(gAVSERVER_config.vstabTskEnable) {

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Delete vsTsk ...\n");
    #endif

    status |= OSA_tskSendMsg(&gVIDEO_ctrl.vsTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Delete aewbTsk ...\n");
  #endif

  status |= OSA_tskSendMsg(&gVIDEO_ctrl.aewbTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Delete captureTsk ...\n");
  #endif

  status |= OSA_tskSendMsg(&gVIDEO_ctrl.captureTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);

  if(gAVSERVER_config.ldcTskEnable) {

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Delete ldcTsk ...\n");
    #endif

    status |= OSA_tskSendMsg(&gVIDEO_ctrl.ldcTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);
  }

  if(gAVSERVER_config.vnfTskEnable) {

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Delete vnfTsk ...\n");
    #endif

    status |= OSA_tskSendMsg(&gVIDEO_ctrl.vnfTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);
  }

  if(gAVSERVER_config.captureSingleResize) {

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Delete resizeTsk ...\n");
    #endif

    status |= OSA_tskSendMsg(&gVIDEO_ctrl.resizeTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);
  }

  if(gAVSERVER_config.encryptTskEnable) {

    #ifdef AVSERVER_DEBUG_MAIN_THR
    OSA_printf(" AVSERVER MAIN: Delete encryptTsk ...\n");
    #endif

    status |= OSA_tskSendMsg(&gVIDEO_ctrl.encryptTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Delete encodeTsk ...\n");
  #endif

  status |= OSA_tskSendMsg(&gVIDEO_ctrl.encodeTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Delete motionTsk ...\n");
  #endif

  status |= OSA_tskSendMsg(&gVIDEO_ctrl.motionTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Delete streamTsk ...\n");
  #endif

  status |= OSA_tskSendMsg(&gVIDEO_ctrl.streamTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Delete displayTsk ...\n");
  #endif

  status |= OSA_tskSendMsg(&gVIDEO_ctrl.displayTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Delete fdTsk ...\n");
  #endif

  status |= OSA_tskSendMsg(&gVIDEO_ctrl.fdTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Delete swosdTsk ...\n");
  #endif

  status |= OSA_tskSendMsg(&gVIDEO_ctrl.swosdTsk, pTsk, AVSERVER_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK);

  status |= VIDEO_captureMemFree();

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: AVSERVER_tskConnectExit() ...\n");
  #endif

  status |= AVSERVER_tskConnectExit();

  OSA_tskSetState(pTsk, AVSERVER_MAIN_STATE_IDLE);

  if(status!=OSA_SOK) {
    OSA_ERROR("\n");
    return status;
  }

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Stop...DONE\n");
  #endif

  return status;
}

int AVSERVER_tskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status;
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  #ifdef AVSERVER_DEBUG_MAIN_THR
  OSA_printf(" AVSERVER MAIN: Recevied CMD = 0x%04x, state = 0x%04x\n", cmd, curState);
  #endif

  switch(cmd) {

    case AVSERVER_MAIN_CMD_START:

      if(curState==AVSERVER_MAIN_STATE_IDLE) {

        status = AVSERVER_tskStart(pTsk, pMsg);

        if(status!=OSA_SOK)
          AVSERVER_tskStop(pTsk, pMsg);
      }

      OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
      break;

    case AVSERVER_MAIN_CMD_STOP:

      if(curState==AVSERVER_MAIN_STATE_RUNNING) {

        AVSERVER_tskStop(pTsk, pMsg);
      }

      OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
      break;
  }


  return OSA_SOK;
}

int AVSERVER_mainCreate()
{
  int status;

  status = OSA_mbxCreate(&gAVSERVER_ctrl.uiMbx);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_mbxCreate()\n");
    return status;
  }

  status = OSA_tskCreate( &gAVSERVER_ctrl.mainTsk, AVSERVER_tskMain, AVSERVER_MAIN_THR_PRI, AVSERVER_MAIN_STACK_SIZE, AVSERVER_MAIN_STATE_IDLE);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }

  return status;
}

int AVSERVER_mainDelete()
{
  int status;

  status = OSA_tskDelete(&gAVSERVER_ctrl.mainTsk);
  status |= OSA_mbxDelete(&gAVSERVER_ctrl.uiMbx);

  if(status!=OSA_SOK)
    OSA_ERROR("\n");

  return status;
}
