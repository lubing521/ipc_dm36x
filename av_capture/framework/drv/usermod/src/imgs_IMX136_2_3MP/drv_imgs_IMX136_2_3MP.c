
#include "drv_imgs_IMX136_2_3MP.h"
#include "imx136_reg_setting.h"
#include <drv_gpio.h>
#include <math.h>
#include "drv_imgs_imx136_board_version.h"

#define __DRV_IMGS_IMX036_DEBUG__
#ifdef __DRV_IMGS_IMX036_DEBUG__
#define DB(fmt, args...) fprintf(stderr,fmt, ## args)
#else
#define DB(fmt, args...)
#endif
#define DBErr(fmt, args...) fprintf(stderr,fmt, ## args)

//#define AEWB_DEBUG 

#define IMX136_SPI_DRIVER_MODE 1/*spi driver mode,0:gpio;1:spi controller*/
//#define IMX136_SPI_TEST

#if (IMX136_SPI_DRIVER_MODE == 0)

#if (IMX136_DM368_ON_IPNC_IPCAM == 1)
#define SPI1_CS_GPO           (23)   
#define SPI1_SCLK_GPO 	(24)   
#define SPI1_SDO_GPO 	(22)        
#else
#define SPI1_CS_GPO           (27)
#define SPI1_SCLK_GPO 	(28)
#define SPI1_SDO_GPO 	(26)
#endif

#endif

#define PCLK 81000000
#define LINE_LENGTH 2400
#define FRAME_LENGTH 1125
#define ROW_TIME (LINE_LENGTH / PCLK) //29.6us  

DRV_ImgsObj gDRV_imgsObj;
DRV_ImgsObj gDRV_imgsObj;


Bool gbIsDDRIn = FALSE;
Bool gbIsAlawEnalbe = FALSE;
int gCurrsensorMode = DRV_IMGS_SENSOR_MODE_1920x1080;

int DRV_imgsGetDdrInMode(void){
	return (int)gbIsDDRIn;
}

void imx136_setgio(unsigned short addr, unsigned short reg)
{
    int i;
    unsigned char imx136_value;
    unsigned char spi_chipid;
    unsigned char page_addr;
     unsigned char regset[3];
	 
    spi_chipid = (unsigned char)(addr >> 8);
    page_addr = (unsigned char)(addr&0xFF);
    imx136_value =( unsigned char) reg;

	regset[0] = spi_chipid;
	regset[1] = page_addr;
	regset[2] = imx136_value;

#if (IMX136_SPI_DRIVER_MODE == 0)
  // sclk low
  DRV_gpioClr(SPI1_SCLK_GPO);    
  // CS low                      
  DRV_gpioClr(SPI1_CS_GPO); 

/*spi_chipid*/
for (i=0; i<8; i++) {         
    if (spi_chipid & 0x01)
      DRV_gpioSet(SPI1_SDO_GPO);       
    else                         
      DRV_gpioClr(SPI1_SDO_GPO);  
    spi_chipid = spi_chipid >> 1; 
                          
    DRV_gpioSet(SPI1_SCLK_GPO);   
    DRV_gpioClr(SPI1_SCLK_GPO);              
  }
/*page address*/	  
  for (i=0; i<8; i++) {         
    if (page_addr & 0x01)
      DRV_gpioSet(SPI1_SDO_GPO);       
    else                         
      DRV_gpioClr(SPI1_SDO_GPO);  
    page_addr = page_addr >> 1; 
                          
    DRV_gpioSet(SPI1_SCLK_GPO);   
    DRV_gpioClr(SPI1_SCLK_GPO);              
  }
/*data*/  
 for (i=0; i<8; i++) {         
    if (imx136_value & 0x01)  
      DRV_gpioSet(SPI1_SDO_GPO); 
    else                         
      DRV_gpioClr(SPI1_SDO_GPO); 
    imx136_value = imx136_value >> 1; 
                                          
    DRV_gpioSet(SPI1_SCLK_GPO);   
    DRV_gpioClr(SPI1_SCLK_GPO);   
  }  
                        
  // CS high                     
    DRV_gpioSet(SPI1_CS_GPO); 
  #else
  
  DRV_SPIWrite8(&gDRV_imgsObj.spiHndl, regset,3);
  
  #endif
}
int InitRegArrary()
{
   int i=0;
   
#ifdef  IMX136_SPI_TEST
do{
          imx136_setgio(IMX136_SETTING[0],IMX136_SETTING[1]);      
}while(1);	 
#endif


//for (i = 0; i < IMX136_REG_COUNT; i = i+2)
for (i = 0; i < 375*2; i = i+2)
{

          imx136_setgio(IMX136_SETTING[i],IMX136_SETTING[i+1]);      
  
}

#ifndef IMX136_LVDS324
 imx136_setgio(0x0244,1); 
 imx136_setgio(0x0247,3); 
 imx136_setgio(0x0248,0xd3); 
 #endif

 imx136_setgio(0x0200,0); //enable the streaming 

OSA_printf("Current FRAME RATE SEL:%d,SETTING INDEX:%d\n",IMX136_FRSEL,i);

 return 0;

}

