/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

/**
  \file csl_soc.h

  \brief Top-level include to get access to all register overlay structures and addresses
*/

#ifndef _CSL_SOC_H_
#define _CSL_SOC_H_

#include <tistdtypes.h>

typedef int CSL_Status;         ///< CSL Status

#ifndef CSL_SOK
#define CSL_SOK             ( 0)  ///< CSL Status : Success
#endif

#ifndef CSL_EFAIL
#define CSL_EFAIL           (-1)  ///< CSL Status : Generic failure
#endif

#ifndef CSL_EINVPARAMS
#define CSL_EINVPARAMS      (-2)  ///< CSL Status : invalid parameters
#endif

#ifndef NULL
#define NULL            (0)     ///< NULL pointer
#endif

#define CSL_SYS_VERSION           "0.10.00"           ///< CSL Version String

#define CSL_SYS_TIMEOUT_NONE      (0)                 ///< Timeout: None
#define CSL_SYS_TIMEOUT_FOREVER   ((unsigned int)-1)  ///< Timeout: Wait forever

/* module Ids */
#define CSL_MODULE_ISP5             ( 0)    ///< Module ID: ISP5
#define CSL_MODULE_VPSS             ( 1)    ///< Module ID: VPSS
#define CSL_MODULE_RSZ              ( 2)    ///< Module ID: Resize
#define CSL_MODULE_IPIPE            ( 3)    ///< Module ID: IPIPE
#define CSL_MODULE_ISIF             ( 4)    ///< Module ID: ISIF/CCDC
#define CSL_MODULE_IPIPEIF          ( 5)    ///< Module ID: IPIPEIF
#define CSL_MODULE_H3A              ( 6)    ///< Module ID: H3A
#define CSL_MODULE_LDC              ( 7)    ///< Module ID: LDC
#define CSL_MODULE_FACE_DETECT      ( 8)    ///< Module ID: Face Detect
#define CSL_MODULE_GPIO             ( 9)    ///< Module ID: GPIO
#define CSL_MODULE_MAX              (10)    ///< Module ID: Max

#define CSL_DRV_NAME                "csl"   ///< Driver name
#define CSL_DRV_MAGICNUM            (0xCC)  ///< Driver magic number

#define CSL_SYS_CMD_LOCK      (0xF0)  ///< prm: [I ] Uint32 timeout
#define CSL_SYS_CMD_UNLOCK    (0xF1)  ///< prm: [  ] NONE

/* Register definition include */
#include <regs/cslr_vpss_001.h>
#include <regs/cslr_isp5_001.h>
#include <regs/cslr_isif_001.h>
#include <regs/cslr_ipipeif_000.h>
#include <regs/cslr_ipipe_001.h>
#include <regs/cslr_rsz_001.h>
#include <regs/cslr_h3a_001.h>
#include <regs/cslr_ldc_002.h>
#include <regs/cslr_face_detect_001.h>
#include <regs/cslr_intc_001.h>
#include <regs/cslr_gpio_002.h>
#include <regs/cslr_sys_001.h>

/* Register overlay structures */
typedef volatile CSL_VpssRegs *CSL_VpssRegsOvly;
typedef volatile CSL_Isp5Regs *CSL_Isp5RegsOvly;
typedef volatile CSL_IsifRegs *CSL_IsifRegsOvly;
typedef volatile CSL_IpipeifRegs *CSL_IpipeifRegsOvly;
typedef volatile CSL_IpipeRegs *CSL_IpipeRegsOvly;
typedef volatile CSL_RszRegs *CSL_RszRegsOvly;
typedef volatile CSL_H3aRegs *CSL_H3aRegsOvly;
typedef volatile CSL_LdcRegs *CSL_LdcRegsOvly;
typedef volatile CSL_FaceDetectRegs *CSL_FaceDetectRegsOvly;
typedef volatile CSL_IntcRegs *CSL_IntcRegsOvly;
typedef volatile CSL_GpioRegs *CSL_GpioRegsOvly;
typedef volatile CSL_SysRegs *CSL_SysRegsOvly;

