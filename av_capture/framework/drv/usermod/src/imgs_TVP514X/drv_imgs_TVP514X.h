
#ifndef _DRV_IMGS_TVP514X_H_
#define _DRV_IMGS_TVP514X_H_

#include <drv_imgs.h>
#include <drv_i2c.h>

#define IMGS_ROW_START_MIN  (0)
#define IMGS_COL_START_MIN  (0)

#define IMGS_H_PAD          (0)
#define IMGS_V_PAD          (0)

#define IMGS_W_MAX     (720)
#define IMGS_H_MAX     (480)

#define IMGS_MHZ        (1000000L)

#define IMGS_PLL_REF_CLK     (24*IMGS_MHZ)

enum tvp514x_mode {
        TVP514X_MODE_INV = -1,
        TVP514X_MODE_AUTO = 0,  /* autoswitch mode (default)   */
        TVP514X_MODE_NTSC = 1,  /* (M, J) NTSC      525-line   */
        TVP514X_MODE_PAL = 2,   /* (B, D, G, H, I, N) PAL      */
        TVP514X_MODE_PAL_M = 3, /* (M) PAL          525-line   */
        TVP514X_MODE_PAL_CN = 4,        /* (Combination-N) PAL         */
        TVP514X_MODE_NTSC_443 = 5,      /* NTSC 4.43        525-line   */
        TVP514X_MODE_SECAM = 6, /* SECAM                       */
        TVP514X_MODE_PAL_60 = 7,        /* PAL 60          525-line    */
        TVP514X_MODE_AUTO_SQP = 8,      /* autoswitch mode (default)   */
        TVP514X_MODE_NTSC_SQP = 9,      /* (M, J) NTSC      525-line   */
        TVP514X_MODE_PAL_SQP = 0xA,     /* (B, D, G, H, I, N) PAL      */
        TVP514X_MODE_PAL_M_SQP = 0xB,   /* (M) PAL          525-line   */
        TVP514X_MODE_PAL_CN_SQP = 0xC,  /* (Combination-N) PAL         */
        TVP514X_MODE_NTSC_443_SQP = 0xD,/* NTSC 4.43 525-line          */
        TVP514X_MODE_SECAM_SQP = 0xE,   /* SECAM                       */
        TVP514X_MODE_PAL_60_SQP = 0xF,  /* PAL 60          525-line    */
};

#define TVP514X_INPUT_SEL                                       (0x00)
#define TVP514X_AFE_GAIN_CTRL                                   (0x01)
#define TVP514X_VIDEO_STD                                       (0x02)
#define TVP514X_OPERATION_MODE                                  (0x03)
#define TVP514X_AUTOSWT_MASK                                    (0x04)
#define TVP514X_COLOR_KILLER                                    (0x05)
#define TVP514X_LUMA_CONTROL1                                   (0x06)
#define TVP514X_LUMA_CONTROL2                                   (0x07)
#define TVP514X_LUMA_CONTROL3                                   (0x08)
#define TVP514X_BRIGHTNESS                                      (0x09)
#define TVP514X_CONTRAST                                        (0x0A)
#define TVP514X_SATURATION                                      (0x0B)
#define TVP514X_HUE                                             (0x0C)
#define TVP514X_CHROMA_CONTROL1                                 (0x0D)
#define TVP514X_CHROMA_CONTROL2                                 (0x0E)
#define TVP514X_OUTPUT1                                         (0x33)
#define TVP514X_OUTPUT2                                         (0x34)
#define TVP514X_OUTPUT3                                         (0x35)
#define TVP514X_OUTPUT4                                         (0x36)
#define TVP514X_OUTPUT5                                         (0x37)
#define TVP514X_OUTPUT6                                         (0x38)
#define TVP514X_CLEAR_LOST_LOCK                                 (0x39)
#define TVP514X_STATUS1                                         (0x3A)
#define TVP514X_VID_STD_STATUS                                  (0x3F)
#define TVP514X_FIFO_OUTPUT_CTRL                                (0xC0)

/* masks */

#define TVP514X_LOST_LOCK_MASK                                  (0x10)
/* mask to enable autoswitch for all standards*/

#define TVP514X_AUTOSWITCH_MASK                                 (0x7F)
#define TVP514X_COMPOSITE_INPUT                                 (0x05)
#define TVP514X_SVIDEO_INPUT                                    (0x46)
/* DEFAULTS */

#define TVP514X_OPERATION_MODE_RESET                            (0x1)
#define TVP514X_OPERATION_MODE_DEFAULT                          (0x0)
#define TVP514X_AFE_GAIN_CTRL_DEFAULT                           (0x0F)
#define TVP514X_COLOR_KILLER_DEFAULT                            (0x10)
#define TVP514X_LUMA_CONTROL1_DEFAULT                           (0x10)
#define TVP514X_LUMA_CONTROL2_DEFAULT                           (0x00)
#define TVP514X_LUMA_CONTROL3_DEFAULT                           (0x02)
#define TVP514X_BRIGHTNESS_DEFAULT                              (0x80)
#define TVP514X_CONTRAST_DEFAULT                                (0x80)
#define TVP514X_SATURATION_DEFAULT                              (0x80)
#define TVP514X_HUE_DEFAULT                                     (0x00)
#define TVP514X_CHROMA_CONTROL1_DEFAULT                         (0x00)
#define TVP514X_CHROMA_CONTROL2_DEFAULT                         (0x0E)
#define TVP514X_OUTPUT1_DEFAULT                                 (0x40)
#define TVP514X_OUTPUT2_DEFAULT                                 (0x11)
#define TVP514X_OUTPUT3_DEFAULT                                 (0xFF)
#define TVP514X_OUTPUT4_DEFAULT                                 (0xFF)
#define TVP514X_OUTPUT5_DEFAULT                                 (0xFF)
#define TVP514X_OUTPUT6_DEFAULT                                 (0xFF)
#define TVP514X_FIFO_OUTPUT_CTRL_DEFAULT                        (0x01)

#define TVP514X_VBUS_ADDRESS_ACCESS0                            (0xE8)
#define TVP514X_VBUS_ADDRESS_ACCESS1                            (0xE9)
#define TVP514X_VBUS_ADDRESS_ACCESS2                            (0xEA)
#define TVP514X_VBUS_DATA_ACCESS                                (0xE0)
#define TVP514X_VBUS_DATA_ACCESS_AUTO_INCR                      (0xE1)

#define TVP514X_LINE_ADDRESS_START                              (0x80)
#define TVP514X_LINE_ADDRESS_MIDDLE                             (0x06)
#define TVP514X_LINE_ADDRESS_END                                (0x00)

#define TVP514X_LINE_ADDRESS_DEFAULT                            (0x00)
#define TVP514X_LINE_MODE_DEFAULT                               (0xFF)

#define TVP514X_VDP_LINE_START                                  (0xD6)
#define TVP514X_VDP_LINE_STOP                                   (0xD7)

#define TVP514X_VBI_NUM_SERVICES                                (3)
#define TVP514X_SLICED_BUF_SIZE                                 (128)

#ifdef BOARD_AP_IPNC
#define IMGS_I2C_ADDR       (0x90)
#endif

#ifdef BOARD_TI_EVM
#define IMGS_I2C_ADDR       (0xBA)
#endif

#define IMGS_RESET_GPIO     (91)

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

int DRV_imgsCheckId();
int DRV_imgsSetRegs();

#endif

