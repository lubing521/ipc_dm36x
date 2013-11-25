/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_dma.h>
#include <csl_sysDrv.h>
#include <asm/arch/edma.h>

#define DMA_PARAM_NO_LINK          (0xFFFF)
#define DMA_PARAM_OPT_SYNCDIM_AB   (1<<2)
#define DMA_PARAM_OPT_TCINTEN      (1<<20)

CSL_Status CSL_ipipeBscBufSwitch(CSL_IpipeHandle hndl);

typedef void (*CSL_DmaCallback)(int, u16, void*);

typedef struct {

  int chId;
  struct paramentry_descriptor dmaParam;
  CSL_DmaCallback callback;
  int countDmaStart;
  int countDmaComplete;
  
} CSL_DmaObj;

CSL_DmaObj gCSL_dmaObj[CSL_DMA_MAX];

static void CSL_dmaBscCallback(int channel, u16 ch_status, void *data)
{
  davinci_stop_dma( channel );
  
  gCSL_dmaObj[CSL_DMA_IPIPE_BSC].countDmaComplete++;
  
  CSL_ipipeBscBufSwitch(&gCSL_drvIpipeObj);
}

static void CSL_dmaHistoCallback(int channel, u16 ch_status, void *data)
{
  davinci_stop_dma( channel );
}


int CSL_dmaCreate(int id)
{
  int tcc;
  CSL_DmaObj *pObj;
  Uint32 srcPhysAddr;
  Uint32 size;

  
  if(id<0 || id>=CSL_DMA_MAX)
    return -1;

  pObj = &gCSL_dmaObj[id];
  
  memset(pObj, 0, sizeof(*pObj));
  
  switch(id) {
    case CSL_DMA_IPIPE_BSC:
      pObj->callback = CSL_dmaBscCallback;
      size = 0x4000;
      srcPhysAddr = CSL_IPIPE_BSC_TBL_0_PHYS_ADDR;
      break;
    case CSL_DMA_IPIPE_HISTO:
      pObj->callback = CSL_dmaHistoCallback;
      size = 0x2000;
      srcPhysAddr = CSL_IPIPE_HST_TBL_0_PHYS_ADDR;
      break;
    default:
      return -1;      
  }
  
  pObj->countDmaComplete = 0;
  pObj->countDmaStart    = 0;  
 
  tcc = EDMA_TCC_ANY;
  pObj->chId=14;
  if (davinci_request_dma(EDMA_DMA_CHANNEL_ANY, "Memory DMA", pObj->callback, pObj, &pObj->chId, &tcc, EVENTQ_1))
  {
    printk( KERN_ERR "Unable to request DMA (channel=%d).\n", pObj->chId );
    return -EFAULT;
  }

  CSL_dmaSetPrm(id, 0, srcPhysAddr, 4, size/4);
  
  return 0;
}

int CSL_dmaDelete(int id)
{
  if(id>=CSL_DMA_MAX)
    return -1;
  
  davinci_stop_dma( gCSL_dmaObj[id].chId );
  davinci_free_dma( gCSL_dmaObj[id].chId );

  return 0;  
}

int CSL_dmaInit()
{
  int i;
  int status=0;
    
  for(i=0; i<CSL_DMA_MAX; i++)
    status |= CSL_dmaCreate(i);
    
  return status;    
}

int CSL_dmaExit()
{
  int i;
  int status=0;
    
  for(i=0; i<CSL_DMA_MAX; i++)
    status |= CSL_dmaDelete(i);
    
  return status;    
}

int CSL_dmaSetDstAddr(int id, Uint32 dstPhysAddr)
{
  if(id>=CSL_DMA_MAX)
    return -1;
    
  gCSL_dmaObj[id].dmaParam.dst = (uint32_t) dstPhysAddr;
  
  davinci_set_dma_params( gCSL_dmaObj[id].chId, &gCSL_dmaObj[id].dmaParam );  
  
  return 0;
}

int CSL_dmaStart(int id)
{
  if(id>=CSL_DMA_MAX)
    return -1;

  gCSL_dmaObj[id].countDmaStart++;
  
  davinci_start_dma(gCSL_dmaObj[id].chId);
    
  return 0;
}

int CSL_dmaPrintCount(int id)
{
  printk( KERN_INFO " CSL DMA: CH %d: start=%d, complete=%d\n", gCSL_dmaObj[id].chId, gCSL_dmaObj[id].countDmaStart, gCSL_dmaObj[id].countDmaComplete);
  
  return 0;
}

int CSL_dmaClearCount(int id)
{
  gCSL_dmaObj[id].countDmaComplete = 0;
  gCSL_dmaObj[id].countDmaStart    = 0;  
  
  return 0;
}

int CSL_dmaStop(int id)
{
  if(id>=CSL_DMA_MAX)
    return -1;

  davinci_stop_dma(gCSL_dmaObj[id].chId);
  
  return 0;
}

int CSL_dmaSetPrm(int id, Uint32 dstPhysAddr, Uint32 srcPhysAddr, Uint16 aCnt, Uint16 bCnt)
{
  if(id>=CSL_DMA_MAX)
    return -1;
    
  gCSL_dmaObj[id].dmaParam.src = (uint32_t) srcPhysAddr;
  gCSL_dmaObj[id].dmaParam.dst = (uint32_t) dstPhysAddr;
  gCSL_dmaObj[id].dmaParam.opt = DMA_PARAM_OPT_TCINTEN | DMA_PARAM_OPT_SYNCDIM_AB | ((gCSL_dmaObj[id].chId&0x3f)<<12);
  gCSL_dmaObj[id].dmaParam.link_bcntrld = DMA_PARAM_NO_LINK; 
  gCSL_dmaObj[id].dmaParam.src_dst_cidx = 0;
  gCSL_dmaObj[id].dmaParam.ccnt = 1;
  gCSL_dmaObj[id].dmaParam.src_dst_bidx = (aCnt<<16) | (aCnt);
  gCSL_dmaObj[id].dmaParam.a_b_cnt = (uint32_t)(bCnt<<16)|aCnt;

  davinci_set_dma_params( gCSL_dmaObj[id].chId, &gCSL_dmaObj[id].dmaParam );  
  
  return 0;
}

