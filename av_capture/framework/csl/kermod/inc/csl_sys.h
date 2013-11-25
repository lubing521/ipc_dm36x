/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#ifndef _CSL_SYS_H_
#define _CSL_SYS_H_

#include <csl_soc.h>

/**
  \file csl_sys.h
  \brief System APIs
*/

/**
  \defgroup  CSL_SYS System

  System APIs

  @{
*/

#ifdef _OS_LINUX_
#include <linux/interrupt.h>
#include <linux/kernel.h>       /* printk() */
#include <linux/slab.h>         /* kmalloc() */

#include <asm/uaccess.h>        /* copy_*_user */
#include <linux/io.h>

#define CSL_PHYS_TO_VIRT(x)       IO_ADDRESS(x)
#define CSL_SYS_INT_BASE_ID       (0)

#define CSL_INT_RET_SOK           (IRQ_HANDLED)

typedef irqreturn_t CSL_IntRet_t;

#define CSL_copyToUser(touserptr, fromkernelptr, size)      copy_to_user((touserptr), (fromkernelptr), (size) )
#define CSL_copyFromUser(tokernelptr, fromuserptr, size)    copy_from_user((tokernelptr), (fromuserptr), (size) )

#define CSL_getUser(kernelvariable, userptr)   get_user((kernelvariable), (userptr))
#define CSL_putUser(kernelvariable, userptr)   put_user((kernelvariable), (userptr))

#define CSL_sysMemAlloc(size)   (void*)kmalloc((size), GFP_KERNEL)
#define CSL_sysMemFree(ptr)     kfree((ptr))

#define CSL_intGlobalDisable(oldIntState)    local_irq_save(oldIntState)
#define CSL_intGlobalRestore(intState)       local_irq_restore(intState)


#else

#define CSL_PHYS_TO_VIRT(x)       (x)

#define CSL_SYS_INT_BASE_ID       (0)

#define CSL_INT_RET_SOK           (CSL_SOK)

typedef CSL_Status CSL_IntRet_t;

#define CSL_copyToUser(touserptr, fromkernelptr, size)      (int)memcpy((touserptr), (fromkernelptr), (size) )
#define CSL_copyFromUser(tokernelptr, fromuserptr, size)    (int)memcpy((tokernelptr), (fromuserptr), (size) )

#define CSL_getUser(kernelvariable, userptr)   (kernelvariable) = *(userptr)
#define CSL_putUser(kernelvariable, userptr)   *(userptr) = (kernelvariable)

#define CSL_sysMemAlloc(size)   (void*)malloc((size))
#define CSL_sysMemFree(ptr)     free((ptr))

#define CSL_intGlobalDisable(intState)
#define CSL_intGlobalRestore(intState)


#endif

#define CSL_SYS_INT_CCDC_VD0            (CSL_SYS_INT_BASE_ID+0)
#define CSL_SYS_INT_CCDC_VD1            (CSL_SYS_INT_BASE_ID+1)
#define CSL_SYS_INT_RSZ                 (CSL_SYS_INT_BASE_ID+2)
#define CSL_SYS_INT_LDC                 (CSL_SYS_INT_BASE_ID+3)
#define CSL_SYS_INT_FD                  (CSL_SYS_INT_BASE_ID+4)
#define CSL_SYS_INT_IPIPE_BSC           (CSL_SYS_INT_BASE_ID+5)
#define CSL_SYS_INT_IPIPE_BOXCAR        (CSL_SYS_INT_BASE_ID+6)

/* Base Address */
#define CSL_SYS_DDR_BASE_PHYS_ADDR  (0x80000000)
#define CSL_SYS_BASE_ADDR           CSL_PHYS_TO_VIRT(CSL_SYS_BASE_PHYS_ADDR) 
#define CSL_ISP5_BASE_ADDR          CSL_PHYS_TO_VIRT(CSL_ISP5_BASE_PHYS_ADDR) 

#define CSL_ISP5_0_REGS             ((CSL_Isp5RegsOvly)       CSL_PHYS_TO_VIRT((Uint32)CSL_ISP5_0_PHYS_REGS) )
#define CSL_VPSS_0_REGS             ((CSL_VpssRegsOvly)       CSL_PHYS_TO_VIRT((Uint32)CSL_VPSS_0_PHYS_REGS) )
#define CSL_RSZ_0_REGS              ((CSL_RszRegsOvly)        CSL_PHYS_TO_VIRT((Uint32)CSL_RSZ_0_PHYS_REGS) )
#define CSL_IPIPE_0_REGS            ((CSL_IpipeRegsOvly)      CSL_PHYS_TO_VIRT((Uint32)CSL_IPIPE_0_PHYS_REGS) )
#define CSL_ISIF_0_REGS             ((CSL_IsifRegsOvly)       CSL_PHYS_TO_VIRT((Uint32)CSL_ISIF_0_PHYS_REGS) )
#define CSL_IPIPEIF_0_REGS          ((CSL_IpipeifRegsOvly)    CSL_PHYS_TO_VIRT((Uint32)CSL_IPIPEIF_0_PHYS_REGS) )
#define CSL_H3A_0_REGS              ((CSL_H3aRegsOvly)        CSL_PHYS_TO_VIRT((Uint32)CSL_H3A_0_PHYS_REGS) )
#define CSL_LDC_0_REGS              ((CSL_LdcRegsOvly)        CSL_PHYS_TO_VIRT((Uint32)CSL_LDC_0_PHYS_REGS) )
#define CSL_FACE_DETECT_0_REGS      ((CSL_FaceDetectRegsOvly) CSL_PHYS_TO_VIRT((Uint32)CSL_FACE_DETECT_0_PHYS_REGS) )

