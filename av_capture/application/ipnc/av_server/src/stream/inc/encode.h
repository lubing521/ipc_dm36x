/*
 * encode.h
 *
 * ============================================================================
 * Copyright (c) Texas Instruments Inc 2007
 *
 * Use of this software is controlled by the terms and conditions found in the
 * license agreement under which this software has been supplied or provided.
 * ============================================================================
 */

#ifndef _ENCODE_H
#define _ENCODE_H

/* Standard Linux headers */
#include <stdio.h>
#include <pthread.h>
#include <stream.h>
/* Appro Test option*/
//#define __DEBUG


/* Enables or disables debug output */
#ifdef __DEBUG
#define DBG(fmt, args...) fprintf(stderr, "Encode Debug: " fmt, ## args)
#else
#define DBG(fmt, args...)
#endif

#define ERR(fmt, args...) fprintf(stderr, "Encode Error: " fmt, ## args);fprintf(stderr, "func:%s line:%d\n",__func__,__LINE__)

/* Macro for clearing structures */
#define CLEAR(x) memset (&(x), 0, sizeof (x))

/* Function error codes */
#define SUCCESS         0
#define FAILURE         -1

/* Thread error codes */
#define THREAD_SUCCESS  (void *) 0
#define THREAD_FAILURE  (void *) -1

/* The codec engine to use in this application */
#define ENGINE_NAME     "encode"

/* Device parameters */
#define V4L2_DEVICE     "/dev/video0"
#define OSD_DEVICE      "/dev/fb/0"
#define FBVID0_DEVICE	"/dev/fb/1"
#define ATTR_DEVICE     "/dev/fb/2"
#define FBVID_DEVICE    "/dev/fb/3"
#define SOUND_DEVICE    "/dev/dsp"
#define MIXER_DEVICE    "/dev/mixer"
#define V4L2VID0_DEVICE	"/dev/video2"
#define V4L2VID1_DEVICE	"/dev/video3"


/* True of false enumeration */
#ifndef TRUE
#define TRUE            1
#define FALSE           0
#endif

#ifndef ENABLE
#define ENABLE          1
#define DISABLE         0
#endif

/* Scaling factors for the video standards */
#define NOSTD           0
#define PALSTD          12
#define NTSCSTD         10

/* Screen dimensions */
#define SCREEN_BPP      16
#define YUV420SEMI_SCREEN_BPP 12
#define ATTR_BPP        4
#define MIN_WIDTH       16
#define MIN_HEIGHT      16
#define D1_WIDTH        720
#define D1_HEIGHT       yScale(480)
#define D1_LINE_WIDTH   D1_WIDTH * SCREEN_BPP / 8
#define D1_FRAME_SIZE   D1_LINE_WIDTH * D1_HEIGHT
#define WIDTH_720P	1280
#define HEIGHT_720P	720
#define LINE_WIDTH_720P   WIDTH_720P * SCREEN_BPP / 8

#define FRAME_SIZE_720P   LINE_WIDTH_720P * HEIGHT_720P

#define ENCODE_BUFFERS	(2)
#define EXT_STREAM_NUM	(2)

enum{
	BITRATE_RCOFF= 0,
	BITRATE_VBR,
	BITRATE_CBR,
	BITRATE_NUM
};

enum {
	FRAME_TYPE_MPEG4_1 = 0,
	FRAME_TYPE_MPEG4_2,
	FRAME_TYPE_JPEG,
	FRAME_TYPE_NUM
};


enum {
	FRAME_RATE_MPEG4_1 = 0,
	FRAME_RATE_MPEG4_2,
	FRAME_RATE_JPEG,
	FRAME_RATE_NUM
};

enum{
	MOTION_STREAM1 = 0,
	MOTION_STREAM2
};

enum{
	DTIME_FMT_DISABLE = 0,
	DTIME_FMT_YMD,
	DTIME_FMT_MDY,
	DTIME_FMT_DMY
};

void SetPtzCmd(int Getcmd);
void SetDrawDateTimeStatus( int IsDraw );
void SetOsdText(char *strText, int nLength);
void SetOsdTextEnable(int enable);
void SetOsdLogoEnable(int enable);
void SetHistogramEnable(int enable);
void SetROICfgEnable(int enable);

