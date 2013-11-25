/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#ifndef _CSL_H3A_H_
#define _CSL_H3A_H_

#ifdef __cplusplus
extern  "C" {
#endif

#include <csl_soc.h>
#include <csl_buf.h>

/**
  \file csl_h3a.h
  \brief H3A APIs
*/

/**
 \ingroup  CSL_VPFE
 \defgroup CSL_H3A H3A
 \brief Hardware 3A (H3A)

 @{
*/

// define's


#define CSL_H3A_MOD_AF      0   ///< H3A module : Auto Focus
#define CSL_H3A_MOD_AEWB    1   ///< H3A module : Auto Exposure/White Balance
#define CSL_H3A_MOD_MAX     2   ///< H3A module : Max number of modules

#define CSL_H3A_OUTFMT_SUM_OF_SQUARES  0  ///< AE/AWB output format : Sum of squares
#define CSL_H3A_OUTFMT_MIN_MAX         1  ///< AE/AWB output format : Min/Max
#define CSL_H3A_OUTFMT_SUM             2  ///< AE/AWB output format : Sum

#define CSL_H3A_AF_RGBPOS_GR_GB   0 ///< RGB Pixel location in AF window : GR and GB as Bayer pattern
#define CSL_H3A_AF_RGBPOS_RG_GB   1 ///< RGB Pixel location in AF window : RG and GB as Bayer pattern
#define CSL_H3A_AF_RGBPOS_GR_BG   2 ///< RGB Pixel location in AF window : GR and BG as Bayer pattern
#define CSL_H3A_AF_RGBPOS_RG_BG   3 ///< RGB Pixel location in AF window : RG and BG as Bayer pattern
#define CSL_H3A_AF_RGBPOS_GG_RB   4 ///< RGB Pixel location in AF window : GG and RB as Custom pattern
#define CSL_H3A_AF_RGBPOS_RB_GG   5 ///< RGB Pixel location in AF window : RB and GG as Custom pattern

#define CSL_H3A_AF_FVMODE_SUM    0  ///< Focus Value accumulation mode : Sum mode
#define CSL_H3A_AF_FVMODE_PEAK   1  ///< Focus Value accumulation mode : Peak mode

// data structure's

/**
  \brief  AE/AWB configuration structure
*/
  typedef struct {

    Bool32    enable;             ///< AE/AWB Engine, TRUE: Enable, FALSE: Disable
    Bool32    alawEnable;         ///< A-Law table, TRUE: Enable, FALSE: Disable
    Bool32    medFiltEnable;      ///< Median filter, TRUE: Enable, FALSE: Disable
    Uint16  satLimit;           ///< Saturation limit for sub sampled pixels

    Uint16  winWidth;           ///< Window Width, Units: pixels, Must be even
    Uint16  winHeight;          ///< Window Height, Units: pixels, Must be even
    Uint16  winNumH;            ///< Number of windows in horizontal direction, Must be between 2 to 36
    Uint16  winNumV;            ///< Number of windows in vertical direction, Must be less than or equal to 128
    Uint16  winStartH;          ///< Horizontal window start position, Must be in the range of 0-4095
    Uint16  winStartV;          ///< Vertical window start position,  Must be in the range of 0-4095
    Uint16  winIncH;            ///< Horizontal sampling point increment, Must be within 2 to 32
    Uint16  winIncV;            ///< Vertical sampling point increment, Must be within 2 to 32

    Uint16  winBlackStartV;     ///< Vertical window start position for the single black line of windows, range: 0-4095
    Uint16  winBlackHeight;     ///< Window height for the single black line of windows, Units: pixels, Must be even

    Uint8  *outputAddr;         ///< Output Address, must be 32-byte aligned

    Uint8   outFormat;          ///< CSL_H3A_OUTFMT_SUM_OF_SQUARES \n CSL_H3A_OUTFMT_MIN_MAX \n CSL_H3A_OUTFMT_SUM
    Uint8   shift;              ///< Shift value for the sum of pixel values, Must be in the range of 0-15

  } CSL_H3aAewbConfig;

/**
  \brief  Frame configuration structure
*/
  typedef struct {

    Uint16  startV;             ///< Start line vertical, specifies the no. of lines after the VD rising edge after which the real frame starts
    Uint16  line_start;         ///< Line start, start pixel of the CCDC interface module into the line buffer
    Uint8   medFiltThreshold;   ///< Median filter threshold

  } CSL_H3aInFrameConfig;

/**
  \brief  AF configuration structure
*/
  typedef struct {

    Bool32    enable;             ///< AF Engine, TRUE: Enable, FALSE: Disable
    Bool32    alawEnable;         ///< A-Law table, TRUE: Enable, FALSE: Disable
    Bool32    vfEnable;           ///< Vertical Focus, TRUE: Enable, FALSE: Disable
    Bool32    medFiltEnable;      ///< Median filter, TRUE: Enable, FALSE: Disable

    Uint8   rgbPos;             ///< CSL_H3A_AF_RGBPOS_GR_GB ... CSL_H3A_AF_RGBPOS_RB_GG, value is used only when VF is disabled
    Uint8   fvAccMode;          ///< CSL_H3A_AF_FVMODE_SUM \n CSL_H3A_AF_FVMODE_PEAK

    Uint16  paxWidth;           ///< Paxel width, Must be greater than or equal to 8 pixels
    Uint16  paxHeight;          ///< Paxel height, Must be even within the range 2-51
    Uint16  paxIncH;            ///< Column increment, Must be even in the range 2-30
    Uint16  paxIncV;            ///< Line increment, Must be even in the range 0-30
    Uint16  paxNumH;            ///< Horizontal paxel count, Must be in the range 2-36 and less than 12, if VF is enabled
    Uint16  paxNumV;            ///< Vertical paxel count, Must be less than or equal to 128 and less than 12, if VF is enabled
    Uint16  paxStartH;          ///< Paxel horizontal start position, Must be in the rangle 2-4094 and greater than or equal to (IIR Horizontantal start position + 2), Must be even if vertical mode is not enabled
    Uint16  paxStartV;          ///< Paxel vertical start position, Must be in the range 0-4095 and greater than or equal to 8 if verical mode is enabled

    Uint16  iirStartH;          ///< IIR Horizontal start position, Must be in the range 0-4094, Must be even if vertical mode is not enabled and if it is enabled, lower bit must be same as paxel horizontal start position

    Uint8  *outputAddr;         ///< Output Address, must be 32-byte aligned

    Int32  iirCoeff0[11];      ///< IIR filter coefficient #0, Must be in the range of signed -32 <= value <= 31 +63/64
    Int32  iirCoeff1[11];      ///< IIR filter coefficient #1, Must be in the range of signed -32 <= value <= 31 +63/64

    Int32   vfvFir1Coeff[5];    ///< Vertical FV FIR 1 coefficient 0-5
    Uint16  vfvFir1Threshold;   ///< Vertical FV FIR 1 threshold
    Uint16  hfvFir1Threshold;   ///< Horizontal FV FIR 1 threshold

    Int32   vfvFir2Coeff[5];    ///< Vertical FV FIR 2 coefficient 0-5
    Uint16  vfvFir2Threshold;   ///< Vertical FV FIR 2 threshold
    Uint16  hfvFir2Threshold;   ///< Horizontal FV FIR 2 threshold

  } CSL_H3aAfConfig;

/**
  \brief  AF packet format with vertical AF disabled
*/
  typedef struct {

    Uint32  hfvSum_0;
    Uint32  hfv1_0;
    Uint32  hfv2_0;
    Uint32  reserved_0;

    Uint32  hfvSum_1;
    Uint32  hfv1_1;
    Uint32  hfv2_1;
    Uint32  reserved_1;

    Uint32  hfvSum_2;
    Uint32  hfv1_2;
    Uint32  hfv2_2;
    Uint32  reserved_2;

  } CSL_H3aAfOutVfDisableOverlay;

/**
  \brief  AF packet format with vertical AF enabled
*/
  typedef struct {

    Uint32  hfvSum;
    Uint32  hfv1;
    Uint32  hfv1Square;
    Uint32  hfv1Count;

    Uint32  hfv2;
    Uint32  hfv2Square;
    Uint32  hfv2Count;
    Uint32  reserved_0;

    Uint32  vfv1;
    Uint32  vfv1Square;
    Uint32  vfv1Count;
    Uint32  reserved_1;

    Uint32  vfv2;
    Uint32  vfv2Square;
    Uint32  vfv2Count;
    Uint32  reserved_2;

  } CSL_H3aAfOutVfEnableOverlay;

/**
  \brief  AE/AWB packet format for sum-only mode
*/
  typedef struct {

    Uint16  subSampleAcc[4];
    Uint16  saturatorAcc[4];
    Uint32  sumOfSquares[4];    ///< AE/AWB packet format for sum of square mode

  } CSL_H3aAewbOutSumModeOverlay;

/**
  \brief  AE/AWB packet format for min-max mode
*/
  typedef struct {

    Uint16  subSampleAcc[4];
    Uint16  saturatorAcc[4];
    Uint16  min[4];
    Uint16  max[4];

  } CSL_H3aAewbOutMinMaxModeOverlay;

/**
  \brief  AE/AWB unsaturated block count
*/
  typedef struct {

    Uint16  unsatCount[8];

  } CSL_H3aAewbOutUnsatBlkCntOverlay;

/**
  \brief  H3A Hardware configuration structure
*/
  typedef struct {

    CSL_H3aInFrameConfig *inFrameConfig;  ///< Frame config parameters if NULL do not set these parameters
    CSL_H3aAfConfig *afConfig;  ///< AF config parameters if NULL do not set these parameters
    CSL_H3aAewbConfig *aewbConfig;  ///< AE/AWB config parameters if NULL do not set these parameters

  } CSL_H3aHwSetup;

/**
  \brief Module Object Structure
*/
  typedef struct {

#ifdef CSL_KERNEL_API
    CSL_H3aRegsOvly regs; 	  ///< Register overlay				//Arun Moved down
    CSL_BufHndl outBuf[CSL_H3A_MOD_MAX];
    CSL_BufInfo curBufInfo[CSL_H3A_MOD_MAX];
    Bool32    outBufSwitchEnable[CSL_H3A_MOD_MAX];
#else
    int     fd;
#endif

  } CSL_H3aObj;

  typedef CSL_H3aObj *CSL_H3aHandle;  ///< Module Handle

// function's

/**
  \brief Open Module Handle

  \param hndl   [ O]  Module Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aOpen(CSL_H3aHandle hndl);

/**
  \brief Close Module Handle

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aClose(CSL_H3aHandle hndl);

/**
  \brief Set all module registers

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aHwSetup(CSL_H3aHandle hndl, CSL_H3aHwSetup * data);

/**
  \brief Set all module registers to power-ON reset value

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aHwReset(CSL_H3aHandle hndl);

/**
  \brief H3A frame config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aInFrameConfig(CSL_H3aHandle hndl, CSL_H3aInFrameConfig * data);

/**
  \brief Enable/Disable AF

  \param hndl   [I ]  Handle
  \param enable [I ]  TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aAfEnable(CSL_H3aHandle hndl, Bool32 enable);

/**
  \brief Enable/Disable AF Vertical Focus

  \param hndl   [I ]  Handle
  \param enable [I ]  TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aAfVfEnable(CSL_H3aHandle hndl, Bool32 enable);

/**
  \brief Enable/Disable AF A-Law Table

  \param hndl   [I ]  Handle
  \param enable [I ]  TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aAfAlawEnable(CSL_H3aHandle hndl, Bool32 enable);

/**
  \brief Set output address for AF data

  \param hndl   [I ]  Handle
  \param addr   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aAfSetOutAddr(CSL_H3aHandle hndl, Uint8 * addr);

/**
  \brief H3A AF config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aAfSetConfig(CSL_H3aHandle hndl, CSL_H3aAfConfig * data);

/**
  \brief Enable/Disable AE/AWB

  \param hndl   [I ]  Handle
  \param enable [I ]  TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aAewbEnable(CSL_H3aHandle hndl, Bool32 enable);

/**
  \brief Enable/Disable AE/AWB A-Law Table

  \param hndl   [I ]  Handle
  \param enable [I ]  TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aAewbAlawEnable(CSL_H3aHandle hndl, Bool32 enable);

/**
  \brief Set output address for AE/AWB data

  \param hndl   [I ]  Handle
  \param addr   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aAewbSetOutAddr(CSL_H3aHandle hndl, Uint8 * addr);

/**
  \brief H3A AE/AWB config

  \param hndl    [I ]  Handle
  \param data    [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aAewbSetConfig(CSL_H3aHandle hndl, CSL_H3aAewbConfig * data);

/**
  \brief Buffer init

  \param hndl     [I ] hndl
  \param h3aModId [I ] module ID
  \param bufInit  [I ] Buffer init parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aBufInit(CSL_H3aHandle hndl, Uint8 h3aModId, CSL_BufInit * bufInit);

/**
  \brief Buffer switch in ISR enable/disable

  \param hndl     [I ] hndl
  \param h3aModId [I ] module ID
  \param enable   [I ] TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aBufSwitchEnable(CSL_H3aHandle hndl, Uint8 h3aModId, Bool32 enable);

/**
  \brief Get Full Buffer

  \param hndl     [I ] hndl
  \param h3aModId [I ] module ID
  \param buf      [O ] Full buffer info
  \param minBuf   [I ] minimum number of buffers that should be full
  \param timeout  [I ] Timeout

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aBufGetFull(CSL_H3aHandle hndl, Uint8 h3aModId, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout);

/**
  \brief Put Empty Buffer

  \param hndl       [I ] hndl
  \param h3aModId   [I ] module ID
  \param buf        [I ] Empty buffer info

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aBufPutEmpty(CSL_H3aHandle hndl, Uint8 h3aModId, CSL_BufInfo * buf);

/**
  \brief Module lock : Mutex Lock

  \param hndl     [I ] hndl
  \param timeout  [I ] CSL_SYS_TIMEOUT_NONE or CSL_SYS_TIMEOUT_FOREVER

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aLock(CSL_H3aHandle hndl, Uint32 timeout);

/**
  \brief Module unlock : Mutex Unlock

  \param hndl     [I ] hndl

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_h3aUnlock(CSL_H3aHandle hndl);


#ifdef __cplusplus
}
#endif
/*@}*/
#endif
