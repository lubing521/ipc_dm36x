
#ifndef _DRV_IMGS_OV2715_2MP_H_
#define _DRV_IMGS_OV2715_2MP_H_

#include <drv_imgs.h>
#include <drv_i2c.h>

#define IMGS_ROW_START_MIN  (0)
#define IMGS_COL_START_MIN  (0)

#define IMGS_H_PAD          (8)
#define IMGS_V_PAD          (6)

#define IMGS_W_MAX     (1920)
#define IMGS_H_MAX     (1080)

#define IMGS_MHZ        (1000000L)

#define IMGS_PLL_REF_CLK     (24*IMGS_MHZ)

#define IMGS_CHIP_ID        (0x2710)

/*SC control registers */
#define SYSTEM_CONTROL00 		0x3008 
#define CHIP_VERSION_H 			0x300A 
#define CHIP_VERSION_L 			0x300B 
#define MIPI_CTRL00 				0x300E 
#define PLL_CTRL00 					0x300F 
#define PLL_CTRL01 					0x3010 
#define PLL_CTRL02 					0x3011 
#define PLL_PREDIVIDER 			0x3012 
#define PAD_OUTPUT_ENABLE00	0x3016 
#define PAD_OUTPUT_ENABLE01	0x3017 
#define PAD_OUTPUT_ENABLE02	0x3018 
#define PAD_OUTPUT_VALUE00	0x3019 
#define PAD_OUTPUT_VALUE01	0x301A 
#define PAD_OUTPUT_VALUE02	0x301B 
#define PAD_OUTPUT_SELECT00	0x301C 
#define PAD_OUTPUT_SELECT01	0x301D 
#define PAD_OUTPUT_SELECT02	0x301E 
#define PAD_OUTPUT_DRIVE_C	0x302C 
#define PAD_INPUT_VALUE00 	0x3040 
#define PAD_INPUT_VALUE01 	0x3041 
#define PAD_INPUT_VALUE02 	0x3042 

/* SCCB control registers */
#define SCCB_ID									0x3100 
#define PLL_CLOCK_SELECT 				0x3103 
#define SCCB_PAD_CLOCK_DIVIDER 	0x3104

/* group sharing registers */
#define GROUP_ADDR0 				0x3200 
#define GROUP_ADDR1 				0x3201 
#define GROUP_ADDR2 				0x3202 
#define GROUP_ADDR3 				0x3203 
#define GROUP_ACCESS				0x3212 

/* analog registers */
#define ANA_ARRAY						0x3621        
#define SENSOR_REG0D				0x370D 


/* unknow register*/
#define SENSOR_RESV1				0x3706
#define SENSOR_RESV2				0x3712
#define SENSOR_RESV3				0x3630
#define SENSOR_RESV4				0x3604
#define SENSOR_RESV5				0x3603
#define SENSOR_RESV6				0x3631
#define SENSOR_RESV7				0x3600
#define SENSOR_RESV8				0x3620
#define SENSOR_RESV9				0x3623
#define SENSOR_RESV10				0x3702
#define SENSOR_RESV11				0x3703
#define SENSOR_RESV12				0x3704
#define SENSOR_RESV13				0x3713
#define SENSOR_RESV14				0x3714
#define SENSOR_RESV15				0x3710
#define SENSOR_RESV16				0x3605
#define SENSOR_RESV17				0x3606
#define SENSOR_RESV18				0x302d
#define SENSOR_RESV19				0x370b
#define SENSOR_RESV20				0x4301
#define SENSOR_RESV21				0x4303
#define SENSOR_RESV22				0x3A1A
#define SENSOR_RESV23				0x5688
#define SENSOR_RESV24				0x3716
#define SENSOR_RESV25				0x382E
#define SENSOR_RESV26				0x381A









/* timing control registers */
#define TIMING_CONTROL_HS_HIGHBYTE   	0x3800         
#define TIMING_CONTROL_HS_LOWBYTE    	0x3801         
#define TIMING_CONTROL_VS_HIGHBYTE   	0x3802         
#define TIMING_CONTROL_VS_LOWBYTE   	0x3803         
#define TIMING_CONTROL_HW_HIGHBYTE   	0x3804         
#define TIMING_CONTROL_HW_LOWBYTE    	0x3805         
#define TIMING_CONTROL_VH_HIGHBYTE   	0x3806         
#define TIMING_CONTROL_VH_LOWBYTE   	0x3807         
#define TIMING_CONTROL_DVP_HSIZE_HIGH 0x3808         
#define TIMING_CONTROL_DVP_HSIZE_LOW 	0x3809		   
#define TIMING_CONTROL_DVP_VSIZE_HIGH 0x380A		   
#define TIMING_CONTROL_DVP_VSIZE_LOW 	0x380B		   
#define TIMING_CONTROL_HTS_HIGHBYTE  	0x380C          
#define TIMING_CONTROL_HTS_LOWBYTE   	0x380D          
#define TIMING_CONTROL_VTS_HIGHBYTE  	0x380E          
#define TIMING_CONTROL_VTS_LOWBYTE   	0x380F         
#define TIMING_CONTROL_HV_OFFSET 			0x3810         
#define TIMING_CONTROL18 							0x3818 

