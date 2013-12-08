
#include "drv_imgs_AR0330_3.1MP.h"
#include <drv_gpio.h>
#include "drv_imgs_AR0330_REG.h"

DRV_ImgsObj gDRV_imgsObj;

// for 1080p60, change PLL_P1 TO 1, OP_SYS_CLK_DIV to 1, the pixel clock will be 148.5Mhz
#define INPUT_CLK    24  //EXTCLK
#define PLL_M        74  //pll_multiplier
#define PLL_pre_div  4   //pre_pll_clk_div
#define PLL_P1       2   //vt_sys_clk_div
#define PLL_P2       6   //vt_pix_clk_div
#define OP_SYS_CLK_DIV 2 //OP_SYS_CLK_DIV

#define OUT_CLK ((INPUT_CLK * PLL_M) / (PLL_pre_div * PLL_P1 * PLL_P2) )//74.25MHz

#define LINE_LENGTH (0x3e8) //1000,1080p 
#define ROW_TIME    29.6297 //Xiangdong //27     // (2*LINE_LENGTH / OUT_CLK)//26.9us,two paths readout,

//#define AEWB_DEBUG

Uint16 gGain_VAL = 0;
static int sensorMode = -1;

int DRV_imgsEnable_AR0330(Bool enable);

int DRV_imgsCheckId_AR0330()
{
    int status;
    Uint16  regAddr;
    Uint16 regValue;

    regAddr = CHIP_VERSION_ADDR;
    regValue = 0;

    status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    
    if(status!=OSA_SOK) 
    {
        OSA_ERROR("DRV_i2cRead16()\n");
        return status;
    }
    if(regValue!=IMGS_CHIP_ID)
        return OSA_EFAIL;

#if 1 // read AR0330 version
    regAddr = 0x300E;
    regValue = 0;
    status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if(status!=OSA_SOK) 
    {
        OSA_ERROR("DRV_i2cRead16()\n");
        return status;
    }
    else
    {
        OSA_printf(" AR0330: 0x%4X = 0x%04x\n", regAddr , regValue);
    }

    regAddr = 0x30F0;
    regValue = 0;
    status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if(status!=OSA_SOK) 
    {
        OSA_ERROR("DRV_i2cRead16()\n");
        return status;
    }
    else
    {
        OSA_printf(" AR0330: 0x%4X = 0x%04x\n", regAddr , regValue);
    }

    regAddr = 0x3072;
    regValue = 0;
    status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if(status!=OSA_SOK) 
    {
        OSA_ERROR("DRV_i2cRead16()\n");
        return status;
    }
    else
    {
        OSA_printf(" AR0330: 0x%4X = 0x%04x\n", regAddr , regValue);
    }
 #endif       
  
    return OSA_SOK;
}

int DRV_imgsOpen_AR0330(DRV_ImgsConfig *config)
{
    int status, retry=10;
    Uint16 width, height;
  
    memset(&gDRV_imgsObj, 0, sizeof(gDRV_imgsObj));

    sensorMode = config->sensorMode;
    DRV_imgGetWidthHeight(config->sensorMode, &width, &height);

    width  +=IMGS_H_PAD;
    height +=IMGS_V_PAD;

    DRV_imgsCalcFrameTime_AR0330(config->fps, width, height, config->binEnable);

    status = DRV_i2cOpen(&gDRV_imgsObj.i2cHndl, IMGS_I2C_ADDR);
    if(status!=OSA_SOK) 
    {
        OSA_ERROR("DRV_i2cOpen()\n");
        return status;
    }

#ifdef BOARD_AP_IPNC
    DRV_gpioSetMode(IMGS_RESET_GPIO, DRV_GPIO_DIR_OUT);
    DRV_gpioClr(IMGS_RESET_GPIO);
    OSA_waitMsecs(100);
    DRV_gpioSet(IMGS_RESET_GPIO);
    OSA_waitMsecs(100);
#endif

    do 
    {
        status = DRV_imgsCheckId_AR0330();
        if(status==OSA_SOK)
            break;
        OSA_waitMsecs(10);
    } while(retry--);

    if(status!=OSA_SOK) 
    {
        OSA_ERROR("DRV_imgsCheckId()\n");
        DRV_i2cClose(&gDRV_imgsObj.i2cHndl);
        return status;
    }
    
    return OSA_SOK;
}

int DRV_imgsClose_AR0330()
{
    int status;

    status = DRV_imgsEnable_AR0330(FALSE);
    status |= DRV_i2cClose(&gDRV_imgsObj.i2cHndl);

    return status;
}

