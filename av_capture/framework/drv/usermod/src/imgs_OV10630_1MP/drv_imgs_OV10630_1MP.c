
#include "drv_imgs_OV10630_1MP.h"
#include <drv_gpio.h>

DRV_ImgsObj gDRV_imgsObj;

int DRV_imgsOpen(DRV_ImgsConfig *config)
{
  int status, retry=10;
  Uint16 width, height;

  memset(&gDRV_imgsObj, 0, sizeof(gDRV_imgsObj));

  DRV_imgGetWidthHeight(config->sensorMode, &width, &height);

  width+=IMGS_H_PAD;
  height+=IMGS_V_PAD;

  DRV_imgsCalcFrameTime(config->fps, width, height, config->binEnable, config->flipH, config->flipV);

  status = DRV_i2cOpen(&gDRV_imgsObj.i2cHndl, IMGS_I2C_ADDR);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cOpen()\n");
    return status;
  }
#if 1
  #ifdef BOARD_AP_IPNC
  DRV_gpioSetMode(IMGS_RESET_GPIO, DRV_GPIO_DIR_OUT);
  DRV_gpioSet(IMGS_RESET_GPIO);
  DRV_gpioClr(IMGS_RESET_GPIO);
  OSA_waitMsecs(100);
  DRV_gpioSet(IMGS_RESET_GPIO);
  OSA_waitMsecs(100);
  #endif
#endif


  do {
    status = DRV_imgsCheckId();
    if(status==OSA_SOK)
      break;
    OSA_waitMsecs(10);
  } while(retry--);

  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_imgsCheckId()\n");
    DRV_i2cClose(&gDRV_imgsObj.i2cHndl);
    return status;
  }

  return 0;
}

int DRV_imgsClose()
{
  int status;

  status = DRV_imgsEnable(FALSE);
  status |= DRV_i2cClose(&gDRV_imgsObj.i2cHndl);

  return status;
}

char* DRV_imgsGetImagerName()
{
  return "OMNIVISION_OV2715_2MP";
}

int DRV_imgsSpecificSetting(void)
{	
	return 0;     
}


int DRV_imgsSet50_60Hz(Bool is50Hz)
{
  int fps;

	int status,i;
	Uint16 regAddr[10];

	Uint8 regValueSend[10]; 

return 0;

	if( is50Hz )
	{
		i = 0;
		regAddr[i] = TIMING_CONTROL_HTS_HIGHBYTE; 
		regValueSend[i] = 0x0B; 		i++;
		regAddr[i] = TIMING_CONTROL_HTS_LOWBYTE; 
		regValueSend[i] = 0x58; 		i++;
	}else{
		i = 0;
		regAddr[i] = TIMING_CONTROL_HTS_HIGHBYTE; 
		regValueSend[i] = 0x09; 		i++;
		regAddr[i] = TIMING_CONTROL_HTS_LOWBYTE; 
		regValueSend[i] = 0x74; 		i++;
	}

	status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValueSend, i);	
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_i2cWrite16()\n");
		return status;
	} 
	return status;


//  if( gDRV_imgsObj.curFrameTime.fps==30
//    ||gDRV_imgsObj.curFrameTime.fps==25
//    ) {
//
//    if(is50Hz)
//      fps = 25;
//    else
//      fps = 30;
//
//    DRV_imgsSetFramerate(fps);
//  }
  return 0;
}

int DRV_imgsSetFramerate(int fps)
{

	return 0;

}

int DRV_imgsBinEnable(Bool enable)
{

		return 0;


}

int DRV_imgsBinMode(int binMode)
{

		return 0;


}


int DRV_imgsSetDgain(int dgain)
{
    return OSA_SOK;
}

int DRV_imgsSetAgain(int again, int setRegDirect)
{

  Uint16 regAddr[8];
  Uint8 regValueSend[8]; 
  Uint16 regValue;
  int status,i;
  
return 0;

  if(setRegDirect) {
    regValue = again;
  } else {
    regValue = DRV_imgsCalcAgain(again);
  }

  i = 0;

	//regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0x01; 		i++;
  
  regAddr[i] = AEC_AGC_ADJ_L; regValueSend[i] = (regValue)&0xff; 	i++;
  regAddr[i] = AEC_AGC_ADJ_H; regValueSend[i] = 0; 								i++;

  //regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0x11; 		i++;
  //regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0xA1; 		i++;

  status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValueSend, i);

  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }

  return status;


}

int DRV_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect)
{

  int status,i;
  Uint16 regAddr[20];
  int regValue=0; 
  Uint8 regValueSend[20]; 
  int regValue2=0; 
  
return 0;
  
  if(setRegDirect) {
    regValue = eshutterInUsec;
  } else  {
    DRV_imgsCalcSW(eshutterInUsec);  
    regValue = gDRV_imgsObj.curFrameTime.SW;
    regValue2 = regValue/16;
    if( regValue2 > 0x44a )
    {
	regValue2 = regValue2 - 0x44a;
    }else{
	regValue2 = 0;
    }
  }
  i = 0;
  regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0x00; 		i++;

//  regAddr[i] = PLL_CTRL01; regValueSend[i] = gDRV_imgsObj.curFrameTime.div_base;	i++;
  regAddr[i] = AEC_PK_EXPO1; regValueSend[i] = (regValue>>16)&0xff;	i++;
  regAddr[i] = AEC_PK_EXPO2; regValueSend[i] = (regValue>>8)&0xff; 	i++;  
  regAddr[i] = AEC_PK_EXPO3; regValueSend[i] = (regValue)&0xff; 		i++;

  regAddr[i] = AEC_PK_VTS_H; regValueSend[i] = (regValue2>>8)&0xff; 		i++;
  regAddr[i] = AEC_PK_VTS_L; regValueSend[i] = (regValue2)&0xff; 		i++;


  regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0x10; 		i++;
  regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0xA0; 		i++;


  status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValueSend, i);  
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  } 
  return status;


}

