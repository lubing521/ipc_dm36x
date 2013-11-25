

#include <osa_flg.h>

int  OSA_flgCreate(OSA_FlgHndl *hndl, Uint32 initPattern)
{
  pthread_mutexattr_t mutex_attr;
  pthread_condattr_t cond_attr;
  int status=OSA_SOK;
 
  status |= pthread_mutexattr_init(&mutex_attr);
  status |= pthread_condattr_init(&cond_attr);  
  
  status |= pthread_mutex_init(&hndl->lock, &mutex_attr);
  status |= pthread_cond_init(&hndl->cond, &cond_attr);  

  hndl->pattern = initPattern;

  if(status!=OSA_SOK)
    OSA_ERROR("OSA_flgCreate() = %d \r\n", status);

  pthread_condattr_destroy(&cond_attr);
  pthread_mutexattr_destroy(&mutex_attr);
    
  return status;
}

int  OSA_flgWait(OSA_FlgHndl *hndl, Uint32 pattern, Uint32 mode, Uint32 timeout)
{
  int status = OSA_EFAIL, found;
  Uint32 setPattern;

  pthread_mutex_lock(&hndl->lock);

  while(1) {

    found = 0;
    setPattern = hndl->pattern & pattern;

    if(mode & OSA_FLG_MODE_AND) {
      if( setPattern == pattern )
        found = 1;
    } else {
      if( setPattern )
        found = 1;
    }

    if(found) {
      if(mode & OSA_FLG_MODE_CLR)
        hndl->pattern &= ~setPattern;

      status = OSA_SOK;
      break;

    } else {
      if(timeout==OSA_TIMEOUT_NONE)
        break;

      pthread_cond_wait(&hndl->cond, &hndl->lock);
    }
  }

  pthread_mutex_unlock(&hndl->lock);

  return status;
}

int  OSA_flgSet(OSA_FlgHndl *hndl, Uint32 pattern)
{
  int status = OSA_SOK;

  pthread_mutex_lock(&hndl->lock);

  hndl->pattern |= pattern;
  status |= pthread_cond_signal(&hndl->cond);

  pthread_mutex_unlock(&hndl->lock);

  return status;
}

int  OSA_flgClear(OSA_FlgHndl *hndl, Uint32 pattern)
{
  int status = OSA_SOK;

  pthread_mutex_lock(&hndl->lock);

  hndl->pattern &= ~pattern;

  pthread_mutex_unlock(&hndl->lock);

  return status;
}

Bool OSA_flgIsSet(OSA_FlgHndl *hndl, Uint32 pattern)
{
  int status;
  
  status = OSA_flgWait(hndl, pattern, OSA_FLG_MODE_AND, OSA_TIMEOUT_NONE);
  
  if(status!=OSA_SOK)
    return FALSE;
    
  return TRUE;    
}

int  OSA_flgDelete(OSA_FlgHndl *hndl)
{
  pthread_cond_destroy(&hndl->cond);
  pthread_mutex_destroy(&hndl->lock);  

  return OSA_SOK;
}



