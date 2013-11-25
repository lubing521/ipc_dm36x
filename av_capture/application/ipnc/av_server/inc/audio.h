
#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <osa_tsk.h>
#include <osa_buf.h>
#include <osa_mutex.h>
#include <osa_flg.h>
#include <drv_audio.h>

#include <alg_audEncDec.h>

#define AUD_STREAM_NEWFRAME (0x800)
#define AUD_STREAM_STOP     (0x801)

#define AUD_ALARM_START     (0x900)
#define AUD_ALARM_END       (0x901)

typedef struct {
  long mtype;
  int     msg;
  int  sampleRate;
} message_buf;

extern int snd_msg_qid;
extern message_buf  sbuf;

typedef struct {
	void *physAddr;
	void *virtAddr;
	Uint32 timestamp;
	Uint32 encFrameSize;
} AUDIO_BufInfo;

typedef enum {
    AUD_PLAY_MODE_PLAY,
    AUD_PLAY_MODE_PAUSE
}AUD_PLAY_MODE_t;

typedef struct {
    void 			*algEncHndl;
	OSA_TskHndl 	audioTsk;
	DRV_AudioHndl 	audioHndl;
	Uint8			*inputBuffer;
	Uint8			*outputBuffer;
	Uint32			inputBufSize;
	Uint32			outputBufSize;
	Uint32          curPlaySampleRate;
	AUD_PLAY_MODE_t audPlayMode;
} AUDIO_Ctrl;

typedef struct {
    void 			*algEncHndl;
	OSA_TskHndl 	audioTsk;
	DRV_AudioHndl 	audioHndl;
	Uint8			*inputBuffer;
	Uint8			*outputBuffer;
	Uint32			inputBufSize;
	Uint32			outputBufSize;
} AUDIOPLAY_Ctrl;

int AUDIO_audioCreate();
int AUDIO_audioDelete();

int AUDIOPLAY_audioCreate();
int AUDIOPLAY_audioDelete();

int AUDIO_streamShmCopy(AUDIO_BufInfo *pBufInfo);

#endif  /*  _AUDIO_H_ */
