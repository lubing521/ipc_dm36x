#include "drv_imgs_IMX122_2_3MP.h"
#include "drv_imgs_IMX122_REG.h"
#include <drv_gpio.h>
#include <math.h>

#define __DRV_IMGS_IMX122_DEBUG__
#ifdef __DRV_IMGS_IMX122_DEBUG__
    #define DB(fmt, args...) fprintf(stderr,fmt, ## args)
#else
    #define DB(fmt, args...)
#endif
#define DBErr(fmt, args...) fprintf(stderr,fmt, ## args)

#define IMX122_DEBUG 0
#define IMX122_SPI_DRIVER_MODE 0

//SPI0
//#define SPI_SCS_GPO     (23)
//#define SPI_SCLK_GPO    (24)
//#define SPI_SIMO_GIO    (22)
//#define SPI1_SDI_GPI     (37)

//SPI1
#define SPI_SCS_GPO     (29)
#define SPI_SCLK_GPO    (28)
#define SPI_SIMO_GIO    (26)
#define SPI_SOMI_GIO    (27)

#define PCLK          81000000
#define LINE_LENGTH   2400
#define FRAME_LENGTH  1125
#define ROW_TIME      (LINE_LENGTH / PCLK) //29.6us

DRV_ImgsObj gDRV_imgsObj;

Bool gbIsDDRIn = FALSE;
Bool gbIsAlawEnalbe = FALSE;
int gCurrsensorMode = DRV_IMGS_SENSOR_MODE_1920x1080;

void imx122_test();
int DRV_imgsSetRegs_IMX122(void);
int DRV_imgsEnable_IMX122(Bool enable);

int DRV_imgsGetDdrInMode_IMX122(void)
{
    return(int)gbIsDDRIn;
}

static Uint8 reg_read(Uint16 addr, Uint8 value)
{
    int i;
    Uint8 ret = 0;
    Uint8 chip_id      = ((Uint8)(addr >> 8) == 0x02) ? 0x82 : 0x83;   //chip_id
    Uint8 reg_addr     = (Uint8)(addr & 0xFF);
    Uint8 imx122_value = value;

    // CS low
    DRV_gpioClr(SPI_SCS_GPO);
    DRV_gpioClr(SPI_SCS_GPO);
    DRV_gpioClr(SPI_SCS_GPO);
    DRV_gpioClr(SPI_SCS_GPO);

    /*chipid*/
    for (i=0; i<8; i++)
    {
        DRV_gpioClr(SPI_SCLK_GPO);

        if (chip_id & 0x01)
            DRV_gpioSet(SPI_SIMO_GIO);
        else
            DRV_gpioClr(SPI_SIMO_GIO);

        DRV_gpioSet(SPI_SCLK_GPO);
        chip_id = chip_id >> 1;
    }
    /*reg address*/
    for (i=0; i<8; i++)
    {
        DRV_gpioClr(SPI_SCLK_GPO);

        if (reg_addr & 0x01)
            DRV_gpioSet(SPI_SIMO_GIO);
        else
            DRV_gpioClr(SPI_SIMO_GIO);

        DRV_gpioSet(SPI_SCLK_GPO);
        reg_addr = reg_addr >> 1;
    }
    /*data*/
    ret = 0;
    for (i=0; i<8; i++)
    {
        DRV_gpioClr(SPI_SCLK_GPO);
        //write data
        if (imx122_value & 0x01)
            DRV_gpioSet(SPI_SIMO_GIO);
        else
            DRV_gpioClr(SPI_SIMO_GIO);

        DRV_gpioSet(SPI_SCLK_GPO);

        imx122_value = imx122_value >> 1;

        //read data
        ret >>= 1;
        if (DRV_gpioGet(SPI_SOMI_GIO) != 0)
        {
            ret |= 0x80;
        }
    }
    DRV_gpioSet(SPI_SCLK_GPO);
    // CS high
    DRV_gpioSet(SPI_SCS_GPO);
    DRV_gpioSet(SPI_SCS_GPO);
    DRV_gpioSet(SPI_SCS_GPO);
    DRV_gpioSet(SPI_SCS_GPO);

    return ret;
}

