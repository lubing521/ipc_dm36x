/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_sys.h>
#include <asm/semaphore.h>

CSL_Status CSL_sysMutexCreate(CSL_SysMutex * hndl)
{
  struct semaphore *pSem;

  if (hndl == NULL)
    return CSL_EFAIL;

  hndl->mutex = NULL;

  pSem = CSL_sysMemAlloc(sizeof(*pSem));

  if (pSem == NULL)
    return CSL_EFAIL;

  init_MUTEX(pSem);

  hndl->mutex = pSem;

  return CSL_SOK;
}

CSL_Status CSL_sysMutexLock(CSL_SysMutex * hndl, Uint32 timeout)
{
  struct semaphore *pSem;

  if (hndl == NULL)
    return CSL_EFAIL;

  pSem = (struct semaphore *) hndl->mutex;

  if (pSem == NULL)
    return CSL_EFAIL;

  if (timeout == CSL_SYS_TIMEOUT_NONE) {

    if (down_trylock(pSem))
      return CSL_EFAIL;

  } else {

    if (down_interruptible(pSem))
      return CSL_EFAIL;

  }

  return CSL_SOK;
}

CSL_Status CSL_sysMutexUnlock(CSL_SysMutex * hndl)
{
  struct semaphore *pSem;

  if (hndl == NULL)
    return CSL_EFAIL;

  pSem = (struct semaphore *) hndl->mutex;

  if (pSem == NULL)
    return CSL_EFAIL;

  up(pSem);

  return CSL_SOK;
}

CSL_Status CSL_sysMutexDelete(CSL_SysMutex * hndl)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  if (hndl->mutex != NULL)
    CSL_sysMemFree(hndl->mutex);
  return CSL_SOK;
}
