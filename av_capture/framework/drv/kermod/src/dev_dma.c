
#include <dev_dma_priv.h>

DMA_Dev gDMA_dev;

void DMA_dumpPARAM(const char *message, int channel)
{
    struct paramentry_descriptor tempParamentry;

    davinci_get_dma_params(channel, &tempParamentry);
    printk( KERN_INFO "\n%s - PaRAM(%d)\n", message, channel);
    printk( KERN_INFO"  OPT     - 0x%.8X\n", tempParamentry.opt);
    printk( KERN_INFO"  SRC     - 0x%.8X\n", tempParamentry.src);
    printk( KERN_INFO"  BCNT    - 0x%.4X, ACNT - 0x%.4X\n", 
             tempParamentry.a_b_cnt >> 16, 
             tempParamentry.a_b_cnt & 0xffff);
    printk( KERN_INFO"  DST     - 0x%.8X\n", tempParamentry.dst);
    printk( KERN_INFO"  DSTBIDX - 0x%.4X, SRCBIDX - 0x%.4X\n", 
             tempParamentry.src_dst_bidx >> 16, 
             tempParamentry.src_dst_bidx & 0xffff);
    printk( KERN_INFO"  BCNTRLD - 0x%.4X, LINK - 0x%.4X\n", 
             tempParamentry.link_bcntrld >> 16, 
             tempParamentry.link_bcntrld & 0xffff);
    printk( KERN_INFO"  DSTCIDX - 0x%.4X, SRCCIDX - 0x%.4X\n", 
             tempParamentry.src_dst_cidx >> 16, 
             tempParamentry.src_dst_cidx & 0xffff);
    printk( KERN_INFO"  CCNT    - 0x%.4X\n", tempParamentry.ccnt);
}/* dump_param_entry() */

unsigned long DMA_getPhysAddr(unsigned long virtp)
{
  pgd_t *pgd;
  pmd_t *pmd;
  pte_t *pte;
  struct mm_struct *mm = current->mm;

  pgd = pgd_offset(mm, virtp);
  if (!(pgd_none(*pgd) || pgd_bad(*pgd))) {
      pmd = pmd_offset(pgd, virtp);

      if (!(pmd_none(*pmd) || pmd_bad(*pmd))) {
          pte = pte_offset_kernel(pmd, virtp);

          if (pte_present(*pte)) {
              return __pa(page_address(pte_page(*pte)) +
                          (virtp & ~PAGE_MASK));
          }
      }
  }

  return 0;
}

int DMA_devOpen(struct inode *inode, struct file *filp)
{
  int status=0;
  int minor, major;

  minor = iminor(inode);
  major = imajor(inode);

#ifdef DMA_DEBUG
  printk(KERN_INFO "DMA: DMA_devOpen()   , %4d, %2d \n", major, minor);
#endif

  return status;                /* success */
}

int DMA_devRelease(struct inode *inode, struct file *filp)
{
  int i;
  DMA_OpenClosePrm openClosePrm;
  
#ifdef DMA_DEBUG
  printk(KERN_INFO "DMA: DMA_devRelease()\n");
#endif

  for(i=0; i<DMA_DEV_MAX_CH; i++)
  {
    if(gDMA_dev.pObj[i]!=NULL) 
    {
      openClosePrm.chId = i;
      openClosePrm.mode = gDMA_dev.pObj[i]->dmaMode;           
      openClosePrm.maxTransfers = 0;
      if(openClosePrm.mode==DRV_DMA_MODE_NORMAL) {
        DMA_copyFillDelete(&openClosePrm);        
      } else 
      if(openClosePrm.mode==DRV_DMA_MODE_DEMUX) {
        DMA_demuxDelete(&openClosePrm);
      } else {
        FUNCERR( "Illegal parameter (chId = %d)\n", openClosePrm.chId);                                
      }
    }
  }

  return 0;
}

int DMA_devIoctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
  int status=0;
  DMA_Obj *pObj;
  
  DMA_OpenClosePrm openClosePrm;
  DMA_GetPhysAddrPrm getPhysAddrPrm;
  
#ifdef DMA_DEBUG
  printk(KERN_INFO "DMA: DMA_devIoctl()\n");
