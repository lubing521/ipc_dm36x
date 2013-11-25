#ifndef _DRV_IMGS_AR0130_1MP_H_
#define _DRV_IMGS_AR0130_1MP_H_

#include <drv_imgs.h>
#include <drv_i2c.h>

#define IMGS_ROW_START_MIN  (40)
#define IMGS_COL_START_MIN  (0)

#define IMGS_H_PAD          (0)
#define IMGS_V_PAD          (gImgsVPad)

#define IMGS_W_MAX          (1280)
#define IMGS_H_MAX          (964)

#define IMGS_MHZ            (1000000L)
#define IMGS_PLL_REF_CLK    (24 * IMGS_MHZ)

#define IMGS_CHIP_ID        (0x2402) 
#define IMGS_I2C_ADDR       (0x20)
#define IMGS_RESET_GPIO     80

/* register addresses */
#define AR0130_CHIP_VERSION        0x3000
#define AR0130_ROW_START_A         0x3002
#define AR0130_COLUMN_START_A      0x3004
#define AR0130_ROW_END_A           0x3006
#define AR0130_COLUMN_END_A        0x3008
#define AR0130_FRAME_LINES_A       0x300A
#define AR0130_LINE_LENGTH         0x300C
#define AR0130_COARSE_IT_TIME_A    0x3012
#define AR0130_FINE_IT_TIME_A      0x3014
#define AR0130_COARSE_IT_TIME_B    0x3016
#define AR0130_FINE_IT_TIME_B      0x3018
#define AR0130_ENABLE_STREAMING    0x301a
#define AR0130_ROW_SPEED           0x3028

//fPIXCLK= (fEXTCLK ¡Á M) / (N ¡Á P1 x P2)
//M = PLL_Multiplier (R0x3030)
//N = Pre_PLL_Clk_Div (R0x302E)
//P1 = Vt_Sys_Clk_Div (R0x302C)
//P2 = Vt_PIX_Clk_Div (R0x302A)

#define AR0130_PLL_Multiplier      0x3030
#define AR0130_Pre_PLL_CLK_DIV     0x302E
#define AR0130_VT_SYS_CLK_DIV      0x302C
#define AR0130_VT_PIX_CLK_DIV      0x302A

#define AR0130_PRE_PLL_CLK_DIV     0x302C
#define AR0130_PLL_MULTIPLIER      0x3030
#define AR0130_SCALING_MODE        0x3032
#define AR0130_READ_MODE           0x3040
#define AR0130_DARK_CONTROL        0x3044
#define AR0130_GLOABL_GAIN         0x305E
#define AR0130_EMBEDDED_DATA       0x3064
#define AR0130_OP_MODE_A           0x3082
#define AR0130_OP_MODE_B           0x3084
#define AR0130_COLUMN_START_B      0x308A
#define AR0130_ROW_START_B         0x308C
#define AR0130_COLUMN_END_B        0x308E
#define AR0130_ROW_END_B           0x3090
#define AR0130_Y_ODD_INC_A         0x30A6
#define AR0130_Y_ODD_INC_B         0x30A8
#define AR0130_FRAME_LINES_B       0x30AA
#define AR0130_DIGITAL_TEST        0x30B0
#define AR0130_DAC_LD_24_25        0x3EE4
#define AR0130_AE_CTRL_REG         0x3100

#define AR0130_FIRST_AGAIN_ADDR_A  0x30B0
#define AR0130_SECOND_AGAIN_ADDR_A 0x3EE4
#define AR0130_DGAIN_ADDR_A        0x305E

typedef struct
{
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

typedef struct
{
    DRV_I2cHndl        i2cHndl;
    DRV_ImgsFrameTime  curFrameTime;
    DRV_ImgsModeConfig curModeConfig;
} DRV_ImgsObj;

extern DRV_ImgsObj          gDRV_imgsObj;
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

extern DRV_ImgsIsifConfig   gDRV_imgsIsifConfig_Common_AR0130;
extern DRV_ImgsIpipeConfig  gDRV_imgsIpipeConfig_Vnfdemo_AR0130;
extern DRV_ImgsIpipeConfig  gDRV_imgsIpipeConfig_Common_AR0130;

extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_Appro_AR0130;
extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_TI_AR0130;
extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_Common_AR0130;

int DRV_imgsReset();
int DRV_imgsCalcFrameTime_AR0130(Uint32 fps, Uint32 dataW, Uint32 dataH, Bool binEnable);
int DRV_imgsCalcSW_AR0130(int exposureTimeInUsecs);
int DRV_imgsCalcAgain_AR0130(int aGain);
int DRV_imgsCalcFrameRate_AR0130(int fps);
int DRV_imgsGetEshutter_AR0130(Uint32 *eshutterInUsec);
int DRV_imgsGetAgain_AR0130(int *again);
int DRV_imgsCheckId_AR0130();
int DRV_imgsSetRegs_AR0130();

#endif

