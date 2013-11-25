
#include <drv_imgs.h>

int DRV_imgsOpen(DRV_ImgsConfig *config)
{
  return -1;
}

int DRV_imgsClose()
{
  return -1;
}

char* DRV_imgsGetImagerName()
{
  return "NO_IMAGER";
}

int DRV_imgsSet50_60Hz(Bool is50Hz)
{
  return -1;
}

int DRV_imgsSetFramerate(int fps)
{
  return -1;
}

int DRV_imgsBinEnable(Bool enable)
{
  return -1;
}

int DRV_imgsSetDgain(int dgain)
{
    return -1;
}

int DRV_imgsSetAgain(int again, int setRegDirect)
{
  return -1;
}

int DRV_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect)
{
  return -1;
}

int DRV_imgsSetDcSub(Uint32 dcSub, int setRegDirect)
{

  return 0;
}

int DRV_imgsEnable(Bool enable)
{
  return -1;
}

DRV_ImgsModeConfig      *DRV_imgsGetModeConfig(int sensorMode)
{
  return NULL;
}

DRV_ImgsIsifConfig      *DRV_imgsGetIsifConfig(int sensorMode)
{
  return NULL;
}

DRV_ImgsIpipeConfig     *DRV_imgsGetIpipeConfig(int sensorMode, int vnfDemoCfg)
{
  return NULL;
}

DRV_ImgsH3aConfig       *DRV_imgsGetH3aConfig(int sensorMode, int aewbVendor)
{
  return NULL;
}

DRV_ImgsLdcConfig       *DRV_imgsGetLdcConfig(int sensorMode, Uint16 ldcInFrameWidth, Uint16 ldcInFrameHeight)
{
  return NULL;
}


