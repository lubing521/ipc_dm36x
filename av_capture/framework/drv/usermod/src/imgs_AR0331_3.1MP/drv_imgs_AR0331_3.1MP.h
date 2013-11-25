
#ifndef _DRV_IMGS_AR0331_3MP_H_
#define _DRV_IMGS_AR0331_3MP_H_

#include <drv_imgs.h>
#include <drv_i2c.h>

#define IMGS_ROW_START_MIN  (40)
#define IMGS_COL_START_MIN  (0)

#define IMGS_H_PAD	(20)
#define IMGS_V_PAD	(0)		//56

#define IMGS_W_MAX    (2048)
#define IMGS_H_MAX     (1536)
/*
#define IMGS_ROW_START_MIN  (40)
#define IMGS_COL_START_MIN  (0)

#define IMGS_H_PAD          (0)
#define IMGS_V_PAD          (4)

#define IMGS_W_MAX     (2592)
#define IMGS_H_MAX     (1944)
*/
#define IMGS_MHZ        (1000000L)

#define IMGS_PLL_REF_CLK     (24*IMGS_MHZ)

#define CHIP_VERSION_ADDR   (0x3000)
#define IMGS_CHIP_ID        (0x2602)

#define AR0331_GLOABL_GAIN		               0x305E
#define AR0331_COARSE_IT_TIME_A	        0x3012
#define AR0331_ANALOG_GAIN                         0x3060
#define AR0331_VT_PIX_CLK_DIV                     0x302A
#define AR0331_VT_SYS_CLK_DIV                    0x302C
#define AR0331_PLL_Multiplier                            0x3030
#define AR0331_Pre_PLL_CLK_DIV                    0x302E
/*
#define CHIP_VERSION        (0x00)
#define ROW_START           (0x01)
#define COL_START           (0x02)
#define ROW_SIZE            (0x03)
#define COL_SIZE            (0x04)
#define HORZ_BLANK          (0x05)
#define VERT_BLANK          (0x06)
#define OUTPUT_CTRL         (0x07)
#define SHUTTER_WIDTH_H     (0x08)
#define SHUTTER_WIDTH_L     (0x09)
#define PIXEL_CLK_CTRL      (0x0a)
#define RESTART             (0x0b)
#define SHUTTER_DELAY       (0x0c)
#define RESET               (0x0d)
#define PLL_CTRL            (0x10)
#define PLL_CFG1            (0x11)
#define PLL_CFG2            (0x12)
#define READ_MODE_1         (0x1e)
#define READ_MODE_2         (0x20)
#define ROW_ADDR_MODE       (0x22)
#define COL_ADDR_MODE       (0x23)
#define GAIN_G1             (0x2b)
#define GAIN_B              (0x2c)
#define GAIN_R              (0x2d)
#define GAIN_G2             (0x2e)
#define GAIN_GLOBAL         (0x35)
#define ROW_BLK_DEF_OFFSET  (0x4B)
*/
#ifdef BOARD_AP_IPNC
#define IMGS_I2C_ADDR       (0x20)
#endif

#ifdef BOARD_TI_EVM
#define IMGS_I2C_ADDR       (0xBA)
#endif

#define IMGS_RESET_GPIO     (36)

typedef struct {

  int fps;        // Hz
  float t_frame;  // ns
  float t_row;    // ns
  float t_pclk;   // ns
  int W;
  int H;
  int SW;
  int SD;
  int HB;
  int VB;
  int HBmin;
  int VBmin;
  int f_pclk;     // Hz

  float shutterOverhead;

  int col_size;
  int row_size;
  int col_skip;
  int row_skip;
  int col_bin;
  int row_bin;
  int col_start;
  int row_start;

  int pll_M;
  int pll_N;
  int pll_DIV;

} DRV_ImgsFrameTime;

typedef struct {

  DRV_I2cHndl i2cHndl;

  DRV_ImgsFrameTime curFrameTime;
  DRV_ImgsModeConfig curModeConfig;

} DRV_ImgsObj;


extern DRV_ImgsObj gDRV_imgsObj;

extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_0;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_1;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_0_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_1_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_0;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_1;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_2;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_0_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_1_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_2_VS;

// for TNF+SNF cases
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_0;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_1;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_0_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_1_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_0;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_1;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_2;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_0_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_1_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_2_VS;

extern DRV_ImgsIsifConfig   gDRV_imgsIsifConfig_Common;

extern DRV_ImgsIpipeConfig  gDRV_imgsIpipeConfig_Vnfdemo;
extern DRV_ImgsIpipeConfig  gDRV_imgsIpipeConfig_Common;

extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_Appro;
extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_TI;
extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_Common;

int DRV_imgsReset();

int DRV_imgsCalcFrameTime(Uint32 fps, Uint32 dataW, Uint32 dataH, Bool binEnable);
int DRV_imgsCalcSW(int exposureTimeInUsecs);
int DRV_imgsCalcAgain(int aGain);
int DRV_imgsCalcFrameRate(int fps);
int DRV_imgsGetEshutter(Uint32 *eshutterInUsec);
int DRV_imgsGetAgain(int *again);
int AR0331_HDR_Enable(int enable);
int DRV_imgsCheckId();
int DRV_imgsSetRegs();
#ifdef AR0331_WDR
int DRV_ImgsGetMean(unsigned int *highMean, unsigned int *lowMean);
int DRV_ImgsGetWDRSetting(unsigned int *wdrMode, unsigned int *ratioT1T2);
int DRV_ImgsSetWDRSetting(unsigned int wdrMode, unsigned int ratioT1T2);
#endif
#endif

