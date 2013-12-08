
#include "drv_imgs_OV2715_2MP.h"
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
    if (status!=OSA_SOK)
    {
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

    do
    {
        status = DRV_imgsCheckId();
        if (status==OSA_SOK)
            break;
        OSA_waitMsecs(10);
    } while (retry--);

    if (status!=OSA_SOK)
    {
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
    if ( is50Hz )
    {
        i = 0;
        regAddr[i] = TIMING_CONTROL_HTS_HIGHBYTE; 
        regValueSend[i] = 0x0B;         i++;
        regAddr[i] = TIMING_CONTROL_HTS_LOWBYTE; 
        regValueSend[i] = 0x58;         i++;
    }
    else
    {
        i = 0;
        regAddr[i] = TIMING_CONTROL_HTS_HIGHBYTE; 
        regValueSend[i] = 0x09;         i++;
        regAddr[i] = TIMING_CONTROL_HTS_LOWBYTE; 
        regValueSend[i] = 0x74;         i++;
    }

    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValueSend, i);    
    if (status!=OSA_SOK)
    {
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

int DRV_imgsSetEshutterAGain(Uint32 eshutterInUsec, int again)
{
    int status,i;
    Uint16 regAddr[20];
    int regValue=0; 
    Uint8 regValueSend[20]; 
    int regValue2=0, regValue3=0; 

    DRV_imgsCalcSW(eshutterInUsec);  
    regValue = gDRV_imgsObj.curFrameTime.SW;
    regValue2 = regValue/16;
    if ( regValue2 > 0x44a )
    {
        regValue2 = regValue2 - 0x44a;
    }
    else
    {
        regValue2 = 0;
    }

    i = 0;
    //group sel
    regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0x00;         i++;

    //exp
    regAddr[i] = AEC_PK_EXPO1; regValueSend[i] = (regValue>>16)&0xff;    i++;
    regAddr[i] = AEC_PK_EXPO2; regValueSend[i] = (regValue>>8)&0xff;     i++;  
    regAddr[i] = AEC_PK_EXPO3; regValueSend[i] = (regValue)&0xff;        i++;
    regAddr[i] = AEC_PK_VTS_H; regValueSend[i] = (regValue2>>8)&0xff;    i++;
    regAddr[i] = AEC_PK_VTS_L; regValueSend[i] = (regValue2)&0xff;       i++;

    //A gain
    regValue3 = DRV_imgsCalcAgain(again);
    regAddr[i] = AEC_AGC_ADJ_L; regValueSend[i] = (regValue3)&0xff;      i++;
    regAddr[i] = AEC_AGC_ADJ_H; regValueSend[i] = 0;                     i++;

    //group end
    regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0x10;         i++;
    regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0xA0;         i++;

    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValueSend, i);  
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite16()\n");
        return status;
    }
    return status;

}