static Uint8 reg_write(Uint16 addr, Uint8 value)
{
    int i;
    Uint8 ret = 0;
    Uint8 chip_id      = (Uint8)(addr >> 8);
    Uint8 reg_addr     = (Uint8)(addr & 0xFF);
    Uint8 imx122_value = value;

    // CS low
    DRV_gpioClr(SPI_SCS_GPO);
    DRV_gpioClr(SPI_SCS_GPO);
    DRV_gpioClr(SPI_SCS_GPO);
    DRV_gpioClr(SPI_SCS_GPO);

    /*chipid*/
    for (i=0; i<8; i++)
    {
        DRV_gpioClr(SPI_SCLK_GPO);

        if (chip_id & 0x01)
            DRV_gpioSet(SPI_SIMO_GIO);
        else
            DRV_gpioClr(SPI_SIMO_GIO);

        DRV_gpioSet(SPI_SCLK_GPO);
        chip_id = chip_id >> 1;
    }
    /*reg address*/
    for (i=0; i<8; i++)
    {
        DRV_gpioClr(SPI_SCLK_GPO);

        if (reg_addr & 0x01)
            DRV_gpioSet(SPI_SIMO_GIO);
        else
            DRV_gpioClr(SPI_SIMO_GIO);

        DRV_gpioSet(SPI_SCLK_GPO);
        reg_addr = reg_addr >> 1;
    }
    /*data*/
    ret = 0;
    for (i=0; i<8; i++)
    {
        DRV_gpioClr(SPI_SCLK_GPO);
        //write data
        if (imx122_value & 0x01)
            DRV_gpioSet(SPI_SIMO_GIO);
        else
            DRV_gpioClr(SPI_SIMO_GIO);

        DRV_gpioSet(SPI_SCLK_GPO);

        imx122_value = imx122_value >> 1;

        //read data
        ret >>= 1;
        if (DRV_gpioGet(SPI_SOMI_GIO) != 0)
        {
            ret |= 0x80;
        }
    }
    DRV_gpioSet(SPI_SCLK_GPO);
    // CS high
    DRV_gpioSet(SPI_SCS_GPO);
    DRV_gpioSet(SPI_SCS_GPO);
    DRV_gpioSet(SPI_SCS_GPO);
    DRV_gpioSet(SPI_SCS_GPO);

    return ret;
}

Uint8 imx122_read(DRV_SPIHndl *hndl, Uint16 addr)
{
    Uint8 value = 0;
    
#if (IMX122_SPI_DRIVER_MODE == 0)
    value = reg_read(addr, 0xFF);
#else
    Uint8 regset[3];
    Uint8 data[3];
    regset[0] = ((Uint8)(addr >> 8) == 0x02) ? 0x82 : 0x83;   //chip_id
    regset[1] = (Uint8)(addr & 0xFF); //reg_addr
    regset[2] = 0xFF;                 //value
    DRV_SPIRead8(hndl, regset, 3, data);
    value = data[2];
#endif
    return value;
}

int imx122_write(DRV_SPIHndl *hndl, Uint16 addr, Uint8 value)
{
    int ret = -1;
#if (IMX122_SPI_DRIVER_MODE == 0)
    reg_write(addr, value);
    ret = 0;
#else
    Uint8 regset[3];
    regset[0] = (Uint8)(addr >> 8);   //chip_id
    regset[1] = (Uint8)(addr & 0xFF); //reg_addr
    regset[2] = value;        //value
    ret = DRV_SPIWrite8(hndl, regset, 3);
#endif
    return ret;
}

