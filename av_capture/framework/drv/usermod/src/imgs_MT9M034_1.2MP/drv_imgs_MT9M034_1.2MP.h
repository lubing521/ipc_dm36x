
#ifndef _DRV_IMGS_MT9P031_5MP_H_
#define _DRV_IMGS_MT9P031_5MP_H_

#include <drv_imgs.h>
#include <drv_i2c.h>

#define IMGS_ROW_START_MIN  (40)
#define IMGS_COL_START_MIN  (0)

#define IMGS_H_PAD          (0)
#define IMGS_V_PAD          (4)

#define IMGS_W_MAX     (2592)
#define IMGS_H_MAX     (1944)

#define IMGS_MHZ        (1000000L)

#define IMGS_PLL_REF_CLK     (24*IMGS_MHZ)

#define IMGS_CHIP_ID        (0x2400)//(0x1801)

/* mt9m034 register addresses */
#define MT9M034_CHIP_VERSION		0x3000
#define MT9M034_ROW_START_A		0x3002
#define MT9M034_COLUMN_START_A	0x3004
#define MT9M034_ROW_END_A			0x3006
#define MT9M034_COLUMN_END_A		0x3008
#define MT9M034_FRAME_LINES_A		0x300A
#define MT9M034_LINE_LENGTH		0x300C
#define MT9M034_COARSE_IT_TIME_A	0x3012
#define MT9M034_FINE_IT_TIME_A	0x3014
#define MT9M034_COARSE_IT_TIME_B	0x3016
#define MT9M034_FINE_IT_TIME_B	0x3018
#define MT9M034_ENABLE_STREAMING	0x301a
#define MT9M034_ROW_SPEED			0x3028
#define MT9M034_VT_PIX_CLK_DIV	0x302A
#define MT9M034_VT_SYS_CLK_DIV	0x302C
#define MT9M034_PRE_PLL_CLK_DIV	0x302C
#define MT9M034_PLL_MULTIPLIER	0x3030
#define MT9M034_SCALING_MODE		0x3032
#define MT9M034_READ_MODE			0x3040
#define MT9M034_DARK_CONTROL		0x3044
#define MT9M034_GLOABL_GAIN		0x305E
#define MT9M034_EMBEDDED_DATA		0x3064
#define MT9M034_OP_MODE_A			0x3082
#define MT9M034_OP_MODE_B			0x3084
#define MT9M034_COLUMN_START_B	0x308A
#define MT9M034_ROW_START_B		0x308C
#define MT9M034_COLUMN_END_B		0x308E
#define MT9M034_ROW_END_B			0x3090
#define MT9M034_Y_ODD_INC_A		0x30A6
#define MT9M034_Y_ODD_INC_B		0x30A8
#define MT9M034_FRAME_LINES_B		0x30AA
#define MT9M034_DIGITAL_TEST		0x30B0
#define MT9M034_DAC_LD_24_25    0x3EE4
#define MT9M034_AE_CTRL_REG		0x3100

#ifdef BOARD_AP_IPNC
#define IMGS_I2C_ADDR       (0x20)//(0x30)//(0x90)
#endif

#ifdef BOARD_TI_EVM
#define IMGS_I2C_ADDR       (0xBA)
#endif

#define IMGS_RESET_GPIO     (57)//(50)//36 //(91)

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
int mt9m034_enable_hdr_mode(int enable, int compand_mode);

int DRV_imgsCheckId();
int DRV_imgsSetRegs();

#endif