void VIDEO_frameRate_setparm( int type, unsigned int frameRate );
void VIDEO_jpeg_quality_setparm( int quality );
void VIDEO_bitRate_setparm(int type, int bitrate);
void AUDIO_aac_bitRate_setparm(int bitrate);

void VIDEO_motion_setparm(ApproMotionPrm* pMotionPrm);
void VIDEO_streamSetVNFStatus(unsigned int streamId, unsigned int value);
void VIDEO_streamSetFace(FaceDetectParam *faceParam);
void VIDEO_streamSetDateTimePrm(DateTimePrm* datetimeParam);
void VIDEO_streamOsdPrm(OSDPrm* osdPrm, int id);
void VIDEO_streamSetOSDEnable(int value);
void VIDEO_streamROIPrm(CodecROIPrm* codecROIPrm, int id);
void VIDEO_codecAdvPrm(CodecAdvPrm* codecAdvPrm, int id);
void VIDEO_codecReset(int enable);

void AUDIO_alarmLevel(int value);
void AUDIO_alarmFlag(int enable);

void VIDEO_aewbSetType(int value);
void VIDEO_aewbPriority(int value);
void VIDEO_dynRangePrm(DynRangePrm* dynRangePrm);
void VIDEO_mirrorPrm(int value);



////////zeng add


void VIDEO_SetRateControlType(int streamId , int nRcType);

void VIDEO_SetCapturePrm(int id ,MSGVCAPCFG* pVcapCfg);
int VIDEO_GetCapturePrm(int id ,MSGVCAPCFG* pVcapCfg);

void VIDEO_SetEncodePrm(int id , MSGVENCCFG* pVencCfg);
int VIDEO_GetEncodePrm(int id , MSGVENCCFG* pVencCfg);



void OSD_SetEnable(int nType , int bEnable);
void OSD_SetTextStr(char* pStr , int nLen);

void MASK_SetVideoMask(int nNumber, int bEnable ,int nx0, int ny0, int nx1, int ny1);


int SetSnapName(char *strText, int nLength);
void SetSnapLocation(int value);

/* Which operation to performed on the captured data before encoding */
typedef enum CaptureOperation {
    CAPOP_COPY,
    CAPOP_SMOOTH,
} CaptureOperation;

/* The speech encoder supported by this demo */
typedef enum SpeechEncoder {
    G711_SPEECH_ENCODER,
    NUM_SPEECH_ENCODERS
} SpeechEncoder;

/* The video encoders supported by this demo */
typedef enum VideoEncoder {
    MPEG4_VIDEO_ENCODER,
    H264_VIDEO_ENCODER,
    JPEG_VIDEO_ENCODER,
    NUM_VIDEO_ENCODERS
} VideoEncoder;

/* The video encoders supported by this demo */
typedef enum UseEncoder {
    MPEG4_USE_ENCODER,
    JPEG_USE_ENCODER,
    JPEG_MPEG4_USE_ENCODER,
    MPEG4_DUAL_USE_ENCODER,
    TRIPLE_USE_ENCODER,
    H264_MPEG4_USE_ENCODER,
    NUM_USE_ENCODERS
} UseEncoder;


/* Whether to use 'mic in' or 'line in' for sound input */
typedef enum SoundInput {
    MIC_SOUND_INPUT,
    LINEIN_SOUND_INPUT
} SoundInput;

/* Global data structure */
typedef struct GlobalData {
    int             quit;                /* Global quit flag */
    int             frames;              /* Video frame counter */
    int             videoBytesEncoded;   /* Video bytes encoded counter */
    int             soundBytesEncoded;   /* Sound bytes encoded counter */
    int             imageWidth;          /* Width of clip */
    int             imageHeight;         /* Height of clip */
    int             yFactor;             /* Vertical scaling (PAL vs. NTSC) */
    pthread_mutex_t mutex;               /* Mutex to protect the global data */
} GlobalData;

/* Global data */
extern GlobalData gbl;

/* Functions to protect the global data */
static inline int gblGetQuit(void)
{
    int quit;

    pthread_mutex_lock(&gbl.mutex);
    quit = gbl.quit;
    pthread_mutex_unlock(&gbl.mutex);

    return quit;
}

static inline void gblSetQuit(void)
{
    pthread_mutex_lock(&gbl.mutex);
    gbl.quit = TRUE;
    pthread_mutex_unlock(&gbl.mutex);
}

