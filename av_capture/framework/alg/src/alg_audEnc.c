#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <alg_priv.h>
#include <ti/xdais/dm/ispeech1_pcm.h>
#include <ti/xdais/dm/isphenc1.h>
#include <ti/xdais/dm/iaudenc1.h>

#include <alg_audEncDec.h>

#ifdef ALG_AUD_INP_FILE_DUMP_DEBUG
FILE *hndlFile_inpaudio;
#endif
#ifdef ALG_AUD_ENC_FILE_DUMP_DEBUG
FILE *hndlFile_audio;
#endif

typedef struct
{
	SPHENC1_Handle sph_hEncode;
	SPHENC1_Params sph_params;
	SPHENC1_DynamicParams sph_dynParams;
    SPHENC1_InArgs 	sph_inArgs;
    SPHENC1_OutArgs	sph_outArgs;
	SPHENC1_Status 	sph_encStatus;
} SPHENC1_XDM_Params;

typedef struct
{
	AUDENC1_Handle aud_hEncode;
	AUDENC1_Params aud_params;
	AUDENC1_DynamicParams aud_dynParams;
	AUDENC1_InArgs 	aud_inArgs;
	AUDENC1_OutArgs aud_outArgs;
	AUDENC1_Status 	aud_encStatus;
} AUDENC1_XDM_Params;

typedef struct
{
	char algName[16];

	union {
		SPHENC1_XDM_Params sphParams;
		AUDENC1_XDM_Params audParams;
	};

} ALG_AudEncObj;

void *ALG_audEncCreate(ALG_AudEncPrm *prm)
{
	ALG_AudEncObj 	*pObj;

	if(gALG_hEngine==NULL)
		return NULL;

	pObj = OSA_memAlloc(sizeof(ALG_AudEncObj));
	if(pObj==NULL)
		return NULL;

	memset(pObj, 0, sizeof(ALG_AudEncObj));

	switch(prm->codec)
	{
		default:
		case ALG_AUD_CODEC_G711:
			pObj->sphParams.sph_params.size 			= sizeof(SPHENC1_Params);
			pObj->sphParams.sph_params.frameSize 		= prm->numSamples;
			pObj->sphParams.sph_params.compandingLaw	= (prm->encMode==PCM_ULAW)?ISPEECH1_PCM_COMPAND_ULAW:ISPEECH1_PCM_COMPAND_ALAW;
			pObj->sphParams.sph_params.packingType		= XDAS_FALSE;
			pObj->sphParams.sph_params.vadSelection		= XDAS_FALSE;
			pObj->sphParams.sph_params.codecSelection	= XDAS_FALSE;
			pObj->sphParams.sph_params.bitRate			= XDAS_FALSE;
			pObj->sphParams.sph_params.reserved			= XDAS_FALSE;
			pObj->sphParams.sph_params.tablesPtr		= NULL;

     		strcpy(pObj->algName, ALG_AUD_CODEC_G711_ENC_NAME);

    		/* Create speech encoder */
    		pObj->sphParams.sph_hEncode = SPHENC1_create(gALG_hEngine, pObj->algName, &pObj->sphParams.sph_params);
			if (pObj->sphParams.sph_hEncode == NULL) {
				OSA_ERROR("Failed to open speech encode algorithm: %s (0x%x)\n", pObj->algName, Engine_getLastError(gALG_hEngine));
			    OSA_memFree(pObj);
			    return NULL;
			}

#ifdef ALG_AUD_ENC_FILE_DUMP_DEBUG
			hndlFile_audio = fopen("audioOut.g711", "wb");
#endif
			break;

		case ALG_AUD_CODEC_AAC:
			pObj->audParams.aud_params.size 				= sizeof(AUDENC1_Params);
			pObj->audParams.aud_params.sampleRate 			= prm->sampRate;
			pObj->audParams.aud_params.bitRate				= prm->bitRate;
			pObj->audParams.aud_params.channelMode			= IAUDIO_1_0;
			pObj->audParams.aud_params.dataEndianness		= XDM_LE_16;
			pObj->audParams.aud_params.encMode				= IAUDIO_CBR;
			pObj->audParams.aud_params.inputFormat			= IAUDIO_INTERLEAVED;
			pObj->audParams.aud_params.inputBitsPerSample	= 16;
			pObj->audParams.aud_params.maxBitRate			= 256000;
			pObj->audParams.aud_params.dualMonoMode			= IAUDIO_DUALMONO_LR;
			pObj->audParams.aud_params.crcFlag				= XDAS_FALSE;
			pObj->audParams.aud_params.ancFlag				= XDAS_FALSE;
			pObj->audParams.aud_params.lfeFlag				= XDAS_FALSE;

     		strcpy(pObj->algName, ALG_AUD_CODEC_AAC_ENC_NAME);

    		/* Create audio encoder */
			pObj->audParams.aud_hEncode = AUDENC1_create(gALG_hEngine, pObj->algName, &pObj->audParams.aud_params);
			if (pObj->audParams.aud_hEncode == NULL) {
				OSA_ERROR("Failed to open audio encode algorithm: %s (0x%x)\n", pObj->algName, Engine_getLastError(gALG_hEngine));
			    OSA_memFree(pObj);
			    return NULL;
			}

#ifdef ALG_AUD_ENC_FILE_DUMP_DEBUG
			hndlFile_audio = fopen("audioOut.aac", "wb");
#endif
			break;
	}

#ifdef ALG_AUD_INP_FILE_DUMP_DEBUG
	hndlFile_inpaudio = fopen("audioInp.pcm", "wb");
#endif

	return pObj;
}

