
#ifndef _ALG_AEWB_H_
#define _ALG_AEWB_H_

#include <alg.h>
#include <drv_h3a.h>

#define ALG_AEWB_ID_NONE    0
#define ALG_AEWB_ID_APPRO   1
#define ALG_AEWB_ID_TI    	2

#define ALG_AEWB_OFF    	0
#define ALG_AEWB_AE   		1
#define ALG_AEWB_AWB    	2
#define ALG_AEWB_AEWB    	3

#define ALG_VIDEO_MODE_NTSC   0
#define ALG_VIDEO_MODE_PAL    1
#define ALG_VIDEO_MODE_NOFLICKER    2

#define ALG_AWB_MODE_INDOOR   0
#define ALG_AWB_MODE_OUTDOOR  1

#define ALG_AE_MODE_DAY      0
#define ALG_AE_MODE_NIGHT    1

#define IMAGE_TUNE_AWB_RGB_SIZE    1024
#define IMAGE_TUNE_AWB_YUV_SIZE    1024   /* If modified, change the same in imageTunePreview.h */

typedef struct {

  Uint16 algId;

  int sensorMode;
  DRV_H3aInfo *pH3aInfo;

  Uint16 vnfDemoCfg;
  Uint16 aewbType;
  Uint16 aewbVendor;
  Uint16 aewbPriority;
  Uint16 aewbBinEnable;
  Uint16 reduceShutter;
  Uint16 flickerType;
  Uint16 shiftValue;
  Uint16 saldre;

} ALG_AewbCreate;

typedef struct {

  Uint16 awbMode;
  Uint16 aeMode;
  Uint16 videoMode;
  Uint32 binMode;

  Uint16 sharpness;
  Uint16 contrast;
  Uint16 brightness;
  Uint16 backLight;
  Uint16 saturation;

  Uint8 *h3aDataVirtAddr;

  Uint16 vnfDemoCfg;
  Uint16 aewbType;
  Uint16 aewbVendor;
  Uint16 aewbPriority;
  Uint16 reduceShutter;
  Uint16 saldre;
  Uint16 sensorMode;

} ALG_AewbRunPrm;

typedef struct {

  Uint32 exposureTimeInUsec;
  Uint32 apertureLevel;
  Uint32 sensorGain;
  Uint32 ipipeGain;

  Uint32 gainR;
  Uint32 gainGr;
  Uint32 gainGb;
  Uint32 gainB;

  Uint32 colorTemp;

} ALG_AewbStatus;

typedef struct {

  Uint8 *h3aDataVirtAddr;
  DRV_H3aInfo *pH3aInfo;

} ALG_AfRunPrm;

/* ANR - IT */
typedef struct {

  Uint32 h3aRegs[32];            ///< H3A Register Dump
  Uint32 awbNumWinH;          ///< Number of AWB windows in H direction
  Uint32 awbNumWinV;          ///< Number of AWB windows in V direction
  Uint32 awbMiscData[16];     ///< AWB algorithm specific data
  Uint32 awbRgbData[3*IMAGE_TUNE_AWB_RGB_SIZE];          //Revisit - Options to use pointer here
  Uint32 awbYuvData[3*IMAGE_TUNE_AWB_YUV_SIZE];          //Revisit - Options to use pointer here

}ALG_AewbData_ITTAwb;

void *ALG_aewbCreate(ALG_AewbCreate *create);
int ALG_aewbRun(void *hndl, ALG_AewbRunPrm *prm, ALG_AewbStatus *status);
int ALG_aewbDelete(void *hndl);

int ALG_afRun(ALG_AfRunPrm * prm, int *focus_value);

#endif
