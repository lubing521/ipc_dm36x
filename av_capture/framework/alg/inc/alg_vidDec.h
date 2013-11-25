
#ifndef _ALG_VID_DEC_H_
#define _ALG_VID_DEC_H_

#include <alg.h>

typedef struct {

  Uint16 codec;
  Uint16 dataFormat; // not used, set to DRV_DATA_FORMAT_YUV420
  Uint16 maxWidth;
  Uint16 maxHeight;

} ALG_VidDecCreate;

typedef struct {

  Uint8 *inAddr;
  Uint32 inDataSize;

  Uint8 *outAddr;
  
  Uint32 inputBufId; 
  
  Uint32 outOffsetH; // must be == OSA_align(width+48, 32)
  Uint32 outOffsetV; // must be >= height+48
  
} ALG_VidDecRunPrm;

typedef struct {

  Uint32 bytesUsed;
  Uint32 isKeyFrame;

  Uint16 frameWidth;
  Uint16 frameHeight;
  
  Uint32 outputBufId;   // this buffer is available for display, but not yet released by decoder
  Uint32 freeBufId;     // this buffer is not used by decoder any mode and can be reused for display 
  
  Uint32 outStartX;     // start position of valid data in buffer
  Uint32 outStartY;     // start position of valid data in buffer

  Uint32 outOffsetH;    
  Uint32 outOffsetV; 

} ALG_VidDecRunStatus;


void *ALG_vidDecCreate(ALG_VidDecCreate *create);
int ALG_vidDecRun(void *hndl, ALG_VidDecRunPrm *prm, ALG_VidDecRunStatus *status);
int ALG_vidDecDelete(void *hndl);

#endif
