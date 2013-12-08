
#include "drv_imgs_TVP514X.h"
#include <drv_gpio.h>
#include <asm-arm/arch-davinci/video_evm.h>
#include <media/davinci/vid_decoder_if.h>

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
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cOpen()\n");
        return status;
    }

    do
    {
        status = DRV_imgsCheckId();
        if (status==OSA_SOK)
            break;
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
    return "TVP514X";
}

int DRV_imgsSpecificSetting(void)
{
    Uint8 regAddr[30];
    Uint8 regValue[30];
    int i, status;

    i=0;
    return status;
}

int DRV_imgsSet50_60Hz(Bool is50Hz)
{
    int fps;

    if (is50Hz)
        fps = 25;
    else
        fps = 30;

    return 0;
}
int DRV_imgsSetFramerate(int fps)
{
    return -1;
}

int DRV_imgsBinEnable(Bool enable)
{
    Uint8 regAddr[4];
    Uint8 regValue[4];
    int i, col_bin, row_bin, status;

    if (!enable)
    {
    }
    else
    {
    }
    return OSA_SOK;
}

int DRV_imgsBinStandard()
{

    Uint8 regAddr;
    Uint8 regValue;
    int i, status, std;
    enum tvp514x_mode mode;
    Uint8 lock_mask;


    regAddr  = TVP514X_VIDEO_STD;
    regValue = 0;

    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    std = regValue;
    regAddr  = TVP514X_VID_STD_STATUS;
    regValue = (std & 0x7);
    if (regValue == TVP514X_MODE_AUTO)
    {
        status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
        if (status!=OSA_SOK)
        {
            OSA_ERROR("DRV_i2cWrite8()\n");
            return status;
        }
    }
    regAddr  = TVP514X_OUTPUT1;
    regValue = 0;

    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    regAddr  = TVP514X_STATUS1;
    regValue = 0xFF;

    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    return std;
}

int DRV_imgsBinMode(int binMode)
{
    return -1;
}

int DRV_imgsSetDgain(int dgain)
{
    return OSA_SOK;
}

int DRV_imgsSetAgain(int again, int setRegDirect)
{
    static Uint8 regAddr[32];
    static Uint8 regValue[32];
    int status;
    int i;
    i = 0;
    regAddr[i]  = TVP514X_AFE_GAIN_CTRL;

    if (setRegDirect)
    {
        regValue[i] = TVP514X_AFE_GAIN_CTRL_DEFAULT;
    }
    else
    {
        regValue[i] = TVP514X_AFE_GAIN_CTRL_DEFAULT;
    }

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, 1);

    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    i = 0;
    regAddr[i] = 0x46;
    regValue[i] = 0x20;
    i++;
    regAddr[i] = 0x47;
    regValue[i] = 0x20;
    i++;
    regAddr[i] = 0x48;
    regValue[i] = 0x20;
    i++;
    regAddr[i] = 0x49;
    regValue[i] = 0x20;

    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    return status;
}

int DRV_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect)
{
    int status;
    status = OSA_SOK;
    return status;
}

//Read AGain & exposure
//return value is 8 times the true analog gain
int DRV_imgsGetAgain(int *again)
{
    int status;
    status = OSA_SOK;
    return -1;
}

int DRV_imgsGetEshutter(Uint32 *eshutterInUsec)
{
    int status;
    status = OSA_SOK;
    return -1;
}

int DRV_imgsSetDcSub(Uint32 dcSub, int setRegDirect)
{
    return -1;
}

