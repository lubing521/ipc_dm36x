/*
 * Libavformat API example: Output a media file in any supported
 * libavformat format. The default codecs are used.
 *
 * Copyright (c) 2003 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <ApproDrvMsg.h>
#include <Appro_interface.h>
#include <Msg_Def.h>
#include <sys/dir.h>
#include <sys/statfs.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <sys/ioctl.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <avformat.h>
#include <swscale.h>

#undef exit

#define ERR(fmt, args...) fprintf(stdout, "%s: error: " fmt, __FILE__, ##args)
//#define DBGLOGS(fmt, args...) fprintf(stdout, "%d: log: " fmt, __LINE__, ##args)
#define DBGLOGS(fmt, args...)

/* 5 seconds stream duration */
int STREAM_FRAME_RATE = 60;
int STREAM_FRAME_ID	= 1;

int PRE_BUFFER = 30;

#define STREAM_PIX_FMT PIX_FMT_YUV420P /* default pix_fmt */

char MMC_PATH[] = "/mnt/mmc";
char TMP_PATH[] = "/tmp";

char *pCurr_path = TMP_PATH;

/**************************************************************/
/* audio output */

float t, tincr, tincr2;
int16_t *samples;
uint8_t *audio_outbuf;
int audio_outbuf_size;
int audio_input_frame_size;
int AbortSignal = 0;
double Pts_Addon_Duration = 0;

int Audio_Serial_Start = -1;
int Video_Serial_Start = 0;
int audio_frame_count=0;
int video_frame_count=0;

static int mpeg4_field[7][3] =
{
{AV_OP_LOCK_MP4_VOL,			AV_OP_LOCK_MP4_CIF_VOL,				-1},
{AV_OP_UNLOCK_MP4_VOL,			AV_OP_UNLOCK_MP4_CIF_VOL,			-1},
{AV_OP_LOCK_MP4,				AV_OP_LOCK_MP4_CIF,					AV_OP_LOCK_MJPEG},
{AV_OP_LOCK_MP4_IFRAME, 		AV_OP_LOCK_MP4_CIF_IFRAME,			AV_OP_LOCK_MJPEG},
{AV_OP_UNLOCK_MP4,				AV_OP_UNLOCK_MP4_CIF,				AV_OP_UNLOCK_MJPEG},
{AV_OP_GET_MPEG4_SERIAL,		AV_OP_GET_MPEG4_CIF_SERIAL,			AV_OP_GET_MJPEG_SERIAL},
{AV_OP_WAIT_NEW_MPEG4_SERIAL,	AV_OP_WAIT_NEW_MPEG4_CIF_SERIAL,	AV_OP_WAIT_NEW_MJPEG_SERIAL}};

#define LOCK_MP4_VOL			0
#define UNLOCK_MP4_VOL			1
#define LOCK_MP4				2
#define LOCK_MP4_IFRAM			3
#define UNLOCK_MP4				4
#define GET_MPEG4_SERIAL		5
#define WAIT_NEW_MPEG4_SERIAL	6

int GetAudioInterface( unsigned long *pAddr , int *pSize, unsigned int *pTime, int IsClean);
int GetVideoInterface( unsigned long *pAddr , int *pSize, int *IsKey, unsigned int *ptime ,int IsClean);
void WaitVideoStart( void );


int GetVideoInfo( int typeID, AV_DATA *pInfo )
{
	int vType = typeID;
	//AV_DATA av_data;

	if( GetAVData(mpeg4_field[GET_MPEG4_SERIAL][vType], -1, pInfo )!= RET_SUCCESS )
	{
		return -1;
	}else{
		//memcpy( pInfo, &av_data, sizeof(AV_DATA) );
	}

	return 0;
}

/**
*	0 	: read
*	> 0 	: write
*
*/
int64_t Controlbasetime( int64_t SetVal )
{
	static int64_t basetime = 0;

	if( SetVal == 0 )
	{
		return basetime;
	}
	else
	{

		if( basetime )
		{
			if( SetVal > basetime )
			{
				basetime = SetVal;
			}
		}
		else
		{
			basetime = SetVal;
		}
	}

	return basetime;

}

