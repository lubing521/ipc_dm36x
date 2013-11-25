
#ifndef _DRV_IMGS_OV9715_1MP_H_
#define _DRV_IMGS_OV9715_1MP_H_

#include <drv_imgs.h>
#include <drv_spi.h>	

#define IMGS_ROW_START_MIN  (40)
#define IMGS_COL_START_MIN  (0)

#define IMGS_H_PAD          (0x98*2)
#define IMGS_V_PAD          (0x1F)

#define IMGS_W_MAX     (2592)
#define IMGS_H_MAX     (1944)

#define IMGS_MHZ        (1000000L)

#define IMGS_PLL_REF_CLK     (24*IMGS_MHZ)

#define IMGS_CHIP_ID        (0x1801)

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


#define IMGS_SPI_ADDR       (0x81)


#define IMGS_RESET_GPIO	(36)	//for H/W v2.0 (30)
#if (1)
#define IMGS_LIGHT_SENSOR			(45)
#define IMGS_LIGHT_SENSOR_LINK	(48)
#define IMGS_ND_CTRL1		(46)
#define IMGS_ND_CTRL2		(49)
#else
#define IMGS_ND_CTRL1		(35)
#define IMGS_ND_CTRL2		(82)
#endif

#define IMGS_SENSOR_LINEWIDTH		(1082)

#define RegBaseVirtStart	(0x01C00000)

#define PINMUX_BASE		(0x01C40000)
#define PINMUX_LENGTH	(0x8C)
#define PINMUX0_OFSET	(0x0000)
#define PINMUX1_OFSET	(0x0004)
#define PINMUX2_OFSET	(0x0008)
#define PINMUX3_OFSET	(0x000C)
#define PINMUX4_OFSET	(0x0010)

#define IMGS_APCHIP_C1_GIO	(93)
#define AP_CHIP_C1_PINMUX	(PINMUX_BASE+PINMUX0_OFSET)
#define AP_CHIP_C1_MASK	(((unsigned int)0x3)<<14)
#define AP_CHIP_C1_VAL 		(((unsigned int)0x1)<<14)

#define IMGS_APCHIP_D1_GIO	(90)
#define AP_CHIP_D1_PINMUX	(PINMUX_BASE+PINMUX1_OFSET)
#define AP_CHIP_D1_MASK	(((unsigned int)0x3)<<4)
#define AP_CHIP_D1_VAL 		(((unsigned int)0x0)<<4)


#define _PWM_BASE_			(0x1C22000)
#define _PWM0_BASE_		(_PWM_BASE_)
#define _PWM1_BASE_		(_PWM_BASE_+0x400)
#define _PWM2_BASE_		(_PWM_BASE_+0x800)
#define _PWM3_BASE_		(_PWM_BASE_+0xC00)

#define _PWM_PCR_			(0x04)
#define _PWM_START_		(0x0C)
#define _PWM_CFG_			(0x08)
#define _PWM_PER_			(0x14)
#define _PWM_PH1D_			(0x18)

/* PINMUX1 GIO92 -> PWM0 for Video */
#define IMGS_AutoIris_VIDEO_PWM_REG 	(_PWM0_BASE_)
#define IMGS_AutoIris_VIDEO_GIO			(92)
#define AP_AutoIris_VIDEO_PINMUX		(PINMUX_BASE+PINMUX1_OFSET)
#define AP_AutoIris_VIDEO_MASK			(((unsigned int)0x3)<<0)
#define AP_AutoIris_VIDEO_VAL 			(((unsigned int)0x2)<<0)
/* PINMUX1 GIO91 -> PWM1 for DC */
#define IMGS_AutoIris_DC_PWM_REG 		(_PWM1_BASE_)
#define IMGS_AutoIris_DC_GIO			(91)
#define AP_AutoIris_DC_PINMUX			(PINMUX_BASE+PINMUX1_OFSET)
#define AP_AutoIris_DC_MASK				(((unsigned int)0x3)<<2)
#define AP_AutoIris_DC_VAL 				(((unsigned int)0x2)<<2)

typedef struct {

  int fps;        // Hz
  int Is50Hz;
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
  int pll_PREDIV;

} DRV_ImgsFrameTime;

typedef struct {

  DRV_SPIHndl spiHndl;	//DRV_I2cHndl i2cHndl;

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

int DRV_imgsCheckId();
int DRV_imgsSetRegs();
int DRV_imgs_APChipInit(int *pGioC1, int *pGioD1);

#endif