int ALG_audDynparams(void *hndl, ALG_AudEncPrm *prm)
{
	ALG_AudEncObj 	*pObj = (ALG_AudEncObj*) hndl;
	AUDENC1_Status  encStatus;
    XDAS_Int32      status;

	switch(prm->codec)
	{
		default:
		case ALG_AUD_CODEC_G711:
			return OSA_SOK;
		break;

		case ALG_AUD_CODEC_AAC:

			pObj->audParams.aud_dynParams.size 					= sizeof(AUDENC1_DynamicParams);
			pObj->audParams.aud_dynParams.sampleRate 			= prm->sampRate;
			pObj->audParams.aud_dynParams.bitRate				= prm->bitRate;
			pObj->audParams.aud_dynParams.channelMode			= IAUDIO_1_0;
			pObj->audParams.aud_dynParams.lfeFlag				= XDAS_FALSE;
			pObj->audParams.aud_dynParams.dualMonoMode			= IAUDIO_DUALMONO_LR;
			pObj->audParams.aud_dynParams.inputBitsPerSample	= 16;

			encStatus.size 		= sizeof(AUDENC1_Status);
			encStatus.data.buf 	= NULL;

			/* Set dynamic parameters */
			status = AUDENC1_control(pObj->audParams.aud_hEncode, XDM_SETPARAMS, &pObj->audParams.aud_dynParams, &encStatus);
			if (status != AUDENC1_EOK) {
				OSA_ERROR("Failed XDM_SETPARAMS audio encode algorithm: %s (0x%x)\n", pObj->algName, Engine_getLastError(gALG_hEngine));
				return OSA_EFAIL;
			}

#ifdef ALG_AUD_ENC_DEBUG
			/* Get buffer requirements */
			status = AUDENC1_control(pObj->audParams.aud_hEncode, XDM_GETBUFINFO, &pObj->audParams.aud_dynParams, &encStatus);
			if (status != AUDENC1_EOK) {
				OSA_ERROR("Failed XDM_GETBUFINFO audio encode algorithm: %s (0x%x)\n", pObj->algName, Engine_getLastError(gALG_hEngine));
				return OSA_EFAIL;
			}
#endif
		break;
	}

	return OSA_SOK;
}

