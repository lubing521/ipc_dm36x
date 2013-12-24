#include <drv_imgs.h>
#include <drv_i2c.h>
#include <drv_spi.h>
#include <drv_gpio.h>

DRV_I2cHndl i2cHndl;
DRV_SPIHndl spiHndl;

#define IMGS_RESET_GPIO         (57)

#define AR0130_I2C_ADDR         (0x20)
#define AR0330_I2C_ADDR         (0x20)
#define AR0331_I2C_ADDR         (0x20)
#define MT9M034_I2C_ADDR        (0x20)
#define MT9P031_I2C_ADDR1       (0xBA)
#define MT9P031_I2C_ADDR2       (0x90)
#define AP0101_I2C_ADDR         (0x90)
#define OV9712_I2C_ADDR         (0x60)

#define AR0130_CHIP_VERSION     (0x3000)
#define AR0330_CHIP_VERSION     (0x3000)
#define AR0331_CHIP_VERSION     (0x3000)
#define MT9M034_CHIP_VERSION    (0x3000)
#define MT9P031_CHIP_VERSION    (0x00)
#define AP0101_CHIP_VERSION     (0x00)

#define AR0130_IMGS_CHIP_ID     (0x2402)
#define AR0330_IMGS_CHIP_ID     (0x2604)
#define AR0331_IMGS_CHIP_ID     (0x2602)
#define MT9M034_IMGS_CHIP_ID    (0x2400)
#define MT9P006_IMGS_CHIP_ID    (0x1801)
#define AP0101_IMGS_CHIP_ID     (0x0160)
#define OV9712_IMGS_CHIP_ID     (0x9711)

DRV_ImgsFuncs *drvImgsFunc = NULL;
eSensorStatus sensorStatus = SENSOR_ERROR;
eSensorType sensorId = SENSOR_UNKNOWN;

Uint8 gImgsI2CAddr = MT9P031_I2C_ADDR1;
Uint16 gImgsVPad = 4;

int AdjustApertureLevel(Uint8 level)
{
#define IRIS_I2C_ADDR     (0x0898U)
    DRV_I2cHndl i2cHnd2;
    int status = OSA_EFAIL;
    Uint16 regAddr16=0;
    Uint16 regValue = 0;

    level = (level >= 100) ? 100 : level;

    Uint8 da_value = 255 * (100 - level) / 100;
    static Uint8 da_value_bak = 50;

    if (da_value == da_value_bak)
    {
        return OSA_SOK;
    }
    da_value_bak = da_value;

    memset(&i2cHnd2, 0, sizeof(i2cHnd2));
    status = DRV_i2cOpen(&i2cHnd2, IRIS_I2C_ADDR);

    if (status == OSA_SOK)
    {
        regValue = (Uint16)da_value << 4;//Please refer to the manual DAC5571
        status = DRV_i2c16Write16(&i2cHnd2, &regAddr16, &regValue, 1);

        if (status != OSA_SOK)
        {
            OSA_ERROR("AdjustApertureLevel error!\n");
        }
        else
        {
            OSA_printf("AdjustApertureLevel level= %d\n", level);
        }
    }

    DRV_i2cClose(&i2cHnd2);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2cClose() error!\n");
        return OSA_EFAIL;
    }

    OSA_waitMsecs(10);

    return OSA_SOK;
}

void TestIris()
{
    static int i = 0;

    i++;

    if (i % 100 == 0)
    {
        AdjustApertureLevel(0);
    }
    else if (i % 100 == 50)
    {
        AdjustApertureLevel(100);
    }
}

eSensorStatus GetSensorStatus()
{
    return sensorStatus;
}

void SetSensorStatus(eSensorStatus status)
{
    sensorStatus = status;
}

eSensorType GetSensorId()
{
    return sensorId;
}

int SensorIsYuvMode()
{
    return (sensorId == AP0101) ? 1 : 0;
}

int SensorIsYuvModeProgressive()
{
    return (sensorId == AP0101) ? 1 : 0;
}

