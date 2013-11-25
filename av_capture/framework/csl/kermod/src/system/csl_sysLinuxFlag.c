/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_sys.h>

CSL_Status CSL_sysFlagCreate(CSL_SysFlag * hndl)
{
  struct completion *pFlag; 

  if (hndl == NULL)
    return CSL_EFAIL;

  hndl->flag = NULL;

  pFlag = CSL_sysMemAlloc(sizeof(*pFlag));

  if (pFlag == NULL)
    return CSL_EFAIL;

  init_completion(pFlag);

  hndl->flag = pFlag;

  return CSL_SOK;
}

CSL_Status CSL_sysFlagClear(CSL_SysFlag * hndl)
{
  struct completion *pFlag;

  if (hndl == NULL)
    return CSL_EFAIL;

  pFlag = (struct completion *) hndl->flag;

  if (pFlag == NULL)
    return CSL_EFAIL;

  INIT_COMPLETION(*pFlag);
  
  return CSL_SOK;
}

CSL_Status CSL_sysFlagSet(CSL_SysFlag * hndl)
{
  struct completion *pFlag;

  if (hndl == NULL)
    return CSL_EFAIL;

  pFlag = (struct completion *) hndl->flag;

  if (pFlag == NULL)
    return CSL_EFAIL;

  complete(pFlag);

  return CSL_SOK;
}

CSL_Status CSL_sysFlagWait(CSL_SysFlag * hndl, Uint32 timeout)
{
  struct completion *pFlag;

  if (hndl == NULL)
    return CSL_EFAIL;

  pFlag = (struct completion *) hndl->flag;

  if (pFlag == NULL)
    return CSL_EFAIL;

  if (timeout != CSL_SYS_TIMEOUT_FOREVER) {
  
    timeout /= 10; // convert it to msecs

    if(wait_for_completion_interruptible_timeout(pFlag, timeout))
      return CSL_EFAIL;
      
  } else {

    if ( wait_for_completion_interruptible(pFlag) )
      return CSL_EFAIL;
      
  }

  INIT_COMPLETION(*pFlag);      

  return CSL_SOK;
}

CSL_Status CSL_sysFlagDelete(CSL_SysFlag * hndl)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  if (hndl->flag != NULL)
    CSL_sysMemFree(hndl->flag);
  return CSL_SOK;
}
