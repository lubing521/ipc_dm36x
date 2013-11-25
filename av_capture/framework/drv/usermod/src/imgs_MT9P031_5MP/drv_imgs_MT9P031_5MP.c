
#include "drv_imgs_MT9P031_5MP.h"
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
  return "MICRON_MT9P031_5MP";
}

int DRV_imgsSpecificSetting(void)
{
	Uint8 regAddr[30];
	Uint16 regValue[30];
	int i, status;

	i=0;

	regAddr[i]  = 0x29;  regValue[i] = 0x481;  	i++;
	regAddr[i]  = 0x3E;  regValue[i] = 0x87;  	i++;
	regAddr[i]  = 0x3F;  regValue[i] = 0x07;  	i++;
	regAddr[i]  = 0x41;  regValue[i] = 0x0003;  i++;
	regAddr[i]  = 0x42;  regValue[i] = 0x0003;  i++;
	regAddr[i]  = 0x43;  regValue[i] = 0x0003;  i++;
	regAddr[i]  = 0x4f;  regValue[i] = 0x0014;  i++;
	regAddr[i]  = 0x48;  regValue[i] = 0x18;  	i++;
	regAddr[i]  = 0x5D;  regValue[i] = 0x2D13;  i++;
	regAddr[i]  = 0x5E;  regValue[i] = 0x41FF;  i++;
	regAddr[i]  = 0x5F;  regValue[i] = 0x1C16;  i++;

	regAddr[i]  = 0x57;  regValue[i] = 0x7;  	i++;
	regAddr[i]  = 0x70;  regValue[i] = 0x005C;  i++;
	regAddr[i]  = 0x71;  regValue[i] = 0x5B00;  i++;
	regAddr[i]  = 0x72;  regValue[i] = 0x5900;  i++;
	regAddr[i]  = 0x73;  regValue[i] = 0x200;  	i++;
	regAddr[i]  = 0x74;  regValue[i] = 0x200;  	i++;
	regAddr[i]  = 0x75;  regValue[i] = 0x2800;  i++;
	regAddr[i]  = 0x76;  regValue[i] = 0x3E29;  i++;
	regAddr[i]  = 0x77;  regValue[i] = 0x3E29;  i++;
	regAddr[i]  = 0x78;  regValue[i] = 0x583F;  i++;
	regAddr[i]  = 0x79;  regValue[i] = 0x5B00;  i++;
	regAddr[i]  = 0x7A;  regValue[i] = 0x5A00;  i++;
	regAddr[i]  = 0x7B;  regValue[i] = 0x5900;  i++;
	regAddr[i]  = 0x7C;  regValue[i] = 0x5900;  i++;
	regAddr[i]  = 0x7E;  regValue[i] = 0x5900;  i++;
	regAddr[i]  = 0x7F;  regValue[i] = 0x5900;  i++;

	status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_i2cWrite16()\n");
		return status;
	}

	return status;
}

int DRV_imgsSet50_60Hz(Bool is50Hz)
{
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
}

int DRV_imgsSetFramerate(int fps)
{
  Uint8 regAddr[4];
  Uint16 regValue[4];
  int i, status;

  //DRV_imgsCalcFrameRate(fps);
  DRV_imgsCalcFrameRate(25);

  i=0;
  regAddr[i]  = VERT_BLANK;
  regValue[i] = gDRV_imgsObj.curFrameTime.VB;
  i++;

  status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }

  return status;
}

int DRV_imgsBinEnable(Bool enable)
{
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
}

int DRV_imgsBinMode(int binMode)
{
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
}


int DRV_imgsSetDgain(int dgain)
{
    return OSA_SOK;
}

int DRV_imgsSetAgain(int again, int setRegDirect)
{
  Uint8 regAddr;
  Uint16 regValue;
  int status;

  regAddr  = GAIN_GLOBAL;

  if(setRegDirect) {
    regValue = again;
  } else {
    regValue = DRV_imgsCalcAgain(again);
  }

  status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }

  return status;
}

int DRV_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect)
{
  int status;
  Uint8 regAddr;
  Uint16 regValue;

  regAddr  = SHUTTER_WIDTH_L;

  if(setRegDirect) {
    regValue = eshutterInUsec;
  } else  {
    DRV_imgsCalcSW(eshutterInUsec);
    regValue = gDRV_imgsObj.curFrameTime.SW;
  }

  status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }

  return status;
}

//Read AGain & exposure
//return value is 8 times the true analog gain
int DRV_imgsGetAgain(int *again)
{
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
}

