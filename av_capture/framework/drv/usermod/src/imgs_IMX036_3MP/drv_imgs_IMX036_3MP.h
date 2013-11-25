
#ifndef _DRV_IMGS_IMX036_3MP_H_
#define _DRV_IMGS_IMX036_3MP_H_

#include <drv_imgs.h>
#include <drv_spi.h>	

#define IMGS_ROW_START_MIN  (40)
#define IMGS_COL_START_MIN  (0)

#define IMGS_H_PAD          (104)	//(72)
#define IMGS_V_PAD          (22)

#define IMGS_H_3M_PAD	(104)
#define IMGS_V_3M_PAD	(52)		//56

#define IMGS_W_MAX    (2200)
#define IMGS_H_MAX     (1608)

#define IMGS_MHZ        (1000000L)

#define IMGS_PLL_REF_CLK     (24*IMGS_MHZ)

#define IMGS_CHIP_PID        (0x9711)
#define IMGS_CHIP_MID        (0x7FA2)

#define	GAIN			(0x00)
#define	BLUE			(0x01)
#define	RED				(0x02)
#define	REG03			(0x03)
#define	REG04			(0x04)
#define	GREEN			(0x05)
#define	BRL				(0x06)
#define	GL				(0x07)
#define	REG08			(0x08)
#define	COM2			(0x09)
#define	PIDH			(0x0a)
#define	PIDL			(0x0b)
#define	COM3			(0x0c)
#define	COM4			(0x0d)
#define	COM5			(0x0e)
#define	COM6			(0x0f)
#define	AEC				(0x10)
#define	CLK				(0x11)
#define	COM7			(0x12)
#define	COM8			(0x13)
#define	COM9			(0x14)
#define	COM10			(0x15)
#define	AECH			(0x16)
#define	HSTART			(0x17)
#define	AHSIZE			(0x18)
#define	VSTART			(0x19)
#define	AVSIZE			(0x1a)
#define	PSHIFT			(0x1b)
#define	MIDH			(0x1c)
#define	MIDL			(0x1d)
#define	COM11			(0x1e)
#define	REG1F			(0x1f)
#define	COM12			(0x20)
#define	COM13			(0x21)
#define	COM14			(0x22)
#define	COM15			(0x23)
#define	WPT				(0x24)
#define	BPT				(0x25)
#define	VPT				(0x26)
#define	COM16			(0x27)
#define	COM17			(0x28)
#define	COM18			(0x29)
#define	REG2A			(0x2a)
#define	REG2B			(0x2b)
#define	COM19			(0x2c)
#define	REG2D			(0x2d)
#define	REG2E			(0x2e)
#define	YAVG			(0x2f)
#define	REG30			(0x30)
#define	REG31			(0x31)
#define	REG32			(0x32)
#define	COM24			(0x37)
#define	COM25			(0x38)
#define	COM28			(0x3c)
#define	RENDL			(0x3d)
#define	RENDH			(0x3e)
#define	REG41			(0x41)
#define	REG42			(0x42)
#define	REG43			(0x43)
#define	ROFFS			(0x44)
#define	BOFFS			(0x45)
#define	GROFFS			(0x46)
#define	GBOFFS			(0x47)
#define	HOFFS			(0x48)
#define	REG49			(0x49)
#define	REG4A			(0x4a)
#define	REG4E			(0x4e)
#define	REG4F			(0x4f)
#define	REG50			(0x50)
#define	REG51			(0x51)
#define	REG55			(0x55)
#define	REG56			(0x56)
#define	REG57			(0x57)
#define	REG58			(0x58)
#define	REG59			(0x59)
#define	REG5C			(0x5c)
#define	REG5D			(0x5d)
#define	REG72			(0x72)
#define	REG73			(0x73)
#define	REG74			(0x74)
#define	REG75			(0x75)
#define	REG76			(0x76)
#define	REG77			(0x77)
#define	REG78			(0x78)
#define	REG79			(0x79)
#define	DSP_CTRL_0		(0x96)
#define	DSP_CTRL_1		(0x97)
#define	DSP_CTRL_2		(0x98)
#define	DSP_CTRL_3		(0x99)
#define	DSP_CTRL_4		(0x9a)
#define	DSP_CTRL_5		(0x9b)
#define	DSP_CTRL_6		(0x9c)
#define	DSP_CTRL_7		(0x9d)
#define	LENC_CTRL_0		(0x9e)
#define	LENC_CTRL_1		(0x9f)
#define	LENC_CTRL_2		(0xa0)
#define	LENC_CTRL_3		(0xa1)
#define	LENC_CTRL_4		(0xa2)
#define	LENC_CTRL_5		(0xa3)
#define	LENC_CTRL_6		(0xa4)
#define	LENC_CTRL_7		(0xa5)
#define	LENC_CTRL_8		(0xa6)
#define	LENC_CTRL_9		(0xa7)
#define	LENC_CTRL_10	(0xa8)
#define	LENC_CTRL_11	(0xa9)
#define	LENC_CTRL_12	(0xaa)
#define	LENC_CTRL_13	(0xab)
#define	LENC_CTRL_14	(0xac)
#define	LENC_CTRL_15	(0xad)
#define	LENC_CTRL_16	(0xae)
#define	LENC_CTRL_17	(0xaf)
#define	LENC_CTRL_18	(0xb0)
#define	LENC_CTRL_19	(0xb1)
#define	LENC_CTRL_20	(0xb2)
#define	LENC_CTRL_21	(0xb3)
#define	LENC_CTRL_22	(0xb4)
#define	LENC_CTRL_23	(0xb5)
#define	AWB_CTRL_0		(0xb6)
#define	AWB_CTRL_1		(0xb7)
#define	AWB_CTRL_2		(0xb8)
#define	AWB_CTRL_3		(0xb9)
#define	AWB_CTRL_4		(0xba)
#define	AWB_CTRL_5		(0xbb)
#define	WBC_CTRL_0		(0xbc)
#define	YAVG_CTRL_0		(0xbd)
#define	YAVG_CTRL_1		(0xbe)
#define	YAVG_CTRL_2		(0xbf)
#define	YAVG_CTRL_3		(0xc0)
#define	YAVG_CTRL_4		(0xc1)
#define	DVP_CTRL_00		(0xc2)
#define	DVP_CTRL_01		(0xc3)
#define	DVP_CTRL_02		(0xc4)
#define	DVP_CTRL_03		(0xc5)
#define	DVP_CTRL_04		(0xc6)
#define	DVP_CTRL_05		(0xc7)
#define	DVP_CTRL_06		(0xc8)
#define	DVP_CTRL_07		(0xc9)
#define	DVP_CTRL_08		(0xca)
#define	DVP_CTRL_09		(0xcb)
#define	DVP_CTRL_0a		(0xcc)
#define	DVP_CTRL_0b		(0xcd)
#define	DVP_CTRL_0c		(0xce)
#define	DVP_CTRL_0d		(0xcf)
#define	DVP_CTRL_0e		(0xd0)
#define	DVP_CTRL_0f		(0xd1)
#define	DVP_CTRL_10		(0xd2)
#define	DVP_CTRL_11		(0xd3)
#define	DVP_CTRL_12		(0xd4)
#define	SC_CTRL_0		(0xd6)