int GetSampleRate( void )
{
	static int CurrentStatus = -255;
	int fd_proc = -1;
	int ret = -1;
	char StrBuffer[300];
	char TmpBuf[50];
	char *pStr = NULL;
	int samplerate = 0;
	char delima_buf[] = ":\r\n ";

	if( CurrentStatus >= -1 )
	{
		fprintf(stderr,"CurrentStatus is = %d \n", CurrentStatus);
		return CurrentStatus;
	}


	fd_proc = open("/proc/asound/card0/pcm0c/sub0/hw_params", O_RDONLY);
	if( fd_proc < 0 )
	{
		fprintf(stderr,"GetSampleRate open file fail \n");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = read(fd_proc,  StrBuffer, sizeof(StrBuffer)-1);
	if( ret <= 0 )
	{
		fprintf(stderr,"read device error !!");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = -1;
	StrBuffer[sizeof(StrBuffer)-1] = '\0';
	pStr = strtok(StrBuffer,delima_buf );

	while( pStr != NULL )
	{
		sscanf( pStr,"%s",TmpBuf);

		if( strncmp(TmpBuf, "rate", sizeof("rate")) == 0 )
		{

			pStr = strtok(NULL, delima_buf);

			sscanf( pStr,"%d",&samplerate);

			fprintf(stderr,"samplerate = %d \n", samplerate);

			ret = samplerate;
			goto CHECK_CPU_END;


		}

		pStr = strtok(NULL, delima_buf);

	}

CHECK_CPU_END:

	if( fd_proc >= 0 )
		close(fd_proc);


	CurrentStatus = ret;

	return ret;


}


/*
 * add an audio output stream
 */
static AVStream *add_audio_stream(AVFormatContext *oc, int codec_id,int BitRate)
{
    AVCodecContext *c;
    AVStream *st;

    st = av_new_stream(oc, 1);
    if (!st) {
        fprintf(stderr, "Could not alloc stream\n");
        exit(1);
    }

    c = st->codec;
    c->codec_id = codec_id;
    c->codec_type = CODEC_TYPE_AUDIO;


    /* put sample parameters */

	if( GetSampleRate() == 16000 )
	{
		 if( BitRate != 32000 && BitRate != 48000 && BitRate != 64000 )
		 {
			BitRate = 32000;
		 }

		 if( codec_id == CODEC_ID_PCM_MULAW )
    		{
			c->bit_rate = 128000;
		}else{
			c->bit_rate = BitRate;
		}
		c->sample_rate = 16000;
	}else{

		 if( BitRate != 24000 && BitRate != 36000 && BitRate != 48000 )
		 {
			BitRate = 24000;
		 }

		 if( codec_id == CODEC_ID_PCM_MULAW )
    		{
			c->bit_rate = 64000;
		}else{
			c->bit_rate = BitRate;
		}
		c->sample_rate = 8000;
	}

    c->channels = 1;

     if(oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= CODEC_FLAG_GLOBAL_HEADER;
    return st;
}

static void open_audio(AVFormatContext *oc, AVStream *st)
{
    AVCodecContext *c;
    AVCodec *codec;

    c = st->codec;

    /* find the audio encoder */
    codec = avcodec_find_encoder(c->codec_id);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }

    /* open it */
    if (avcodec_open(c, codec) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }

    /* init signal generator */
    t = 0;
    tincr = 2 * M_PI * 110.0 / c->sample_rate;
    /* increment frequency by 110 Hz per second */
    tincr2 = 2 * M_PI * 110.0 / c->sample_rate / c->sample_rate;

    audio_outbuf_size = 8000;
    audio_outbuf = av_malloc(audio_outbuf_size);

    /* ugly hack for PCM codecs (will be removed ASAP with new PCM
       support to compute the input frame size in samples */
    if (c->frame_size <= 1) {
        audio_input_frame_size = audio_outbuf_size / c->channels;
        switch(st->codec->codec_id) {
        case CODEC_ID_PCM_S16LE:
        case CODEC_ID_PCM_S16BE:
        case CODEC_ID_PCM_U16LE:
        case CODEC_ID_PCM_U16BE:
            audio_input_frame_size >>= 1;
            break;
        default:
            break;
        }
    } else {
        audio_input_frame_size = c->frame_size;
    }
    samples = av_malloc(audio_input_frame_size * 2 * c->channels);
}

static int write_audio_frame(AVFormatContext *oc, AVStream *st)
{
    AVCodecContext *c;
    AVPacket pkt;
	unsigned long ulAddr = 0;
	int			audio_size = 0;

	static int64_t audioLasttime = -1;
	unsigned int audiotime = 0;
	int64_t audiotimediff = 0;
	int64_t InvCnt = 0;
	static int64_t LastCnt = -1;

    av_init_packet(&pkt);

    c = st->codec;

	if( GetAudioInterface(&ulAddr,&audio_size,&audiotime,0) < 0)
	{
		return -1;
	}
	if( audioLasttime < 0 )
	{
		audioLasttime =  audiotime;
		Controlbasetime( audiotime );
	}

    pkt.size= audio_size;//avcodec_encode_audio(c, audio_outbuf, audio_outbuf_size, samples);
 if (c->coded_frame && c->coded_frame->pts != AV_NOPTS_VALUE)
    pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, st->time_base);
    pkt.flags |= PKT_FLAG_KEY;
    pkt.stream_index= st->index;
    pkt.data= (uint8_t *)ulAddr;//audio_outbuf;

    audiotimediff =audiotime - Controlbasetime(0);
	if( audiotimediff < 0 )
	{
		audiotimediff = 0;
	}
    InvCnt = ((audiotimediff) *st->time_base.den+500 )/ (st->time_base.num *1000);

    if(LastCnt >= InvCnt )
    {
	InvCnt=LastCnt+1;
    }

    st->pts.val = InvCnt;


	LastCnt = InvCnt;
/*
    fprintf( stderr, "\n\naudio num = %f\n", (double)st->time_base.num);
	fprintf( stderr, "audio den = %f\n", (double)st->time_base.den);
	fprintf(stderr, "audiotimediff = %f\n", (double)audiotimediff);
	fprintf(stderr, "audio st->pts.val = %f\n", (double)st->pts.val);
	fprintf(stderr, "InvCnt = %f\n", (double)InvCnt);
	fprintf(stderr, "LastCnt = %f\n", (double)LastCnt);
*/
    /* write the compressed frame in the media file */
    if (av_write_frame(oc, &pkt) != 0) {
        fprintf(stderr, "Error while writing audio frame\n");
        exit(1);
    }
    audio_frame_count++;

	return  0;
}

static void close_audio(AVFormatContext *oc, AVStream *st)
{
    avcodec_close(st->codec);

    av_free(samples);
    av_free(audio_outbuf);
}

/**************************************************************/
/* video output */

AVFrame *picture, *tmp_picture;
uint8_t *video_outbuf;
int video_outbuf_size;

/* add a video output stream */
static AVStream *add_video_stream(AVFormatContext *oc, int codec_id)
{
    AVCodecContext *c;
    AVStream *st;
	AV_DATA av_Info;

	if( GetVideoInfo( STREAM_FRAME_ID, &av_Info ) < 0 )
	{
		fprintf(stderr, "Could not GetVideoInfo() \n");
        exit(1);
	}

    st = av_new_stream(oc, 0);
    if (!st) {
        fprintf(stderr, "Could not alloc stream\n");
        exit(1);
    }

    c = st->codec;
    c->codec_id = codec_id;
    c->codec_type = CODEC_TYPE_VIDEO;

    /* put sample parameters */
    c->bit_rate = 4000000;//av_Info.quality;
    /* resolution must be a multiple of two */
    c->width = av_Info.width;
    c->height = av_Info.height;
    /* time base: this is the fundamental unit of time (in seconds) in terms
       of which frame timestamps are represented. for fixed-fps content,
       timebase should be 1/framerate and timestamp increments should be
       identically 1. */
    c->time_base.den = (STREAM_FRAME_RATE)/100;
    c->time_base.num = 1;//100;
    c->gop_size = 10;

    c->me_range = 16;
    c->max_qdiff = 4;
    c->qmin = 10;
    c->qmax = 51;
    c->qcompress = 0.6;
    c->flags = 0;
	if( STREAM_FRAME_ID < 2 )
	{
		c->pix_fmt = STREAM_PIX_FMT;
	}else{
		c->pix_fmt = PIX_FMT_YUVJ420P;
	}
    if (c->codec_id == CODEC_ID_MPEG2VIDEO) {
        /* just for testing, we also add B frames */
        c->max_b_frames = 2;
    }
    if (c->codec_id == CODEC_ID_MPEG1VIDEO){
        /* Needed to avoid using macroblocks in which some coeffs overflow.
           This does not happen with normal video, it just happens here as
           the motion of the chroma plane does not match the luma plane. */
        c->mb_decision=2;
    }
    // some formats want stream headers to be separate
   // if(!strcmp(oc->oformat->name, "mp4") || !strcmp(oc->oformat->name, "mov") || !strcmp(oc->oformat->name, "3gp"))
       // c->flags |= CODEC_FLAG_GLOBAL_HEADER;
      if(oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= CODEC_FLAG_GLOBAL_HEADER;

    return st;
}

static void open_video(AVFormatContext *oc, AVStream *st)
{
    AVCodec *codec;
    AVCodecContext *c;

    c = st->codec;

    /* find the video encoder */
    codec = avcodec_find_encoder(c->codec_id);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }

    /* open the codec */
    if (avcodec_open(c, codec) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }

    video_outbuf = NULL;
	picture	= NULL;
	tmp_picture = NULL;
#if 0
    if (!(oc->oformat->flags & AVFMT_RAWPICTURE)) {
        /* allocate output buffer */
        /* XXX: API change will be done */
        /* buffers passed into lav* can be allocated any way you prefer,
           as long as they're aligned enough for the architecture, and
           they're freed appropriately (such as using av_free for buffers
           allocated with av_malloc) */
        video_outbuf_size = 200000;
        video_outbuf = av_malloc(video_outbuf_size);
    }

    /* allocate the encoded raw picture */
    picture = alloc_picture(c->pix_fmt, c->width, c->height);
    if (!picture) {
        fprintf(stderr, "Could not allocate picture\n");
        exit(1);
    }

    /* if the output format is not YUV420P, then a temporary YUV420P
       picture is needed too. It is then converted to the required
       output format */
    tmp_picture = NULL;
    if (c->pix_fmt != PIX_FMT_YUV420P) {
        tmp_picture = alloc_picture(PIX_FMT_YUV420P, c->width, c->height);
        if (!tmp_picture) {
            fprintf(stderr, "Could not allocate temporary picture\n");
            exit(1);
        }
    }
#endif
}


static int write_video_frame(AVFormatContext *oc, AVStream *st)
{
	unsigned long ulAddr = 0;
    int out_size;
	int IsKey = 0;
	static int64_t videoLasttime = -1;
	unsigned int videotime = 0;
	int64_t videotimediff = 0;
	int64_t InvCnt = 0;
	int ret;
    AVCodecContext *c;
	static int syncIframe = 0;
	static int64_t lastCnt = -1;


    c = st->codec;

    /* encode the image */
	if( GetVideoInterface( &ulAddr,&out_size,&IsKey, &videotime ,0) < 0 )
		return -1;

	//fprintf( stderr,"video time = %f \n", videotime );
	if( videoLasttime < 0 )
	{
		videoLasttime = videotime;
		Controlbasetime( videotime );
	}

	c->coded_frame->key_frame = IsKey;

#define SYNC_IDR_FRAME
#ifdef SYNC_IDR_FRAME
	//fprintf( stderr,"AVI DEBUG MSG:%d %d\n", IsKey, syncIframe);
	/* Code added so that every AVI starts with I frame or IDR frame */
	if(IsKey){
		syncIframe=1;
		//fprintf( stderr,"AVI DEBUG MSG: SYNC UP DONE\n");
	}
	else {
		if(syncIframe==0)
			return 0;
	}
#endif

    if (out_size > 0) {
        AVPacket pkt;
        av_init_packet(&pkt);
  if (c->coded_frame->pts != AV_NOPTS_VALUE)
        pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, st->time_base);
        if(c->coded_frame->key_frame)
            pkt.flags |= PKT_FLAG_KEY;
        pkt.stream_index= st->index;
        pkt.data= (uint8_t *)ulAddr;//video_outbuf;
        pkt.size= out_size;

		videotimediff = videotime - Controlbasetime(0);
		if( videotimediff < 0 )
		{
			videotimediff = 0;
		}
		InvCnt = (videotimediff*st->time_base.den+(int64_t)500) / (st->time_base.num*(int64_t)1000);

		if( lastCnt >= InvCnt)
		{
			InvCnt=lastCnt+1;
		}


		st->pts.val = InvCnt;

		lastCnt = st->pts.val;

/*
		fprintf( stderr, "\n\n videonum = %f\n", (double)st->time_base.num);
		fprintf( stderr, " video den = %f\n", (double)st->time_base.den);
		fprintf( stderr, " videotimediff = %f\n", (double)videotimediff);
		fprintf( stderr, " videopts.val = %f\n\n", (double)st->pts.val);
		fprintf( stderr, " lastCnt = %f\n\n", (double)lastCnt);
*/

            /* write the compressed frame in the media file */
            ret = av_write_frame(oc, &pkt);
    } else {
        ret = 0;
    }

    if (ret != 0) {
        fprintf(stderr, "Error while writing video frame\n");
        exit(1);
    }
    video_frame_count = InvCnt;

	return 0;
}

