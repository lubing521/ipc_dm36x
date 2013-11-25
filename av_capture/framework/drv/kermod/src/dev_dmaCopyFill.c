
#include <dev_dma_priv.h>

int DMA_copyFill1D(  DMA_CopyFillObj *pCopyFillObj)
{
  int i;
  unsigned int aCnt, bCnt, cCnt;
  unsigned int remainSize, transferSize;
  unsigned int *pFill32;
  int status, k;

  for(i=0; i<pCopyFillObj->count; i++) {

    if(pCopyFillObj->copyFillType==DMA_CMD_FILL_1D) {

      pFill32 =  (unsigned int *)pCopyFillObj->transferPrm[i].dstVirtAddr;

      for(k=0; k<8; k++) {
        status = copy_to_user(pFill32, &pCopyFillObj->transferPrm[i].fillValue, sizeof(unsigned int));
        if(status<0) {
          FUNCERR("copy_to_user()\n");
          return -1;
        }
        pFill32++;
      }

      pCopyFillObj->transferPrm[i].srcPhysAddr = pCopyFillObj->transferPrm[i].dstPhysAddr;
    }

    if(pCopyFillObj->copyFillType==DMA_CMD_FILL_1D) {
      aCnt = 32;
      bCnt = 512;
    } else {
      aCnt = 32;
      bCnt = 512;
    }

    transferSize = 0;
    remainSize = pCopyFillObj->transferPrm[i].size;

    if(remainSize > aCnt*bCnt )
    {
      cCnt = remainSize /(aCnt*bCnt);

      pCopyFillObj->dmaParam.src = (uint32_t) pCopyFillObj->transferPrm[i].srcPhysAddr;
      pCopyFillObj->dmaParam.dst = (uint32_t) pCopyFillObj->transferPrm[i].dstPhysAddr;
      pCopyFillObj->dmaParam.link_bcntrld = DMA_PARAM_NO_LINK | (bCnt <<16);

      pCopyFillObj->dmaParam.a_b_cnt = (uint32_t)(bCnt<<16)|aCnt;

      pCopyFillObj->dmaParam.ccnt = cCnt;
      pCopyFillObj->dmaParam.opt = DMA_PARAM_OPT_TCINTEN | DMA_PARAM_OPT_SYNCDIM_AB |
                                        ((pCopyFillObj->chId&0x3f)<<12)
                                        |DMA_PARAM_OPT_ITCCHEN
                                        ;

      if(pCopyFillObj->copyFillType==DMA_CMD_FILL_1D) {
        pCopyFillObj->dmaParam.src_dst_cidx = (0) | (aCnt*bCnt << 16);
        pCopyFillObj->dmaParam.src_dst_bidx = (0) | (aCnt<<16);
      } else {
        pCopyFillObj->dmaParam.src_dst_cidx = (aCnt*bCnt) | (aCnt*bCnt << 16);
        pCopyFillObj->dmaParam.src_dst_bidx = aCnt | (aCnt<<16);
      }

      davinci_set_dma_params( pCopyFillObj->chId, &pCopyFillObj->dmaParam );

      #ifdef DMA_DEBUG
      DMA_dumpPARAM("CopyFill1D", pCopyFillObj->chId);
      #endif
      #if 1
      davinci_start_dma(pCopyFillObj->chId);

      wait_for_completion_interruptible(&pCopyFillObj->dma_complete);
      INIT_COMPLETION(pCopyFillObj->dma_complete);
      #endif

      transferSize = aCnt*bCnt*cCnt;
      remainSize = remainSize - transferSize;
    }

    if(remainSize>0) {

      if(pCopyFillObj->copyFillType==DMA_CMD_FILL_1D) {
        aCnt = 4;
        bCnt = remainSize/aCnt;
      } else {
        aCnt = remainSize;
        bCnt = 1;
      }
      cCnt = 1;

      if(pCopyFillObj->copyFillType==DMA_CMD_FILL_1D) {
        pCopyFillObj->dmaParam.src = (uint32_t) pCopyFillObj->transferPrm[i].srcPhysAddr;
      } else {
        pCopyFillObj->dmaParam.src = (uint32_t) pCopyFillObj->transferPrm[i].srcPhysAddr+transferSize;
      }

      pCopyFillObj->dmaParam.dst = (uint32_t) pCopyFillObj->transferPrm[i].dstPhysAddr+transferSize;
      pCopyFillObj->dmaParam.link_bcntrld = DMA_PARAM_NO_LINK | (bCnt <<16);

      pCopyFillObj->dmaParam.a_b_cnt = (uint32_t)(bCnt<<16)|aCnt;

      pCopyFillObj->dmaParam.ccnt = cCnt;
      pCopyFillObj->dmaParam.opt = DMA_PARAM_OPT_TCINTEN | DMA_PARAM_OPT_SYNCDIM_AB |
                                        ((pCopyFillObj->chId&0x3f)<<12)
                                        ;

      if(pCopyFillObj->copyFillType==DMA_CMD_FILL_1D) {
        pCopyFillObj->dmaParam.src_dst_cidx = (0) | (aCnt*bCnt << 16);
        pCopyFillObj->dmaParam.src_dst_bidx = (0) | (aCnt<<16);
      } else {
        pCopyFillObj->dmaParam.src_dst_cidx = (aCnt*bCnt) | (aCnt*bCnt << 16);
        pCopyFillObj->dmaParam.src_dst_bidx = aCnt | (aCnt<<16);
      }

      davinci_set_dma_params( pCopyFillObj->chId, &pCopyFillObj->dmaParam );

      #ifdef DMA_DEBUG
      DMA_dumpPARAM("CopyFill1D", pCopyFillObj->chId);
      #endif

      #if 1
      davinci_start_dma(pCopyFillObj->chId);

      wait_for_completion_interruptible(&pCopyFillObj->dma_complete);
      INIT_COMPLETION(pCopyFillObj->dma_complete);
      #endif

      transferSize = aCnt*bCnt*cCnt;
      remainSize = remainSize - transferSize;
    }
  }

  return 0;
}