#define REG56_VSYNC_MASK	(0x08)
#define REG56_VSYNC_SHIFT	(3)
#define REG56_VSYNC_INPUT	(0)
#define REG56_VSYNC_OUTPUT	(1)

#define REG56_PCLK_MASK		(0x04)
#define REG56_PCLK_SHIFT	(2)
#define REG56_PCLK_INPUT	(0)
#define REG56_PCLK_OUTPUT	(1)

#define REG56_HREF_MASK		(0x10)
#define REG56_HREF_SHIFT	(4)
#define REG56_HREF_INPUT	(0)
#define REG56_HREF_OUTPUT	(1)

#ifdef BOARD_AP_IPNC
#define IMGS_I2C_ADDR       (0x60)
#endif

#define IMGS_RESET_GPIO	(36)	

#define IMGS_SPI_ADDR		(0x02)
#define IMGS_SPI_ADDR01	(0x03)
#define IMGS_LIGHT_SENSOR			(45)
#define IMGS_LIGHT_SENSOR_LINK	(45)
#define IMGS_ND_CTRL1		(46)
#define IMGS_ND_CTRL2		(49)
#define IMGS_SENSOR_LINEWIDTH		(1125)
#define IMGS_SENSOR_LINEWIDTH_3MP	(1608)


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
int DRV_imgsSetRegs(Uint16 width, Uint16 height, int fps);
int DRV_imgs_APChipInit(int *pGioC1, int *pGioD1);

#endif