static inline int gblGetAndResetFrames(void)
{
    int frames;

    pthread_mutex_lock(&gbl.mutex);
    frames = gbl.frames;
    gbl.frames = 0;
    pthread_mutex_unlock(&gbl.mutex);

    return frames;
}

static inline void gblIncFrames(void)
{
    pthread_mutex_lock(&gbl.mutex);
    gbl.frames++;
    pthread_mutex_unlock(&gbl.mutex);
}

static inline int gblGetAndResetVideoBytesEncoded(void)
{
    int videoBytesEncoded;

    pthread_mutex_lock(&gbl.mutex);
    videoBytesEncoded = gbl.videoBytesEncoded;
    gbl.videoBytesEncoded = 0;
    pthread_mutex_unlock(&gbl.mutex);

    return videoBytesEncoded;
}

static inline void gblIncVideoBytesEncoded(int videoBytesEncoded)
{
    pthread_mutex_lock(&gbl.mutex);
    gbl.videoBytesEncoded += videoBytesEncoded;
    pthread_mutex_unlock(&gbl.mutex);
}

static inline int gblGetAndResetSoundBytesEncoded(void)
{
    int soundBytesEncoded;

    pthread_mutex_lock(&gbl.mutex);
    soundBytesEncoded = gbl.soundBytesEncoded;
    gbl.soundBytesEncoded = 0;
    pthread_mutex_unlock(&gbl.mutex);

    return soundBytesEncoded;
}

static inline void gblIncSoundBytesEncoded(int soundBytesEncoded)
{
    pthread_mutex_lock(&gbl.mutex);
    gbl.soundBytesEncoded += soundBytesEncoded;
    pthread_mutex_unlock(&gbl.mutex);
}

static inline int gblGetImageWidth(void)
{
    int imageWidth;

    pthread_mutex_lock(&gbl.mutex);
    imageWidth = gbl.imageWidth;
    pthread_mutex_unlock(&gbl.mutex);

    return imageWidth;
}

static inline void gblSetImageWidth(int imageWidth)
{
    pthread_mutex_lock(&gbl.mutex);
    gbl.imageWidth = imageWidth;
    pthread_mutex_unlock(&gbl.mutex);
}

static inline int gblGetImageHeight(void)
{
    int imageHeight;

    pthread_mutex_lock(&gbl.mutex);
    imageHeight = gbl.imageHeight;
    pthread_mutex_unlock(&gbl.mutex);

    return imageHeight;
}

static inline void gblSetImageHeight(int imageHeight)
{
    pthread_mutex_lock(&gbl.mutex);
    gbl.imageHeight = imageHeight;
    pthread_mutex_unlock(&gbl.mutex);
}

static inline int gblGetYFactor(void)
{
    int yFactor;

    pthread_mutex_lock(&gbl.mutex);
    yFactor = gbl.yFactor;
    pthread_mutex_unlock(&gbl.mutex);

    return yFactor;
}

static inline void gblSetYFactor(int yFactor)
{
    pthread_mutex_lock(&gbl.mutex);
    gbl.yFactor = yFactor;
    pthread_mutex_unlock(&gbl.mutex);
}

/* Scale vertically depending on video standard */
#define yScale(x) (((x) * gblGetYFactor()) / 10)

/* Cleans up cleanly after a failure */
#define cleanup(x)                                  \
    status = (x);                                   \
    gblSetQuit();                                   \
    goto cleanup

#define cleanup2(x)                                  \
    status = (x);                                   \
    gblSetQuit();                                   \
    goto cleanup2


/* Breaks a processing loop for a clean exit */
#define breakLoop(x)                                \
    status = (x);                                   \
    gblSetQuit();                                   \
    break


#define MAX_STREAM_EXT_NUM	(2)
typedef struct	_Res_Set
{
	int	width;
	int	height;
	int	index;
	int	diff;
}Res_Set;
typedef struct _stream_ext_params
{
	int	ext_res_num;
	Res_Set	res_setting[MAX_STREAM_EXT_NUM];
} stream_ext_params;
#define VPFE_EXT_HEADER_LEN	(0)//( ((sizeof(Res_Set)+31)/32)*32 )

#endif /* _ENCODE_H */
