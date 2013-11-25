
#include "drv_imgs_MN34041_2.1MP.h"
#include <drv_gpio.h>
#include <math.h>

#define __DRV_IMGS_IMX036_DEBUG__
#ifdef __DRV_IMGS_IMX036_DEBUG__
#define DB(fmt, args...) fprintf(stderr,fmt, ## args)
#else
#define DB(fmt, args...)
#endif
#define DBErr(fmt, args...) fprintf(stderr,fmt, ## args)

//SPI0
#define SPI1_CS_GPO     (23)   
#define SPI1_SCLK_GPO 	(24)   
#define SPI1_SDO_GPO 	(22)      
//#define SPI1_SDI_GPI 	(37)   

#define PCLK 81000000
#define LINE_LENGTH 2400
#define FRAME_LENGTH 1125
#define ROW_TIME (LINE_LENGTH / PCLK) //29.6us  

const unsigned short MN34041_ADDR[382] = {
0x0001,
0x0002,
0x0004,
0x0000,
0x0003,
0x0006,
0x0005,
0x0000,
0x0036,
0x0037,
0x0020,
0x0021,
0x0022,
0x0023,
0x0024,
0x0025,
0x0026,
0x0027,
0x0028,
0x0030,
0x0031,
0x0032,
0x0033,
0x0034,
0x0035,
0x0038,
0x0039,
0x003a,
0x003b,
0x003f,
0x0040,
0x0041,
0x0042,
0x0043,
0x0044,
0x0045,
0x0046,
0x0047,
0x0048,
0x0049,
0x004a,
0x004b,
0x0056,
0x0057,
0x0058,
0x0059,
0x005a,
0x0070,
0x0071,
0x0072,
0x0073,
0x0074,
0x0075,
0x0076,
0x0077,
0x0078,
0x0079,
0x0080,
0x0081,
0x0082,
0x0083,
0x0084,
0x0085,
0x0086,
0x0087,
0x0088,
0x0089,
0x008a,
0x008b,
0x008c,
0x008d,
0x008e,
0x008f,
0x0090,
0x0091,
0x0092,
0x0093,
0x0094,
0x0095,
0x0096,
0x0097,
0x00a0,
0x00a1,//shutter start line
0x00a2,
0x00a3,
0x00a4,
0x00a5,
0x00a6,
0x00a7,
0x00a8,
0x00a9,
0x00c0,
0x00c1,
0x00c2,
0x00c3,
0x00c4,
0x00c5,
0x00c6,
0x00c7,
0x00ca,
0x00cb,
0x00cc,
0x00cd,
0x00ce,
0x0100,
0x0101,
0x0102,
0x0103,
0x0104,
0x0105,
0x0106,
0x0170,
0x0171,
0x0172,
0x0173,
0x0174,
0x0175,
0x0176,
0x0177,
0x0178,
0x0179,
0x017a,
0x017b,
0x017c,
0x0190,
0x0191,
0x0192,
0x0193,
0x0194,
0x0195,
0x0196,
0x0197,
0x0198,
0x0199,
0x019a,
0x019b,
0x019c,
0x019d,
0x019e,
0x01a0,
0x01a1,
0x01a2,
0x01a3,
0x01a4,
0x01a5,
0x01a6,
0x01a7,
0x01a8,
0x01a9,
0x01aa,
0x01ab,
0x01ac,
0x01ad,
0x01ae,
0x01af,
0x01b0,
0x01b1,
0x01b2,
0x01b3,
0x01b4,
0x01b5,
0x01b6,
0x01b7,
0x01b8,
0x01b9,
0x01ba,
0x01bb,
0x01bc,
0x01bd,
0x01be,
0x01c4,
0x01c5,
0x01c6,
0x0200,
0x0201,
0x0202,
0x0203,
0x0204,
0x0205,
0x0206,
0x0270,
0x0271,
0x0272,
0x0273,
0x0274,
0x0275,
0x0276,
0x0277,
0x0278,
0x0279,
0x027a,
0x027b,
0x027c,
0x0290,
0x0291,
0x0292,
0x0293,
0x0294,
0x0295,
0x0296,
0x0297,
0x0298,
0x0299,
0x029a,
0x029b,
0x029c,
0x029d,
0x029e,
0x02a0,
0x02a1,
0x02a2,
0x02a3,
0x02a4,
0x02a5,
0x02a6,
0x02a7,
0x02a8,
0x02a9,
0x02aa,
0x02ab,
0x02ac,
0x02ad,
0x02ae,
0x02af,
0x02b0,
0x02b1,
0x02b2,
0x02b3,
0x02b4,
0x02b5,
0x02b6,
0x02b7,
0x02b8,
0x02b9,
0x02ba,
0x02bb,
0x02bc,
0x02bd,
0x02be,
0x02c4,
0x02c5,
0x02c6,
0x0108,
0x0109,
0x010a,
0x010b,
0x010c,
0x010d,
0x010e,
0x010f,
0x0110,
0x0111,
0x0112,
0x0113,
0x0114,
0x0115,
0x0116,
0x0117,
0x0118,
0x0119,
0x011a,
0x011b,
0x011c,
0x011d,
0x011e,
0x011f,
0x0120,
0x0121,
0x0122,
0x0123,
0x0124,
0x0125,
0x0126,
0x0127,
0x0128,
0x0129,
0x012a,
0x012b,
0x012c,
0x012d,
0x012e,
0x012f,
0x0130,
0x0131,
0x0132,
0x0133,
0x0134,
0x0135,
0x0136,
0x0137,
0x0138,
0x0139,
0x013a,
0x0140,
0x0141,
0x0142,
0x0143,
0x0144,
0x0145,
0x0146,
0x0150,
0x0151,
0x0152,
0x0153,
0x0154,
0x0155,
0x0156,
0x0157,
0x0158,
0x0159,
0x015a,
0x015b,
0x015c,
0x015d,
0x015e,
0x015f,
0x0160,
0x0161,
0x0162,
0x0163,
0x0164,
0x0165,
0x0166,
0x0167,
0x0168,
0x0169,
0x016a,
0x016b,
0x0180,
0x0181,
0x0182,
0x0183,
0x0184,
0x0185,
0x0186,
0x0187,
0x0188,
0x0189,
0x018a,
0x018b,
0x018c,
0x018d,
0x01d0,
0x01d1,
0x01d2,
0x01d3,
0x01d4,
0x01d5,
0x01d6,
0x01d7,
0x01d8,
0x01d9,
0x01da,
0x01db,
0x01dc,
0x01dd,
0x01de,
0x01df,
0x01e0,
0x01e1,
0x01e2,
0x01e3,
0x01e4,
0x01e5,
0x01e6,
0x01e7,
0x01e8,
0x01e9,
0x01ea,
0x01eb,
0x01ec,
0x01ed,
0x01ee,
0x01ef,
0x01f0,
0x01f1,
0x01f2,
0x01f3,
0x01f4,
0x01f5,
0x01f6,
0x0000,
};