int DMA_copyFill2D(  DMA_CopyFillObj *pCopyFillObj)
{
  int i, status, k;
  unsigned int aCnt, bCnt;
  unsigned int *pFill32;

  for(i=0; i<pCopyFillObj->count; i++) {

    if(pCopyFillObj->copyFillType==DMA_CMD_FILL_2D) {
      pFill32 =  (unsigned int *)pCopyFillObj->transferPrm[i].dstVirtAddr;

      for(k=0; k<4; k++) {
        status = copy_to_user(pFill32, &pCopyFillObj->transferPrm[i].fillValue, sizeof(unsigned int));
        if(status<0) {
          FUNCERR("copy_to_user()\n");
          return -1;
        }
        pFill32++;
      }

      pCopyFillObj->transferPrm[i].srcPhysAddr = pCopyFillObj->transferPrm[i].dstPhysAddr;
    }

    pCopyFillObj->dmaParam.src = (uint32_t) pCopyFillObj->transferPrm[i].srcPhysAddr;
    pCopyFillObj->dmaParam.dst = (uint32_t) pCopyFillObj->transferPrm[i].dstPhysAddr;

    pCopyFillObj->dmaParam.opt = DMA_PARAM_OPT_TCINTEN | DMA_PARAM_OPT_SYNCDIM_AB |
                                ((pCopyFillObj->chId&0x3f)<<12);

    if(pCopyFillObj->copyFillType==DMA_CMD_FILL_2D) {

      aCnt = 8;
      bCnt = (pCopyFillObj->transferPrm[i].copyWidth+aCnt-1)/aCnt;

      pCopyFillObj->dmaParam.link_bcntrld = DMA_PARAM_NO_LINK | (bCnt << 16);
      pCopyFillObj->dmaParam.src_dst_cidx = (pCopyFillObj->transferPrm[i].dstOffsetH<<16)|(0);
      pCopyFillObj->dmaParam.ccnt = pCopyFillObj->transferPrm[i].copyHeight;

      pCopyFillObj->dmaParam.opt |= DMA_PARAM_OPT_ITCCHEN;

      pCopyFillObj->dmaParam.src_dst_bidx = (aCnt<<16) | (0);

      pCopyFillObj->dmaParam.a_b_cnt = (uint32_t)(bCnt<<16)|aCnt;

    } else {

      if(pCopyFillObj->transferPrm[i].skipH == 0 ) {

        pCopyFillObj->dmaParam.link_bcntrld = DMA_PARAM_NO_LINK;
        pCopyFillObj->dmaParam.src_dst_cidx = 0;
        pCopyFillObj->dmaParam.ccnt = 1;

        pCopyFillObj->dmaParam.src_dst_bidx = (pCopyFillObj->transferPrm[i].dstOffsetH<<16)|pCopyFillObj->transferPrm[i].srcOffsetH;
        pCopyFillObj->dmaParam.a_b_cnt = (uint32_t)(pCopyFillObj->transferPrm[i].copyHeight<<16)|
                                          pCopyFillObj->transferPrm[i].copyWidth;
      } else {

        aCnt = pCopyFillObj->transferPrm[i].skipH;
        bCnt = (pCopyFillObj->transferPrm[i].copyWidth)/aCnt;

        pCopyFillObj->dmaParam.link_bcntrld = DMA_PARAM_NO_LINK | (bCnt << 16);
        pCopyFillObj->dmaParam.src_dst_cidx = (pCopyFillObj->transferPrm[i].dstOffsetH<<16)|pCopyFillObj->transferPrm[i].srcOffsetH;
        pCopyFillObj->dmaParam.ccnt = pCopyFillObj->transferPrm[i].copyHeight;

        pCopyFillObj->dmaParam.opt |= DMA_PARAM_OPT_ITCCHEN;

        pCopyFillObj->dmaParam.src_dst_bidx = ((aCnt*2) << 16) | (aCnt);

        pCopyFillObj->dmaParam.a_b_cnt = (uint32_t)(bCnt<<16)|aCnt;
      }
    }

    davinci_set_dma_params( pCopyFillObj->chId, &pCopyFillObj->dmaParam );

    #ifdef DMA_DEBUG
    DMA_dumpPARAM("CopyFill2D", pCopyFillObj->chId);
    #endif

    #if 1
    davinci_start_dma(pCopyFillObj->chId);

    wait_for_completion_interruptible(&pCopyFillObj->dma_complete);
    INIT_COMPLETION(pCopyFillObj->dma_complete);
    #endif
  }

  return 0;
}

