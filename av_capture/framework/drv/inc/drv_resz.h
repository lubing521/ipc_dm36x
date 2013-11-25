
#ifndef _DRV_RESZ_H_
#define _DRV_RESZ_H_

#include <drv.h>
#include <drv_csl.h>

typedef struct {

  Uint16 outType;
  
  Bool32 flipH;
  Bool32 flipV;

  Uint8  *outVirtAddr;
  Uint8  *outPhysAddr;
  Uint16 outWidth;
  Uint16 outHeight;
  Uint16 outOffsetH;
  Uint16 outOffsetV;
  
} DRV_ReszOutPrm;

typedef struct {

  Uint16 inType;
  
  Uint8  *inVirtAddr;
  Uint8  *inPhysAddr;
  Uint16 inStartX;
  Uint16 inStartY;
  Uint16 inWidth;
  Uint16 inHeight;
  Uint16 inOffsetH;         // in pixels
  Uint16 inOffsetV;         // in lines 

  DRV_ReszOutPrm *pOut[CSL_RSZ_CH_MAX];

  Bool   enableInvAlaw;
  Bool   enableInvDpcm;

  Uint16 clkDivM;
  Uint16 clkDivN;
 
} DRV_ReszRunPrm;

int DRV_reszInit();
int DRV_reszExit();

int DRV_reszRun(DRV_ReszRunPrm *prm);


#endif
