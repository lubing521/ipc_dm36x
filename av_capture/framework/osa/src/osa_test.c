

#include <osa_tsk.h>

#define AUTO_TEST_DELAY    (1)
#define MANUAL_TEST_DELAY  (100)

#define OSA_TEST_STATE__READY  0
#define OSA_TEST_STATE__RUN    1
#define OSA_TEST_STATE__PAUSE  2

#define OSA_TEST_CMD__INIT     0
#define OSA_TEST_CMD__START    1
#define OSA_TEST_CMD__STOP     2
#define OSA_TEST_CMD__PAUSE    3
#define OSA_TEST_CMD__RESUME   4

#define OSA_TEST_PRC_STACK (1*KB)

OSA_MbxHndl gOSA_testMbx;

OSA_TskHndl gOSA_testPseq;
OSA_TskHndl gOSA_testPstA;
OSA_TskHndl gOSA_testPstB;
OSA_TskHndl gOSA_testPstC;

int OSA_TEST_PSEQ_Main(struct OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState );
int OSA_TEST_PSTA_Main(struct OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState );
int OSA_TEST_PSTB_Main(struct OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState );
int OSA_TEST_PSTC_Main(struct OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState );

int OSA_TEST_Init();
int OSA_TEST_Exit();

int OSA_TEST_Start(Uint32 delay);
int OSA_TEST_Pause();
int OSA_TEST_Resume();
int OSA_TEST_Stop();

int OSA_TEST_ManualTestRun();
int OSA_TEST_AutoTestRun();

char OSA_TEST_GetInput();

char gOSA_TEST_strMainMenu[] =
{
// *INDENT-OFF*
"\r\n"
"\r\n =================="
"\r\n OSA Test Main Menu"
"\r\n =================="
"\r\n"
"\r\n 1: Manual Test"
"\r\n 2: Auto Test"
"\r\n"
"\r\n 0: Exit"
"\r\n"
"\r\n Enter Choice : "
// *INDENT-ON*
};

int OSA_tskTestMain(int argc, char **argv)
{
  char ch;
  Bool done = FALSE;

  OSA_printf(" \r\n");
  OSA_printf(" Thread Priority [ min = %d , max = %d, default = %d ] \r\n", OSA_THR_PRI_MIN, OSA_THR_PRI_MAX, OSA_THR_PRI_DEFAULT);

  do {
    OSA_printf(gOSA_TEST_strMainMenu);

    ch = OSA_TEST_GetInput();
    OSA_printf("\r\n");

    switch (ch) {

      case '1':
        OSA_TEST_ManualTestRun();
        break;

      case '2':
        OSA_TEST_AutoTestRun();
        break;

      case '0':
        done = TRUE;
        break;
    }
  } while(!done);

  return OSA_SOK;
}


int OSA_TEST_PSEQ_Start(OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState)
{
  OSA_TskHndl *prcToList[] = 
  { 
    &gOSA_testPstA,
    &gOSA_testPstB,
    &gOSA_testPstC,
    NULL
  };
  
  Uint32 delay;

  delay = *(Uint32*)OSA_msgGetPrm(pMsg);

  OSA_tskSendMsg(&gOSA_testPstA, pPrc, OSA_TEST_CMD__INIT, &delay, OSA_MBX_WAIT_ACK);
  OSA_tskSendMsg(&gOSA_testPstB, pPrc, OSA_TEST_CMD__INIT, &delay, OSA_MBX_WAIT_ACK);
  OSA_tskSendMsg(&gOSA_testPstC, pPrc, OSA_TEST_CMD__INIT, &delay, OSA_MBX_WAIT_ACK);

  OSA_tskBroadcastMsg(prcToList, pPrc, OSA_TEST_CMD__START, NULL, 0);

  OSA_printf("\r\n PSEQ  : STARTED ");

  OSA_tskSetState(pPrc, OSA_TEST_STATE__RUN);

  return OSA_SOK;    
}

int OSA_TEST_PSEQ_Stop(OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState)
{
  OSA_tskSendMsg(&gOSA_testPstC, pPrc, OSA_TEST_CMD__STOP, NULL, OSA_MBX_WAIT_ACK);
  OSA_tskSendMsg(&gOSA_testPstB, pPrc, OSA_TEST_CMD__STOP, NULL, OSA_MBX_WAIT_ACK);
  OSA_tskSendMsg(&gOSA_testPstA, pPrc, OSA_TEST_CMD__STOP, NULL, OSA_MBX_WAIT_ACK);

  OSA_printf("\r\n PSEQ  : STOPPED ");

  OSA_tskSetState(pPrc, OSA_TEST_STATE__READY);

  return OSA_SOK;    
}

