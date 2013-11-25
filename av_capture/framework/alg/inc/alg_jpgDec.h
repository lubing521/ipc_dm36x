
#ifndef _ALG_JPG_DEC_H_
#define _ALG_JPG_DEC_H_

#include <alg.h>

typedef struct {

  Uint16 dataFormat;
  Uint16 maxWidth;
  Uint16 maxHeight;

} ALG_JpgDecCreate;

typedef struct {

  Uint8 *inAddr;
  Uint32 inDataSize;
  
  Uint8 *outAddr;
  Uint16 outStartX;
  Uint16 outStartY;
  Uint16 outOffsetH;
  Uint16 outOffsetV;

} ALG_JpgDecRunPrm;

typedef struct {

  Uint32 bytesUsed;
  Uint16 frameWidth;
  Uint16 frameHeight;

} ALG_JpgDecRunStatus;

void *ALG_jpgDecCreate(ALG_JpgDecCreate *create);
int ALG_jpgDecRun(void *hndl, ALG_JpgDecRunPrm *prm, ALG_JpgDecRunStatus *status);
int ALG_jpgDecDelete(void *hndl);


#endif


