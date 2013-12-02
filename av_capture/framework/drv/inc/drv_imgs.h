
#ifndef _DRV_IMGS_H_
#define _DRV_IMGS_H_

#include <drv.h>
#include <drv_csl.h>
#include <imageTuneParams.h>

typedef enum{
	DRV_IMGS_SENSOR_MODE_640x480 = 0,
	DRV_IMGS_SENSOR_MODE_720x480,
	DRV_IMGS_SENSOR_MODE_800x600,
	DRV_IMGS_SENSOR_MODE_1024x768,
	DRV_IMGS_SENSOR_MODE_1280x720,
	DRV_IMGS_SENSOR_MODE_1280x960,
	DRV_IMGS_SENSOR_MODE_1280x1024,
	DRV_IMGS_SENSOR_MODE_1600x1200,
    DRV_IMGS_SENSOR_MODE_1620x1080,
	DRV_IMGS_SENSOR_MODE_1920x1080,
	DRV_IMGS_SENSOR_MODE_2048x1536,
	DRV_IMGS_SENSOR_MODE_2592x1920
} DRV_IMGS_SENSOR_MODE;

#define DRV_IMGS_VNF_PAD_VALUE  		(32)  	  // pads 16 extra pixels on all four sides, this is needed Katana NF
#define DRV_IMGS_SENSOR_MODE_PIXEL_PAD  (0x0200)  // pads 16 extra pixels on all four sides, this is needed Katana NF
#define DRV_IMGS_SENSOR_MODE_VSTAB      (0x0100)  // pads 10% extra pixels on all fours side, this is needed for video stabilization

#define DRV_IMGS_SENSOR_MODE_DEFAULT    (DRV_IMGS_SENSOR_MODE_1280x720)

typedef struct {

  int  sensorMode;
  int  fps;
  Bool binEnable;
  Bool flipH;
  Bool flipV;
  Bool bIsDdrIn;	
  Bool bIsALawEnable;

} DRV_ImgsConfig;

typedef struct {

  IMAGE_TUNE_CcdcParams ccdcParams;
  CSL_CcdcSyncConfig syncConfig;

} DRV_ImgsIsifConfig;

typedef struct {

  IMAGE_TUNE_IpipeifParams ipipeifParams;
  IMAGE_TUNE_IpipeParams   ipipeParams;

} DRV_ImgsIpipeConfig;

typedef struct {

  IMAGE_TUNE_LdcParams ldcParams;

} DRV_ImgsLdcConfig;

typedef struct {

  Uint16  medFiltThreshold;

  Bool    aewbMedFiltEnable;
  Uint16  aewbSatLimit;
  Uint16  aewbWinStartX;
  Uint16  aewbWinStartY;
  Uint16  aewbWinNumH;
  Uint16  aewbWinNumV;
  Uint8   aewbOutFormat;
  Uint8   aewbShift;

  Bool    afVfEnable;
  Bool    afMedFiltEnable;
  Uint8   afRgbPos;
  Uint16  afFvAccMode;
  Uint16  afPaxStartX;
  Uint16  afPaxStartY;
  Uint16  afPaxNumH;
  Uint16  afPaxNumV;
  Int32   afIirCoeff0[11];
  Int32   afIirCoeff1[11];
  Int32   afVfvFir1Coeff[5];
  Int32   afVfvFir2Coeff[5];
  Uint16  afVfvFir1Threshold;
  Uint16  afHfvFir1Threshold;
  Uint16  afVfvFir2Threshold;
  Uint16  afHfvFir2Threshold;

} DRV_ImgsH3aConfig;

typedef struct {

  int  sensorDataWidth;
  int  sensorDataHeight;
  int  validStartX;
  int  validStartY;
  int  validWidth;
  int  validHeight;
  Bool binEnable;
  int	 vdint0;
  int	 vdint1;
  int	 vdint2;

} DRV_ImgsModeConfig;


int DRV_imgsOpen(DRV_ImgsConfig *config);
int DRV_imgsClose();

char* DRV_imgsGetImagerName();

int DRV_imgsSpecificSetting(void);
int DRV_imgsSetAgain(int again, int setRegDirect);
int DRV_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect);
int DRV_imgsSetDcSub(Uint32 dcSub, int setRegDirect);
int DRV_imgsBinEnable(Bool enable);
int DRV_imgsBinMode(int binMode);
int DRV_imgsSetFramerate(int fps);
int DRV_imgsSet50_60Hz(Bool is50Hz);

int DRV_imgsEnable(Bool enable);

DRV_ImgsModeConfig      *DRV_imgsGetModeConfig(int sensorMode);
DRV_ImgsIsifConfig      *DRV_imgsGetIsifConfig(int sensorMode);
DRV_ImgsH3aConfig       *DRV_imgsGetH3aConfig(int sensorMode, int aewbVendor);
DRV_ImgsIpipeConfig     *DRV_imgsGetIpipeConfig(int sensorMode, int vnfDemoCfg );
DRV_ImgsLdcConfig       *DRV_imgsGetLdcConfig(int sensorMode, Uint16 ldcInFrameWidth, Uint16 ldcInFrameHeight);

#ifdef AR0331_WDR
int DRV_ImgsGetMean(unsigned int *highMean, unsigned int *lowMean);
int DRV_ImgsGetWDRSetting(unsigned int *wdrMode, unsigned int *ratioT1T2);
int DRV_ImgsSetWDRSetting(unsigned int wdrMode, unsigned int ratioT1T2);
#endif
#endif

