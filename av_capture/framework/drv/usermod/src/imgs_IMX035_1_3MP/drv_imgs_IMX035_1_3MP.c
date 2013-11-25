
#include "drv_imgs_IMX035_1_3MP.h"
#include <drv_gpio.h>
#include <math.h>

#define __DRV_IMGS_IMX035_DEBUG__
#ifdef __DRV_IMGS_IMX035_DEBUG__
#define DB(fmt, args...) fprintf(stderr,fmt, ## args)
#else
#define DB(fmt, args...)
#endif
#define DBErr(fmt, args...) fprintf(stderr,fmt, ## args)

#define IMX035_AGAIN_IDXMAX	(61)
unsigned short IMX035_AGainTbl[IMX035_AGAIN_IDXMAX]=
{
0x1000,
0xf74,
0xeee,
0xe6c,
0xdef,
0xd76,
0xd01,
0xc90,
0xc23,
0xbb9,
0xb53,
0xaf1,
0xa92,
0xa36,
0x9dd,
0x987,
0x935,
0x8e4,
0x897,
0x84d,
0x804,
0x7bf,
0x77b,
0x73a,
0x6fb,
0x6bf,
0x684,
0x64b,
0x615,
0x5e0,
0x5ad,
0x57b,
0x54c,
0x51e,
0x4f1,
0x4c6,
0x49d,
0x475,
0x44e,
0x429,
0x404,
0x3e1,
0x3c0,
0x39f,
0x380,
0x361,
0x344,
0x327,
0x30c,
0x2f1,
0x2d8,
0x2bf,
0x2a7,
0x290,
0x27a,
0x264,
0x250,
0x23b,
0x228,
0x215,
0x203
};

DRV_ImgsObj gDRV_imgsObj;
Bool gbIsDDRIn = FALSE;
Bool gbIsAlawEnalbe = FALSE;
extern int Sensor_imgsSet50_60Hz(Bool is50Hz);
extern int Sensor_imgsSetAgain(int again, int setRegDirect);
extern int Sensor_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect, int SenSor_LineWidth);
extern int Sensor_imgsSetOB(int Level);
extern int Sensor_imgsSetRegs(void);

int DRV_imgsGetDdrInMode(void){
	return (int)gbIsDDRIn;
}

int DRV_imgsOpen(DRV_ImgsConfig *config)
{
	int status=0;
	Uint16 width, height;
	int Use_V_PAD=0;

	//DB("############ %s: OPEN ###########\n", __FUNCTION__ );

	memset(&gDRV_imgsObj, 0, sizeof(gDRV_imgsObj));

	DRV_imgGetWidthHeight(config->sensorMode, &width, &height);

	if ( width==1280 && height==736 ) {
		//printf ( " \n $$$$$$$$$$$$ DRV_imgsOpen720P $$$$$$$$$$$$\n\n" );
		Use_V_PAD = IMGS_V_PAD + 112;	//	(+112;  for 1280x720 vertical center)
	} else if ( width==1280 && height==960 ) {
		//printf ( " \n $$$$$$$$$$$$ DRV_imgsOpenD1 $$$$$$$$$$$$\n\n" );
		Use_V_PAD = IMGS_V_PAD + 22;	//	(+112;  for 1280x720 vertical center)
	} else if ( width==1280 && height==864 ) {
		//printf ( " \n $$$$$$$$$$$$ DRV_imgsOpenD1 $$$$$$$$$$$$\n\n" );
		Use_V_PAD = IMGS_V_PAD + 68;	//	(+112;  for 1280x720 vertical center)
	}else{
		Use_V_PAD = IMGS_V_PAD + 0;	//	(+112;  for 1280x720 vertical center)
	}
	
	width+=IMGS_H_PAD;
	height+=Use_V_PAD;


	#if (0)  
	DRV_imgsCalcFrameTime(config->fps, width, height, config->binEnable);
	#else
	//DRV_imgsCalcFrameTime(config->fps, width, height, config->binEnable);
	gDRV_imgsObj.curModeConfig.sensorDataWidth   = width;
	gDRV_imgsObj.curModeConfig.sensorDataHeight  = height;
	gDRV_imgsObj.curModeConfig.validStartX       = IMGS_H_PAD/2;
	gDRV_imgsObj.curModeConfig.validStartY       = Use_V_PAD;    
	gDRV_imgsObj.curModeConfig.validWidth        = width-IMGS_H_PAD;      
	gDRV_imgsObj.curModeConfig.validHeight       = height-Use_V_PAD;        
	gDRV_imgsObj.curModeConfig.binEnable         = 0;

	gDRV_imgsObj.curModeConfig.vdint0 = 1000;	//800;
	gDRV_imgsObj.curModeConfig.vdint1 = 150;
	gDRV_imgsObj.curModeConfig.vdint2 = 0;
	#endif
	//DB("############reset###########\n");
	/* Sensor reset */
	DRV_gpioSetMode(IMGS_RESET_GPIO, DRV_GPIO_DIR_OUT);
	DRV_gpioSet(IMGS_RESET_GPIO);  
	DRV_gpioClr(IMGS_RESET_GPIO);
	OSA_waitMsecs(50);
	DRV_gpioSet(IMGS_RESET_GPIO);  
	OSA_waitMsecs(50);  
	//DB("############SPI OPEN###########\n");	
	status = DRV_SPIOpen(&gDRV_imgsObj.spiHndl, IMGS_SPI_ADDR);
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_spiOpen()\n");
		return status;
	}
	//DB("############SET REG###########\n");	
	status = DRV_imgsSetRegs();	//width,  height, config->fps
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_imgsCheckId()\n");
		DRV_SPIClose(&gDRV_imgsObj.spiHndl);    
		return status;
	}
	//DB("############OPEN END###########\n");	
	gbIsDDRIn = config->bIsDdrIn;
	gbIsAlawEnalbe = config->bIsALawEnable;

	return 0;
}