int InitRegTable(void)
{
    int i = 0;

    //imx122_write(&gDRV_imgsObj.spiHndl, 0x0200, 0x31); //standy mode
    //OSA_waitMsecs(100);

    for (i = 0; i < (sizeof(IMX122_SETTING) / sizeof(unsigned short)); i = i+2)
    {
        imx122_write(&gDRV_imgsObj.spiHndl, IMX122_SETTING[i], IMX122_SETTING[i+1]);
    }

    OSA_waitMsecs(100);
    imx122_write(&gDRV_imgsObj.spiHndl, 0x0200, 0x30); //cancel standy mode
    OSA_waitMsecs(100);

#if (IMX122_DEBUG == 1)
    OSA_printf("---------------imx122 reg read test---------------\n");
    Uint16 addr = 0;
    Uint8 value_w = 0;
    Uint8 value_r = 0;
    for (i = 0; i < (sizeof(IMX122_SETTING) / sizeof(unsigned short)); i = i+2)
    {
        addr = (Uint16)IMX122_SETTING[i];
        value_r = imx122_read(&gDRV_imgsObj.spiHndl, addr);
        value_w = (Uint8)IMX122_SETTING[i+1];
        if (value_r != value_w)
        {
            OSA_printf("0x%04x = 0x%02x -> 0x%02x\n", addr, value_r, value_w);
        }
        else
        {
            OSA_printf("0x%04x = 0x%02x\n", addr, value_r);
        }
    }
#endif
    return 0;
}

void imx122_test()
{
#if (IMX122_SPI_DRIVER_MODE == 0)
    DRV_gpioSetMode(IMGS_RESET_GPIO, DRV_GPIO_DIR_OUT);
    while (1)
    {
        OSA_printf("---------------DRV_gpioClr\n");
        DRV_gpioClr(SPI_SCS_GPO);
        DRV_gpioClr(SPI_SCLK_GPO);
        DRV_gpioClr(SPI_SIMO_GIO);
        DRV_gpioClr(IMGS_RESET_GPIO);

        OSA_waitMsecs(1000);

        OSA_printf("---------------DRV_gpioSet\n");
        DRV_gpioSet(SPI_SCS_GPO);
        DRV_gpioSet(SPI_SCLK_GPO);
        DRV_gpioSet(SPI_SIMO_GIO);
        DRV_gpioSet(IMGS_RESET_GPIO);
        OSA_waitMsecs(1000);
    }
#endif
}

static void GPIO_init(void)
{
    int status = 0;
    int value32 = 0;
    
    DB("############SPI1 SOFT GPIO SET###########\n");

    DRV_gpioSetMode(IMGS_RESET_GPIO, DRV_GPIO_DIR_OUT);
    DRV_gpioClr(IMGS_RESET_GPIO);
    OSA_waitMsecs(100);
    DRV_gpioSet(IMGS_RESET_GPIO);
    OSA_waitMsecs(100);

    // init PINMUX3  GIO26 SPI1_SIMO
    status = CSL_gpioGetPinmux(&gCSL_gpioHndl, 3, &value32);
    if (status==OSA_SOK)
    {
        //DB("-----------------original pinmux3 = 0x%x\n", value32);
        value32 &= 0x7fffffff;
        status = CSL_gpioSetPinmux(&gCSL_gpioHndl, 3, value32);
    }

    // init PINMUX4  GIO27 28 29
    status = CSL_gpioGetPinmux(&gCSL_gpioHndl, 4, &value32);
    if (status==OSA_SOK)
    {
        //DB("-----------------original pinmux4 = 0x%x\n", value32);
        value32 &= 0xffffffc0;
        status = CSL_gpioSetPinmux(&gCSL_gpioHndl, 4, value32);
    }

    DRV_gpioSetMode(SPI_SCS_GPO, DRV_GPIO_DIR_OUT);
    DRV_gpioSetMode(SPI_SCLK_GPO, DRV_GPIO_DIR_OUT);
    DRV_gpioSetMode(SPI_SIMO_GIO, DRV_GPIO_DIR_OUT);
    DRV_gpioSetMode(SPI_SOMI_GIO, DRV_GPIO_DIR_IN);

    DRV_gpioSet(SPI_SCS_GPO);
    DRV_gpioSet(SPI_SCLK_GPO);
    DRV_gpioSet(SPI_SIMO_GIO);
}

