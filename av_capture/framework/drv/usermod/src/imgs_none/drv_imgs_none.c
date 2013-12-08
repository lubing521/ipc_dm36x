
#include <drv_imgs.h>

static int DRV_imgsOpen_none(DRV_ImgsConfig *config)
{
    return OSA_SOK;
}

static int DRV_imgsClose_none()
{
    return OSA_SOK;
}

static char* DRV_imgsGetImagerName_none()
{
    return "NO_IMAGER";
}

static int DRV_imgsSet50_60Hz_none(Bool is50Hz)
{
    return OSA_SOK;
}

static int DRV_imgsSetFramerate_none(int fps)
{
    return OSA_SOK;
}

static int DRV_imgsBinEnable_none(Bool enable)
{
    return OSA_SOK;
}

static int DRV_imgsSetDgain_none(int dgain)
{
    return OSA_SOK;
}

static int DRV_imgsSetAgain_none(int again, int setRegDirect)
{
    return OSA_SOK;
}

static int DRV_imgsSetEshutter_none(Uint32 eshutterInUsec, int setRegDirect)
{
    return OSA_SOK;
}

static int DRV_imgsSetDcSub_none(Uint32 dcSub, int setRegDirect)
{
    return OSA_SOK;
}

static int DRV_imgsEnable_none(Bool enable)
{
    return OSA_SOK;
}

static DRV_ImgsModeConfig *DRV_imgsGetModeConfig_none(int sensorMode)
{
    return NULL;
}

static DRV_ImgsIsifConfig *DRV_imgsGetIsifConfig_none(int sensorMode)
{
    return NULL;
}

static DRV_ImgsIpipeConfig *DRV_imgsGetIpipeConfig_none(int sensorMode, int vnfDemoCfg)
{
    return NULL;
}

static DRV_ImgsH3aConfig *DRV_imgsGetH3aConfig_none(int sensorMode, int aewbVendor)
{
    return NULL;
}

static DRV_ImgsLdcConfig *DRV_imgsGetLdcConfig_none(int sensorMode, Uint16 ldcInFrameWidth, Uint16 ldcInFrameHeight)
{
    return NULL;
}

int DRV_imgsSpecificSetting_none(void)
{
    return OSA_SOK;
}

int DRV_imgsBinMode_none(Bool enable)
{
    return OSA_SOK;
}

DRV_ImgsFuncs noneImgsFuncs = 
{
    .imgsOpen            = DRV_imgsOpen_none,
    .imgsClose           = DRV_imgsClose_none,
    .imgsSetMirror       = NULL,
    .imgsGetImagerName   = DRV_imgsGetImagerName_none,
    .imgsSetAgain        = DRV_imgsSetAgain_none,
    .imgsSetDgain        = DRV_imgsSetDgain_none,
    .imgsSetEshutter     = DRV_imgsSetEshutter_none,
    .imgsSetDcSub        = DRV_imgsSetDcSub_none,
    .imgsBinEnable       = DRV_imgsBinEnable_none,
    .imgsBinMode         = DRV_imgsBinMode_none,
    .imgsSetFramerate    = DRV_imgsSetFramerate_none,
    .imgsSet50_60Hz      = DRV_imgsSet50_60Hz_none, 
    .imgsEnable          = DRV_imgsEnable_none,
    .imgsGetModeConfig   = DRV_imgsGetModeConfig_none,
    .imgsGetIsifConfig   = DRV_imgsGetIsifConfig_none,
    .imgsGetH3aConfig    = DRV_imgsGetH3aConfig_none,
    .imgsGetIpipeConfig  = DRV_imgsGetIpipeConfig_none,
    .imgsGetLdcConfig    = DRV_imgsGetLdcConfig_none
};