char* DRV_imgsGetImagerName_AR0330()
{
    return "Aptina_AR0330_3.1MP";
}

int DRV_imgsSetMirror_AR0330(Bool flipH, Bool flipV)
{
    int status = OSA_SOK;
    Uint16 regAddr  = 0;
    Uint16 regValue = 0;

    regAddr = 0x3040;
    
    if (flipH) 
    {
        //regValue |= 1 << 14;
        regValue &= ~(1 << 14);
    }
    else
    {
        //regValue &= ~(1 << 14);
        regValue |= 1 << 14;
    }

    if (flipV) 
    {
        //regValue |= 1 << 15;
        regValue &= ~(1 << 15);
    }
    else
    {
        //regValue &= ~(1 << 15);
        regValue |= 1 << 15;
    }
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
#if 0
    //Disable streaming (R0x301A[2]=0)
    regAddr = AR0130_ENABLE_STREAMING;
    regValue = 0x19d4 & ~(1 << 2);
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
   
    //Disable column correction (R0x30D4[15]=0)
    regAddr = 0x30D4;
    regValue = 0xE007 & ~(1 << 15);
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    //Enable streaming (R0x301A[2]=1)
    regAddr = AR0130_ENABLE_STREAMING;
    regValue = 0x19d4;
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    //Wait one frame time, or more
    OSA_waitMsecs(50);

    //Disable streaming (R0x301A[2]=0)
    regAddr = AR0130_ENABLE_STREAMING;
    regValue = 0x19d4 & ~(1 << 2);
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    //Enable column correction (R30D4[15]=1)
    regAddr = 0x30D4;
    regValue = 0xE007;
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    //Enable streaming (R0x301A[2]=1)
    regAddr  = AR0130_ENABLE_STREAMING;
    regValue = 0x19d4;
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    
    //Bayer resampling must be enabled, by setting bit 4 of register 0 x 306E[4] = 1.
    regAddr = 0x306E;
    regValue = 0x9011;
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
#endif
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_imgsSetMirror Error!\n");
        return status;
    }
    else
    {
        OSA_printf("DRV_imgsSetMirror OK!\n");
    }
    
    return OSA_SOK;
}

int DRV_imgsSpecificSetting_AR0330(void)
{
    return OSA_SOK;
}

int DRV_imgsSet50_60Hz_AR0330(Bool is50Hz)
{
    int fps = 0;
    if (gDRV_imgsObj.curFrameTime.fps == 30 || gDRV_imgsObj.curFrameTime.fps == 25)
    {
        if (is50Hz)
        {
            fps = 25;
        }
        else
        {
            fps = 30;
        }
        DRV_imgsSetFramerate_AR0330(fps);
    }

    //OSA_printf("fps = %d\n", gDRV_imgsObj.curFrameTime.fps);
    return OSA_SOK;
}

int DRV_imgsSetFramerate_AR0330(int fps)
{  
    Uint16 regAddr[2];
    Uint16 regValue[2];
    int i = 0;
    int status = 0; 
    switch(fps)
    {
      case 30:
         i = 0;
         regAddr[i] = 0x300A; regValue[i] = 0x051C; i++;    // FRAME_LENGTH_LINES
         regAddr[i] = 0x300C; regValue[i] = 0x04E1; i++;    // LINE_LENGTH_PCK  4c7
         //regAddr[i] = 0x300A; regValue[i] = 0x001E; i++;    // FRAME_LENGTH_LINES
         //regAddr[i] = 0x300C; regValue[i] = 0x04E0; i++;    // LINE_LENGTH_PCK
         break;
      case 25:
         i = 0;	 
         regAddr[i] = 0x300A; regValue[i] = 0x051C; i++;    // FRAME_LENGTH_LINES
         regAddr[i] = 0x300C; regValue[i] = 0x05D8; i++;    // LINE_LENGTH_PCK   5BB
         //regAddr[i] = 0x300A; regValue[i] = 0x006E; i++;    // FRAME_LENGTH_LINES
         //regAddr[i] = 0x300C; regValue[i] = 0x04E0; i++;    // LINE_LENGTH_PCK 
         break;
      default: 
         break;
    }
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if(status!=OSA_SOK) 
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    return status;		
}

int DRV_imgsBinEnable_AR0330(Bool enable)
{
    return OSA_SOK;
}

int DRV_imgsBinMode_AR0330(int binMode)
{
    return OSA_SOK;
}

