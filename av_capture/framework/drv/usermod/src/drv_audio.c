#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/soundcard.h>

#include <drv_audio.h>

//#define DRV_AUDIO_DEBUG

typedef struct {
	snd_pcm_t *sound_handle;
} DRV_AudioObj;

int DRV_audioOpen(DRV_AudioHndl *hndl, DRV_AudioConfig *config)
{

	int rc = 0;
	unsigned int val = 0;
	int dir = 0;
	unsigned int buffer_time = 0;
	snd_pcm_uframes_t frames;
	snd_pcm_uframes_t buffer_frames;
	snd_pcm_hw_params_t *sound_params = NULL;
	DRV_AudioObj *pObj;

	hndl->hndl = OSA_memAlloc(sizeof(DRV_AudioObj));
	if( hndl->hndl == NULL )
	{
		OSA_ERROR("OSA_memAlloc fail \n");
		goto error_exit;
	}

	pObj = (DRV_AudioObj*)hndl->hndl;
	memset(pObj,0,sizeof(DRV_AudioObj));

	/* Open PCM device for record. */
	rc = snd_pcm_open(&(pObj->sound_handle), "default", SND_PCM_STREAM_CAPTURE, 0);
	if (rc < 0)
	{
		OSA_ERROR("unable to open pcm device: %s\n", snd_strerror(rc));
		goto error_exit;
	}

	/* Allocate a hardware parameters object. */
	snd_pcm_hw_params_alloca(&sound_params);

	/* Fill it in with default values. */
	snd_pcm_hw_params_any(pObj->sound_handle, sound_params);

	/* Set the desired hardware parameters. */

	/* Sampling rate*/
	val = config->samplingRate;
	snd_pcm_hw_params_set_rate_near(pObj->sound_handle, sound_params, &val, &dir);
	if (val != config->samplingRate)
	{
		OSA_ERROR("Rate doesn't match (requested %iHz, get %iHz)\n", config->samplingRate, val);
		goto error_exit;
	}

	/* Signed 16-bit little-endian format */
	snd_pcm_hw_params_set_format(pObj->sound_handle, sound_params, SND_PCM_FORMAT_S16_LE);
	/* Interleaved mode */
	snd_pcm_hw_params_set_access(pObj->sound_handle, sound_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	/* Num channels */
	snd_pcm_hw_params_set_channels(pObj->sound_handle, sound_params, config->numChannels);

	/* Set period size of frames. */
	frames = config->buff_Of_Samples;
	snd_pcm_hw_params_set_period_size_near(pObj->sound_handle, sound_params, &frames, &dir);

	/* set the buffer time */
 	if( buffer_time )
 	{
 		rc = snd_pcm_hw_params_set_buffer_time_near(pObj->sound_handle, sound_params, &buffer_time, &dir);
 		if (rc < 0) {
 		    OSA_ERROR("Unable to set buffer time %i for playback: %s\n", buffer_time, snd_strerror(rc));
 		    goto error_exit;
 		}
 	}
 	else{
		buffer_frames =  frames*4;
		snd_pcm_hw_params_set_buffer_size_near(pObj->sound_handle, sound_params, &buffer_frames);
 	}

	/* Write the parameters to the driver */
	rc = snd_pcm_hw_params(pObj->sound_handle, sound_params);
	if (rc < 0)
	{

		OSA_ERROR("unable to set hw parameters: %s\n",
			snd_strerror(rc));
		goto error_exit;
	}

	/* Use a buffer large enough to hold one period */
	snd_pcm_hw_params_get_period_size(sound_params, &frames, &dir);

	snd_pcm_hw_params_get_period_time(sound_params, &val, &dir);

#ifdef DRV_AUDIO_DEBUG
	OSA_printf("AUDIO : period size = %d frames, dir = %d\n", (int)frames, dir);
	OSA_printf("AUDIO : period time = %d us, dir = %d\n", val, dir);
#endif

	return OSA_SOK;

error_exit:

	if( hndl->hndl )
	{
		OSA_memFree(hndl->hndl);
	}

	return OSA_EFAIL;
}

int DRV_audioRead(DRV_AudioHndl *hndl,void *pInBuf, int readsamples)
{
	int rc;
	DRV_AudioObj *pObj;
	if(hndl==NULL)
	  return OSA_EFAIL;

	if(hndl->hndl==NULL)
	  return OSA_EFAIL;

	pObj = (DRV_AudioObj *)hndl->hndl;

	/* Read stereo data from codec */
	rc = snd_pcm_readi(pObj->sound_handle, pInBuf, readsamples);
	if (rc == -EPIPE)
	{
		/* EPIPE means overrun */
		OSA_ERROR("audio overrun occurred\n");
		snd_pcm_prepare(pObj->sound_handle);
		return 0;
	}
	else {
		if( rc < 0 )
		{
			OSA_ERROR("audio error from read : %s\n", snd_strerror(rc));
		}

		return rc;
	}
}

int DRV_audioClose(DRV_AudioHndl *hndl)
{
	DRV_AudioObj *pObj;

  if(hndl==NULL)
    return OSA_EFAIL;

  if(hndl->hndl==NULL)
    return OSA_EFAIL;

  pObj = (DRV_AudioObj *)hndl->hndl;

  snd_pcm_drain(pObj->sound_handle);
  snd_pcm_close(pObj->sound_handle);

  OSA_memFree(pObj);

  return OSA_SOK;
}

int DRV_audioSetVolume(DRV_AudioHndl *hndl, Uint32 volumeLevel)
{
	return 0;
}

int DRV_audioplayOpen(DRV_AudioHndl *hndl, DRV_AudioConfig *config)
{
	int rc = 0;
	unsigned int val = 0;
	int dir = 0;
	snd_pcm_uframes_t frames;
	snd_pcm_uframes_t buffer_frames;
	DRV_AudioObj *pObj;
	snd_pcm_hw_params_t *sound_params = NULL;
	int status = -1;

	hndl->hndl = OSA_memAlloc(sizeof(DRV_AudioObj));
	if( hndl->hndl == NULL )
	{
		OSA_ERROR("OSA_memAlloc fail \n");
		goto error_exit;
	}

	pObj = (DRV_AudioObj*)hndl->hndl;
	memset(pObj,0,sizeof(DRV_AudioObj));

	/* Open PCM device for playback. */
	rc = snd_pcm_open(&(pObj->sound_handle), "default", SND_PCM_STREAM_PLAYBACK, 0);
	if (rc < 0)
	{
		OSA_ERROR("unable to open pcm device: %s\n", snd_strerror(rc));
		goto error_exit;
	}

	/* Allocate a hardware parameters object. */
	snd_pcm_hw_params_alloca(&sound_params);

	/* Fill it in with default values. */
	status = snd_pcm_hw_params_any(pObj->sound_handle, sound_params);

    if (status < 0) {
        OSA_ERROR("Failed snd_pcm_hw_params_any on AUDIO_DEVICE (%s)\n", snd_strerror(status));
        goto error_exit;
    }

	/* Set the desired hardware parameters. */

	/* Interleaved mode */
	status = snd_pcm_hw_params_set_access(pObj->sound_handle, sound_params, SND_PCM_ACCESS_RW_INTERLEAVED );
    if (status < 0) {
        OSA_ERROR("Failed snd_pcm_hw_params_set_access on AUDIO_DEVICE (%s)\n", snd_strerror(status));
        goto error_exit;
    }

	/* Signed 16-bit little-endian format */
	status = snd_pcm_hw_params_set_format(pObj->sound_handle, sound_params, SND_PCM_FORMAT_U8 );
    if (status < 0) {
        OSA_ERROR("Failed snd_pcm_hw_params_set_format on AUDIO_DEVICE (%s)\n", snd_strerror(status));
        goto error_exit;
    }

	/* channels */
	status = snd_pcm_hw_params_set_channels(pObj->sound_handle, sound_params, config->numChannels);
    if (status < 0) {
        OSA_ERROR("Failed snd_pcm_hw_params_set_channels on AUDIO_DEVICE (%s)\n", snd_strerror(status));
        goto error_exit;
    }

	/* Sampling rate*/
	val = config->samplingRate;
	status = snd_pcm_hw_params_set_rate_near(pObj->sound_handle, sound_params, &val, &dir);
    if (status < 0) {
        OSA_ERROR("Failed snd_pcm_hw_params_set_rate_near on AUDIO_DEVICE (%s)\n", snd_strerror(status));
        goto error_exit;
    }

	if (val != config->samplingRate)
	{
		OSA_ERROR("Rate doesn't match (requested %iHz, get %iHz)\n", config->samplingRate, val);
		goto error_exit;
	}

	/* Set period size of frames. */
	frames = config->buff_Of_Samples;
	status = snd_pcm_hw_params_set_period_size_near(pObj->sound_handle, sound_params, &frames, &dir);
    if (status < 0) {
        OSA_ERROR("Failed snd_pcm_hw_params_set_period_size_near on AUDIO_DEVICE (%s)\n", snd_strerror(status));
        goto error_exit;
    }

	/* Set buffer size of frames. */
	buffer_frames = config->bufferSize;
	status = snd_pcm_hw_params_set_buffer_size_near(pObj->sound_handle, sound_params, &buffer_frames);
	if (status < 0) {
		OSA_ERROR("Failed snd_pcm_hw_params_set_buffer_size_near on AUDIO_DEVICE (%s)\n", snd_strerror(status));
		goto error_exit;
	}

	/* Write the parameters to the driver */
	rc = snd_pcm_hw_params(pObj->sound_handle, sound_params);
	if (rc < 0)
	{
		OSA_ERROR("unable to set hw parameters: %s\n",snd_strerror(rc));
		goto error_exit;
	}

#if 0
	/* Use a buffer large enough to hold one period */
	snd_pcm_hw_params_get_period_size(sound_params, &frames, &dir);
	//OSA_printf("AUDIOPLAY : period size = %d frames\n", (int)frames);

	snd_pcm_hw_params_get_period_time(sound_params, &val, &dir);
	//OSA_printf("AUDIOPLAY : period time = %d us\n", val);
#endif

	snd_pcm_hw_params_get_channels(sound_params, &val);
	snd_pcm_hw_params_get_rate(sound_params, &val, &dir);
	snd_pcm_hw_params_get_period_time(sound_params,&val, &dir);
	snd_pcm_hw_params_get_period_size(sound_params,&frames, &dir);
	snd_pcm_hw_params_get_buffer_time(sound_params,&val, &dir);
	snd_pcm_hw_params_get_buffer_size(sound_params,(snd_pcm_uframes_t *) &val);
	snd_pcm_hw_params_get_periods(sound_params, &val, &dir);

#ifdef DRV_AUDIO_DEBUG
	OSA_printf("channels = %d\n", val);
	OSA_printf("rate = %d bps\n", val);
	OSA_printf("period time = %d us\n", val);
	OSA_printf("period size = %d frames\n", (int)frames);
	OSA_printf("buffer time = %d us\n", val);
	OSA_printf("buffer size = %d frames\n", val);
	OSA_printf("periods per buffer = %d frames\n", val);
#endif

	return OSA_SOK;

error_exit:

	if( hndl->hndl )
	{
		OSA_memFree(hndl->hndl);
	}

	return OSA_EFAIL;
}

int DRV_audioWrite(DRV_AudioHndl *hndl, void *pInBuf, int writeSamples)
{
	int rc;
	DRV_AudioObj *pObj;

	if(hndl==NULL)
	  return OSA_EFAIL;

	if(hndl->hndl==NULL)
	  return OSA_EFAIL;

	pObj = (DRV_AudioObj *)hndl->hndl;

	while (writeSamples > 0)
	{
		rc = snd_pcm_writei(pObj->sound_handle, pInBuf, writeSamples);

		if (rc == -EPIPE)
		{
			/* EPIPE means overrun */
			OSA_printf("WARNING: AUDIOWRITE: Underrun occurred. Preparing pcm..\n");
			if (snd_pcm_prepare(pObj->sound_handle) < 0) {
				OSA_ERROR("Failed to recover from underrun\n");
			}

			//return 0;
		}
		else{
			if( rc < 0 )
			{
				OSA_ERROR("error from write : %s\n",snd_strerror(rc));
				if (snd_pcm_prepare(pObj->sound_handle) < 0) {
					OSA_ERROR("Failed to recover from snd_strerror\n");
				}
			}

			return rc;
		}
		if (rc == -EPIPE) {
			//memset(pInBuf, 0x7F, writeSamples*2*2);
			//printf("rc != writeSamples\n");
		}
		else {
			writeSamples -= rc;
		}
	}
}