int DRV_imgsGetEshutter(Uint32 *eshutterInUsec)
{
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
}

int DRV_imgsSetDcSub(Uint32 dcSub, int setRegDirect)
{

  return 0;
}

int DRV_imgsEnable(Bool enable)
{
  Uint8 regAddr;
  Uint16 regValue;
  int status=OSA_SOK;

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
  OSA_waitMsecs(10);

#ifdef SENSOR_REG_DUMP
  {
	  unsigned char i;
	  Uint8 dumpAddr[128];
	  Uint16 dumpValue[128];
	  FILE *dumpFile;

	  for (i=0;i<128;i++)
	  	dumpAddr[i] = i;

	  DRV_i2cRead16(&gDRV_imgsObj.i2cHndl, dumpAddr, dumpValue, 128);
  	  OSA_waitMsecs(100);

	  dumpFile = fopen("sensor_dump.txt","wb");
	  for (i=0;i<128;i++)
	  	fprintf(dumpFile, "0x%x \t0x%x\n", dumpAddr[i], dumpValue[i]);
	  fclose(dumpFile);
  }
#endif

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
  Uint8 regAddr[8];
  Uint16 regValue[8];
  int status, i;

  i=0;

  regAddr[i]  = OUTPUT_CTRL;
  regValue[i] = 0x1f80;

  regAddr[i]  = RESET;
  regValue[i] = 0x1;
  i++;

  status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }

  if(status==OSA_SOK) {

    i=0;

    regAddr[i]  = RESET;
    regValue[i] = 0x0;
    i++;

    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
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
  Uint8  regAddr;
  Uint16 regValue;

  regAddr = CHIP_VERSION;
  regValue = 0;

  status = DRV_i2cRead16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
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
  int i, status;
  static Uint8 regAddr[32];
  static Uint16 regValue[32];

  DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;

  status = DRV_imgsReset();
  if(status!=OSA_SOK)
    return status;

  OSA_waitMsecs(10);

  i=0;

  regAddr[i]  = OUTPUT_CTRL;
  regValue[i] = 0x1F83;
  i++;

  regAddr[i]  = PLL_CTRL;
  regValue[i] = 0x51;
  i++;

  regAddr[i]  = PLL_CFG1;
  regValue[i] = ( (Uint16)(pFrame->pll_M & 0xFF) << 8 )| ((pFrame->pll_N) & 0x3F);
  i++;

  regAddr[i]  = PLL_CFG2;
  regValue[i] = (pFrame->pll_DIV) & 0x1F;
  i++;

  status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }

  OSA_waitMsecs(10);

  i=0;

  regAddr[i]  = PLL_CTRL;
  regValue[i] = 0x53;  // enable PLL
  i++;

  status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }
  OSA_waitMsecs(10);
  i=0;

  regAddr[i]  = ROW_START;
  regValue[i] = pFrame->row_start;
  i++;

  regAddr[i]  = COL_START;
  regValue[i] = pFrame->col_start;
  i++;

  regAddr[i]  = ROW_SIZE;
  regValue[i] = pFrame->row_size;
  i++;

  regAddr[i]  = COL_SIZE;
  regValue[i] = pFrame->col_size;
  i++;

  regAddr[i]  = HORZ_BLANK;
  regValue[i] = pFrame->HB;
  i++;

  regAddr[i]  = VERT_BLANK;
  regValue[i] = pFrame->VB;
  i++;

  regAddr[i]  = SHUTTER_WIDTH_H;
  regValue[i] = 0;
  i++;

  regAddr[i]  = SHUTTER_WIDTH_L;
  regValue[i] = pFrame->SW;
  i++;

  regAddr[i]  = SHUTTER_DELAY;
  regValue[i] = pFrame->SD;
  i++;

  regAddr[i]  = ROW_ADDR_MODE;
  regValue[i] = (pFrame->row_skip & 0x7 ) | ((pFrame->row_bin & 0x3) << 4);
  i++;

  regAddr[i]  = COL_ADDR_MODE;
  regValue[i] = (pFrame->col_skip & 0x7 ) | ((pFrame->col_bin & 0x3) << 4);
  i++;

  regAddr[i]  = PIXEL_CLK_CTRL;
  regValue[i] = 0x8000;
  i++;

  regAddr[i]  = ROW_BLK_DEF_OFFSET  ;
  regValue[i] = 64;
  i++;

  status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }

  DRV_imgsSetAgain(1000, 0); // default aGain
  DRV_imgsSetEshutter(8333, 0);

  return status;
}


