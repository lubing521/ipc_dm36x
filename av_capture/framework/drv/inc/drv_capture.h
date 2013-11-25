
// image sensor capture driver wrapper APIs

#ifndef _DRV_CAPTURE_H_
#define _DRV_CAPTURE_H_

#include <drv_imgs.h>
#include <drv_isif.h>
#include <drv_ipipe.h>
#include <drv_h3a.h>

typedef struct {

  DRV_ImgsConfig  imgsConfig;
  DRV_IsifConfig  isifConfig;
  DRV_H3aConfig   h3aConfig;
  DRV_IpipeConfig ipipeConfig;
  
} DRV_CaptureConfig;

typedef struct {

  DRV_IpipeInfo   ipipeInfo;  
  DRV_IsifInfo    isifInfo;
  DRV_H3aInfo     h3aInfo;
  
} DRV_CaptureInfo;

int DRV_captureOpen(DRV_CaptureConfig *config, DRV_CaptureInfo *info);
int DRV_captureStart(Bool rawOutEnable, Bool yuvOutEnable);
int DRV_captureStop();
int DRV_captureClose();


#endif  /*  _DRV_CAPTURE_H_ */