#if 1 //Gang: read AGain & exposure
//return value is 8 times the true analog gain
int DRV_imgsGetAgain(int *again)
{
#if 0
  Uint8 regAddr;
  Uint16 regValue;
  int status;

  regAddr = GAIN_G1;

  status = DRV_i2cRead16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cRead16()\n");
    return status;
  }

  *again = regValue & 0x3f;

  if (regValue & 0x40)
  {
    *again <<= 1;
  }

  return status;
#else
	return 0;
#endif
}

int DRV_imgsGetEshutter(Uint32 *eshutterInUsec)
{
#if 0

  int status;
  Uint8 regAddr;
  Uint16 regValue;

  regAddr  = SHUTTER_WIDTH_L;

  status = DRV_i2cRead16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }

  *eshutterInUsec = DRV_imgsInvertSW(regValue);

  return status;
#else
	return 0;
#endif
}
#endif

int DRV_imgsSetDcSub(Uint32 dcSub, int setRegDirect)
{

  return 0;
}

int DRV_imgsEnable(Bool enable)
{
	int status=OSA_SOK;
#if 0 	
  Uint8 regAddr;
  Uint16 regValue;
  

  DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;

  if(enable) {
    status = DRV_imgsSetRegs();
    if(status!=OSA_SOK) {
      OSA_ERROR("DRV_imgsSetRegs()\n");
      return status;
    }
    DRV_imgsSpecificSetting();
  }
  if( (pFrame->f_pclk == (96*IMGS_MHZ))&&( pFrame->row_bin > 0 || pFrame->col_bin > 0))
  {
	  regAddr  = OUTPUT_CTRL;
	  regValue = 0x1f8C | ((enable & 0x1) << 1);
  }else{
	  regAddr  = OUTPUT_CTRL;
	  regValue = 0x1f80 | ((enable & 0x1) << 1);
  }

  status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }
#else
	if(enable) {
    status = DRV_imgsSetRegs(); 
  }

#endif
  OSA_waitMsecs(10);
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
#ifdef IMGS_OV10630_1MP
//    case DRV_IMGS_SENSOR_MODE_752x512:
    case DRV_IMGS_SENSOR_MODE_720x480:
//    case DRV_IMGS_SENSOR_MODE_1620x1080:			//D1
#else
    case DRV_IMGS_SENSOR_MODE_752x512:
    case DRV_IMGS_SENSOR_MODE_720x480:
    case DRV_IMGS_SENSOR_MODE_1620x1080:			//D1
