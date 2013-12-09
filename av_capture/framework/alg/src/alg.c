
#include <alg_priv.h>
#include <osa_cmem.h>
#include <vicplib.h>

Engine_Handle gALG_hEngine = NULL;
OSA_MutexHndl vnf_imcop_lock;
OSA_MutexHndl vicp_imcop_lock;

static CPIS_Init vicpInit;
static int numInit = 0;

int VICPLIB_init()
{
    if (numInit == 0)
    {
        vicpInit.cacheWbInv        = (Cache_wbInv)CMEM_cacheWb;
        vicpInit.staticDmaAlloc    = 1;
        vicpInit.maxNumProcFunc    = 1;
        vicpInit.memSize           = CPIS_getMemSize(vicpInit.maxNumProcFunc);
        vicpInit.mem               = OSA_cmemAlloc(vicpInit.memSize, NULL);
        vicpInit.initFC            = 0;
        vicpInit.lock              = &OSA_mutexLock;
        vicpInit.lockArg           = &vicp_imcop_lock;
        vicpInit.unlock            = &OSA_mutexUnlock;
        vicpInit.unlockArg         = &vicp_imcop_lock;
        vicpInit.maxNumDma         = 6;
        vicpInit.codecEngineHandle = (void*)gALG_hEngine;
        if (CPIS_init(&vicpInit) == -1)
        {
            return -1;
        };
    }
    numInit++;

    return 0;
}

int VICPLIB_deinit()
{
    numInit--;
    if (numInit == 0)
    {
        OSA_cmemFree(vicpInit.mem);
        return(CPIS_deInit());
    }
    return 0;
}

int ALG_sysInit()
{
    IRESMAN_Edma3ChanParams configParams;
    IRESMAN_VicpParams      iresmanConfigParams;
    IRESMAN_HdVicpParams    hdvicpConfigParams;
    IRESMAN_AddrSpaceParams addrspaceConfigParams;
    int status = OSA_SOK;

    CERuntime_init();

    /* Reset, load, and start DSP Engine */
    gALG_hEngine = Engine_open(ALG_CODEC_ENGINE_NAME, NULL, NULL);

    if (gALG_hEngine == NULL)
    {
        OSA_ERROR("Engine_open(%s)\n", ALG_CODEC_ENGINE_NAME);
        return OSA_EFAIL;
    }

    if (RMAN_init() != IRES_OK)
    {
        OSA_ERROR("RMAN_init()\n");
        Engine_close(gALG_hEngine);
        return OSA_EFAIL;
    }

    //Initialize IRES params.
    iresmanConfigParams.baseConfig.allocFxn = RMAN_PARAMS.allocFxn;
    iresmanConfigParams.baseConfig.freeFxn  = RMAN_PARAMS.freeFxn;
    iresmanConfigParams.baseConfig.size     = sizeof(IRESMAN_VicpParams);

    if (RMAN_register(&IRESMAN_VICP2, (IRESMAN_Params *)&iresmanConfigParams) != IRES_OK)
    {
        OSA_ERROR("VICP Protocol regn failed\n");
        status = OSA_EFAIL;
        goto error_exit;
    }

    configParams.baseConfig.allocFxn = RMAN_PARAMS.allocFxn;
    configParams.baseConfig.freeFxn  = RMAN_PARAMS.freeFxn;
    configParams.baseConfig.size     = sizeof(IRESMAN_Edma3ChanParams);

    if (RMAN_register(&IRESMAN_EDMA3CHAN, (IRESMAN_Params *)&configParams) != IRES_OK)
    {
        OSA_ERROR("EDMA3 Protocol regn failed\n");
        status = OSA_EFAIL;
        goto error_exit;
    }

    hdvicpConfigParams.baseConfig.allocFxn = RMAN_PARAMS.allocFxn;
    hdvicpConfigParams.baseConfig.freeFxn  = RMAN_PARAMS.freeFxn;
    hdvicpConfigParams.baseConfig.size     = sizeof(IRESMAN_HdVicpParams);
    hdvicpConfigParams.numResources        = 1;
    if (RMAN_register(&IRESMAN_HDVICP, (IRESMAN_Params *)&hdvicpConfigParams) != IRES_OK)
    {
        OSA_ERROR("HDVICP Protocol Registration Failed \n");
        status = OSA_EFAIL;
        goto error_exit;
    }

    addrspaceConfigParams.baseConfig.allocFxn = RMAN_PARAMS.allocFxn;
    addrspaceConfigParams.baseConfig.freeFxn  = RMAN_PARAMS.freeFxn;
    addrspaceConfigParams.baseConfig.size     = sizeof(IRESMAN_AddrSpaceParams);;
    if (RMAN_register(&IRESMAN_ADDRSPACE, (IRESMAN_Params*)&addrspaceConfigParams) != IRES_OK)
    {
        OSA_ERROR("ADDRSPACE Protocol Registration Failed \n");
        status = OSA_EFAIL;
        goto error_exit;
    }

    if (OSA_mutexCreate(&vnf_imcop_lock) != 0)
    {
        OSA_ERROR("VNF IMCOP LOCK FAILED \n");
        status = OSA_EFAIL;
        goto error_exit;
    }

    if (OSA_mutexCreate(&vicp_imcop_lock) != 0)
    {
        OSA_ERROR("VICP IMCOP LOCK FAILED \n");
        status = OSA_EFAIL;
        goto error_exit;
    }

    if (VICPLIB_init() != 0)
    {
        OSA_ERROR("VICPLIB_init() FAILED \n");
        status = OSA_EFAIL;
        goto error_exit;
    }

error_exit:
    if (status != OSA_SOK)
    {
        Engine_close(gALG_hEngine);
        VICPLIB_deinit();
        RMAN_unregister(&IRESMAN_HDVICP);
        RMAN_unregister(&IRESMAN_EDMA3CHAN);
        RMAN_unregister(&IRESMAN_VICP2);
        RMAN_unregister(&IRESMAN_ADDRSPACE);
        RMAN_exit();
        OSA_mutexDelete(&vnf_imcop_lock);
        OSA_mutexDelete(&vicp_imcop_lock);
    }
    return status;
}

int ALG_sysExit()
{
    if (gALG_hEngine)
    {
        Engine_close(gALG_hEngine);
        gALG_hEngine = NULL;
    }

    VICPLIB_deinit();

    RMAN_unregister(&IRESMAN_HDVICP);
    RMAN_unregister(&IRESMAN_EDMA3CHAN);
    RMAN_unregister(&IRESMAN_VICP2);
    RMAN_unregister(&IRESMAN_ADDRSPACE);
    RMAN_exit();

    OSA_mutexDelete(&vnf_imcop_lock);
    OSA_mutexDelete(&vicp_imcop_lock);

    return OSA_SOK;
}

int ALG_sysTestMain(int argc, char **argv)
{
    int status = DRV_init();
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_init()\n");
        return status;
    }

    status = ALG_sysInit();
    if (status == OSA_SOK)
    {
        ALG_sysExit();
    }
    DRV_exit();

    return status;
}