static void close_video(AVFormatContext *oc, AVStream *st)
{
    avcodec_close(st->codec);
	if( picture )
	{
    	av_free(picture->data[0]);
	    av_free(picture);
	}
    if (tmp_picture) {
        av_free(tmp_picture->data[0]);
        av_free(tmp_picture);
    }
	if( video_outbuf )
	{
    	av_free(video_outbuf);
	}
}

void WaitVideoStart( void )
{
	int vType = STREAM_FRAME_ID;
	AV_DATA av_data;

	DBGLOGS("func :%s\n",__func__);
	while(1)
	{
		GetAVData(mpeg4_field[GET_MPEG4_SERIAL][vType], -1, &av_data );
		if( vType < 2 )
		{
			if( av_data.flags != AV_FLAGS_MP4_I_FRAME )
			{
				usleep(500);
			}else{

				Video_Serial_Start = av_data.serial - PRE_BUFFER;
				Audio_Serial_Start = -1;

				break;
			}
		}else{
			Video_Serial_Start = av_data.serial - PRE_BUFFER;
			Audio_Serial_Start = -1;
			break;
		}
	}

	DBGLOGS("func :%s\n",__func__);
}

char AVI_FirstFrame[1024*1024];


int GetVideoInterface( unsigned long *pAddr , int *pSize, int *IsKey , unsigned int *ptime ,int IsClean)
{
	AV_DATA vol_data;
	int vType = STREAM_FRAME_ID;
	static int	SerialBook = -1;
	static int	SerialLock = -1;
	static int	IsFirst = 1;
	int	nOffset = 0;
	int	ret = 0;
	AV_DATA av_data;
	char mpeg4_header[] = {0x00, 0x00, 0x01, 0xb0, 0x01, 0x00, 0x00, 0x01, 0xb5, 0x09};
	DBGLOGS("func :%s\n",__func__);
	if( IsClean == 0 )
	{
		DBGLOGS("func :%s\n",__func__);
		if( SerialBook < 0 )
		{
			DBGLOGS("func :%s\n",__func__);
			if( vType < 2 )
			{
				DBGLOGS("func :%s\n",__func__);
				memcpy( (AVI_FirstFrame+nOffset), mpeg4_header, sizeof(mpeg4_header));
				nOffset += sizeof(mpeg4_header);

				if(GetAVData(mpeg4_field[LOCK_MP4_VOL][vType], -1, &vol_data) != RET_SUCCESS)
				{
					ERR("Error on Get Vol data\n");
					return -1;
				}

				memcpy( (AVI_FirstFrame+nOffset), vol_data.ptr, vol_data.size);
				nOffset += vol_data.size;
				WaitVideoStart();
				SerialBook = Video_Serial_Start;
			}else{
				GetAVData(mpeg4_field[GET_MPEG4_SERIAL][vType], -1, &av_data );
				SerialBook = av_data.serial;
			}
		}

		DBGLOGS("func :%s\n",__func__);
		while(1)
		{
			DBGLOGS("func :%s\n",__func__);
			ret = GetAVData(mpeg4_field[LOCK_MP4][vType], SerialBook, &av_data );

			DBGLOGS("func :%s\n",__func__);
			if (ret == RET_SUCCESS)
			{
				DBGLOGS("func :%s\n",__func__);
				if( IsFirst == 1 )
				{
					DBGLOGS("func :%s\n",__func__);
					memcpy( (AVI_FirstFrame+nOffset), av_data.ptr, av_data.size);
					nOffset += av_data.size;

					*pAddr = (unsigned long)AVI_FirstFrame;
					*pSize = nOffset;

					Audio_Serial_Start = av_data.ref_serial[FMT_AUDIO]+1;

					//fprintf(stderr,"Video_Serial_Start = %d \n",Video_Serial_Start);
					//fprintf(stderr,"Audio_Serial_Start = %d \n",Audio_Serial_Start);

					IsFirst = 0;
				}else{
					DBGLOGS("func :%s\n",__func__);
					*pAddr = (unsigned long)av_data.ptr;
					*pSize = (int)av_data.size;
				}
				DBGLOGS("func :%s\n",__func__);
				if( av_data.flags == AV_FLAGS_MP4_I_FRAME )
				{
					*IsKey = 1;
				}
				else
				{
					*IsKey = 0;
				}

				*ptime = av_data.timestamp;

				if (SerialLock >= 0)
				{
					DBGLOGS("func :%s\n",__func__);
					GetAVData(mpeg4_field[UNLOCK_MP4][vType], SerialLock, &av_data);
				}
				DBGLOGS("func :%s\n",__func__);

				SerialLock = SerialBook;
				SerialBook++;



				return 0;
			}
			else if (ret == RET_NO_VALID_DATA)
			{
				DBGLOGS("func :%s\n",__func__);
				/* wait new frame */
				//GetAVData(mpeg4_field[WAIT_NEW_MPEG4_SERIAL][vType], -1, &av_data );
				//usleep(5);
				return -1;
			}
			else
			{
				DBGLOGS("func :%s\n",__func__);
				if( vType < 2 )
				{
					DBGLOGS("func :%s\n",__func__);
					WaitVideoStart();
					GetAVData(mpeg4_field[GET_MPEG4_SERIAL][vType], -1, &av_data );
					SerialBook = av_data.serial;
				}else{
					DBGLOGS("func :%s\n",__func__);
					GetAVData(mpeg4_field[GET_MPEG4_SERIAL][vType], -1, &av_data );
					if( av_data.serial < SerialBook )
					{
						SerialBook = av_data.serial+1;
						return -1;
					}else{
						SerialBook = av_data.serial;
					}

				}
				DBGLOGS("func :%s\n",__func__);

			}

		}
	}
	else
	{
		DBGLOGS("func :%s\n",__func__);
		GetAVData(mpeg4_field[UNLOCK_MP4][vType], SerialLock, &av_data);
		SerialBook = -1;
		SerialLock = -1;
		IsFirst = 1;
		DBGLOGS("func :%s\n",__func__);
	}

	return 0;
}