int DRV_imgsSetDgain_AR0330(int dgain)
{
    return OSA_SOK;
}

/*Current Gain setting,only for Context A*/
int DRV_imgsSetAgain_AR0330(int again, int setRegDirect)
{
    Uint16 regAddr[2];
    Uint16 regValue[2];

    int status=0;
    regAddr[0] = AR0331_ANALOG_GAIN;
    regAddr[1] = AR0331_GLOBAL_GAIN;

    if (again > 127938) again = 127938;
    if (again < 1000) again = 1000;

    if (again >= 8000)
    {
        regValue[0] = 0x30;//x8
        regValue[1] = again*128/8000;
    }
    else if (again >= 5333)
    {
        regValue[0] = 0x28;//5.3
        regValue[1] = again*128/5333;
    }
    else if (again >= 4000)
    {
        regValue[0] = 0x20;//4
        regValue[1] = again*128/4000;
    }
    else if (again >= 2666)
    {
        regValue[0] = 0x18;//2.6
        regValue[1] = again*128/2666;
    }
    else if (again >= 2000)
    {
        regValue[0] = 0x10;//2
        regValue[1] = again*128/2000;
    }
    else if (again >= 1333)
    {
        regValue[0] = 0x08;//1.3
        regValue[1] = again*128/1333;
    }
    else
    {
        regValue[0] = 0x00;
        regValue[1] = again*128/1000;
    }
    //printf("analog:%d\n",regValue);
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, 2);  
    if(status!=OSA_SOK) 
    {
        OSA_ERROR("I2C write error\n");
        return status;
    }
    OSA_waitMsecs(10);
    
    return OSA_SOK;
}

#if 0
int DRV_imgsSetEshutter_AR0330(Uint32 eshutterInUsec, int setRegDirect)
{
    int status;
    Uint16 regAddr;
    Uint16 regValue;
    regAddr =AR0331_COARSE_IT_TIME_A;
    regValue = (float)(eshutterInUsec/ROW_TIME);
    // regValue = 0x134; // 8333/27
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if(status!=OSA_SOK) 
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
  
#ifdef AEWB_DEBUG
    OSA_printf("eshutterInUsec:%d\n",eshutterInUsec);
#endif

    return status;
}

#else
int DRV_imgsSetEshutter_AR0330(Uint32 eshutterInUsec, int setRegDirect)
{
    int status;
    Uint16 regValue;
    Uint16 regAddr;  

    regAddr  = AR0331_COARSE_IT_TIME_A;
    regValue = (float)(eshutterInUsec/ROW_TIME);
    if (setRegDirect)
    {
        regValue = (float)(eshutterInUsec/ROW_TIME);
    }
    else
    {
        DRV_imgsCalcSW_AR0330(eshutterInUsec);
        regValue = gDRV_imgsObj.curFrameTime.SW;
    }

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if(status!=OSA_SOK) 
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    return status;
}
#endif

int AR0331_HDR_Enable_AR0330(int enable)
{
    return OSA_SOK;
}

int DRV_imgsGetAgain_AR0330(int *again)
{
    return OSA_SOK;
}

int DRV_imgsGetEshutter_AR0330(Uint32 *eshutterInUsec)
{
    return OSA_SOK;
}

int DRV_imgsSetDcSub_AR0330(Uint32 dcSub, int setRegDirect)
{
    return OSA_SOK;
}

int DRV_imgsEnable_AR0330(Bool enable)
{
    int status = OSA_SOK;
    if (enable) 
    {
        status = DRV_imgsSetRegs_AR0330();
        if(status != OSA_SOK) 
        {
            OSA_ERROR("DRV_imgsSetRegs()\n");
            return status;
        }
    }
    return OSA_SOK;
}

DRV_ImgsModeConfig      *DRV_imgsGetModeConfig_AR0330(int sensorMode)
{
    return &gDRV_imgsObj.curModeConfig;
}

DRV_ImgsIsifConfig      *DRV_imgsGetIsifConfig_AR0330(int sensorMode)
{
    return &gDRV_imgsIsifConfig_Common_AR0330;
}

DRV_ImgsIpipeConfig     *DRV_imgsGetIpipeConfig_AR0330(int sensorMode, int vnfDemoCfg)
{
  if(vnfDemoCfg)
  	return &gDRV_imgsIpipeConfig_Vnfdemo_AR0330;
  else
  	return &gDRV_imgsIpipeConfig_Common_AR0330;
}