int DRV_imgsOpen(DRV_ImgsConfig *config)
{
	int status=0;
	Uint16 width, height;
        int value32;

	memset(&gDRV_imgsObj, 0, sizeof(gDRV_imgsObj));

	DRV_imgGetWidthHeight(config->sensorMode, &width, &height);

OSA_printf("sensor mode:%d;width:%d;height:%d;%s",config->sensorMode,width,height,__FUNCTION__);

	width+=IMGS_H_PAD;
	height+=IMGS_V_PAD;


#ifdef IMX136_LVDS324
	/*for lvds324*/
       status = DRV_i2cOpen(&gDRV_imgsObj.i2cHndl, IMGS_I2C_ADDR);
       if(status!=OSA_SOK) {
         OSA_ERROR("DRV_i2cOpen()\n");
         return status;
        }
#endif

	#if (0)  
	DRV_imgsCalcFrameTime(config->fps, width, height, config->binEnable);
	#else
	DRV_imgsCalcFrameTime(config->fps, width, height, config->binEnable);
	gDRV_imgsObj.curModeConfig.sensorDataWidth   = width;
	gDRV_imgsObj.curModeConfig.sensorDataHeight  = height-10;//That's the isif Vdint
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

#if (IMX136_SPI_DRIVER_MODE ==0)

#if(IMX136_DM368_ON_IPNC_IPCAM==1)

		//SPI0 GIO INILIZATION
		  // init PINMUX3  GIO23 24 22
		  status = CSL_gpioGetPinmux(&gCSL_gpioHndl, 3, &value32);
		  if(status==OSA_SOK) {
		      value32 &= 0xe1ffffff;
		      //value32 &=~((3<<26)|(1<<28)|(1<<25));
		    status = CSL_gpioSetPinmux(&gCSL_gpioHndl, 3, value32);
		    OSA_printf("set pinmux3 = 0x%x\n", value32);
		  }
 #else
		  // init PINMUX3
		  status = CSL_gpioGetPinmux(&gCSL_gpioHndl, 3, &value32);
		  if(status==OSA_SOK) {
		    value32 &= ~(0x1<<31); // clear bit 31, GIO 26

		    status = CSL_gpioSetPinmux(&gCSL_gpioHndl, 3, value32);
		    OSA_printf("set pinmux3 = 0x%x\n", value32);
		  }

		  // init PINMUX4
		  status = CSL_gpioGetPinmux(&gCSL_gpioHndl, 4, &value32);
		  if(status==OSA_SOK) {
		    value32 &= 0xFFCFFFF0; // clear bit 21,20,3, 2, 1, 0, for GIO 27, 28

		    status = CSL_gpioSetPinmux(&gCSL_gpioHndl, 4, value32);
		    OSA_printf("set pinmux4 = 0x%x\n", value32);
		  }
 #endif
  DRV_gpioSetMode(SPI1_CS_GPO, DRV_GPIO_DIR_OUT);
  DRV_gpioSetMode(SPI1_SCLK_GPO, DRV_GPIO_DIR_OUT);
  DRV_gpioSetMode(SPI1_SDO_GPO, DRV_GPIO_DIR_OUT);

 #else

	status = DRV_SPIOpen(&gDRV_imgsObj.spiHndl, 0);//devAddr unused in DRV_SPIOpen
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_spiOpen()\n");	
		return status;
	}

 #endif

	DB("############SET REG###########\n");	
	status = DRV_imgsSetRegs(width,  height, config->fps);
	
	gbIsDDRIn = config->bIsDdrIn;
	gbIsAlawEnalbe = config->bIsALawEnable;

	return 0;
}

int DRV_imgsClose()
{
	int status;
	//DRV_imgs_DarkframeRelease();
	status = DRV_imgsEnable(FALSE);

	return status;
}

