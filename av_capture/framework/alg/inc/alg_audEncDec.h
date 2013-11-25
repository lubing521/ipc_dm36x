
#ifndef _ALG_AUD_ENC_DEC_H_
#define _ALG_AUD_ENC_DEC_H_

#include <alg.h>

//#define ALG_AUD_INP_FILE_DUMP_DEBUG
//#define ALG_AUD_ENC_FILE_DUMP_DEBUG
//#define ALG_AUD_ENC_DEBUG
//#define TIINTERNAL_G711

typedef enum {
	PCM_ALAW=0,
	PCM_ULAW,
	MPEG2_AUDIO,
	MPEG4_AUDIO
}AUD_ENC_MODE;

#define AUDIOALARM_THRESHOLD	(1)

typedef struct {

  Uint16 codec;
  Uint16 encMode;
  Uint16 numSamples;

  Uint32 sampRate;
  Uint32 bitRate;

} ALG_AudEncPrm;

typedef struct {

  Uint16 codec;

} ALG_AudDecPrm;

typedef struct {

  Uint8 *inAddr;
  Uint8 *outAddr;

  Uint32 inDataSize;
  Uint32 outBufMaxSize;

} ALG_AudioRunPrm;

typedef struct {

  Uint32 inDataUsedSize;
  Uint32 outDataSize;

} ALG_AudioRunStatus;

int ALG_audEncDecInit();
int ALG_audEncDecExit();

void *ALG_audEncCreate(ALG_AudEncPrm *prm);
int ALG_audDynparams(void *hndl, ALG_AudEncPrm *prm);
int ALG_audEncRun(void *hndl, ALG_AudEncPrm *encPrm, ALG_AudioRunPrm *runPrm, ALG_AudioRunStatus *status);
int ALG_audEncDelete(void *hndl, ALG_AudEncPrm *prm);
int ALG_audAlarmCheck(Uint8 *inpBuf, int numSamples, int alarmLevel);

#ifdef TIINTERNAL_G711
int ALG_TI_ulawEncode(short *dst, short *src, short bufsize);
#endif

void *ALG_audDecCreate(ALG_AudDecPrm *prm);
int ALG_audDecRun(void *hndl, ALG_AudioRunPrm *prm, ALG_AudioRunStatus *status);
int ALG_audDecDelete(void *hndl);


#endif