int DRV_imgsSetAgain(int again, int setRegDirect)
{

    Uint16 regAddr[8];
    Uint8 regValueSend[8]; 
    Uint16 regValue;
    int status,i;

    if (setRegDirect)
    {
        regValue = again;
    }
    else
    {
        regValue = DRV_imgsCalcAgain(again);
    }

    i = 0;

    //regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0x01;         i++;

    regAddr[i] = AEC_AGC_ADJ_L; regValueSend[i] = (regValue)&0xff;     i++;
    regAddr[i] = AEC_AGC_ADJ_H; regValueSend[i] = 0;                                 i++;

    //regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0x11;         i++;
    //regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0xA1;         i++;

    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValueSend, i);

    if (status!=OSA_SOK)
    {
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

    if (setRegDirect)
    {
        regValue = eshutterInUsec;
    }
    else
    {
        DRV_imgsCalcSW(eshutterInUsec);  
        regValue = gDRV_imgsObj.curFrameTime.SW;
        regValue2 = regValue/16;
        if ( regValue2 > 0x44a )
        {
            regValue2 = regValue2 - 0x44a;
        }
        else
        {
            regValue2 = 0;
        }
    }
    i = 0;
    regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0x00;         i++;

//  regAddr[i] = PLL_CTRL01; regValueSend[i] = gDRV_imgsObj.curFrameTime.div_base;    i++;
    regAddr[i] = AEC_PK_EXPO1; regValueSend[i] = (regValue>>16)&0xff;    i++;
    regAddr[i] = AEC_PK_EXPO2; regValueSend[i] = (regValue>>8)&0xff;     i++;  
    regAddr[i] = AEC_PK_EXPO3; regValueSend[i] = (regValue)&0xff;         i++;

    regAddr[i] = AEC_PK_VTS_H; regValueSend[i] = (regValue2>>8)&0xff;         i++;
    regAddr[i] = AEC_PK_VTS_L; regValueSend[i] = (regValue2)&0xff;         i++;


    regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0x10;         i++;
    regAddr[i] = GROUP_ACCESS; regValueSend[i] = 0xA0;         i++;


    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValueSend, i);  
    if (status!=OSA_SOK)
    {
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

    if (status!=OSA_SOK)
    {
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
    if (status!=OSA_SOK)
    {
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

    if (enable)
    {
        status = DRV_imgsSetRegs();
        if (status!=OSA_SOK)
        {
            OSA_ERROR("DRV_imgsSetRegs()\n");
            return status;
        }
        DRV_imgsSpecificSetting();
    }
    if ( (pFrame->f_pclk == (96*IMGS_MHZ))&&( pFrame->row_bin > 0 || pFrame->col_bin > 0))
    {
        regAddr  = OUTPUT_CTRL;
        regValue = 0x1f8C | ((enable & 0x1) << 1);
    }
    else
    {
        regAddr  = OUTPUT_CTRL;
        regValue = 0x1f80 | ((enable & 0x1) << 1);
    }

    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite16()\n");
        return status;
    }
#else
    if (enable)
    {
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
    if (vnfDemoCfg)
        return &gDRV_imgsIpipeConfig_Vnfdemo;
    else
        return &gDRV_imgsIpipeConfig_Common;
}

DRV_ImgsH3aConfig *DRV_imgsGetH3aConfig(int sensorMode, int aewbVendor)
{
    if (aewbVendor == 1)
    {
        printf("APPRO Setting Applied\n");
        return &gDRV_imgsH3aConfig_Appro;
    }
    else if (aewbVendor == 2)
    {
        printf("TI Setting Applied\n");
        return &gDRV_imgsH3aConfig_TI;
    }
    else
    {
        printf("Common Setting Applied\n");
        return &gDRV_imgsH3aConfig_Common;
    }
}

DRV_ImgsLdcConfig *DRV_imgsGetLdcConfig(int sensorMode, Uint16 ldcInFrameWidth, Uint16 ldcInFrameHeight)
{
    OSA_printf(" DRV LDC: GetConfig %04x %dx%d\n", sensorMode, ldcInFrameWidth, ldcInFrameHeight);

    sensorMode &= 0xFF;

    switch (sensorMode)
    {
#ifdef IMGS_OV2715_2MP
//    case DRV_IMGS_SENSOR_MODE_752x512:
    case DRV_IMGS_SENSOR_MODE_720x480:
//    case DRV_IMGS_SENSOR_MODE_1620x1080:            //D1
#else
    case DRV_IMGS_SENSOR_MODE_752x512:
    case DRV_IMGS_SENSOR_MODE_720x480:
    case DRV_IMGS_SENSOR_MODE_1620x1080:            //D1
#endif
        if (ldcInFrameWidth==864)
            return &gDRV_imgsLdcConfig_736x480_0_VS;

        if (ldcInFrameWidth==352)
            return &gDRV_imgsLdcConfig_736x480_1_VS;

        if (ldcInFrameWidth==736)
            return &gDRV_imgsLdcConfig_736x480_0;

        if (ldcInFrameWidth==752)
            return &gDRV_imgsLdcConfig_736x480_0;

        if (ldcInFrameWidth==288)
            return &gDRV_imgsLdcConfig_736x480_1;

        if (ldcInFrameWidth==768)
            return &gDRV_imgsLdcConfig_768x512_0;

        if (ldcInFrameWidth==320)
            return &gDRV_imgsLdcConfig_768x512_1;

        if (ldcInFrameWidth==928)
            return &gDRV_imgsLdcConfig_768x512_0_VS;

        if (ldcInFrameWidth==384)
            return &gDRV_imgsLdcConfig_768x512_1_VS;

        break;

    case DRV_IMGS_SENSOR_MODE_1280x720:
    case DRV_IMGS_SENSOR_MODE_1920x1080:            //720P / 1080P

        if (ldcInFrameWidth==1280)
            return &gDRV_imgsLdcConfig_1280x736_0;

        if (ldcInFrameWidth==320)
            return &gDRV_imgsLdcConfig_1280x736_1;

        if (ldcInFrameWidth==640)
            return &gDRV_imgsLdcConfig_1280x736_2;

        if (ldcInFrameWidth==1536)
            return &gDRV_imgsLdcConfig_1280x736_0_VS;

        if (ldcInFrameWidth==384)
            return &gDRV_imgsLdcConfig_1280x736_1_VS;

        if (ldcInFrameWidth==768)
            return &gDRV_imgsLdcConfig_1280x736_2_VS;

        if (ldcInFrameWidth==1312)
            return &gDRV_imgsLdcConfig_1312x768_0;

        if (ldcInFrameWidth==352)
            return &gDRV_imgsLdcConfig_1312x768_1;

        if (ldcInFrameWidth==672)
            return &gDRV_imgsLdcConfig_1312x768_2;

        if (ldcInFrameWidth==1600)
            return &gDRV_imgsLdcConfig_1312x768_0_VS;

        if (ldcInFrameWidth==448)
            return &gDRV_imgsLdcConfig_1312x768_1_VS;

        if (ldcInFrameWidth==832)
            return &gDRV_imgsLdcConfig_1312x768_2_VS;

        break;

    }

    return NULL;
}

int DRV_imgsReset()
{
    Uint16 regAddr[80];
    Uint8 regValue[80];
    int status, i;

    i=0;  
    regAddr[i]  = PLL_CLOCK_SELECT;                         regValue[i] = 0x93;  i++;
    regAddr[i]  = SYSTEM_CONTROL00;                     regValue[i] = 0x82;  i++;
    regAddr[i]  = PAD_OUTPUT_ENABLE01;                 regValue[i] = 0x7f;  i++;
    regAddr[i]  = PAD_OUTPUT_ENABLE02;                 regValue[i] = 0xfc;  i++;
    regAddr[i]  = TIMING_CONTROL_HS_LOWBYTE;     regValue[i] = 0xa0;  i++;
    regAddr[i]  = SENSOR_RESV1;                             regValue[i] = 0x61;  i++;
    regAddr[i]  = SENSOR_RESV2;                             regValue[i] = 0x0c;  i++;
    regAddr[i]  = SENSOR_RESV3;                             regValue[i] = 0x6d;  i++;
    regAddr[i]  = TIMING_CONTROL_HS_LOWBYTE;     regValue[i] = 0xb4;  i++;
    regAddr[i]  = ANA_ARRAY;                                     regValue[i] = 0x04;  i++;
    regAddr[i]  = SENSOR_RESV4;                             regValue[i] = 0x60;  i++;
    regAddr[i]  = SENSOR_RESV5;                             regValue[i] = 0xa7;  i++;
    regAddr[i]  = SENSOR_RESV6;                             regValue[i] = 0x26;  i++;
    regAddr[i]  = SENSOR_RESV7;                             regValue[i] = 0x04;  i++;
    regAddr[i]  = SENSOR_RESV8;                             regValue[i] = 0x37;/*0x07;*/  i++;
    regAddr[i]  = SENSOR_RESV9;                             regValue[i] = 0x00;/*0x40;*/  i++;
    regAddr[i]  = SENSOR_RESV10;                             regValue[i] = 0x9e;  i++;
    regAddr[i]  = SENSOR_RESV11;                             regValue[i] = 0x74;  i++;
    regAddr[i]  = SENSOR_RESV12;                             regValue[i] = 0x10;  i++;
    regAddr[i]  = SENSOR_REG0D;                             regValue[i] = 0x07;  i++;
    regAddr[i]  = SENSOR_RESV13;                             regValue[i] = 0x8b;  i++;
    regAddr[i]  = SENSOR_RESV14;                             regValue[i] = 0x74;  i++;
    regAddr[i]  = SENSOR_RESV15;                             regValue[i] = 0x9e;  i++;
    regAddr[i]  = TIMING_CONTROL_HS_LOWBYTE;     regValue[i] = 0xc4;  i++;
    regAddr[i]  = SENSOR_RESV16;                             regValue[i] = 0x05;  i++;
    regAddr[i]  = SENSOR_RESV17;                             regValue[i] = 0x12;  i++;
    regAddr[i]  = SENSOR_RESV18;                             regValue[i] = 0x90;  i++;
    regAddr[i]  = SENSOR_RESV19;                             regValue[i] = 0x40;  i++;
    /*updates*/
    regAddr[i]  = SENSOR_RESV24;                             regValue[i] = 0x31;  i++;

    regAddr[i]  = TIMING_CONTROL_HTS_LOWBYTE; regValue[i] = 0x74;  i++;
    regAddr[i]  = DVP_CTRL00;                                 regValue[i] = 0x04;  i++;

    regAddr[i]  = AWB_CONTROL_01;                         regValue[i] = 0x20;  i++;
    regAddr[i]  = AWB_FRAME_COUNTER;                     regValue[i] = 0x00;  i++;
    regAddr[i]  = SENSOR_RESV20;                             regValue[i] = 0xff;  i++;
    regAddr[i]  = SENSOR_RESV21;                             regValue[i] = 0x00;  i++;
    regAddr[i]  = AEC_CONTROL0;                             regValue[i] = 0x78;  i++;
    regAddr[i]  = PLL_CTRL00;                                 regValue[i] = 0x88;  i++;
    regAddr[i]  = PLL_CTRL02;                                 regValue[i] = 0x28;  i++;
    regAddr[i]  = SENSOR_RESV22;                             regValue[i] = 0x06;  i++;
    regAddr[i]  = AEC_G_CEIL_H;                             regValue[i] = 0x00;  i++;
    regAddr[i]  = AEC_G_CEIL_L;                             regValue[i] = 0x7a;  i++;
    regAddr[i]  = AEC_PK_MANUAL;                             regValue[i] = 0x00;  i++;
    regAddr[i]  = AEC_CONTROL13;                             regValue[i] = 0x54;  i++;
    /*updates*/
    regAddr[i]  = SENSOR_RESV25;                             regValue[i] = 0x0F;  i++;
    regAddr[i]  = SENSOR_RESV26;                             regValue[i] = 0x1A;  i++;    


    regAddr[i]  = SENSOR_RESV23;                             regValue[i] = 0x03;  i++;

    regAddr[i]  = AVG_START_POSITION_AT_VERTICAL_H; regValue[i] = 0x07;  i++;
    regAddr[i]  = AVG_START_POSITION_AT_VERTICAL_L; regValue[i] = 0xa0;  i++;
    regAddr[i]  = AVG_END_POSITION_AT_VERTICAL_H;     regValue[i] = 0x04;  i++;
    regAddr[i]  = AVG_END_POSITION_AT_VERTICAL_L;     regValue[i] = 0x43;  i++;

    regAddr[i]  = AEC_CONTROLF;                 regValue[i] = 0x40;  i++;
    regAddr[i]  = AEC_CONTROL10;                 regValue[i] = 0x38;  i++;
    regAddr[i]  = AEC_CONTROL1B;                 regValue[i] = 0x48;  i++;
    regAddr[i]  = AEC_CONTROL1E;                 regValue[i] = 0x30;  i++;
    regAddr[i]  = AEC_CONTROL11;                 regValue[i] = 0x90;  i++;
    regAddr[i]  = AEC_CONTROL1F;                 regValue[i] = 0x10;  i++;

    /* BLC Setting */
    regAddr[i]  = BLC_CONTROL_00;                regValue[i] = 0x01;  i++;
    regAddr[i]  = BLACK_LEVEL_TARGET_H;    regValue[i] = 0x00;  i++;
    regAddr[i]  = BLACK_LEVEL_TARGET_L;    regValue[i] = 0x00;  i++;//0x05;

    /* Disable AEC/AGC*/
    regAddr[i]  = AEC_PK_MANUAL;                             regValue[i] = 0x07;  i++;

    /* Disable AWB */
    regAddr[i]  = AWB_GAIN_PK_AWB;                         regValue[i] = 0x01;  i++;    
    regAddr[i]    = AWB_GAIN_PK_RED_H;                    regValue[i] = 0x04;  i++;
    regAddr[i]    = AWB_GAIN_PK_RED_L;                    regValue[i] = 0x00;  i++;
    regAddr[i]    = AWB_GAIN_PK_GREEN_H;                regValue[i] = 0x04;  i++;
    regAddr[i]    = AWB_GAIN_PK_GREEN_L;                regValue[i] = 0x00;  i++;
    regAddr[i]    = AWB_GAIN_PK_BLUE_H;                 regValue[i] = 0x04;  i++;
    regAddr[i]    = AWB_GAIN_PK_BLUE_L;                 regValue[i] = 0x00;  i++;



    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);  
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    if (status==OSA_SOK)
    {

        i=0;

        /*
            0x00 : 30fps, 
            0x10 : 15fps, 
            0x20 : 10fps
        */
        regAddr[i]    = PLL_CTRL01;        regValue[i] = 0x00;    i++;

        status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    }

    if (status!=OSA_SOK)
    {
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
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cRead16()\n");
        return status;  
    }

    regAddr = CHIP_VERSION_L;
    regValueL = 0;

    status = DRV_i2c16Read8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValueL, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cRead16()\n");
        return status;  
    }

    regValue = (regValueH<<8)|regValueL;

    if (regValue!=IMGS_CHIP_ID)
        return OSA_EFAIL;

    return OSA_SOK;
}