int OSA_TEST_PSEQ_Pause(OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState)
{
  OSA_TskHndl *prcToList[] = 
  { 
    &gOSA_testPstA,
    &gOSA_testPstB,
    &gOSA_testPstC,
    NULL
  };

  OSA_tskBroadcastMsg(prcToList, pPrc, OSA_TEST_CMD__PAUSE, NULL, OSA_MBX_WAIT_ACK);

  OSA_printf("\r\n PSEQ  : PAUSED ");

  OSA_tskSetState(pPrc, OSA_TEST_STATE__PAUSE);

  return OSA_SOK;    
}

int OSA_TEST_PSEQ_Resume(OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState)
{
  OSA_TskHndl *prcToList[] = 
  { 
    &gOSA_testPstA,
    &gOSA_testPstB,
    &gOSA_testPstC,
    NULL
  };

  OSA_tskBroadcastMsg(prcToList, pPrc, OSA_TEST_CMD__RESUME, NULL, OSA_MBX_WAIT_ACK);

  OSA_printf("\r\n PSEQ  : RESUMED ");

  OSA_tskSetState(pPrc, OSA_TEST_STATE__RUN);

  return OSA_SOK;    
}

int OSA_TEST_PSEQ_Main(struct OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int retVal = OSA_SOK;
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  switch(curState) {
    case OSA_TEST_STATE__READY:
      
      switch(cmd) {
        case OSA_TEST_CMD__START:
          OSA_TEST_PSEQ_Start(pPrc, pMsg, curState);
          OSA_tskAckOrFreeMsg(pMsg, retVal);
          break;
        
        default:
          OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
          break;
      }
      break;

    case OSA_TEST_STATE__RUN:

      switch(cmd) {
        case OSA_TEST_CMD__STOP:
          OSA_TEST_PSEQ_Stop(pPrc, pMsg, curState);
          OSA_tskAckOrFreeMsg(pMsg, retVal);
          break;

        case OSA_TEST_CMD__PAUSE:
          OSA_TEST_PSEQ_Pause(pPrc, pMsg, curState);
          OSA_tskAckOrFreeMsg(pMsg, retVal);
          break;
        
        default:
          OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
          break;
      }
      break;

    case OSA_TEST_STATE__PAUSE:

      switch(cmd) {
        case OSA_TEST_CMD__STOP:
          OSA_TEST_PSEQ_Stop(pPrc, pMsg, curState);
          OSA_tskAckOrFreeMsg(pMsg, retVal);
          break;

        case OSA_TEST_CMD__START:
        case OSA_TEST_CMD__RESUME:
          OSA_TEST_PSEQ_Resume(pPrc, pMsg, curState);
          OSA_tskAckOrFreeMsg(pMsg, retVal);
          break;
        
        default:
          OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
          break;
      }
      break;

    default:
      OSA_assert(0);
      break;
  }

  return retVal;
}

int OSA_TEST_PSTA_MainRun(OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState)
{
  int retVal;
  Bool done=FALSE;
  Uint16 cmd;
  Uint32 delay;

  delay = *(Uint32*)OSA_msgGetPrm(pMsg);

  // init
  OSA_waitMsecs(delay);
  OSA_printf("\r\n PST A : INIT DONE ");
  OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  OSA_tskWaitCmd(pPrc, NULL, OSA_TEST_CMD__START);

  OSA_printf("\r\n PST A : STARTED");
  OSA_tskSetState(pPrc, OSA_TEST_STATE__RUN);

  // run
  while(!done) {

    OSA_waitMsecs(delay);

    if(OSA_tskGetState(pPrc)==OSA_TEST_STATE__RUN) {
      OSA_printf("\r\n PST A : RUNNING ");
    }

    retVal = OSA_tskCheckMsg(pPrc, &pMsg);
    if(retVal!=OSA_SOK)
      continue;

    cmd = OSA_msgGetCmd(pMsg);

    switch(cmd) {
      case OSA_TEST_CMD__STOP:
        done = TRUE;
        break;
      case OSA_TEST_CMD__PAUSE:
        OSA_waitMsecs(delay);
        OSA_printf("\r\n PST A : PAUSED ");
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        OSA_tskSetState(pPrc, OSA_TEST_STATE__PAUSE);
        break;  
      case OSA_TEST_CMD__RESUME:
        OSA_waitMsecs(delay);
        OSA_printf("\r\n PST A : RESUMED ");
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        OSA_tskSetState(pPrc, OSA_TEST_STATE__RUN);
        break;  
      default:
        OSA_assert(0);
        break;
    }
  }

  // exit
  OSA_waitMsecs(delay);
  OSA_printf("\r\n PST A : STOPPED");
  OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
  OSA_tskSetState(pPrc, OSA_TEST_STATE__READY);

  return OSA_SOK;
}

