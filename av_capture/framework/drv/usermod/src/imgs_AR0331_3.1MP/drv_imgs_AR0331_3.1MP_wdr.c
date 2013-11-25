#ifdef AR0331_WDR
#include "drv_imgs_AR0331_3.1MP.h"
#include <drv_gpio.h>

DRV_ImgsObj gDRV_imgsObj;

#define INPUT_CLK    24  //EXTCLK
#define PLL_M        198  //pll_multiplier
#define PLL_pre_div  8   //pre_pll_clk_div
#define PLL_P1       1   //vt_sys_clk_div
#define PLL_P2       8   //vt_pix_clk_div
#define OUT_CLK ((INPUT_CLK * PLL_M) / (PLL_pre_div * PLL_P1 * PLL_P2))//74.25MHz

#define LINE_LENGTH (0x3e8) //1000,1080p 
#define ROW_TIME    29.9      // (2*LINE_LENGTH / OUT_CLK)//27.2us,two paths readout,HDR line_length:1100

#define rev2_HDR
/*
Currently,we use 16x HDR(T1/T2),so the maximum coarse integration time is minimum (70*(T1/T2):1120,0x460;frame_length-71:0x41e)
*/
int DRV_imgsOpen(DRV_ImgsConfig *config)
{
  int status, retry=10;
  Uint16 width, height;
  
  memset(&gDRV_imgsObj, 0, sizeof(gDRV_imgsObj));

  DRV_imgGetWidthHeight(config->sensorMode, &width, &height);

  width+=IMGS_H_PAD;
  height+=IMGS_V_PAD;

  DRV_imgsCalcFrameTime(config->fps, width, height, config->binEnable);

  status = DRV_i2cOpen(&gDRV_imgsObj.i2cHndl, IMGS_I2C_ADDR);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cOpen()\n");
    return status;
  }

  #ifdef BOARD_AP_IPNC
  DRV_gpioSetMode(IMGS_RESET_GPIO, DRV_GPIO_DIR_OUT);
  DRV_gpioSet(IMGS_RESET_GPIO);
  DRV_gpioClr(IMGS_RESET_GPIO);
  OSA_waitMsecs(50);
  DRV_gpioSet(IMGS_RESET_GPIO);
  OSA_waitMsecs(50);
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
  return "Aptina_AR0331_3.1MP";
}

int DRV_imgsSpecificSetting(void)
{
	return 0;
}

int DRV_imgsSet50_60Hz(Bool is50Hz)
{
 /*
 int fps;

  if( gDRV_imgsObj.curFrameTime.fps==30
    ||gDRV_imgsObj.curFrameTime.fps==25
    ) {

    if(is50Hz)
      fps = 25;
    else
      fps = 30;

    DRV_imgsSetFramerate(fps);
  }
  return 0;
*/
return 0;
}

