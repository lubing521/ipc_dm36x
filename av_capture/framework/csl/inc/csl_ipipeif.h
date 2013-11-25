/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#ifndef _CSL_IPIPEIF_H_
#define _CSL_IPIPEIF_H_

#ifdef __cplusplus
extern  "C" {
#endif

#include <csl_soc.h>

/**
  \file csl_ipipeif.h
  \brief IPIPEIF APIs
*/

/**
 \ingroup  CSL_VPFE
 \defgroup CSL_IPIPEIF IPIPEIF
 \brief Image PIPE InterFace (IPIPEIF)

 @{
*/

// define's
#define CSL_IPIPEIF_IPIPEIF_CLOCK_SELECT_PCLK              0  ///< IPIPE and IPIPEIF clock select : Pixel clock
#define CSL_IPIPEIF_IPIPEIF_CLOCK_SELECT_SDRAM_DIV_CLK     1  ///< IPIPE and IPIPEIF clock select : Divided SDRAM clock as per CLKDIV

#define CSL_IPIPEIF_IPIPEIF_DPCM_BITS_8_BIT_TO_10_BIT      0  ///< DPCM bit mode for SDRAM data : 8 bit to 10 bit DPCM
#define CSL_IPIPEIF_IPIPEIF_DPCM_BITS_10_BIT_TO_12_BIT     1  ///< DPCM bit mode for SDRAM data : 10 bit to 12 bit DPCM

#define CSL_IPIPEIF_IPIPEIF_DPCM_PERDICTION_MODE_SIMPLE    0  ///< DPCM prediction mode for SDRAM data : Simple predictor
#define CSL_IPIPEIF_IPIPEIF_DPCM_PERDICTION_MODE_ADVANCED  1  ///< DPCM prediction mode for SDRAM data : Advanced predictor

#define CSL_IPIPEIF_IPIPEIF_SDRAM_IN_MODE_CONTINUOUS       0  ///< IPIPEIF SDRAM mode config : continuous mode
#define CSL_IPIPEIF_IPIPEIF_SDRAM_IN_MODE_ONE_SHOT         1  ///< IPIPEIF SDRAM mode config : one shot mode

#define CSL_IPIPEIF_SDRAM_IN_DATASHIFT_BITS_11_0  0 ///< Reads bits 0 to 11
#define CSL_IPIPEIF_SDRAM_IN_DATASHIFT_BITS_10_0  1 ///< Reads bits 0 to 10 and & with 0
#define CSL_IPIPEIF_SDRAM_IN_DATASHIFT_BITS_09_0  2 ///< Reads bits 0 to 9  and & with 00
#define CSL_IPIPEIF_SDRAM_IN_DATASHIFT_BITS_08_0  3 ///< Reads bits 0 to 8  and & with 000
#define CSL_IPIPEIF_SDRAM_IN_DATASHIFT_BITS_07_0  4 ///< Reads bits 0 to 7  and & with 0000
#define CSL_IPIPEIF_SDRAM_IN_DATASHIFT_BITS_15_4  5 ///< Reads bits 4 to 15

#define CSL_IPIPEIF_SDRAM_IN_RAW_UNPACK_MODE_NORMAL_16  0 ///<IPIPEIF PACK MODE      : 16 bits per pixel
#define CSL_IPIPEIF_SDRAM_IN_RAW_UNPACK_MODE_PACK_08    1 ///<IPIPEIF PACK MODE      :  8 bits per pixel
#define CSL_IPIPEIF_SDRAM_IN_RAW_UNPACK_MODE_PACK_10    2 ///<IPIPEIF PACK MODE      :  8 bits per pixel + inverse ALAW (8 bits to 10 bits)
#define CSL_IPIPEIF_SDRAM_IN_RAW_UNPACK_MODE_PACK_12    3 ///<IPIPEIF PACK MODE      : 12 bits per pixel

#define CSL_IPIPEIF_DFS_DIRECTION_SENSOR_PARALLEL_IF_MINUS_SDRAM 0  ///< DFS DIRECTION : Sensor parallel IF (capture frame) - SDRAM(dark frame)
#define CSL_IPIPEIF_DFS_DIRECTION_SDRAM_MINUS_SENSOR_PARALLEL_IF 1  ///< DFS DIRECTION : SDRAM (capture frame) - Sensor parallel IF (dark frame)

#define CSL_IPIPEIF_HDPOLARITY_POSITIVE              0  ///< HD Polarity :Positive
#define CSL_IPIPEIF_HDPOLARITY_NEGATIVE              1  ///< HD Polarity :Negative

#define CSL_IPIPEIF_VDPOLARITY_POSITIVE              0  ///< VD Polarity :Positive
#define CSL_IPIPEIF_VDPOLARITY_NEGATIVE              1  ///< VD Polarity :Negative

#define CSL_IPIPEIF_INPUT_SOURCE_PARALLEL_PORT_RAW                0 ///< Input source : Raw data from parallel port
#define CSL_IPIPEIF_INPUT_SOURCE_SDRAM_RAW                        1 ///< Input source : Raw data from SDRAM
#define CSL_IPIPEIF_INPUT_SOURCE_PARALLEL_PORT_SDRAM_DARKFRAME    2 ///< Input source : ISIF dark frame from SDRAM and Parallel port
#define CSL_IPIPEIF_INPUT_SOURCE_SDRAM_YUV                        3 ///< Input source : YUV data from SDRAM

#define CSL_IPIPEIF_CHROMA_POS_CbYCrY  0  ///< Chroma position CbYCrY
#define CSL_IPIPEIF_CHROMA_POS_YCbYCr  1  ///< Chroma position YCbYCr

#define CSL_IPIPEIF_ISIF_INPUT_TYPE_RAW 0 ///< ISIF input data type : RAW
#define CSL_IPIPEIF_ISIF_INPUT_TYPE_YUV 1 ///< ISIF input data type : YUV

#define CSL_IPIPEIF_INTERRUPT_SOURCE_PARALLEL_PORTIF 0  ///<Interrupt source : Start position of VD from Parallel port I/F
#define CSL_IPIPEIF_INTERRUPT_SOURCE_ISIF            1  ///<Interrupt source : Start position of VD from ISIF

/**
  \brief IPIPEIF clock configuration  parameters
*/
  typedef struct {

    Uint8   clkSel;             ///< CSL_IPIPEIF_IPIPEIF_CLOCK_SELECT_PCLK \n CSL_IPIPEIF_IPIPEIF_CLOCK_SELECT_SDRAM_DIV_CLK .Valid when input source is SDRAM
    Uint16  clkDivM;            ///< Numerator value of ratio used to calculate IPIPEIF clock frequency
    Uint16  clkDivN;            ///< Denominator value of ratio used to calculate IPIPEIF clock frequency \n IPIPEIF clock frequency = clkDivM/clkDivN times VPSS clock frequency ; Valid only when CLKSEL register is set. M/N should be lesser than 0.5 when VPSSCLK >121.5 MHz

    Uint16  pixelsPerLine;      ///< VD sync interval when input source is SDRAM \n Start pixel in vertical sync when input source is parallel port
    Uint16  linesPerFrame;      ///< HD sync interval when input source is SDRAM \n Start pixel in horizontal sync when input source is parallel port

  } CSL_IpipeifClkConfig;

/**
  \brief DPCM configuration
*/
  typedef struct {

    Bool32    enable;             ///< TRUE :Enable DPCM for SDRAM input \n FALSE:Disable DPCM for SDRAM input
    Uint8   bits;               ///< CSL_IPIPEIF_IPIPEIF_DPCM_BITS_8_BIT_TO_10_BIT \n CSL_IPIPEIF_IPIPEIF_DPCM_BITS_10_BIT_TO_12_BIT
    Uint8   predType;           ///< CSL_IPIPEIF_IPIPEIF_DPCM_PERDICTION_MODE_SIMPLE \n CSL_IPIPEIF_IPIPEIF_DPCM_PERDICTION_MODE_ADVANCED

  } CSL_IpipeifDpcmConfig;

/**
  \brief SDRAM input frame configuration
*/
  typedef struct {

    Uint8  *inputAddr;          ///< Address of frame ,specified in bytes
    Uint16  inputWidth;         ///< Width of frame , specified in pixels
    Uint16  inputHeight;        ///< Height of frame , specified in lines
    Uint16  inputLineOffset;    ///< Address offset of each line , in bytes

  } CSL_IpipeifSdramInFrameConfig;

/**
  \brief SDRAM input configuration
*/
  typedef struct {

    Bool32    enable;             ///< TRUE : enable \n FALSE : disable
    Bool32    oneShotEnable;      ///< CSL_IPIPEIF_IPIPEIF_SDRAM_CONFIG_MODE_CONTINUOUS \n  CSL_IPIPEIF_IPIPEIF_SDRAM_CONFIG_MODE_ONE_SHOT

    Uint8   rawDataShift;       ///< CSL_IPIPEIF_IPIPEIF_SDRAM_IN_DATASHIFT_BITS_11_0 \n CSL_IPIPEIF_IPIPEIF_SDRAM_IN_DATASHIFT_BITS_07_0 \n CSL_IPIPEIF_IPIPEIF_SDRAM_IN_DATASHIFT_BITS_15_4

    Uint8   rawUnpackMode;      ///< CSL_IPIPEIF_IPIPEIF_SDRAM_IN_RAW_UNPACK_MODE_NORMAL_16 \n CSL_IPIPEIF_IPIPEIF_SDRAM_IN_RAW_UNPACK_MODE_PACK_08 ... CSL_IPIPEIF_IPIPEIF_SDRAM_IN_RAW_UNPACK_MODE_PACK_12
    Uint8  *inputAddr;          ///< Input address  ,specified in bytes
    Uint16  inputWidth;         ///< Width  , specified in pixels
    Uint16  inputHeight;        ///< Height , specified in lines
    Uint16  inputLineOffset;    ///< Address offset of each line , specified in bytes

    Bool32    dpcEnable;          ///< TRUE : enable \n FALSE :disable
    Uint32  dpcThreshold;       ///< DPC threshold value // to do  units

    CSL_IpipeifDpcmConfig *dpcmConfig;  ///< DPCM config
    CSL_IpipeifClkConfig *clkConfig;  ///< clock config

  } CSL_IpipeifSdramInConfig;

/**
  \brief Input source2 configuration
*/
  typedef struct {

    Uint8   inputSource2;       ///< CSL_IPIPEIF_INPUT_SOURCE_PARALLEL_PORT_RAW: from ISIF, CSL_IPIPEIF_INPUT_SOURCE_SDRAM_RAW: SDRAM DDR, CSL_IPIPEIF_INPUT_SOURCE_PARALLEL_PORT_SDRAM_DARKFRAME: Dark frame Sub, CSL_IPIPEIF_INPUT_SOURCE_SDRAM_YUV: SDRAM YUV

    Bool32    pixelDecimationEnable;  ///< TRUE : Pixel Decimation enable \n FALSE : No decimation
    Uint8   pixelDecimationRatio; ///< Pixel decimation ratio should be in the range of 1/7 to 1 .

    Bool32    avgFilterEnable;    ///< TRUE : Averaging filter on \n FALSE: Averaging filter off

    Bool32    alignHsyncVsync;    ///< TRUE : Align HSYNC and VSYNC to initial position defined in INIRSZ \n FALSE: No align
    Uint16  initReszPosH;       ///< intial position of resizer in HSYNC

    Uint32  outClip;            ///< ouptut clipping value after after gain control

    Uint32  gain;               ///< Gain parameter for IPIPE output data . to be in the range of 0.00195 to 1.99805

    Uint8   isifInType;         ///< Valid only when inputSource2 = ISIF, CSL_IPIPEIF_ISIF_INPUT_TYPE_RAW \n CSL_IPIPEIF_ISIF_INPUT_TYPE_YUV
    Bool32    yuvConvertEnable;   ///< FALSE: YUV 16-bit from ISIF, TRUE: YUV 8-bit from ISIF convert to YUV 16-bit , CGF1.INSRC2 should be set to 1 and ISIF input type should be set to YUV

    Bool32    vsyncMaskEnable;    ///< TRUE:VSYNC output mask enabled \n FALSE :VSYNC output mask disabled

  } CSL_IpipeifInSource2Config;

/**
  \brief Dark frame subtraction related configuration parameters
*/
  typedef struct {

    Bool32    gainEnable;         ///< TRUE: DFS gain control enable \n FALSE: DFS gain control disable
    Uint16  gain;               ///< DFS gain value
    Uint16  gainThreshold;      ///< DFS gain threshold value
    Uint8   subtractDir;        ///< CSL_IPIPEIF_DFS_DIRECTION_SENSOR_PARALLEL_IF_MINUS_SDRAM \n , 1: CSL_IPIPEIF_DFS_DIRECTION_SDRAM_MINUS_SENSOR_PARALLEL_IF

  } CSL_IpipeifDarkFrameConfig;

/**
  \brief Configuration of Horizontal resizing for H3A
*/
  typedef struct {

    Bool32    pixelDecimationEnable;  ///< TRUE : Enable pixel decimator \n FALSE : Disable pixel decimator
    Uint8   pixelDecimationRatio; ///< Pixel decimator ratio. Can be configured in the range 16 - 112
    Bool32    avgFilterEnable;    ///< TRUE : Averaging filter on \n FALSE: Averaging filter off
    Bool32    alignHsyncVsync;    ///< TRUE : Align HSYNC and VSYNC to initial position defined in INIRSZ \n FALSE: No align
    Uint16  initReszPosH;       ///< Inital position of resizer

  } CSL_IpipeifH3aConfig;

/**
  \brief VPIF(Parallel Port) / ISIF input configuration
*/
  typedef struct {

    Bool32    dpcEnable;          ///< TRUE : Enable DPC for VPIF/ISIF input data \n FALSE : Disable Enable DPC for VPIF/ISIF input data
    Uint32  dpcThreshold;       ///< DPC threshold value
    Bool32    wenUseEnable;       ///< TRUE : Use external WEN signal \n FALSE : Not to use external WEN signal
    Uint16  vdPol;              ///< CSL_IPIPEIF_HDPOLARITY_POSITIVE \n  CSL_IPIPEIF_HDPOLARITY_NEGATIVE
    Uint16  hdPol;              ///< CSL_IPIPEIF_VDPOLARITY_POSITIVE \n  CSL_IPIPEIF_VDPOLARITY_NEGATIVE

  } CSL_IpipeifVpifIsifInConfig;

/**
  \brief Hardware Setup structure
*/
  typedef struct {

    Bool32    bypassModuleIfNullConfig; ///< if any sub-module related parameters structure's are NULL, then if \n TRUE : Set the registers such that the module is either disabled or bypassed or set to default values \n FALSE: Do not set any sub-module related registers, i.e old values are maintained

    Uint8   inputSource1;       ///< CSL_IPIPEIF_INPUT_SOURCE_PARALLEL_PORT_RAW \n CSL_IPIPEIF_INPUT_SOURCE_SDRAM_RAW \n CSL_IPIPEIF_INPUT_SOURCE_PARALLEL_PORT_SDRAM_DARKFRAME \n CSL_IPIPEIF_INPUT_SOURCE_SDRAM_YUV
    Uint8   chromaPos;          ///< CSL_IPIPEIF_CHROMA_POS_CbYCrY \n  CSL_IPIPEIF_CHROMA_POS_YCbYCr
    Uint8   interuptSource;     ///< CSL_IPIPEIF_INTERRUPT_SOURCE_PARALLEL_PORTIF \n CSL_IPIPEIF_INTERRUPT_SOURCE_ISIF

    CSL_IpipeifSdramInConfig *sdramInConfig;  ///< SDRAM input config
    CSL_IpipeifInSource2Config *inSource2Config;  ///< Input source config
    CSL_IpipeifVpifIsifInConfig *vpifIsifInConfig;  ///< ISIF input config
    CSL_IpipeifDarkFrameConfig *darkFrameConfig;  ///< Dark frame config
    CSL_IpipeifH3aConfig *h3aConfig;  ///< H3A config

  } CSL_IpipeifHwSetup;

/**
  \brief Module Object Structure
*/
  typedef struct {

    CSL_IpipeifRegsOvly regs;   ///< Register overlay
    
#ifdef CSL_KERNEL_API

#else
    int     fd;
#endif

  } CSL_IpipeifObj;

/**
  \brief Sensor sync related configuration
*/
  typedef CSL_IpipeifObj *CSL_IpipeifHandle;  ///<MOdule Handle

/**
  \brief Open Module Handle

  \param hndl   [ O]  Module Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifOpen(CSL_IpipeifHandle hndl);

/**
  \brief Close Module Handle

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifClose(CSL_IpipeifHandle hndl);

/**
  \brief Set all module registers

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifHwSetup(CSL_IpipeifHandle hndl, CSL_IpipeifHwSetup * data);

/**
  \brief Set all module registers to power-ON reset value

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifHwReset(CSL_IpipeifHandle hndl);

/**
  \brief Set sdram input config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifSetSdramInConfig(CSL_IpipeifHandle hndl, CSL_IpipeifSdramInConfig * data);

/**
  \brief Set in sdram input frame config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifSetSdramInFrameConfig(CSL_IpipeifHandle hndl, CSL_IpipeifSdramInFrameConfig * data);

/**
  \brief Set input source2 config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifSetInSource2Config(CSL_IpipeifHandle hndl, CSL_IpipeifInSource2Config * data);

/**
  \brief Set VPIF ISIF input config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifSetVpifIsifInConfig(CSL_IpipeifHandle hndl, CSL_IpipeifVpifIsifInConfig * data);

/**
  \brief Set dark frame config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifSetDarkFrameConfig(CSL_IpipeifHandle hndl, CSL_IpipeifDarkFrameConfig * data);

/**
  \brief Set color H3A config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifSetH3aConfig(CSL_IpipeifHandle hndl, CSL_IpipeifH3aConfig * data);

/**
  \brief Set clock config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifSetClkConfig(CSL_IpipeifHandle hndl, CSL_IpipeifClkConfig * data);

/**
  \brief Set dpcm config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifSetDpcmConfig(CSL_IpipeifHandle hndl, CSL_IpipeifDpcmConfig * data);

/**
  \brief Set input source1

  \param hndl           [I ]  Handle
  \param inputSource1   [I ]  CSL_IPIPEIF_INPUT_SOURCE_PARALLEL_PORT_RAW \n CSL_IPIPEIF_INPUT_SOURCE_SDRAM_RAW \n CSL_IPIPEIF_INPUT_SOURCE_PARALLEL_PORT_SDRAM_DARKFRAME \n CSL_IPIPEIF_INPUT_SOURCE_SDRAM_YUV

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifSetInputSource1(CSL_IpipeifHandle hndl, Uint8 inputSource1);

/**
  \brief Sets the IPIPEIF address as the value specified in address

  \param hndl         [I ]  Handle
  \param addr         [I ]  Input address

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifSetSdramInAddr(CSL_IpipeifHandle hndl, Uint8 * addr);

/**
  \brief Enable  SDRAM  as input source

  \param hndl           [I ]  Handle
  \param enable         [I ]  TRUE: SDRAM input enabled, FALSE: SDRAM input not enabled

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifSdramInEnable(CSL_IpipeifHandle hndl, Bool32 enable);

/**
  \brief Enable SDRAM one shot mode

  \param hndl           [I ]  Handle
  \param enable         [I ]  TRUE : One shot mode enabled,FALSE:Continuous mode enabled

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifSdramInOneShotEnable(CSL_IpipeifHandle hndl, Bool32 enable);

/**
  \brief Clears under flow defect value by writing TRUE (1) to DTUDF register

  \param hndl   [I ]  handle

  \return CSL_SOK on success, else CSL_Exxxx
*/

  CSL_Status CSL_ipipeifUnderflowDetectClear(CSL_IpipeifHandle hndl);

/**
  \brief Checks under flow defect value

  \param hndl             [I ]  Handle
  \param underflowDetect  [ O]  TRUE if underflow has occured , else FALSE

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifIsUnderflowDetect(CSL_IpipeifHandle hndl, Bool32 * underflowDetect);

/**
  \brief Module lock : Mutex Lock

  \param hndl     [I ] hndl
  \param timeout  [I ] CSL_SYS_TIMEOUT_NONE or CSL_SYS_TIMEOUT_FOREVER

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifLock(CSL_IpipeifHandle hndl, Uint32 timeout);

/**
  \brief Module unlock : Mutex Unlock

  \param hndl     [I ] hndl

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ipipeifUnlock(CSL_IpipeifHandle hndl);

#ifdef __cplusplus
}
#endif
/*@}*/
#endif
