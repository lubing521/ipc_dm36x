/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#ifndef _CSL_CCDC_H_
#define _CSL_CCDC_H_

#ifdef __cplusplus
extern  "C" {
#endif

#include <csl_soc.h>
#include <csl_buf.h>

/**
  \file csl_ccdc.h
  \brief CCDC APIs
*/

/**
 \ingroup  CSL_VPFE
 \defgroup CSL_CCDC CCDC / ISIF
 \brief CCD Controller (CCDC) / Image Sensor Interface (ISIF)

 @{
*/

// define's

#define CSL_CCDC_IN_DATA_TYPE_RAW     0 ///< Input Data Type: Raw Data
#define CSL_CCDC_IN_DATA_TYPE_YUV16   1 ///< Input Data Type: YCbCr 16-bit
#define CSL_CCDC_IN_DATA_TYPE_YUV8    2 ///< Input Data Type: YCbCr 8-bit

#define CSL_CCDC_DATA_POLARITY_NO_CHANGE          0 ///< Data polarity: No Change
#define CSL_CCDC_DATA_POLARITY_ONES_COMPLEMENT    1 ///< Data polarity: One's complement

#define CSL_CCDC_MSB_POSITION_BIT15   0 ///< MSB Position: bit15
#define CSL_CCDC_MSB_POSITION_BIT14   1 ///< MSB Position: bit14
#define CSL_CCDC_MSB_POSITION_BIT13   2 ///< MSB Position: bit13
#define CSL_CCDC_MSB_POSITION_BIT12   3 ///< MSB Position: bit12
#define CSL_CCDC_MSB_POSITION_BIT11   4 ///< MSB Position: bit11
#define CSL_CCDC_MSB_POSITION_BIT10   5 ///< MSB Position: bit10
#define CSL_CCDC_MSB_POSITION_BIT9    6 ///< MSB Position: bit9
#define CSL_CCDC_MSB_POSITION_BIT8    7 ///< MSB Position: bit8
#define CSL_CCDC_MSB_POSITION_BIT7    8 ///< MSB Position: bit7

#define CSL_CCDC_Y_POS_EVEN     0 ///< Y Position: Even
#define CSL_CCDC_Y_POS_ODD      1 ///< Y Position: Odd

#define CSL_CCDC_SIGNAL_POLARITY_POSITIVE   0 ///< FID, HD, VD Signal polarity : Positive
#define CSL_CCDC_SIGNAL_POLARITY_NEGATIVE   1 ///< FID, HD, VD Signal polarity : Negative

#define CSL_CCDC_SIGNAL_DIR_INPUT   0 ///< FID, HD, VD Signal direction : Input
#define CSL_CCDC_SIGNAL_DIR_OUTPUT  1 ///< FID, HD, VD Signal direction : Output

#define CSL_CCDC_SDR_OFFSET_PLUS_1LINE      0 ///< SDRAM Line Offset : +1 line
#define CSL_CCDC_SDR_OFFSET_PLUS_2LINE      1 ///< SDRAM Line Offset : +2 line
#define CSL_CCDC_SDR_OFFSET_PLUS_3LINE      2 ///< SDRAM Line Offset : +3 line
#define CSL_CCDC_SDR_OFFSET_PLUS_4LINE      3 ///< SDRAM Line Offset : +4 line
#define CSL_CCDC_SDR_OFFSET_MINUS_1LINE     4 ///< SDRAM Line Offset : -1 line
#define CSL_CCDC_SDR_OFFSET_MINUS_2LINE     5 ///< SDRAM Line Offset : -2 line
#define CSL_CCDC_SDR_OFFSET_MINUS_3LINE     6 ///< SDRAM Line Offset : -3 line
#define CSL_CCDC_SDR_OFFSET_MINUS_4LINE     7 ///< SDRAM Line Offset : -4 line

#define CSL_CCDC_SDR_OUT_TYPE_16BITS_PER_PIXEL    0 ///< SDRAM output type : 16bits/pixel
#define CSL_CCDC_SDR_OUT_TYPE_12BITS_PER_PIXEL    1 ///< SDRAM output type : 12bits/pixel
#define CSL_CCDC_SDR_OUT_TYPE_8BITS_PER_PIXEL     2 ///< SDRAM output type :  8bits/pixel

#define CSL_CCDC_COL_PAT_R      0 ///< Color Pattern color : R
#define CSL_CCDC_COL_PAT_Gr     1 ///< Color Pattern color : Gr
#define CSL_CCDC_COL_PAT_Gb     2 ///< Color Pattern color : Gb
#define CSL_CCDC_COL_PAT_B      3 ///< Color Pattern color : B

#define CSL_CCDC_DPCM_PREDICTOR_1   0 ///< DPCM Predictor 1
#define CSL_CCDC_DPCM_PREDICTOR_2   1 ///< DPCM Predictor 2

#define CSL_CCDC_REC656_DATA_WIDTH_8BIT   0 ///< REC656 Data Width :  8-bit
#define CSL_CCDC_REC656_DATA_WIDTH_10BIT  1 ///< REC656 Data Width : 10-bit

#define CSL_CCDC_CFA_PATTERN_MOSIAC       0 ///< CFA Pattern type : Mosiac
#define CSL_CCDC_CFA_PATTERN_STRIPE       1 ///< CFA Pattern type : Stripe

#define CSL_CCDC_SDR_ADDR_INIT_DWEN   0 ///< Source signal of SDRAM address initializing: DWEN register
#define CSL_CCDC_SDR_ADDR_INIT_FID    1 ///< Source signal of SDRAM address initializing: FID signal

#define CSL_CCDC_VDFC_MODE_FED_TRHU_IF_SAT    0 ///< VDFC Mode: fed thru if data saturating
#define CSL_CCDC_VDFC_MODE_HORZ_INTP_IF_SAT   1 ///< VDFC Mode: Horz interpollation if data saturating
#define CSL_CCDC_VDFC_MODE_HORZ_INTP          2 ///< VDFC Mode: horz interpollation


#define CSL_CCDC_CLAMP_MODE_IGNORE_COLOR      0 ///< Clamp mode : Clamp value calc regardless of color
#define CSL_CCDC_CLAMP_MODE_USE_COLOR         1 ///< Clamp mode : Clamp value calc based on color

#define CSL_CCDC_HORZ_CLAMP_MODE_DISABLE        0 ///< Horizontal Clamp mode : Disabled
#define CSL_CCDC_HORZ_CLAMP_MODE_ENABLE         1 ///< Horizontal Clamp mode : Enable
#define CSL_CCDC_HORZ_CLAMP_MODE_NOT_UPDATED    2 ///< Horizontal Clamp mode : Value not updated

#define CSL_CCDC_HORZ_CLAMP_BASE_WIN_LEFT_MOST   0  ///< Base Window position : Left most
#define CSL_CCDC_HORZ_CLAMP_BASE_WIN_RIGHT_MOST  1  ///< Base Window position : Right most

#define CSL_CCDC_VERT_CLAMP_RESET_VAL_SRC_HDIR_BASE_VAL   0 ///< Reset Value Source : Horz dir base value
#define CSL_CCDC_VERT_CLAMP_RESET_VAL_SRC_CFG_REG         1 ///< Reset Value Source : config register
#define CSL_CCDC_VERT_CLAMP_RESET_VAL_SRC_NO_UPDATE       2 ///< Reset Value Source : No update

#define CSL_CCDC_FMT_LINE_SPLITING        0 ///< Formatter mode : line split
#define CSL_CCDC_FMT_LINE_COMBINING       1 ///< Formatter mode : line combine
#define CSL_CCDC_FMT_LINE_ALTERNATING     2 ///< Formatter mode : line alternate

#define CSL_CCDC_FMT_AUTO_INC     0 ///< Formatter : Auto decrement
#define CSL_CCDC_FMT_AUTO_DEC     1 ///< Formatter : Auto increment


#define CSL_CCDC_INT_TYPE_VD0   0 ///< CCDC Interrupt : VD0
#define CSL_CCDC_INT_TYPE_VD1   1 ///< CCDC Interrupt : VD1
#define CSL_CCDC_INT_TYPE_MAX   2 ///< CCDC Interrupt : MAX

#define CSL_ISIF_LIN_TBL_MAX_ENTRIES (192) ///< Max possible entries in linearization table

// data structure's

/**
  \brief Input Data related configuration
*/
  typedef struct {

    Bool32    hLpfEnable;         ///< TRUE: Horizontal LPF enable, FALSE: Horizontal LPF disable
    Uint32  inDataMsbPosition;  ///< CSL_CCDC_MSB_POSITION_BIT15 .. CSL_CCDC_MSB_POSITION_BIT7
    Uint8   inDataType;         ///< CSL_CCDC_IN_DATA_TYPE_RAW \n CSL_CCDC_IN_DATA_TYPE_YUV16 \n CSL_CCDC_IN_DATA_TYPE_YUV8
    Uint8   dataPolarity;       ///< CSL_CCDC_DATA_POLARITY_NO_CHANGE \n CSL_CCDC_DATA_POLARITY_ONES_COMPLEMENT
    Uint8   yPos;               ///< Valid only in YUV 8-bit input mode \n CSL_CCDC_Y_POS_EVEN \n CSL_CCDC_Y_POS_ODD

  } CSL_CcdcInDataConfig;

/**
  \brief Sensor sync related configuration
*/
  typedef struct {

    Bool32    interlaceMode;      ///< TRUE: interlaced mode, FALSE: non-interlaced mode
    Bool32    wenUseEnable;       ///< TRUE: Use External WEN signal, FALSE: ignore external WEN signal

    Uint8   fidPolarity;        ///< CSL_CCDC_SIGNAL_POLARITY_POSITIVE \n CSL_CCDC_SIGNAL_POLARITY_NEGATIVE
    Uint8   hdPolarity;         ///< CSL_CCDC_SIGNAL_POLARITY_POSITIVE \n CSL_CCDC_SIGNAL_POLARITY_NEGATIVE
    Uint8   vdPolarity;         ///< CSL_CCDC_SIGNAL_POLARITY_POSITIVE \n CSL_CCDC_SIGNAL_POLARITY_NEGATIVE
    Uint8   hdVdDir;            ///< CSL_CCDC_SIGNAL_DIR_INPUT \n CSL_CCDC_SIGNAL_DIR_OUTPUT
    Uint8   fidDir;             ///< CSL_CCDC_SIGNAL_DIR_INPUT \n CSL_CCDC_SIGNAL_DIR_OUTPUT

    Uint16  hdWidth;            ///< HD signal width, in units of PCLK
    Uint16  vdWidth;            ///< VD signal width, in units of HD

    Uint16  pixelsPerLine;      ///< Pixels per line, in units of pixels
    Uint16  linesPerFrame;      ///< Lines per frame, in units of lines

  } CSL_CcdcSyncConfig;

/**
  \brief SDRAM Output Line Offset Configuration
*/
  typedef struct {

    Bool32    fidInv;             ///< TRUE: FID signal inverse, FALSE: FID signal no inverse

    Uint8   fidOffsetOdd;       ///< line offset odd field \n CSL_CCDC_SDR_OFFSET_PLUS_1LINE .. CSL_CCDC_SDR_OFFSET_PLUS_4LINE
    Uint8   lineOffsetEvenEven; ///< line offset even line even field \n CSL_CCDC_SDR_OFFSET_PLUS_1LINE .. CSL_CCDC_SDR_OFFSET_PLUS_4LINE \n CSL_CCDC_SDR_OFFSET_MINUS_1LINE .. CSL_CCDC_SDR_OFFSET_MINUS_4LINE
    Uint8   lineOffsetOddEven;  ///< line offset odd  line even field \n CSL_CCDC_SDR_OFFSET_PLUS_1LINE .. CSL_CCDC_SDR_OFFSET_PLUS_4LINE \n CSL_CCDC_SDR_OFFSET_MINUS_1LINE .. CSL_CCDC_SDR_OFFSET_MINUS_4LINE
    Uint8   lineOffsetEvenOdd;  ///< line offset even line odd  field \n CSL_CCDC_SDR_OFFSET_PLUS_1LINE .. CSL_CCDC_SDR_OFFSET_PLUS_4LINE \n CSL_CCDC_SDR_OFFSET_MINUS_1LINE .. CSL_CCDC_SDR_OFFSET_MINUS_4LINE
    Uint8   lineOffsetOddOdd;   ///< line offset odd  line odd  field \n CSL_CCDC_SDR_OFFSET_PLUS_1LINE .. CSL_CCDC_SDR_OFFSET_PLUS_4LINE \n CSL_CCDC_SDR_OFFSET_MINUS_1LINE .. CSL_CCDC_SDR_OFFSET_MINUS_4LINE

  } CSL_CcdcSdramOffsetConfig;

/**
  \brief SDRAM output size configuration
*/
  typedef struct {

    Uint16  outStartH;          ///< Output start pixel in H direction, in units of pixels
    Uint16  outStartV0;         ///< Output start line (field 0), in units of lines
    Uint16  outStartV1;         ///< Output start line (field 1), in units of lines
    Uint16  outWidth;           ///< Output width , in units of pixels
    Uint16  outHeight;          ///< Output height , in units of lines
    Uint16  outFid;             ///< Output FID

  } CSL_CcdcSdramOutSizeConfig;

/**
  \brief SDRAM output related configuration
*/
  typedef struct {

    Uint8   outDataShift;       ///< 0..4 in units of bits, 0: No Shift, 1: 1bit right shift .. 4: 4bit right shift

    Uint16  outStartH;          ///< Output start pixel in H direction, in units of pixels
    Uint16  outStartV0;         ///< Output start line (field 0), in units of lines
    Uint16  outStartV1;         ///< Output start line (field 1), in units of lines
    Uint16  outWidth;           ///< Output width , in units of pixels
    Uint16  outHeight;          ///< Output height , in units of lines

    Uint8   culHEven;           ///< Culling pattern H direction - even field
    Uint8   culHOdd;            ///< Culling pattern H direction - odd field
    Uint8   culV;               ///< Culling pattern V direction

    Uint16  outLineOffset;      ///< Output line offset, in units of bytes, MUST be multiple of 32bytes
    Bool32    outAddrDecrement;   ///< FALSE: Addr increment, TRUE: Address decrement

    CSL_CcdcSdramOffsetConfig sdramOffsetConfig;  ///< Output line offset config

    Uint8  *outAddr;            ///< Output Address, MUST be 32-byte aligned

    Bool32    alawEnable;         ///< TRUE: Enabled A-law, FALSE: Disable A-law
    Bool32    byteSwapEnable;     ///< TRUE: Byte-swap data, FALSE: No byte-swapping

    Uint8   packMode;           ///< CSL_CCDC_SDR_OUT_TYPE_16BITS_PER_PIXEL \n CSL_CCDC_SDR_OUT_TYPE_12BITS_PER_PIXEL \n CSL_CCDC_SDR_OUT_TYPE_8BITS_PER_PIXEL

  } CSL_CcdcSdramOutConfig;

/**
  \brief Linearization related configuration
*/
  typedef struct {

    Bool32    enable;             ///< TRUE: enable, FALSE: disable
    Uint32  corrValShift;       ///< Correction Value Left Shift, 0: No Shift, 1..6: Left shift by 1..6 bits
    Uint32  lutInScale;         ///< look-up table input scale (U11Q10)
    Uint32  numLutEntries;      ///< Number of look-up table entries, valid values are 2, 4, 8, 16, 32, 64, 128 (max)
    Uint32 *lutTable;           ///< Max CSL_ISIF_LIN_TBL_MAX_ENTRIES entries

  } CSL_CcdcLinerizationConfig;

/**
  \brief Color Pattern
*/
  typedef struct {

    Uint32   colPat0[2][2];      ///< Color pattern for field 0, CSL_CCDC_COL_PAT_R, CSL_CCDC_COL_PAT_Gr, CSL_CCDC_COL_PAT_Gb, CSL_CCDC_COL_PAT_B
    Uint32   colPat1[2][2];      ///< Color pattern for field 1, CSL_CCDC_COL_PAT_R, CSL_CCDC_COL_PAT_Gr, CSL_CCDC_COL_PAT_Gb, CSL_CCDC_COL_PAT_B

  } CSL_CcdcColPatConfig;

/**
  \brief Gain / offset related configuration
*/
  typedef struct {

    Uint32  gainR;              ///< R  gain (U12Q9)
    Uint32  gainGr;             ///< Gr gain (U12Q9)
    Uint32  gainGb;             ///< Gb gain (U12Q9)
    Uint32  gainB;              ///< B  gain (U12Q9)

    Uint32  offset;             ///< offset (U12Q0)

    Bool32    ipipeGainEnable;    ///< IPIPE Path Gain   Control, TRUE: enable, FALSE: disable
    Bool32    ipipeOffsetEnable;  ///< IPIPE Path Offset Control, TRUE: enable, FALSE: disable

    Bool32    h3aGainEnable;      ///< H3A Path Gain   Control, TRUE: enable, FALSE: disable
    Bool32    h3aOffsetEnable;    ///< H3A Path Offset Control, TRUE: enable, FALSE: disable

    Bool32    sdramOutGainEnable; ///< SDRAM OUT Path Gain   Control, TRUE: enable, FALSE: disable
    Bool32    sdramOutOffsetEnable; ///< SDRAM OUT Path Offset Control, TRUE: enable, FALSE: disable

  } CSL_CcdcGainOffsetConfig;

/**
  \brief Flash timing signal related configuration
*/
  typedef struct {

    Bool32    timingEnable;       ///< TRUE: enable Flash timing signal, FALSE: disable
    Uint16  timingStartLine;    ///< Start line of Flash timing signal, relative to start of VD
    Uint32  timingWidth;        ///< Width of Flash timing signal, in units of Crystal clock

  } CSL_CcdcFlashConfig;

/**
  \brief VD interrupt timing configuration
*/
  typedef struct {

    Uint16  vdInt0;             ///< VD0 interrupt timing relative to VD start, in units of HD \n 0xFFFF means do not update value for this interrupt
    Uint16  vdInt1;             ///< VD1 interrupt timing relative to VD start, in units of HD \n 0xFFFF means do not update value for this interrupt
    Uint16  vdInt2;             ///< VD2 interrupt timing relative to VD start, in units of HD \n 0xFFFF means do not update value for this interrupt

  } CSL_CcdcVdIntConfig;

/**
  \brief DPCM related configuration
*/
  typedef struct {

    Bool32    enable;             ///< TRUE: enable 10-8 bit DPCM, FALSE: disable
    Uint8   predictor;          ///< CSL_CCDC_DPCM_PREDICTOR_1 \n CSL_CCDC_DPCM_PREDICTOR_2

  } CSL_CcdcDpcmConfig;

/**
  \brief REC656 related configuration
*/
  typedef struct {

    Bool32    enable;             ///< REC656 mode, TRUE: enable, FALSE: disable
    Bool32    errorCorrectEnable; ///< Error correction, TRUE: enable, FALSE: disable
    Uint8   dataWidth;          ///< CSL_CCDC_REC656_DATA_WIDTH_8BIT \n CSL_CCDC_REC656_DATA_WIDTH_10BIT

  } CSL_CcdcRec656Config;

/**
  \brief Miscellaneous configuration
*/
  typedef struct {

    Uint8   cfaPattern;         ///< CSL_CCDC_CFA_PATTERN_MOSIAC \n CSL_CCDC_CFA_PATTERN_STRIPE

    Bool32    vdLatchDisable;     ///< TRUE: Disable register latch on VD, FALSE: Latch register on VD
    Bool32    inverseMsbCout;     ///< MSB Inverse of Cout port, TRUE: enable, FALSE: disable
    Bool32    inverseMsbCin;      ///< MSB Inverse of Cin  port, TRUE: enable, FALSE: disable

    Bool32    sdramAddrInitExtTiggerEnable; ///< TRUE: SDRAM address is initialized at the rising edge of FID input signal or DWEN signal \n FALSE: SDRAM address is initialized at VD signal
    Uint8   sdramAddrInitTiggerSource;  ///< Source signal of SDRAM address initializing, \n CSL_CCDC_SDR_ADDR_INIT_DWEN \n CSL_CCDC_SDR_ADDR_INIT_FID

    Bool32    orWenIntExt;        ///< TRUE: OR internal and external WEN signals, FALSE: AND internal and external WEN signals
    Bool32    fidLatchAtVdDisable;  ///< TRUE: Disable FID latch at VD, FALSE: Enable FID latch at VD

    Bool32    ycInSwap;           ///< TRUE: Y-signal (YIN7-0), C-signal (CIN7-0) swap, FALSE: no swap
    Bool32    ycOutSwap;          ///< TRUE: Y-signal (YOUT7-0), C-signal (COUT7-0) swap, FALSE: no swap

  } CSL_CcdcMiscConfig;

/**
  \brief DFC, LSC start offset configuration
*/
  typedef struct {

    Uint32  hOffset;            ///< CCD Horizontal data offset for DFC & LSC (Not used if Formatter is enabled)
    Uint32  vOffset;            ///< CCD Vertical data offset for DFC & LSC

  } CSL_CcdcDfcLscOffsetConfig;

/**
  \brief Vertical DFC table
*/
  typedef struct {

    Uint32  numDefects;         ///< Number of defects

    Uint32  startX[8];          ///< Defect pixel start X in pixels
    Uint32  startY[8];          ///< Defect pixel start Y in lines

    Uint32  levelEqual[8];      ///< Defect level of the Vertical line defect position (V = Vdefect)
    Uint32  levelUpper[8];      ///< Defect level of the Vertical line defect position (V < Vdefect)
    Uint32  levelLower[8];      ///< Defect level of the Vertical line defect position (V > Vdefect)

  } CSL_CcdcVdfcTable;

/**
  \brief DFC related configuration
*/
  typedef struct {

    Bool32    enable;             ///< Vertical DFC, TRUE: enable, FALSE: disable
    Uint32  mode;               ///< CSL_CCDC_VDFC_MODE_FED_TRHU_IF_SAT \n CSL_CCDC_VDFC_MODE_HORZ_INTP_IF_SAT \n CSL_CCDC_VDFC_MODE_HORZ_INTP
    Bool32    upperPixelsDisable; ///< Upper Pixel VDFC disable, TRUE: Disable, FALSE: Enable
    Uint32  levelValueShift;    ///< Level value left shitf, 0..6bits left shift
    Uint32  satLevel;           ///< Saturation Level (U12)

    CSL_CcdcVdfcTable *vdfcTable; ///< VDFC Table

  } CSL_CcdcDfcConfig;

/**
  \brief Optical Black Clamp configuration
*/
  typedef struct {

    Bool32    enable;             ///< TRUE: enable, FALSE: disable
    Uint32  mode;               ///< CSL_CCDC_CLAMP_MODE_IGNORE_COLOR \n CSL_CCDC_CLAMP_MODE_USE_COLOR
    Uint32  dcOffset;           ///< DC offset for Black Clamp (S13) \n It's added to Image Data regardless of the Clamp Enable status.

    Uint32  clampStartV;        ///< Clamp start line

    Uint32  horzClampMode;      ///< CSL_CCDC_HORZ_CLAMP_MODE_DISABLE \n CSL_CCDC_HORZ_CLAMP_MODE_ENABLE \n CSL_CCDC_HORZ_CLAMP_MODE_NOT_UPDATED
    Uint32  horzClampBlack;
    Uint32  horzClampWinStartH; ///< Window Start H, in pixels
    Uint32  horzClampWinStartV; ///< Window Start V, in lines
    Uint32  horzClampWinSizeV;  ///< Window Size, in pixels, valid values 2, 4, 8, 16
    Uint32  horzClampWinSizeH;  ///< Window Size, in pixels, valid values 32, 64, 128, 256
    Uint32  horzClampBaseWin;   ///< CSL_CCDC_HORZ_CLAMP_BASE_WIN_LEFT_MOST \n CSL_CCDC_HORZ_CLAMP_BASE_WIN_RIGHT_MOST
    Uint32  horzClampNumWin;    ///< Number of windows

    Uint32  vertClampResetValue;  ///< Reset Value (U12)
    Uint32  vertClampResetValueSource;  ///< CSL_CCDC_VERT_CLAMP_RESET_VAL_SRC_HDIR_BASE_VAL \n CSL_CCDC_VERT_CLAMP_RESET_VAL_SRC_CFG_REG \n CSL_CCDC_VERT_CLAMP_RESET_VAL_SRC_NO_UPDATE
    Uint32  vertClampLineAvgCoeff;  ///< Line average coefficient (k) (U8Q8)
    Uint32  vertClampWinStartH; ///< Window Start H, in pixels
    Uint32  vertClampWinStartV; ///< Window Start H, in lines
    Uint32  vertClampWinSizeH;  ///< Window Size H, in pixels, valid values 2, 4, 8, 16, 32, 64 \n Window Height is always 1 line
    Uint32  vertClampNumWin;    ///< Number of windows
    Uint32  vertClampBlackOptical;

  } CSL_CcdcClampConfig;

/**
  \brief LSC related configuration
*/
  typedef struct {

    Bool32    enable;             ///< TRUE: enable, FALSE: disable

    Uint32  inWidth;            ///< Input data width, in pixels
    Uint32  inHeight;           ///< Input data width, in lines

    Uint32  paxelWidth;         ///< Paxel Width, in pixels, valid values 8, 16, 32, 64, 128
    Uint32  paxelHeight;        ///< Paxel Width, in lines, valid values 8, 16, 32, 64, 128
    Uint32  gainFormat;         ///< Gain format 0..8, refer to register manual for details

    Bool32    offsetEnable;       ///< Offset control, TRUE: enable, FALSE: disable
    Uint32  offsetScale;        ///< Offset scale factor (U8Q7)
    Uint32  offsetShift;        ///< Offset left shift, 0..5 bit shift

    Uint32  initX;              ///< Init position X
    Uint32  initY;              ///< Init position Y

    Uint8  *gainTableAddr;      ///< Gain Table Address, MUST be 32byte aligned
    Uint32  gainTableLineOffset;  ///< Gain table line offset in bytes, MUST be multiple of 32bytes

    Uint8  *offsetTableAddr;    ///< Offset Table Address, MUST be 32byte aligned
    Uint32  offsetTableLineOffset;  ///< Offset table line offset in bytes, MUST be multiple of 32bytes

    Bool32    intSofEnable;       ///< Start of frame  interrupt: TRUE: enable, FALSE: disable
    Bool32    intPreFetchErrorEnable; ///< Pre-fetch error interrupt: TRUE: enable, FALSE: disable
    Bool32    intDoneEnable;      ///< LSC Complete    interrupt: TRUE: enable, FALSE: disable

  } CSL_CcdcLscConfig;

/**
  \brief  Data Formatter/Color Space Conversion Input Framing structure
*/
  typedef struct {

    Uint16  fmtsph;             ///< Start pixel horizontal from start of HD
    Uint16  fmtlnh;             ///< Number of pixels in horizontal direction
    Uint16  fmtslv;             ///< Start line from start of VD
    Uint16  fmtlnv;             ///< Number of lines in vertical direction

  } CSL_CcdcFmtCscInFrameConfig;

/**
  \brief  Data Formatter configuration
*/
  typedef struct {

    Bool32    fmtEnable;          ///< TRUE: enable, FALSE: disable
    Uint8   fmtMode;            ///< Formatter mode \n CSL_CCDC_FMT_LINE_SPLITING \n CSL_CCDC_FMT_LINE_COMBINING \n CSL_CCDC_FMT_LINE_ALTERNATING
    Uint8   numLines;           ///< Line Splitting: number of output lines per input line \n Line Combining: number of input lines per output line \n valid range (1-4)
    Uint8   addrInc;            ///< Address increment/decrement (depending on program entry update) value \n valid range (1-8)

    Uint8   progLength[4];      ///< Number of program entries in each of the 4 SETS: @n Line Splitting: Uses only SET0 & SET1: valid range (1-16) @n Line Combining: Uses all 4 sets: valid range (1-8)

    Uint16  outLineLength;      ///< Length of an output line: valid range (0-4480)

    Uint16  hCnt;               ///< HD Interval for output lines in line splitting mode: valid range (0-8192)

    Uint8   fmtAddrLine[16];    ///< Output line for data formatter address pointers 0-15 (valid range: 0-3)
    Uint16  fmtAddrInit[16];    ///< Initial address value for Data formatter address pointers 0-15 (can not exceed outLineLength)

    Uint8   valid[32];          ///< Program Entry Valid Flag: 0=>Skip, 1=>Valid
    Uint8   update[32];         ///< Address Pointer increment/decrement: CSL_CCDC_FMT_AUTO_DEC, CSL_CCDC_FMT_AUTO_INC
    Uint8   select[32];         ///< Address Pointer select: valid range (0-15) corresponding to address pointers 0-15

  } CSL_CcdcFmtConfig;

/**
  \brief Color Space conversion related configuration
*/
  typedef struct {

    Uint8   enable;             ///< TRUE: enable, FALSE: disable
    Uint8   matrix[4][4];       ///< Conversion Matrix, (U8Q5)

  } CSL_CcdcCscConfig;

/**
  \brief HW Setup Structure
*/
  typedef struct {

    Bool32    enable;             ///< TRUE: HD/VD enable, FALSE: HD/VD Disable
    Bool32    sdramOutEnable;     ///< TRUE: SDRAM Output Enable, FALSE: SDRAM Output Disable

    Bool32    bypassModuleIfNullConfig; ///< if any sub-module related parameters structure's are NULL, then if \n TRUE : Set the registers such that the module is either disabled or bypassed or set to default values \n FALSE: Do not set any sub-module related registers, i.e old values are maintained

    CSL_CcdcInDataConfig *inDataConfig; ///< Input data config
    CSL_CcdcSyncConfig *syncConfig; ///< Sensor sync config
    CSL_CcdcMiscConfig *miscConfig; ///< Misc config
    CSL_CcdcColPatConfig *colPatConfig; ///< Color Pattern config
    CSL_CcdcLinerizationConfig *linerizationConfig; ///< Linearization config
    CSL_CcdcFmtCscInFrameConfig *fmtInFrameConfig;  ///< Formatter/CSC in-frame config
    CSL_CcdcFmtConfig *fmtConfig; ///< Formatter config
    CSL_CcdcCscConfig *cscConfig; ///< Color Space conversion config
    CSL_CcdcClampConfig *clampConfig; ///< Optical Black Clamp config
    CSL_CcdcDfcLscOffsetConfig *dfcLscOffsetConfig; ///< DFC/LSC Start Offset config
    CSL_CcdcDfcConfig *dfcConfig; ///< DFC config
    CSL_CcdcLscConfig *lscConfig; ///< LSC config
    CSL_CcdcGainOffsetConfig *gainOffsetConfig; ///< Gain / Offset config
    CSL_CcdcSdramOutSizeConfig *sdramOutSizeConfig; ///< SDRAM output size config
    CSL_CcdcSdramOutConfig *sdramOutConfig; ///< SDRAM output config
    CSL_CcdcDpcmConfig *dpcmConfig; ///< DPCM config
    CSL_CcdcRec656Config *rec656Config; ///< REC656 config
    CSL_CcdcFlashConfig *flashConfig; ///< Flash timing signal config
    CSL_CcdcVdIntConfig *vdIntConfig; ///< VD interrupt timing config

  } CSL_CcdcHwSetup;

/**
  \brief Module Object Structure
*/
  typedef struct {

#ifdef CSL_KERNEL_API
    CSL_IsifRegsOvly regs;      ///< Register overlay
    Bool32    dataOutBufSwitchEnable;
    CSL_BufHndl dataOutBuf;
    CSL_BufInfo curBufInfo;

    Bool32    lscBufSwitchEnable;
    CSL_BufHndl lscBuf;
    CSL_BufInfo curLscBufInfo;
    Uint32    lscGainTableBufSize;  
    Uint16    lscHval;
    Uint16    lscVval;
    Uint32    ldcPrefecthErrCount;
    
    CSL_SysFlag intFlag[CSL_CCDC_INT_TYPE_MAX];
    int count;
#else
    int     fd;
#endif

  } CSL_CcdcObj;

  typedef CSL_CcdcObj *CSL_CcdcHandle;  ///< Module Handle

// functions

/**
  \brief Open Module Handle

  \param hndl   [ O]  Module Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcOpen(CSL_CcdcHandle hndl);

/**
  \brief Close Module Handle

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcClose(CSL_CcdcHandle hndl);

/**
  \brief Set all module registers

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcHwSetup(CSL_CcdcHandle hndl, CSL_CcdcHwSetup * data);

/**
  \brief Set all module registers to power-ON reset value

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcHwReset(CSL_CcdcHandle hndl);

/**
  \brief Enable/Disable CCDC HD/VD

  \param hndl   [I ]  Handle
  \param enable [I ]  TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcEnable(CSL_CcdcHandle hndl, Bool32 enable);

/**
  \brief Enable/Disable CCDC SDRAM output

  \param hndl   [I ]  Handle
  \param enable [I ]  TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSdramOutEnable(CSL_CcdcHandle hndl, Bool32 enable);

/**
  \brief Set input frame config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetInDataConfig(CSL_CcdcHandle hndl, CSL_CcdcInDataConfig * data);

/**
  \brief Set sensor sync config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetSyncConfig(CSL_CcdcHandle hndl, CSL_CcdcSyncConfig * data);

/**
  \brief Set miscellaneous config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetMiscConfig(CSL_CcdcHandle hndl, CSL_CcdcMiscConfig * data);

/**
  \brief Set color pattern config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetColPatConfig(CSL_CcdcHandle hndl, CSL_CcdcColPatConfig * data);

/**
  \brief Set linearization config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetLinerizationConfig(CSL_CcdcHandle hndl, CSL_CcdcLinerizationConfig * data);

/**
  \brief Set formatter/CSC input frame config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetFmtCscInFrameConfig(CSL_CcdcHandle hndl, CSL_CcdcFmtCscInFrameConfig * data);

/**
  \brief Set formatter config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetFmtConfig(CSL_CcdcHandle hndl, CSL_CcdcFmtConfig * data);

/**
  \brief Set color space conversion config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetCscConfig(CSL_CcdcHandle hndl, CSL_CcdcCscConfig * data);

/**
  \brief Set optical black clamp config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetClampConfig(CSL_CcdcHandle hndl, CSL_CcdcClampConfig * data);

/**
  \brief Set DFC/LSC start offset config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetDfcLscOffsetConfig(CSL_CcdcHandle hndl, CSL_CcdcDfcLscOffsetConfig * data);

/**
  \brief Set DFC config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetDfcConfig(CSL_CcdcHandle hndl, CSL_CcdcDfcConfig * data);

/**
  \brief Set LSC config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetLscConfig(CSL_CcdcHandle hndl, CSL_CcdcLscConfig * data);


/**
  \brief Get LSC related status

  This API gets LSC related status like prefetch error, LSC is Done.
  If status is TRUE this API clears the pending status.

  If param is set to NULL, that status is not read nor cleared.

  \param hndl          [I ] Handle
  \param intSof        [I ] Start of LSC valid region status
  \param prefetchDone  [I ] Prefetch Completed
  \param prefetchError [I ] Prefetch error
  \param lscDone       [I ] LSC is Done

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcGetLscStatus(CSL_CcdcHandle hndl, Bool32 * intSof, Bool32 * prefetchDone, Bool32 * prefetchError, Bool32 * lscDone);

/**
  \brief Set gain / offset config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetGainOffsetConfig(CSL_CcdcHandle hndl, CSL_CcdcGainOffsetConfig * data);

/**
  \brief Set SDRAM output frame size config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetSdramOutSizeConfig(CSL_CcdcHandle hndl, CSL_CcdcSdramOutSizeConfig * data);

/**
  \brief Set SDRAM output config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetSdramOutConfig(CSL_CcdcHandle hndl, CSL_CcdcSdramOutConfig * data);

/**
  \brief Get SDRAM output config

  \param hndl   [I ]  Handle
  \param data   [ O]  Output parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcGetSdramOutSizeConfig(CSL_CcdcHandle hndl, CSL_CcdcSdramOutSizeConfig * data);

/**
  \brief Set DPCM config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetDpcmConfig(CSL_CcdcHandle hndl, CSL_CcdcDpcmConfig * data);

/**
  \brief Set REC656 config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetRec656Config(CSL_CcdcHandle hndl, CSL_CcdcRec656Config * data);

/**
  \brief Set flash timing signal config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetFlashConfig(CSL_CcdcHandle hndl, CSL_CcdcFlashConfig * data);

/**
  \brief Set VD interrupt timing config

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetVdIntConfig(CSL_CcdcHandle hndl, CSL_CcdcVdIntConfig * data);

/**
  \brief Set SDRAM output address config

  \param hndl   [I ]  Handle
  \param addr   [I ]  SDRAM output address, MUST be 32-byte aligned

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcSetSdramOutAddr(CSL_CcdcHandle hndl, Uint8 * addr);


/**
  \brief Write DFC table

  \param hndl   [I ]  Handle
  \param data   [I ]  Input Parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcWriteDfcTable(CSL_CcdcHandle hndl, CSL_CcdcVdfcTable * data);

/**
  \brief Read DFC table

  \param hndl   [I ]  Handle
  \param data   [ O]  Output Parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcReadDfcTable(CSL_CcdcHandle hndl, CSL_CcdcVdfcTable * data);

/**
  \brief Buffer init

  \param hndl     [I ] hndl
  \param bufInit  [I ] Buffer init parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcBufInit(CSL_CcdcHandle hndl, CSL_BufInit * bufInit);

/**
  \brief Buffer switch in ISR enable/disable

  \param hndl     [I ] hndl
  \param enable   [I ] TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcBufSwitchEnable(CSL_CcdcHandle hndl, Bool32 enable);

/**
  \brief Get Full Buffer

  \param hndl     [I ] hndl
  \param buf      [ O] Full buffer info
  \param minBuf   [I ] minimum number of buffers that should be full
  \param timeout  [I ] Timeout

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcBufGetFull(CSL_CcdcHandle hndl, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout);

/**
  \brief Put Empty Buffer

  \param hndl     [I ] hndl
  \param buf      [I ] Empty buffer info

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcBufPutEmpty(CSL_CcdcHandle hndl, CSL_BufInfo * buf);

/**
  \brief Enable/Disable module interrupt

  \param hndl     [I ] hndl
  \param intType  [I ] interrupt type
  \param enable   [I ] TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcIntEnable(CSL_CcdcHandle hndl, Uint8 intType, Bool32 enable);

/**
  \brief Clear module interrupt

  \param hndl     [I ] hndl
  \param intType  [I ] interrupt type

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcIntClear(CSL_CcdcHandle hndl, Uint8 intType);

/**
  \brief Wait for module interrupt

  \param hndl         [I ] hndl
  \param intType      [I ] interrupt type
  \param numIntWait   [I ] Number of interrupts to wait

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcIntWait(CSL_CcdcHandle hndl, Uint8 intType, Uint32 numIntWait);


/**
  \brief Module lock : Mutex Lock

  \param hndl     [I ] hndl
  \param timeout  [I ] CSL_SYS_TIMEOUT_NONE or CSL_SYS_TIMEOUT_FOREVER

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcLock(CSL_CcdcHandle hndl, Uint32 timeout);

/**
  \brief Module unlock : Mutex Unlock

  \param hndl     [I ] hndl

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_ccdcUnlock(CSL_CcdcHandle hndl);

  CSL_Status CSL_ccdcLscBufInit(CSL_CcdcHandle hndl, CSL_BufInit * bufInit);
  CSL_Status CSL_ccdcLscBufSwitchEnable(CSL_CcdcHandle hndl, Bool32 enable);
  CSL_Status CSL_ccdcLscBufGetEmpty(CSL_CcdcHandle hndl, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout);
  CSL_Status CSL_ccdcLscBufPutFull(CSL_CcdcHandle hndl, CSL_BufInfo * buf);
  CSL_Status CSL_ccdcLscSetGainTableAddr(CSL_CcdcHandle hndl, Uint8 *inAddr);
  CSL_Status CSL_ccdcLscSetOffsetTableAddr(CSL_CcdcHandle hndl, Uint8 *inAddr);  
  CSL_Status CSL_ccdcLscEnable(CSL_CcdcHandle hndl, Bool32 enable);
  CSL_Status CSL_ccdcLscBufSetGainTableSize(CSL_CcdcHandle hndl, Uint32 gainTableSize);

#ifdef __cplusplus
}
#endif
/*@}*/
#endif
