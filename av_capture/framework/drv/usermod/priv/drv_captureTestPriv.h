

#ifndef _DRV_CAPTURE_TEST_PRIV_H_
#define _DRV_CAPTURE_TEST_PRIV_H_

#include <drv_capture.h>
#include <drv_display.h>
#include <drv_frameCopy.h>
#include <drv_faceDetect.h>
#include <osa_file.h>
#include <osa_cmem.h>
#include <alg_vstab.h>

//#define DRV_CAPTURE_TEST_ENABLE_LSC

#define DRV_CAPTURE_TEST_PRINT_H3A_INFO
//#define DRV_CAPTURE_TEST_SAVE_H3A_DATA
#define DRV_CAPTURE_TEST_APPLY_2A

//#define DRV_CAPTURE_TEST_ENABLE_BOXCAR
//#define DRV_CAPTURE_TEST_PRINT_BOXCAR_INFO
//#define DRV_CAPTURE_TEST_SAVE_BOXCAR_DATA

//#define DRV_CAPTURE_TEST_ENABLE_BSC
//#define DRV_CAPTURE_TEST_PRINT_BSC_INFO
//#define DRV_CAPTURE_TEST_SAVE_BSC_DATA
//#define DRV_CAPTURE_TEST_ENABLE_VS

#define DRV_CAPTURE_TEST_ENABLE_RSZ_B_COPY
//#define DRV_CAPTURE_TEST_SAVE_RSZ_A_DATA

//#define DRV_CAPTURE_TEST_ENABLE_FD

#define APPRO_2A	(1)
#define TI_2A		(2)


#define DRV_CAPTURE_TEST_NUM_BUF       (3)
#define DRV_CAPTURE_TEST_SENSOR_MODE   (DRV_IMGS_SENSOR_MODE_1280x720)
#define DRV_CAPTURE_TEST_DATA_FORMAT   (DRV_DATA_FORMAT_YUV420)

typedef struct {

  DRV_CaptureConfig     captureConfig;
  DRV_DisplayWinConfig  displayConfig;

  DRV_CaptureInfo    captureInfo;
  DRV_DisplayWinHndl displayHndl;
  DRV_DisplayBufInfo displayInfo;
  int                displayBufId;
  int                testQuit;

  Uint32 tmpBufSize;
  Uint8  *tmpBufVirtAddr;
  Uint8  *tmpBufPhysAddr;
  Uint32 tmpBufCurSize;

  void   *vsHndl;
  ALG_VstabStatus vsStatus;
  OSA_PrfHndl vsPrf;

} DRV_CaptureTestCtrl;

int DRV_captureTestLscTableInit(Uint16 numLscBuf);

#endif /* _DRV_CAPTURE_TEST_PRIV_H_  */