int SaveVideoFile( char *pFileName , int	save_cnt)
{
	FILE *pfd = NULL;
	AV_DATA vol_data;
	int vType = STREAM_FRAME_ID;
	int error_code = 0;
	int	SerialBook = -1;
	int	SerialLock = -1;
	int	ret = 0;
	AV_DATA av_data;
	char mpeg4_header[] = {0x00, 0x00, 0x01, 0xb0, 0x01, 0x00, 0x00, 0x01, 0xb5, 0x09};

	pfd = fopen(pFileName,"wb");
	if( pfd == NULL )
	{
		ERR("open file error!!");
		error_code = -1;
		goto RECORD_QUIT;
	}


	if( fseek( pfd, 0, SEEK_SET ) < 0)
	{
		ERR("seek file error!!");
		error_code = -1;
		goto RECORD_QUIT;
	}
	if( fwrite( mpeg4_header , sizeof(char), sizeof(mpeg4_header), pfd ) <= 0 )
	{
		ERR("save file error!!");
		error_code = -1;
		goto RECORD_QUIT;
	}

	if(GetAVData(mpeg4_field[LOCK_MP4_VOL][vType], -1, &vol_data) != RET_SUCCESS)
	{
		ERR("Error on Get Vol data\n");
		goto RECORD_QUIT;
	}

	if( fwrite( vol_data.ptr , sizeof(char), vol_data.size, pfd )  <= 0)
	{
		ERR("save file error!!");
		error_code = -1;
		goto RECORD_QUIT;
	}


	GetAVData(mpeg4_field[UNLOCK_MP4_VOL][vType], -1, &vol_data);
	do
	{
		GetAVData(mpeg4_field[GET_MPEG4_SERIAL][vType], -1, &av_data );
	} while (av_data.flags != AV_FLAGS_MP4_I_FRAME);
	SerialBook = av_data.serial;


	while( save_cnt > 0)
	{

		ret = GetAVData(mpeg4_field[LOCK_MP4][vType], SerialBook, &av_data );


		if (ret == RET_SUCCESS)
		{
			if( fwrite( av_data.ptr,av_data.size,1, pfd )  <= 0)
			{
				ERR("save file error!!");
				error_code = -1;
				goto RECORD_QUIT;
			}


			if (SerialLock >= 0)
			{
				GetAVData(mpeg4_field[UNLOCK_MP4][vType], SerialLock, &av_data);
			}

			SerialLock = SerialBook;

			save_cnt--;
			SerialBook++;
		}
		else if (ret == RET_NO_VALID_DATA) {

			/* wait new frame */
			GetAVData(mpeg4_field[WAIT_NEW_MPEG4_SERIAL][vType], -1, &av_data );

		}
		else {

			GetAVData(mpeg4_field[GET_MPEG4_SERIAL][vType], -1, &av_data );
			SerialBook = av_data.serial;

		}

	}

	GetAVData(mpeg4_field[UNLOCK_MP4][vType], SerialLock, &av_data);


RECORD_QUIT:


	if( pfd )
	{
		fclose(pfd);
	}
	return error_code;
}