#endif
      if(ldcInFrameWidth==864)
        return &gDRV_imgsLdcConfig_736x480_0_VS;

      if(ldcInFrameWidth==352)
        return &gDRV_imgsLdcConfig_736x480_1_VS;

      if(ldcInFrameWidth==736)
        return &gDRV_imgsLdcConfig_736x480_0;
        
      if(ldcInFrameWidth==752)
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
    case DRV_IMGS_SENSOR_MODE_1920x1080:			//720P / 1080P

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
  Uint16 regAddr[200];
  Uint8 regValue[200];
  int status, i;

	//注释掉的是1280x720的，用了1280x800取代它

	memset(regAddr, 0, 200*sizeof(Uint16));
	memset(regValue, 0, 200*sizeof(Uint8));
	i=0;  
	regAddr[i] = OV10630_REG_0103;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_301b;regValue[i] = 0xff;  i++;
	regAddr[i] = OV10630_REG_301c;regValue[i] = 0xff;  i++;
	regAddr[i] = OV10630_REG_301a;regValue[i] = 0xff;  i++;
	//regAddr[i] = OV10630_REG_300c;regValue[i] = 0x61;  i++;
	regAddr[i] = OV10630_REG_3021;regValue[i] = 0x03;  i++;
	regAddr[i] = OV10630_REG_3011;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_6900;regValue[i] = 0x0c;  i++;
	regAddr[i] = OV10630_REG_6901;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_3033;regValue[i] = 0x08;  i++;
	regAddr[i] = OV10630_REG_3503;regValue[i] = 0x10;  i++;
	regAddr[i] = OV10630_REG_302d;regValue[i] = 0x2f;  i++;
	regAddr[i] = OV10630_REG_3025;regValue[i] = 0x03;  i++;
	regAddr[i] = OV10630_REG_3003;regValue[i] = 0x20;  i++;//0x14
	regAddr[i] = OV10630_REG_3004;regValue[i] = 0x21;  i++;//0x13
	regAddr[i] = OV10630_REG_3005;regValue[i] = 0x20;  i++;
	regAddr[i] = OV10630_REG_3006;regValue[i] = 0x91;  i++;
	regAddr[i] = OV10630_REG_3600;regValue[i] = 0x74;  i++;
	regAddr[i] = OV10630_REG_3601;regValue[i] = 0x2b;  i++;
	regAddr[i] = OV10630_REG_3612;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_3611;regValue[i] = 0x67;  i++;
	regAddr[i] = OV10630_REG_3633;regValue[i] = 0xba;  i++;
	regAddr[i] = OV10630_REG_3602;regValue[i] = 0x2f;  i++;
	regAddr[i] = OV10630_REG_3603;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_3630;regValue[i] = 0xa8;  i++;
	regAddr[i] = OV10630_REG_3631;regValue[i] = 0x16;  i++;
	regAddr[i] = OV10630_REG_3714;regValue[i] = 0x10;  i++;
	regAddr[i] = OV10630_REG_371d;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_4300;regValue[i] = 0x38;  i++;//output yuv:ox38   raw:0xf8
	regAddr[i] = OV10630_REG_3007;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_3024;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_3020;regValue[i] = 0x0b;  i++;
	regAddr[i] = OV10630_REG_3702;regValue[i] = 0x10;  i++;//0x0d
	regAddr[i] = OV10630_REG_3703;regValue[i] = 0x24;  i++;//0x20
	regAddr[i] = OV10630_REG_3704;regValue[i] = 0x19;  i++;//0x15
	regAddr[i] = OV10630_REG_3709;regValue[i] = 0x28;  i++;
	regAddr[i] = OV10630_REG_370d;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_3712;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_3713;regValue[i] = 0x20;  i++;
	regAddr[i] = OV10630_REG_3715;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_381d;regValue[i] = 0x40;  i++;
	regAddr[i] = OV10630_REG_381c;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_3824;regValue[i] = 0x10;  i++;
	regAddr[i] = OV10630_REG_3815;regValue[i] = 0x8c;  i++;
	regAddr[i] = OV10630_REG_3804;regValue[i] = 0x05;  i++;
	regAddr[i] = OV10630_REG_3805;regValue[i] = 0x1f;  i++;
	regAddr[i] = OV10630_REG_3800;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_3801;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_3806;regValue[i] = 0x03;  i++;
	regAddr[i] = OV10630_REG_3807;regValue[i] = 0x29;  i++;//0x01
	regAddr[i] = OV10630_REG_3802;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_3803;regValue[i] = 0x06;  i++;//0x2e
	regAddr[i] = OV10630_REG_3808;regValue[i] = 0x05;  i++;
	regAddr[i] = OV10630_REG_3809;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_380a;regValue[i] = 0x03;  i++;//1280x800
	regAddr[i] = OV10630_REG_380b;regValue[i] = 0x20;  i++;
	regAddr[i] = OV10630_REG_380c;regValue[i] = 0x07;  i++;//0x06
	regAddr[i] = OV10630_REG_380d;regValue[i] = 0x71;  i++;//0xf6
	regAddr[i] = OV10630_REG_380e;regValue[i] = 0x03;  i++;//0x02
	regAddr[i] = OV10630_REG_380f;regValue[i] = 0x48;  i++;//0xec
	regAddr[i] = OV10630_REG_3811;regValue[i] = 0x08;  i++;
	regAddr[i] = OV10630_REG_381f;regValue[i] = 0x0c;  i++;
	regAddr[i] = OV10630_REG_3621;regValue[i] = 0x63;  i++;
	regAddr[i] = OV10630_REG_5005;regValue[i] = 0x08;  i++;
	regAddr[i] = OV10630_REG_56d5;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_56d6;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_56d7;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_56d8;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_56d9;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_56da;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_56db;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_56dc;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_56e8;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_56e9;regValue[i] = 0x7f;  i++;
	regAddr[i] = OV10630_REG_56ea;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_56eb;regValue[i] = 0x7f;  i++;
	regAddr[i] = OV10630_REG_5100;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5101;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_5102;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5103;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_5104;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5105;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_5106;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5107;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_5108;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5109;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_510a;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_510b;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_510c;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_510d;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_510e;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_510f;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5110;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5111;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_5112;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5113;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_5114;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5115;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_5116;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5117;regValue[i] = 0x80;  i++;

	status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);  
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_i2c16Write16()\n");
		printf("10630 set error\n");
		return status;
	}  

	memset(regAddr, 0, 200*sizeof(Uint16));
	memset(regValue, 0, 200*sizeof(Uint8));
	i = 0;
	regAddr[i] = OV10630_REG_5118;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5119;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_511a;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_511b;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_511c;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_511d;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_511e;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_511f;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_56d0;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5006;regValue[i] = 0x24;  i++;
	regAddr[i] = OV10630_REG_5608;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_52d7;regValue[i] = 0x06;  i++;
	regAddr[i] = OV10630_REG_528d;regValue[i] = 0x08;  i++;
	regAddr[i] = OV10630_REG_5293;regValue[i] = 0x12;  i++;
	regAddr[i] = OV10630_REG_52d3;regValue[i] = 0x12;  i++;
	regAddr[i] = OV10630_REG_5288;regValue[i] = 0x06;  i++;
	regAddr[i] = OV10630_REG_5289;regValue[i] = 0x20;  i++;
	regAddr[i] = OV10630_REG_52c8;regValue[i] = 0x06;  i++;
	regAddr[i] = OV10630_REG_52c9;regValue[i] = 0x20;  i++;
	regAddr[i] = OV10630_REG_52cd;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_5381;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5382;regValue[i] = 0xff;  i++;
	
	regAddr[i] = OV10630_REG_5300;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_5301;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5302;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5303;regValue[i] = 0x0e;  i++;
	regAddr[i] = OV10630_REG_5304;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5305;regValue[i] = 0x0e;  i++;
	regAddr[i] = OV10630_REG_5306;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5307;regValue[i] = 0x36;  i++;
	regAddr[i] = OV10630_REG_5308;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5309;regValue[i] = 0xd9;  i++;
	regAddr[i] = OV10630_REG_530a;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_530b;regValue[i] = 0x0f;  i++;
	regAddr[i] = OV10630_REG_530c;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_530d;regValue[i] = 0x2c;  i++;
	regAddr[i] = OV10630_REG_530e;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_530f;regValue[i] = 0x59;  i++;
	regAddr[i] = OV10630_REG_5310;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5311;regValue[i] = 0x7b;  i++;
	regAddr[i] = OV10630_REG_5312;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5313;regValue[i] = 0x22;  i++;
	regAddr[i] = OV10630_REG_5314;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5315;regValue[i] = 0xd5;  i++;
	regAddr[i] = OV10630_REG_5316;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5317;regValue[i] = 0x13;  i++;
	regAddr[i] = OV10630_REG_5318;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5319;regValue[i] = 0x18;  i++;
	regAddr[i] = OV10630_REG_531a;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_531b;regValue[i] = 0x26;  i++;
	regAddr[i] = OV10630_REG_531c;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_531d;regValue[i] = 0xdc;  i++;
	regAddr[i] = OV10630_REG_531e;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_531f;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_5320;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5321;regValue[i] = 0x24;  i++;
	regAddr[i] = OV10630_REG_5322;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5323;regValue[i] = 0x56;  i++;
	regAddr[i] = OV10630_REG_5324;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5325;regValue[i] = 0x85;  i++;
	regAddr[i] = OV10630_REG_5326;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5327;regValue[i] = 0x20;  i++;
	regAddr[i] = OV10630_REG_5609;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_560a;regValue[i] = 0x40;  i++;
	regAddr[i] = OV10630_REG_560b;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_560c;regValue[i] = 0x40;  i++;
	regAddr[i] = OV10630_REG_560d;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_560e;regValue[i] = 0xfa;  i++;
	regAddr[i] = OV10630_REG_560f;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5610;regValue[i] = 0xfa;  i++;
	regAddr[i] = OV10630_REG_5611;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_5612;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_5613;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_5614;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_5615;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_5616;regValue[i] = 0x2c;  i++;
	regAddr[i] = OV10630_REG_5617;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_5618;regValue[i] = 0x2c;  i++;
	regAddr[i] = OV10630_REG_563b;regValue[i] = 0x01;  i++;

	status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);  
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_i2c16Write16()\n");
		printf("10630 set error\n");
		return status;
	}  

	memset(regAddr, 0, 200*sizeof(Uint16));
	memset(regValue, 0, 200*sizeof(Uint8));
	i = 0;

	regAddr[i] = OV10630_REG_563c;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_563d;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_563e;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_563f;regValue[i] = 0x03;  i++;
	regAddr[i] = OV10630_REG_5640;regValue[i] = 0x03;  i++;
	regAddr[i] = OV10630_REG_5641;regValue[i] = 0x03;  i++;
	regAddr[i] = OV10630_REG_5642;regValue[i] = 0x05;  i++;
	regAddr[i] = OV10630_REG_5643;regValue[i] = 0x09;  i++;
	regAddr[i] = OV10630_REG_5644;regValue[i] = 0x05;  i++;
	regAddr[i] = OV10630_REG_5645;regValue[i] = 0x05;  i++;
	regAddr[i] = OV10630_REG_5646;regValue[i] = 0x05;  i++;
	regAddr[i] = OV10630_REG_5647;regValue[i] = 0x05;  i++;
	regAddr[i] = OV10630_REG_5651;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5652;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_521a;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_521b;regValue[i] = 0x03;  i++;
	regAddr[i] = OV10630_REG_521c;regValue[i] = 0x06;  i++;
	regAddr[i] = OV10630_REG_521d;regValue[i] = 0x0a;  i++;
	regAddr[i] = OV10630_REG_521e;regValue[i] = 0x0e;  i++;
	regAddr[i] = OV10630_REG_521f;regValue[i] = 0x12;  i++;
	regAddr[i] = OV10630_REG_5220;regValue[i] = 0x16;  i++;
	regAddr[i] = OV10630_REG_5223;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_5225;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_5227;regValue[i] = 0x08;  i++;
	regAddr[i] = OV10630_REG_5229;regValue[i] = 0x0c;  i++;
	regAddr[i] = OV10630_REG_522b;regValue[i] = 0x12;  i++;
	regAddr[i] = OV10630_REG_522d;regValue[i] = 0x18;  i++;
	regAddr[i] = OV10630_REG_522f;regValue[i] = 0x1e;  i++;
	regAddr[i] = OV10630_REG_5241;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_5242;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_5243;regValue[i] = 0x03;  i++;
	regAddr[i] = OV10630_REG_5244;regValue[i] = 0x06;  i++;
	regAddr[i] = OV10630_REG_5245;regValue[i] = 0x0a;  i++;
	regAddr[i] = OV10630_REG_5246;regValue[i] = 0x0e;  i++;
	regAddr[i] = OV10630_REG_5247;regValue[i] = 0x12;  i++;
	regAddr[i] = OV10630_REG_5248;regValue[i] = 0x16;  i++;
	regAddr[i] = OV10630_REG_524a;regValue[i] = 0x03;  i++;
	regAddr[i] = OV10630_REG_524c;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_524e;regValue[i] = 0x08;  i++;
	regAddr[i] = OV10630_REG_5250;regValue[i] = 0x0c;  i++;
	regAddr[i] = OV10630_REG_5252;regValue[i] = 0x12;  i++;
	regAddr[i] = OV10630_REG_5254;regValue[i] = 0x18;  i++;
	regAddr[i] = OV10630_REG_5256;regValue[i] = 0x1e;  i++;
	regAddr[i] = OV10630_REG_4605;regValue[i] = 0x08;  i++;
	regAddr[i] = OV10630_REG_4606;regValue[i] = 0x07;  i++;
	regAddr[i] = OV10630_REG_4607;regValue[i] = 0x71;  i++;
	regAddr[i] = OV10630_REG_460a;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_460b;regValue[i] = 0x70;  i++;
	regAddr[i] = OV10630_REG_460c;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_4620;regValue[i] = 0x0e;  i++;
	regAddr[i] = OV10630_REG_4700;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_4701;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_4702;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_4708;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_4004;regValue[i] = 0x08;  i++;
	regAddr[i] = OV10630_REG_4005;regValue[i] = 0x18;  i++;
	regAddr[i] = OV10630_REG_4001;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_4050;regValue[i] = 0x20;  i++;
	regAddr[i] = OV10630_REG_4051;regValue[i] = 0x22;  i++;
	regAddr[i] = OV10630_REG_4057;regValue[i] = 0x9c;  i++;
	regAddr[i] = OV10630_REG_405a;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_4202;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_3023;regValue[i] = 0x10;  i++;
	regAddr[i] = OV10630_REG_0100;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_6f0e;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_6f0f;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_460e;regValue[i] = 0x08;  i++;
	regAddr[i] = OV10630_REG_460f;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_4610;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_4611;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_4612;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_4613;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_4605;regValue[i] = 0x08;  i++;
	regAddr[i] = OV10630_REG_4608;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_4609;regValue[i] = 0x08;  i++;
	regAddr[i] = OV10630_REG_6804;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_6805;regValue[i] = 0x06;  i++;
	regAddr[i] = OV10630_REG_6806;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5120;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_3510;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_3504;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_6800;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_6f0d;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_5000;regValue[i] = 0xff;  i++;
	regAddr[i] = OV10630_REG_5001;regValue[i] = 0xbf;  i++;
	regAddr[i] = OV10630_REG_5002;regValue[i] = 0xfe;  i++;
	regAddr[i] = OV10630_REG_503d;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c450;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c452;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_c453;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c454;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c455;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c456;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c457;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c458;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c459;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c45b;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c45c;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c45d;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c45e;regValue[i] = 0x00;  i++;

	status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);  
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_i2c16Write16()\n");
		printf("10630 set error\n");
		return status;
	}  

	memset(regAddr, 0, 200*sizeof(Uint16));
	memset(regValue, 0, 200*sizeof(Uint8));
	i = 0;

	regAddr[i] = OV10630_REG_c45f;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c460;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c461;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c462;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c464;regValue[i] = 0x88;  i++;
	regAddr[i] = OV10630_REG_c465;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c466;regValue[i] = 0x8a;  i++;
	regAddr[i] = OV10630_REG_c467;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c468;regValue[i] = 0x86;  i++;
	regAddr[i] = OV10630_REG_c469;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c46a;regValue[i] = 0x40;  i++;
	regAddr[i] = OV10630_REG_c46b;regValue[i] = 0x50;  i++;
	regAddr[i] = OV10630_REG_c46c;regValue[i] = 0x30;  i++;
	regAddr[i] = OV10630_REG_c46d;regValue[i] = 0x28;  i++;
	regAddr[i] = OV10630_REG_c46e;regValue[i] = 0x60;  i++;
	regAddr[i] = OV10630_REG_c46f;regValue[i] = 0x40;  i++;
	regAddr[i] = OV10630_REG_c47c;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c47d;regValue[i] = 0x38;  i++;
	regAddr[i] = OV10630_REG_c47e;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c47f;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c480;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c481;regValue[i] = 0xff;  i++;
	regAddr[i] = OV10630_REG_c482;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c483;regValue[i] = 0x40;  i++;
	regAddr[i] = OV10630_REG_c484;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c485;regValue[i] = 0x18;  i++;
	regAddr[i] = OV10630_REG_c486;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c487;regValue[i] = 0x18;  i++;
	regAddr[i] = OV10630_REG_c488;regValue[i] = 0x34;  i++;//0x2e
	regAddr[i] = OV10630_REG_c489;regValue[i] = 0x70;  i++;//0x80
	regAddr[i] = OV10630_REG_c48a;regValue[i] = 0x34;  i++;//0x2e
	regAddr[i] = OV10630_REG_c48b;regValue[i] = 0x70;  i++;//0x80
	regAddr[i] = OV10630_REG_c48c;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c48d;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_c48e;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c48f;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_c490;regValue[i] = 0x07;  i++;
	regAddr[i] = OV10630_REG_c492;regValue[i] = 0x20;  i++;
	regAddr[i] = OV10630_REG_c493;regValue[i] = 0x08;  i++;
	regAddr[i] = OV10630_REG_c498;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_c499;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c49a;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_c49b;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c49c;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_c49d;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c49e;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_c49f;regValue[i] = 0x60;  i++;
	regAddr[i] = OV10630_REG_c4a0;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_c4a1;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c4a2;regValue[i] = 0x06;  i++;
	regAddr[i] = OV10630_REG_c4a3;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c4a4;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c4a5;regValue[i] = 0x10;  i++;
	regAddr[i] = OV10630_REG_c4a6;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c4a7;regValue[i] = 0x40;  i++;
	regAddr[i] = OV10630_REG_c4a8;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c4a9;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_c4aa;regValue[i] = 0x0d;  i++;
	regAddr[i] = OV10630_REG_c4ab;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c4ac;regValue[i] = 0x0f;  i++;
	regAddr[i] = OV10630_REG_c4ad;regValue[i] = 0xc0;  i++;
	regAddr[i] = OV10630_REG_c4b4;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c4b5;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c4b6;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c4b7;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c4ba;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c4bb;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c4be;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_c4bf;regValue[i] = 0x33;  i++;
	regAddr[i] = OV10630_REG_c4c8;regValue[i] = 0x03;  i++;
	regAddr[i] = OV10630_REG_c4c9;regValue[i] = 0xd0;  i++;
	regAddr[i] = OV10630_REG_c4ca;regValue[i] = 0x0e;  i++;
	regAddr[i] = OV10630_REG_c4cb;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c4cc;regValue[i] = 0x10;  i++;//0x0e
	regAddr[i] = OV10630_REG_c4cd;regValue[i] = 0x18;  i++;//0x51
	regAddr[i] = OV10630_REG_c4ce;regValue[i] = 0x10;  i++;//0x0e
	regAddr[i] = OV10630_REG_c4cf;regValue[i] = 0x18;  i++;//0x51
	regAddr[i] = OV10630_REG_c4d0;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_c4d1;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_c4e0;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_c4e1;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_c4e2;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c4e4;regValue[i] = 0x10;  i++;
	regAddr[i] = OV10630_REG_c4e5;regValue[i] = 0x20;  i++;
	regAddr[i] = OV10630_REG_c4e6;regValue[i] = 0x30;  i++;
	regAddr[i] = OV10630_REG_c4e7;regValue[i] = 0x40;  i++;
	regAddr[i] = OV10630_REG_c4e8;regValue[i] = 0x50;  i++;
	regAddr[i] = OV10630_REG_c4e9;regValue[i] = 0x60;  i++;
	regAddr[i] = OV10630_REG_c4ea;regValue[i] = 0x70;  i++;
	regAddr[i] = OV10630_REG_c4eb;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_c4ec;regValue[i] = 0x90;  i++;
	regAddr[i] = OV10630_REG_c4ed;regValue[i] = 0xa0;  i++;
	regAddr[i] = OV10630_REG_c4ee;regValue[i] = 0xb0;  i++;
	regAddr[i] = OV10630_REG_c4ef;regValue[i] = 0xc0;  i++;
	regAddr[i] = OV10630_REG_c4f0;regValue[i] = 0xd0;  i++;
	regAddr[i] = OV10630_REG_c4f1;regValue[i] = 0xe0;  i++;
	regAddr[i] = OV10630_REG_c4f2;regValue[i] = 0xf0;  i++;
	regAddr[i] = OV10630_REG_c4f3;regValue[i] = 0x80;  i++;

	status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);  
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_i2c16Write16()\n");
		printf("10630 set error\n");
		return status;
	}  

	memset(regAddr, 0, 200*sizeof(Uint16));
	memset(regValue, 0, 200*sizeof(Uint8));
	i = 0;

	regAddr[i] = OV10630_REG_c4f4;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c4f5;regValue[i] = 0x20;  i++;
	regAddr[i] = OV10630_REG_c4f6;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_c4f7;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c4f8;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_c4f9;regValue[i] = 0x0b;  i++;
	regAddr[i] = OV10630_REG_c4fa;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c4fb;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c4fc;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c4fd;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c4fe;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_c4ff;regValue[i] = 0x02;  i++;
	regAddr[i] = OV10630_REG_c500;regValue[i] = 0x68;  i++;
	regAddr[i] = OV10630_REG_c501;regValue[i] = 0x74;  i++;
	regAddr[i] = OV10630_REG_c502;regValue[i] = 0x70;  i++;
	regAddr[i] = OV10630_REG_c503;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_c504;regValue[i] = 0x05;  i++;
	regAddr[i] = OV10630_REG_c505;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_c506;regValue[i] = 0x03;  i++;
	regAddr[i] = OV10630_REG_c507;regValue[i] = 0x80;  i++;
	regAddr[i] = OV10630_REG_c508;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c509;regValue[i] = 0xc0;  i++;
	regAddr[i] = OV10630_REG_c50a;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c50b;regValue[i] = 0xa0;  i++;
	regAddr[i] = OV10630_REG_c50c;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c50d;regValue[i] = 0x2c;  i++;
	regAddr[i] = OV10630_REG_c50e;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c50f;regValue[i] = 0x0a;  i++;
	regAddr[i] = OV10630_REG_c510;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c511;regValue[i] = 0x01;  i++;//0x00
	regAddr[i] = OV10630_REG_c512;regValue[i] = 0x01;  i++;//0xe5
	regAddr[i] = OV10630_REG_c513;regValue[i] = 0x80;  i++;//0x14
	regAddr[i] = OV10630_REG_c514;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_c515;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c518;regValue[i] = 0x03;  i++;
	regAddr[i] = OV10630_REG_c519;regValue[i] = 0x48;  i++;
	regAddr[i] = OV10630_REG_c51a;regValue[i] = 0x07;  i++;
	regAddr[i] = OV10630_REG_c51b;regValue[i] = 0x70;  i++;
	regAddr[i] = OV10630_REG_c2e0;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c2e1;regValue[i] = 0x51;  i++;
	regAddr[i] = OV10630_REG_c2e2;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c2e3;regValue[i] = 0xd6;  i++;
	regAddr[i] = OV10630_REG_c2e4;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c2e5;regValue[i] = 0x5e;  i++;
	regAddr[i] = OV10630_REG_c2e9;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c2ea;regValue[i] = 0x7a;  i++;
	regAddr[i] = OV10630_REG_c2eb;regValue[i] = 0x90;  i++;
	regAddr[i] = OV10630_REG_c2ed;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c2ee;regValue[i] = 0x7a;  i++;
	regAddr[i] = OV10630_REG_c2ef;regValue[i] = 0x64;  i++;
	regAddr[i] = OV10630_REG_c308;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c309;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c30a;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c30c;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c30d;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c30e;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c30f;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c310;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c311;regValue[i] = 0x60;  i++;
	regAddr[i] = OV10630_REG_c312;regValue[i] = 0xff;  i++;
	regAddr[i] = OV10630_REG_c313;regValue[i] = 0x08;  i++;
	regAddr[i] = OV10630_REG_c314;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c315;regValue[i] = 0x7f;  i++;
	regAddr[i] = OV10630_REG_c316;regValue[i] = 0xff;  i++;
	regAddr[i] = OV10630_REG_c317;regValue[i] = 0x0b;  i++;
	regAddr[i] = OV10630_REG_c318;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c319;regValue[i] = 0x0c;  i++;
	regAddr[i] = OV10630_REG_c31a;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c31b;regValue[i] = 0xe0;  i++;
	regAddr[i] = OV10630_REG_c31c;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_c31d;regValue[i] = 0x14;  i++;
	

	status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);  
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_i2c16Write16()\n");
		printf("10630 set error\n");
		return status;
	}  
	
 memset(regAddr, 0, 200*sizeof(Uint16));  //Richard Added to update AWB Matrix
 memset(regValue, 0, 200*sizeof(Uint8));
 i = 0;

 regAddr[i] = OV10630_REG_5580;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_5581;regValue[i] = 0x52;  i++;
 regAddr[i] = OV10630_REG_5582;regValue[i] = 0x11;  i++;
 regAddr[i] = OV10630_REG_5583;regValue[i] = 0x10;  i++;
 regAddr[i] = OV10630_REG_5584;regValue[i] = 0x25;  i++;
 regAddr[i] = OV10630_REG_5585;regValue[i] = 0x24;  i++;
 regAddr[i] = OV10630_REG_5586;regValue[i] = 0x0d;  i++;
 regAddr[i] = OV10630_REG_5587;regValue[i] = 0x10;  i++;
 regAddr[i] = OV10630_REG_5588;regValue[i] = 0x0b;  i++;
 regAddr[i] = OV10630_REG_5589;regValue[i] = 0x74;  i++;
 regAddr[i] = OV10630_REG_558a;regValue[i] = 0x63;  i++;
 regAddr[i] = OV10630_REG_558b;regValue[i] = 0xa5;  i++;
 regAddr[i] = OV10630_REG_558c;regValue[i] = 0x96;  i++;
 regAddr[i] = OV10630_REG_558d;regValue[i] = 0x4f;  i++;
 regAddr[i] = OV10630_REG_558e;regValue[i] = 0x31;  i++;
 regAddr[i] = OV10630_REG_558f;regValue[i] = 0x65;  i++;
 regAddr[i] = OV10630_REG_5590;regValue[i] = 0x51;  i++;
 regAddr[i] = OV10630_REG_5591;regValue[i] = 0xf0;  i++;
 regAddr[i] = OV10630_REG_5592;regValue[i] = 0x10;  i++;
 regAddr[i] = OV10630_REG_5593;regValue[i] = 0xf0;  i++;
 regAddr[i] = OV10630_REG_5594;regValue[i] = 0xf0;  i++;
 regAddr[i] = OV10630_REG_5595;regValue[i] = 0xf0;  i++;
 regAddr[i] = OV10630_REG_5596;regValue[i] = 0x03;  i++;
 regAddr[i] = OV10630_REG_5597;regValue[i] = 0x02;  i++;
 regAddr[i] = OV10630_REG_559e;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_559f;regValue[i] = 0x10;  i++;
 regAddr[i] = OV10630_REG_55a0;regValue[i] = 0x10;  i++;
 regAddr[i] = OV10630_REG_55a1;regValue[i] = 0x10;  i++;
 regAddr[i] = OV10630_REG_55a2;regValue[i] = 0x76;  i++;
 regAddr[i] = OV10630_REG_55a3;regValue[i] = 0x61;  i++;
 regAddr[i] = OV10630_REG_55a4;regValue[i] = 0xb2;  i++;
 regAddr[i] = OV10630_REG_55a5;regValue[i] = 0xb9;  i++;
 regAddr[i] = OV10630_REG_55a6;regValue[i] = 0x38;  i++;
 regAddr[i] = OV10630_REG_55a7;regValue[i] = 0x36;  i++;
 regAddr[i] = OV10630_REG_55a8;regValue[i] = 0x64;  i++;
 regAddr[i] = OV10630_REG_55a9;regValue[i] = 0x4e;  i++;
 regAddr[i] = OV10630_REG_55aa;regValue[i] = 0xf0;  i++;
 regAddr[i] = OV10630_REG_55ab;regValue[i] = 0x04;  i++;
 regAddr[i] = OV10630_REG_55ac;regValue[i] = 0xf0;  i++;
 regAddr[i] = OV10630_REG_55ad;regValue[i] = 0xf0;  i++;
 regAddr[i] = OV10630_REG_55ae;regValue[i] = 0xf0;  i++;
 regAddr[i] = OV10630_REG_c4b8;regValue[i] = 0x01;  i++;
 regAddr[i] = OV10630_REG_c4b9;regValue[i] = 0x01;  i++;
 regAddr[i] = OV10630_REG_c31e;regValue[i] = 0x00;  i++; //Richard: starting color matrix
 regAddr[i] = OV10630_REG_c31f;regValue[i] = 0xde;  i++;
 regAddr[i] = OV10630_REG_c320;regValue[i] = 0xFF;  i++;
 regAddr[i] = OV10630_REG_c321;regValue[i] = 0x39;  i++;
 regAddr[i] = OV10630_REG_c322;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_c323;regValue[i] = 0xea;  i++;
 regAddr[i] = OV10630_REG_c324;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_c325;regValue[i] = 0xe3;  i++;
 regAddr[i] = OV10630_REG_c326;regValue[i] = 0x00;  i++;
 regAddr[i] = OV10630_REG_c327;regValue[i] = 0x52;  i++;
 regAddr[i] = OV10630_REG_c328;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_c329;regValue[i] = 0xcb;  i++;
 regAddr[i] = OV10630_REG_c32a;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_c32b;regValue[i] = 0xe4;  i++;
 regAddr[i] = OV10630_REG_c32c;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_c32d;regValue[i] = 0xab;  i++;
 regAddr[i] = OV10630_REG_c32e;regValue[i] = 0x00;  i++;
 regAddr[i] = OV10630_REG_c32f;regValue[i] = 0x71;  i++;
 regAddr[i] = OV10630_REG_c330;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_c331;regValue[i] = 0xf9;  i++;
 regAddr[i] = OV10630_REG_c332;regValue[i] = 0x00;  i++;
 regAddr[i] = OV10630_REG_c333;regValue[i] = 0xd9;  i++;
 regAddr[i] = OV10630_REG_c334;regValue[i] = 0x00;  i++;
 regAddr[i] = OV10630_REG_c335;regValue[i] = 0x2e;  i++;
 regAddr[i] = OV10630_REG_c336;regValue[i] = 0x00;  i++;
 regAddr[i] = OV10630_REG_c337;regValue[i] = 0xc8;  i++;
 regAddr[i] = OV10630_REG_c338;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_c339;regValue[i] = 0x4a;  i++;
 regAddr[i] = OV10630_REG_c33a;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_c33b;regValue[i] = 0xed;  i++;
 regAddr[i] = OV10630_REG_c33c;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_c33d;regValue[i] = 0xe5;  i++;
 regAddr[i] = OV10630_REG_c33e;regValue[i] = 0x00;  i++;
 regAddr[i] = OV10630_REG_c33f;regValue[i] = 0x53;  i++;
 regAddr[i] = OV10630_REG_c340;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_c341;regValue[i] = 0xc7;  i++;
 regAddr[i] = OV10630_REG_c342;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_c343;regValue[i] = 0xe8;  i++;
 regAddr[i] = OV10630_REG_c344;regValue[i] = 0xff;  i++;
 regAddr[i] = OV10630_REG_c345;regValue[i] = 0xa2;  i++;
 regAddr[i] = OV10630_REG_c346;regValue[i] = 0x00;  i++;
 regAddr[i] = OV10630_REG_c347;regValue[i] = 0x76;  i++;
 
 
 status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);  
 if(status!=OSA_SOK) {
  OSA_ERROR("DRV_i2c16Write16()\n");
  printf("10630 set error\n");
  return status;
 } //end awb & color modification

	memset(regAddr, 0, 200*sizeof(Uint16));
	memset(regValue, 0, 200*sizeof(Uint8));
	i = 0;

	
	regAddr[i] = OV10630_REG_c348;regValue[i] = 0x01;  i++;
	regAddr[i] = OV10630_REG_c349;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_6700;regValue[i] = 0x04;  i++;
	regAddr[i] = OV10630_REG_6701;regValue[i] = 0x7b;  i++;
	regAddr[i] = OV10630_REG_6702;regValue[i] = 0xfd;  i++;
	regAddr[i] = OV10630_REG_6703;regValue[i] = 0xf9;  i++;
	regAddr[i] = OV10630_REG_6704;regValue[i] = 0x3d;  i++;
	regAddr[i] = OV10630_REG_6705;regValue[i] = 0x71;  i++;
	regAddr[i] = OV10630_REG_6706;regValue[i] = 0x71;  i++;
	regAddr[i] = OV10630_REG_6708;regValue[i] = 0x05;  i++;
	regAddr[i] = OV10630_REG_3822;regValue[i] = 0x50;  i++;
	regAddr[i] = OV10630_REG_6f06;regValue[i] = 0x6f;  i++;
	regAddr[i] = OV10630_REG_6f07;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_6f0a;regValue[i] = 0x6f;  i++;
	regAddr[i] = OV10630_REG_6f0b;regValue[i] = 0x00;  i++;
	regAddr[i] = OV10630_REG_6f00;regValue[i] = 0x03;  i++;
	regAddr[i] = OV10630_REG_3042;regValue[i] = 0xf0;  i++;
	regAddr[i] = OV10630_REG_301b;regValue[i] = 0xf0;  i++;
	regAddr[i] = OV10630_REG_301c;regValue[i] = 0xf0;  i++;
	regAddr[i] = OV10630_REG_301a;regValue[i] = 0xf0;  i++;	
	
	status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);  
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_i2c16Write16()\n");
		printf("10630 set error\n");
		return status;
	}  

	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! all 10630 set ok!\n");
