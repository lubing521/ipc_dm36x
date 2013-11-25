

#include <osa_que.h>

int OSA_queCreate(OSA_QueHndl *hndl, Uint32 maxLen)
{
  pthread_mutexattr_t mutex_attr;
  pthread_condattr_t cond_attr;
  int status=OSA_SOK;

  hndl->curRd = hndl->curWr = 0;
  hndl->count = 0;
  hndl->len   = maxLen;
  hndl->queue = OSA_memAlloc(sizeof(Int32)*hndl->len);
  
  if(hndl->queue==NULL) {
    OSA_ERROR("OSA_queCreate() = %d \r\n", status);
    return OSA_EFAIL;
  }
 
  status |= pthread_mutexattr_init(&mutex_attr);
  status |= pthread_condattr_init(&cond_attr);  
  
  status |= pthread_mutex_init(&hndl->lock, &mutex_attr);
  status |= pthread_cond_init(&hndl->condRd, &cond_attr);    
  status |= pthread_cond_init(&hndl->condWr, &cond_attr);  

  if(status!=OSA_SOK)
    OSA_ERROR("OSA_queCreate() = %d \r\n", status);
    
  pthread_condattr_destroy(&cond_attr);
  pthread_mutexattr_destroy(&mutex_attr);
    
  return status;
}

int OSA_queDelete(OSA_QueHndl *hndl)
{
  if(hndl->queue!=NULL)
    OSA_memFree(hndl->queue);
    
  pthread_cond_destroy(&hndl->condRd);
  pthread_cond_destroy(&hndl->condWr);
  pthread_mutex_destroy(&hndl->lock);  
  
  return OSA_SOK;
}



int OSA_quePut(OSA_QueHndl *hndl, Int32 value, Uint32 timeout)
{
  int status = OSA_EFAIL;

  pthread_mutex_lock(&hndl->lock);

  while(1) {
    if( hndl->count < hndl->len ) {
      hndl->queue[hndl->curWr] = value;
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


int OSA_queGet(OSA_QueHndl *hndl, Int32 *value, Uint32 timeout)
{
  int status = OSA_EFAIL;
  
  pthread_mutex_lock(&hndl->lock);
  
  while(1) {
    if(hndl->count > 0 ) {

      if(value!=NULL) {
        *value = hndl->queue[hndl->curRd];
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