int GetAudioInterface( unsigned long *pAddr , int *pSize, unsigned int *pTime, int IsClean)
{

	int error_code = 0;
	static int	SerialBook = -1;
	static int	SerialLock = -1;
	int	ret = 0;
	AV_DATA av_data;

	if( IsClean == 0 )
	{
		while(1)
		{
			if( SerialBook < 0 )
			{
				if( Audio_Serial_Start < 0 )
				{
					do
					{
						GetAVData(AV_OP_GET_ULAW_SERIAL, -1, &av_data );
						fprintf(stderr,"Audio_Serial_Start = %d \n",av_data.serial);
					} while (av_data.serial < 0);

					SerialBook = av_data.serial;
				}else{
					SerialBook = Audio_Serial_Start;
				}
			}


			ret = GetAVData(AV_OP_LOCK_ULAW, SerialBook, &av_data );

			if (ret == RET_SUCCESS)
			{
				//fprintf(stderr, "GetAudioInterface %d \n",SerialBook );

				*pAddr = (unsigned long)av_data.ptr;
				*pSize = (int)av_data.size;
				*pTime = av_data.timestamp;

				if (SerialLock >= 0)
				{
					GetAVData(AV_OP_UNLOCK_ULAW, SerialLock, &av_data);

				}

				SerialLock = SerialBook;

				SerialBook++;

				return 0;

			}
			else {


				GetAVData(AV_OP_GET_ULAW_SERIAL, -1, &av_data );

				if( SerialBook > av_data.serial )
				{
					/* wait new frame */
					//GetAVData(AV_OP_WAIT_NEW_ULAW_SERIAL, -1, &av_data );
					//usleep(5);
					return -1;

				}else{
					SerialBook = av_data.serial;
				}





			}
		}
	}
	else
	{
		if (SerialLock >= 0)
			GetAVData(AV_OP_UNLOCK_ULAW, SerialLock, &av_data);

		SerialBook = -1;
		SerialLock = -1;
	}

	return error_code;
}