const unsigned short MN34041_REG[382] = {
0x001B,
0x0002,
0x01c2,
0x0003,
0x0209,
0x442a,
0x0000,
0x0013,
0x0021,
0x0300,
0x0080,
0x0080,
0x0000,
0x0000,
0x0010,
0x0011,
0x0012,
0x0013,
0x0000,
0x0424,
0x110A,
0x7450,
0x0000,
0x0000,
0x0281,
0x0001,
0x0210,
0x0333,
0x0111,
0x0000,
0x0004,
0x0200,
0x0100,
0x0070,
0x0000,
0x0000,
0x0000,
0x0010,
0x0000,
0x0002,
0x0ffe,
0x0004,
0x0000,
0x1fff,
0x2000,
0x0000,
0x0011,
0x2084,
0xffff,
0x0000,
0x0000,
0x0000,
0x0004,
0x0230,
0x0541,
0x0001,
0x0011,
0x0002,
0x0003,
0x0000,
0x0001,
0x0012,
0x0013,
0x0010,
0x0011,
0x000a,
0x000b,
0x0008,
0x0009,
0x0006,
0x0007,
0x0004,
0x0005,
0x0016,
0x0017,
0x0014,
0x0015,
0x001a,
0x001b,
0x0018,
0x0019,
0x3000,
0x0000,//shutter start line value
0x0002,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x000f,
0x0022,
0x5540,
0x5fd5,
0xf757,
0xdf5f,
0x208a,
0x0071,
0x0557,
0x0000,
0x0080,
0x0000,
0x0000,
0x0000,
0x0000,
0x03a8,
0x03a8,
0x02c0,
0x037a,
0x002b,
0x00de,
0x00fa,
0x0002,
0x000d,
0x0007,
0x0004,
0x002a,
0x0062,
0x0079,
0x0326,
0x0003,
0x0033,
0x0360,
0x0002,
0x000d,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x01ba,
0xb060,
0x7c0a,
0x0000,
0x0313,
0x0b08,
0x3906,
0x0000,
0x0464,
0x0000,
0x0000,
0x0464,
0x0000,
0x0453,
0x0000,
0x0464,
0x0000,
0x0454,
0x0000,
0x0000,
0x0000,
0x0454,
0x0000,
0x0000,
0x0000,
0x0454,
0x0000,
0x0000,
0x0000,
0x0454,
0x0000,
0x0000,
0x0000,
0x0453,
0x0000,
0x0000,
0x0000,
0x0453,
0x0000,
0x0000,
0x0000,
0x0011,
0x03a8,
0x03a8,
0x02c0,
0x037a,
0x002b,
0x00de,
0x00fa,
0x0002,
0x000d,
0x0007,
0x0004,
0x002a,
0x0062,
0x0079,
0x0326,
0x0003,
0x0033,
0x0360,
0x0002,
0x000d,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x01ba,
0xb060,
0x7c0a,
0x0000,
0x0313,
0x0b08,
0x3906,
0x0000,
0x0464,
0x0000,
0x0000,
0x0464,
0x0000,
0x0453,
0x0000,
0x0464,
0x0000,
0x0454,
0x0000,
0x0000,
0x0000,
0x0454,
0x0000,
0x0000,
0x0000,
0x0454,
0x0000,
0x0000,
0x0000,
0x0454,
0x0000,
0x0000,
0x0000,
0x0453,
0x0000,
0x0000,
0x0000,
0x0453,
0x0000,
0x0000,
0x0000,
0x0011,
0x0000,
0x000f,
0x0009,
0x0000,
0x0016,
0x0000,
0x000f,
0x0000,
0x0009,
0x0000,
0x0016,
0x0003,
0x000a,
0x0000,
0x0009,
0x0000,
0x0016,
0x0018,
0x0017,
0x0000,
0x0002,
0x0009,
0x0012,
0x0001,
0x003a,
0x0000,
0x0000,
0x0000,
0x0011,
0x0000,
0x0003,
0x0003,
0x0000,
0x0010,
0x0000,
0x0003,
0x0000,
0x0011,
0x0000,
0x0009,
0x0009,
0x0012,
0x0000,
0x0000,
0x0009,
0x0009,
0x0012,
0x0006,
0x0000,
0x0010,
0x0000,
0x0020,
0x0036,
0x0000,
0x0001,
0x0003,
0x0000,
0x0000,
0x0011,
0x0001,
0x0001,
0x0001,
0x0010,
0x0000,
0x0003,
0x0000,
0x0013,
0x0000,
0x0000,
0x0008,
0x0000,
0x0004,
0x0005,
0x000a,
0x0006,
0x000f,
0x000d,
0x0004,
0x0002,
0x0002,
0x0000,
0x0010,
0x0009,
0x0003,
0x0000,
0x0000,
0x0017,
0x00c5,
0x0000,
0x0000,
0x00fa,
0x00a5,
0x01ef,
0x03d9,
0x01fb,
0x035f,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0006,
0x0005,
0x0001,
0x0006,
0x0001,
0x0007,
0x0001,
0x0002,
0x0001,
0x0001,
0x00c9,
0x8000,
0x003e,
0x0015,
0x003e,
0x00c8,
0x0043,
0x00a9,
0x00a9,
0x00a9,
0x00fb,
0x00b0,
0x00b9,
0x01bb,
0x02ec,
0x02ec,
0x02ec,
0x01bd,
0x034a,
0x03d8,
0x03fc,
0x0113,
};
DRV_ImgsObj gDRV_imgsObj;
Bool gbIsDDRIn = FALSE;
Bool gbIsAlawEnalbe = FALSE;
int gCurrsensorMode = DRV_IMGS_SENSOR_MODE_1920x1080;