int DRV_imgsSetFramerate(int fps)
{
/*
	Uint16 regAddr[4];
	Uint16 regValue[4];
	int i = 0;
	int j;
	int status = 0;

  switch(fps){
      case 30:
         i = 0;
		       regAddr[i] = AR0331_PLL_Multiplier; regValue[i] = 0x0031; i++; 	
		       regAddr[i] = AR0331_Pre_PLL_CLK_DIV; regValue[i] = 0x0002; i++; 	
			regAddr[i] = AR0331_VT_SYS_CLK_DIV; regValue[i] = 0x0001; i++; 	
			regAddr[i] = AR0331_VT_PIX_CLK_DIV; regValue[i] = 0x0008; i++; 
        break;
      case 24:
         i = 0;
		       regAddr[i] = AR0331_PLL_Multiplier; regValue[i] = 0x002D; i++; 	
		       regAddr[i] = AR0331_Pre_PLL_CLK_DIV; regValue[i] = 0x0002; i++; 
			regAddr[i] = AR0331_VT_SYS_CLK_DIV; regValue[i] = 0x0001; i++; 	
			regAddr[i] = AR0331_VT_PIX_CLK_DIV; regValue[i] = 0x000A; i++; 	
        break;
      case 15:
         i = 0;
		       regAddr[i] = AR0331_PLL_Multiplier; regValue[i] = 0x002D; i++; 	
		       regAddr[i] = AR0331_Pre_PLL_CLK_DIV; regValue[i] = 0x0002; i++; 
			regAddr[i] = AR0331_VT_SYS_CLK_DIV; regValue[i] = 0x0001; i++; 	
			regAddr[i] = AR0331_VT_PIX_CLK_DIV; regValue[i] = 0x0010; i++; 	
        break;
      case 8:
                 i = 0;
		       regAddr[i] = AR0331_PLL_Multiplier; regValue[i] = 0x002D; i++; 	
		       regAddr[i] = AR0331_Pre_PLL_CLK_DIV; regValue[i] = 0x0004; i++; 
			regAddr[i] = AR0331_VT_SYS_CLK_DIV; regValue[i] = 0x0001; i++; 	
			regAddr[i] = AR0331_VT_PIX_CLK_DIV; regValue[i] = 0x000F; i++; 	
        break;
      default: 
         break;
       }
for(j = 0; j < i ; j++)
{
	status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_i2c16Write16()\n");
		return status;
	}
	OSA_waitMsecs(1);
}
      OSA_waitMsecs(50);
    return status;
*/    
    return 0;
}

int DRV_imgsBinEnable(Bool enable)
{
/*
  Uint8 regAddr[4];
  Uint16 regValue[4];
  int i, col_bin, row_bin, status;

  if(!enable) {
    col_bin = 0;
    row_bin = 0;
  } else {
    col_bin = gDRV_imgsObj.curFrameTime.col_bin;
    row_bin = gDRV_imgsObj.curFrameTime.row_bin;
  }

  i=0;
  regAddr[i]  = ROW_ADDR_MODE;
  regValue[i] = (gDRV_imgsObj.curFrameTime.row_skip & 0x7 ) | ((row_bin & 0x3) << 4);
  i++;

  regAddr[i]  = COL_ADDR_MODE;
  regValue[i] = (gDRV_imgsObj.curFrameTime.col_skip & 0x7 ) | ((col_bin & 0x3) << 4);
  i++;

  status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }

  return status;
*/
return 0;
}

int DRV_imgsBinMode(int binMode)
{
/*
  Uint8 regAddr[4];
  Uint16 regValue[4];
  int i, status;

  i=0;
  regAddr[i]  = READ_MODE_2;
  regValue[i] = binMode;
  i++;


  status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }

  return status;
*/
return 0;
}

int DRV_imgsSetDgain(int dgain)
{
    return OSA_SOK;
}


int DRV_imgsSetAgain(int again, int setRegDirect)
{
  Uint16 regAddr;
  Uint16 regValue;

  int status=0;
  int coarse_gain, fine_gain;
  regAddr = AR0331_ANALOG_GAIN;

  if(again < 1000) again = 1000;
  if(again > 16000) again = 16000;
  float gain_float = 1000*32/again;

  if(again >= 8000) {
      coarse_gain = 3;
      fine_gain = 32 - gain_float * 8;
  }else if(again >= 4000){
      coarse_gain = 2;
      fine_gain = 32 - gain_float * 4;
  }else if(again >= 2000){
      coarse_gain = 1;
      fine_gain = 32 - gain_float * 2;
  }else{
    coarse_gain = 0;
    fine_gain = 32 - gain_float;
  }
  if(fine_gain >= 15) fine_gain = 15;
  if(fine_gain < 0) fine_gain = 0;

  regValue = (coarse_gain << 4) | fine_gain;
  //printf("fine gain, coarse gain :%d, %d\n",fine_gain, coarse_gain);
  //printf("again = %d, analog:%x\n",again, regValue);
  status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);  
	if(status!=OSA_SOK) {
		OSA_ERROR("I2C write error\n");
		return status;
}
	OSA_waitMsecs(10);
