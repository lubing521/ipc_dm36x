/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#ifndef _CSL_IPIPE_H_
#define _CSL_IPIPE_H_

#ifdef __cplusplus
extern  "C" {
#endif

#include <csl_soc.h>
#include <csl_buf.h>

/**
  \file csl_ipipe.h
  \brief IPIPE APIs
*/

/**
 \ingroup  CSL_VPFE
 \defgroup CSL_IPIPE IPIPE
 \brief Image Pipe (IPIPE)

 @{
*/

// define's

#define CSL_IPIPE_IPIPE_MODE_CONTINUOUS 0 ///< IPIPE MODE : Continuous mode
#define CSL_IPIPE_IPIPE_MODE_ONE_SHOT 1 ///< IPIPE MODE : One shot mode

#define CSL_IPIPE_IPIPE_DATA_PATH_BAYER_IN_YCBCR_RGB_OUT 0  ///<Bayer input, YCbCr (or RGB) output
#define CSL_IPIPE_IPIPE_DATA_PATH_BAYER_IN_BAYER_OUT     1  ///<Bayer input, Bayer output (Bayer output is from white balance output)
#define CSL_IPIPE_IPIPE_DATA_PATH_BAYER_IN_DISABLE_OUT   2  ///<Bayer input, Output disable (For histogram or Boxcar only mode)
#define CSL_IPIPE_IPIPE_DATA_PATH_YCBCR_IN_YCBCR_RGB_OUT 3  ///<YCbCr (16bit) input, YCbCr (or RGB) output

#define CSL_IPIPE_DPC_LUT_TBL_ENTRIES_1024      0 ///< Defect Pixel Correction Look-up table  entries : Maximum of 1024
#define CSL_IPIPE_DPC_LUT_TBL_ENTRIES_INFINITE  1 ///< Defect Pixel Correction Look-up table entries : Infinite

#define CSL_IPIPE_DPC_REPLACE_BLACK 0 ///< Defect Pixel Correction Method : Replace with black dot
#define CSL_IPIPE_DPC_REPLACE_WHITE 1 ///< Defect Pixel Correction Method : Replace with white dot

#define CSL_IPIPE_DPC_OTF_DEFECT_DETECTION_METHOD_MAX1_MIN1 0 ///< DPC on the fly defect detection method : MAX1MIN1
#define CSL_IPIPE_DPC_OTF_DEFECT_DETECTION_METHOD_MAX2_MIN1 1 ///< DPC on the fly defect detection method : MAX2MIN2

#define CSL_IPIPE_DPC_OTF_ALG_DPC2 0  ///< DPC on the fly defect detection algorithm: DPC2.0
#define CSL_IPIPE_DPC_OTF_ALG_DPC3 1  ///< DPC on the fly defect detection algorithm: DPC3.0

#define CSL_IPIPE_NF_SPR_SRC_SPR_REG 0  ///< Noise Filter Spread Value Source : Use value in SPR register
#define CSL_IPIPE_NF_SPR_SRC_LUT_SPR 1  ///< Noise Filter Spread Value Source : Use value in SPR lookup table

#define CSL_IPIPE_NF_GREEN_SAMPLING_METHOD_BOX 0  ///< Noise filter sampling method for green pixels : Box
#define CSL_IPIPE_NF_GREEN_SAMPLING_METHOD_DIAMOND 1  ///< Noise filter sampling method for green pixels : Diamond

#define CSL_IPIPE_GIC_ALGORITHM_SELECT_DIFF_INDEX 0 ///< Green Imbalance Correction Algorithm : Select difference as index
#define CSL_IPIPE_GIC_ALGORITHM_SELECT_1D_HP_INDEX 1  ///< Green Imbalance Correction Algorithm : Select 1D High Pass value as index

#define CSL_IPIPE_GIC_THRESHOLD_SELECTION_GIC 0 ///< Green Imbalance Correction Threshold :Select GIC register value
#define CSL_IPIPE_GIC_THRESHOLD_SELECTION_NF2 1 ///< Green Imbalance Correction Threshold :Select NF2 value

#define CSL_IPIPE_CFA_MODE_2DAC      0  ///< Colour filter array interpolation mode : 2DirAC
#define CSL_IPIPE_CFA_MODE_2DAC_DAA  1  ///< Colour filter array interpolation mode : 2DirAC and DAA (Digital Anti Aliasing)
#define CSL_IPIPE_CFA_MODE_DAA       2  ///< Colour filter array interpolation mode : DAA (Digital Anti Aliasing)

#define CSL_IPIPE_GAMMA_CORRECTION_TABLE_SIZE_64   0  ///< Gamma correction table size :  64 Words
#define CSL_IPIPE_GAMMA_CORRECTION_TABLE_SIZE_128  1  ///< Gamma correction table size : 128 Words
#define CSL_IPIPE_GAMMA_CORRECTION_TABLE_SIZE_256  2  ///< Gamma correction table size : 256 Words
#define CSL_IPIPE_GAMMA_CORRECTION_TABLE_SIZE_512  3  ///< Gamma correction table size : 512 Words

#define CSL_IPIPE_GAMMA_CORRECTION_TABLE_SELECT_RAM 0 ///< Gamma correction table select : RAM
#define CSL_IPIPE_GAMMA_CORRECTION_TABLE_SELECT_ROM 1 ///< Gamma correction table select : ROM

#define CSL_IPIPE_YUV_CHROMA_POS_LUM 0  ///< Chrominance poisition : Luminance Position
#define CSL_IPIPE_YUV_CHROMA_POS_MID_LUM 1  ///< Chrominance poisition :Mid of Luminance Position

#define CSL_IPIPE_GBCE_METHOD_Y_VALUE 0 ///< Global Brightness and Contrast Enable Selection Method : Y is read from LUT. Cb and Cr are unchanged
#define CSL_IPIPE_GBCE_METHOD_GAIN_VALUE 1  ///< Global Brightness and Contrast Enable Selection Method: Gain Y, Cb, and Cr are multiplied by the value in LUT

#define CSL_IPIPE_YEE_EE_ES_MERGING_METHOD_ABSOLUTE_MAX 0 ///< Edge Enhancement(EE) and Edge Sharpener(ES) Enhancement method : Absolute maximum of ES and EE
#define CSL_IPIPE_YEE_EE_ES_MERGING_METHOD_SUMMATION    1 ///< Edge Enhancement(EE) and Edge Sharpener(ES) Enhancement method : Summation of ES and EE

#define CSL_IPIPE_CAR_METHOD_GAIN_CONTROL 0 ///< Chroma Artifact Reduction Method Selection : Gain Control
#define CSL_IPIPE_CAR_METHOD_DYNAMIC CONTROL 1  ///< Chroma Artifact Reduction Method Selection : Dynamic Control

#define CSL_IPIPE_CAR_HPF_TYPE_Y          0 ///< Chroma Artifact Reduction HPF Type : Y
#define CSL_IPIPE_CAR_HPF_TYPE_HORIZONTAL 1 ///< Chroma Artifact Reduction HPF Type : Horizontal HPF
#define CSL_IPIPE_CAR_HPF_TYPE_VERTICAL   2 ///< Chroma Artifact Reduction HPF Type :vertical HPF
#define CSL_IPIPE_CAR_HPF_TYPE_2D         3 ///< Chroma Artifact Reduction HPF Type :2D HPF
#define CSL_IPIPE_CAR_HPF_TYPE_2D_EE      4 ///< Chroma Artifact Reduction HPF Type :2D HPF from edge enhancement

#define CSL_IPIPE_BOXCAR_BLOCK_SIZE_8_8  0  ///< Block Size in box car sampling : 8 * 8
#define CSL_IPIPE_BOXCAR_BLOCK_SIZE_16_16 1 ///< Block Size in box car sampling : 16 * 16

#define CSL_IPIPE_BSC_COLOUR_SELECT_Y 0 ///< Boundary Signal Calculator Select cour to be summed : Y
#define CSL_IPIPE_BSC_COLOUR_SELECT_CB 1  ///< Boundary Signal Calculator Select cour to be summed : CB
#define CSL_IPIPE_BSC_COLOUR_SELECT_CR 2  ///< Boundary Signal Calculator Select cour to be summed : CR

#define CSL_IPIPE_SOURCE_COLOUR_R    0  ///< Colour : Red
#define CSL_IPIPE_SOURCE_COLOUR_GR   1  ///< Colour : Green (Red line)
#define CSL_IPIPE_SOURCE_COLOUR_GB   2  ///< Colour : Green (Blue line)
#define CSL_IPIPE_SOURCE_COLOUR_B    3  ///< Colour : Red

#define CSL_IPIPE_HISTOGRAM_SOURCE_NF 0 ///< Histogram Input Selection :From 1st Noise Filter (Bayer format)
#define CSL_IPIPE_HISTOGRAM_SOURCE_RGB2YUV 1  ///< Histogram Input Selection :From RGB2YUV (YCbCr-444 format)

#define CSL_IPIPE_HISTOGRAM_GREEN_SAMPLING_GR 0 ///< Green Sampling Method : Gr is selected
#define CSL_IPIPE_HISTOGRAM_GREEN_SAMPLING_GB 1 ///< Green Sampling Method : Gb is selected
#define CSL_IPIPE_HISTOGRAM_GREEN_SAMPLING_GAVG 2 ///< Green Sampling Method : Gavg is selected

#define CSL_IPIPE_HISTOGRAM_BIN_NUM_32      0 ///< Histogram number of bins: 32
#define CSL_IPIPE_HISTOGRAM_BIN_NUM_64      1 ///< Histogram number of bins: 64
#define CSL_IPIPE_HISTOGRAM_BIN_NUM_128     2 ///< Histogram number of bins: 128
#define CSL_IPIPE_HISTOGRAM_BIN_NUM_256     3 ///< Histogram number of bins: 256

#define CSL_IPIPE_HISTOGRAM_OUT_TABLE_0_1   0 ///< Histogram output table select : Tables 0 , 1
#define CSL_IPIPE_HISTOGRAM_OUT_TABLE_2_3   1 ///< Histogram output table select : Tables 2 , 3

#define CSL_IPIPE_INT_TYPE_BOXCAR           0 ///< IPIPE Interrupt Type : Boxcar Done
#define CSL_IPIPE_INT_TYPE_BSC              1 ///< IPIPE Interrupt Type : BSC Done
#define CSL_IPIPE_INT_TYPE_MAX              2 ///< IPIPE Interrupt Type : Max

#define CSL_IPIPE_DPC_LUT_MAX_ENTRIES            (256)    ///< DPC Look-up table max entries
#define CSL_IPIPE_GAMMA_TABLE_MAX_ENTRIES        (512)    ///< Gamma table max entries
#define CSL_IPIPE_3D_LUT_MAX_ENTRIES             (732)    ///< 3D LUT table max entries
#define CSL_IPIPE_GBCE_GAIN_TABLE_MAX_ENTRIES    (1024)   ///< GBCE gain table max entries
#define CSL_IPIPE_EDGE_ENHANCE_TABLE_MAX_ENTRIES (1024)   ///< Edge enhance table max entries
#define CSL_IPIPE_HISTOGRAM_RESULTS_MAX_ENTRIES  (1024)   ///< Histogram results, Max entries
#define CSL_IPIPE_BSC_RESULTS_MAX_ENTRIES        (1024)   ///< BSC results, Max entries

// data structure's

/**
  \brief Colour Pattern configuration
*/
  typedef struct {

    Uint32  colPat[2][2];       ///< 2x2 Block Color pattern \n CSL_IPIPE_SOURCE_COLOUR_R....CSL_IPIPE_SOURCE_COLOUR_B

  } CSL_IpipeColPatConfig;


/**
  \brief Input frame configuration
*/
  typedef struct {

    Uint16  inStartX;           ///< Horizontal start position ,  in pixels
    Uint16  inStartY;           ///< Vertical start position   ,  in lines
    Uint16  inWidth;            ///< Horizontal size of processing area , in pixels
    Uint16  inHeight;           ///< Vertical size of processing area   , in lines

  } CSL_IpipeInFrameConfig;

/**
  \brief IPIPE clock configuration
*/
  typedef struct {

    Bool32    mmrClkEnable;       ///< TRUE:Enable MMR interface clock \n FALSE : Disable MMR interface clock , used for MMR registers access.
    Bool32    yeeCarClkEnable;    ///< Enable/Disbale clock corresponding to Edge enhancement and Chroma artifact reduction
    Bool32    cfa2yuvHistoBscClkEnable; ///< Enable/Disbale clock corresponding to CFA to YUV(422), Histogram and BSC
    Bool32    dpc2WbClkEnable;    ///< Enable/Disbale clock corresponding to Defect Pixel correction to WhiteBalance and Histogram(RAW input)
    Bool32    boxcarClkEnable;    ///< Enable/Disbale clock corresponding to Boxcar

  } CSL_IpipeClkConfig;

/**
  \brief Defect Pixel Correction configuration
*/
  typedef struct {

    Bool32    lutEnable;            ///< TRUE : Enable LUT Defect Pixel Correction  \n FALSE : Disable LUT Defect Pixel Correction
    Uint32  lutType;              ///< CSL_IPIPE_DPC_LUT_TBL_ENTRIES_1024 \n CSL_IPIPE_DPC_LUT_TBL_ENTRIES_INFINITE
    Uint32  lutOption0CorMethod;  ///< CSL_IPIPE_DPC_REPLACE_BLACK \n CSL_IPIPE_DPC_REPLACE_WHITE
    Uint32  lutStartAddr;         ///< Start address in look-up table
    Uint32  lutNumEntries;        ///< Number of valid entries in look-up table. Valid only if lutType is CSL_IPIPE_DPC_LUT_TBL_ENTRIES_1024

    Uint32 *lutAddr;            ///< LUT Table Address, CSL_IPIPE_DPC_LUT_MAX_ENTRIES max entries

    Bool32  otfEnable;          ///< TRUE:Enable on-the-fly DPC method \n FALSE :Diable on-the-fly DPC method
    Uint32  otfType;            ///< CSL_IPIPE_DPC_DEFECT_DETECTION_METHOD_MAX1_MIN1 \n CSL_IPIPE_DPC_DEFECT_DETECTION_METHOD_MAX2_MIN1
    Uint32  otfAlg;             ///< CSL_IPIPE_DPC_OTF_ALG_DPC2 \n CSL_IPIPE_DPC_OTF_ALG_DPC3
    Uint32  otf2DetThres[4];    ///< Defect Detection Threshold for R,Gr,Gb,B for DPC2 algorithm
    Uint32  otf2CorThres[4];    ///< Defect Correction Threshold for R,Gr,Gb,B for DPC2 algorithm

    Uint32  otf3ActAdjust;      ///< DPC3.0 Activity Adjustment Activity = (Max2 - Min2) >> (6 - SHF)
    Uint32  otf3DetThres;       ///< DPC3.0 Defect Detection Threshold
    Uint32  otf3DetThresSlope;  ///< DPC3.0 Defect Detection Threshold Slope
    Uint32  otf3DetThresMin;    ///< DPC3.0 Defect Detection Threshold Minimum
    Uint32  otf3DetThresMax;    ///< DPC3.0 Defect Detection Threshold Maximum
    Uint32  otf3CorThres;       ///< DPC3.0 Defect Correction Threshold
    Uint32  otf3CorThresSlope;  ///< DPC3.0 Defect Correction Threshold Slope
    Uint32  otf3CorThresMin;    ///< DPC3.0 Defect Correction Threshold Minimum
    Uint32  otf3CorThresMax;    ///< DPC3.0 Defect Correction Threshold Maximum

  } CSL_IpipeDpcConfig;

/**
  \brief Lens shading correction configuration
*/
  typedef struct {

    Uint32  vertOffset;         ///< LSC vertical offset
    Uint32  vertQuadCoeff;      ///< LSC Vertical Quadratic Coefficient
    Uint32  vertLinCoeff;       ///< LSC Veritcal Linear Coefficient
    Uint32  vertQuadShiftLen;   ///< LSC Vertical Quadratic Shift Length
    Uint32  vertLinShiftLen;    ///< LSC Vertical Linear Shift Length

    Uint32  horzOffset;         ///< LSC Horizontal offset
    Uint32  horzQuadCoeff;      ///< LSC Horizontal Quadratic Coefficient
    Uint32  horzLinCoeff;       ///< LSC HOrizontal Linear Coefficient
    Uint32  horzQuadShiftLen;   ///< LSC Horizontal Quadratic Shift Length
    Uint32  horzLinShiftLen;    ///< LSC Horizontal Linear Shift Length

    Uint32  gain[4];            ///< LSC total gain values for R, GR , GB and B
    Uint32  offset[4];          ///< LSC total offset values for R, GR , GB and B

    Uint32  gainShiftLen;       ///< LSC gain shift length
    Uint32  gainMax;            ///< LSC Maximum value of gain

  } CSL_IpipeLscConfig;

/**
  \brief Noise Filter configuration
*/
  typedef struct {

    Bool32  enable;             ///< TRUE : Enable noise filter \n FALSE :Disable noise filter

    Uint32  spreadVal;          ///< Spread value of noise filter
    Uint32  spreadValSrc;       ///< Spread value source : CSL_IPIPE_NF_SPR_SRC_SPR_REG \n CSL_IPIPE_NF_SPR_SRC_LUT_SPR
    Uint32  lutAddrShift;       ///< Downshift value in LUT reference address
    Uint32  greenSampleMethod;  ///< Green pixels sampling method \n CSL_IPIPE_NF_GREEN_SAMPLING_METHOD_BOX \n  CSL_IPIPE_NF_GREEN_SAMPLING_METHOD_DIAMOND
    Bool32  lscGainEnable;      ///< TRUE : Applies LSC gain to threshold value \n FALSE : LSC gain not applied

    Uint32  edgeDetectThresMin; ///< Edge Detection Minimum Threshold for NF
    Uint32  edgeDetectThresMax; ///< Edge Detection Maximum Threshold for NF

    Uint32  lutThresTable[8];      ///< LUT Values (threshold) Threshold values in noise filter algorithm
    Uint32  lutIntensityTable[8];  ///< LUT Values (intensity) Intensity values in noise filter algorithm
    Uint32  lutSpreadTable[8];     ///< LUT Values (spread) Spread values in noise filter  algorithm . Valid only if CLK0 of VPSS registers is 1

  } CSL_IpipeNfConfig;

/**
  \brief Green Imbalance Correction configuration
*/
  typedef struct {

    Bool32    enable;             ///< TRUE:Enable Green Imbalance Correction \n FALSE:Disable green imbalance correction
    Uint32  type;               ///< Algorithm selection \n CSL_IPIPE_GIC_ALGORITHM_SELECT_DIFF_INDEX \n CSL_IPIPE_GIC_ALGORITHM_SELECT_1D_HP_INDEX
    Uint32  thresSel;           ///< Threshold Selection \n CSL_IPIPE_GIC_THRESHOLD_SELECTION_GIC \n CSL_IPIPE_GIC_THRESHOLD_SELECTION_NF2
    Bool32    lscGainEnable;      ///< TRUE : LSC gain applied to threshold value \n FALSE : LSC gain not applied

    Uint32  gain;               ///< GAIN or weight value for mixing of GICed data and the original data
    Uint32  nf2ThresGainScale;  ///< NF2 threshold gain value
    Uint32  thresVal;           ///< First threshold value.
    Uint32  thresSlope;         ///< GIC Theshold slope

  } CSL_IpipeGicConfig;

/**
  \brief White Balance configuration
*/
  typedef struct {

    Uint32  offset[4];          ///< White balance offset values for R, Gr , Gb , B
    Uint32  gain[4];            ///< White balance gain values for R, Gr , Gb , B

  } CSL_IpipeWbConfig;

/**
  \brief CFA (Colour Filter Array) configuration
*/
  typedef struct {

    Uint32  mode;               ///< CSL_IPIPE_CFA_MODE_2DAC \n CSL_IPIPE_CFA_MODE_2DAC_DAA \n CSL_IPIPE_CFA_MODE_DAA

    Uint32  twoDirHpValLowThres;  ///< 2DirCFA HP Value Low Threshold
    Uint32  twoDirHpValSlope;   ///< 2DirCFA HP Value Slope
    Uint32  twoDirHpMixThres;   ///< 2DirCFA HP Mix Threshold
    Uint32  twoDirHpMixSlope;   ///< 2DirCFA HP Mix Slope
    Uint32  twoDirDirThres;     ///< 2DirCFA Direction Threshold
    Uint32  twoDirDirSlope;     ///< 2DirCFA Direction Slope
    Uint32  twoDirNonDirWeight; ///< 2DirCFA NonDirectional Weight

    Uint32  daaHueFrac;         ///< Hue Fraction
    Uint32  daaEdgeThres;       ///< Edge Threshold
    Uint32  daaThresMin;        ///< Threshold Minimum
    Uint32  daaThresSlope;      ///< Threshold Slope
    Uint32  daaSlopeMin;        ///< Slope Minimum
    Uint32  daaSlopeSlope;      ///< Slope Slope
    Uint32  daaLpWeight;        ///< LP Weight

  } CSL_IpipeCfaConfig;

/**
  \brief RGB2RGB configuration
*/
  typedef struct {

    Uint32  matrix[3][3];       ///< Matix co-efficient values for RGB to RGB blending , Range(-8 to +7.996)
    Uint32  offset[3];          ///< Offset values for RGB to RGB blending

  } CSL_IpipeRgb2RgbConfig;

/**
  \brief Gamma configuration
*/
  typedef struct {

    Uint32  tableSize;          ///< CSL_IPIPE_GAMMA_CORRECTION_TABLE_SIZE_64 ....\n CSL_IPIPE_GAMMA_CORRECTION_TABLE_SIZE_512
    Uint32  tableSrc;           ///< CSL_IPIPE_GAMMA_CORRECTION_TABLE_SELECT_RAM \n CSL_IPIPE_GAMMA_CORRECTION_TABLE_SELECT_ROM
    Bool32    bypassR;            ///< TRUE: bypass, FALSE: no bypass
    Bool32    bypassG;            ///< TRUE: bypass, FALSE: no bypass
    Bool32    bypassB;            ///< TRUE: bypass, FALSE: no bypass

    Uint32 *tableR;             ///< Gamma table R, CSL_IPIPE_GAMMA_TABLE_MAX_ENTRIES entries
    Uint32 *tableG;             ///< Gamma table G, CSL_IPIPE_GAMMA_TABLE_MAX_ENTRIES entries
    Uint32 *tableB;             ///< Gamma table B, CSL_IPIPE_GAMMA_TABLE_MAX_ENTRIES entries

  } CSL_IpipeGammaConfig;

/**
  \brief 3D Lookup table configuration
*/
  typedef struct {

    Bool32    enable;             ///< TRUE : Enable 3D LUT \n FALSE: Disable 3D LUT

    Uint32 *table;              ///< 3D LUT CSL_IPIPE_3D_LUT_MAX_ENTRIES entries

  } CSL_Ipipe3dLutConfig;

/**
  \brief RGB to YUV conversion configuration
*/
  typedef struct {

    Uint32  matrix[3][3];       ///< Gain values for RGB to YUV conversion Range (-8 to +7.996)
    Uint32  offset[3];          ///< Offset values for Y , CB , CR

    Bool32    cLpfEnable;         ///< LPF Enable for Chrominance
    Uint32  cPos;               ///< Chrominance phase position YUV_CHROMA_POS_LUM \n YUV_CHROMA_POS_MID_LUM

  } CSL_IpipeRgb2YuvConfig;

/**
  \brief Contrast Brightness configuration
*/
  typedef struct {

    Uint32  brightness;         ///< Brightness Offset value for brightness control
    Uint32  contrast;           ///< Contrast Multiplier coefficient for contrast control

  } CSL_IpipeCntBrtConfig;

/**
  \brief Global brightness contrast enhancement (GBCE) configuration
*/
  typedef struct {

    Bool32  enable;             ///< TRUE: Enable \n FALSE: Disable
    Uint32  type;               ///< CSL_IPIPE_GBCE_METHOD_Y_VALUE \n CSL_IPIPE_GBCE_METHOD_GAIN_VALUE

    Uint32 *gainTable;          ///< GBCE Gain Table, CSL_IPIPE_GBCE_GAIN_TABLE_MAX_ENTRIES entries

  } CSL_IpipeGbceConfig;

/**
  \brief Edge Enhancement configuration
*/
  typedef struct {

    Uint32  enable;             ///< TRUE : Enable Edge Enhancement \n  FALSE: Disable edge enhancement
    Uint32  haloReduceEnable;   ///< TRUE : Enable Halo Reduction \n FALSE: Disable Halo Reduction
    Uint32  mergeMethod;        ///< CSL_IPIPE_YEE_EE_ES_MERGING_METHOD_ABSOLUTE_MAX \n CSL_IPIPE_YEE_EE_ES_MERGING_METHOD_SUMMATION

    Uint32  hpfShift;           ///< Down shift length of high pass filter
    Uint32  hpfCoeff[3][3];     ///< HPF coefficient values for Edge Enhancement

    Uint32  lowerThres;         ///< Lower Threshold before referring to LUT

    Uint32  edgeSharpGain;      ///< Edge Sharpener Gain
    Uint32  edgeSharpHpValLowThres; ///< Edge Sharpener Lower Threshold
    Uint32  edgeSharpHpValUpLimit;  ///< Edge Sharpener HP Value Upper Limit
    Uint32  edgeSharpGainGradient;  ///< Edge Sharpener Gain on Gradient
    Uint32  edgeSharpOffsetGradient;  ///< Edge Sharpener Offset on Gradient

    Uint32 *table;              ///< Edge Enhance table, CSL_IPIPE_EDGE_ENHANCE_TABLE_MAX_ENTRIES entries

  } CSL_IpipeEdgeEnhanceConfig;

/**
  \brief Chroma Artifact Reduction configuration
*/
  typedef struct {

    Bool32  enable;             ///< TRUE : Enable Chroma artifact reduction \n FALSE : Disable Chroma artifact reduction
    Uint32  type;               ///< CSL_IPIPE_CAR_METHOD_GAIN_CONTROL \n CSL_IPIPE_CAR_METHOD_DYNAMIC_CONTROL

    Uint32  sw0MedFiltThres;    ///< Gain Control Threshold
    Uint32  sw1GainCtrlThres;   ///< Median Filter Threshold

    Uint32  hpfType;            ///< CSL_IPIPE_CAR_HPF_TYPE_Y 0 \n CSL_IPIPE_CAR_HPF_TYPE_HORIZONTAL
    Uint32  hpfShift;           ///< Down shift value for HPF
    Uint32  hpfValThres;        ///< HPF Threshold Value

    Uint32  gain1Intensity;     ///< Intensity value of Gain1
    Uint32  gain1Shift;         ///< Shift value of Gain1
    Uint32  gain1Min;           ///< Minimum value of Gain1

    Uint32  gain2Intensity;     ///< Intensity value of Gain2
    Uint32  gain2Shift;         ///< Shift value of Gain2
    Uint32  gain2Min;           ///< Minimum value of Gain2

  } CSL_IpipeChromaArtifactReduceConfig;

/**
  \brief Chroma Gain Supression configuration
*/
  typedef struct {

    Bool32    enable;             ///< TRUE:Enable Chroma Gain Suppression \n FALSE : Disable Chroma Gain Suppression

    Uint32  gain1LThres;        ///< Y dark side threshold
    Uint32  gain1LSlope;        ///< Y dark side slope. Write access to this bit is allowed if CKE0=1 in VPSS registers.
    Uint32  gain1LShift;        ///< Y dark side down-shift value . Write access to this bit is allowed if CKE0=1 in VPSS registers.
    Uint32  gain1LMin;          ///< Y dark side minimum .Write access to this bit is allowed if CKE0=1 in VPSS registers.

    Uint32  gain1HThres;        ///< Y bright side threshold
    Uint32  gain1HSlope;        ///< Y bright side slope
    Uint32  gain1HShift;        ///< Y bright side down-shift value
    Uint32  gain1HMin;          ///< Y bright side minimum

    Uint32  gain2LThres;        ///< Chroma unsaturated side threshold
    Uint32  gain2LSlope;        ///< Chroma unsaturated side slope. Write access to this bit is allowed if CKE0=1 in VPSS registers.
    Uint32  gain2LShift;        ///< Chroma unsaturated side down-shift value. Write access to this bit is allowed if CKE0=1 in VPSS registers.
    Uint32  gain2LMin;          ///< Chroma unsaturated side minimum. Write access to this bit is allowed if CKE0=1 in VPSS registers.

  } CSL_IpipeChromaGainSupressConfig;

/**
  \brief Boxcar configuration
*/
  typedef struct {

    Bool32    enable;             ///< TRUE:Enable boxcar \n FALSE: Disable boxcar
    Bool32    oneShotEnable;      ///< CSL_IPIPE_IPIPE_MODE_ONE_SHOT \n CSL_IPIPE_IPIPE_MODE_CONTINUOUS
    Uint8   blockSize;          ///< BOXCAR_BLOCK_SIZE_8_8  \n BOXCAR_BLOCK_SIZE_16_16
    Uint8   inShift;            ///< Down shift value of input data of Boxcar
    Uint8  *outAddr;            ///< Output Address

  } CSL_IpipeBoxcarConfig;

/**
  \brief Histogram Region configuration
*/
  typedef struct {

    Uint16  startX;             ///< Horizontal Start position , in pixels
    Uint16  startY;             ///< Vertical Start position , in lines
    Uint16  width;              ///< Width of region
    Uint16  height;             ///< Height of region

  } CSL_IpipeHistogramRegionConfig;

/**
  \brief Histogram configuration
*/
  typedef struct {

    Bool32    enable;             ///< TRUE : Enable Histogram \n FALSE : Disable Histogram
    Bool32    oneShotEnable;      ///< CSL_IPIPE_IPIPE_MODE_CONTINUOUS \n CSL_IPIPE_IPIPE_MODE_ONE_SHOT

    Uint8   source;             ///< CSL_IPIPE_HISTOGRAM_SOURCE_NF \n CSL_IPIPE_HISTOGRAM_SOURCE_RGB2YUV
    Uint8   greenType;          ///< CSL_IPIPE_HISTOGRAM_GREEN_SAMPLING_GR ...CSL_IPIPE_HISTOGRAM_GREEN_SAMPLING_GAVG .Valid only when NF is source

    Uint8   inShift;            ///< Shift Length of input data
    Uint8   numBins;            ///< CSL_IPIPE_HISTOGRAM_BIN_NUM_32 ...CSL_IPIPE_HISTOGRAM_BIN_NUM_256

    Bool32    regionEnable[4];    ///< Enable/Disable respective region
    Bool32    colorEnable[4];     ///< Enable/Disable respective colour

    Uint8   outTableSel;        ///< CSL_IPIPE_HISTOGRAM_OUT_TABLE_0_1 \n CSL_IPIPE_HISTOGRAM_OUT_TABLE_2_3
    Bool32    clearOnVd;          ///< TRUE: Clear results on VD , FALSE: Do not clear results on VD

    Uint8   matrixCoeff[4];     ///< Matrix co efficient values for R , GR , GB , B values

    CSL_IpipeHistogramRegionConfig regionConfig[4]; ///< Start positions and dimensions for Histogram regions

  } CSL_IpipeHistogramConfig;


/**
  \brief Histogram results
*/
  typedef struct {   //ANR - Hist

    Uint32 *table;     ///< [ O] Hist results,
    Uint32  offset;    ///< [i ] offset
    Uint32  size;       ///< [ i] length,
  } CSL_IpipeHistResults;


/**
  \brief Boundary Signal Calculator configuration
*/
  typedef struct {

    Uint32  enable;             ///< TRUE:Enable BSC \n FALSE:Disable BSC
    Uint32  oneShotEnable;      ///< TRUE:One shot mode enable, FALSE: One shot mode disable

    Uint8   colorSelect;        ///< CSL_IPIPE_BSC_COLOUR_SELECT_Y \n CSL_IPIPE_BSC_COLOUR_SELECT_CB \n CSL_IPIPE_BSC_COLOUR_SELECT_CR

    Uint32  rowEnable;          ///< TRUE:Enable BSC column sampling \n FALSE: Disable column sampling
    Uint8   rowNumVectors;      ///< Number of row sum vectors
    Uint8   rowInShift;         ///< Down shift value for row sum vectors
    Uint16  rowStartV;          ///< Vertical position of the first sampling pixel
    Uint16  rowNumV;            ///< Vertical number of sampled lines for row sum
    Uint8   rowSkipV;           ///< Vertical spacing between adjacent sampled lines for row sum
    Uint16  rowStartH;          ///< Horizontal starting position of row sum
    Uint16  rowNumH;            ///< Horizontal number of sampled lines for row sum
    Uint8   rowSkipH;           ///< Horizontal spacing between adjacent sampled lines for row sum

    Uint32  colEnable;          ///< TRUE: Enable BSC column sampling \n FALSE: Disable column sampling
    Uint8   colNumVectors;      ///< Number of Column Vectors
    Uint8   colInShift;         ///< Down shift value of input for column sum
    Uint16  colStartV;          ///< Vertical starting position of column sum
    Uint16  colNumV;            ///< Vertical number of sampled lines for column sum
    Uint8   colSkipV;           ///< Vertical spacing between adjacent sampled lines for column sum
    Uint16  colStartH;          ///< Horizontal position of the first sampling pixel
    Uint16  colNumH;            ///< Horizontal number of sampled lines for column sum
    Uint8   colSkipH;           ///< Interval of the columns

  } CSL_IpipeBscConfig;

/**
  \brief Boundary Signal Calculator results
*/
  typedef struct {

    Uint32 *rowSumTable;     ///< [ O] Row sum results, total entries is rowNumVectors*rowNumV
    Uint32  rowNumV;         ///< [ O] Number of rows in V direction
    Uint32  rowNumVectors;   ///< [ O] Number of vectors,

    Uint32 *colSumTable;     ///< [ O] Column sum results
    Uint32  colNumH;         ///< [ O] Number of cols in H direction
    Uint32  colNumVectors;   ///< [ O] Number of col vectors

  } CSL_IpipeBscResults;


/**
  \brief Hardware Setup configuration
*/
  typedef struct {

    Bool32    bypassModuleIfNullConfig; ///< if any sub-module related parameters structure's are NULL, then if \n TRUE : Set the registers such that the module is either disabled or bypassed or set to default values \n FALSE: Do not set any sub-module related registers, i.e old values are maintained

    Bool32    enable;             ///< TRUE: Enable IPIPE \n FALSE:Disable IPIPE
    Bool32    oneShotEnable;      ///< TRUE : One shot mode enabled \n FALSE : Continuous mode enabled
    Bool32    wenUseEnable;       ///< CAM_WEN Mode Selection TRUE:use the ipipeif_wrt \n FALSE : does not use ipipeif_wrt
    Uint8   dataPath;           ///< CSL_IPIPE_IPIPE_DATA_PATH_BAYER_IN_YCBCR_RGB_OUT ..... CSL_IPIPE_IPIPE_DATA_PATH_YCBCR_IN_YCBCR_RGB_OUT

    CSL_IpipeColPatConfig *colpatConfig;  ///< Colour pattern configuration
    CSL_IpipeClkConfig *clkConfig;  ///< Clock configuration
    CSL_IpipeInFrameConfig *inFrameConfig;  ///< Input frame configuration

    CSL_IpipeLscConfig *lscConfig;  ///< Lens Shading Correction configuration
    CSL_IpipeDpcConfig *dpcConfig;  ///< Defect Pixel Correction configuration
    CSL_IpipeNfConfig *nf1Config; ///< Noise filter 1 configuration
    CSL_IpipeNfConfig *nf2Config; ///< Noise filter 2 configuration
    CSL_IpipeGicConfig *gicConfig;  ///< Green Imbalance Correction Configuration
    CSL_IpipeWbConfig *wbConfig;  ///<  White Balance Configuraion
    CSL_IpipeCfaConfig *cfaConfig;  ///< Colour Filter Array Configuration
    CSL_IpipeRgb2RgbConfig *rgb2Rgb1Config; ///< RGB to RGB Conversion Configuration
    CSL_IpipeGammaConfig *gammaConfig;  ///<  Gamma configuration
    CSL_IpipeRgb2RgbConfig *rgb2Rgb2Config; ///< RGB to RGB Conversion Configuration
    CSL_Ipipe3dLutConfig *lut3DConfig;  ///< 3D Lookup table configuration
    CSL_IpipeRgb2YuvConfig *rgb2YuvConfig;  ///< RGB to YUV Conversion Configuration
    CSL_IpipeCntBrtConfig *cntBrtConfig;  ///< Contrast and Brightness Configuration
    CSL_IpipeGbceConfig *gbceConfig;  ///< Global Brightness and Contrast Enchancement Configuration
    CSL_IpipeEdgeEnhanceConfig *edgeEnhanceConfig;  ///< Edge enhancement Configuration
    CSL_IpipeChromaArtifactReduceConfig *chromaArtifactReduceConfig;  ///< Chroma Artifact Reduction Configuration
    CSL_IpipeChromaGainSupressConfig *chromaGainSupressConfig;  ///< Chroma Gain Suppression Configuration

    CSL_IpipeBoxcarConfig *boxcarConfig;  ///< Box car related configuration
    CSL_IpipeHistogramConfig *histogramConfig;  ///< Histogram Configuration
    CSL_IpipeBscConfig *bscConfig;  ///< Boundary Signal Calculator Configuration

  } CSL_IpipeHwSetup;

/**
  \brief Module Object Structure
*/
  typedef struct {

#ifdef CSL_KERNEL_API
    CSL_IpipeRegsOvly regs;     ///< Register overlay
    CSL_BufInfo bscCurBufInfo;
    CSL_BufHndl bscOutBuf;
    int     bscCount;
    Bool32    bscOutBufSwitchEnable;

    CSL_BufInfo boxcarCurBufInfo;
    CSL_BufHndl boxcarOutBuf;
    int         boxcarCount;
    Bool32      boxcarOutBufSwitchEnable;

    CSL_SysFlag intFlag[CSL_IPIPE_INT_TYPE_MAX];
#else
    int     fd;
#endif

  } CSL_IpipeObj;


  typedef CSL_IpipeObj *CSL_IpipeHandle;  ///< Module Handle

// function's

/**
  \brief Open Module Handle

  \param hndl   [ O]  Module Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeOpen(CSL_IpipeHandle hndl);

/**
  \brief Close Module Handle

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeClose(CSL_IpipeHandle hndl);

/**
  \brief Set all module registers

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeHwSetup(CSL_IpipeHandle hndl, CSL_IpipeHwSetup * data);

/**
  \brief Set all module registers to power-ON reset value

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeHwReset(CSL_IpipeHandle hndl);

/**
  \brief Set Colour Pattern config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetColPatConfig(CSL_IpipeHandle hndl, CSL_IpipeColPatConfig * data);

/**
  \brief Set IPIPE clock config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetClkConfig(CSL_IpipeHandle hndl, CSL_IpipeClkConfig * data);

/**
  \brief Set input frame config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetInFrameConfig(CSL_IpipeHandle hndl, CSL_IpipeInFrameConfig * data);

/**
  \brief Set Lens Shading Correction config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetLscConfig(CSL_IpipeHandle hndl, CSL_IpipeLscConfig * data);

/**
  \brief Set DPC config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetDpcConfig(CSL_IpipeHandle hndl, CSL_IpipeDpcConfig * data);

/**
  \brief Set Noise Filter 1 config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetNf1Config(CSL_IpipeHandle hndl, CSL_IpipeNfConfig * data);

/**
  \brief Set Noise Filter 2 config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetNf2Config(CSL_IpipeHandle hndl, CSL_IpipeNfConfig * data);

/**
  \brief Set Green Imbalance Correction (GIC) config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetGicConfig(CSL_IpipeHandle hndl, CSL_IpipeGicConfig * data);

/**
  \brief Set White Balance config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetWbConfig(CSL_IpipeHandle hndl, CSL_IpipeWbConfig * data);

/**
  \brief Set Colour Filter Array config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetCfaConfig(CSL_IpipeHandle hndl, CSL_IpipeCfaConfig * data);

/**
  \brief Set RGB2RBG - 1config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetRgb2Rgb1Config(CSL_IpipeHandle hndl, CSL_IpipeRgb2RgbConfig * data);

/**
  \brief Set Gamma config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetGammaConfig(CSL_IpipeHandle hndl, CSL_IpipeGammaConfig * data);

/**
  \brief Set RGB2RGB - 2 config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetRgb2Rgb2Config(CSL_IpipeHandle hndl, CSL_IpipeRgb2RgbConfig * data);

/**
  \brief Set 3D Lookup table config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSet3dLutConfig(CSL_IpipeHandle hndl, CSL_Ipipe3dLutConfig * data);

/**
  \brief Set RGB2YUV config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetRgb2YuvConfig(CSL_IpipeHandle hndl, CSL_IpipeRgb2YuvConfig * data);

/**
  \brief Set Contrast Brightness config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetCntBrtConfig(CSL_IpipeHandle hndl, CSL_IpipeCntBrtConfig * data);

/**
  \brief Set Global Brigtness and Contrast Enhancement (GBCE) config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetGbceConfig(CSL_IpipeHandle hndl, CSL_IpipeGbceConfig * data);

/**
  \brief Set Edge Enhancement config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetEdgeEnhanceConfig(CSL_IpipeHandle hndl, CSL_IpipeEdgeEnhanceConfig * data);

/**
  \brief Set Chroma Artifact Reduction config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetChromaArtifactReduceConfig(CSL_IpipeHandle hndl, CSL_IpipeChromaArtifactReduceConfig * data);

/**
  \brief Set Chroma Gain Supression config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetChromaGainSupressConfig(CSL_IpipeHandle hndl, CSL_IpipeChromaGainSupressConfig * data);

/**
  \brief Enable/Disable IPIPE

  \param hndl   [I ]  Handle
  \param enable [I ]  TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeEnable(CSL_IpipeHandle hndl, Bool32 enable);

/**
  \brief Enable/Disable One-shot mode of IPIPE

  \param hndl     [I ]  Handle
  \param enable   [I ]  TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeOneShotEnable(CSL_IpipeHandle hndl, Bool32 enable);

/**
  \brief Set data path

  \param hndl       [I ] Handle
  \param dataPath   [I ] CSL_IPIPE_IPIPE_DATA_PATH_BAYER_IN_YCBCR_RGB_OUT....CSL_IPIPE_IPIPE_DATA_PATH_BAYER_IN_DISABLE_OUT

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetDataPath(CSL_IpipeHandle hndl, Uint8 dataPath);

/**
  \brief Set Boxcar config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetBoxcarConfig(CSL_IpipeHandle hndl, CSL_IpipeBoxcarConfig * data);

/**
  \brief Enable/Disable Boxcar

  \param hndl   [I ] Handle
  \param enable [I ] TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeBoxcarEnable(CSL_IpipeHandle hndl, Bool32 enable);

/**
  \brief Set Boundary Signal Calculator config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeSetBscConfig(CSL_IpipeHandle hndl, CSL_IpipeBscConfig * data);

/**
  \brief Enable/Disable BSC

  \param hndl   [I ] Handle
  \param enable [I ] TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeBscEnable(CSL_IpipeHandle hndl, Bool32 enable);

/**
  \brief Read BSC results

  \param hndl       [I ] Handle
  \param data       [ O] BSC results

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeBscReadResults(CSL_IpipeHandle hndl, CSL_IpipeBscResults *data);

/**
  \brief Set Histogram config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/

  CSL_Status CSL_ipipeSetHistogramConfig(CSL_IpipeHandle hndl, CSL_IpipeHistogramConfig * data);

/**
  \brief Enable/Disable Histogram

  \param hndl   [I ] Handle
  \param enable [I ] TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeHistogramEnable(CSL_IpipeHandle hndl, Bool32 enable);

  CSL_Status CSL_ipipeHistogramReadIdleY(CSL_IpipeHandle hndl, Uint32 * table);


/**
  \brief Enable/Disable Histogram Region

  \param hndl   [I ]  Handle
  \param enable [I ] TRUE: enable, FALSE: disable, array of 4 element one for each regions

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeHistogramRegionEnable(CSL_IpipeHandle hndl, Bool32 * enable);

/**
  \brief Enable/Disable Histogram Colour

  \param hndl   [I ] Handle
  \param enable [I ] TRUE: enable, FALSE: disable, array of 4 element one for each color

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeHistogramColorEnable(CSL_IpipeHandle hndl, Bool32 * enable);

/**
  \brief Histogram output table select

  \param hndl    [I ] Handle
  \param tableID [I ] tableID: CSL_IPIPE_HISTOGRAM_OUT_TABLE_0_1 or CSL_IPIPE_HISTOGRAM_OUT_TABLE_2_3

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeHistogramTableSelect(CSL_IpipeHandle hndl, Uint32 tableID);

/**
  \brief Read Histogram results

  \param hndl       [I ]  Handle
  \param table      [ O]  Histogram results

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeHistogramReadResults(CSL_IpipeHandle hndl, Uint32 * table,Uint32 offset, Uint32 size);  //ANR-Hist

/**
  \brief Buffer init

  \param hndl     [I ] hndl
  \param bufInit  [I ] Buffer init parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeBscBufInit(CSL_IpipeHandle hndl, CSL_BufInit * bufInit);

/**
  \brief Buffer switch in ISR enable/disable

  \param hndl     [I ] hndl
  \param enable   [I ] TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeBscBufSwitchEnable(CSL_IpipeHandle hndl, Bool32 enable);

/**
  \brief Get Full Buffer

  \param hndl     [I ] hndl
  \param buf      [ O] Full buffer info
  \param minBuf   [I ] minimum number of buffers that should be full
  \param timeout  [I ] Timeout

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeBscBufGetFull(CSL_IpipeHandle hndl, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout);

/**
  \brief Put Empty Buffer

  \param hndl     [I ] hndl
  \param buf      [I ] Empty buffer info

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeBscBufPutEmpty(CSL_IpipeHandle hndl, CSL_BufInfo * buf);

/**
  \brief Enable/Disable module interrupt

  \param hndl          [I ] hndl
  \param ipipeIntType  [I ] interrupt type
  \param enable        [I ] TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeIntEnable(CSL_IpipeHandle hndl, Uint8 ipipeIntType, Bool32 enable);

/**
  \brief Clear module interrupt

  \param hndl          [I ] hndl
  \param ipipeIntType  [I ] interrupt type

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeIntClear(CSL_IpipeHandle hndl, Uint8 ipipeIntType);

/**
  \brief Wait for module interrupt

  \param hndl         [I ] hndl
  \param ipipeIntType [I ] interrupt type

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeIntWait(CSL_IpipeHandle hndl, Uint8 ipipeIntType);

/**
  \brief Module lock : Mutex Lock

  \param hndl     [I ] hndl
  \param timeout  [I ] CSL_SYS_TIMEOUT_NONE or CSL_SYS_TIMEOUT_FOREVER

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeLock(CSL_IpipeHandle hndl, Uint32 timeout);

/**
  \brief Module unlock : Mutex Unlock

  \param hndl     [I ] hndl

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeUnlock(CSL_IpipeHandle hndl);

/**
  \brief Buffer init

  \param hndl     [I ] hndl
  \param bufInit  [I ] Buffer init parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeBoxcarBufInit(CSL_IpipeHandle hndl, CSL_BufInit * bufInit);

/**
  \brief Buffer switch in ISR enable/disable

  \param hndl     [I ] hndl
  \param enable   [I ] TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeBoxcarBufSwitchEnable(CSL_IpipeHandle hndl, Bool32 enable);

/**
  \brief Get Full Buffer

  \param hndl     [I ] hndl
  \param buf      [ O] Full buffer info
  \param minBuf   [I ] minimum number of buffers that should be full
  \param timeout  [I ] Timeout

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeBoxcarBufGetFull(CSL_IpipeHandle hndl, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout);

/**
  \brief Put Empty Buffer

  \param hndl     [I ] hndl
  \param buf      [I ] Empty buffer info

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeBoxcarBufPutEmpty(CSL_IpipeHandle hndl, CSL_BufInfo * buf);

/**
  \brief Set Boxcar output address

  \param hndl     [I ] hndl
  \param outAddr  [I ] output address

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeBoxcarSetOutAddr(CSL_IpipeHandle hndl, Uint8 *outAddr);

#ifdef __cplusplus
}
#endif
/*@}*/
#endif
