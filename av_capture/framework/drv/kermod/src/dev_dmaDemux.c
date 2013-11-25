
#include <dev_dma_priv.h>

#include <asm/arch/dma.h>

struct paramentry_descriptor *davinci_get_param_addr(int chId)
{
  if(chId < 0 || chId > EDMA_MAX_PARAM_SET) {
    FUNCERR("Invalid channel ID (chId = %d)\n", chId);
    return NULL;
  }
  
  return (struct paramentry_descriptor *)IO_ADDRESS(EDMA_PARAM_BASE + EDMA_PARAM_ENTRY_SIZE*chId);
}

int DMA_demuxRun(DMA_DemuxPrm *pUserPrm )
{
  DMA_Obj *pObj;
  DMA_DemuxObj *pDemuxObj;
  DRV_DmaDemux *prm;
  
  DMA_DemuxPrm kernelPrm;
  
  int i, status;
  
  status = copy_from_user(&kernelPrm, pUserPrm, sizeof(kernelPrm));
  if(status < 0) {
    FUNCERR( "copy_from_user()\n");                    
    return status;
  }
  
  if(kernelPrm.chId >= DMA_DEV_MAX_CH) {
    FUNCERR( "Illegal parameter (chId = %d)\n", kernelPrm.chId );  
    return -1;
  }
      
  pObj = gDMA_dev.pObj[kernelPrm.chId];
  if(pObj==NULL) {
    FUNCERR( "Illegal parameter (chId = %d)\n", kernelPrm.chId );  
    return -1;
  }
  
  pDemuxObj = pObj->demuxObj;
  if(pDemuxObj==NULL) {
    FUNCERR( "Illegal parameter (chId = %d)\n", kernelPrm.chId );    
    return -1;
  }

  prm = &pDemuxObj->demuxPrm;
    
  memcpy(prm, &kernelPrm.prm, sizeof(pDemuxObj->demuxPrm));
  
  prm->srcPhysAddrList = pDemuxObj->srcPhysAddrList;
  prm->dstPhysAddrList = pDemuxObj->dstPhysAddrList;
  
  if(prm->numLines>pDemuxObj->maxLines) {
    FUNCERR( "Illegal parameter (chId = %d)\n", kernelPrm.chId );    
    return -1;
  }

  status = copy_from_user(prm->srcPhysAddrList, kernelPrm.prm.srcPhysAddrList, sizeof(unsigned long)*prm->numLines);
  if(status < 0) {
    FUNCERR( "copy_from_user() (chId = %d)\n", kernelPrm.chId );    
    return status;
  }
  
  status = copy_from_user(prm->dstPhysAddrList, kernelPrm.prm.dstPhysAddrList, sizeof(unsigned long)*prm->numLines);
  if(status < 0) {
    FUNCERR( "copy_from_user() (chId = %d)\n", kernelPrm.chId );      
    return status;
  }

  pDemuxObj->curLine = 0;

  for(i=0; i<DRV_DMA_MAX_DEMUX_PARAM; i++) {
    pDemuxObj->dmaParamDemux[i].a_b_cnt = (uint32_t)(1<<16)|prm->copyWidth;
  }

  for(i=0; i<DRV_DMA_MAX_DEMUX_PARAM; i+=1) {
    if(pDemuxObj->curLine<prm->numLines) {
      pDemuxObj->dmaParamDemux[i].src = (uint32_t) pDemuxObj->srcPhysAddrList[pDemuxObj->curLine];
      pDemuxObj->dmaParamDemux[i].dst = (uint32_t) pDemuxObj->dstPhysAddrList[pDemuxObj->curLine];
    }
    pDemuxObj->curLine++;
  }

  for(i=0; i<DRV_DMA_MAX_DEMUX_PARAM; i++) {
    davinci_set_dma_params(pDemuxObj->channelDemux[i], &pDemuxObj->dmaParamDemux[i]);    
  }
      
  for(i=1; i<DRV_DMA_MAX_DEMUX_PARAM; i++) {
    davinci_dma_link_lch(pDemuxObj->channelDemux[i-1], pDemuxObj->channelDemux[i]);
    davinci_get_dma_params(pDemuxObj->channelDemux[i-1], &pDemuxObj->dmaParamDemux[i-1]);      
  }
  
  #ifdef DMA_DEBUG
  for(i=0; i<DRV_DMA_MAX_DEMUX_PARAM; i++) {
    DMA_dumpPARAM("Demux", pDemuxObj->channelDemux[i]);
  }  
  #endif

  #if 1
  davinci_start_dma(pDemuxObj->channelDemux[0]);

  wait_for_completion_interruptible(&pDemuxObj->dma_complete);
  
  INIT_COMPLETION(pDemuxObj->dma_complete);
  #endif
  
  return 0;
}