int DRV_imgsClose()
{
	int status;

	//printf ( " ~~~ %s ~~~ \n", __FUNCTION__ );

	//DRV_imgs_DarkframeRelease();
	status = DRV_imgsEnable(FALSE);
	status |= DRV_SPIClose(&gDRV_imgsObj.spiHndl);

	return status;
}

char* DRV_imgsGetImagerName()
{
	return "SONY_IMX035_1_3MP";
}

int DRV_imgsSpecificSetting(void)
{
	return OSA_SOK;
}


int DRV_imgsSet50_60Hz(Bool is50Hz)
{
	return Sensor_imgsSet50_60Hz(is50Hz);
}

int DRV_imgsSetFramerate(int fps)
{
	//printf(" !!!!!! Fn: %s() Not ready !!!!!! \n", __FUNCTION__ );

	return OSA_SOK;
}

int DRV_imgsBinEnable(Bool enable)
{
	//printf(" !!!!!! Fn: %s() Not ready !!!!!! \n", __FUNCTION__ );

	return OSA_SOK;
}

int DRV_imgsBinMode(int binMode)
{
	//printf(" !!!!!! Fn: %s() Not ready !!!!!! \n", __FUNCTION__ );

	return OSA_SOK;
}

//return value is 8 times the true analog gain
int DRV_imgsGetAgain(int *again)
{
	//printf(" !!!!!! Fn: %s() Not ready !!!!!! \n", __FUNCTION__ );

	return OSA_SOK;
}

int DRV_imgsGetEshutter(Uint32 *eshutterInUsec)
{
	//printf(" !!!!!! Fn: %s() Not ready !!!!!! \n", __FUNCTION__ );

	return OSA_SOK;
}

/*
int DRV_imgsSetAgain(int again, int setRegDirect)
{
printf("again %d------\n",again);
	return Sensor_imgsSetAgain(again,setRegDirect); 
}
*/

int DRV_imgsSetDgain(int dgain)
{
    return OSA_SOK;
}

int DRV_imgsSetAgain(int again, int setRegDirect)
{
     //IMGS_SPI_ADDR of IMX035:0x81

    	int	status = OSA_SOK;
	#define IMX035_REG00_SET(X,Y) count=0;regset[count++]=IMGS_SPI_ADDR;regset[count++]=X;regset[count++]=Y;DRV_SPIWrite8(&gDRV_imgsObj.spiHndl, regset,count);
	#define IMX035_REGSET3(X,V1,V2) count=0;regset[count++]=IMGS_SPI_ADDR;regset[count++]=X;regset[count++]=V1;regset[count++]=V2;DRV_SPIWrite8(&gDRV_imgsObj.spiHndl, regset,count);
	int count=0;
	Uint8 regset[5];
	int SetIdx;
	//unsigned long tmp;
	unsigned short SetVal1,SetVal2;
	#define MAX(a, b)   (((a)>(b))? (a):(b))
	#define MIN(a, b)   (((a)<(b))? (a):(b))
	
	if(again<1000 || again>128000){
		DBErr("XXXXX DRV_imgsSetAgain() Error(%d)\n",again);
		//status = OSA_EFAIL;
		return status;
	}

      	if(again>16000){
		SetIdx = IMX035_AGAIN_IDXMAX-1;
		SetVal1 = IMX035_AGainTbl[SetIdx];
		SetVal2 = (200*log((double)again/16000)/(3*log(10)));
	}else{
		SetIdx = (200*(log((double)again/1000))/(3*log(10)));
		SetIdx = MIN(SetIdx,IMX035_AGAIN_IDXMAX-1);
		SetVal1 = IMX035_AGainTbl[SetIdx];
		SetVal2 = 0;
	}
      //printf("IMX035 Analog GAIN:%d\n",SetVal1);
      //AGAIN for IMX035,address:0X1C 0X1D
       IMX035_REGSET3(0x1c,(SetVal1&0xFF),((SetVal1>>8)&0x1F));

    return status;
}


int DRV_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect)
{
       
	return Sensor_imgsSetEshutter(eshutterInUsec,setRegDirect,IMGS_SENSOR_LINEWIDTH);
}