return OSA_SOK;



	if(status==OSA_SOK) {

		i=0;
			 
		/*
			0x00 : 30fps, 
			0x10 : 15fps, 
			0x20 : 10fps
		*/
		regAddr[i]	= PLL_CTRL01;		regValue[i] = 0x00;	i++;

		status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
	}

	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_i2cWrite16()\n");
		return status;
	}

  return status;
}

int DRV_imgsCheckId()
{
  	int status;
  	Uint16  regAddr;
  	Uint8 regValueH;  
  	Uint8 regValueL;  
  	Uint16 regValue;  
  

  	regAddr = CHIP_VERSION_H;
  	regValueH = 0;



  	status = DRV_i2c16Read8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValueH, 1);
  	if(status!=OSA_SOK) {
    	OSA_ERROR("DRV_i2cRead16()\n");
    	return status;  
      	}
	printf("register addr:%x, and its value is %x\n", regAddr, regValueH);

   regAddr = CHIP_VERSION_L;
  	regValueL = 0;
  
  	status = DRV_i2c16Read8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValueL, 1);
  	if(status!=OSA_SOK) 
	{
    	OSA_ERROR("DRV_i2cRead16()\n");
    	return status;  
  	}

	printf("register addr:%x, and its value is %x\n", regAddr, regValueL);  
  return OSA_SOK;
  regValue = (regValueH<<8)|regValueL;



