
#include <drv_imgs.h>

int DRV_imgsTestMain(int argc, char **argv)
{
  int status;
  DRV_ImgsConfig imgsConfig;

  status = DRV_init();
  if(status!=OSA_SOK)
    return status;

  imgsConfig.sensorMode = DRV_IMGS_SENSOR_MODE_640x480;
  imgsConfig.fps        = 30;
  imgsConfig.binEnable  = TRUE;      

  OSA_printf( " IMGS: Imager %s is selected\n", DRV_imgsGetImagerName());

  status = DRV_imgsOpen(&imgsConfig);

  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_imgsOpen(%d)\n", imgsConfig.sensorMode);
  }

  if(status==OSA_SOK) {
  
    DRV_imgsEnable(TRUE);
    
    OSA_DEBUG_WAIT;
    
    DRV_imgsEnable(FALSE);
  
    DRV_imgsClose();
  }

  DRV_exit();
  
  return status;
}
