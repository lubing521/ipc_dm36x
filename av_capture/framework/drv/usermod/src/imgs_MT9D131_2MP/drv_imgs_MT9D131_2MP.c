
#include "drv_imgs_MT9D131_2MP.h"
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

    DRV_gpioSetMode(IMGS_RESET_GPIO, DRV_GPIO_DIR_OUT);
    DRV_gpioSet(IMGS_RESET_GPIO);
    DRV_gpioClr(IMGS_RESET_GPIO);
    OSA_waitMsecs(500);
    DRV_gpioSet(IMGS_RESET_GPIO);
    OSA_waitMsecs(100);

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
    return "MICRON_MT9D131_2MP";
}

int DRV_imgsSpecificSetting(void)
{
    Uint8  regAddr = 0;
    Uint16 regValue = 0;
    int i, status = OSA_SOK;

    // Sensor Reset
    regAddr = 0x0D; regValue = 0x0001;    //Reset
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    OSA_waitMsecs(10);
    regAddr = 0x0D; regValue = 0x0000;    //Reset done
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    OSA_waitMsecs(10);

    regAddr = 0xF0; regValue = 0x0000;    //set page0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    regAddr = 0x66; regValue = 0x1402; 
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0x67; regValue = 0x0500; 
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0x65; regValue = 0xA000;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    OSA_waitMsecs(1000); 

    regAddr = 0x65; regValue = 0x2000;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    OSA_waitMsecs(1000); 
   
    //*** YCbCr in Context B ***
    regAddr = 0xF0; regValue = 0x0001;    //set page1
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    
    regAddr = 0xC6; regValue = 0x270B;   
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0030;  
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x2774;  
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0501;  
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    OSA_waitMsecs(10); 

    //*** Set Context B Blanking to Minimum ***
    regAddr = 0xF0; regValue = 0x0000;    //set page0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    
    regAddr = 0x05; regValue = 0x011E; 
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0x06; regValue = 0x000B; 
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    
    //*** cropping the field of view to achieve higher frame rate ***
    regAddr = 0xF0; regValue = 0x0001;  //set page1
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    
    regAddr = 0xC6; regValue = 0x2709; 	// MCU_ADDRESS [MODE_OUTPUT_HEIGHT_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x02D0; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x2707; 	// MCU_ADDRESS [MODE_OUTPUT_WIDTH_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0500; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x2735; 	// MCU_ADDRESS [MODE_CROP_X0_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0000; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x2737; 	// MCU_ADDRESS [MODE_CROP_X1_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0500; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x2739; 	// MCU_ADDRESS [MODE_CROP_Y0_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0000; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x273B; 	// MCU_ADDRESS [MODE_CROP_Y1_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x02D0; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x271B; 	// MCU_ADDRESS [MODE_SENSOR_ROW_START_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x001C; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x271D; 	// MCU_ADDRESS [MODE_SENSOR_COL_START_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x003C; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x271F; 	// MCU_ADDRESS [MODE_SENSOR_ROW_HEIGHT_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x02D0; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x2721; 	// MCU_ADDRESS [MODE_SENSOR_COL_WIDTH_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0500; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x271B; 	// MCU_ADDRESS [MODE_SENSOR_ROW_START_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x00FE; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x271D; 	// MCU_ADDRESS [MODE_SENSOR_COL_START_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x00BE; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA120; 	// MCU_ADDRESS [SEQ_CAP_MODE]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0002; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA103; 	// MCU_ADDRESS [SEQ_CMD]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0002; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA217; 	// MCU_ADDRESS
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0004; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA20E; 	// MCU_ADDRESS
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0004; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA103; 	// MCU_ADDRESS
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0005; 	// MCU_DATA_0

    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA404; 	// MCU_ADDRESS [FD_MODE]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x00C1; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA12A; 	// MCU_ADDRESS [SEQ_PREVIEW_1_FD]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0002; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA103; 	// MCU_ADDRESS
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0005; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    // I/O Slew rate test
    regAddr  =  0x0A;
    status   =  DRV_i2cRead16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regValue |= ( ((unsigned short)(0x0000)) << 0 );   //10:8 DOUT slew rate 7 fastest, 0 slowest
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    // Reduce Flicker
    regAddr = 0xC6; regValue = 0xA12A;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0002;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA404;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x00C0;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA103;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0006;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    // Delay 300
    OSA_waitMsecs(300);

    regAddr = 0xC6; regValue = 0xA103;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0005;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    // return status;
    return OSA_SOK;
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
    return OSA_SOK; 
}

int DRV_imgsBinEnable(Bool enable)
{
    return OSA_SOK;
}

int DRV_imgsBinMode(int binMode)
{
    return OSA_SOK;
}


int DRV_imgsSetDgain(int dgain)
{
    return OSA_SOK;
}

int DRV_imgsSetAgain(int again, int setRegDirect)
{
    return OSA_SOK; 
}

int DRV_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect)
{
    return OSA_SOK;
}

//Read AGain & exposure
//return value is 8 times the true analog gain
int DRV_imgsGetAgain(int *again)
{
    return OSA_SOK;
}

int DRV_imgsGetEshutter(Uint32 *eshutterInUsec)
{
    return OSA_SOK; 
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

#ifdef SENSOR_REG_DUMP
    {
        unsigned char i;
        Uint8 dumpAddr[128];
        Uint16 dumpValue[128];
        FILE *dumpFile;

        for (i=0;i<128;i++)
            dumpAddr = i;

        DRV_i2cRead16(&gDRV_imgsObj.i2cHndl, dumpAddr, dumpValue, 128);
        OSA_waitMsecs(100);

        dumpFile = fopen("sensor_dump.txt","wb");
        for (i=0;i<128;i++)
            fprintf(dumpFile, "0x%x \t0x%x\n", dumpAddr, dumpValue);
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
    return OSA_SOK;
}

int DRV_imgsCheckId()
{

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
}