static void DMA_demuxCallback(int channel, u16 ch_status, void *data)
{
  DMA_DemuxObj *pDemuxObj=(DMA_DemuxObj *)data;
  
  DRV_DmaDemux *prm = &pDemuxObj->demuxPrm;
  int i;
    
  if(pDemuxObj->curLine >= prm->numLines ) {
    complete(&pDemuxObj->dma_complete);      
    return;        
  }

  for(i=0; i<DRV_DMA_MAX_DEMUX_PARAM; i+=1) {
    if(pDemuxObj->curLine<prm->numLines) {
      if(i<2) {
        pDemuxObj->dmaParamDemux[i].src = (uint32_t) prm->srcPhysAddrList[pDemuxObj->curLine];
        pDemuxObj->dmaParamDemux[i].dst = (uint32_t) prm->dstPhysAddrList[pDemuxObj->curLine];
      
      } else {
        pDemuxObj->dmaParamAddr[i]->src = (uint32_t) prm->srcPhysAddrList[pDemuxObj->curLine];
        pDemuxObj->dmaParamAddr[i]->dst = (uint32_t) prm->dstPhysAddrList[pDemuxObj->curLine];
      }        
    }
    pDemuxObj->curLine++;
  }

  for(i=0; i<2; i++) {
    davinci_set_dma_params(pDemuxObj->channelDemux[i], &pDemuxObj->dmaParamDemux[i]);    
  }

  davinci_start_dma(pDemuxObj->channelDemux[0]);    
}