int DRV_imgsSetFormat()
{
    static Uint8 regAddr[32];
    static Uint8 regValue[32];
    int i = 0;
    int status=OSA_SOK;

    i = 0;
    regAddr[i] = TVP514X_VBUS_ADDRESS_ACCESS0 +1;
    regValue[i] = (0x00);
    i++;

    regAddr[i] = TVP514X_VBUS_ADDRESS_ACCESS0 +2;
    regValue[i] = (0x06);
    i++;

    regAddr[i] = TVP514X_VBUS_ADDRESS_ACCESS0 +3;
    regValue[i] = (0x80);
    i++;

    regAddr[i] = TVP514X_VBUS_DATA_ACCESS_AUTO_INCR;
    regValue[i] = TVP514X_LINE_ADDRESS_DEFAULT;
    i++;

    regAddr[i] = TVP514X_VBUS_DATA_ACCESS_AUTO_INCR;
    regValue[i] = TVP514X_LINE_MODE_DEFAULT;
    i++;


    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    i = 0;
    regAddr[i] = TVP514X_VBUS_ADDRESS_ACCESS0 +1;
    regValue[i] = (0x02);
    i++;

    regAddr[i] = TVP514X_VBUS_ADDRESS_ACCESS0 +2;
    regValue[i] = (0x06);
    i++;

    regAddr[i] = TVP514X_VBUS_ADDRESS_ACCESS0 +3;
    regValue[i] = (0x80);
    i++;

    regAddr[i] = TVP514X_VBUS_DATA_ACCESS_AUTO_INCR;
    regValue[i] = TVP514X_LINE_ADDRESS_DEFAULT;
    i++;

    regAddr[i] = TVP514X_VBUS_DATA_ACCESS_AUTO_INCR;
    regValue[i] = TVP514X_LINE_MODE_DEFAULT;
    i++;


    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    i = 0;
    regAddr[i] = TVP514X_VBUS_ADDRESS_ACCESS0 +1;
    regValue[i] = (0x08);
    i++;

    regAddr[i] = TVP514X_VBUS_ADDRESS_ACCESS0 +2;
    regValue[i] = (0x06);
    i++;

    regAddr[i] = TVP514X_VBUS_ADDRESS_ACCESS0 +3;
    regValue[i] = (0x80);
    i++;

    regAddr[i] = TVP514X_VBUS_DATA_ACCESS_AUTO_INCR;
    regValue[i] = TVP514X_LINE_ADDRESS_DEFAULT;
    i++;

    regAddr[i] = TVP514X_VBUS_DATA_ACCESS_AUTO_INCR;
    regValue[i] = TVP514X_LINE_MODE_DEFAULT;
    i++;


    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }
    i = 0;
    regAddr[i] = TVP514X_VBUS_ADDRESS_ACCESS0 +1;
    regValue[i] = (0x06);
    i++;

    regAddr[i] = TVP514X_VBUS_ADDRESS_ACCESS0 +2;
    regValue[i] = (0x06);
    i++;

    regAddr[i] = TVP514X_VBUS_ADDRESS_ACCESS0 +3;
    regValue[i] = (0x80);
    i++;

    regAddr[i] = TVP514X_VBUS_DATA_ACCESS_AUTO_INCR;
    regValue[i] = TVP514X_LINE_ADDRESS_DEFAULT;
    i++;

    regAddr[i] = TVP514X_VBUS_DATA_ACCESS_AUTO_INCR;
    regValue[i] = TVP514X_LINE_MODE_DEFAULT;
    i++;


    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    return status;
}

int DRV_imgsEnable(Bool enable)
{
    static Uint8 regAddr[32];
    static Uint8 regValue[32];
    int i = 0;
    int status=OSA_SOK;

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
    enum tvp514x_mode mode = TVP514X_MODE_INV;

    i = 0;
    regAddr[i] = TVP514X_OUTPUT1;
    regValue[i] = 0;
    i++;
    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);

    i = 0;
    regAddr[i] = TVP514X_VIDEO_STD;
    regValue[i] = 0x0;
    i++;
    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    i = 0;
    regAddr[i] = TVP514X_AUTOSWT_MASK;
    regValue[i] = TVP514X_AUTOSWITCH_MASK;
    i++;

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }
    status = DRV_imgsSetFormat();
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_imgsSetFormat()\n");
        return status;
    }

    i = 0;
    regAddr[i] = 0xC0;
    regValue[i] = 0x01;
    i++;

    regAddr[i] = 0xD6;
    regValue[i] = 0x07;
    i++;

    regAddr[i] = 0xD6;
    regValue[i] = 0x07;
    i++;

    regAddr[i] = 0xD7;
    regValue[i] = 0x16;
    i++;

    regAddr[i] = 0xD8;
    regValue[i] = 0xF0;
    i++;

    regAddr[i] = 0xD9;
    regValue[i] = 0x00;
    i++;

    regAddr[i] = 0xDA;
    regValue[i] = 0xFF;
    i++;

    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
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
    if (vnfDemoCfg)
        return &gDRV_imgsIpipeConfig_Vnfdemo;
    else
        return &gDRV_imgsIpipeConfig_Common;
}

DRV_ImgsH3aConfig       *DRV_imgsGetH3aConfig(int sensorMode, int aewbVendor)
{
    if (aewbVendor==1)
    {
        return &gDRV_imgsH3aConfig_Appro;
    }
    else if (aewbVendor==2)
    {
        return &gDRV_imgsH3aConfig_TI;
    }
    else
    {
        return &gDRV_imgsH3aConfig_Common;
    }
}

DRV_ImgsLdcConfig       *DRV_imgsGetLdcConfig(int sensorMode, Uint16 ldcInFrameWidth, Uint16 ldcInFrameHeight)
{
    sensorMode &= 0xFF;

    switch (sensorMode)
    {
    
    case DRV_IMGS_SENSOR_MODE_720x480:

        if (ldcInFrameWidth==864)
            return &gDRV_imgsLdcConfig_736x480_0_VS;

        if (ldcInFrameWidth==352)
            return &gDRV_imgsLdcConfig_736x480_1_VS;

        if (ldcInFrameWidth==736)
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
    Uint8 regAddr[8];
    Uint8 regValue[8];
    int status, i;

    i=0;

    regAddr[i]  = TVP514X_OPERATION_MODE;
    regValue[i] = TVP514X_OPERATION_MODE_RESET;

    regAddr[i]  = TVP514X_OPERATION_MODE;
    regValue[i] = TVP514X_OPERATION_MODE_DEFAULT;
    i++;

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }
    return status;
}