int SaveAudioFile( char *pFileName , int	save_cnt)
{
	FILE *pfd = NULL;
	int error_code = 0;
	int	SerialBook = -1;
	int	SerialLock = -1;
	int	ret = 0;
	AV_DATA av_data;

	pfd = fopen(pFileName,"wb");
	if( pfd == NULL )
	{
		ERR("open file error!!");
		error_code = -1;
		goto RECORD_AU_QUIT;
	}


	if( fseek( pfd, 0, SEEK_SET ) < 0)
	{
		ERR("seek file error!!");
		error_code = -1;
		goto RECORD_AU_QUIT;
	}

	do
	{
		GetAVData(AV_OP_GET_ULAW_SERIAL, -1, &av_data );
	} while (av_data.serial < 0);

	SerialBook = av_data.serial;

	while( save_cnt > 0)
	{

		ret = GetAVData(AV_OP_LOCK_ULAW, SerialBook, &av_data );

		if (ret == RET_SUCCESS)
		{



			if( fwrite( av_data.ptr,av_data.size,1, pfd )  <= 0)
			{
				ERR("save file error!!");
				error_code = -1;
				goto RECORD_AU_QUIT;
			}

			if (SerialLock >= 0)
			{
				GetAVData(AV_OP_UNLOCK_ULAW, SerialLock, &av_data);

			}

			SerialLock = SerialBook;

			save_cnt--;
			SerialBook++;

		}
		else {


			GetAVData(AV_OP_GET_ULAW_SERIAL, -1, &av_data );

			if( SerialBook > av_data.serial )
			{
				/* wait new frame */
				GetAVData(AV_OP_WAIT_NEW_ULAW_SERIAL, -1, &av_data );
			}else{
				SerialBook = av_data.serial;
			}



		}

	}

	GetAVData(AV_OP_UNLOCK_ULAW, SerialLock, &av_data);


RECORD_AU_QUIT:


	if( pfd )
	{
		fclose(pfd);
	}
	return error_code;
}

void Init_Interface(int Msg_id)
{
	int qid;
	if(ApproDrvInit(Msg_id))
		exit(1);

	if (func_get_mem(&qid))
	{
		ApproDrvExit();
		exit(1);
	}
}

void Clean_Interface(void)
{
	ApproInterfaceExit();
}

long long GetDiskfreeSpace(const char *pDisk)
{
	long long int freespace = 0;
	struct statfs disk_statfs;

	if( statfs(pDisk, &disk_statfs) >= 0 )
	{
		freespace = (((long long int)disk_statfs.f_bsize  * (long long int)disk_statfs.f_bfree)/(long long int)1024);
	}
	return freespace;
}
long long int RESERVE_SPACE	= 2*1024; //reserve RESERVE_SPACE kbytes for write tail
long long int LIMIT_SPACE	= 1*1024*1024*1024; // Limit  1GB size in each file
int IsLimit_Reach = 0;
int	CheckCardFreeSapce(AVFormatContext *oc)
{
	long long int freespace = GetDiskfreeSpace(pCurr_path);
	long long int curfilesize = 0;
	long long int indexsize = ((audio_frame_count+video_frame_count+1)>>(10-4));
	if( oc != NULL )
	{
		curfilesize = url_ftell(oc->pb);
		if( (curfilesize+indexsize) >= LIMIT_SPACE )
		{
			fprintf(stderr,"1GB limit meet !!!!\n");
			fprintf(stderr,"curfilesize = %d !!!!\n",(int)curfilesize);
			fprintf(stderr,"indexsize = %d !!!!\n",(int)indexsize);
			IsLimit_Reach = 1;
			return -1;
		}

	}

	if( freespace < (RESERVE_SPACE + indexsize) )
	{
		fprintf(stderr,"Space = %ld Kbyte is not enough \n", (long)freespace);
		fprintf(stderr,"audio_frame_count = %d\n",audio_frame_count);
		fprintf(stderr,"video_frame_count = %d\n",video_frame_count);
		return -1;
	}else{
		return 0;
	}
}