int DRV_imgsOpen_IMX122(DRV_ImgsConfig *config)
{
    int status = 0;
    Uint16 width, height;
    int value32;

    memset(&gDRV_imgsObj, 0, sizeof(gDRV_imgsObj));

    DRV_imgGetWidthHeight(config->sensorMode, &width, &height);

    width  += IMGS_H_PAD;
    height += IMGS_V_PAD;

#if 1
    DRV_imgsCalcFrameTime_IMX122(config->fps, width, height, config->binEnable);
#else
    DRV_imgsCalcFrameTime(config->fps, width, height, config->binEnable);
    gDRV_imgsObj.curModeConfig.sensorDataWidth   = width;
    gDRV_imgsObj.curModeConfig.sensorDataHeight  = height;//That's the isif Vdint
    gDRV_imgsObj.curModeConfig.validStartX       = IMGS_H_PAD/2;
    gDRV_imgsObj.curModeConfig.validStartY       = IMGS_V_PAD/2;
    gDRV_imgsObj.curModeConfig.validWidth        = width-IMGS_H_PAD;
    gDRV_imgsObj.curModeConfig.validHeight       = height-IMGS_V_PAD;
    gDRV_imgsObj.curModeConfig.binEnable         = 0;
#endif

    /* Sensor reset */
    DRV_gpioSetMode(IMGS_RESET_GPIO, DRV_GPIO_DIR_OUT);
    DRV_gpioClr(IMGS_RESET_GPIO);
    OSA_waitMsecs(100);
    DRV_gpioSet(IMGS_RESET_GPIO);
    OSA_waitMsecs(100);

#if (IMX122_SPI_DRIVER_MODE == 0)
    GPIO_init();
#else
    DB("####DRV_SPI Open\n",status);
    status = DRV_SPIOpen(&gDRV_imgsObj.spiHndl, 0); //devAddr unused in DRV_SPIOpen
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_spiOpen()\n");
        return status;
    }  
#endif
    
    //gbIsDDRIn = config->bIsDdrIn;
    //gbIsAlawEnalbe = config->bIsALawEnable;
    return 0;
}

int DRV_imgsClose_IMX122()
{
    int status = DRV_imgsEnable_IMX122(FALSE);
    return status;
}

char* DRV_imgsGetImagerName_IMX122()
{
    return "SONY_IMX122_2_3MP";
}

int DRV_imgsSpecificSetting_IMX122(void)
{
    return OSA_SOK;
}

int DRV_imgsSet50_60Hz_IMX122(Bool is50Hz)
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
        DRV_imgsSetFramerate_IMX122(fps);
    }

    return OSA_SOK;
}

int DRV_imgsSetFramerate_IMX122(int fps)
{  
    switch(fps)
    {
      case 30:
         imx122_write(&gDRV_imgsObj.spiHndl, 0x0203, 0x4C);
         imx122_write(&gDRV_imgsObj.spiHndl, 0x0204, 0x04);
         break;
      case 25:
         imx122_write(&gDRV_imgsObj.spiHndl, 0x0203, 0x28);
         imx122_write(&gDRV_imgsObj.spiHndl, 0x0204, 0x05);
         break;
      default: 
         imx122_write(&gDRV_imgsObj.spiHndl, 0x0203, 0x28);
         imx122_write(&gDRV_imgsObj.spiHndl, 0x0204, 0x05);
         break;
    }

    return OSA_SOK;		
}

int DRV_imgsBinEnable_IMX122(Bool enable)
{
    return OSA_EFAIL;
}