return 0;
}

int DRV_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect)
{
  int status;
  Uint16 regAddr;
  Uint16 regValue;
  regAddr =AR0331_COARSE_IT_TIME_A;
  regValue = (float)(eshutterInUsec/ROW_TIME);
  if(regValue >= 1054 )
  regValue = 1054;
 
  status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2c16Write16()\n");
    return status;
  }
 //printf("eshutterInUsec:%d\n",eshutterInUsec);
  return status;
 
//  return 0;
}
int AR0331_HDR_Enable(int enable)
{
   return 0;
}
//Read AGain & exposure
int DRV_imgsGetAgain(int *again)
{
/*
  Uint16 regAddr;
  Uint16 regValue;
  int status;

  regAddr = AR0331_GLOABL_GAIN;

  status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2c16Read16()\n");
    return status;
  }

  *again = regValue;
  return status;
 */
 return 0;
}

int DRV_imgsGetEshutter(Uint32 *eshutterInUsec)
{
/*
  int status;
  Uint16 regAddr;
  Uint16 regValue;

  regAddr = AR0331_COARSE_IT_TIME_A;

  status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2c16Write16()\n");
    return status;
  }

 *eshutterInUsec = regValue*ROW_TIME;

  return status;
  */
  return 0;
}

int DRV_imgsSetDcSub(Uint32 dcSub, int setRegDirect)
{

  return 0;
}

int DRV_imgsEnable(Bool enable)
{
  int status=OSA_SOK;

  if(enable) {
    status = DRV_imgsSetRegs();
    if(status!=OSA_SOK) {
      OSA_ERROR("DRV_imgsSetRegs()\n");
      return status;
    }
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
  	return &gDRV_imgsH3aConfig_Appro;
  }
  else if(aewbVendor==2) {
  	return &gDRV_imgsH3aConfig_TI;
  }
  else {
  	return &gDRV_imgsH3aConfig_Common;
  }
}

DRV_ImgsLdcConfig       *DRV_imgsGetLdcConfig(int sensorMode, Uint16 ldcInFrameWidth, Uint16 ldcInFrameHeight)
{
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
  return 0;
}

int DRV_imgsCheckId()
{
  int status;
  Uint16  regAddr;
  Uint16 regValue;

  regAddr = CHIP_VERSION_ADDR;
  regValue = 0;

  status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
  printf("Read Sensor ID:0x%4x\n",regValue);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cRead16()\n");
    return status;
  }
  if(regValue!=IMGS_CHIP_ID)
    return OSA_EFAIL;

  return OSA_SOK;
}