int DRV_imgsGetDdrInMode(void){
	return (int)gbIsDDRIn;
}
void SpiWrite_gio(unsigned short addr,unsigned short reg)   
{                                
  int i;                         
  // sclk low                    
  DRV_gpioClr(SPI1_SCLK_GPO);    
  // CS low                      
  DRV_gpioClr(SPI1_CS_GPO);      
                                 

  for (i=0; i<16; i++) {         
    if (addr & 0x00000001)       
      DRV_gpioSet(SPI1_SDO_GPO); 
    else                         
      DRV_gpioClr(SPI1_SDO_GPO); 
    addr = addr >> 1; 
                                          
    DRV_gpioSet(SPI1_SCLK_GPO); 
 
    DRV_gpioClr(SPI1_SCLK_GPO);  
  }
/***********************************/  
 for (i=0; i<16; i++) {         
    if (reg & 0x00000001)       
      DRV_gpioSet(SPI1_SDO_GPO); 
    else                         
      DRV_gpioClr(SPI1_SDO_GPO); 
    reg = reg >> 1; 
                                          
    DRV_gpioSet(SPI1_SCLK_GPO); 
 
    DRV_gpioClr(SPI1_SCLK_GPO);  
  }  

                            
  // CS high                     
  DRV_gpioSet(SPI1_CS_GPO);      
}                                