#define CSL_ISP5_0_OFFSET              (0x0000)
#define CSL_VPSS_0_OFFSET              (0x0200) 
#define CSL_RSZ_0_OFFSET               (0x0400) 
#define CSL_IPIPE_0_OFFSET             (0x0800) 
#define CSL_ISIF_0_OFFSET              (0x1000) 
#define CSL_IPIPEIF_0_OFFSET           (0x1200) 
#define CSL_H3A_0_OFFSET               (0x1400) 
#define CSL_LDC_0_OFFSET               (0x1600) 
#define CSL_FACE_DETECT_0_OFFSET       (0x1800) 
#define CSL_IPIPE_HST_TBL_0_OFFSET     (0x2000)
#define CSL_IPIPE_HST_TBL_1_OFFSET     (0x2800)
#define CSL_IPIPE_HST_TBL_2_OFFSET     (0x3000)
#define CSL_IPIPE_HST_TBL_3_OFFSET     (0x3800)
#define CSL_IPIPE_BSC_TBL_0_OFFSET     (0x4000)
#define CSL_IPIPE_BSC_TBL_1_OFFSET     (0x6000)
#define CSL_IPIPE_DPC_TBL_0_OFFSET     (0x8000)
#define CSL_IPIPE_DPC_TBL_1_OFFSET     (0x8400)
#define CSL_IPIPE_YEE_TBL_OFFSET       (0x8800)
#define CSL_IPIPE_GBCE_TBL_OFFSET      (0x9000)
#define CSL_IPIPE_3DLUT_TBL_0_OFFSET   (0x9800)
#define CSL_IPIPE_3DLUT_TBL_1_OFFSET   (0x9C00)
#define CSL_IPIPE_3DLUT_TBL_2_OFFSET   (0xA000)
#define CSL_IPIPE_3DLUT_TBL_3_OFFSET   (0xA400)
#define CSL_IPIPE_GAMMA_R_TBL_3_OFFSET (0xA800)
#define CSL_IPIPE_GAMMA_G_TBL_3_OFFSET (0xB000)
#define CSL_IPIPE_GAMMA_B_TBL_3_OFFSET (0xB800)
#define CSL_ISIF_LIN_TBL_0_OFFSET      (0xC000)
#define CSL_ISIF_LIN_TBL_1_OFFSET      (0xC400)
#define CSL_LDC_LUT_TBL_OFFSET         (0xF000)

#define CSL_ISP5_BASE_MAX_OFFSET       (0x10000) 

#define CSL_SYS_BASE_PHYS_ADDR           (0x01c00000) 
#define CSL_PLL1_BASE_PHYS_ADDR          (0x01c40800)
#define CSL_PLL2_BASE_PHYS_ADDR          (0x01c40c00)

#define CSL_SYS_BASE_MAX_OFFSET          (0x80000)   

#define CSL_ISP5_BASE_PHYS_ADDR          (CSL_SYS_BASE_PHYS_ADDR + 0x70000)

#define CSL_SYS_PHYS_REGS                ((CSL_SysRegsOvly)        (CSL_SYS_BASE_PHYS_ADDR + 0x40000) )
#define CSL_INTC_PHYS_REGS               ((CSL_IntcRegsOvly)       (CSL_SYS_BASE_PHYS_ADDR + 0x48000) )
#define CSL_GPIO_PHYS_REGS               ((CSL_GpioRegsOvly)       (CSL_SYS_BASE_PHYS_ADDR + 0x67000) )

#define CSL_ISP5_0_PHYS_REGS             ((CSL_Isp5RegsOvly)       (CSL_ISP5_BASE_PHYS_ADDR + CSL_ISP5_0_OFFSET       ))
#define CSL_VPSS_0_PHYS_REGS             ((CSL_VpssRegsOvly)       (CSL_ISP5_BASE_PHYS_ADDR + CSL_VPSS_0_OFFSET       ))
#define CSL_RSZ_0_PHYS_REGS              ((CSL_RszRegsOvly)        (CSL_ISP5_BASE_PHYS_ADDR + CSL_RSZ_0_OFFSET        ))
#define CSL_IPIPE_0_PHYS_REGS            ((CSL_IpipeRegsOvly)      (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_0_OFFSET      ))
#define CSL_ISIF_0_PHYS_REGS             ((CSL_IsifRegsOvly)       (CSL_ISP5_BASE_PHYS_ADDR + CSL_ISIF_0_OFFSET       ))
#define CSL_IPIPEIF_0_PHYS_REGS          ((CSL_IpipeifRegsOvly)    (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPEIF_0_OFFSET    ))
#define CSL_H3A_0_PHYS_REGS              ((CSL_H3aRegsOvly)        (CSL_ISP5_BASE_PHYS_ADDR + CSL_H3A_0_OFFSET        ))
#define CSL_LDC_0_PHYS_REGS              ((CSL_LdcRegsOvly)        (CSL_ISP5_BASE_PHYS_ADDR + CSL_LDC_0_OFFSET        ))
#define CSL_FACE_DETECT_0_PHYS_REGS      ((CSL_FaceDetectRegsOvly) (CSL_ISP5_BASE_PHYS_ADDR + CSL_FACE_DETECT_0_OFFSET))