int ALG_audEncRun(void *hndl, ALG_AudEncPrm *encPrm, ALG_AudioRunPrm *runPrm, ALG_AudioRunStatus *runStatus)
{
	ALG_AudEncObj *pObj	= (ALG_AudEncObj*) hndl;
    XDAS_Int32 status, bytesGenerated;

	switch(encPrm->codec)
	{
		default:
		case ALG_AUD_CODEC_G711:
		{
			XDM1_SingleBufDesc  inBufDesc;
			XDM1_SingleBufDesc  outBufDesc;

			inBufDesc.buf           = (XDAS_Int8*)runPrm->inAddr;
			inBufDesc.bufSize       = runPrm->inDataSize;

			outBufDesc.buf          = (XDAS_Int8*)runPrm->outAddr;
			outBufDesc.bufSize      = runPrm->outBufMaxSize;

			pObj->sphParams.sph_inArgs.size   = sizeof(SPHENC1_InArgs);
			pObj->sphParams.sph_outArgs.size  = sizeof(SPHENC1_OutArgs);

			/* We never use the inArgs.data fields, so initialize them to NULL. */
			pObj->sphParams.sph_inArgs.data.buf 		= (XDAS_Int8*) NULL;
			pObj->sphParams.sph_inArgs.data.bufSize 	= 0L;

			/* Encode the speech buffer */
#ifdef TIINTERNAL_G711
			bytesGenerated = ALG_TI_ulawEncode((short*)runPrm->outAddr,(short*)runPrm->inAddr, runPrm->inDataSize);
#else
			status = SPHENC1_process(pObj->sphParams.sph_hEncode, &inBufDesc, &outBufDesc,
											&pObj->sphParams.sph_inArgs, &pObj->sphParams.sph_outArgs);
			if (status != SPHENC1_EOK) {
				OSA_ERROR("SPHENC1_process() failed with error (%d ext: 0x%x)\n",
						  (Int)status, (Uns)pObj->sphParams.sph_outArgs.extendedError);
				bytesGenerated = 0;
			}
			else
				bytesGenerated = (runPrm->inDataSize>>1);
#endif
		}
		break;

		case ALG_AUD_CODEC_AAC:
		{
			XDM1_BufDesc      	inBufDesc;
			XDM1_BufDesc 		outBufDesc;

			inBufDesc.numBufs       	= 1;
			inBufDesc.descs[0].buf      = (XDAS_Int8*)runPrm->inAddr;
			inBufDesc.descs[0].bufSize  = runPrm->inDataSize;

			outBufDesc.numBufs      	= 1;
			outBufDesc.descs[0].buf     = (XDAS_Int8*)runPrm->outAddr;
			outBufDesc.descs[0].bufSize = runPrm->outBufMaxSize;

			pObj->audParams.aud_inArgs.size             = sizeof(AUDENC1_InArgs);
			pObj->audParams.aud_inArgs.numInSamples	  	= encPrm->numSamples;
			pObj->audParams.aud_inArgs.ancData.bufSize  = 0L;
			pObj->audParams.aud_inArgs.ancData.buf      = (XDAS_Int8 *) NULL;

			pObj->audParams.aud_outArgs.size            = sizeof(AUDENC1_OutArgs);

			/* Encode the audio buffer */
			status = AUDENC1_process(pObj->audParams.aud_hEncode, &inBufDesc, &outBufDesc,
										&pObj->audParams.aud_inArgs, &pObj->audParams.aud_outArgs);
			if (status != AUDENC1_EOK) {
				OSA_ERROR("AUDENC1_process() failed with error (%d ext: 0x%x)\n",
						  (Int)status, (Uns)pObj->audParams.aud_outArgs.extendedError);
				bytesGenerated = 0;
			}
			else
				bytesGenerated = pObj->audParams.aud_outArgs.bytesGenerated;
		}
		break;
	}

	runStatus->inDataUsedSize 	= (encPrm->numSamples*2);
	runStatus->outDataSize 		= bytesGenerated;

#ifdef ALG_AUD_ENC_DEBUG
	OSA_printf("AUDIO ENCODER STATUS MESSAGE: In data used (%d) Bytes Generated(%d) \n",
						runStatus->inDataUsedSize, runStatus->outDataSize);
#endif

#ifdef ALG_AUD_INP_FILE_DUMP_DEBUG
	fwrite(runPrm->inAddr, 1, runPrm->inDataSize, hndlFile_inpaudio);
#endif
#ifdef ALG_AUD_ENC_FILE_DUMP_DEBUG
	fwrite(runPrm->outAddr, 1, bytesGenerated, hndlFile_audio);
#endif

	return bytesGenerated;
}