char* DRV_imgsGetImagerName()
{
	return "SONY_IMX136_2_3MP";
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
    //case DRV_IMGS_SENSOR_MODE_1620x1080:

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

/*
   0 dB to 24 dB ：Analog Gain 24 dB (step pitch 0.1 dB),for 24db,that's 15.8---->16
24.3 dB to 42 dB ：Analog Gain 24 dB + Digital Gain 0.3 to 18 dB (step pitch 0.3 dB)
*/
/*
about again from 2A algothrim,times = again/1000;
*/
     double gain_times;
     double gain_db;
     unsigned int gain_reg;

//42db:125000
      if(again < 1000) again = 1000;
      if(again > 125000) again = 125000;

      gain_times = again/1000.0;
      gain_db = 200 * log10(gain_times);
      gain_reg = (unsigned int)(gain_db + 0.5);
      if (gain_reg > 420) gain_reg = 420;
      if (gain_reg < 0) gain_reg = 0;
 
     imx136_setgio(0x0214,(gain_reg&0xff)); 
     imx136_setgio(0x0215,((gain_reg>>8)&0xff));

#ifdef AEWB_DEBUG
      OSA_printf("gain value:%d\n",gain_reg);
#endif

 return 0;
}
//set the shutter start line,setRegDirect = 0
int DRV_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect)
{
    Uint32	SetValue,SUB_Control=0,tempdata;
    Uint8	Send_HH,Send_H,Send_L;

    DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;

    if(setRegDirect) {
        // eshutterInUsec in line
        SetValue = eshutterInUsec;
    } else  {
        // eshutterInUsec in Usec.
        DRV_imgsCalcSW(eshutterInUsec);
        SetValue = pFrame->SW;
    }

#ifdef AEWB_DEBUG
    OSA_printf("IMX136 ShutterInUsec:%d ; SetRegDirect:%d ; SetValue:%d \n",eshutterInUsec,setRegDirect,SetValue);
#endif

    if(SetValue<IMGS_SENSOR_LINEWIDTH){
        SUB_Control = IMGS_SENSOR_LINEWIDTH - SetValue; 
    }else{
        SUB_Control = 0;
    }
    Send_HH =(Uint8)(SUB_Control>>16);
    Send_H  =(Uint8)((SUB_Control>>8)&0x00FF);
    Send_L  =(Uint8)(SUB_Control & 0x00FF);

    imx136_setgio(0x0222,Send_HH); 
    imx136_setgio(0x0221,Send_H); 
    imx136_setgio(0x0220,Send_L); 

    /* long exposure ,need re-set VMAX*/
    if(SetValue<IMGS_SENSOR_LINEWIDTH){
        tempdata = IMGS_SENSOR_LINEWIDTH;
    }else{
        tempdata = SetValue;
    }

    Send_HH=(Uint8)(tempdata>>16);
    Send_H =(Uint8)((tempdata>>8)&0x00FF);
    Send_L =(Uint8)(tempdata & 0x00FF);

    imx136_setgio(0x021A,Send_HH); 
    imx136_setgio(0x0219,Send_H); 
    imx136_setgio(0x0218,Send_L);

    return 0;
}


int DRV_imgsSetRegs(Uint16 width, Uint16 height, int fps)
{

unsigned int status;

#ifdef IMX136_LVDS324
	unsigned char lvdsRegAddr[12];
	unsigned char lvdsRegValue[12];
	
	lvdsRegAddr[0]=0x00;//first chip id address
	lvdsRegAddr[1]=0x0b;//FRAME_WIDTH_MSB
	lvdsRegAddr[2]=0x0c;//FRAME_WIDTH_LSB
	lvdsRegAddr[3]=0x0d;//FRAME_HEIGHT_MSB
	lvdsRegAddr[4]=0x0e;//FRAME_HEIGHT_LSB
	//do
	//	{
        status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, lvdsRegAddr, lvdsRegValue, 5);
	//Iss_deviceRead8(gImx136Lvds_I2c_ctrl.i2cInstId,0x2d,lvdsRegAddr,lvdsRegValue, 5);
	OSA_printf("lvds324 chip id 1st:0x%x\n",lvdsRegValue[0]);
	OSA_printf("defaut width:0x%x,height:0x%x",(lvdsRegValue[1]<<8)|(lvdsRegValue[2]),(lvdsRegValue[3]<<8)|(lvdsRegValue[4]));
	//	}while(1);

        lvdsRegAddr[0]=0x09; lvdsRegValue[0]=0x21;
        //lvdsRegAddr[0]=0x09; lvdsRegValue[0]=0x31; /*for 0x11:output data lsb first;0x31:output msb first,both for  sony wide channel 12bpp*/
	lvdsRegAddr[1]=0x0A; lvdsRegValue[1]=0x62; /*0x62--->SCLK:58MHz~75MHz;OUTCLK:116~150MHz*/
						   /*0x61--->SCLK:31MHz~60MHz;OUTCLK:62~120MHz*/
        status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, lvdsRegAddr, lvdsRegValue, 2);
#endif

        /*for sensor register*/
        InitRegArrary();

return 0;
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