int DRV_imgsSetRegs()
{

    int status;

    status = DRV_imgsReset();
    if (status!=OSA_SOK)
        return status;

    return status;
}

int DRV_imgsAPChipInit(int *pGioC1, int *pGioD1)
{
#define PINMUX_BASE        (0x01C40000)
#define PINMUX_LENGTH    (0x8C)
#define PINMUX0_OFSET    (0x0000)
#define PINMUX1_OFSET    (0x0004)
#define PINMUX2_OFSET    (0x0008)
#define PINMUX3_OFSET    (0x000C)
#define PINMUX4_OFSET    (0x0010)
#define IMGS_APCHIP_C1_GIO    (93)
#define AP_CHIP_C1_PINMUX    (PINMUX_BASE+PINMUX0_OFSET)
#define AP_CHIP_C1_MASK    (((unsigned int)0x3)<<14)
#define AP_CHIP_C1_VAL         (((unsigned int)0x1)<<14)    
#define IMGS_APCHIP_D1_GIO    (90)
#define AP_CHIP_D1_PINMUX    (PINMUX_BASE+PINMUX1_OFSET)
#define AP_CHIP_D1_MASK    (((unsigned int)0x3)<<4)
#define AP_CHIP_D1_VAL         (((unsigned int)0x0)<<4)

    unsigned int *pPINMUXC1 = NULL;
    unsigned int *pPINMUXD1 = NULL;
    unsigned int val1, val2;
    unsigned char *pRegStart; 
    int ret = 0;

    if (gCSL_vpssHndl.regBaseVirtAddr==0 || pGioC1==0 || pGioD1==0)
    {
        return -1;
    }
    pRegStart = (unsigned char *)gCSL_vpssHndl.regBaseVirtAddr;

    pPINMUXC1    = (unsigned int *)( pRegStart+ (AP_CHIP_C1_PINMUX-CSL_SYS_BASE_PHYS_ADDR));
    pPINMUXD1    = (unsigned int *)( pRegStart+ (AP_CHIP_D1_PINMUX-CSL_SYS_BASE_PHYS_ADDR));
    /* Enable C1 */
    *pPINMUXC1 = ((*pPINMUXC1 & (~AP_CHIP_C1_MASK)) | AP_CHIP_C1_VAL);
    val1 = (*pPINMUXC1 & AP_CHIP_C1_MASK);
    if (AP_CHIP_C1_VAL!= val1)
    {
        OSA_ERROR("PINMUXC1 setting error(%X)!!!\n",val1);
        ret = -1;
        goto __DRV_imgs_APChipInit_End__;
    }
    /* Enable D1 */
    *pPINMUXD1 = (*pPINMUXD1 & (~AP_CHIP_D1_MASK)) | (AP_CHIP_D1_VAL);
    val2 = (*pPINMUXD1 & AP_CHIP_D1_MASK);
    if (AP_CHIP_D1_VAL != val2)
    {
        OSA_ERROR("PINMUXD1 setting error(%X)!!!\n",val2);
        ret = -1;
        goto __DRV_imgs_APChipInit_End__;
    }

    *pGioC1 = IMGS_APCHIP_C1_GIO;
    *pGioD1 = IMGS_APCHIP_D1_GIO;

    __DRV_imgs_APChipInit_End__:

    return ret;
}