int DRV_imgsBinMode_IMX122(int binMode)
{
    return OSA_SOK;
}

//return value is 8 times the true analog gain
int DRV_imgsGetAgain_IMX122(int *again)
{
    /* Not support */
    return OSA_EFAIL;
}

int DRV_imgsGetEshutter_IMX122(Uint32 *eshutterInUsec)
{
    /* Not support */
    return OSA_EFAIL;
}

int DRV_imgsSetOB_IMX122(int Level)
{
    return OSA_SOK;
}

int DRV_imgsSetDcSub_IMX122(Uint32 dcSub, int setRegDirect)
{
    return OSA_SOK;
}

int DRV_imgsEnable_IMX122(Bool enable)
{
    int status = -1;

    if (enable)
    {
        DB("\n#####IMX122 Sensor RAW OUTPUT Enable\n\n",status);
        status = DRV_imgsSetRegs_IMX122();
    }

    return OSA_SOK;
}

int DRV_imgsGetLightSensorMode_IMX122(void)
{
    /* return Low light:1, Normal:0 */
    /* LS:Normal 1, low light 0 */
    return OSA_SOK;
}

int DRV_imgs_IsDDRIn_IMX122(void)
{
    return gbIsDDRIn;
}

int DRV_imgs_IsAlawEnable_IMX122(void)
{
    return gbIsAlawEnalbe;
}

int DRV_imgsNDShutter_IMX122(int bIsNight)
{
    return OSA_SOK;
}

DRV_ImgsModeConfig *DRV_imgsGetModeConfig_IMX122(int sensorMode)
{
    return &gDRV_imgsObj.curModeConfig;
}

DRV_ImgsIsifConfig *DRV_imgsGetIsifConfig_IMX122(int sensorMode)
{
    return &gDRV_imgsIsifConfig_Common_IMX122;
}

DRV_ImgsIpipeConfig *DRV_imgsGetIpipeConfig_IMX122(int sensorMode, int vnfDemoCfg)
{
    if (vnfDemoCfg)
        return &gDRV_imgsIpipeConfig_Vnfdemo_IMX122;
    else
        return &gDRV_imgsIpipeConfig_Common_IMX122;
}

DRV_ImgsH3aConfig *DRV_imgsGetH3aConfig_IMX122(int sensorMode, int aewbVendor)
{
    if (aewbVendor == 1)
    {
        printf("APPRO Setting Applied\n");
        return &gDRV_imgsH3aConfig_Appro_IMX122;
    }
    else if (aewbVendor == 2)
    {
        printf("TI Setting Applied\n");
        return &gDRV_imgsH3aConfig_TI_IMX122;
    }
    else
    {
        printf("Common Setting Applied\n");
        return &gDRV_imgsH3aConfig_Common_IMX122;
    }
}

