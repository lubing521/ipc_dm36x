#include <stdio.h>
#include <csl_hndl.h>


int CSL_sysTestMain(int argc, char **argv)
{
    int status = CSL_sysInit();
    if (status != CSL_SOK)
    {
        CSL_sysExit();
        return status;
    }

    status = CSL_vpssLock(&gCSL_vpssHndl, CSL_SYS_TIMEOUT_FOREVER);
    printf("CSL_vpssLock %d\n", status);
    status = CSL_vpssUnlock(&gCSL_vpssHndl);    
    printf("CSL_vpssUnlock %d\n", status);

    status = CSL_rszLock(&gCSL_rszHndl, CSL_SYS_TIMEOUT_FOREVER);
    printf("CSL_rszLock %d\n", status);
    status = CSL_rszUnlock(&gCSL_rszHndl);    
    printf("CSL_rszUnlock %d\n", status);
    status = CSL_ipipeLock(&gCSL_ipipeHndl, CSL_SYS_TIMEOUT_FOREVER);
    printf("CSL_ipipeLock %d\n", status);
    status = CSL_ipipeUnlock(&gCSL_ipipeHndl);    
    printf("CSL_ipipeUnlock %d\n", status);

    status = CSL_ccdcLock(&gCSL_ccdcHndl, CSL_SYS_TIMEOUT_FOREVER);
    printf("CSL_ccdcLock %d\n", status);
    status = CSL_ccdcUnlock(&gCSL_ccdcHndl);    
    printf("CSL_ccdcUnlock %d\n", status);

    status = CSL_ipipeifLock(&gCSL_ipipeifHndl, CSL_SYS_TIMEOUT_FOREVER);
    printf("CSL_ipipeifLock %d\n", status);
    status = CSL_ipipeifUnlock(&gCSL_ipipeifHndl); 
    printf("CSL_ipipeifUnlock %d\n", status);   

    status = CSL_h3aLock(&gCSL_h3aHndl, CSL_SYS_TIMEOUT_FOREVER);
    printf("CSL_h3aLock %d\n", status);
    status = CSL_h3aUnlock(&gCSL_h3aHndl);    
    printf("CSL_h3aUnlock %d\n", status);

    status = CSL_ldcLock(&gCSL_ldcHndl, CSL_SYS_TIMEOUT_FOREVER);
    printf("CSL_ldcLock %d\n", status);
    status = CSL_ldcUnlock(&gCSL_ldcHndl);    
    printf("CSL_ldcUnlock %d\n", status);

    status = CSL_faceDetectLock(&gCSL_faceDetectHndl, CSL_SYS_TIMEOUT_FOREVER);
    printf("CSL_faceDetectLock %d\n", status);
    status = CSL_faceDetectUnlock(&gCSL_faceDetectHndl);    
    printf("CSL_faceDetectUnlock %d\n", status);

    CSL_sysExit();

    return status;
}