int CheckSensor()
{
#define RETRY_NUM 6
    int status = OSA_EFAIL, retry = 1;
    Uint8  regAddr8   = 0;
    Uint8  regValue8  = 0;
    Uint16 regAddr16  = 0;
    Uint16 regValue16 = 0;
    Uint16 chipId     = 0;

    memset(&i2cHndl, 0, sizeof(i2cHndl));
    memset(&spiHndl, 0, sizeof(spiHndl));

    DRV_gpioSetMode(IMGS_RESET_GPIO, DRV_GPIO_DIR_OUT);
    DRV_gpioClr(IMGS_RESET_GPIO);
    OSA_waitMsecs(100);
    DRV_gpioSet(IMGS_RESET_GPIO);
    OSA_waitMsecs(100);

    // detect MT9P031_I2C_ADDR1 (default)
    retry = 1;
    status = DRV_i2cOpen(&i2cHndl, MT9P031_I2C_ADDR1);
    if (status == OSA_SOK)
    {
        do
        {
            regAddr8 = MT9P031_CHIP_VERSION;
            regValue16 = 0;
            status = DRV_i2cRead16(&i2cHndl, &regAddr8, &regValue16, 1);
            if (regValue16 == MT9P006_IMGS_CHIP_ID)
            {
                OSA_printf("\n[%d]----------------Sensor is MT9P006\n\n", retry);
                gImgsI2CAddr = MT9P031_I2C_ADDR1;
                drvImgsFunc = &MT9P031ImgsFuncs;
                sensorId = MT9P031;
                goto EXIT;
            }
            OSA_waitMsecs(10);
        }
        while (++retry <= RETRY_NUM);
    }
    DRV_i2cClose(&i2cHndl);

    // detect MT9P031_I2C_ADDR2
    retry = 1;
    status = DRV_i2cOpen(&i2cHndl, MT9P031_I2C_ADDR2);
    if (status == OSA_SOK)
    {
        do
        {
            regAddr8 = MT9P031_CHIP_VERSION;
            regValue16 = 0;
            status = DRV_i2cRead16(&i2cHndl, &regAddr8, &regValue16, 1);
            if (regValue16 == MT9P006_IMGS_CHIP_ID)
            {
                OSA_printf("\n[%d]----------------Sensor is MT9P006\n\n", retry);
                gImgsI2CAddr = MT9P031_I2C_ADDR2;
                drvImgsFunc = &MT9P031ImgsFuncs;
                sensorId = MT9P031;
                goto EXIT;
            }
            else if (regValue16 == AP0101_IMGS_CHIP_ID)
            {
                OSA_printf("\n[%d]----------------Sensor is AP0101\n\n", retry);
                drvImgsFunc = &AP0101ImgsFuncs;
                sensorId = AP0101;
                goto EXIT;
            }
            OSA_waitMsecs(10);
        }
        while (++retry <= RETRY_NUM);
    }
    DRV_i2cClose(&i2cHndl);

    // detect AR0130 AR0330 AR0331 MT9M034
    retry = 1;
    status = DRV_i2cOpen(&i2cHndl, AR0130_I2C_ADDR);// AR0130 and MT9M034 have same I2C addr and same CHIP VERSION addr
    if (status == OSA_SOK)
    {
        do
        {
            regAddr16 = AR0130_CHIP_VERSION;
            regValue16 = 0;
            status = DRV_i2c16Read16(&i2cHndl, &regAddr16, &regValue16, 1);
            if (regValue16 == AR0130_IMGS_CHIP_ID)
            {
                OSA_printf("\n[%d]----------------Sensor is AR0130\n\n", retry);
                drvImgsFunc = &AR0130ImgsFuncs;
                sensorId = AR0130;
                goto EXIT;
            }
            else if (regValue16 == AR0330_IMGS_CHIP_ID)
            {
                OSA_printf("\n[%d]----------------Sensor is AR0330\n\n", retry);
                drvImgsFunc = &AR0330ImgsFuncs;
                sensorId = AR0330;
                goto EXIT;
            }
            else if (regValue16 == AR0331_IMGS_CHIP_ID)
            {
                OSA_printf("\n[%d]----------------Sensor is AR0331\n\n", retry);
                drvImgsFunc = &AR0331ImgsFuncs;
                sensorId = AR0331;
                goto EXIT;
            }
            else if (regValue16 == MT9M034_IMGS_CHIP_ID)
            {
                OSA_printf("\n[%d]----------------Sensor is MT9M034\n\n", retry);
                drvImgsFunc = &MT9M034ImgsFuncs;
                sensorId = MT9M034;
                goto EXIT;
            }
            OSA_waitMsecs(10);
        }
        while (++retry <= RETRY_NUM);
    }
    DRV_i2cClose(&i2cHndl);

    retry = 1;
    status = DRV_i2cOpen(&i2cHndl, OV9712_I2C_ADDR);// OV9712 addr
    if (status == OSA_SOK)
    {
        do
        {
            regAddr8 = 0x0a;
            regValue8 = 0;
            status = DRV_i2cRead8(&i2cHndl, &regAddr8, &regValue8, 1);
            chipId = regValue8;

            regAddr8 = 0x0b;
            regValue8 = 0;
            status = DRV_i2cRead8(&i2cHndl, &regAddr8, &regValue8, 1);
            chipId = (chipId << 8) | regValue8;

            if (chipId == OV9712_IMGS_CHIP_ID)
            {
                OSA_printf("\n[%d]----------------Sensor is OV9712\n\n", retry);
                drvImgsFunc = &OV9712ImgsFuncs;
                sensorId = OV9712;
                goto EXIT;
            }
            OSA_waitMsecs(10);
        }
        while (++retry <= RETRY_NUM);
    }
    DRV_i2cClose(&i2cHndl);
    
    if (DRV_imgsCheckId_IMX122(&spiHndl) == OSA_SOK)
    {
        OSA_printf("\n----------------Detect Sensor is IMX122\n\n");
        drvImgsFunc = &IMX122ImgsFuncs;
        sensorId = IMX122;
    }
        
EXIT:
    DRV_i2cClose(&i2cHndl);

    if (sensorId == SENSOR_UNKNOWN)
    {
        OSA_printf("\n----------------Default video input is YUV data\n\n");
        drvImgsFunc = &AP0101ImgsFuncs;
        sensorId = AP0101;
    }

    return OSA_SOK;
}

int DRV_imgsTestMain(int argc, char **argv)
{
    DRV_ImgsConfig imgsConfig;

    int status = DRV_init();
    if (status != OSA_SOK)
    {
        return status;
    }

    imgsConfig.sensorMode = DRV_IMGS_SENSOR_MODE_640x480;
    imgsConfig.fps        = 30;
    imgsConfig.binEnable  = TRUE;

    OSA_printf(" IMGS: Imager %s is selected\n", drvImgsFunc->imgsGetImagerName());

    status = drvImgsFunc->imgsOpen(&imgsConfig);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_imgsOpen(%d)\n", imgsConfig.sensorMode);
    }

    if (status == OSA_SOK)
    {
        drvImgsFunc->imgsEnable(TRUE);
        OSA_DEBUG_WAIT;
        drvImgsFunc->imgsEnable(FALSE);
        drvImgsFunc->imgsClose();
    }

    DRV_exit();

    return status;
}