int DRV_imgsOpen(DRV_ImgsConfig *config)
{
	int status=0;
	Uint16 width, height;
int value32;

	DB("############OPEN###########\n");
	memset(&gDRV_imgsObj, 0, sizeof(gDRV_imgsObj));
	gCurrsensorMode = config->sensorMode;
	DRV_imgGetWidthHeight(config->sensorMode, &width, &height);
	if(gCurrsensorMode==DRV_IMGS_SENSOR_MODE_2048x1536){
		width+=IMGS_H_3M_PAD;
		height+=IMGS_V_3M_PAD;
	}else{
	width+=IMGS_H_PAD;
	height+=IMGS_V_PAD;
	}
	#if (1)  
	DRV_imgsCalcFrameTime(config->fps, width, height, config->binEnable);
	#else
	DRV_imgsCalcFrameTime(config->fps, width, height, config->binEnable);
	gDRV_imgsObj.curModeConfig.sensorDataWidth   = width;
	gDRV_imgsObj.curModeConfig.sensorDataHeight  = height;
	gDRV_imgsObj.curModeConfig.validStartX       = IMGS_H_PAD/2;
	gDRV_imgsObj.curModeConfig.validStartY       = IMGS_V_PAD/2;    
	gDRV_imgsObj.curModeConfig.validWidth        = width-IMGS_H_PAD;      
	gDRV_imgsObj.curModeConfig.validHeight       = height-IMGS_V_PAD;        
	gDRV_imgsObj.curModeConfig.binEnable         = 0;
	#endif
	DB("############reset###########\n");
	/* Sensor reset */
	DRV_gpioSetMode(IMGS_RESET_GPIO, DRV_GPIO_DIR_OUT);
	DRV_gpioSet(IMGS_RESET_GPIO);  
	DRV_gpioClr(IMGS_RESET_GPIO);
	OSA_waitMsecs(50);
	DRV_gpioSet(IMGS_RESET_GPIO);  
	OSA_waitMsecs(50); 

//SPI0 GIO INILIZATION
  // init PINMUX3  GIO23 24 22
  status = CSL_gpioGetPinmux(&gCSL_gpioHndl, 3, &value32);
  if(status==OSA_SOK) {
      value32 &= 0xe1ffffff;
      //value32 &=~((3<<26)|(1<<28)|(1<<25));
    status = CSL_gpioSetPinmux(&gCSL_gpioHndl, 3, value32);
    OSA_printf("set pinmux3 = 0x%x\n", value32);
  }
    
  DRV_gpioSetMode(SPI1_CS_GPO, DRV_GPIO_DIR_OUT);
  DRV_gpioSetMode(SPI1_SCLK_GPO, DRV_GPIO_DIR_OUT);
  DRV_gpioSetMode(SPI1_SDO_GPO, DRV_GPIO_DIR_OUT);

	DB("############SET REG###########\n");	
	status = DRV_imgsSetRegs(width,  height, config->fps);
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_imgsCheckId()\n");
		DRV_SPIClose(&gDRV_imgsObj.spiHndl);    
		return status;
	}
	DB("############OPEN END###########\n");	
	gbIsDDRIn = config->bIsDdrIn;
	gbIsAlawEnalbe = config->bIsALawEnable;

	return 0;
}

int DRV_imgsClose()
{
	int status;
	//DRV_imgs_DarkframeRelease();
	status = DRV_imgsEnable(FALSE);
	status |= DRV_SPIClose(&gDRV_imgsObj.spiHndl);

	return status;
}

char* DRV_imgsGetImagerName()
{
	return "MN34041_2.1MP";
}

int DRV_imgsSpecificSetting(void)
{
  return OSA_SOK;
}


int DRV_imgsSet50_60Hz(Bool is50Hz)
{
	return 0;

}

