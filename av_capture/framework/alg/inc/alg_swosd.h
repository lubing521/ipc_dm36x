
#ifndef _ALG_SWOSD_H_
#define _ALG_SWOSD_H_

#include <alg.h>
#include <drv_faceDetect.h>
#include <drv_frameCopy.h>

#define ALG_SWOSD_DATA_TYPE_ICON     0
#define ALG_SWOSD_DATA_TYPE_STRING   1

#define FR_OSD_DRAW  //ANR
#define DRAW_HIST  1 // ANR - Hist Set 1 to draw histogram on SWOSD

typedef struct {

  Uint16 maxOverlays;

} ALG_SwosdCreate;


typedef struct {

  Uint16 videoWidth;
  Uint16 videoHeight;
  Uint16 videoOffsetH;
  Uint16 videoOffsetV;
  Uint16 videoDataFormat;

  Uint8  *videoInOutAddr;

  Uint16 faceInfoInWidth;
  Uint16 faceInfoInHeight;
  Uint16 faceInfoROIx;
  Uint16 faceInfoROIy;
  Uint16 faceInfoROIw;
  Uint16 faceInfoROIh;
  DRV_FaceDetectRunStatus faceInfo;

  Bool privacyMask;
  Bool histEnable;
  Bool fxdROIflag;

  Uint8   histStatus;      /*ANR - Hist */
  Uint32  histData[256];   /*ANR - Hist */

} ALG_SwosdRunPrm;

typedef struct {

  Uint32 enable;
  Uint8  *dataAddr;
  Uint16 dataFormat;
  Uint16 dataType;
  Uint16 startX;
  Uint16 startY;
  Uint16 width;
  Uint16 height;
  Uint16 fontId;  // valid only for ALG_SWOSD_DATA_TYPE_STRING
  Uint16 offsetH;
  Uint16 offsetV;
  Uint16 transperencyVal;   // YC value

} ALG_SwosdOverlayInfo;

typedef struct {

  Uint16 startX;
  Uint16 startY;
  Uint16 width;
  Uint16 height;
  Uint32 colorY;
  Uint32 colorC;
  Uint16 thickness;
  Bool  privacyMask;

} ALG_SwosdRectInfo;

typedef struct {

  Uint16 startX;
  Uint16 startY;
  Uint16 length;
  Bool   vertLine;
  Uint16 colorY;
  Uint16 colorC;
  Uint16 thickness;

} ALG_SwosdLineInfo;

void *ALG_swosdCreate(ALG_SwosdCreate *create);

int ALG_swosdOverlaySet(void *hndl, Uint16 overlayId, ALG_SwosdOverlayInfo *overlay);
int ALG_swosdOverlayGet(void *hndl, Uint16 overlayId, ALG_SwosdOverlayInfo *overlay);
int ALG_swosdOverlayEnable(void *hndl, Uint16 overlayId, Bool enable);

int ALG_swosdRun(void *hndl, ALG_SwosdRunPrm *prm);

int ALG_swosdDelete(void *hndl);

int ALG_swosdDrawLine(ALG_SwosdRunPrm *bufInfo, ALG_SwosdLineInfo *lineInfo);
int ALG_swosdDrawRect(ALG_SwosdRunPrm *bufInfo, ALG_SwosdRectInfo *rectInfo);
int ALG_swosdDrawHist( ALG_SwosdRunPrm *prm);  //ANR -Hist

#endif