/* all registers' readout value are always zero, why????*/
  if(regValue!=IMGS_CHIP_ID)
    return OSA_EFAIL;  

  return OSA_SOK;
}

int DRV_imgsSetRegs()
{

	int status;
	
	status = DRV_imgsReset();
  if(status!=OSA_SOK) 
    return status;

	return status;
}

int DRV_imgsAPChipInit(int *pGioC1, int *pGioD1)
{
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

	unsigned int *pPINMUXC1 = NULL;
	unsigned int *pPINMUXD1 = NULL;
	unsigned int val1, val2;
	unsigned char *pRegStart; 
	int ret = 0;

	if(gCSL_vpssHndl.regBaseVirtAddr==0 || pGioC1==0 || pGioD1==0){
		return -1;
	}
	pRegStart = (unsigned char *)gCSL_vpssHndl.regBaseVirtAddr;
	
	pPINMUXC1	= (unsigned int *)( pRegStart+ (AP_CHIP_C1_PINMUX-CSL_SYS_BASE_PHYS_ADDR));
	pPINMUXD1	= (unsigned int *)( pRegStart+ (AP_CHIP_D1_PINMUX-CSL_SYS_BASE_PHYS_ADDR));
	/* Enable C1 */
	*pPINMUXC1 = ((*pPINMUXC1 & (~AP_CHIP_C1_MASK)) | AP_CHIP_C1_VAL);
	val1 = (*pPINMUXC1 & AP_CHIP_C1_MASK);
	if(AP_CHIP_C1_VAL!= val1){
		OSA_ERROR("PINMUXC1 setting error(%X)!!!\n",val1);
		ret = -1;
		goto __DRV_imgs_APChipInit_End__;
	}
	/* Enable D1 */
	*pPINMUXD1 = (*pPINMUXD1 & (~AP_CHIP_D1_MASK)) | (AP_CHIP_D1_VAL);
	val2 = (*pPINMUXD1 & AP_CHIP_D1_MASK);
	if(AP_CHIP_D1_VAL != val2){
		OSA_ERROR("PINMUXD1 setting error(%X)!!!\n",val2);
		ret = -1;
		goto __DRV_imgs_APChipInit_End__;
	}

	*pGioC1 = IMGS_APCHIP_C1_GIO;
	*pGioD1 = IMGS_APCHIP_D1_GIO;

__DRV_imgs_APChipInit_End__:

	return ret;
}