int DRV_imgsSetFramerate(int fps)
{
  return OSA_SOK;

}

int DRV_imgsBinEnable(Bool enable)
{

  return OSA_EFAIL;

}

int DRV_imgsBinMode(int binMode)
{

  return OSA_SOK;

}

//return value is 8 times the true analog gain
int DRV_imgsGetAgain(int *again)
{
	/* Not support */
	return OSA_EFAIL;
}

int DRV_imgsGetEshutter(Uint32 *eshutterInUsec)
{
	/* Not support */
	return OSA_EFAIL;
}

int DRV_imgsSetOB(int Level)
{
	int	status = OSA_SOK;

	return status;
}

int DRV_imgsSetDcSub(Uint32 dcSub, int setRegDirect)
{
	return OSA_SOK;
}

int DRV_imgsEnable(Bool enable)
{
	return OSA_SOK;
}

int DRV_imgsGetLightSensorMode(void){
	/* return Low light:1, Normal:0 */
	/* LS:Normal 1, low light 0 */
	return 0;
}

int DRV_imgs_IsDDRIn(void){
	return gbIsDDRIn;
}

int DRV_imgs_IsAlawEnable(void){
	return gbIsAlawEnalbe;
}

int DRV_imgsNDShutter(int bIsNight)
{	
	return 0; 	
}

DRV_ImgsModeConfig      *DRV_imgsGetModeConfig(int sensorMode)
{
  return &gDRV_imgsObj.curModeConfig;
}

DRV_ImgsIsifConfig      *DRV_imgsGetIsifConfig(int sensorMode)
{
  return &gDRV_imgsIsifConfig_Common;
}

DRV_ImgsIpipeConfig     *DRV_imgsGetIpipeConfig(int sensorMode, int vnfDemoCfg)
{
  if(vnfDemoCfg)
  	return &gDRV_imgsIpipeConfig_Vnfdemo;
  else
  	return &gDRV_imgsIpipeConfig_Common;
}

DRV_ImgsH3aConfig       *DRV_imgsGetH3aConfig(int sensorMode, int aewbVendor)
{
  if(aewbVendor==1) {
  	printf("APPRO Setting Applied\n");
  	return &gDRV_imgsH3aConfig_Appro;
  }
  else if(aewbVendor==2) {
  	printf("TI Setting Applied\n");
  	return &gDRV_imgsH3aConfig_TI;
  }
  else {
  	printf("Common Setting Applied\n");
  	return &gDRV_imgsH3aConfig_Common;
  }
}

DRV_ImgsLdcConfig       *DRV_imgsGetLdcConfig(int sensorMode, Uint16 ldcInFrameWidth, Uint16 ldcInFrameHeight)
{
  OSA_printf(" DRV LDC: GetConfig %04x %dx%d\n", sensorMode, ldcInFrameWidth, ldcInFrameHeight);

  sensorMode &= 0xFF;

  switch(sensorMode) {

    case DRV_IMGS_SENSOR_MODE_720x480:
    case DRV_IMGS_SENSOR_MODE_1620x1080:

      if(ldcInFrameWidth==864)
        return &gDRV_imgsLdcConfig_736x480_0_VS;

      if(ldcInFrameWidth==352)
        return &gDRV_imgsLdcConfig_736x480_1_VS;

      if(ldcInFrameWidth==736)
        return &gDRV_imgsLdcConfig_736x480_0;

      if(ldcInFrameWidth==288)
        return &gDRV_imgsLdcConfig_736x480_1;

      if(ldcInFrameWidth==768)
        return &gDRV_imgsLdcConfig_768x512_0;

      if(ldcInFrameWidth==320)
        return &gDRV_imgsLdcConfig_768x512_1;

      if(ldcInFrameWidth==928)
        return &gDRV_imgsLdcConfig_768x512_0_VS;

      if(ldcInFrameWidth==384)
        return &gDRV_imgsLdcConfig_768x512_1_VS;

      break;

    case DRV_IMGS_SENSOR_MODE_1280x720:
    case DRV_IMGS_SENSOR_MODE_1920x1080:

      if(ldcInFrameWidth==1280)
        return &gDRV_imgsLdcConfig_1280x736_0;

      if(ldcInFrameWidth==320)
        return &gDRV_imgsLdcConfig_1280x736_1;

      if(ldcInFrameWidth==640)
        return &gDRV_imgsLdcConfig_1280x736_2;

      if(ldcInFrameWidth==1536)
        return &gDRV_imgsLdcConfig_1280x736_0_VS;

      if(ldcInFrameWidth==384)
        return &gDRV_imgsLdcConfig_1280x736_1_VS;

      if(ldcInFrameWidth==768)
        return &gDRV_imgsLdcConfig_1280x736_2_VS;

      if(ldcInFrameWidth==1312)
        return &gDRV_imgsLdcConfig_1312x768_0;

      if(ldcInFrameWidth==352)
        return &gDRV_imgsLdcConfig_1312x768_1;

      if(ldcInFrameWidth==672)
        return &gDRV_imgsLdcConfig_1312x768_2;

      if(ldcInFrameWidth==1600)
        return &gDRV_imgsLdcConfig_1312x768_0_VS;

      if(ldcInFrameWidth==448)
        return &gDRV_imgsLdcConfig_1312x768_1_VS;

      if(ldcInFrameWidth==832)
        return &gDRV_imgsLdcConfig_1312x768_2_VS;

      break;

  }

  return NULL;
}