int DRV_imgsCheckId()
{
    int status;
    Uint8  regAddr;
    Uint8 regValue;

    regAddr = TVP514X_INPUT_SEL;
    regValue = 0;

    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cRead8()\n");
        return status;
    }

    regAddr = TVP514X_INPUT_SEL;
    regValue = 0x05;

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cRead8()\n");
        return status;
    }

    regAddr = TVP514X_INPUT_SEL;
    regValue = 0;

    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cRead8()\n");
        return status;
    }
    if (regValue!=TVP514X_COMPOSITE_INPUT)
        return OSA_EFAIL;

    return OSA_SOK;
}

int DRV_imgsSetRegs()
{
    int i, status;
    static Uint8 regAddr[32],mode;
    static Uint8 regValue[32];
    DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;

    status = DRV_imgsReset();
    if (status!=OSA_SOK)
        return status;

    i=0;
    regAddr[i]  = 0xE8;
    regValue[i] = 0x02;
    i++;

    regAddr[i]  = 0xE9;
    regValue[i] = 0x00;
    i++;

    regAddr[i]  = 0xEA;
    regValue[i] = 0x80;
    i++;

    regAddr[i]  = 0xE0;
    regValue[i] = 0x01;
    i++;

    regAddr[i]  = 0xE8;
    regValue[i] = 0x60;
    i++;

    regAddr[i]  = 0xE9;
    regValue[i] = 0x00;
    i++;

    regAddr[i]  = 0xEA;
    regValue[i] = 0xB0;
    i++;

    regAddr[i]  = 0xE0;
    regValue[i] = 0x01;
    i++;

    regAddr[i]  = 0xE8;
    regValue[i] = 0x16;
    i++;

    regAddr[i]  = 0xE9;
    regValue[i] = 0x00;
    i++;

    regAddr[i]  = 0xEA;
    regValue[i] = 0xA0;
    i++;

    regAddr[i]  = 0xE0;
    regValue[i] = 0x16;
    i++;

    regAddr[i]  = 0xE8;
    regValue[i] = 0x60;
    i++;

    regAddr[i]  = 0xE9;
    regValue[i] = 0x00;
    i++;

    regAddr[i]  = 0xEA;
    regValue[i] = 0xB0;
    i++;

    regAddr[i]  = 0xE0;
    regValue[i] = 0x00;
    i++;

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    i=0;
    regAddr[i]  = TVP514X_AFE_GAIN_CTRL;
    regValue[i] = TVP514X_AFE_GAIN_CTRL_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_COLOR_KILLER;
    regValue[i] = TVP514X_COLOR_KILLER_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_LUMA_CONTROL1;
    regValue[i] = TVP514X_LUMA_CONTROL1_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_LUMA_CONTROL2;
    regValue[i] = TVP514X_LUMA_CONTROL2_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_LUMA_CONTROL3;
    regValue[i] = TVP514X_LUMA_CONTROL3_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_BRIGHTNESS;
    regValue[i] = TVP514X_BRIGHTNESS_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_CONTRAST;
    regValue[i] = TVP514X_CONTRAST_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_SATURATION;
    regValue[i] = TVP514X_SATURATION_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_HUE;
    regValue[i] = TVP514X_HUE_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_CHROMA_CONTROL1;
    regValue[i] = TVP514X_CHROMA_CONTROL1_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_CHROMA_CONTROL2;
    regValue[i] = TVP514X_CHROMA_CONTROL2_DEFAULT;
    i++;

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    i=0;
    regAddr[i]  = TVP514X_OUTPUT5;
    regValue[i] = TVP514X_OUTPUT5_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_OUTPUT6;
    regValue[i] = TVP514X_OUTPUT6_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_OUTPUT1;
    regValue[i] = TVP514X_OUTPUT1_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_OUTPUT2;
    regValue[i] = TVP514X_OUTPUT2_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_OUTPUT3;
    regValue[i] = TVP514X_OUTPUT3_DEFAULT;
    i++;

    regAddr[i]  = TVP514X_OUTPUT4;
    regValue[i] = TVP514X_OUTPUT4_DEFAULT;
    i++;

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }

    i = 0;
    regAddr[i] = TVP514X_INPUT_SEL;
    regValue[i] = TVP514X_COMPOSITE_INPUT;
    i++;

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);

    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }
    OSA_waitMsecs(500);

    i = 0;
    regAddr[i] = TVP514X_CLEAR_LOST_LOCK;
    regValue[i] = 0x01;
    i++;

    status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);

    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }
    OSA_waitMsecs(500);

    i = 0;
    regAddr[i] = TVP514X_STATUS1;
    regValue[i] = 0;
    mode = 0;
    i++;

    status = DRV_i2cRead8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);

    mode = regValue[i-1];

    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }
    if (TVP514X_LOST_LOCK_MASK == (mode & TVP514X_LOST_LOCK_MASK))
    {

        OSA_ERROR("DRV_i2cWrite8()\n");
        return status;
    }
    DRV_imgsSetAgain(1000, 0); // default aGain

    return status;
}