int OSA_TEST_PSTA_Main(struct OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState )
{
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  OSA_assert(curState == OSA_TEST_STATE__READY);

  switch(cmd) {
    case OSA_TEST_CMD__INIT:
      OSA_TEST_PSTA_MainRun(pPrc, pMsg, curState);
      break;
    default:
      OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
      break;
  }

  return OSA_SOK;
}

int OSA_TEST_PSTB_MainRun(OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState)
{
  int retVal;
  Bool done=FALSE;
  Uint16 cmd;
  Uint32 delay;

  delay = *(Uint32*)OSA_msgGetPrm(pMsg);

  // init
  OSA_waitMsecs(delay);
  OSA_printf("\r\n PST B : INIT DONE ");
  OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  OSA_tskWaitCmd(pPrc, NULL, OSA_TEST_CMD__START);

  OSA_printf("\r\n PST B : STARTED");
  OSA_tskSetState(pPrc, OSA_TEST_STATE__RUN);

  // run
  while(!done) {

    OSA_waitMsecs(delay);

    if(OSA_tskGetState(pPrc)==OSA_TEST_STATE__RUN) {
      OSA_printf("\r\n PST B : RUNNING ");
    }

    retVal = OSA_tskCheckMsg(pPrc, &pMsg);
    if(retVal!=OSA_SOK)
      continue;

    cmd = OSA_msgGetCmd(pMsg);

    switch(cmd) {
      case OSA_TEST_CMD__STOP:
        done = TRUE;
        break;
      case OSA_TEST_CMD__PAUSE:
        OSA_waitMsecs(delay);
        OSA_printf("\r\n PST B : PAUSED ");
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        OSA_tskSetState(pPrc, OSA_TEST_STATE__PAUSE);
        break;  
      case OSA_TEST_CMD__RESUME:
        OSA_waitMsecs(delay);
        OSA_printf("\r\n PST B : RESUMED ");
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        OSA_tskSetState(pPrc, OSA_TEST_STATE__RUN);
        break;  
      default:
        OSA_assert(0);
        break;
    }
  }

  // exit
  OSA_waitMsecs(delay);
  OSA_printf("\r\n PST B : STOPPED");
  OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
  OSA_tskSetState(pPrc, OSA_TEST_STATE__READY);

  return OSA_SOK;
}

int OSA_TEST_PSTB_Main(struct OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState )
{
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  OSA_assert(curState == OSA_TEST_STATE__READY);

  switch(cmd) {
    case OSA_TEST_CMD__INIT:
      OSA_TEST_PSTB_MainRun(pPrc, pMsg, curState);
      break;
    default:
      OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
      break;
  }

  return OSA_SOK;
}

int OSA_TEST_PSTC_MainRun(OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState)
{
  int retVal;
  Bool done=FALSE;
  Uint16 cmd;
  Uint32 delay;

  delay = *(Uint32*)OSA_msgGetPrm(pMsg);

  // init
  OSA_waitMsecs(delay);
  OSA_printf("\r\n PST C : INIT DONE ");
  OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  OSA_tskWaitCmd(pPrc, NULL, OSA_TEST_CMD__START);

  OSA_printf("\r\n PST C : STARTED");
  OSA_tskSetState(pPrc, OSA_TEST_STATE__RUN);

  // run
  while(!done) {

    OSA_waitMsecs(delay);

    if(OSA_tskGetState(pPrc)==OSA_TEST_STATE__RUN) {
      OSA_printf("\r\n PST C : RUNNING ");
    }

    retVal = OSA_tskCheckMsg(pPrc, &pMsg);
    if(retVal!=OSA_SOK)
      continue;

    cmd = OSA_msgGetCmd(pMsg);

    switch(cmd) {
      case OSA_TEST_CMD__STOP:
        done = TRUE;
        break;
      case OSA_TEST_CMD__PAUSE:
        OSA_waitMsecs(delay);
        OSA_printf("\r\n PST C : PAUSED ");
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        OSA_tskSetState(pPrc, OSA_TEST_STATE__PAUSE);
        break;  
      case OSA_TEST_CMD__RESUME:
        OSA_waitMsecs(delay);
        OSA_printf("\r\n PST C : RESUMED ");
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        OSA_tskSetState(pPrc, OSA_TEST_STATE__RUN);
        break;  
      default:
        OSA_assert(0);
        break;
    }
  }

  // exit
  OSA_waitMsecs(delay);
  OSA_printf("\r\n PST C : STOPPED");
  OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
  OSA_tskSetState(pPrc, OSA_TEST_STATE__READY);

  return OSA_SOK;
}