int DRV_imgsReset()
{
	return OSA_SOK;
}

int DRV_imgsCheckId()
{
	return OSA_SOK;
}

int DRV_imgsSetDgain(int dgain)
{
    return OSA_SOK;
}

int DRV_imgsSetAgain(int again, int setRegDirect)
{
    if( again >= 8000 ) 
    {   
        SpiWrite_gio(0x0020,0xC0C0);
    }   
    else if (again >= 4000)
    {   
        SpiWrite_gio(0x0020,0xC080);
    }   
    else if (again >= 2000)
    {    
        SpiWrite_gio(0x0020,0x8080);
    }   
    else
    {   
        SpiWrite_gio(0x0020,0x0080);
    }   
 
    OSA_waitMsecs(100);
 
    return OSA_SOK;
}
//set the shutter start line
int DRV_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect)
{

      int shtpos;   

   shtpos = FRAME_LENGTH - (eshutterInUsec)/30;//(ROW_TIME);

//   printf("Shutter start line = %d --------\n",shtpos);
   SpiWrite_gio(0x00A1,shtpos);

     OSA_waitMsecs(20);

      return OSA_SOK;
}


int DRV_imgsSetRegs(Uint16 width, Uint16 height, int fps)
{
	int i;

	for(i=0;i<382;i++)
	{
		SpiWrite_gio(MN34041_ADDR[i],MN34041_REG[i]);
		
		if(i==6)
			OSA_waitMsecs(10);
    		if(i==380)
        		OSA_waitMsecs(100);
	}
}

int DRV_imgs_APChipInit(int *pGioC1, int *pGioD1){
	unsigned int *pPINMUXC1 = NULL;
	unsigned int *pPINMUXD1 = NULL;
	unsigned int val1, val2;
	unsigned char *pRegStart; 
	int ret = 0;

	if(gCSL_vpssHndl.regBaseVirtAddr==0 || pGioC1==0 || pGioD1==0){
		return -1;
	}
	pRegStart = (unsigned char *)gCSL_vpssHndl.regBaseVirtAddr;
	
	pPINMUXC1	= (unsigned int *)( pRegStart+ (AP_CHIP_C1_PINMUX-RegBaseVirtStart));
	pPINMUXD1	= (unsigned int *)( pRegStart+ (AP_CHIP_D1_PINMUX-RegBaseVirtStart));
	/* Enable C1 */
	*pPINMUXC1 = ((*pPINMUXC1 & (~AP_CHIP_C1_MASK)) | AP_CHIP_C1_VAL);
	val1 = (*pPINMUXC1 & AP_CHIP_C1_MASK);
	if(AP_CHIP_C1_VAL!= val1){
		DBErr("PINMUXC1 setting error(%X)!!!\n",val1);
		ret = -1;
		goto __DRV_imgs_APChipInit_End__;
	}
	/* Enable D1 */
	*pPINMUXD1 = (*pPINMUXD1 & (~AP_CHIP_D1_MASK)) | (AP_CHIP_D1_VAL);
	val2 = (*pPINMUXD1 & AP_CHIP_D1_MASK);
	if(AP_CHIP_D1_VAL != val2){
		DBErr("PINMUXD1 setting error(%X)!!!\n",val2);
		ret = -1;
		goto __DRV_imgs_APChipInit_End__;
	}

	*pGioC1 = IMGS_APCHIP_C1_GIO;
	*pGioD1 = IMGS_APCHIP_D1_GIO;

__DRV_imgs_APChipInit_End__:

	return ret;
}