/* AEC/AGC registers */
#define AEC_PK_EXPO1			0x3500 
#define AEC_PK_EXPO2			0x3501 
#define AEC_PK_EXPO3			0x3502 
#define AEC_PK_MANUAL			0x3503 
#define AEC_AGC_ADJ_H			0x350A 
#define AEC_AGC_ADJ_L 		0x350B 
#define AEC_PK_VTS_H			0x350C 
#define AEC_PK_VTS_L			0x350D 
#define AEC_CONTROL0 			0x3A00 
#define AEC_CONTROL1 			0x3A01 
#define AEC_MAX_EXPO_60_1	0x3A02  
#define AEC_MAX_EXPO_60_2	0x3A03  
#define AEC_MAX_EXPO_60_3	0x3A04 
#define AEC_B50_STEP_H		0x3A08 
#define AEC_B50_STEP_L 		0x3A09 
#define AEC_B60_STEP_H 		0x3A0A 
#define AEC_B60_STEP_L 		0x3A0B 
#define AEC_CONTROLD 			0x3A0D 
#define AEC_CONTROLE 			0x3A0E 
#define AEC_CONTROLF 			0x3A0F 
#define AEC_CONTROL10			0x3A10 
#define AEC_CONTROL11 		0x3A11 
#define AEC_CONTROL12 		0x3A12 
#define AEC_CONTROL13 		0x3A13 
#define AEC_MAX_EXPO50_1	0x3A14 
#define AEC_MAX_EXPO50_2 	0x3A15 
#define AEC_MAX_EXPO50_3 	0x3A16 
#define AEC_CONTROL17			0x3A17 
#define AEC_G_CEIL_H 			0x3A18 
#define AEC_G_CEIL_L 			0x3A19 
#define AEC_CONTROL1B 		0x3A1B 
#define AEC_LED_ADD_ROW_H	0x3A1C 
#define AEC_LED_ADD_ROW_L 0x3A1D 
#define AEC_CONTROL1E			0x3A1E  
#define AEC_CONTROL1F			0x3A1F  
#define AEC_CONTROL20			0x3A20

/* BLC control */
#define BLC_CONTROL_00				0x4000         
#define BLC_CONTROL_01				0x4001         
#define BLC_CONTROL_02				0x4002         
#define BLACK_LEVEL_TARGET_H	0x4006		   
#define BLACK_LEVEL_TARGET_L	0x4007		   
#define BLC_FRAME_CONTROL			0x401D


/*  DVP registers */
#define DVP_CTRL00	 		0x4700          
#define DVP_CTRL01 			0x4704          
#define DVP_CTRL02 			0x4708          
#define DVP_CTRL03 			0x4709 
#define POLARITY_CTRL00	0x4740


/*  ISP control registers  */  
#define ISP_CONTROL0 		0x5000        
#define ISP_CONTROL2 		0x5002        
#define ISP_CONTROL5 		0x5005        
#define ISP_CONTROL31		0x501F        
#define ISP_TEST				0x503D        
#define ISP_SENSOR_BIAS 0x503E		  
#define ISP_SENSOR_GAIN 0x503F		  

/*  AWB control registers  */
#define AWB_GAIN_PK_RED_H					0x3400		
#define AWB_GAIN_PK_RED_L					0x3401		
#define AWB_GAIN_PK_GREEN_H				0x3402		
#define AWB_GAIN_PK_GREEN_L				0x3403		
#define AWB_GAIN_PK_BLUE_H				0x3404		
#define AWB_GAIN_PK_BLUE_L				0x3405		
#define AWB_GAIN_PK_AWB 					0x3406		
#define AWB_CONTROL_00   					0x5180      
#define AWB_CONTROL_01   					0x5181      
#define AWB_CONTROL_02   					0x5182      
#define STABLE_RANGE 							0x5183		
#define RED_GAIN_LIMIT						0x518C      
#define GREEN_GAIN_LIMIT					0x518D      
#define BLUE_GAIN_LIMIT						0x518E      
#define AWB_FRAME_COUNTER					0x518F		
#define RED_BEFORE_GAIN_AVERAGE		0x5196		
#define GREEN_BEFORE_GAIN_AVERAGE	0x5197		
#define BLUE_BEFORE_GAIN_AVERAGE	0x5198		
#define RED_AFTER_GAIN_AVERAGE_H	0x5199		
#define RED_AFTER_GAIN_AVERAGE_L	0x519A		
#define GREEN_AFTER_GAIN_AVERAGE_H	0x519B		
#define GREEN_AFTER_GAIN_AVERAGE_L	0x519C		
#define BLUE_AFTER_GAIN_AVERAGE_H		0x519D		
#define BLUE_AFTER_GAIN_AVERAGE_L		0x519E	


/* AVG registers  */
#define AVG_START_POSITION_AT_HORIZONTAL_H 	0x5680		
#define AVG_START_POSITION_AT_HORIZONTAL_L 	0x5681		
#define AVG_END_POSITION_AT_HORIZONTAL_H 		0x5682		
#define AVG_END_POSITION_AT_HORIZONTAL_L 		0x5683		
#define AVG_START_POSITION_AT_VERTICAL_H		0x5684		
#define AVG_START_POSITION_AT_VERTICAL_L		0x5685		
#define AVG_END_POSITION_AT_VERTICAL_H 			0x5686      
#define AVG_END_POSITION_AT_VERTICAL_L     	0x5687       
#define AVG_R10 														0x5690   

#define IMGS_I2C_ADDR       (0x6C)

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

  int div_base;

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

int DRV_imgsCalcFrameTime(Uint32 fps, Uint32 dataW, Uint32 dataH, Bool binEnable, Bool flipH, Bool flipV);
int DRV_imgsCalcSW(int exposureTimeInUsecs);
int DRV_imgsCalcAgain(int aGain);
int DRV_imgsCalcFrameRate(int fps);
int DRV_imgsGetEshutter(Uint32 *eshutterInUsec);
int DRV_imgsGetAgain(int *again);

int DRV_imgsCheckId();
int DRV_imgsSetRegs();

int DRV_imgsAPChipInit(int *pGioC1, int *pGioD1);


#endif