int ALG_audEncDelete(void *hndl, ALG_AudEncPrm *prm)
{
	ALG_AudEncObj *pObj=(ALG_AudEncObj*) hndl;
  	if(pObj==NULL)
    	return OSA_EFAIL;

	switch(prm->codec)
	{
		default:
		case ALG_AUD_CODEC_G711:
			SPHENC1_delete(pObj->sphParams.sph_hEncode);
		break;

		case ALG_AUD_CODEC_AAC:
			AUDENC1_delete(pObj->audParams.aud_hEncode);
		break;
	}

	OSA_memFree(pObj);

#ifdef ALG_AUD_INP_FILE_DUMP_DEBUG
	fclose(hndlFile_inpaudio);
#endif
#ifdef ALG_AUD_ENC_FILE_DUMP_DEBUG
	fclose(hndlFile_audio);
#endif
	return OSA_SOK;
}

#ifdef TIINTERNAL_G711
int ALG_TI_ulawEncode(short *dst, short *src, short bufsize)
{

    unsigned short i;
    short data;
    unsigned short isNegative;
    short nOut;
    short lowByte = 1;
    int outputSize = bufsize / 2;

    for (i = 0; i < outputSize; i++)
    {
        data = *(src + i);
        data >>= 2;
        isNegative = (data < 0 ? 1 : 0);
        if (isNegative)
            data = -data;

        if (data <= 1) {
            nOut = (char) data;
        } else if (data <= 31) {
            nOut = ((data - 1) >> 1) + 1;
        } else if (data <= 95) {
            nOut = ((data - 31) >> 2) + 16;
        } else if (data <= 223) {
            nOut = ((data - 95) >> 3) + 32;
        } else if (data <= 479) {
            nOut = ((data - 223) >> 4) + 48;
        } else if (data <= 991) {
            nOut = ((data - 479) >> 5) + 64;
        } else if (data <= 2015) {
            nOut = ((data - 991) >> 6) + 80;
        } else if (data <= 4063) {
            nOut = ((data - 2015) >> 7) + 96;
        } else if (data <= 7903) {
            nOut = ((data - 4063) >> 8) + 112;
        } else {
            nOut = 127;
        }

        if (isNegative) {
            nOut = 127 - nOut;
        } else {
            nOut = 255 - nOut;
        }

        // Pack the bytes in a word
        if (lowByte)
            *(dst + (i >> 1)) = (nOut & 0x00FF);
        else
            *(dst + (i >> 1)) |= ((nOut << 8) & 0xFF00);

        lowByte ^= 0x1;
    }

	return (outputSize);
}
#endif

int ALG_audAlarmCheck(Uint8 *inpBuf, int numSamples, int alarmLevel)
{
    unsigned short i;
	int audAlarm = FALSE;
	int sampLimit = (alarmLevel*32767)/100;
	short *src = (short *)inpBuf;
    short data, alarmcount=0;
    unsigned short isNegative;

	//sampLimit = (sampLimit*50)/100;

    for (i = 0; i < numSamples; i++)
    {
        data = src[i];
        isNegative = (data < 0) ? 1 : 0;
        if (isNegative)
            data = -data;

		if(data>=sampLimit)
			alarmcount++;
	}

#ifdef ALG_AUD_ENC_DEBUG
	OSA_printf("AUDIO ALARM DEBUG MESSAGE: samples(%d) level(%d) samplimit(%d) count(%d)\n",
					numSamples, alarmLevel, sampLimit, alarmcount);
#endif

	if(alarmcount>=AUDIOALARM_THRESHOLD)
		audAlarm = TRUE;

	return audAlarm;
}