#define CSL_IPIPE_HST_TBL_0_PHYS_ADDR     (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_HST_TBL_0_OFFSET    )
#define CSL_IPIPE_HST_TBL_1_PHYS_ADDR     (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_HST_TBL_1_OFFSET    )
#define CSL_IPIPE_HST_TBL_2_PHYS_ADDR     (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_HST_TBL_2_OFFSET    )
#define CSL_IPIPE_HST_TBL_3_PHYS_ADDR     (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_HST_TBL_3_OFFSET    )
#define CSL_IPIPE_BSC_TBL_0_PHYS_ADDR     (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_BSC_TBL_0_OFFSET    )
#define CSL_IPIPE_BSC_TBL_1_PHYS_ADDR     (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_BSC_TBL_1_OFFSET    )
#define CSL_IPIPE_DPC_TBL_0_PHYS_ADDR     (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_DPC_TBL_0_OFFSET    )
#define CSL_IPIPE_DPC_TBL_1_PHYS_ADDR     (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_DPC_TBL_1_OFFSET    )
#define CSL_IPIPE_YEE_TBL_PHYS_ADDR       (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_YEE_TBL_OFFSET      )
#define CSL_IPIPE_GBCE_TBL_PHYS_ADDR      (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_GBCE_TBL_OFFSET     )
#define CSL_IPIPE_3DLUT_TBL_0_PHYS_ADDR   (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_3DLUT_TBL_0_OFFSET  )
#define CSL_IPIPE_3DLUT_TBL_1_PHYS_ADDR   (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_3DLUT_TBL_1_OFFSET  )
#define CSL_IPIPE_3DLUT_TBL_2_PHYS_ADDR   (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_3DLUT_TBL_2_OFFSET  )
#define CSL_IPIPE_3DLUT_TBL_3_PHYS_ADDR   (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_3DLUT_TBL_3_OFFSET  )
#define CSL_IPIPE_GAMMA_R_TBL_3_PHYS_ADDR (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_GAMMA_R_TBL_3_OFFSET)
#define CSL_IPIPE_GAMMA_G_TBL_3_PHYS_ADDR (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_GAMMA_G_TBL_3_OFFSET)
#define CSL_IPIPE_GAMMA_B_TBL_3_PHYS_ADDR (CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_GAMMA_B_TBL_3_OFFSET)
#define CSL_ISIF_LIN_TBL_0_PHYS_ADDR      (CSL_ISP5_BASE_PHYS_ADDR + CSL_ISIF_LIN_TBL_0_OFFSET     )
#define CSL_ISIF_LIN_TBL_1_PHYS_ADDR      (CSL_ISP5_BASE_PHYS_ADDR + CSL_ISIF_LIN_TBL_1_OFFSET     )
#define CSL_LDC_LUT_TBL_PHYS_ADDR         (CSL_ISP5_BASE_PHYS_ADDR + CSL_LDC_LUT_TBL_OFFSET        )




#ifdef CSL_KERNEL_API
#include <csl_sys.h>
#endif

int CSL_sysInit(void);
int CSL_sysExit(void);

#endif /* _CSL_SOC_H_ */

/**
  \mainpage DM360 CSL

  The document details the APIs that are part of the DM360 CSL (Chip support library)

  The Chip Support Library consists of the below modules,

  - \ref CSL_VPSS
    - \ref CSL_VPFE
      - \ref CSL_CCDC
      - \ref CSL_IPIPEIF
      - \ref CSL_IPIPE
      - \ref CSL_RSZ
      - \ref CSL_H3A
      - \ref CSL_LDC
      - \ref CSL_FD
    - \ref CSL_GPIO

  \par See Also,
  - \ref TI_DISCLAIMER
*/

/**
 \page  TI_DISCLAIMER  TI Disclaimer

 \htmlinclude ti_disclaim.htm
*/