int DMA_copyFillRun( DMA_CopyFillPrm *pUserPrm )
{
  DMA_CopyFillPrm kernelPrm;
  DMA_Obj *pObj;
  DMA_CopyFillObj *pCopyFillObj;
  DRV_DmaCopy1D copy1D;
  DRV_DmaFill1D fill1D;
  DRV_DmaCopy2D copy2D;
  DRV_DmaFill2D fill2D;
  int status, i;

  status = copy_from_user(&kernelPrm, pUserPrm, sizeof(kernelPrm));
  if(status < 0) {
    FUNCERR( "copy_from_user()\n" );
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

  pCopyFillObj = pObj->copyFillObj;
  if(pCopyFillObj==NULL) {
    FUNCERR( "Illegal parameter (chId = %d)\n", kernelPrm.chId );
    return -1;
  }

  if(kernelPrm.count>pCopyFillObj->maxCount) {
    FUNCERR( "Illegal parameter (chId = %d)\n", kernelPrm.chId );
    return -1;
  }

  pCopyFillObj->count = kernelPrm.count;
  pCopyFillObj->curCount = 0;

  pCopyFillObj->copyFillType = kernelPrm.copyFillType;
  for(i=0; i<pCopyFillObj->count; i++)
  {
    switch(pCopyFillObj->copyFillType)
    {
      case DMA_CMD_COPY_1D:
        status = copy_from_user(&copy1D, kernelPrm.prm, sizeof(copy1D));
        if(status < 0) {
          FUNCERR( "copy_from_user() (chId=%d)\n", kernelPrm.chId );
          return status;
        }

        kernelPrm.prm += sizeof(copy1D);

        pCopyFillObj->transferPrm[i].srcPhysAddr = copy1D.srcPhysAddr;
        pCopyFillObj->transferPrm[i].dstPhysAddr = copy1D.dstPhysAddr;
        pCopyFillObj->transferPrm[i].copyWidth = 0;
        pCopyFillObj->transferPrm[i].copyHeight = 0;
        pCopyFillObj->transferPrm[i].srcOffsetH = 0;
        pCopyFillObj->transferPrm[i].dstOffsetH = 0;
        pCopyFillObj->transferPrm[i].skipH = 0;
        pCopyFillObj->transferPrm[i].dstVirtAddr = 0;
        pCopyFillObj->transferPrm[i].fillValue = 0;
        pCopyFillObj->transferPrm[i].size = copy1D.size;
        break;

      case DMA_CMD_COPY_2D:
        status = copy_from_user(&copy2D, kernelPrm.prm, sizeof(copy2D));
        if(status < 0) {
          FUNCERR( "copy_from_user() (chId=%d)\n", kernelPrm.chId );
          return status;
        }

        kernelPrm.prm += sizeof(copy2D);

        pCopyFillObj->transferPrm[i].srcPhysAddr = copy2D.srcPhysAddr;
        pCopyFillObj->transferPrm[i].dstPhysAddr = copy2D.dstPhysAddr;
        pCopyFillObj->transferPrm[i].copyWidth = copy2D.copyWidth;
        pCopyFillObj->transferPrm[i].copyHeight = copy2D.copyHeight;
        pCopyFillObj->transferPrm[i].srcOffsetH = copy2D.srcOffsetH;
        pCopyFillObj->transferPrm[i].dstOffsetH = copy2D.dstOffsetH;
        pCopyFillObj->transferPrm[i].skipH = copy2D.skipH;
        pCopyFillObj->transferPrm[i].dstVirtAddr = 0;
        pCopyFillObj->transferPrm[i].fillValue = 0;
        pCopyFillObj->transferPrm[i].size = 0;
        break;

      case DMA_CMD_FILL_1D:
        status = copy_from_user(&fill1D, kernelPrm.prm, sizeof(fill1D));
        if(status < 0) {
          FUNCERR( "copy_from_user() (chId=%d)\n", kernelPrm.chId );
          return status;
        }

        kernelPrm.prm += sizeof(fill1D);

        pCopyFillObj->transferPrm[i].srcPhysAddr = 0;
        pCopyFillObj->transferPrm[i].dstPhysAddr = fill1D.dstPhysAddr;
        pCopyFillObj->transferPrm[i].copyWidth = 0;
        pCopyFillObj->transferPrm[i].copyHeight = 0;
        pCopyFillObj->transferPrm[i].srcOffsetH = 0;
        pCopyFillObj->transferPrm[i].dstOffsetH = 0;
        pCopyFillObj->transferPrm[i].skipH = 0;
        pCopyFillObj->transferPrm[i].dstVirtAddr = fill1D.dstVirtAddr;
        pCopyFillObj->transferPrm[i].fillValue = fill1D.fillValue;
        pCopyFillObj->transferPrm[i].size = fill1D.size;
        break;

      case DMA_CMD_FILL_2D:
        status = copy_from_user(&fill2D, kernelPrm.prm, sizeof(fill2D));
        if(status < 0) {
          FUNCERR( "copy_from_user() (chId=%d)\n", kernelPrm.chId );
          return status;
        }

        kernelPrm.prm += sizeof(fill2D);

        pCopyFillObj->transferPrm[i].srcPhysAddr = 0;
        pCopyFillObj->transferPrm[i].dstPhysAddr = fill2D.dstPhysAddr;
        pCopyFillObj->transferPrm[i].copyWidth = fill2D.copyWidth;
        pCopyFillObj->transferPrm[i].copyHeight = fill2D.copyHeight;
        pCopyFillObj->transferPrm[i].srcOffsetH = 0;
        pCopyFillObj->transferPrm[i].dstOffsetH = fill2D.dstOffsetH;
        pCopyFillObj->transferPrm[i].skipH = 0;
        pCopyFillObj->transferPrm[i].dstVirtAddr = fill2D.dstVirtAddr;
        pCopyFillObj->transferPrm[i].fillValue = fill2D.fillValue;
        pCopyFillObj->transferPrm[i].size = 0;
        break;
      default:
        return -1;
    }
  }


  switch(pCopyFillObj->copyFillType) {
    case DMA_CMD_COPY_2D:
    case DMA_CMD_FILL_2D:
      status = DMA_copyFill2D(pCopyFillObj);
      break;
    case DMA_CMD_COPY_1D:
    case DMA_CMD_FILL_1D:
      status = DMA_copyFill1D(pCopyFillObj);
      break;

    default:
      return -1;
  }

  return status ;
}

static void DMA_copyFillCallback(int channel, u16 ch_status, void *data)
{
  DMA_CopyFillObj *pCopyFillObj=(DMA_CopyFillObj *)data;

  davinci_stop_dma(pCopyFillObj->chId);
  complete(&pCopyFillObj->dma_complete);
}

int DMA_copyFillCreate(DMA_OpenClosePrm *prm)
{
  DMA_Obj *pObj;
  int tcc;

#ifdef DMA_DEBUG
  printk(KERN_INFO "DMA: DMA_copyFillCreate() ... \n");
#endif


  pObj = kmalloc(sizeof(DMA_Obj), GFP_KERNEL);
  if(pObj==NULL) {
    FUNCERR( "Unable to alloc memory\n" );
    return -1;
  }

  pObj->dmaMode = prm->mode;

  pObj->demuxObj = NULL;

  pObj->copyFillObj = kmalloc(sizeof(DMA_CopyFillObj), GFP_KERNEL);
  if(pObj->copyFillObj==NULL)
  {
    FUNCERR( "Unable to alloc memory\n" );
    kfree(pObj);
    return -1;
  }

  memset(pObj->copyFillObj, 0, sizeof(DMA_CopyFillObj));

  pObj->copyFillObj->transferPrm = kmalloc(sizeof(DRV_CopyFillPrm)*prm->maxTransfers, GFP_KERNEL);
  if(pObj->copyFillObj->transferPrm==NULL)
  {
    FUNCERR( "Unable to alloc memory\n" );
    kfree(pObj->copyFillObj);
    kfree(pObj);
    return -1;
  }

  pObj->copyFillObj->maxCount = prm->maxTransfers;

  memset(pObj->copyFillObj->transferPrm, 0, sizeof(DRV_CopyFillPrm)*prm->maxTransfers);

  init_completion(&pObj->copyFillObj->dma_complete);

  tcc = EDMA_TCC_ANY;
  pObj->copyFillObj->chId=0;
  if (davinci_request_dma(EDMA_DMA_CHANNEL_ANY, "Memory DMA", DMA_copyFillCallback,
                          pObj->copyFillObj, &pObj->copyFillObj->chId, &tcc, DMA_NORMAL_EVENT_QUEUE))
  {
    FUNCERR( "Unable to request DMA (channel=%d).\n", pObj->copyFillObj->chId );
    kfree(pObj->copyFillObj->transferPrm);
    kfree(pObj->copyFillObj);
    kfree(pObj);
    return -EFAULT;
  }

  if(pObj->copyFillObj->chId >= DMA_DEV_MAX_CH) {

    FUNCERR( "Invalid channel ID (channel=%d).\n", pObj->copyFillObj->chId );
    davinci_free_dma( pObj->copyFillObj->chId );
    kfree(pObj->copyFillObj->transferPrm);
    kfree(pObj->copyFillObj);
    kfree(pObj);

    return -EFAULT;
  }

  gDMA_dev.pObj[pObj->copyFillObj->chId] = pObj;

  prm->chId = pObj->copyFillObj->chId;

#ifdef DMA_DEBUG
  printk(KERN_INFO "DMA: DMA_copyFillCreate() success ... chId is %d\n", prm->chId);
#endif

  return 0;
}

int DMA_copyFillDelete(DMA_OpenClosePrm *prm)
{
  DMA_Obj *pObj;

  if(prm->chId >= DMA_DEV_MAX_CH)
    return -1;

  pObj = gDMA_dev.pObj[prm->chId];
  if(pObj==NULL) {
    FUNCERR( "Illegal parameter (chId = %d)\n", prm->chId );
    return -1;
  }

  if(pObj->copyFillObj==NULL)  {
    FUNCERR( "Illegal parameter (chId = %d)\n", prm->chId );
    return -1;
  }

  davinci_stop_dma( pObj->copyFillObj->chId );
  davinci_free_dma( pObj->copyFillObj->chId );

  if(pObj->copyFillObj->transferPrm!=NULL)
    kfree(pObj->copyFillObj->transferPrm);
  kfree(pObj->copyFillObj);
  kfree(pObj);

  gDMA_dev.pObj[prm->chId] = NULL;

  return 0;
}


