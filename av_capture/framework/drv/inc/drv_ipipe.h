
#ifndef _DRV_IPIPE_H_
#define _DRV_IPIPE_H_

#include <drv_imgs.h>
#include <osa_buf.h>

#define DRV_IPIPE_BSC_BUFFER_MAX    (0x4000)
#define DRV_IPIPE_HISTO_BUFFER_MAX  (0x2000)

#define DRV_IPIPE_RSZ_A   (CSL_RSZ_A)
#define DRV_IPIPE_RSZ_B   (CSL_RSZ_B)

#define DRV_IPIPE_INPUT_SRC_ISIF    (0)
#define DRV_IPIPE_INPUT_SRC_DDR     (1)

#define DRV_IPIPE_HISTO_MODE_1_REGION    (0)
#define DRV_IPIPE_HISTO_MODE_2_REGION    (1)
#define DRV_IPIPE_HISTO_MODE_2x2_REGION  (2)
#define DRV_IPIPE_HISTO_MODE_NONE                   (0xFFFF) // histogram not needed

#define DRV_IPIPE_BOXCAR_BLOCK_SIZE_8x8            (CSL_IPIPE_BOXCAR_BLOCK_SIZE_8_8)
#define DRV_IPIPE_BOXCAR_BLOCK_SIZE_16x16          (CSL_IPIPE_BOXCAR_BLOCK_SIZE_16_16)
#define DRV_IPIPE_BOXCAR_BLOCK_SIZE_NONE           (0xFFFF)  // boxcar not needed

typedef struct {

  Uint16 gainR;
  Uint16 gainGr;
  Uint16 gainGb;
  Uint16 gainB;

} DRV_IpipeWb;

typedef struct {

  Bool   enable;
  Uint16 outFormat;
  Uint16 width;
  Uint16 height;
  Uint16 offsetH;
  Uint16 offsetV;

} DRV_IpipeRszInfo;

typedef struct {

  Uint16 width; // width in units of blocks, 1block = 4*2bytes
  Uint16 height;
  Uint16 offsetH; // width*4

} DRV_IpipeBoxcarInfo;

typedef struct {

  DRV_IpipeRszInfo         rszInfo[CSL_RSZ_CH_MAX];
  CSL_IpipeBscConfig       bscInfo;
  CSL_IpipeHistogramConfig histoInfo;
  DRV_IpipeBoxcarInfo      boxcarInfo;

} DRV_IpipeInfo;

typedef struct {

  Bool   enable;
  Uint16 outFormat;
  Uint16 width;
  Uint16 height;
  Uint16 flipH;
  Uint16 flipV;
  Uint16 numBuf;

} DRV_IpipeRszConfig;

typedef struct {

  int sensorMode;
  int vnfDemoCfg;
  int inputSrc;
  DRV_IpipeRszConfig rszOutConfig[CSL_RSZ_CH_MAX];

  Uint16 boxcarBlockSize;   // DRV_IPIPE_BOXCAR_BLOCK_SIZE_xxx
  Uint16 boxcarInShift;     // input shift for boxcar 0..4
  Uint16 histogramMode;     // DRV_IPIPE_HISTO_MODE_xxx,
  Uint16 bscNumVectorsRow;
  Uint16 bscNumVectorsCol;
  Uint16 rszValidDataStartOffset;

} DRV_IpipeConfig;

typedef struct {

  Bool isLeftPanDone;
  Bool isRightPanDone;
  Bool isUpTiltDone;
  Bool isDownTiltDone;

  Uint16 totalInWidth;
  Uint16 totalInHeight;
  Uint16 curInStartX;
  Uint16 curInStartY;
  Uint16 curInWidth;
  Uint16 curInHeight;

} DRV_IpipeDigitalPtzStatus;

typedef struct {

  Uint16 enableRtChange; //set to TRUE if the Runtime parameters change required for this resizer, else FALSE
  Uint16 outWidth;
  Uint16 outHeight;
  
} DRV_ipipeReszOutRtPrm;