void CheckReserveSpace(void)
{
	long long int freespace = GetDiskfreeSpace(pCurr_path);
	fprintf(stderr,"Space on %s = %ld Kbyte \n", pCurr_path,(long)freespace);
#if 1
	RESERVE_SPACE = 30;
#else
	if( freespace < 4*1024 )
	{
		RESERVE_SPACE	= 2*1024;
	}
	else if( freespace < 8*1024 )
	{
		RESERVE_SPACE	= 2*1024;
	}
	else if( freespace < 16*1024 )
	{
		RESERVE_SPACE	= 2*1024;
	}
	else if( freespace < 32*1024 )
	{
		RESERVE_SPACE	= 3*1024;
	}
	else if( freespace < 64*1024 )
	{
		RESERVE_SPACE	= 5*1024;
	}
	else if( freespace < 128*1024 )
	{
		RESERVE_SPACE	= 10*1024;
	}
	else if( freespace < 256*1024 )
	{
		RESERVE_SPACE	= 10*1024;
	}
	else if( freespace < 512*1024 )
	{
		RESERVE_SPACE	= 20*1024;
	}
	else if( freespace < 1024*1024 )
	{
		RESERVE_SPACE	= 40*1024;
	}
	else if( freespace < 2048*1024 )
	{
		RESERVE_SPACE	= 80*1024;
	}
	else if( freespace < 4096*1024 )
	{
		RESERVE_SPACE	= 160*1024;
	}else{
		RESERVE_SPACE	= 160*1024;
	}
#endif
}


static void signalHandler(int signum)
{
	fprintf(stderr,"AVI caught SIGTERM: shutting down\n");
	AbortSignal = 1;
}

static void signalHandler_user1(int signum)
{
	//fprintf(stderr,"AVI caught SIGUSER1: add time\n");
	Pts_Addon_Duration += 10;
}

