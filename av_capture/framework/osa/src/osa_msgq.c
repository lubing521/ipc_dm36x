

#include <osa_msgq.h>

int OSA_msgqCreate(OSA_MsgqHndl *hndl)
{
  pthread_mutexattr_t mutex_attr;
  pthread_condattr_t cond_attr;
  int status=OSA_SOK;
 
  status |= pthread_mutexattr_init(&mutex_attr);
  status |= pthread_condattr_init(&cond_attr);  
  
  status |= pthread_mutex_init(&hndl->lock, &mutex_attr);
  status |= pthread_cond_init(&hndl->condRd, &cond_attr);    
  status |= pthread_cond_init(&hndl->condWr, &cond_attr);  

  hndl->curRd = hndl->curWr = 0;
  hndl->count = 0;
  hndl->len   = OSA_MSGQ_LEN_MAX;

  if(status!=OSA_SOK)
    OSA_ERROR("OSA_msgqCreate() = %d \r\n", status);
    
  pthread_condattr_destroy(&cond_attr);
  pthread_mutexattr_destroy(&mutex_attr);
    
  return status;
}

int OSA_msgqDelete(OSA_MsgqHndl *hndl)
{
  pthread_cond_destroy(&hndl->condRd);
  pthread_cond_destroy(&hndl->condWr);
  pthread_mutex_destroy(&hndl->lock);  
  
  return OSA_SOK;
}

OSA_MsgHndl *OSA_msgqAllocMsgHndl(OSA_MsgqHndl *to, OSA_MsgqHndl *from, Uint16 cmd, void *prm, Uint16 msgFlags)
{
  OSA_MsgHndl *msg;
  
  msg = OSA_memAlloc( sizeof(OSA_MsgHndl) );
  
  if(msg!=NULL) {
    msg->pTo = to;
    msg->pFrom = from;
    msg->pPrm = prm;
    msg->status = OSA_SOK;
    msg->cmd = cmd;
    msg->flags = msgFlags;
  }

  return msg;    
}

int OSA_msgqSend(OSA_MsgqHndl *hndl, OSA_MsgHndl *msg, Uint32 timeout)
{
  int status = OSA_EFAIL;

  pthread_mutex_lock(&hndl->lock);

  while(1) {
    if( hndl->count < hndl->len ) {
      hndl->queue[hndl->curWr] = msg;
      hndl->curWr = (hndl->curWr+1)%hndl->len;
      hndl->count++;
      status = OSA_SOK;
      pthread_cond_signal(&hndl->condRd);
      break;
    } else {
      if(timeout == OSA_TIMEOUT_NONE)
        break;

      status = pthread_cond_wait(&hndl->condWr, &hndl->lock);
    }
  }

  pthread_mutex_unlock(&hndl->lock);

  return status;
}


int OSA_msgqSendMsg(OSA_MsgqHndl *to, OSA_MsgqHndl *from, Uint16 cmd, void *prm, Uint16 msgFlags, OSA_MsgHndl **msg)
{
  int status;
  OSA_MsgHndl *data;

  data = OSA_msgqAllocMsgHndl(to, from, cmd, prm, msgFlags);
  if(data==NULL)
    return OSA_EFAIL;
  
  status = OSA_msgqSend(to, data, OSA_TIMEOUT_FOREVER);
  
  if(status==OSA_SOK) {
    if(msg!=NULL)
      *msg = data;
  }

  return status;
}

int OSA_msgqRecvMsg(OSA_MsgqHndl *hndl, OSA_MsgHndl **msg, Uint32 timeout)
{
  int status = OSA_EFAIL;
  
  pthread_mutex_lock(&hndl->lock);
  
  while(1) {
    if(hndl->count > 0 ) {

      if(msg!=NULL) {
        *msg = hndl->queue[hndl->curRd];
      }
      
      hndl->curRd = (hndl->curRd+1)%hndl->len;
      hndl->count--;
      status = OSA_SOK;
      pthread_cond_signal(&hndl->condWr);
      break;
    } else {
      if(timeout == OSA_TIMEOUT_NONE)
        break;

      status = pthread_cond_wait(&hndl->condRd, &hndl->lock);
    }
  }

  pthread_mutex_unlock(&hndl->lock);

  return status;
}

int OSA_msgqSendAck(OSA_MsgHndl *msg, int ackRetVal)
{
  int status;

  msg->status = ackRetVal;
  
  status = OSA_msgqSend(msg->pFrom, msg, OSA_TIMEOUT_FOREVER);

  return status;
}

int OSA_msgqFreeMsgHndl(OSA_MsgHndl *msg)
{
  OSA_memFree(msg);
  return OSA_SOK;
}