#endif

  if(!DMA_IOCTL_CMD_IS_VALID(cmd))
    return -1;

  cmd = DMA_IOCTL_CMD_GET(cmd);
  
  switch(cmd)
  {
    case DMA_CMD_CH_OPEN:
    
      down_interruptible(&gDMA_dev.semLock);
    
      status = copy_from_user(&openClosePrm, (void*)arg, sizeof(openClosePrm));
      if(status==0) {
        if(openClosePrm.mode==DRV_DMA_MODE_NORMAL) {
          status = DMA_copyFillCreate(&openClosePrm);        
        } else 
        if(openClosePrm.mode==DRV_DMA_MODE_DEMUX) {
          status = DMA_demuxCreate(&openClosePrm);
        } else {
          FUNCERR( "Illegal parameter\n");            
          status = -1;
        }
        
        if(status==0) {
          status = copy_to_user((void*)arg, &openClosePrm, sizeof(openClosePrm));
        }
      }
      
      up(&gDMA_dev.semLock);      
      break;
      
    case DMA_CMD_CH_CLOSE:
    
      down_interruptible(&gDMA_dev.semLock);
          
      status = copy_from_user(&openClosePrm, (void*)arg, sizeof(openClosePrm));
      if(status==0) {
        if(openClosePrm.chId>=DMA_DEV_MAX_CH) {
          FUNCERR( "Illegal parameter (chId = %d)\n", openClosePrm.chId);                    
          status = -1;
        }
          
        if(status==0) {
          pObj = gDMA_dev.pObj[openClosePrm.chId];
          if(pObj==NULL) {
            FUNCERR( "Illegal parameter (chId = %d)\n", openClosePrm.chId);                              
            status=-1;
          }
          
          if(status==0) { 
            openClosePrm.mode = pObj->dmaMode;           
            if(openClosePrm.mode==DRV_DMA_MODE_NORMAL) {
              status = DMA_copyFillDelete(&openClosePrm);        
            } else 
            if(openClosePrm.mode==DRV_DMA_MODE_DEMUX) {
              status = DMA_demuxDelete(&openClosePrm);
            } else {
              FUNCERR( "Illegal parameter (chId = %d)\n", openClosePrm.chId);                                
              status = -1;
            }
          }
        }  
      }
      
      up(&gDMA_dev.semLock);      
      break;      

    case DMA_CMD_GET_PHYS_ADDR:
    
      down_interruptible(&gDMA_dev.semLock);
    
      status = copy_from_user(&getPhysAddrPrm, (void*)arg, sizeof(getPhysAddrPrm));
      if(status==0) {
      
        getPhysAddrPrm.physAddr = DMA_getPhysAddr(getPhysAddrPrm.virtAddr);

        status = copy_to_user((void*)arg, &getPhysAddrPrm, sizeof(getPhysAddrPrm));
      }
      
      up(&gDMA_dev.semLock);      
    
      break;      

    case DMA_CMD_COPY_2D:
    case DMA_CMD_COPY_1D:
    case DMA_CMD_FILL_2D:
    case DMA_CMD_FILL_1D:
      status = DMA_copyFillRun( (DMA_CopyFillPrm *)arg);
      break;
      
    case DMA_CMD_DEMUX:
      status = DMA_demuxRun( (DMA_DemuxPrm*)arg );      
      break;
      
    default:
      status = -1;
      break;    
  }

  return status;
}


struct file_operations gDMA_devFileOps = {
  .owner = THIS_MODULE,
  .open = DMA_devOpen,
  .release = DMA_devRelease,
  .ioctl = DMA_devIoctl,
};

int DMA_devInit(void)
{
  int     result, i;
  dev_t   dev = 0;

#ifdef DMA_DEBUG
  printk(KERN_INFO "DMA: DMA_devInit() \n");
#endif

  result = alloc_chrdev_region(&dev, 0, 1, DMA_DRV_NAME);

  if (result < 0) {
    FUNCERR( "DMA: can't get device major num \n");
    return result;
  }
  
  for(i=0; i<DMA_DEV_MAX_CH; i++)
  {
    gDMA_dev.pObj[i]=NULL;
  }

  gDMA_dev.major = MAJOR(dev);
  
  sema_init(&gDMA_dev.semLock, 1);

  cdev_init(&gDMA_dev.cdev, &gDMA_devFileOps);

  gDMA_dev.cdev.owner = THIS_MODULE;
  gDMA_dev.cdev.ops = &gDMA_devFileOps;

  result = cdev_add(&gDMA_dev.cdev, dev, 1);

  if (result)
    FUNCERR( "DMA: Error [%d] while adding device [%s] \n", result, DMA_DRV_NAME);

  printk(KERN_INFO "DMA: Module install successful, device major num = %d \n", gDMA_dev.major);

  return result;
}

void DMA_devExit(void)
{
  dev_t   devno = MKDEV(gDMA_dev.major, 0);
  int i;
  
#ifdef DMA_DEBUG
  printk(KERN_INFO "DMA: DMA_devExit() \n");
#endif

  cdev_del(&gDMA_dev.cdev);

  unregister_chrdev_region(devno, 1);

  //Anshuman - Removing it as it is no more needed after FC bug fix
#if 0  
  for(i=0; i<EDMA_MAX_PARAM_SET; i++)
    davinci_free_dma(i);

  for(i=EDMA_QDMA_CHANNEL_0; i<=EDMA_QDMA_CHANNEL_7; i++)    
    davinci_free_dma(i);
#endif  
}