DRV_ImgsH3aConfig       *DRV_imgsGetH3aConfig_AR0330(int sensorMode, int aewbVendor)
{
  if(aewbVendor==1) {
  	return &gDRV_imgsH3aConfig_Appro_AR0330;
  }
  else if(aewbVendor==2) {
  	return &gDRV_imgsH3aConfig_TI_AR0330;
  }
  else {
  	return &gDRV_imgsH3aConfig_Common_AR0330;
  }
}

DRV_ImgsLdcConfig       *DRV_imgsGetLdcConfig_AR0330(int sensorMode, Uint16 ldcInFrameWidth, Uint16 ldcInFrameHeight)
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

int DRV_imgsReset_AR0330()
{
    return OSA_SOK;
}

int DRV_imgsSetRegs_AR0330()
{
    DRV_ImgsModeConfig *pModeCfg = &gDRV_imgsObj.curModeConfig;

    int status,i=0;
    int j;
  
    if (sensorMode == DRV_IMGS_SENSOR_MODE_2176x1296)
    {
        for (j=0; j < sizeof(AR0330_2176_1296)/sizeof(unsigned short); j=j+2)
        {
            status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &AR0330_2176_1296[j], &AR0330_2176_1296[j+1], 1);  
	        if(status != OSA_SOK)
	        {
	            OSA_printf("I2C write Error, index:%d\n",j);
	            return status;
	        }
            if (AR0330_2176_1296[j] == 0x301A)	
                OSA_waitMsecs(200);
            else
                OSA_waitMsecs(1);	
        } 

        OSA_printf("AR0330_2176_1296 Settings: Regs %d\n", j/2);
    } 
    else if (sensorMode == DRV_IMGS_SENSOR_MODE_1920x1080)
    {
        for (j=0; j < sizeof(AR0330_1920_1080)/sizeof(unsigned short); j=j+2)
        {
            status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &AR0330_1920_1080[j], &AR0330_1920_1080[j+1], 1);  
	        if(status != OSA_SOK)
	        {
	            OSA_printf("I2C write Error, index:%d\n",j);
	            return status;
	        }
            if (AR0330_1920_1080[j] == 0x301A)	
                OSA_waitMsecs(200);
            else
                OSA_waitMsecs(1);	
        } 

        OSA_printf("AR0330_1920_1080 Settings: Regs %d\n", j/2);
    } 
    else
    {
        for (j=0; j < sizeof(AR0330_2048_1536_20fps)/sizeof(unsigned short); j=j+2)
        {
            status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &AR0330_2048_1536_20fps[j], &AR0330_2048_1536_20fps[j+1], 1);  
	        if(status != OSA_SOK)
	        {
	            OSA_printf("I2C write Error, index:%d\n",j);
	            return status;
	        }
            if(AR0330_2048_1536_20fps[j] == 0x301A)	
                OSA_waitMsecs(200);
            else
                OSA_waitMsecs(1);	
        }
        OSA_printf("AR0330_2048_1536_20fps Settings: Regs %d\n", j/2);
    } 

    return OSA_SOK;
}

DRV_ImgsFuncs  AR0330ImgsFuncs = 
{
    .imgsOpen            = DRV_imgsOpen_AR0330,
    .imgsClose           = DRV_imgsClose_AR0330,
    .imgsSetMirror       = DRV_imgsSetMirror_AR0330,
    .imgsGetImagerName   = DRV_imgsGetImagerName_AR0330,
    .imgsSetAgain        = DRV_imgsSetAgain_AR0330,
    .imgsSetDgain        = DRV_imgsSetDgain_AR0330,
    .imgsSetEshutter     = DRV_imgsSetEshutter_AR0330,
    .imgsSetDcSub        = DRV_imgsSetDcSub_AR0330,
    .imgsBinEnable       = DRV_imgsBinEnable_AR0330,
    .imgsBinMode         = DRV_imgsBinMode_AR0330,
    .imgsSetFramerate    = DRV_imgsSetFramerate_AR0330,
    .imgsSet50_60Hz      = DRV_imgsSet50_60Hz_AR0330, 
    .imgsEnable          = DRV_imgsEnable_AR0330,
    .imgsGetModeConfig   = DRV_imgsGetModeConfig_AR0330,
    .imgsGetIsifConfig   = DRV_imgsGetIsifConfig_AR0330,
    .imgsGetH3aConfig    = DRV_imgsGetH3aConfig_AR0330,
    .imgsGetIpipeConfig  = DRV_imgsGetIpipeConfig_AR0330,
    .imgsGetLdcConfig    = DRV_imgsGetLdcConfig_AR0330
};

