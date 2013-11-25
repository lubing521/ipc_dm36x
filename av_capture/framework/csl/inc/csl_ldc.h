/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#ifndef _CSL_LDC_H_
#define _CSL_LDC_H_

#ifdef __cplusplus
extern  "C" {
#endif

#include <csl_soc.h>

/**
  \file csl_ldc.h
  \brief LDC APIs
*/

/**
 \ingroup  CSL_VPFE
 \defgroup CSL_LDC LDC
 \brief Lens Distortion Correction (LDC)

 @{
*/

// define's
#define CSL_LDC_STATE_IDLE 0    ///< Lens Distortion Correction State : Idle
#define CSL_LDC_STATE_BUSY 1    ///< Lens Distortion Correction State : Busy

#define CSL_LDC_INPUT_MODE_YUV422_LD 0  ///< Lens Distortion Correction Input Data Mode : YCbCr 4:2:2 Lens distortion
#define CSL_LDC_INPUT_MODE_BAYER_CAC 1  ///< Lens Distortion Correction Input Data Mode : Bayer mode chromatic aberration
#define CSL_LDC_INPUT_MODE_YUV420_LD 2  ///< Lens Distortion Correction Input Data Mode : YCbCr 4:2:0 Lens distortion

#define CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT 0  ///< Lens Distortion Correction Bayer Format : Unpacked 12-bit data
#define CSL_LDC_BAYER_FORMAT_PACKED_12_BIT   1  ///< Lens Distortion Correction Bayer Format : Packed 12-bit data
#define CSL_LDC_BAYER_FORMAT_PACKED_8_BIT    2  ///< Lens Distortion Correction Bayer Format : Packed 8-bit data
#define CSL_LDC_BAYER_FORMAT_A_LAW_8_BIT     3  ///< Lens Distortion Correction Bayer Format : A-Law data (8-bit)

#define CSL_LDC_BAYER_START_PHASE_R   0 ///< LDC Initial color for LD back mapping : R
#define CSL_LDC_BAYER_START_PHASE_GR  1 ///< LDC Initial color for LD back mapping : GR
#define CSL_LDC_BAYER_START_PHASE_GB  2 ///< LDC Initial color for LD back mapping : GB
#define CSL_LDC_BAYER_START_PHASE_B   3 ///< LDC Initial color for LD back mapping : B

#define CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC  0 ///< Interpolation type for Y data : Bicubic Interpolation
#define CSL_LDC_Y_INTERPOLATION_TYPE_BILINEAR 1 ///< Interpolation type for Y data : Bilinear Interpolation

#define CSL_LDC_TABLE_MAX_ENTRIES   (256)  ///< Max entires in LDC Table 

// data structure's

/**
  \brief LDC frame configuration
*/
  typedef struct {

    Uint32   outBlkH;            ///< Output block height . Must be > 0 , even and lesser than or equal to frame width
    Uint32   outBlkV;            ///< Output block width .Must be > 0,multiple of 16, 32 or 64 and lesser than or equal to frame height
    Uint32   pixelPad;           ///< Number of pixels to be padded , must be greater than 0 .

    Uint32  frameWidth;         ///< Frame width,must be multiple of outBlkH
    Uint32  frameHeight;        ///< Frame height, must be multiple of outBlkV

    Uint32  initX;              ///< Output starting X coordinate (must be even)
    Uint32  initY;              ///< Output starting Y coordinate (must be even)

    Uint8  *inputAddr;          ///< Read frame base address , must be 32 bit aligned
    Uint32  inputLineOffset;    ///< Read frame line offset , in bytes .must be 32 bit aligned
    Uint8  *inputAddr420C;      ///< Read frame base address for Chroma (YUV420 mode), must be 32 bit aligned

    Uint8  *outputAddr;         ///< Write frame base address , must be 32 bit aligned
    Uint32  outputLineOffset;   ///< Write frame line offset , in bytes .must be 32 bit aligned
    Uint8  *outputAddr420C;     ///< Write frame base address for Chroma (YUV420 mode), must be 32 bit aligned

  } CSL_LdcFrameConfig;

/**
  \brief LDC Hardware Setup configuration
*/
  typedef struct {

    Bool32    enable;             ///< TRUE:Enable LDC \n FALSE:Disable LDC

    Bool32    ldMapEnable;        ///< TRUE:LD Mapping Enable \n FALSE:LD Mapping Disable

    Uint32   inputType;          ///< CSL_LDC_INPUT_MODE_YUV422_LD \n CSL_LDC_INPUT_MODE_BAYER_CAC \n CSL_LDC_INPUT_MODE_YUV420_LD

    Uint32   bayerType;          ///< CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT \n CSL_LDC_BAYER_FORMAT_PACKED_12_BIT \n CSL_LDC_BAYER_FORMAT_PACKED_8_BIT \n CSL_LDC_BAYER_FORMAT_A_LAW_8_BIT only for Bayer input

    Uint32   bayerStartPhase;    ///< CSL_LDC_BAYER_START_PHASE_R ...CSL_LDC_BAYER_START_PHASE_B, only for Bayer input

    Uint32   interpolationTypeY; ///< CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC \n CSL_LDC_Y_INTERPOLATION_TYPE_BILINEAR

    Uint32   backMapRightShift;  ///< Right shift bits in back mapping . 0..15
    Uint32   backMapThreshold;   ///< Threshold in back mapping

    Uint32 *table;              ///< LDC Table, must be less than CSL_LDC_TABLE_MAX_ENTRIES

    Uint32  lensCenterX;        ///< Lens center X coordinate
    Uint32  lensCenterY;        ///< Lens center Y coordinate

    Uint32   kvh;                ///< Vertical upper scaling factor
    Uint32   kvl;                ///< Vertical lower scaling factor
    Uint32   khr;                ///< Horizontal right scaling factor
    Uint32   khl;                ///< Horizontal left scaling factor

    Uint32  affineTranswrap[6]; ///< Affine Transwrap register A to F

    CSL_LdcFrameConfig *frameConfig;  ///< Frame config parameters if NULL do not set these parameters

  } CSL_LdcHwSetup;

/**
  \brief Module Object Structure
*/
  typedef struct {

#ifdef CSL_KERNEL_API
    CSL_LdcRegsOvly regs;       ///< Register overlay
    CSL_SysFlag intFlag;
#else
    int     fd;
#endif

  } CSL_LdcObj;

  typedef CSL_LdcObj *CSL_LdcHandle;  ///< Module Handle

// function's

/**
  \brief Open Module Handle

  \param hndl   [ O]  Module Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ldcOpen(CSL_LdcHandle hndl);

/**
  \brief Close Module Handle

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ldcClose(CSL_LdcHandle hndl);

/**
  \brief Set all module registers

  \param hndl   [I ]  Handle
  \param setup  [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ldcHwSetup(CSL_LdcHandle hndl, CSL_LdcHwSetup * setup);

/**
  \brief Set all module registers to power-ON reset value

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ldcHwReset(CSL_LdcHandle hndl);

/**
  \brief LDC frame config

  \param hndl           [I ]  Handle
  \param frameConfig    [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ldcFrameConfig(CSL_LdcHandle hndl, CSL_LdcFrameConfig * frameConfig);

/**
  \brief Enable/Disable LDC

  \param hndl   [I ]  Handle
  \param enable [I ]  TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ldcEnable(CSL_LdcHandle hndl, Bool32 enable);

/**
  \brief Checks the status of LDC

  \param hndl   [I ]  Handle
  \param isBusy [ O]  TRUE: LDC is busy , FALSE: LDC is idle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ldcIsBusy(CSL_LdcHandle hndl, Bool32 * isBusy);

/**
  \brief Writes to LDC look up table

  \param hndl   [I ]  Handle
  \param table  [I ]  Pointer to table

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ldcWriteTable(CSL_LdcHandle hndl, Uint32 * table);

/**
  \brief Reads from LDC look up table

  \param hndl   [I ]  Handle
  \param table  [ O]  Pointer to table

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ldcReadTable(CSL_LdcHandle hndl, Uint32 * table);

/**
  \brief Enable/Disable module interrupt
  
  \param hndl     [I ] hndl
  \param enable   [I ] TRUE: enable, FALSE: disable
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
  CSL_Status CSL_ldcIntEnable(CSL_LdcHandle hndl, Bool32 enable);

/**
  \brief Clear module interrupt
  
  \param hndl     [I ] hndl
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
  CSL_Status CSL_ldcIntClear(CSL_LdcHandle hndl);

/**
  \brief Wait for module interrupt
  
  \param hndl     [I ] hndl
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
  CSL_Status CSL_ldcIntWait(CSL_LdcHandle hndl);

/**
  \brief Module lock : Mutex Lock
  
  \param hndl     [I ] hndl
  \param timeout  [I ] CSL_SYS_TIMEOUT_NONE or CSL_SYS_TIMEOUT_FOREVER
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
  CSL_Status CSL_ldcLock(CSL_LdcHandle hndl, Uint32 timeout);

/**
  \brief Module unlock : Mutex Unlock
  
  \param hndl     [I ] hndl
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
  CSL_Status CSL_ldcUnlock(CSL_LdcHandle hndl);

#ifdef __cplusplus
}
#endif
/*@}*/
#endif
