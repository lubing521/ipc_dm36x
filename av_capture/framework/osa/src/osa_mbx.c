

#include <osa_mbx.h>


int OSA_mbxCreate(OSA_MbxHndl *pMbxHndl)
{
  int status=OSA_SOK;
  
  status |= OSA_msgqCreate(&pMbxHndl->rcvMbx);
  status |= OSA_msgqCreate(&pMbxHndl->ackMbx);

  if(status!=OSA_SOK)
    OSA_ERROR("OSA_mbxCreate() = %d \r\n", status);

  return status;
}

int OSA_mbxDelete(OSA_MbxHndl *pMbxHndl)
{
  OSA_msgqDelete(&pMbxHndl->rcvMbx);
  OSA_msgqDelete(&pMbxHndl->ackMbx);

  return OSA_SOK;
}

int OSA_mbxFreeMsg(OSA_MsgHndl *pMsg)
{
  if(pMsg->flags & OSA_MBX_FREE_PRM) {
    if(pMsg->pPrm!=NULL) {
      OSA_memFree(pMsg->pPrm);
    }
  }

  OSA_assertSuccess( OSA_msgqFreeMsgHndl(pMsg) );

  return OSA_SOK;
}

int OSA_mbxSendMsg(OSA_MbxHndl *pMbxTo, OSA_MbxHndl *pMbxFrom, Uint16 cmd, void *pPrm, Uint16 flags)
{
  OSA_MsgHndl *pSentMsg, *pRcvMsg;
  Bool waitAck;
  int retVal=OSA_SOK;
  OSA_MsgqHndl *ackMbx;

  OSA_assert(pMbxTo!=NULL);

  if(pMbxFrom==NULL) {
    if(flags & OSA_MBX_WAIT_ACK) {
      // if from mail box is NULL, then cannot wait for ACK
      OSA_assert(0);
    }
    ackMbx = NULL;
  } else {
    ackMbx = &pMbxFrom->ackMbx;
  }

  retVal = OSA_msgqSendMsg(&pMbxTo->rcvMbx, ackMbx, cmd, pPrm, flags, &pSentMsg);
  OSA_assertSuccess(retVal);

  if( (flags & OSA_MBX_WAIT_ACK) && ackMbx != NULL ) {

    waitAck = TRUE;

    do {
      // wait for ACK
      retVal = OSA_msgqRecvMsg(ackMbx, &pRcvMsg, OSA_TIMEOUT_FOREVER);
      OSA_assertSuccess(retVal);

      if(pRcvMsg==pSentMsg) {
        // ACK received for sent MSG
        waitAck = FALSE;
        retVal  = OSA_msgGetAckStatus(pRcvMsg);
      }

      OSA_mbxFreeMsg(pRcvMsg);

    } while(waitAck);
  }

  return retVal;
}

