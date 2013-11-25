
#ifndef _DRV_LDC_H_
#define _DRV_LDC_H_

#include <drv_imgs.h>

#define DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID    (0)

#define DRV_LDC_STREAM_ID_MAX   (4) 

typedef struct {

  int sensorMode;

  Uint16 inWidth;
  Uint16 inHeight;
  
  DRV_ImgsLdcConfig *pLdcConfig;

} DRV_LdcConfig;

typedef struct {

  Uint16 inFormat;

  Uint8 *inPhysAddr;
  Uint16 inOffsetH;
  Uint16 inOffsetV;

  Uint16 inStartX; 
  Uint16 inStartY;
  Uint16 inWidth;
  Uint16 inHeight;

  Uint8 *outPhysAddr;
  Uint16 outOffsetH;
  Uint16 outOffsetV;
  
  Uint16 OBH;         // set to 0 for auto calc 
  Uint16 OBV;         // set to 0 for auto calc
  Uint16 PIXEL_PAD;   // set to 0 for auto calc

} DRV_LdcRunPrm;

int DRV_ldcOpen(int streamId, DRV_LdcConfig *config);
int DRV_ldcClose(int streamId);

int DRV_ldcRun(int streamId, DRV_LdcRunPrm *prm);

int DRV_ldcInit();
int DRV_ldcExit();

#endif