typedef struct {

  Uint16 inStartX;
  Uint16 inStartY;
  Uint16 inWidth;
  Uint16 inHeight;

  DRV_ipipeReszOutRtPrm pOut[CSL_RSZ_CH_MAX];
 
} DRV_IpipeReszRtPrm;

typedef struct {

  Uint32 bsc_row_vct;
  Uint32 bsc_row_vpos;
  Uint32 bsc_row_vnum;
  Uint32 bsc_row_vskip;
  Uint32 bsc_row_hpos;
  Uint32 bsc_row_hnum;
  Uint32 bsc_row_hskip;
  Uint32 bsc_row_shf;

  Uint32 bsc_col_vct;
  Uint32 bsc_col_vpos;
  Uint32 bsc_col_vnum;
  Uint32 bsc_col_vskip;
  Uint32 bsc_col_hpos;
  Uint32 bsc_col_hnum;
  Uint32 bsc_col_hskip;
  Uint32 bsc_col_shf;

} DRV_IpipeBscConfig;

int DRV_ipipeOpen(DRV_IpipeConfig *config);
int DRV_ipipeClose();

int DRV_ipipeEnable(Bool rszOutEnable, Bool bscEnable, Bool histoEnable, Bool boxcarEnable);

int DRV_ipipeGetInfo(DRV_IpipeInfo *info);

// dzoom, 100 = 1.00x, 110 = 1.10x .. 400 = 4.00x
// pan  -8 .. +8
// tilt -8 .. +8
int DRV_ipipeSetDigitalPtz(Uint16 dzoom, Int16 pan, Int16 tilt, DRV_IpipeDigitalPtzStatus *status);
int DRV_ipipeEnableFlip(Bool flipH, Bool flipV);
int DRV_ipipeSetParams(void);

int DRV_ipipeGetRszBuf(int rszId, int *bufId, int timeout);
int DRV_ipipePutRszBuf(int rszId, int bufId);
OSA_BufInfo *DRV_ipipeGetRszBufInfo(int rszId, int bufId);

int DRV_ipipeGetBscBuf(int *bufId, int timeout);
int DRV_ipipePutBscBuf(int bufId);
OSA_BufInfo *DRV_ipipeGetBscBufInfo(int bufId);

int DRV_ipipeGetHistoBuf(int *bufId, int timeout);
int DRV_ipipePutHistoBuf(int bufId);
OSA_BufInfo *DRV_ipipeGetHistoBufInfo(int bufId);

int DRV_ipipeGetBoxcarBuf(int *bufId, int timeout);
int DRV_ipipePutBoxcarBuf(int bufId);
OSA_BufInfo *DRV_ipipeGetBoxcarBufInfo(int bufId);

int DRV_ipipeSetRgb2Rgb(CSL_IpipeRgb2RgbConfig *rgb2rgb);
int DRV_ipipeSetRgb2Rgb2(CSL_IpipeRgb2RgbConfig *rgb2rgb);

int DRV_ipipeSetYoffet(int Yoffset);
int DRV_ipipeSetRgb2Yuv(CSL_IpipeRgb2YuvConfig *config);
int DRV_ipipeBscSetParams(void);
int DRV_ipipeBscUpdateParams(DRV_IpipeBscConfig *pBscConfig);

int DRV_ipipeSetWb(DRV_IpipeWb *wbData);

int DRV_ipipeSetContrastBrightness(Uint16 contrast, Uint16 brightness);

int DRV_ipipeSetEdgeEnhance(CSL_IpipeEdgeEnhanceConfig *config);

int DRV_ipipeSetNf2(CSL_IpipeNfConfig *config);
int DRV_ipipeSetDpcConfig(CSL_IpipeDpcConfig *config);

int DRV_ipipeSetReszRtPrms(DRV_IpipeReszRtPrm *rszPrm);

int DRV_ipipeGetHistogram (void);
int DRV_ipipeCpyHistData(Uint32* histData, Uint8* histStatus);

int DRV_ipipeLock();
int DRV_ipipeUnlock();


#endif