int DRV_imgsSetRegs()
 {
  Uint16  regAddr[500];
  Uint16 regValue[500];
  int status,i=0;
  int j;
  memset(regAddr, 0, 500*sizeof(Uint16));
  memset(regValue, 0,500*sizeof(Uint16));
//rev2 HDR
#ifdef rev2_HDR
//rev2 HDR release
        regAddr[i] = 0x301A; regValue[i] =  0x0001 	// RESET_; i++ ; regAddr[i] ISTER
; i++ ; regAddr[i] = 0x301A; regValue[i] =  0x10D8 	// RESET_; i++ ; regAddr[i] ISTER
; i++ ; regAddr[i] = 0x3088; regValue[i] =  0x8000 	// SEQ_CTRL_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4540 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x6134 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4A31 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4342 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4560 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2714 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x3DFF 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x3DFF 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x3DEA 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2704 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x3D10 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2705 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x3D10 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2715 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x3527 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x053D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1045 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4027 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0427 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x143D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xFF3D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xFF3D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xEA62 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2728 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x3627 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x083D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x6444 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2C2C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2C2C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4B00 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x432D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x6343 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1664 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x435F 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4F50 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2604 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2694 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x27FC 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x530D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5C0D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5754 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1709 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5556 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4953 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0753 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x034D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x286C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4C09 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x282C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2828 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x261C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4E5C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0960 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4500 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4580 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x26BE 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x27F8 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1702 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x27FA 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5C0B 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1712 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x26BA 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5C03 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1713 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x27F2 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x171C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5F28 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0817 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0360 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x173C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x26B2 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1616 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5F4D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1926 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x9316 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1627 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xFA45 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xA017 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0527 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xFB17 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1F45 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x801F 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1705 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x27FA 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x171E 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5D17 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0C26 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x9248 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4D4E 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x269A 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2808 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4C0B 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x6017 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0327 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xF217 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2626 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x9216 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x165F 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4D19 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2693 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1616 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x27FA 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2643 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5C01 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4540 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2798 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1720 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4A65 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4316 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x6643 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5A43 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x165B 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4327 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x9C45 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x6017 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0627 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x9D17 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1C45 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4023 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1705 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2798 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5D26 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4417 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0E28 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0053 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x014B 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5251 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1244 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4B01 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x432D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4643 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1647 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x434F 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5026 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0426 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x8427 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xFC53 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0D5C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0D57 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5417 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0955 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5649 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5307 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5303 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4D28 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x6C4C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0928 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2C28 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2826 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0C4E 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5C09 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x6045 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0045 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x8026 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xAE27 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xF817 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0227 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xFA5C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0B17 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1226 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xAA5C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0317 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0B27 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xF217 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x265F 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2808 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1703 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x6017 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0326 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xA216 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x165F 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4D1A 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2683 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1616 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x27FA 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x45A0 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1705 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x27FB 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x171F 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4580 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2017 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0527 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xFA17 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1E5D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x170C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2682 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x484D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4E26 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x8A28 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x084C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0B60 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1707 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x27F2 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1738 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2682 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1616 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5F4D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1A26 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x8316 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1627 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0xFA26 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x435C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0145 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4027 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x9817 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1F4A 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1244 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0343 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1604 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4316 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5843 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1659 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4316 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x279C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4560 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x1705 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x279D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x171D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4540 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2217 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x0527 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x985D 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2645 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x170E 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2800 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5301 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4B52 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x5112 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x4460 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2C2C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x3086; regValue[i] =  0x2C2C 	// SEQ_DATA_PORT
; i++ ; regAddr[i] = 0x301A; regValue[i] =  0x10D8 	// RESET_; i++ ; regAddr[i] ISTER
; i++ ; regAddr[i] = 0x30B0; regValue[i] =  0x0000 	// DIGITAL_TEST
; i++ ; regAddr[i] = 0x30BA; regValue[i] =  0x07EC 	// DIGITAL_CTRL
; i++ ; regAddr[i] = 0x31AC; regValue[i] =  0x100C 	// DATA_FORMAT_BITS
; i++ ; regAddr[i] = 0x302A; regValue[i] =  PLL_P2      // VT_PIX_CLK_DIV
; i++ ; regAddr[i] = 0x302C; regValue[i] =  PLL_P1 	// VT_SYS_CLK_DIV
; i++ ; regAddr[i] = 0x302E; regValue[i] =  PLL_pre_div // PRE_PLL_CLK_DIV
; i++ ; regAddr[i] = 0x3030; regValue[i] =  PLL_M 	// PLL_MULTIPLIER
; i++ ; regAddr[i] = 0x3036; regValue[i] =  0x000C 	// OP_PIX_CLK_DIV
; i++ ; regAddr[i] = 0x3038; regValue[i] =  0x0001 	// OP_SYS_CLK_DIV
; i++ ; regAddr[i] = 0x3002; regValue[i] =  0x00E4 	// Y_ADDR_START
; i++ ; regAddr[i] = 0x3004; regValue[i] =  0x0042 	// X_ADDR_START
; i++ ; regAddr[i] = 0x3006; regValue[i] =  0x0523 	// Y_ADDR_END
; i++ ; regAddr[i] = 0x3008; regValue[i] =  0x07C9 	// X_ADDR_END
; i++ ; regAddr[i] = 0x300A; regValue[i] =  0x0465 	// FRAME_LENGTH_LINES
; i++ ; regAddr[i] = 0x300C; regValue[i] =  0x44c   // LINE_LENGTH_PCK
; i++ ; regAddr[i] = 0x3012; regValue[i] =  0x03F4 	// COARSE_INTEGRATION_TIME
; i++ ; regAddr[i] = 0x30A2; regValue[i] =  0x0001 	// X_ODD_INC
; i++ ; regAddr[i] = 0x30A6; regValue[i] =  0x0001 	// Y_ODD_INC
; i++ ; regAddr[i] = 0x3040; regValue[i] =  0xC000 	// READ_MODE
; i++ ; regAddr[i] = 0x31AE; regValue[i] =  0x0301 	// SERIAL_FORMAT
; i++ ; regAddr[i] = 0x3082; regValue[i] =  0x0008 	// OPERATION_MODE_CTRL
; i++ ; regAddr[i] = 0x305E; regValue[i] =  0x0080 	// GLOBAL_GAIN
; i++ ; regAddr[i] = 0x3082; regValue[i] =  0x0008 	// OPERATION_MODE_CTRL
; i++ ; regAddr[i] = 0x318C; regValue[i] =  0xC001 	// HDR_MC_CTRL2
; i++ ; regAddr[i] = 0x3198; regValue[i] =  0x061E 	// HDR_MC_CTRL8
; i++ ; regAddr[i] = 0x301E; regValue[i] =  0x0000 	// DATA_PEDESTAL
; i++ ; regAddr[i] = 0x30FE; regValue[i] =  0x0000 	// NOISE_PEDESTAL
; i++ ; regAddr[i] = 0x320A; regValue[i] =  0x0000 	// ADACD_PEDESTAL
; i++ ; regAddr[i] = 0x301A; regValue[i] =  0x10D8 	// RESET_; i++ ; regAddr[i] ISTER
; i++ ; regAddr[i] = 0x2410; regValue[i] =  0x0010 	// ALTM_POWER_GAIN
; i++ ; regAddr[i] = 0x2412; regValue[i] =  0x0010 	// ALTM_POWER_OFFSET
; i++ ; regAddr[i] = 0x2442; regValue[i] =  0x0080 	// ALTM_CONTROL_KEY_K0
; i++ ; regAddr[i] = 0x2444; regValue[i] =  0x0000 	// ALTM_CONTROL_KEY_K01_LO
; i++ ; regAddr[i] = 0x2446; regValue[i] =  0x0004 	// ALTM_CONTROL_KEY_K01_HI
; i++ ; regAddr[i] = 0x2440; regValue[i] =  0x0002 	// ALTM_CONTROL_DAMPER
; i++ ; regAddr[i] = 0x2450; regValue[i] =  0x0000 	// ALTM_OUT_PEDESTAL
; i++ ; regAddr[i] = 0x2438; regValue[i] =  0x0010 	// ALTM_CONTROL_MIN_FACTOR
; i++ ; regAddr[i] = 0x243A; regValue[i] =  0x0020 	// ALTM_CONTROL_MAX_FACTOR
; i++ ; regAddr[i] = 0x243C; regValue[i] =  0x0000 	// ALTM_CONTROL_DARK_FLOOR
; i++ ; regAddr[i] = 0x243E; regValue[i] =  0x0200 	// ALTM_CONTROL_BRIGHT_FLOOR
; i++ ; regAddr[i] = 0x31D0; regValue[i] =  0x0000 	// COMPANDING
; i++ ; regAddr[i] = 0x301E; regValue[i] =  0x0000 	// DATA_PEDESTAL
; i++ ; regAddr[i] = 0x2400; regValue[i] =  0x0002 	// ALTM_CONTROL
; i++ ; regAddr[i] = 0x301A; regValue[i] =  0x10DC 	// RESET_; i++ ; regAddr[i] ISTER
; i++ ; regAddr[i] = 0x3202; regValue[i] =  0x00cf;//0x00A0 	// ADACD_NOISE_MODEL1
; i++ ; regAddr[i] = 0x3206; regValue[i] =  0x0a06;//0x1810 	// RESERVED_MFR_3206
; i++ ; regAddr[i] = 0x3208; regValue[i] =  0x1a12;//0x2820 	// RESERVED_MFR_3208
; i++ ; regAddr[i] = 0x3200; regValue[i] =  0x0002 	// ADACD_CONTROL
; i++ ; regAddr[i] = 0x31E0; regValue[i] =  0x0200 	// PIX_DEF_ID
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0006 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x318A; regValue[i] =  0x0E10 	// HDR_MC_CTRL1
; i++ ; regAddr[i] = 0x3064; regValue[i] =  0x1982 	// SMIA_TEST
; i++ ; regAddr[i] = 0x3064; regValue[i] =  0x1982 	// SMIA_TEST
; i++ ; regAddr[i] = 0x301A; regValue[i] =  0x10DE 	// RESET_; i++ ; regAddr[i] ISTER
; i++ ; regAddr[i] = 0x3202; regValue[i] =  0x00cf;//0x00A0 	// ADACD_NOISE_MODEL1
; i++ ; regAddr[i] = 0x3206; regValue[i] =  0x0a06;//0x1810 	// RESERVED_MFR_3206
; i++ ; regAddr[i] = 0x3208; regValue[i] =  0x1a12;//0x2820 	// RESERVED_MFR_3208
; i++ ; regAddr[i] = 0x3200; regValue[i] =  0x0002 	// ADACD_CONTROL
; i++ ; regAddr[i] = 0x3180; regValue[i] =  0x8089 	// DELTA_DK_CONTROL
; i++ ; regAddr[i] = 0x30F4; regValue[i] =  0x4000 	// RESERVED_MFR_30F4
; i++ ; regAddr[i] = 0x3ED4; regValue[i] =  0x8F6C 	// RESERVED_MFR_3ED4
; i++ ; regAddr[i] = 0x3ED6; regValue[i] =  0x6666 	// RESERVED_MFR_3ED6
; i++ ; regAddr[i] = 0x3EDA; regValue[i] =  0x8899 	// RESERVED_MFR_3EDA
; i++ ; regAddr[i] = 0x3EE6; regValue[i] =  0x00F0 	// RESERVED_MFR_3EE6
; i++ ; regAddr[i] = 0x3ED2; regValue[i] =  0xBF46 	// DAC_LD_6_7
; i++ ; regAddr[i] = 0x301A; regValue[i] =  0x10DC 	// RESET_; i++ ; regAddr[i] ISTER
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0006 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0091 	// RED_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D4 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3012; regValue[i] =  0x0445 	// COARSE_INTEGRATION_TIME
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x000F 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0095 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0083 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0083 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DB 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0010 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0095 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0083 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0083 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DD 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0092 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D9 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0092 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DB 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0092 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DC 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DD 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DE 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DE 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DE 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DE 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DD 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DD 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DC 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DB 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DA 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00DA 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D9 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D8 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D8 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D7 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D7 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D6 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D6 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D5 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D5 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D4 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0012 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0093 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0080 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0080 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D4 	// BLUE_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0010 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x000F 	// ANALOG_GAIN
; i++ ; regAddr[i] = 0x3060; regValue[i] =  0x0006	// ANALOG_GAIN,the last setting for Analog Gain,X1.231**********
; i++ ; regAddr[i] = 0x305A; regValue[i] =  0x0096 	// RED_GAIN
; i++ ; regAddr[i] = 0x3056; regValue[i] =  0x0083 	// GREEN1_GAIN
; i++ ; regAddr[i] = 0x305C; regValue[i] =  0x0083 	// GREEN2_GAIN
; i++ ; regAddr[i] = 0x3058; regValue[i] =  0x00D9 	// BLUE_GAIN

/* the following configurations are added by Xiangdong for embedded statistics for off-chip AE */
; i++ ; regAddr[i] = 0x314C; regValue[i] =  0x1000 	// HIST_DIV = 4096  
/* Xiangdong: roi window is the same as the active video window */
; i++ ; regAddr[i] = 0x3140; regValue[i] =  0x0042 	// ae_roi_x_start_offset 
; i++ ; regAddr[i] = 0x3142; regValue[i] =  0x00e4 	// ae_roi_y_start_offset
; i++ ; regAddr[i] = 0x3144; regValue[i] =  0x0788 	// ae_roi_x_size 
; i++ ; regAddr[i] = 0x3146; regValue[i] =  0x0440 	// ae_roi_y_size 

; i++ ;
#endif
for(j=0; j < i; j++)
{
   status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr[j], &regValue[j], 1);  
	if(status != OSA_SOK)
	{
	   printf("I2C write Error,index:%d\n",j);
	   return status;
	}
if(regAddr[j] == 0x301A )	
   OSA_waitMsecs(100);

   OSA_waitMsecs(1);	
}
printf("Finished HDR Init with AR0331 rev2.\n");
return 0;
}


