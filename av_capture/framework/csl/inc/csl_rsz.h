/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#ifndef _CSL_RSZ_H_
#define _CSL_RSZ_H_

#ifdef __cplusplus
extern  "C" {
#endif

#include <csl_soc.h>
#include <csl_buf.h>

/**
  \file csl_rsz.h
  \brief Resizer APIs
*/

/**
 \ingroup  CSL_VPFE
 \defgroup CSL_RSZ Resizer
 \brief Resizer (RSZ)

 @{
*/

// define's

#define CSL_RSZ_A          0    ///< Resize Channel : Channel A
#define CSL_RSZ_B          1    ///< Resize Channel : Channel B
#define CSL_RSZ_CH_MAX     2    ///< Resize Channel : Max

#define CSL_RSZ_MODE_CONTINUOUS 0 ///< Resizer Mode : Continuous
#define CSL_RSZ_MODE_ONE_SHOT 1 ///< Resizer Mode : One shot

#define CSL_RSZ_TYPE_4_TAP_CUBIC_CONVOLUTION 0  ///< Resizer Method : 4-tap cubic convolution
#define CSL_RSZ_TYPE_2_TAP_LINEAR_CONVOLUTION  1  ///< Resizer Method : 2-tap linear interpolation

#define CSL_RSZ_DOWN_SCALE_BY_2 0 ///< Resizer downscale ratio : 1/2
#define CSL_RSZ_DOWN_SCALE_BY_4 1 ///< Resizer downscale ratio : 1/4
#define CSL_RSZ_DOWN_SCALE_BY_8 2 ///< Resizer downscale ratio : 1/8
#define CSL_RSZ_DOWN_SCALE_BY_16 3  ///< Resizer downscale ratio : 1/16
#define CSL_RSZ_DOWN_SCALE_BY_32 4  ///< Resizer downscale ratio : 1/32
#define CSL_RSZ_DOWN_SCALE_BY_64 5  ///< Resizer downscale ratio : 1/64
#define CSL_RSZ_DOWN_SCALE_BY_128 6 ///< Resizer downscale ratio : 1/128
#define CSL_RSZ_DOWN_SCALE_BY_256 7 ///< Resizer downscale ratio : 1/256

#define CSL_RSZ_YUV_OUT 0       ///< Resizer output mode : YUV output
#define CSL_RSZ_RGB_OUT 1       ///< Resizer output mode : RGB output

#define CSL_RSZ_MODE_MASK_PIXELS 0  ///< Resizer mode : Mask first or last two pixels for 4:2:2 to 4:4:4 conversion
#define CSL_RSZ_MODE_OUTPUT_PIXELS 1  ///< Resizer mode : Output first or last two pixels for 4:2:2 to 4:4:4 conversion

#define CSL_RSZ_OUTPUT_BITS_16 0  ///< Resizer Output Number of bits : 16
#define CSL_RSZ_OUTPUT_BITS_32 1  ///< Resizer Output Number of bits : 32

#define CSL_RSZ_PASS_THROUGH_MODE_INPUT_YCbCr 0 ///< Input format for passthrough mode : YCbCr data
#define CSL_RSZ_PASS_THROUGH_MODE_INPUT_RAW   1 ///< Input format for passthrough mode : Raw data

#define CSL_RSZ_INPUT_DATA_PATH_IPIPE 0 ///< Data Path through RSZ : from IPIPE
#define CSL_RSZ_INPUT_DATA_PATH_IPIPEIF 1 ///< Data Path through RSZ : from IPIPEIF

#define CSL_RSZ_CHROMA_FORMAT_422 0 ///< Chroma Format Selection : 422 image
#define CSL_RSZ_CHROMA_FORMAT_420 1 ///< Chroma Format Selection : 420 image

#define CSL_RSZ_420_COL_SELECT_Y 0  ///< Y/C Selection  in 420 input mode : Y
#define CSL_RSZ_420_COL_SELECT_C 1  ///< Y/C Selection  in 420 input mode : C

#define CSL_RSZ_CHROMA_POS_SAME_AS_Y 0  ///< Phase position of the output of the Chrominance : same position with Luminance
#define CSL_RSZ_CHROMA_POS_MID_OF_Y  1  ///< Phase position of the output of the Chrominance : the middle of the Luminance

// data structure's
/**
  \brief Resizer input configuration
*/
  typedef struct {

    Bool32    wenUseEnable;       ///< CAM_WEN Mode Selection \n TRUE: use the ipipeif_wrt \n FALSE: does not use ipipeif_wrt
    Bool32    passThruEnable;     ///< TRUE : Enable Passthrough mode \n FALSE : Disable pass through mode
    Uint8   passThruInFormat;   ///< CSL_RSZ_PASS_THROUGH_MODE_INPUT_YCbCr \n CSL_RSZ_PASS_THROUGH_MODE_INPUT_RAW

    Uint16  inputDataPath;      ///< CSL_RSZ_INPUT_DATA_PATH_IPIPE \n CSL_RSZ__INPUT_DATA_PATH_IPIPEIF

    Uint8   chormaFormatType;   ///< CSL_RSZ_CHROMA_FORMAT_422 \n CSL_RSZ_CHROMA_FORMAT_420
    Uint8   yuv420Color;        ///< CSL_RSZ_420_COL_SELECT_Y \n CSL_RSZ_420_COL_SELECT_C

    Uint16  chormaPos;          ///< CSL_RSZ_CHROMA_POS_SAME_AS_Y \n CSL_RSZ_CHROMA_POS_MID_OF_Y
    Bool32    chormaFlip;         ///< TRUE:Flipping disable \n FALSE:Flipping enable

  } CSL_RszInConfig;

/**
  \brief Input frame configuration
*/
  typedef struct {

    Uint16  inStartX;           ///< Horizontal start position , in pixels
    Uint16  inStartY;           ///< Verical start position , in lines

    Uint16  inWidth;            ///< Horizontal size of processing area , in pixels
    Uint16  inHeight;           ///< Verical size of processing area , in lines

  } CSL_RszInFrameConfig;

/**
  \brief Dzoom configuration
*/
  typedef struct {

    Uint16  inStartX;           ///< Horizontal start position , in pixels
    Uint16  inStartY;           ///< Verical start position , in lines

    Uint16  inWidth;            ///< Horizontal size of processing area , in pixels
    Uint16  inHeight;           ///< Verical size of processing area , in lines
    
    Uint16  hrsz[2];            ///< Resize ratios for Rsz A, Rsz B
    Uint16  vrsz[2];            ///< Resize ratios for Rsz A, Rsz B

  } CSL_RszDzoomConfig;

/**
  \brief Resizer Clock configuration
*/
  typedef struct {

    Bool32    mmrClkEnable;       ///< TRUE:MMR clock enable \n FALSE:MMR clock disable
    Bool32    coreClkEnable;      ///< TRUE:RSZ Core Clock Enable \n FALSE:RSZ Core Clock Disable

  } CSL_RszClkConfig;

/**
  \brief Resizer Min-Max configuration
*/
  typedef struct {

    Uint8   minY;               ///< Minimum Luminance Value
    Uint8   maxY;               ///< Maximum Luminance Value

    Uint8   minC;               ///< Minimum Chrominance Value
    Uint8   maxC;               ///< Maximum Chrominance Value

  } CSL_RszMinMaxConfig;

/**
  \brief Resizer ouptut configuration
*/
  typedef struct {

    Bool32    flipH;              ///< TRUE : Enable horizontal flipping \n FALSE:Disable horizontal flipping
    Bool32    flipV;              ///< TRUE : Enable vertical flipping \n FALSE:Disable vertical flipping

    Uint16  irqInterval;        ///< Interval of  circular IRQ Interrupt signal
    Uint8   dmaInterval;        ///< Minimum interval between two consecutive SDRAM requests

    Bool32    enable;             ///< TRUE: Enable Resize Channel \n FALSE :Disable Resize Channel
    Bool32    oneShotEnable;      ///< CSL_RSZ_MODE_CONTINUOUS \n CSL_RSZ_MODE_ONE_SHOT 1

    Bool32    yuv420YOutEnable;   ///< TRUE:Enable Luminance output \n FALSE:Enable Luminance output
    Bool32    yuv420COutEnable;   ///< TRUE:Enable Chrominance output \n FALSE:Enable Chrominance output

    Uint16  inStartX;           ///< Horizontal start position of image processing , in pixels
    Uint16  inStartY;           ///< Vertical start position of image processing , in lines

    Uint16  outWidth;           ///< Horizontal size of output image , in pixels
    Uint16  outHeight;          ///< Vertical size of output image , in lines

    Uint16  vertStartPhaseY;    ///< Initial Phase of Vertical Resizing Process for Luminance
    Uint16  vertStartPhaseC;    ///< Initial Phase of Vertical Resizing Proces for Chrominance

    Uint16  vertReszRatio;      ///< [I ] resize ratio, if set to 0, auto calculate \n [ O] Actual resize ratio that is set

    Uint8   vertReszTypeY;      ///< CSL_RSZ_TYPE_4_TAP_CUBIC_CONVOLUTION \n CSL_RSZ_TYPE_2_TAP_LINEAR_CONVOLUTION
    Uint8   vertReszTypeC;      ///< CSL_RSZ_TYPE_4_TAP_CUBIC_CONVOLUTION \n CSL_RSZ_TYPE_2_TAP_LINEAR_CONVOLUTION
    Uint8   vertLpfIntensityY;  ///< Vertical LPF Intensity for Luminance
    Uint8   vertLpfIntensityC;  ///< Vertical LPF Intensity for Chrominance

    Uint16  horzStartPhase;     ///< Initial Phase of Horizontal Resizing Process
    Uint16  horzStartPhaseAdd;  ///< Additonal Phase of Horizontal Resizing Process

    Uint16  horzReszRatio;      ///< [I ] resize ratio, if set to 0, auto calculate \n [ O] Actual resize ratio that is set

    Uint8   horzReszTypeY;      ///< CSL_RSZ_TYPE_4_TAP_CUBIC_CONVOLUTION \n CSL_RSZ_TYPE_2_TAP_LINEAR_CONVOLUTION
    Uint8   horzReszTypeC;      ///< CSL_RSZ_TYPE_4_TAP_CUBIC_CONVOLUTION \n CSL_RSZ_TYPE_2_TAP_LINEAR_CONVOLUTION
    Uint8   horzLpfIntensityY;  ///< Horizontal LPF Intensity for Luminance
    Uint8   horzLpfIntensityC;  ///< Horizontal LPF Intensity for Chrominance

    Bool32    downScaleModeEnable;  ///< TRUE : Enable downscale mode \n FALSE: Disable downscale mode
    Uint8   downScaleModeAvgSizeV;  ///< CSL_RSZ_DOWN_SCALE_BY_2 ...CSL_RSZ_DOWN_SCALE_BY_256
    Uint8   downScaleModeAvgSizeH;  ///< CSL_RSZ_DOWN_SCALE_BY_2 ...CSL_RSZ_DOWN_SCALE_BY_256

    Bool32    rgbOutEnable;       ///< CSL_RSZ_YUV_OUT \n CSL_RSZ_RGB_OUT
    Bool32    rgbOutMaskFirstEnable;  ///< CSL_RSZ_MODE_MASK_PIXELS \n CSL_RSZ_MODE_OUTPUT_PIXELS
    Bool32    rgbOutMaskLastEnable; ///< CSL_RSZ_MODE_MASK_PIXELS \n CSL_RSZ_MODE_OUTPUT_PIXELS
    Uint8   rgbOutType;         ///< CSL_RSZ_OUTPUT_BITS_16 \n CSL_RSZ_OUTPUT_BITS_32
    Uint8   rgbOutAlphaVal;     ///< Alpha value used in 32 bit output mode

    Uint8  *outStartAddr;       ///< SDRAM Start Address. Lowest 2 bits should be "00" for flip and "11" for no flip. Lowest 3 bits should be "000" for RGB 32bit output
    Uint16  outLineOffset;      ///< SDRAM Line Offset , should be a multiple of 32 bytes
    Uint8  *outBaseAddr;        ///< SDRAM Base Address
    Uint16  outStartLine;       ///< Start Line of SDRAM Pointer , should be multiple of 4
    Uint16  outEndLine;         ///< End line of SDRAM Pointer , should be multiple of 4

    Uint8  *yuv420CoutStartAddr;  ///< SDRAM Start Address.Lowest 2 bits should be "00" for flip and "11" for no flip. Lowest 3 bits should be "000" for RGB 32bit output
    Uint16  yuv420CoutLineOffset; ///< SDRAM Line Offset , should be a multiple of 32 bytes
    Uint8  *yuv420CoutBaseAddr; ///< SDRAM Base Address
    Uint16  yuv420CoutStartLine;  ///< Start Line of SDRAM Pointer , should be multiple of 4
    Uint16  yuv420CoutEndLine;  ///< End line of SDRAM Pointer , should be multiple of 4

  } CSL_RszOutConfig;

/**
  \brief Output frame configuration
*/

  typedef struct {

    Uint16  outWidth;           ///< Output Width
    Uint16  outHeight;          ///< Output Height
    Uint16  outLineOffset;      ///< Output Line Offset in bytes
    Uint16  outBufHeight;       ///< Output Buffer height

    Uint16  horzReszRatio;      ///< [I ] resize ratio, if set to 0, auto calculate \n [ O] Actual resize ratio that is set
    Uint16  vertReszRatio;      ///< [I ] resize ratio, if set to 0, auto calculate \n [ O] Actual resize ratio that is set

  } CSL_RszOutFrameConfig;


  typedef struct {

    Uint16 format;      ///< [I ] YUV format: CSL_RSZ_CHROMA_FORMAT_422 or CSL_RSZ_CHROMA_FORMAT_420
    Uint16 width;       ///< [I ] Width in pixels
    Uint16 height;      ///< [I ] Height in lines
    Uint16 offsetH;     ///< [I ] Offset H in pixels
    Uint16 offsetV;     ///< [I ] Offset V in lines, Chroma offset is offsetH*offsetV bytes
  
  } CSL_RszBufConfig;


/**
  \brief Input frame configuration
*/
  typedef struct {

    Bool32    enable;             ///< TRUE: Enable Resizer \n FALSE: Disable resizer
    Bool32    oneShotEnable;      ///< CSL_RSZ_MODE_CONTINUOUS \n  CSL_RSZ_MODE_ONE_SHOT

    CSL_RszInConfig *inConfig;  ///< Input configuration parameters

    CSL_RszInFrameConfig *inFrameConfig;  ///< Input frame configuration parameters
    CSL_RszClkConfig *clkConfig;  ///< Clock configuration parameters
    CSL_RszMinMaxConfig *minMaxConfig;  ///< Min-Max configuration parameters

    CSL_RszOutConfig *rszAConfig; ///< Output configuration for resizer A
    CSL_RszOutConfig *rszBConfig; ///< Output configuration for resizer B

  } CSL_RszHwSetup;

/**
  \brief Resize Values Calculator Structure
*/
  typedef struct {

    Uint16  outputWidth;        ///< [I ] Width of output image (in pixels)
    Uint16  outputHeight;       ///< [I ] Height of output image (in lines)
    Uint16  inputWidth;         ///< [IO] Width of input image (in pixels) (Set to 0 if this is calculated based on outputWidth and rszH)*/
    Uint16  inputHeight;        ///< [IO] Height of input image (in lines) (Set to 0 if this is calculated based on outputHeight and rszV)
    Uint16  rszH;               ///< [IO] Horizontal resize value (represented by 256 / resizeRatio) (Set to 0 if this is calculated based on outputWidth and inputWidth)
    Uint16  rszV;               ///< [IO] Vertical resize value (represented by 256 / resizeRatio) (Set to 0 if this is calculated based on outputHeight and inputHeight)

  } CSL_RszCalcValues;

/**
  \brief Input frame configuration
*/
  typedef struct {

    Uint16  valLastPhase;       ///< Phase value of last line of the previous resize process
    Uint16  numLastLines;       ///< The number of actually produced lines of the previous resize process
    Uint16  vLastSdr;           ///< Vertical position of last line of the previous resize process in the output memory space

  } CSL_RszStatus;

/**
  \brief Resizer Slice Information

 All fields needed for setting the each vertical slice in frame divided-V mode.  These values are
 calculated by the CSL_rszCalcSlicePrm() function and is used in the #CSL_RszFmDivOutput struct.
*/
  typedef struct {

    Uint8   stPixIn;            ///< [ O] Resizer input horizontal start pixel
    Uint16  stPhase;            ///< [ O] Resizer horizontal start phase
    Uint16  outWidth;           ///< [ O] Resizer output width
    Uint8   stPixOut;           ///< [ O] Resizer output horizontal start pixel
    Uint8  *inAddr;             ///< [ O] IPIPEIF input address
    Uint8  *outAddr;            ///< [ O] Resizer output address

  } CSL_RszSliceInfo;

/**
  \brief Resizer Frame Division Mode Output Information

  All fields needed for setting the each the output specific parameters for a resizer instance.
  This structure is used in the #CSL_RszFmDivPrm struct.
*/
  typedef struct {

    Uint16  outputWidth;        ///< [I ] Output width of frame
    Uint16  outputHeight;       ///< [I ] Output height of frame
    Uint8  *outputAddr;         ///< [I ] Starting address of output image in SDRAM
    Uint16  outputLineOffsetH;  ///< [I ] Horizontal line offset

  /** [I ] Height of output buffer. Resize params will be setup such that after outputBufHeight
    is reached, data will be output again from outputAddr in a Ring buffer fashion.  If
    outputBufHeight is 0 or >= outputHeight, no ring buffer is implemented.
  */
    Uint16  outputBufHeight;

    Uint16  hRsz;               ///< [O ] Horizontal resize ratio value calculated by the CSL_rszCalcFmDivPrm() function
    Uint16  vRsz;               ///< [O ] Vertical resize ratio value calculated by the CSL_rszCalcFmDivPrm() function

    CSL_RszSliceInfo sliceInfo[4];  ///< [O ] Updated by CSL_rszCalcSlicePrm(), ax 4 vertical slices supported

  /** [O ] This should called with CSL_rszGetStatus()
    at least once for each row of blocks before calling the CSL_rszSetSlicePrm() function for the first
    block of a new row.
  */
    CSL_RszStatus rszStatus;

    Uint16  rpsNew;             ///< [ O] Internal parameter to calculate output line of next row of blocks

    Uint16  accOutLines;        ///< [ O] Internal parameter to keep track of cumulative number of output lines generated

  } CSL_RszFmDivOutput;


/**
  \brief Resizer Frame Division Mode Parameters

  All fields needed for setting slice based IPIPE + resize or just sliced base resize.
  This is used by the CSL_rszCalcFmDivPrm(), CSL_rszCalcSlicePrm(), CSL_rszSetFmDivPrm(), &
  CSL_rszSetSlicePrm() functions.
*/
  typedef struct {


    Uint16  inputWidth;         ///< [I ] Input width of frame
    Uint16  inputHeight;        ///< [I ] Input height of frame
    Uint8  *inputAddr;          ///< [I ] Starting address of image
    Uint16  inputLineOffsetH;   ///< [I ] Horizontal line offset

    CSL_RszFmDivOutput *rza;    ///< [I ] Configuration of RZA output parameters (if RZA is disabled, set to NULL)
    CSL_RszFmDivOutput *rzb;    ///< [I ] Configuration of RZB output parameters (if RZB is disabled, set to NULL)

  /** [I ] Overlap pixels per side due to any processing done before the resizer
    (i.e. CFA, NF, YUV444 to 422, & YEE)  This can either be set explicitly or automatically
    calculated by the API by passing the CSL_RSZ_AUTO_CALC value.  (NOTE: if auto-calc is
    selected, then the CSL_rszCalcFmDivPrm() function requires that other IPIPE parameters be
    set first so that the function can determine the overlap by reading the register values). */
    Int16   ipipeOverlapH;

  /** [I ] Overlap lines per side due to any processing done before the resizer
    (i.e. CFA, NF, & YEE)  This can either be set explicitly or automatically calculated
    by the API by passing the CSL_RSZ_AUTO_CALC value.  (NOTE: if auto-calc is selected,
    then the CSL_rszCalcFmDivPrm() function requires that other IPIPE parameters be set first
    so that the function can determine the overlap by reading the register values). */
    Int16   ipipeOverlapV;

  /** [I/O ] Number of input lines per resize (b value in TRM) if user wants to split image into
    horizontal slices (Frame Division Mode - H).  This can either be set explicitly or automatically
    calculated by the CSL_rszCalcFmDivPrm() function.  To auto-calculate based on the
    #numHorzSlices value, pass the CSL_RSZ_AUTO_CALC value. This value must be even.  This value
    may be slighly decrimented to an even number (no more than by 3) to simplify the computations. */
    Int16   numLinesPerResize;

  /** [I/O ] Number of horizontal slices if user wants to split image into horizontal slices
    (Frame Division Mode - H).  This can either be set explicitly or automatically
    calculated by the CSL_rszCalcFmDivPrm() function.  To auto-calculate based on the
    #numLinesPerResize value, pass the CSL_RSZ_AUTO_CALC value. */
    Int16   numHorzSlices;

    Uint16  numVertSlices;      ///< [ O] Number of vertical slices (Minimal number returned from the CSL_rszCalcFmDivPrm() )


    Uint16  totalOverlapH;      ///< [ O] Total horizontal overlap on each side (Returned from the CSL_rszCalcFmDivPrm() function)
    Uint16  totalOverlapV;      ///< [ O] Total vertical overlap on each side (Returned from the CSL_rszCalcFmDivPrm() function)

    Uint16  hNum;               ///< [ O] IPIPEIF & IPIPE input width (calculated by the CSL_rszCalcSlicePrm() function)

  /** [I/O ] If the frame is divided into horizontal slices, then this keeps track of the current
    row which is being operated on.  This is automatically updated by the CSL_rszSetSlicePrm()
    fuction. */
    Uint8   rowNum;

  /** [I/O ] If the frame is divided into vertical slices, then this keeps track of the current
    column which is being operated on.  This is automatically updated by the CSL_rszSetSlicePrm()
    fuction. */
    Uint8   colNum;

  } CSL_RszFmDivPrm;

/**
  \brief Resizer Runtime configuration per resizer A or B
*/
  typedef struct {
	
    Uint16  inStartX;           ///< Horizontal start position of image processing , in pixels; 
    //<for the input of this Resizer, different from inStartX of InFrameConfig
    Uint16  inStartY;           ///< Vertical start position of image processing , in lines
    Uint16  outWidth;           ///< Horizontal size of output image , in pixels
    Uint16  outHeight;          ///< Vertical size of output image , in lines
   
    Uint16  hrsz;              ///< Resize ratios for Rsz A, Rsz B
    Uint16  vrsz;              ///< Resize ratios for Rsz A, Rsz B

  } CSL_RszRuntimeConfig;

/**
  \brief Resizer Object
*/
  typedef struct {

    CSL_RszRegsOvly regs;       ///< Register overlay

#ifdef CSL_KERNEL_API
    CSL_BufHndl outBuf[CSL_RSZ_CH_MAX];
    CSL_BufInfo curBufInfo[CSL_RSZ_CH_MAX];
    Bool32    outBufSwitchEnable[CSL_RSZ_CH_MAX];
    Uint32  yuv420BufCoffset[CSL_RSZ_CH_MAX];
    CSL_SysFlag intFlag;
    int flipH[CSL_RSZ_CH_MAX];
    int flipV[CSL_RSZ_CH_MAX];
    int flipHOffsetYC[CSL_RSZ_CH_MAX];
    int flipVOffsetY[CSL_RSZ_CH_MAX];
    int flipVOffsetC[CSL_RSZ_CH_MAX];                            
    CSL_RszRuntimeConfig rszRtCfg[CSL_RSZ_CH_MAX];
    int rszRuntimeConfigFlag[CSL_RSZ_CH_MAX];
#else
    int     fd;
#endif

  } CSL_RszObj;

  typedef CSL_RszObj *CSL_RszHandle;  ///< Module Handle

// function's
/**
  \brief Open Module Handle

  \param hndl   [ O]  Module Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszOpen(CSL_RszHandle hndl);

/**
  \brief Close Module Handle

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszClose(CSL_RszHandle hndl);

/**
  \brief Set all module registers

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszHwSetup(CSL_RszHandle hndl, CSL_RszHwSetup * data);

/**
  \brief Set all module registers to power-ON reset value

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszHwReset(CSL_RszHandle hndl);

/**
  \brief Set Resizer Input  config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszSetInConfig(CSL_RszHandle hndl, CSL_RszInConfig * data);

/**
  \brief Set Resizer Input Frame config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszSetInFrameConfig(CSL_RszHandle hndl, CSL_RszInFrameConfig * data);

/**
  \brief Get Resizer Input Frame config

  \param hndl   [I ]  Handle
  \param data   [O ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszGetInFrameConfig(CSL_RszHandle hndl, CSL_RszInFrameConfig * data);

/**
  \brief Set Resizer Clock config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszSetClkConfig(CSL_RszHandle hndl, CSL_RszClkConfig * data);

/**
  \brief Set Resizer Min Max config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszSetMinMaxConfig(CSL_RszHandle hndl, CSL_RszMinMaxConfig * data);

/**
  \brief Set Resizer Output config

  \param hndl   [I ]  Handle
  \param rszMod [I ]  Resizer Channel A or B
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszSetOutConfig(CSL_RszHandle hndl, Uint8 rszMod, CSL_RszOutConfig * data);

  /**
	\brief Set Resizer Runtime config
  
	\param hndl   [I ]	Handle
	\param rszMod [I ]	Resizer Channel A or B
	\param data   [I ]	Input parameters
  
	\return CSL_SOK on success, else CSL_Exxxx
  */
  CSL_Status CSL_rszSetRuntimeConfig(CSL_RszHandle hndl,Uint8 rszMod,CSL_RszRuntimeConfig * data);

/**
  \brief Enable/Disable resizer

  \param hndl   [I]  Handle
  \param enable [I]  TRUE: Enable resizer;FALSE:Disable resizer

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszEnable(CSL_RszHandle hndl, Bool32 enable);

/**
  \brief Enable/Disable One shot mode of resizer

  \param hndl   [I]  Handle
  \param enable [I]  TRUE: Enable one shot mode ;FALSE:Disable one shot mode

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszOneShotEnable(CSL_RszHandle hndl, Bool32 enable);

/**
  \brief Enable/Disable One shot mode of resizer for resizer Channel A or B

  \param hndl   [I]  Handle
  \param rszMod [I]  Resizer Channel A or B
  \param enable [I]  TRUE: Enable resizer output ;FALSE:Disable resizer output

  \return Module Handle on success, else NULL
*/
  CSL_Status CSL_rszOutEnable(CSL_RszHandle hndl, Uint8 rszMod, Bool32 enable);

/**
  \brief Enable/Disable One shot mode of resizer Channel A or B

  \param hndl   [I ]  Handle
  \param rszMod [I ]  Resizer Channel A or B
  \param enable [I ]  TRUE: Enable one shot mode ;FALSE:Disable one shot mode

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszOutOneShotEnable(CSL_RszHandle hndl, Uint8 rszMod, Bool32 enable);

/**
  \brief Set output address for resizer Channel A or B

  \param hndl           [I ]  Handle
  \param rszMod         [I ]  Resizer Channel A or B
  \param addr           [I ]  Output Address
  \param yuv420CoutAddr [I ] YUV420 mode C out address

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszSetOutAddr(CSL_RszHandle hndl, Uint8 rszMod, Uint8 * addr, Uint8 *yuv420CoutAddr);

/**
  \brief Get Resizer Status

  \param hndl   [I ]  Handle
  \param rszMod [I ]  Resizer Channel A or B
  \param status [ O]  Status of resizer

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszGetStatus(CSL_RszHandle hndl, Uint8 rszMod, CSL_RszStatus * status);

/**
  \brief Set Output frame config for resizer Channel A or B

  Only resize output related registers are changed, input related register are not changed
  if inFrameConfig == NULL, get in frame config by reading RSZ registers

  \param hndl          [I ] Handle
  \param rszMod        [I ] Resizer Channel A or B
  \param inFrameConfig [I ] Resizer Input frame config
  \param outConfig     [I ] Resizer Output frame config

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszSetOutFrameConfig(CSL_RszHandle hndl, Uint8 rszMod, CSL_RszInFrameConfig * inFrameConfig, CSL_RszOutFrameConfig * outConfig);

/**
  \brief Get Output frame config for resizer Channel A or B

  \param hndl       [I ]  Handle
  \param rszMod     [I ]  Resizer Channel A or B
  \param outConfig  [ O] Resizer Output frame config

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszGetOutFrameConfig(CSL_RszHandle hndl, Uint8 rszMod, CSL_RszOutFrameConfig * outConfig);

/**
  \brief Set Dzoom

  Adjust resize output related registers to get required Dzoom
  Make sure output/input and other resize parameters are set before calling this API

  \param hndl         [I ] Handle
  \param dzoomConfig  [I ] Dzoom config

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszSetDzoomConfig(CSL_RszHandle hndl, CSL_RszDzoomConfig *dzoomConfig);


/**
  \brief Set flip
  
  Flip will take effect on next frame.

  \param hndl    [I ] Handle
  \param rszMod  [I ] Resizer Channel A or B
  \param flipH   [I ] Flip in H direction
  \param flipV   [I ] Flip in V direction
  
  \return CSL_SOK on success, else CSL_Exxxx      
*/
  CSL_Status CSL_rszSetFlip(CSL_RszHandle hndl, Uint8 rszMod, Bool32 flipH, Bool32 flipV);

/**
  \brief Buffer init

  \param hndl             [I ] hndl
  \param rszMod           [I ] resize channel
  \param bufInit          [I ] Buffer init parameters
  \param yuv420BufCoffset [I ] Offset from start address of Y data where C data is located

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszBufInit(CSL_RszHandle hndl, Uint8 rszMod, CSL_BufInit * bufInit, CSL_RszBufConfig *bufConfig);

/**
  \brief Buffer switch in ISR enable/disable

  \param hndl     [I ] hndl
  \param rszMod   [I ] resize channel
  \param enable   [I ] TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszBufSwitchEnable(CSL_RszHandle hndl, Uint8 rszMod, Bool32 enable);

/**
  \brief Get Full Buffer

  \param hndl     [I ] hndl
  \param rszMod   [I ] resize channel
  \param buf      [ O] Full buffer info
  \param minBuf   [I ] minimum number of buffers that should be full
  \param timeout  [I ] Timeout

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszBufGetFull(CSL_RszHandle hndl, Uint8 rszMod, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout);

/**
  \brief Put Empty Buffer

  \param hndl     [I ] hndl
  \param rszMod   [I ] resize channel
  \param buf      [I ] Empty buffer info

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszBufPutEmpty(CSL_RszHandle hndl, Uint8 rszMod, CSL_BufInfo * buf);

/**
  \brief Enable/Disable module interrupt

  \param hndl     [I ] hndl
  \param enable   [I ] TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszIntEnable(CSL_RszHandle hndl, Bool32 enable);

/**
  \brief Clear module interrupt

  \param hndl     [I ] hndl

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszIntClear(CSL_RszHandle hndl);

/**
  \brief Wait for module interrupt

  \param hndl         [I ] hndl

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszIntWait(CSL_RszHandle hndl);

/**
  \brief Module lock : Mutex Lock

  \param hndl     [I ] hndl
  \param timeout  [I ] CSL_SYS_TIMEOUT_NONE or CSL_SYS_TIMEOUT_FOREVER

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszLock(CSL_RszHandle hndl, Uint32 timeout);

/**
  \brief Module unlock : Mutex Unlock

  \param hndl     [I ] hndl

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_rszUnlock(CSL_RszHandle hndl);


// frame division mode APIs
  CSL_Status CSL_rszCalcFmDivPrm(CSL_RszHandle hndl, CSL_RszFmDivPrm * data);
  CSL_Status CSL_rszCalcSlicePrm(CSL_RszHandle hndl, CSL_RszFmDivPrm * data);

  CSL_Status CSL_rszSetFmDivPrm(CSL_RszHandle hndl, CSL_RszFmDivPrm * data);
  CSL_Status CSL_rszSetSlicePrm(CSL_RszHandle hndl, CSL_RszFmDivPrm * data);

#ifdef __cplusplus
}
#endif
/*@}*/
#endif