DRV_ImgsLdcConfig *DRV_imgsGetLdcConfig_IMX122(int sensorMode, Uint16 ldcInFrameWidth, Uint16 ldcInFrameHeight)
{
    //OSA_printf(" DRV LDC: GetConfig %04x %dx%d\n", sensorMode, ldcInFrameWidth, ldcInFrameHeight);
    sensorMode &= 0xFF;
    switch (sensorMode)
    {
    case DRV_IMGS_SENSOR_MODE_720x480:
    case DRV_IMGS_SENSOR_MODE_1620x1080:
        if (ldcInFrameWidth == 864)
            return &gDRV_imgsLdcConfig_736x480_0_VS_IMX122;

        if (ldcInFrameWidth == 352)
            return &gDRV_imgsLdcConfig_736x480_1_VS_IMX122;

        if (ldcInFrameWidth == 736)
            return &gDRV_imgsLdcConfig_736x480_0_IMX122;

        if (ldcInFrameWidth == 288)
            return &gDRV_imgsLdcConfig_736x480_1_IMX122;

        if (ldcInFrameWidth == 768)
            return &gDRV_imgsLdcConfig_768x512_0_IMX122;

        if (ldcInFrameWidth == 320)
            return &gDRV_imgsLdcConfig_768x512_1_IMX122;

        if (ldcInFrameWidth == 928)
            return &gDRV_imgsLdcConfig_768x512_0_VS_IMX122;

        if (ldcInFrameWidth == 384)
            return &gDRV_imgsLdcConfig_768x512_1_VS_IMX122;

        break;

    case DRV_IMGS_SENSOR_MODE_1280x720:
    case DRV_IMGS_SENSOR_MODE_1920x1080:

        if (ldcInFrameWidth == 1280)
            return &gDRV_imgsLdcConfig_1280x736_0_IMX122;

        if (ldcInFrameWidth == 320)
            return &gDRV_imgsLdcConfig_1280x736_1_IMX122;

        if (ldcInFrameWidth == 640)
            return &gDRV_imgsLdcConfig_1280x736_2_IMX122;

        if (ldcInFrameWidth == 1536)
            return &gDRV_imgsLdcConfig_1280x736_0_VS_IMX122;

        if (ldcInFrameWidth == 384)
            return &gDRV_imgsLdcConfig_1280x736_1_VS_IMX122;

        if (ldcInFrameWidth == 768)
            return &gDRV_imgsLdcConfig_1280x736_2_VS_IMX122;

        if (ldcInFrameWidth == 1312)
            return &gDRV_imgsLdcConfig_1312x768_0_IMX122;

        if (ldcInFrameWidth == 352)
            return &gDRV_imgsLdcConfig_1312x768_1_IMX122;

        if (ldcInFrameWidth == 672)
            return &gDRV_imgsLdcConfig_1312x768_2_IMX122;

        if (ldcInFrameWidth == 1600)
            return &gDRV_imgsLdcConfig_1312x768_0_VS_IMX122;

        if (ldcInFrameWidth == 448)
            return &gDRV_imgsLdcConfig_1312x768_1_VS_IMX122;

        if (ldcInFrameWidth == 832)
            return &gDRV_imgsLdcConfig_1312x768_2_VS_IMX122;

        break;

    }

    return NULL;
}

int DRV_imgsReset_IMX122()
{
    return OSA_SOK;
}

int DRV_imgsCheckId_IMX122(DRV_SPIHndl *hndl)
{
    Uint16 addr = 0x0201;
    Uint8 value_w = 0x80;
    Uint8 value_r = 0;

#if (IMX122_SPI_DRIVER_MODE == 0)
    GPIO_init();
#endif

    imx122_write(hndl, addr, value_w);
    value_r = imx122_read(hndl, addr);
    if (value_r != value_w)
    {
        DB("[0x%04x] W:0x%02x, R:0x%02x\n", addr, value_w, value_r);
        return OSA_EFAIL;
    }
    else
    {
    }
 
    return OSA_SOK;
}

int DRV_imgsSetDgain_IMX122(int dgain)
{
    return OSA_SOK;
}

int DRV_imgsSetAgain_IMX122(int again, int setRegDirect)
{
    /*
       0 dB to 24 dB nalog Gain 24 dB (step pitch 0.3 dB)
       24.3 dB to 42 dB nalog Gain 24 dB + Digital Gain 0.3 to 18 dB (step pitch 0.3 dB)
       */
    float gain_times;
    double gain_db;
    unsigned int gain_reg;
    int i =0;

    if (again <= 1000) again = 1000;
    //if (again >= 125000) again = 125000;
    if (again >= 60000) again = 60000;

    gain_times = again / 1000.0;
    gain_db = 20.0 * log10(gain_times);
    gain_reg = (unsigned char) (gain_db / 0.3 + 0.5);    //for 0.3db/step

    //gain_reg = gain_reg > 48 ? 48 : gain_reg;
    //gain_reg = 80;

    imx122_write(&gDRV_imgsObj.spiHndl, 0x021E, gain_reg & 0xff);

#if 0
    OSA_printf("gain value:%d,again:%d,gain_db:%f,gain_times:%f\n",gain_reg,again,gain_db,gain_times);
#endif

    return OSA_SOK;
}