void init_signals(void)
{
    struct sigaction sigAction;


    sigAction.sa_flags = 0;
    sigemptyset(&sigAction.sa_mask);
	sigaddset(&sigAction.sa_mask, SIGINT);
	sigaddset(&sigAction.sa_mask, SIGUSR1);


	sigAction.sa_handler = signalHandler;
    sigaction(SIGINT, &sigAction, NULL);
	AbortSignal = 0;

	sigAction.sa_handler = signalHandler_user1;
    sigaction(SIGUSR1, &sigAction, NULL);
	Pts_Addon_Duration = 0.5;

}
static int GetfreememInfo( void )
{
	int fd_mem = -1;
	int ret = -1;
	char StrBuffer[200];
	char TmpBuf[50];
	char *pStr = NULL;
	int freeInKb = 0;
	char delima_buf[] = ":\r\n ";

	fd_mem = open("/proc/meminfo", O_RDONLY);
	if( fd_mem < 0 )
	{
		printf("CheckCpuSpeed open file fail \n");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = read(fd_mem,  StrBuffer, sizeof(StrBuffer)-1);
	if( ret <= 0 )
	{
		printf("read device error !!");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = -1;
	StrBuffer[sizeof(StrBuffer)-1] = '\0';
	pStr = strtok(StrBuffer,delima_buf );

	while( pStr != NULL )
	{
		sscanf( pStr,"%s",TmpBuf);

		if( strncmp(TmpBuf, "MemFree", sizeof("MemFree")) == 0 )
		{

			pStr = strtok(NULL, delima_buf);

			//fprintf(stderr, " %s \n", pStr);

			sscanf( pStr,"%d",&freeInKb);
			if(freeInKb <2048){
				fprintf(stderr,"AVI check free Memory = %ld Kbyte is not enough !!!!!!!!!!!!!\n", (long)freeInKb);
				ret = -1;
			}else{

				ret = 0;
			}
			goto CHECK_CPU_END;

		}

		pStr = strtok(NULL, delima_buf);

	}

CHECK_CPU_END:

	if( fd_mem >= 0 )
		close(fd_mem);

	return ret;


}

/**************************************************************/
/* media file output */
extern int EachFileSave;
int main(int argc, char **argv)
{
    const char *filename;
	const char *interval;
	const char *msg_id;
	const char *frame_rate;
	const char *Is_Audio;
	const char *pIs_chkcard;
	const char *pStream_id;
	const char *pBitRate;

    AVOutputFormat *fmt;
    AVFormatContext *oc;
    AVStream *audio_st, *video_st;
    double audio_pts, video_pts;
	int STREAM_DURATION = 0;
	//int STREAM_FRAME_RATE = 30;
	int	nMsg_id = 0;
	int	nIsAudio = 0;
	int	nIsChkcard = 0;
	int	nBitRate = 24000;

    int i;
	int audio_ret = 0;
	int video_ret = 0;

    /* initialize libavcodec, and register all codecs and formats */
    av_register_all();

    if (argc != 9) {
        printf("usage: %s <output_file.avi>  <interval>  <msg_id> <frame_rate>  <Is_audio> <Is_chkcard> <stream_id> <bitRate>\n", argv[0]);
        exit(1);
    }
	EachFileSave = 4*1024*1024/2;
    filename 	= argv[1];
	interval 	= argv[2];
	msg_id 		= argv[3];
	frame_rate	= argv[4];
	Is_Audio	= argv[5];
	pIs_chkcard = argv[6];
	pStream_id	= argv[7];
	pBitRate 	= argv[8];

	STREAM_DURATION 	= atoi(interval);
	STREAM_FRAME_RATE	= atoi(frame_rate)*100;
	nMsg_id				= atoi(msg_id);
	nIsAudio			= atoi(Is_Audio);
	nIsChkcard			= atoi(pIs_chkcard);
	STREAM_FRAME_ID		= atoi(pStream_id);
	nBitRate		= atoi(pBitRate);

	//fprintf(stderr, "FRAMERATE AVI DEBUG MSG: %d\n", STREAM_FRAME_RATE);

	fprintf(stderr, " FileName = %s interval = %d sec  Msg_id = %d \n  Is_audio = %d Is_chkcard = %d stream_id = %d nBitRate = %d\n",
																	filename,
																	STREAM_DURATION,
																	nMsg_id,
																	nIsAudio,
																	nIsChkcard,
																	STREAM_FRAME_ID,
																	nBitRate);
	if( STREAM_FRAME_ID < 0 || STREAM_FRAME_ID >= 5 )
	{
		fprintf(stderr, " stream_id = %d is invalide !!!\n", STREAM_FRAME_ID);
		exit(1);
	}
	if( nIsChkcard == 1 )
	{
		pCurr_path = MMC_PATH;
	}else{
		pCurr_path = TMP_PATH;
	}

	CheckReserveSpace();

	if( CheckCardFreeSapce(NULL) < 0)
		exit(1);

	init_signals();

    /* auto detect the output format from the name. default is
       mpeg. */
    fmt = av_guess_format(NULL,filename, NULL);
    if (!fmt) {
        printf("Could not deduce output format from file extension: using MPEG.\n");
        fmt = av_guess_format("mpeg", NULL, NULL);
    }
    if (!fmt) {
        fprintf(stderr, "Could not find suitable output format\n");
        exit(1);
    }

	Init_Interface(nMsg_id);

    /* allocate the output media context */
    oc = avformat_alloc_context();
    if (!oc) {
        fprintf(stderr, "Memory error\n");
        exit(1);
    }
    oc->oformat = fmt;
    snprintf(oc->filename, sizeof(oc->filename), "%s", filename);

    /* add the audio and video streams using the default format codecs
	and initialize the codecs */
    video_st = NULL;
    audio_st = NULL;
	if( STREAM_FRAME_ID < 2)
	{
		fmt->video_codec = CODEC_ID_MPEG4;
	}
	else if( STREAM_FRAME_ID == 2)
	{
		PRE_BUFFER = 0;
		fmt->video_codec = CODEC_ID_MJPEG;
	}
	else
	{
		fmt->video_codec = CODEC_ID_H264;
		PRE_BUFFER = 30;
		if( STREAM_FRAME_ID == 3 )
		{
			STREAM_FRAME_ID = 0;
		}
		else if( STREAM_FRAME_ID == 4 )
		{
			STREAM_FRAME_ID = 1;
		}
	}

	if( nIsAudio == 1 )
	{
		fmt->audio_codec = CODEC_ID_PCM_MULAW;
	}
	else if( nIsAudio == 2 )
	{
		fmt->audio_codec = CODEC_ID_AAC;
	}else{
		fmt->audio_codec = CODEC_ID_NONE;
	}

    if (fmt->video_codec != CODEC_ID_NONE) {
        video_st = add_video_stream(oc, fmt->video_codec);
    }
    if (fmt->audio_codec != CODEC_ID_NONE) {
        audio_st = add_audio_stream(oc, fmt->audio_codec,nBitRate);
    }

    /* set the output parameters (must be done even if no
       parameters). */
    if (av_set_parameters(oc, NULL) < 0) {
        fprintf(stderr, "Invalid output format parameters\n");
        exit(1);
    }

    dump_format(oc, 0, filename, 1);

    /* now that all the parameters are set, we can open the audio and
	video codecs and allocate the necessary encode buffers */
    if (video_st)
        open_video(oc, video_st);
    if (audio_st)
        open_audio(oc, audio_st);

    /* open the output file, if needed */
    if (!(fmt->flags & AVFMT_NOFILE)) {
        if (url_fopen(&oc->pb, filename, URL_WRONLY) < 0) {
            fprintf(stderr, "Could not open '%s'\n", filename);
            exit(1);
        }
    }

    /* write the stream header, if any */
    av_write_header(oc);

	WaitVideoStart();
    while( AbortSignal == 0 ) {
        /* compute current audio and video time */
        if (audio_st)
            audio_pts = (double)audio_st->pts.val * audio_st->time_base.num / audio_st->time_base.den;
        else
            audio_pts = 0.0;

        if (video_st)
            video_pts = (double)video_st->pts.val * video_st->time_base.num / video_st->time_base.den;
        else
            video_pts = 0.0;

        if ((!audio_st || audio_pts >= (double)STREAM_DURATION+Pts_Addon_Duration) &&
            (!video_st || video_pts >= (double)STREAM_DURATION+Pts_Addon_Duration))
            break;

		//if( nIsChkcard == 1 )
		{
			if( CheckCardFreeSapce(oc) < 0)
				break;

			if( nIsChkcard != 1 )
			{
				if( GetfreememInfo() < 0 )
					break;
			}
		}
		video_ret = -1;
		audio_ret = -1;
        /* write interleaved audio and video frames */
        if (!video_st || (video_st && audio_st && audio_pts < video_pts)) {
			audio_ret = write_audio_frame(oc, audio_st);
        } else {
            video_ret = write_video_frame(oc, video_st);
        }

		if( video_ret < 0 && audio_ret < 0 )
		{
			usleep(10000);
		}
    }

	GetAudioInterface(NULL,NULL,NULL,1);
	GetVideoInterface(NULL,NULL,NULL,NULL,1);

    /* write the trailer, if any */
    av_write_trailer(oc);

    /* close each codec */
    if (video_st)
        close_video(oc, video_st);
    if (audio_st)
        close_audio(oc, audio_st);


    /* free the streams */
    for(i = 0; i < oc->nb_streams; i++) {
        av_freep(&oc->streams[i]->codec);
        av_freep(&oc->streams[i]);
    }

    if (!(fmt->flags & AVFMT_NOFILE)) {
        /* close the output file */
        url_fclose(oc->pb);
    }

    /* free the stream */
    av_free(oc);

	Clean_Interface();

	if( IsLimit_Reach )
	{
		return 99;
	}

    return 0;
}