/*
int DRV_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect)
{
	int	status = OSA_SOK;
	Uint32	SetValue,SUB_Control=0,tempdata;
	Uint8	Send_HH,Send_H,Send_L;
	DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;
	#define IMX035_REG00_SET(X,Y) count=0;regset[count++]=IMGS_SPI_ADDR;regset[count++]=X;regset[count++]=Y;DRV_SPIWrite8(&gDRV_imgsObj.spiHndl, regset,count);
	#define IMX035_REGSET3(X,V1,V2) count=0;regset[count++]=IMGS_SPI_ADDR;regset[count++]=X;regset[count++]=V1;regset[count++]=V2;DRV_SPIWrite8(&gDRV_imgsObj.spiHndl, regset,count);
	int count=0;
	Uint8 regset[5];

	if(eshutterInUsec==0){
		printf("STError:\t%d\n",eshutterInUsec);
		return status;
	}
		
	if(setRegDirect) {
		// eshutterInUsec in line
		SetValue = eshutterInUsec;
	} else  {
		// eshutterInUsec in Usec.
		DRV_imgsCalcSW(eshutterInUsec);
		SetValue = pFrame->SW;
	}

{
	
	if(SetValue<IMGS_SENSOR_LINEWIDTH){
		SUB_Control = IMGS_SENSOR_LINEWIDTH - SetValue; 
	}else{
		SUB_Control = 0;
	}
//        SUB_Control = 1100;//test under low light
//printf("sub_control,ST(%d):\t%d\n",setRegDirect,SUB_Control);
	Send_H=(Uint8)((SUB_Control>>8)&0x00FF);
	Send_L=(Uint8)(SUB_Control & 0x00FF);
	IMX035_REGSET3(0x08,Send_L,Send_H);		
	}

	return status;
}
*/
int DRV_imgsSetOB(int Level)
{
	return Sensor_imgsSetOB(Level);
}

int DRV_imgsSetDcSub(Uint32 dcSub, int setRegDirect)
{
	//printf(" !!!!!! Fn: %s() Not ready !!!!!! \n", __FUNCTION__ );

	return OSA_SOK;
}

int DRV_imgsEnable(Bool enable)
{
	//printf(" !!!!!! Fn: %s() Not ready !!!!!! \n", __FUNCTION__ );

	return OSA_SOK;
}

int DRV_imgsGetLightSensorMode(void)
{
	/* return Low light:1, Normal:0 */
	/* LS:Normal 1, low light 0 */
	static int is1stTime=1;
	static int PreVal = -1;//,PreVal2;
	if(is1stTime){
		DRV_gpioSetMode(IMGS_LIGHT_SENSOR, DRV_GPIO_DIR_IN);
		DRV_gpioSetMode(IMGS_LIGHT_SENSOR_LINK, DRV_GPIO_DIR_IN);		
		is1stTime = 0;
	}

	PreVal = !DRV_gpioGet(IMGS_LIGHT_SENSOR);
	//PreVal2 = DRV_gpioGet(IMGS_LIGHT_SENSOR_LINK);
	//DBErr("LS:%d\n",PreVal);
	return PreVal;
}

int DRV_imgs_IsDDRIn(void){
	return gbIsDDRIn;
}

int DRV_imgs_IsAlawEnable(void){
	return gbIsAlawEnalbe;
}

int DRV_imgsNDShutter(int bIsNight)
{
	int status=0;
	static int FirstTimeIn=1;
	static int Pre_Mode=-1;

	if(Pre_Mode==bIsNight){
		return (0);
	}
	Pre_Mode = bIsNight;
	
	if(FirstTimeIn){
		DRV_gpioSetMode(IMGS_ND_CTRL1, DRV_GPIO_DIR_OUT);
		DRV_gpioSetMode(IMGS_ND_CTRL2, DRV_GPIO_DIR_OUT);
		/* Day mode. (default) */
		DRV_gpioSet(IMGS_ND_CTRL2); 
		OSA_waitMsecs(30); // 20ms   
		DRV_gpioClr(IMGS_ND_CTRL2);	
		OSA_waitMsecs(30); // 20ms   

		DRV_gpioClr(IMGS_ND_CTRL1);
		DRV_gpioClr(IMGS_ND_CTRL2);
		FirstTimeIn = 0;
		DBErr("ND Init\n");
	}else{
		DRV_gpioSetMode(IMGS_ND_CTRL1, DRV_GPIO_DIR_OUT);
		DRV_gpioSetMode(IMGS_ND_CTRL2, DRV_GPIO_DIR_OUT);
	}
	
	if(bIsNight){
		/* Night mode. */
		DBErr("ND:Night\n");
		DRV_gpioSet(IMGS_ND_CTRL1);
		OSA_waitMsecs(30); // 20ms
		DRV_gpioClr(IMGS_ND_CTRL1);
	}else{
		/* Day mode. */
		DBErr("ND:Day\n");
		DRV_gpioSet(IMGS_ND_CTRL2); 
		OSA_waitMsecs(30); // 20ms   
		DRV_gpioClr(IMGS_ND_CTRL2);	
	}	
	return status; 	
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

int DRV_imgsSetRegs()
{
	return Sensor_imgsSetRegs(); 
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
