
#ifndef _DRV_AUDIO_H_
#define _DRV_AUDIO_H_

#include <drv.h>
#include <osa_buf.h>


typedef struct {

  void *hndl;

} DRV_AudioHndl;

typedef struct {

  Uint16 deviceId;
  Uint16 samplingRate;
  Uint16 numChannels;
  Uint16 buff_Of_Samples;
  Uint16 bufferSize;

} DRV_AudioConfig;


int DRV_audioOpen(DRV_AudioHndl *hndl, DRV_AudioConfig *config);
int DRV_audioClose(DRV_AudioHndl *hndl);

int DRV_audioRead(DRV_AudioHndl *hndl,void *pInBuf, int readsamples);


int DRV_audioGetBuf(DRV_AudioHndl *hndl, int *bufId);
int DRV_audioPutBuf(DRV_AudioHndl *hndl, int bufId);

int DRV_audioStart(DRV_AudioHndl *hndl);
int DRV_audioStop(DRV_AudioHndl *hndl);
int DRV_audioPause(DRV_AudioHndl *hndl);
int DRV_audioResume(DRV_AudioHndl *hndl);
int DRV_audioFlush(DRV_AudioHndl *hndl);
int DRV_audioSetVolume(DRV_AudioHndl *hndl, Uint32 volumeLevel);

int DRV_audioplayOpen(DRV_AudioHndl *hndl, DRV_AudioConfig *config);
int DRV_audioWrite(DRV_AudioHndl *hndl, void *pInBuf, int writeSamples);

#endif

