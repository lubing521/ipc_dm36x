
#ifndef _ALG_JPG_ENC_H_
#define _ALG_JPG_ENC_H_

#include <alg.h>

#define ALG_JPG_ENC_Q_VALUE_MIN   (5)  // lowest quality
#define ALG_JPG_ENC_Q_VALUE_MAX   (95) // highest quality

typedef struct {

  Uint16 dataFormat;
  Uint16 width;
  Uint16 height;
  Uint16 offsetH;
  Uint16 offsetV;
  int    qValue;
  Uint16 encDataFormat;

} ALG_JpgEncCreate;

typedef struct {

  Uint16 inStartX;
  Uint16 inStartY;

  Uint8 *inAddr;
  Uint8 *outAddr;

  Uint32 outDataMaxSize;

  Uint32 snapEnable;
  char snapFilename[48];
} ALG_JpgEncRunPrm;

typedef struct {

  Uint32 bytesGenerated;

} ALG_JpgEncRunStatus;

void *ALG_jpgEncCreate(ALG_JpgEncCreate *create);
int ALG_jpgEncSetQvalue(void *hndl, int qValue);
//changed by zeng
int ALG_jpgEncRun(void *hndl, ALG_JpgEncRunPrm *prm, ALG_JpgEncRunStatus *status, CallBackSetOSDPixel func);
int ALG_jpgEncDelete(void *hndl);


#endif