int DRV_ImgsGetMean(unsigned int *highMean, unsigned int *lowMean)
{
  Uint16  regAddr[5];
  Uint16 regValue[5];
  int status;
  Uint32 totalMean, lowPerc;
  memset(regAddr, 0, 2*sizeof(Uint16));
  memset(regValue, 0,2*sizeof(Uint16));

  regAddr[0] = 0x3150;
  regAddr[1] = 0x3152;
  regAddr[2] = 0x315c;
  regAddr[3] = 0x315e;
  regAddr[4] = 0x3160;
  
  status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, 5);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2c16Read16()\n");
    return status;
  }
  totalMean = (((int)regValue[0])<<16) + regValue[1];
  *lowMean = (((int)regValue[2])<<16) + regValue[3];
  lowPerc = regValue[4];

  *highMean = (totalMean*65536.0 - (*lowMean)*lowPerc)/ (65536.0 - lowPerc);
  
  //printf("lowMean = %d, highMean = %d, lowPerc = %d, totalMean = %d\n",
  //  *lowMean, *highMean, lowPerc, totalMean);
  
  return 0;

}

int DRV_ImgsGetWDRSetting(unsigned int *wdrMode, unsigned int *ratioT1T2)
{
  Uint16 regAddr;
  Uint16 regValue; 
  int status;
  int shift;

  regAddr = 0x3082;
 
  status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2c16Read16()\n");
    return status;
  }
  *wdrMode = regValue & 0x1;
  shift = (regValue >> 2) & 0x3;

  *ratioT1T2 = 1 << (2+shift);
  //printf("wdrMode = %d, ratioT1T2 = %d\n", *wdrMode, *ratioT1T2);

  return 0;
}

int DRV_ImgsSetWDRSetting(unsigned int wdrMode, unsigned int ratioT1T2)
{
  Uint16 regAddr;
  Uint16 regValue; 
  int status;
  int shift;

  regAddr = 0x3082;
  if (ratioT1T2 == 4) shift = 0;
  else if (ratioT1T2 == 8) shift = 1;
  else if (ratioT1T2 == 16) shift = 2;
  else if (ratioT1T2 == 32) shift = 3;
  else 
  {
       OSA_ERROR("T1 T2 ratio is invalid\n");
       return OSA_EFAIL;
  }
  regValue = (shift << 1) | (wdrMode & 0x1);
 
  status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2c16Write16()\n");
    return status;
  } 
  return 0;
}

#endif //#ifdef AR0331_WDR