int OSA_TEST_PSTC_Main(struct OSA_TskHndl *pPrc, OSA_MsgHndl *pMsg, Uint32 curState )
{
  Uint16 cmd = OSA_msgGetCmd(pMsg);

  OSA_assert(curState == OSA_TEST_STATE__READY);

  switch(cmd) {
    case OSA_TEST_CMD__INIT:
      OSA_TEST_PSTC_MainRun(pPrc, pMsg, curState);
      break;
    default:
      OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
      break;
  }

  return OSA_SOK;
}

int OSA_TEST_Init()
{
  OSA_tskCreate(&gOSA_testPseq, OSA_TEST_PSEQ_Main, 40, OSA_TEST_PRC_STACK, OSA_TEST_STATE__READY);
  OSA_tskCreate(&gOSA_testPstA, OSA_TEST_PSTA_Main, 50, OSA_TEST_PRC_STACK, OSA_TEST_STATE__READY);
  OSA_tskCreate(&gOSA_testPstB, OSA_TEST_PSTB_Main, 50, OSA_TEST_PRC_STACK, OSA_TEST_STATE__READY);
  OSA_tskCreate(&gOSA_testPstC, OSA_TEST_PSTC_Main, 50, OSA_TEST_PRC_STACK, OSA_TEST_STATE__READY);

  OSA_mbxCreate(&gOSA_testMbx);

  return OSA_SOK;
}

int OSA_TEST_Exit()
{
  OSA_tskDelete(&gOSA_testPseq);
  OSA_tskDelete(&gOSA_testPstA);
  OSA_tskDelete(&gOSA_testPstB);
  OSA_tskDelete(&gOSA_testPstC);

  OSA_mbxDelete(&gOSA_testMbx);

  return OSA_SOK;
}

char OSA_TEST_GetInput() 
{
  return getchar();
}

char gOSA_TEST_strMenu[] =
{
// *INDENT-OFF*
"\r\n"
"\r\n ================"
"\r\n KerLib Test Menu"
"\r\n ================"
"\r\n"
"\r\n 1: Start"
"\r\n 2: Stop"
"\r\n 3: Pause"
"\r\n 4: Resume"
"\r\n"
"\r\n 0: Exit"
"\r\n"
"\r\n Enter Choice : "
// *INDENT-ON*
};

int OSA_TEST_ManualTestRun()
{
  char ch;
  Bool done = FALSE;

  OSA_TEST_Init();

  do {
    OSA_printf(gOSA_TEST_strMenu);

    ch = OSA_TEST_GetInput();
    OSA_printf("\r\n");

    switch (ch) {

      case '1':
        OSA_TEST_Start(MANUAL_TEST_DELAY);
        break;

      case '2':
        OSA_TEST_Stop();
        break;

      case '3':
        OSA_TEST_Pause();
        break;

      case '4':
        OSA_TEST_Resume();
        break;

      case '0':
        OSA_TEST_Stop();
        done = TRUE;
        break;
    }
  } while(!done);

  OSA_TEST_Exit();

  return OSA_SOK;
}

int OSA_TEST_AutoTestRun()
{
  int i, delay, loop, loop2, loop3;
  int count=100;
  
  loop3 = count;

  delay = AUTO_TEST_DELAY;

  do {
    OSA_TEST_Init();

    loop2 = 2;

    do {
      loop  = 2;

      // start/stop test
      for(i=0;i<loop;i++) {
        OSA_TEST_Start(delay);
        OSA_waitMsecs(delay);
        OSA_TEST_Stop();
      }

      // send command in incorrect state
      OSA_TEST_Pause();
      OSA_TEST_Resume();
      OSA_TEST_Stop();

      // pause/resume test
      OSA_TEST_Start(delay);
      
      for(i=0;i<loop;i++) {
        OSA_TEST_Pause();
        OSA_waitMsecs(delay);
        OSA_TEST_Resume();
      }

      OSA_TEST_Stop();
    } while(loop2--);

    OSA_TEST_Exit();
  } while(loop3--);

  return OSA_SOK;
}

int OSA_TEST_SendCmdToPseq(Uint16 cmd, Uint32 delay) {

  Uint32 prm = delay;

  return OSA_mbxSendMsg( &gOSA_testPseq.mbxHndl, &gOSA_testMbx, cmd, &prm, OSA_MBX_WAIT_ACK);
}

int OSA_TEST_Start(Uint32 delay)
{
  return OSA_TEST_SendCmdToPseq(OSA_TEST_CMD__START, delay);
}

int OSA_TEST_Pause()
{
  return OSA_TEST_SendCmdToPseq(OSA_TEST_CMD__PAUSE, 0);
}

int OSA_TEST_Resume()
{
  return OSA_TEST_SendCmdToPseq(OSA_TEST_CMD__RESUME, 0);
}

int OSA_TEST_Stop()
{
  return OSA_TEST_SendCmdToPseq(OSA_TEST_CMD__STOP, 0);
}




