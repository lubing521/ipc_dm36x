/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#ifndef _CSL_VPSS_H_
#define _CSL_VPSS_H_

#ifdef __cplusplus
extern  "C" {
#endif

#include <csl_soc.h>

/**
  \file csl_vpss.h
  \brief VPSS / ISP Common APIs
*/

/**
  \defgroup  CSL_VPSS VPSS

  Video Processing Sub-System modules

  @{
*/

// define's

#define CSL_VPSS_NO_UPDATE    (0xFF)  ///< if this value is used for any of the below fields the corresponding value in register is not updated

#define CSL_VPSS_PCLK_SEL_MMR     0 ///< PCLK Sync clock select : MMR_CLK
#define CSL_VPSS_PCLK_SEL_VPSS    1 ///< PCLK Sync clock select : VPSS_CLK

#define CSL_VPSS_LDC_CLK_MODULE   0 ///< LDC Clock Select : Module can access to memory
#define CSL_VPSS_LDC_CLK_ARM      1 ///< LDC Clock Select : ARM can access to memory

#define CSL_VPSS_INT_EVT_SEL_ISIF_INT0              0 ///< VPSS Interrupt/Event selection : ISIF_INT0
#define CSL_VPSS_INT_EVT_SEL_ISIF_INT1              1 ///< VPSS Interrupt/Event selection : ISIF_INT1
#define CSL_VPSS_INT_EVT_SEL_ISIF_INT2              2 ///< VPSS Interrupt/Event selection : ISIF_INT2
#define CSL_VPSS_INT_EVT_SEL_ISIF_INT3              3 ///< VPSS Interrupt/Event selection : ISIF_INT3
#define CSL_VPSS_INT_EVT_SEL_IPIPE_INT_REG          4 ///< VPSS Interrupt/Event selection : IPIPE_INT_REG
#define CSL_VPSS_INT_EVT_SEL_IPIPE_INT_LAST_PIX     5 ///< VPSS Interrupt/Event selection : IPIPE_INT_LAST_PIX
#define CSL_VPSS_INT_EVT_SEL_IPIPE_INT_DMA          6 ///< VPSS Interrupt/Event selection : IPIPE_INT_DMA
#define CSL_VPSS_INT_EVT_SEL_IPIPE_INT_BSC          7 ///< VPSS Interrupt/Event selection : IPIPE_INT_BSC
#define CSL_VPSS_INT_EVT_SEL_IPIPE_INT_HST          8 ///< VPSS Interrupt/Event selection : IPIPE_INT_HST
#define CSL_VPSS_INT_EVT_SEL_IPIPEIF_INT            9 ///< VPSS Interrupt/Event selection : IPIPEIF_INT
#define CSL_VPSS_INT_EVT_SEL_AEW_INT                10  ///< VPSS Interrupt/Event selection : AEW_INT
#define CSL_VPSS_INT_EVT_SEL_AF_INT                 11  ///< VPSS Interrupt/Event selection : AF_INT
#define CSL_VPSS_INT_EVT_SEL_H3A_INT                12  ///< VPSS Interrupt/Event selection : H3A_INT
#define CSL_VPSS_INT_EVT_SEL_RSZ_INT_REG            13  ///< VPSS Interrupt/Event selection : RSZ_INT_REG
#define CSL_VPSS_INT_EVT_SEL_RSZ_INT_LAST_PIX       14  ///< VPSS Interrupt/Event selection : RSZ_INT_LAST_PIX
#define CSL_VPSS_INT_EVT_SEL_RSZ_INT_DMA            15  ///< VPSS Interrupt/Event selection : RSZ_INT_DMA
#define CSL_VPSS_INT_EVT_SEL_RSZ_INT_CYC_RZA        16  ///< VPSS Interrupt/Event selection : RSZ_INT_CYC_RZA
#define CSL_VPSS_INT_EVT_SEL_RSZ_INT_CYC_RZB        17  ///< VPSS Interrupt/Event selection : RSZ_INT_CYC_RZB
#define CSL_VPSS_INT_EVT_SEL_LDC_INT                18  ///< VPSS Interrupt/Event selection : LDC_INT
#define CSL_VPSS_INT_EVT_SEL_FDIF_INT               19  ///< VPSS Interrupt/Event selection : FDIF_INT
#define CSL_VPSS_INT_EVT_SEL_OSD_INT                20  ///< VPSS Interrupt/Event selection : OSD_INT
#define CSL_VPSS_INT_EVT_SEL_VENC_INT               21  ///< VPSS Interrupt/Event selection : VENC_INT
#define CSL_VPSS_INT_EVT_SEL_RSZ_INT_EOF0           22  ///< VPSS Interrupt/Event selection : RSZ_INT_EOF0
#define CSL_VPSS_INT_EVT_SEL_RSZ_INT_EOF1           23  ///< VPSS Interrupt/Event selection : RSZ_INT_EOF1
#define CSL_VPSS_INT_EVT_SEL_H3A_INT_EOF            24  ///< VPSS Interrupt/Event selection : H3A_INT_EOF
#define CSL_VPSS_INT_EVT_SEL_IPIPE_INT_EOF          25  ///< VPSS Interrupt/Event selection : IPIPE_INT_EOF
#define CSL_VPSS_INT_EVT_SEL_LDC_INT_EOF            26  ///< VPSS Interrupt/Event selection : LDC_INT_EOF
#define CSL_VPSS_INT_EVT_SEL_FD_INT_EOF             27  ///< VPSS Interrupt/Event selection : FD_INT_EOF
#define CSL_VPSS_INT_EVT_SEL_IPIPE_INT_DPC_INI      28  ///< VPSS Interrupt/Event selection : IPIPE_INT_DPC_INI
#define CSL_VPSS_INT_EVT_SEL_IPIPE_INT_DPC_RNEW0    29  ///< VPSS Interrupt/Event selection : IPIPE_INT_DPC_RNEW0
#define CSL_VPSS_INT_EVT_SEL_IPIPE_INT_DPC_RNEW1    30  ///< VPSS Interrupt/Event selection : IPIPE_INT_DPC_RNEW1

#define CSL_VPSS_BL_WBL_ISIF_OR_IPIPE_SEL_IPIPE    0 ///< BL WBL select : IPIPE
#define CSL_VPSS_BL_WBL_ISIF_OR_IPIPE_SEL_ISIF     1 ///< BL WBL select : ISIF

#define CSL_VPSS_BL_WBL_LDC_OR_IPIPE_SEL_LDC       0 ///< BL WBL select : LDC
#define CSL_VPSS_BL_WBL_LDC_OR_IPIPE_SEL_IPIPE     1 ///< BL WBL select : IPIPE


// data structure's

/**
  \brief  VPSS clock configuration
*/
  typedef struct {

    // in ISP5 regs
    Uint8   pclkSelect;         ///< CSL_VPSS_PCLK_SEL_MMR \n CSL_VPSS_PCLK_SEL_VPSS

    Uint8   pclkSyncClkEnable;  ///< PCLK Sync module, TRUE: Enable, FALSE: Disable, CSL_VPSS_NO_UPDATE: No change
    Uint8   blClkEnable;        ///< BL Clock, TRUE: Enable, FALSE: Disable, CSL_VPSS_NO_UPDATE: No change
    Uint8   isifClkEnable;      ///< ISIF Clock, TRUE: Enable, FALSE: Disable, CSL_VPSS_NO_UPDATE: No change
    Uint8   h3aClkEnable;       ///< H3A Clock, TRUE: Enable, FALSE: Disable, CSL_VPSS_NO_UPDATE: No change
    Uint8   ipipeifClkEnable;   ///< IPIPE Clock, TRUE: Enable, FALSE: Disable, CSL_VPSS_NO_UPDATE: No change
    Uint8   ipipeClkEnable;     ///< IPIPE Clock, TRUE: Enable, FALSE: Disable, CSL_VPSS_NO_UPDATE: No change
    Uint8   rszClkEnable;       ///< RSZ Clock, TRUE: Enable, FALSE: Disable, CSL_VPSS_NO_UPDATE: No change

    // in VPSS regs
    Uint8   ldcClkEnable;       ///< LDC Clock, TRUE: Enable, FALSE: Disable, CSL_VPSS_NO_UPDATE: No change
    Uint8   fdifClkEnable;      ///< FDIF Clock, TRUE: Enable, FALSE: Disable, CSL_VPSS_NO_UPDATE: No change

    Uint8   ldcClkSelect;       ///< CSL_VPSS_LDC_CLK_MODULE \n CSL_VPSS_LDC_CLK_ARM, CSL_VPSS_NO_UPDATE: No change

  } CSL_VpssClkConfig;

/**
  \brief  VPSS Interrupt/Event selection configuration
*/
  typedef struct {

    Uint8   intSel[9];          ///< CSL_VPSS_INT_EVT_SEL_ISIF_INT0 ...CSL_VPSS_INT_EVT_SEL_IPIPE_INT_DPC_RNEW1, CSL_VPSS_NO_UPDATE: No change
    Uint8   evtSel[4];          ///< CSL_VPSS_INT_EVT_SEL_ISIF_INT0 ...CSL_VPSS_INT_EVT_SEL_IPIPE_INT_DPC_RNEW1, CSL_VPSS_NO_UPDATE: No change

  } CSL_VpssIntEvtConfig;

/**
  \brief  VPSS Read/Write priority configuration
*/
  typedef struct {

    Uint8   priVpssWr;          ///< Write priority for VPSS, Should set highest priority (0) for best performance, CSL_VPSS_NO_UPDATE: No change
    Uint8   priVpssRd;          ///< Read priority for VPSS, Should set highest priority (0) for best performance, CSL_VPSS_NO_UPDATE: No change

  } CSL_VpssPriConfig;

/**
  \brief  BL WBL configuration
*/
  typedef struct {

    Uint8   wblSelIsifIpipe;    ///< CSL_VPSS_BL_WBL_SEL_IPIPE_ISIF \n CSL_VPSS_BL_WBL_SEL_ISIF_IPIPE, CSL_VPSS_NO_UPDATE: No change
    Uint8   wblSelIpipeLdc;     ///< CSL_VPSS_BL_WBL_SEL_LDC_IPIPE \n CSL_VPSS_BL_WBL_SEL_IPIPE_LDC, CSL_VPSS_NO_UPDATE: No change

  } CSL_VpssWblSelConfig;

/**
  \brief CLK DIV structure
*/
  typedef struct {

    Uint16 PLL1_M;
    Uint16 PLL1_PREDIV;
    Uint16 PLL1_POSTDIV;
    Uint16 PLL1_PLLDIV[10];

    Uint16 PLL2_M;
    Uint16 PLL2_PREDIV;
    Uint16 PLL2_POSTDIV;
    Uint16 PLL2_PLLDIV[10];

    Uint32 PERICLKCTL;
    Uint32 VPSSCLKCTL;

  } CSL_VpssClkDiv;


/**
  \brief  VPSS Hardware configuration structure
*/
  typedef struct {

    CSL_VpssWblSelConfig *wblSelConfig; ///< WBL config parameters if NULL do not set these parameters
    CSL_VpssPriConfig *priConfig; ///< Read/Write priority config parameters if NULL do not set these parameters
    CSL_VpssIntEvtConfig *intEvtConfig; ///< Interrupt/Event config parameters if NULL do not set these parameters
    CSL_VpssClkConfig *clkConfig; ///< Clock config parameters if NULL do not set these parameters

  } CSL_VpssHwSetup;

/**
  \brief Module Object Structure
*/
  typedef struct {

#ifdef CSL_KERNEL_API
    CSL_VpssRegsOvly regs;      ///< Register overlay
    CSL_Isp5RegsOvly ispRegs;   ///< ISP5 Register overlay
#else
    int     fd;
    unsigned int            regBaseLen;     ///< Length of register addresses mapped to virtual memory
    unsigned long           regBasePhysAddr;///< Physical memory address of ISP registers
    void                   *regBaseVirtAddr;///< Virtual memory mapped address of ISP registers
    CSL_VpssRegsOvly        vpssRegs;       ///< Register overlay
    CSL_Isp5RegsOvly        ispRegs;        ///< Register overlay
    CSL_IsifRegsOvly        isifRegs;       ///< Register overlay    
    CSL_IpipeRegsOvly       ipipeRegs;      ///< Register overlay        
    CSL_IpipeifRegsOvly     ipipeifRegs;    ///< Register overlay
    CSL_RszRegsOvly         rszRegs;        ///< Register overlay                    
    CSL_H3aRegsOvly         h3aRegs;        ///< Register overlay
    CSL_LdcRegsOvly         ldcRegs;        ///< Register overlay                                            
    CSL_FaceDetectRegsOvly  faceDetectRegs; ///< Register overlay
#endif

  } CSL_VpssObj;

  typedef CSL_VpssObj *CSL_VpssHandle;  ///< Module Handle

// functions

/**
  \brief Open Module Handle

  \param hndl   [ O]  Module Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_vpssOpen(CSL_VpssHandle hndl);

/**
  \brief Close Module Handle

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_vpssClose(CSL_VpssHandle hndl);

/**
  \brief Set all module registers

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_vpssHwSetup(CSL_VpssHandle hndl, CSL_VpssHwSetup * data);

/**
  \brief Set all module registers to power-ON reset value

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_vpssHwReset(CSL_VpssHandle hndl);

/**
  \brief VPSS clock config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_vpssSetClkConfig(CSL_VpssHandle hndl, CSL_VpssClkConfig * data);

/**
  \brief VPSS interrupt/event config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_vpssSetIntEvtConfig(CSL_VpssHandle hndl, CSL_VpssIntEvtConfig * data);

/**
  \brief VPSS read/write priority config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_vpssSetPriConfig(CSL_VpssHandle hndl, CSL_VpssPriConfig * data);

/**
  \brief VPSS WBL config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_vpssSetWblSelConfig(CSL_VpssHandle hndl, CSL_VpssWblSelConfig * data);

/**
  \brief Module lock : Mutex Lock

  \param hndl     [I ] hndl
  \param timeout  [I ] CSL_SYS_TIMEOUT_NONE or CSL_SYS_TIMEOUT_FOREVER

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_vpssLock(CSL_VpssHandle hndl, Uint32 timeout);

/**
  \brief Module unlock : Mutex Unlock

  \param hndl     [I ] hndl

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_vpssUnlock(CSL_VpssHandle hndl);


/**
  \brief return CLKDIV values for different modules
  
  \param hndl     [I ] hndl
  \param prm      [ O] parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_vpssGetClkDiv(CSL_VpssHandle hndl, CSL_VpssClkDiv *prm);

/**
  \brief Set CLKDIV values for different modules
  
  \param hndl     [I ] hndl
  \param prm      [I ] parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_vpssSetClkDiv(CSL_VpssHandle hndl, CSL_VpssClkDiv *prm);


#ifdef __cplusplus
}
#endif
/*@}*/
#endif
/**
  \ingroup CSL_VPSS
  \defgroup  CSL_VPFE VPFE

  \brief Video Processing Front End modules
*/