int DMA_demuxCreate(DMA_OpenClosePrm *prm)
{
  DMA_Obj *pObj;
  DMA_DemuxObj *pDemuxObj;
  int tcc, i;

#ifdef DMA_DEBUG
  printk(KERN_INFO "DMA: DMA_demuxCreate() ... \n");
#endif
 
  pObj = kmalloc(sizeof(DMA_Obj), GFP_KERNEL);
  if(pObj==NULL) {
    FUNCERR( "Unable to alloc memory\n" );  
    return -1;
  }
  
  pObj->dmaMode = prm->mode;
  
  pObj->copyFillObj = NULL;
  
  pObj->demuxObj = kmalloc(sizeof(DMA_DemuxObj), GFP_KERNEL);
  if(pObj->demuxObj==NULL)
  {
    FUNCERR( "Unable to alloc memory\n" );    
    kfree(pObj);
    return -1;
  }
  
  memset(pObj->demuxObj, 0, sizeof(DMA_DemuxObj));
  
  pObj->demuxObj->srcPhysAddrList = kmalloc( sizeof(unsigned long)*prm->maxTransfers, GFP_KERNEL);
  pObj->demuxObj->dstPhysAddrList = kmalloc( sizeof(unsigned long)*prm->maxTransfers, GFP_KERNEL);  
  
  if(pObj->demuxObj->srcPhysAddrList==NULL
    || pObj->demuxObj->dstPhysAddrList==NULL
    ) {
    
    FUNCERR( "Unable to alloc memory\n" );      

    if(pObj->demuxObj->srcPhysAddrList)    
      kfree(pObj->demuxObj->srcPhysAddrList);

    if(pObj->demuxObj->dstPhysAddrList)    
      kfree(pObj->demuxObj->dstPhysAddrList);
      
    kfree(pObj->demuxObj);  
    kfree(pObj);
    
    return -1;
  }
  
  pObj->demuxObj->maxLines = prm->maxTransfers;
  
  memset(pObj->demuxObj->srcPhysAddrList, 0, sizeof(unsigned long)*prm->maxTransfers);
  memset(pObj->demuxObj->dstPhysAddrList, 0, sizeof(unsigned long)*prm->maxTransfers);  
  
  pDemuxObj = pObj->demuxObj;
  
  init_completion(&pDemuxObj->dma_complete);  
  
  tcc = EDMA_TCC_ANY;
  pDemuxObj->channelDemux[0] = 14;
  if (davinci_request_dma(EDMA_DMA_CHANNEL_ANY, "Memory DMA", DMA_demuxCallback, 
                          pDemuxObj, &pDemuxObj->channelDemux[0], &tcc, DMA_DEMUX_EVENT_QUEUE))
  {
    FUNCERR( "Unable to request DMA (channel=%d).\n", pDemuxObj->channelDemux[0] );
        
    if(pObj->demuxObj->srcPhysAddrList)    
      kfree(pObj->demuxObj->srcPhysAddrList);

    if(pObj->demuxObj->dstPhysAddrList)    
      kfree(pObj->demuxObj->dstPhysAddrList);
      
    kfree(pObj->demuxObj);  
    kfree(pObj);        
    return -EFAULT;
  }

  for(i=1; i<DRV_DMA_MAX_DEMUX_PARAM; i++) {

    pDemuxObj->channelDemux[i] = 15;
    if (davinci_request_dma(DAVINCI_EDMA_PARAM_ANY, "Memory DMA", DMA_demuxCallback, 
                            pDemuxObj, &pDemuxObj->channelDemux[i], &tcc, DMA_DEMUX_EVENT_QUEUE))
    {
      FUNCERR( "Unable to request DMA (channel=%d).\n", pDemuxObj->channelDemux[i] );
      
      // free allocated channels
      while(i) {
        i--;
        davinci_free_dma( pObj->demuxObj->channelDemux[i] );
      }
      
      if(pObj->demuxObj->srcPhysAddrList)    
        kfree(pObj->demuxObj->srcPhysAddrList);

      if(pObj->demuxObj->dstPhysAddrList)    
        kfree(pObj->demuxObj->dstPhysAddrList);
    
      kfree(pObj->demuxObj);  
      kfree(pObj);        
      return -EFAULT;
    }
  }
    
  for(i=0; i<DRV_DMA_MAX_DEMUX_PARAM; i++) {
            
    /* no link */
    pDemuxObj->dmaParamAddr[i] = davinci_get_param_addr(pDemuxObj->channelDemux[i]);
    
    pDemuxObj->dmaParamDemux[i].link_bcntrld = DMA_PARAM_NO_LINK;

    /* no support for 3 dimension transfers */
    pDemuxObj->dmaParamDemux[i].src_dst_cidx = 0;
    pDemuxObj->dmaParamDemux[i].ccnt = 1;
    pDemuxObj->dmaParamDemux[i].opt =   DMA_PARAM_OPT_SYNCDIM_AB 
                                | ((pDemuxObj->channelDemux[0] & 0x3f)<<12)
                                ;
    pDemuxObj->dmaParamDemux[i].src_dst_bidx = 0;
    
    if(i==(DRV_DMA_MAX_DEMUX_PARAM-1)) {
      pDemuxObj->dmaParamDemux[i].opt |= TCINTEN; // enable interrupt for last linked DMA
    } else {
      pDemuxObj->dmaParamDemux[i].opt |= TCCHEN;  // enable chaining for intermediate linked DMAs    
    }
    
    davinci_set_dma_params(pDemuxObj->channelDemux[i], &pDemuxObj->dmaParamDemux[i]);
  }
 
  gDMA_dev.pObj[pDemuxObj->channelDemux[0]] = pObj;
  prm->chId = pDemuxObj->channelDemux[0];
  
#ifdef DMA_DEBUG
  printk(KERN_INFO "DMA: DMA_demuxCreate() success ... chId is %d\n", prm->chId);
#endif
  
  return 0;
}


int DMA_demuxDelete(DMA_OpenClosePrm *prm)
{
  DMA_Obj *pObj;
  int i;
  
  if(prm->chId >= DMA_DEV_MAX_CH) {
    FUNCERR( "Illegal parameter (chId = %d)\n", prm->chId );    
    return -1;
  }
    
  pObj = gDMA_dev.pObj[prm->chId];
  if(pObj==NULL) {
    FUNCERR( "Illegal parameter (chId = %d)\n", prm->chId );  
    return -1;
  }
    
  if(pObj->demuxObj==NULL)  {
    FUNCERR( "Illegal parameter (chId = %d)\n", prm->chId );  
    return -1;
  }
    
  davinci_stop_dma( pObj->demuxObj->channelDemux[0] );
    
  for(i=0; i<DRV_DMA_MAX_DEMUX_PARAM; i++)
    davinci_free_dma( pObj->demuxObj->channelDemux[i] );

  if(pObj->demuxObj->srcPhysAddrList!=NULL)
    kfree(pObj->demuxObj->srcPhysAddrList);
    
  if(pObj->demuxObj->dstPhysAddrList!=NULL)
    kfree(pObj->demuxObj->dstPhysAddrList);
    
  kfree(pObj->demuxObj);
  kfree(pObj);  
  
  gDMA_dev.pObj[prm->chId] = NULL;  

  return -1;
}


