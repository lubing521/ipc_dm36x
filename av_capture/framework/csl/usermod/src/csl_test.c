#include <csl_hndl.h>


int CSL_sysTestMain(int argc, char **argv)
{
  int status;
  
  status = CSL_sysInit();
  if(status!=CSL_SOK) {
    CSL_sysExit();
    return status;
  }
  
  status = CSL_vpssLock(&gCSL_vpssHndl, CSL_SYS_TIMEOUT_FOREVER);
  status = CSL_vpssUnlock(&gCSL_vpssHndl);    
    
  status = CSL_rszLock(&gCSL_rszHndl, CSL_SYS_TIMEOUT_FOREVER);
  status = CSL_rszUnlock(&gCSL_rszHndl);    

  status = CSL_ipipeLock(&gCSL_ipipeHndl, CSL_SYS_TIMEOUT_FOREVER);
  status = CSL_ipipeUnlock(&gCSL_ipipeHndl);    

  status = CSL_ccdcLock(&gCSL_ccdcHndl, CSL_SYS_TIMEOUT_FOREVER);
  status = CSL_ccdcUnlock(&gCSL_ccdcHndl);    

  status = CSL_ipipeifLock(&gCSL_ipipeifHndl, CSL_SYS_TIMEOUT_FOREVER);
  status = CSL_ipipeifUnlock(&gCSL_ipipeifHndl);    

  status = CSL_h3aLock(&gCSL_h3aHndl, CSL_SYS_TIMEOUT_FOREVER);
  status = CSL_h3aUnlock(&gCSL_h3aHndl);    

  status = CSL_ldcLock(&gCSL_ldcHndl, CSL_SYS_TIMEOUT_FOREVER);
  status = CSL_ldcUnlock(&gCSL_ldcHndl);    

  status = CSL_faceDetectLock(&gCSL_faceDetectHndl, CSL_SYS_TIMEOUT_FOREVER);
  status = CSL_faceDetectUnlock(&gCSL_faceDetectHndl);    

  CSL_sysExit();
  
  return status;
}