int OSA_mbxBroadcastMsg(OSA_MbxHndl *pMbxToList[], OSA_MbxHndl *pMbxFrom, Uint16 cmd, void *pPrm, Uint16 flags)
{
  OSA_MsgHndl *pSentMsgList[OSA_MBX_BROADCAST_MAX];
  Bool  ackList[OSA_MBX_BROADCAST_MAX];
  OSA_MsgHndl *pRcvMsg;
  Bool waitAck, unknownAck;
  Uint32 i, numMsg;
  int retVal=OSA_SOK, ackRetVal = OSA_SOK;
  OSA_MsgqHndl *ackMbx;

  OSA_assert(pMbxToList!=NULL);

  if(pMbxFrom==NULL) {
    if(flags & OSA_MBX_WAIT_ACK) {
      // if from mail box is NULL, then cannot wait for ACK
      OSA_assert(0);
    }
    ackMbx = NULL;
  } else {
    ackMbx = &pMbxFrom->ackMbx;
  }

   // count number of mailboxes to boadcast to
   numMsg=0;
   while(pMbxToList[numMsg]!=NULL) {
     numMsg++;
     if(numMsg>=OSA_MBX_BROADCAST_MAX) {
        // cannot broadcast to more than OSA_MBX_BROADCAST_MAX mailboxes
        OSA_assert(0);
     }
   }

   if(numMsg == 0) {
     // no mailboxes in 'to' mailbox list
     return OSA_SOK;
   }

   // Cannot broadcast with malloc'ed prm and not wait for ACK
   if(flags & OSA_MBX_FREE_PRM) {
     if( !(flags & OSA_MBX_WAIT_ACK) && numMsg > 1) {
       OSA_assert(0);
     }
   }

   // reset sent MSG list and ACK list
   for(i=0; i<OSA_MBX_BROADCAST_MAX; i++) {
     ackList[i]  = FALSE;
     pSentMsgList[i] = NULL;
   }

   // send MSG to all mailboxes
   for(i=0; i<numMsg; i++) {
      retVal = OSA_msgqSendMsg(&pMbxToList[i]->rcvMbx, ackMbx, cmd, pPrm, flags, &pSentMsgList[i]);
      OSA_assertSuccess(retVal);
   }

   if((flags & OSA_MBX_WAIT_ACK) && ackMbx!=NULL ) {
      // wait for ACKs
      do {
        // wait ACK
        retVal = OSA_msgqRecvMsg(ackMbx, &pRcvMsg, OSA_TIMEOUT_FOREVER);
        OSA_assertSuccess(retVal);

        unknownAck=TRUE;

        // mark ACK as received for sent MSG
        for(i=0; i<numMsg; i++) {
          if(pRcvMsg == pSentMsgList[i] ) {
            ackList[i]=TRUE;
            unknownAck=FALSE;
            if(ackRetVal==OSA_SOK) {
              ackRetVal  = OSA_msgGetAckStatus(pRcvMsg);
            }
            break;
          }
        }

        // check if all ACKs received
        waitAck = FALSE;
        for(i=0; i<numMsg; i++) {
          if(ackList[i]==FALSE ) {
            waitAck = TRUE;
            break;
          }
        }

        if(unknownAck) {
          // ACK received is for some old message, hence free MSG and prm
          OSA_mbxFreeMsg(pRcvMsg);
        } else {
          // only free MSG now, free prm after all ACKs are received
          OSA_assertSuccess( OSA_msgqFreeMsgHndl(pRcvMsg) );
        }

      } while(waitAck);

      if(flags & OSA_MBX_FREE_PRM) {
        if(pPrm!=NULL) {
          OSA_memFree(pPrm);
        }
      }

      retVal = ackRetVal;
   }

   return retVal;
}


int OSA_mbxWaitMsg(OSA_MbxHndl *pMbxHndl, OSA_MsgHndl **pMsg)
{
   int retVal;

   retVal = OSA_msgqRecvMsg(&pMbxHndl->rcvMbx, pMsg, OSA_TIMEOUT_FOREVER);
   OSA_assertSuccess(retVal);

   return retVal;
}

int OSA_mbxCheckMsg(OSA_MbxHndl *pMbxHndl, OSA_MsgHndl **pMsg)
{
   int retVal;

   retVal = OSA_msgqRecvMsg(&pMbxHndl->rcvMbx, pMsg, OSA_TIMEOUT_NONE);

   return retVal;
}

int OSA_mbxAckOrFreeMsg(OSA_MsgHndl *pMsg, int ackRetVal)
{
  int retVal=OSA_SOK;

  if(pMsg==NULL)
    return OSA_EFAIL;

  if(pMsg->flags & OSA_MBX_WAIT_ACK) {
     // ACK message
     retVal = OSA_msgqSendAck(pMsg, ackRetVal);
     OSA_assertSuccess(retVal);
  } else {
    // FREE message and prm
    OSA_mbxFreeMsg(pMsg);
  }

  return retVal;
}

int OSA_mbxFlush(OSA_MbxHndl *pMbxHndl)
{
  int retVal;
  OSA_MsgHndl *pMsg;

  // flush receive mailbox
  do {
    retVal = OSA_mbxCheckMsg(pMbxHndl, &pMsg);
    if(retVal==OSA_SOK) {
      OSA_mbxAckOrFreeMsg(pMsg, OSA_SOK);
    }
  }while(retVal==OSA_SOK);

  // flush ACK mailbox
  do {
    retVal = OSA_msgqRecvMsg(&pMbxHndl->ackMbx, &pMsg, OSA_TIMEOUT_NONE);
    if(retVal==OSA_SOK) {
      OSA_mbxFreeMsg(pMsg);
    }
  } while(retVal==OSA_SOK);

  return retVal;
}


int OSA_mbxWaitCmd(OSA_MbxHndl *pMbxHndl, OSA_MsgHndl **pMsg, Uint16 waitCmd)
{
  OSA_MsgHndl *pRcvMsg;

  while(1) {
    OSA_mbxWaitMsg(pMbxHndl, &pRcvMsg);
    if(OSA_msgGetCmd(pRcvMsg)==waitCmd)
      break;
    OSA_mbxAckOrFreeMsg(pRcvMsg, OSA_SOK);
  }

  if(pMsg==NULL) {
    OSA_mbxAckOrFreeMsg(pRcvMsg, OSA_SOK);
  } else {
    *pMsg = pRcvMsg;
  }

  return OSA_SOK;
}

