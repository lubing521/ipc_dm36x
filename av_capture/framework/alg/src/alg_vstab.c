

#include <alg_priv.h>
#include <alg_vstab.h>
#include <osa_cmem.h>
#include <drv_ipipe.h>
#include <ivs1.h>

//#define VS_CMEM_ALLOC OSA_cmemAlloc
#define VS_CMEM_ALLOC OSA_cmemAllocCached

#define ALG_BSC_DATA_SIZE_MAX   (DRV_IPIPE_BSC_BUFFER_MAX)

typedef struct {

  ALG_VstabCreate createPrm;

  IALG_Handle  vsHandle;
  void *prevBscBuf;

} ALG_VsObj;

// VS ALG delete
Void VS_ALG_delete(IALG_Handle alg)
{
    IALG_MemRec memTab[2];
    IALG_Fxns *fxns;

    fxns = alg->fxns;

    memTab[0].base = alg;

    fxns->algFree(alg, memTab);

    OSA_cmemFree(memTab[0].base);
    OSA_cmemFree(memTab[1].base);
}

// VS ALG create
IALG_Handle VS_ALG_create(IALG_Fxns *fxns,IALG_Handle p,IALG_Params *params)
{
    IALG_MemRec memTab[2];
    IALG_Handle alg;
    IALG_Fxns *fxnsPtr;
    int n;

    n = fxns->algAlloc(params, &fxnsPtr, memTab);

    if(n <= 0)
    {
        return (NULL);
    }

    // Allocate memory requested by VS algorithm
    memTab[0].base = VS_CMEM_ALLOC(memTab[0].size,32);
    memTab[1].base = VS_CMEM_ALLOC(memTab[1].size,32);

    alg = (IALG_Handle)memTab[0].base;
    alg->fxns = fxns;

    if (fxns->algInit(alg,memTab,p,params) == IALG_EOK)
    {
        return (alg);
    }
    else
    {
        VS_ALG_delete(alg);
        return (NULL);
    }
}

void *ALG_vstabCreate(ALG_VstabCreate *create)
{
  ALG_VsObj *pObj;
  IVS1_Params  vsParams;

  pObj = OSA_memAlloc(sizeof(ALG_VsObj));
  if(pObj == NULL)
  {
    return NULL;
  }

  memset(pObj, 0, sizeof(*pObj));
  memcpy(&pObj->createPrm,create,sizeof(pObj->createPrm));

  vsParams.vertImgSizeBeforeBSC           = create->totalFrameHeight;
  vsParams.horzImgSizeBeforeBSC           = create->totalFrameWidth;
  vsParams.vertDownsampleRatioInImgSensor = 1;
  vsParams.horzDownSampleRatioInImgSensor = 1;
  vsParams.minPanCoeffHorz                = 29846;           // should be between 0 - 1 in Q15 format 0.91 * 2^15 = 29846
  vsParams.maxPanCoeffHorz                = (1 * (1<<15));   // should be between 0 - 1 in Q15 format
  vsParams.minPanCoeffVert                = 29846;           // should be between 0 - 1 in Q15 format 0.91 * 2^15 = 29846
  vsParams.maxPanCoeffVert                = (1 * (1<<15));   // should be between 0 - 1 in Q15 format

  pObj->vsHandle = (IALG_Handle)VS_ALG_create((IALG_Fxns *)&VS_TI_IVS,
                                              (IALG_Handle)NULL,
                                              (IALG_Params *)&vsParams);

  // copy the BSC params
  create->bsc_row_vct   = vsParams.bsc_row_vct;
  create->bsc_row_vpos  = vsParams.bsc_row_vpos;
  create->bsc_row_vnum  = vsParams.bsc_row_vnum;
  create->bsc_row_vskip = vsParams.bsc_row_vskip;
  create->bsc_row_hpos  = vsParams.bsc_row_hpos;
  create->bsc_row_hnum  = vsParams.bsc_row_hnum;
  create->bsc_row_hskip = vsParams.bsc_row_hskip;
  create->bsc_row_shf   = vsParams.bsc_row_shf;

  create->bsc_col_vct   = vsParams.bsc_col_vct;
  create->bsc_col_vpos  = vsParams.bsc_col_vpos;
  create->bsc_col_vnum  = vsParams.bsc_col_vnum;
  create->bsc_col_vskip = vsParams.bsc_col_vskip;
  create->bsc_col_hpos  = vsParams.bsc_col_hpos;
  create->bsc_col_hnum  = vsParams.bsc_col_hnum;
  create->bsc_col_hskip = vsParams.bsc_col_hskip;
  create->bsc_col_shf   = vsParams.bsc_col_shf;

  // Allocate bsc prev buf
  pObj->prevBscBuf = VS_CMEM_ALLOC(ALG_BSC_DATA_SIZE_MAX,32);
  if(pObj->prevBscBuf == NULL)
  {
    return NULL;
  }

  return pObj;
}

int ALG_vstabRun(void *hndl, ALG_VstabRunPrm *prm, ALG_VstabStatus *status)
{
  int retVal;
  ALG_VsObj *pObj = (ALG_VsObj *)hndl;
  IVS1_InArgs  inArgs;
  IVS1_OutArgs outArgs;
  IVS1_Fxns *ivsFxns;

  ivsFxns = (IVS1_Fxns *)(pObj->vsHandle)->fxns;

  if(pObj->vsHandle == NULL || prm == NULL)
  {
    return OSA_EFAIL;
  }

  // Invalidate the buffer
  OSA_cmemCacheInv(prm->bscDataVirtAddr, ALG_BSC_DATA_SIZE_MAX);

  // Activate the alg
  (pObj->vsHandle)->fxns->algActivate(pObj->vsHandle);

  // Apply VS
  inArgs.bscBufferCurrV  = (XDAS_UInt32)prm->bscDataVirtAddr;
  inArgs.bscBufferCurrH  = (XDAS_UInt32)prm->bscDataVirtAddr + (ALG_BSC_DATA_SIZE_MAX/2);
  inArgs.bscBufferPrevV  = (XDAS_UInt32)pObj->prevBscBuf;
  inArgs.bscBufferPrevH  = (XDAS_UInt32)pObj->prevBscBuf + (ALG_BSC_DATA_SIZE_MAX/2);

  retVal = ivsFxns->process((IVS1_Handle)pObj->vsHandle,&inArgs,&outArgs);

  // Deactivate the alg
  (pObj->vsHandle)->fxns->algDeactivate(pObj->vsHandle);

  if(retVal != IVS1_EOK)
  {
	status->startX = 0;
	status->startY = 0;
    return OSA_EFAIL;
  }
  else
  {
	status->startX = (outArgs.startX * pObj->createPrm.stabFrameWidth)/outArgs.winWidth;
	status->startY = (outArgs.startY * pObj->createPrm.stabFrameHeight)/outArgs.winHeight;
  }

  // copy cur bsc data into prev bsc buf
  memcpy(pObj->prevBscBuf,prm->bscDataVirtAddr,ALG_BSC_DATA_SIZE_MAX);

  return OSA_SOK;
}

int ALG_vstabDelete(void *hndl)
{
  ALG_VsObj *pObj = (ALG_VsObj *)hndl;

  if(pObj->vsHandle == NULL)
  {
    return OSA_EFAIL;
  }

  // delete the VS alg
  VS_ALG_delete(pObj->vsHandle);

  // Free the prev bsc buf
  OSA_cmemFree(pObj->prevBscBuf);

  OSA_memFree(pObj);

  return OSA_SOK;
}

