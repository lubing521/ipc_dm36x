/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_sys.h>
#include <csl_sysDrv.h>
#include <linux/config.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>           /* everything... */
#include <linux/cdev.h>
#include <linux/mm.h>

typedef struct {

  struct cdev cdev;             /* Char device structure    */
  int     major;

} CSL_SysLinuxObj;

CSL_SysLinuxObj gCSL_sysLinuxObj;

Uint32 CSL_sysGetCurTime()
{
  struct timeval tv;

  do_gettimeofday(&tv);

  return (tv.tv_sec * 1000) + (tv.tv_usec + 500) / 1000;
}

int CSL_sysLinuxDrvOpen(struct inode *inode, struct file *filp)
{
  CSL_Status status;
  int     minor, major;

  minor = iminor(inode);
  major = imajor(inode);

#ifdef CSL_DEBUG
  printk(KERN_INFO "CSL: CSL_sysLinuxDrvOpen()   , %4d, %2d \n", major, minor);
#endif

  filp->private_data = (void *) minor;

  status = CSL_sysDrvOpen(minor);

  return status;                /* success */
}

int CSL_sysLinuxDrvRelease(struct inode *inode, struct file *filp)
{
  int     minor;

  minor = (int) filp->private_data;

#ifdef CSL_DEBUG
  printk(KERN_INFO "CSL: CSL_sysLinuxDrvRelease(), %4d, %2d \n", gCSL_sysLinuxObj.major, minor);
#endif

  CSL_sysDrvClose(minor);

  return 0;
}

int CSL_sysLinuxDrvIoctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
  int     minor;
  CSL_Status status;

  minor = (int) filp->private_data;

#ifdef CSL_DEBUG
  printk(KERN_INFO "CSL: CSL_sysLinuxDrvIoctl()  , %4d, %2d \n", gCSL_sysLinuxObj.major, minor);
#endif

  status = CSL_sysDrvIoctl(minor, cmd, (void *) arg);

  return status;
}

int CSL_sysLinuxDrvMmap(struct file *filp, struct vm_area_struct *vma) 
{
  int     minor;
  CSL_Status status;

  minor = (int) filp->private_data;
  
  if(minor!=CSL_MODULE_VPSS)
    return CSL_EFAIL;

#ifdef CSL_DEBUG
  printk(KERN_INFO "CSL: CSL_sysLinuxDrvMmap()  , remap_pfn_range() start = 0x%lx, end = 0x%lx, pgoff = 0x%lx\n", vma->vm_start, vma->vm_end, vma->vm_pgoff);
#endif

  vma->vm_page_prot = pgprot_noncached (vma->vm_page_prot);
  
  status = remap_pfn_range (vma, vma->vm_start, 
                            #ifdef LINUX_2_6_18
			                      (vma->vm_pgoff >> PAGE_SHIFT), 
                            #else 
                      		  vma->vm_pgoff, 
                            #endif 
 			                      vma->vm_end - vma->vm_start, vma->vm_page_prot
 			                    );
	if(status)  
  {
    #ifdef CSL_DEBUG
    printk(KERN_WARNING "CSL: CSL_sysLinuxDrvMmap()  , ERROR \n");
    #endif
    return -EAGAIN;
  }
  
  return 0;
}


struct file_operations gCSL_sysLinuxFileOps = {
  .owner = THIS_MODULE,
  .open = CSL_sysLinuxDrvOpen,
  .release = CSL_sysLinuxDrvRelease,
  .ioctl = CSL_sysLinuxDrvIoctl,
  .mmap = CSL_sysLinuxDrvMmap,
};

int CSL_sysLinuxDrvInit(void)
{
  int     result;
  dev_t   dev = 0;

#ifdef CSL_DEBUG
  printk(KERN_INFO "CSL: CSL_sysLinuxDrvInit() \n");
#endif

  result = CSL_sysDrvInit();
  if (result) {
    printk(KERN_WARNING "CSL: ERROR in CSL_sysDrvInit() \n");
    return result;
  }

  result = alloc_chrdev_region(&dev, 0, CSL_MODULE_MAX, CSL_DRV_NAME);

  if (result < 0) {
    printk(KERN_WARNING "CSL: can't get device major num \n");
    return result;
  }

  gCSL_sysLinuxObj.major = MAJOR(dev);

  cdev_init(&gCSL_sysLinuxObj.cdev, &gCSL_sysLinuxFileOps);

  gCSL_sysLinuxObj.cdev.owner = THIS_MODULE;
  gCSL_sysLinuxObj.cdev.ops = &gCSL_sysLinuxFileOps;

  result = cdev_add(&gCSL_sysLinuxObj.cdev, dev, CSL_MODULE_MAX);

  if (result)
    printk(KERN_WARNING "CSL: Error [%d] while adding device [%s] \n", result, CSL_DRV_NAME);

  printk(KERN_INFO "CSL: Module install successful, device major num = %d \n", gCSL_sysLinuxObj.major);
  printk(KERN_INFO "CSL: Module version %s, built on %s %s \n", CSL_SYS_VERSION, __DATE__, __TIME__);  

  return result;
}

void CSL_sysLinuxDrvExit(void)
{
  dev_t   devno = MKDEV(gCSL_sysLinuxObj.major, 0);

#ifdef CSL_DEBUG
  printk(KERN_INFO "CSL: CSL_sysLinuxDrvExit() \n");
#endif

  cdev_del(&gCSL_sysLinuxObj.cdev);

  unregister_chrdev_region(devno, CSL_MODULE_MAX);

  CSL_sysDrvExit();
}

module_init(CSL_sysLinuxDrvInit);
module_exit(CSL_sysLinuxDrvExit);

MODULE_AUTHOR("Texas Instruments");
MODULE_LICENSE("GPL");
