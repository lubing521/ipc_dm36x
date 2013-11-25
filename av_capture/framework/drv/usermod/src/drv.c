
#include <drv.h>
#include <drv_csl.h>
#include <drv_dma.h>
#include <osa_cmem.h>
#include <drv_resz.h>
#include <drv_frameCopy.h>
#include <drv_imageTune.h>
#include <drv_ldc.h>
#include <drv_imgs.h>

#include <drv_audio.h>

int DRV_vpssInit();
int DRV_vpssExit();

int DRV_init()
{
  int status;
  static int isInitDone=0;

  if(isInitDone)
    return OSA_SOK;

  isInitDone=1;

  OSA_init();

  status = CSL_sysInit();
  if(status!=OSA_SOK) {
    OSA_ERROR("CSL init\n");
    goto error_exit;
  }

  status = DRV_vpssInit();
  if(status!=OSA_SOK) {
    OSA_ERROR("VPSS init\n");
    goto csl_error_exit;
  }

  status = CMEM_init();
  if(status!=OSA_SOK) {
    OSA_ERROR("CMEM init\n");
    goto csl_error_exit;
  }

  status = DRV_dmaInit();
  if(status!=OSA_SOK) {
    OSA_ERROR("DMA init\n");
    goto cmem_error_exit;
  }

  status = DRV_reszInit();
  if(status!=OSA_SOK) {
    OSA_ERROR("RESZ init\n");
    goto dma_error_exit;
  }

  status = DRV_frameCopyInit();
  if(status!=OSA_SOK) {
    OSA_ERROR("FRAME COPY init\n");
    goto resz_error_exit;
  }

  #ifndef DRV_EXCLUDE_IMAGE_TUNE
  status = DRV_imageTuneInit();
  if(status!=OSA_SOK) {
    OSA_ERROR("IMAGE TUNE init\n");
    goto framecopy_error_exit;
  }

  status = DRV_ldcInit();
  if(status!=OSA_SOK) {
    OSA_ERROR("LDC init\n");
    goto imagetune_error_exit;
  }
  #endif


  return OSA_SOK;

imagetune_error_exit:
  DRV_imageTuneExit();

framecopy_error_exit:
  DRV_frameCopyExit();

resz_error_exit:
  DRV_reszExit();

dma_error_exit:
  DRV_dmaExit();

cmem_error_exit:
  CMEM_exit();

csl_error_exit:
  CSL_sysExit();

error_exit:

  OSA_ERROR("DRV_init()\n");

  return OSA_EFAIL;
}

int DRV_exit()
{
  int status = OSA_SOK;

  static int isExitDone=0;

  if(isExitDone)
    return OSA_SOK;

  isExitDone=1;

  #ifndef DRV_EXCLUDE_IMAGE_TUNE
  status |= DRV_ldcExit();
  status |= DRV_imageTuneExit();
  #endif

  status |= DRV_frameCopyExit();
  status |= DRV_reszExit();
  status |= DRV_dmaExit();
  status |= DRV_vpssExit();
  status |= CMEM_exit();
  status |= CSL_sysExit();

  OSA_exit();

  return status;
}

int DRV_drvTestMain(int argc, char **argv)
{
  int status;

  status = DRV_init();
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_init()\n");
    return status;
  }

  DRV_exit();

  return OSA_SOK;
}

void DRV_imgGetWidthHeight(int sensorMode, Uint16 *width, Uint16 *height)
{
  int mode = sensorMode & 0xFF;

  switch(mode) {

    default:
    case DRV_IMGS_SENSOR_MODE_640x480:
      *width  = 640;
      *height = 480;
      break;

    case DRV_IMGS_SENSOR_MODE_720x480  :
      *width  = 720;
      *height = 480;
      break;

    case DRV_IMGS_SENSOR_MODE_800x600  :
      *width  = 800;
      *height = 600;
      break;

    case DRV_IMGS_SENSOR_MODE_1024x768 :
      *width  = 1024;
      *height = 768;
      break;

    case DRV_IMGS_SENSOR_MODE_1280x720 :
      *width  = 1280;
      *height = 720;
      break;

    case DRV_IMGS_SENSOR_MODE_1280x960 :
      *width  = 1280;
      *height = 960;
      break;

    case DRV_IMGS_SENSOR_MODE_1280x1024:
      *width  = 1280;
      *height = 1024;
      break;

    case DRV_IMGS_SENSOR_MODE_1600x1200:
      *width  = 1600;
      *height = 1200;
      break;

    case DRV_IMGS_SENSOR_MODE_1920x1080:
      *width  = 1920;
      *height = 1080;
      break;

    case DRV_IMGS_SENSOR_MODE_2048x1536:
      *width  = 2048;
      *height = 1536;
      break;

    case DRV_IMGS_SENSOR_MODE_2592x1920:
      *width  = 2592;
      *height = 1920;
      break;
   }

  //DRV_imgWidthHeightPad(sensorMode, width, height);

  if(sensorMode & DRV_IMGS_SENSOR_MODE_VSTAB) {
	  /* Done here to keep the same Field of View when VNF is ON */
	  if(sensorMode & DRV_IMGS_SENSOR_MODE_PIXEL_PAD){
		*width  += 32;
		*height += 32;
	  }
    // add 20% extra for VS
    *width = ((Uint32)(*width)*120)/100;
    *height= ((Uint32)(*height)*120)/100;
  }

  // align width to 32, height 32 for LDC
  *width  = OSA_align(*width, 32);
  *height = OSA_align(*height,32);

}

void DRV_imgWidthHeightPad(int sensorMode, Uint16 *width, Uint16 *height)
{
  if(sensorMode & DRV_IMGS_SENSOR_MODE_PIXEL_PAD) {
    *width += 32;
    *height+= 32;
  }

  if(sensorMode & DRV_IMGS_SENSOR_MODE_VSTAB) {
    // add 20% extra for VS
    *width = ((Uint32)(*width)*120)/100;
    *height= ((Uint32)(*height)*120)/100;
  }

  // align width to 32, height 32 for LDC
  *width  = OSA_align(*width, 32);
  *height = OSA_align(*height,32);
}

