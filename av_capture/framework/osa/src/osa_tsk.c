#include <osa_tsk.h>
#include <sys/prctl.h>


void *OSA_tskThrMain(void *pPrm)
{
  OSA_MsgHndl *pMsg;
  OSA_TskHndl *pPrc;

  pPrc = (OSA_TskHndl *)pPrm;
  OSA_assert(pPrc!=NULL);

  while(1) {

    OSA_assertSuccess( OSA_tskWaitMsg(pPrc, &pMsg) );
    OSA_assert(pPrc->fncMain!=NULL);
    pPrc->fncMain(pPrc, pMsg, pPrc->curState);
  }
  
  return NULL;
} 

int OSA_setTskName(char *name)
{
    return prctl(PR_SET_NAME, name);
}

int OSA_tskCreate(OSA_TskHndl *pPrc, OSA_TskFncMain fncMain, Uint32 tskPri, Uint32 tskStackSize, Uint32 initState)
{
  pPrc->curState      = initState;
  pPrc->fncMain       = fncMain;

  OSA_assert(pPrc->fncMain      != NULL);

  OSA_mbxCreate(&pPrc->mbxHndl);

  OSA_assertSuccess( OSA_thrCreate(&pPrc->thrHndl, OSA_tskThrMain, tskPri, tskStackSize,  pPrc) );
  
  return OSA_SOK;
}

int OSA_tskDelete(OSA_TskHndl *pPrc)
{
  OSA_thrDelete(&pPrc->thrHndl);
  OSA_mbxDelete(&pPrc->mbxHndl);

  pPrc->curState      = 0;
  pPrc->fncMain       = NULL;

  return OSA_SOK;
}

int OSA_tskSendMsg(OSA_TskHndl *pPrcTo, OSA_TskHndl *pPrcFrom, Uint16 cmd, void *pPrm, Uint16 flags)
{
  int retVal;
  OSA_MbxHndl *pMbxFrom;

  OSA_assert(pPrcTo!=NULL);

  if(pPrcFrom==NULL) {
    pMbxFrom = NULL;
  } else {
    pMbxFrom = &pPrcFrom->mbxHndl;
  }

  retVal = OSA_mbxSendMsg(&pPrcTo->mbxHndl, pMbxFrom, cmd, pPrm, flags);

  return retVal;
}

int OSA_tskBroadcastMsg(OSA_TskHndl *pPrcToList[], OSA_TskHndl *pPrcFrom, Uint16 cmd, void *pPrm, Uint16 flags)
{
  int retVal;

  OSA_MbxHndl *pMbxToList[OSA_MBX_BROADCAST_MAX];
  Uint32 i, numMsg;
  OSA_MbxHndl *pMbxFrom;

  OSA_assert(pPrcToList!=NULL);

  if(pPrcFrom==NULL) {
    pMbxFrom = NULL;
  } else {
    pMbxFrom = &pPrcFrom->mbxHndl;
  }

  for(i=0; i<OSA_MBX_BROADCAST_MAX; i++)
  {
    pMbxToList[i]=NULL;
  }

  numMsg = 0;
  while(pPrcToList[numMsg]!=NULL) {
    pMbxToList[numMsg] = &pPrcToList[numMsg]->mbxHndl;
    numMsg++;
    if(numMsg>=OSA_MBX_BROADCAST_MAX) {
      // cannot broadcast to more than OSA_mbx_BROADCAST_MAX mailboxes
      OSA_assert(0);
    }
  }

  if(numMsg == 0) {
     // no mailboxes in 'to' mailbox list
     return OSA_SOK;
  }

  retVal = OSA_mbxBroadcastMsg(&pMbxToList[0], pMbxFrom, cmd, pPrm, flags);

  return retVal;
}


int OSA_tskWaitMsg(OSA_TskHndl *pPrc, OSA_MsgHndl **pMsg)
{
  int retVal;

  retVal = OSA_mbxWaitMsg(&pPrc->mbxHndl, pMsg);

  return retVal;
}

int OSA_tskCheckMsg(OSA_TskHndl *pPrc, OSA_MsgHndl **pMsg)
{
  int retVal;

  retVal = OSA_mbxCheckMsg(&pPrc->mbxHndl, pMsg);

  return retVal;
}

int OSA_tskAckOrFreeMsg(OSA_MsgHndl *pMsg, int ackRetVal)
{
  int retVal;

  retVal = OSA_mbxAckOrFreeMsg(pMsg, ackRetVal);

  return retVal;
}

int OSA_tskFlushMsg(OSA_TskHndl *pPrc)
{
  int retVal;

  retVal = OSA_mbxFlush(&pPrc->mbxHndl);

  return retVal;
}

int OSA_tskWaitCmd(OSA_TskHndl *pPrc, OSA_MsgHndl **pMsg, Uint16 waitCmd)
{
  int retVal;

  retVal = OSA_mbxWaitCmd(&pPrc->mbxHndl, pMsg, waitCmd);

  return retVal;
}

int OSA_tskSetState(OSA_TskHndl *pPrc, Uint32 curState)
{
  pPrc->curState = curState;
  return OSA_SOK;
}

Uint32 OSA_tskGetState(OSA_TskHndl *pPrc)
{
  return pPrc->curState;
}