//set the shutter start line,setRegDirect = 0
int DRV_imgsSetEshutter_IMX122(Uint32 eshutterInUsec, int setRegDirect)
{
    Uint32  SetValue,SUB_Control=0,tempdata;
    Uint8   Send_HH,Send_H,Send_L;
    DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;

    if (setRegDirect)
    {
        // eshutterInUsec in line
        SetValue = eshutterInUsec;
    }
    else
    {
        // eshutterInUsec in Usec.
        DRV_imgsCalcSW_IMX122(eshutterInUsec);
        SetValue = pFrame->SW;
    }

#if 0
    OSA_printf("IMX122 ShutterInUsec:%d ; SetRegDirect:%d ; SetValue:%d \n",eshutterInUsec,setRegDirect,SetValue);
#endif

    if (SetValue < IMGS_SENSOR_LINEWIDTH)
    {
        SUB_Control = IMGS_SENSOR_LINEWIDTH - SetValue;
    }
    else
    {
        SUB_Control = 0;
    }
    Send_H = (Uint8)((SUB_Control >> 8) & 0x00FF);
    Send_L = (Uint8)(SUB_Control & 0x00FF);

    imx122_write(&gDRV_imgsObj.spiHndl, 0x0209,Send_H);
    imx122_write(&gDRV_imgsObj.spiHndl, 0x0208,Send_L);

    /* long exposure ,need re-set VMAX*/
    if (SetValue < IMGS_SENSOR_LINEWIDTH)
    {
        tempdata = IMGS_SENSOR_LINEWIDTH;
    }
    else
    {
        tempdata = SetValue;
    }
    Send_H = (Uint8)((tempdata >> 8) & 0x00FF);
    Send_L = (Uint8)(tempdata & 0x00FF);

    imx122_write(&gDRV_imgsObj.spiHndl, 0x0206, Send_H);
    imx122_write(&gDRV_imgsObj.spiHndl, 0x0205, Send_L);

    return OSA_SOK;
}

int DRV_imgsSetRegs_IMX122(void)
{
    InitRegTable();
    InitRegTable(); //note: Part of the sensor need to be initialized twice
    return OSA_SOK;
}

DRV_ImgsFuncs IMX122ImgsFuncs =
{
    .imgsOpen            = DRV_imgsOpen_IMX122,
    .imgsClose           = DRV_imgsClose_IMX122,
    .imgsSetMirror       = NULL,
    .imgsGetImagerName   = DRV_imgsGetImagerName_IMX122,
    .imgsSetAgain        = DRV_imgsSetAgain_IMX122,
    .imgsSetDgain        = DRV_imgsSetDgain_IMX122,
    .imgsSetEshutter     = DRV_imgsSetEshutter_IMX122,
    .imgsSetDcSub        = DRV_imgsSetDcSub_IMX122,
    .imgsBinEnable       = DRV_imgsBinEnable_IMX122,
    .imgsBinMode         = DRV_imgsBinMode_IMX122,
    .imgsSetFramerate    = DRV_imgsSetFramerate_IMX122,
    .imgsSet50_60Hz      = DRV_imgsSet50_60Hz_IMX122,
    .imgsEnable          = DRV_imgsEnable_IMX122,
    .imgsGetModeConfig   = DRV_imgsGetModeConfig_IMX122,
    .imgsGetIsifConfig   = DRV_imgsGetIsifConfig_IMX122,
    .imgsGetH3aConfig    = DRV_imgsGetH3aConfig_IMX122,
    .imgsGetIpipeConfig  = DRV_imgsGetIpipeConfig_IMX122,
    .imgsGetLdcConfig    = DRV_imgsGetLdcConfig_IMX122
};