#define CSL_SYS_REGS                ((CSL_SysRegsOvly)        CSL_PHYS_TO_VIRT((Uint32)CSL_SYS_PHYS_REGS) )
#define CSL_INTC_REGS               ((CSL_IntcRegsOvly)       CSL_PHYS_TO_VIRT((Uint32)CSL_INTC_PHYS_REGS) )
#define CSL_GPIO_REGS               ((CSL_GpioRegsOvly)       CSL_PHYS_TO_VIRT((Uint32)CSL_GPIO_PHYS_REGS) )

#define CSL_IPIPE_HST_TBL_0_ADDR     CSL_PHYS_TO_VIRT(CSL_IPIPE_HST_TBL_0_PHYS_ADDR) 
#define CSL_IPIPE_HST_TBL_1_ADDR     CSL_PHYS_TO_VIRT(CSL_IPIPE_HST_TBL_1_PHYS_ADDR)
#define CSL_IPIPE_HST_TBL_2_ADDR     CSL_PHYS_TO_VIRT(CSL_IPIPE_HST_TBL_2_PHYS_ADDR)
#define CSL_IPIPE_HST_TBL_3_ADDR     CSL_PHYS_TO_VIRT(CSL_IPIPE_HST_TBL_3_PHYS_ADDR)
#define CSL_IPIPE_BSC_TBL_0_ADDR     CSL_PHYS_TO_VIRT(CSL_IPIPE_BSC_TBL_0_PHYS_ADDR)
#define CSL_IPIPE_BSC_TBL_1_ADDR     CSL_PHYS_TO_VIRT(CSL_IPIPE_BSC_TBL_1_PHYS_ADDR)
#define CSL_IPIPE_DPC_TBL_0_ADDR     CSL_PHYS_TO_VIRT(CSL_IPIPE_DPC_TBL_0_PHYS_ADDR)
#define CSL_IPIPE_DPC_TBL_1_ADDR     CSL_PHYS_TO_VIRT(CSL_IPIPE_DPC_TBL_1_PHYS_ADDR)
#define CSL_IPIPE_YEE_TBL_ADDR       CSL_PHYS_TO_VIRT(CSL_IPIPE_YEE_TBL_PHYS_ADDR)
#define CSL_IPIPE_GBCE_TBL_ADDR      CSL_PHYS_TO_VIRT(CSL_IPIPE_GBCE_TBL_PHYS_ADDR)
#define CSL_IPIPE_3DLUT_TBL_0_ADDR   CSL_PHYS_TO_VIRT(CSL_IPIPE_3DLUT_TBL_0_PHYS_ADDR)
#define CSL_IPIPE_3DLUT_TBL_1_ADDR   CSL_PHYS_TO_VIRT(CSL_IPIPE_3DLUT_TBL_1_PHYS_ADDR)
#define CSL_IPIPE_3DLUT_TBL_2_ADDR   CSL_PHYS_TO_VIRT(CSL_IPIPE_3DLUT_TBL_2_PHYS_ADDR)
#define CSL_IPIPE_3DLUT_TBL_3_ADDR   CSL_PHYS_TO_VIRT(CSL_IPIPE_3DLUT_TBL_3_PHYS_ADDR)
#define CSL_IPIPE_GAMMA_R_TBL_3_ADDR CSL_PHYS_TO_VIRT(CSL_IPIPE_GAMMA_R_TBL_3_PHYS_ADDR)
#define CSL_IPIPE_GAMMA_G_TBL_3_ADDR CSL_PHYS_TO_VIRT(CSL_IPIPE_GAMMA_G_TBL_3_PHYS_ADDR)
#define CSL_IPIPE_GAMMA_B_TBL_3_ADDR CSL_PHYS_TO_VIRT(CSL_IPIPE_GAMMA_B_TBL_3_PHYS_ADDR)

#define CSL_ISIF_LIN_TBL_0_ADDR      CSL_PHYS_TO_VIRT(CSL_ISIF_LIN_TBL_0_PHYS_ADDR)
#define CSL_ISIF_LIN_TBL_1_ADDR      CSL_PHYS_TO_VIRT(CSL_ISIF_LIN_TBL_1_PHYS_ADDR)

#define CSL_LDC_LUT_TBL_ADDR         CSL_PHYS_TO_VIRT(CSL_LDC_LUT_TBL_PHYS_ADDR)


typedef struct {

  void   *flag;

} CSL_SysFlag;

typedef struct {

  void   *mutex;

} CSL_SysMutex;

typedef CSL_IntRet_t(*CSL_SysIntIsr) (int intId, void *prm, void *reserved);

CSL_Status CSL_sysFlagCreate(CSL_SysFlag * hndl);
CSL_Status CSL_sysFlagClear(CSL_SysFlag * hndl);
CSL_Status CSL_sysFlagSet(CSL_SysFlag * hndl);
CSL_Status CSL_sysFlagWait(CSL_SysFlag * hndl, Uint32 timeout);
CSL_Status CSL_sysFlagDelete(CSL_SysFlag * hndl);

CSL_Status CSL_sysMutexCreate(CSL_SysMutex * hndl);
CSL_Status CSL_sysMutexLock(CSL_SysMutex * hndl, Uint32 timeout);
CSL_Status CSL_sysMutexUnlock(CSL_SysMutex * hndl);
CSL_Status CSL_sysMutexDelete(CSL_SysMutex * hndl);

CSL_Status CSL_sysIntEnable(Uint32 intId, Bool32 enable);
CSL_Status CSL_sysIntClear(Uint32 intId);
CSL_Status CSL_sysIntAttachIsr(Uint32 intId, CSL_SysIntIsr isr, void *prm);

Uint32  CSL_sysGetCurTime(void);

void   *CSL_sysGetBaseAddr(Uint16 moduleId);

/*@}*/

#endif
