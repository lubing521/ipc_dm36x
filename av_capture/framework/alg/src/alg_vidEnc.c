
#include <alg_priv.h>
#include <alg_vidEnc.h>
#include <alg_jpgEnc.h>
#include <osa_cmem.h>
#include <osa_file.h>
#include <ti/sdo/codecs/h264enc/ih264venc.h>
#include <ti/sdo/codecs/mpeg4enc/imp4venc.h>
#include <alg_vidDec.h>

typedef struct
{
	VIDENC1_Handle   hEncode;
	void *hJpgEncode;

	ALG_VidEncCreate createPrm;
	char algName[16];

	union{
		VIDENC1_Params    	params;
		IH264VENC_Params	params_h264;
		IMP4VENC_Params		params_mpeg4;
	};
	union{
		VIDENC1_DynamicParams	dynamicParams;
		IH264VENC_DynamicParams dynamicParams_h264;
		IMP4VENC_DynamicParams  dynamicParams_mpeg4;
	};

	VIDENC1_Status	encStatus;

	XDAS_UInt8 *MV_Addr;
	XDAS_UInt32 MV_Size;

} ALG_VidEncObj;

XDAS_Int32 ALG_vidEncSetDynamicParams(ALG_VidEncObj *pObj);

void *ALG_vidEncCreate(ALG_VidEncCreate *create)
{
  XDAS_Int32              status;
  ALG_VidEncObj           *pObj;

  if(gALG_hEngine==NULL)
    return NULL;

  pObj = OSA_memAlloc(sizeof(ALG_VidEncObj));
  if(pObj==NULL)
    return NULL;

  memset(pObj, 0, sizeof(ALG_VidEncObj));

  switch(create->codec) {
    case ALG_VID_CODEC_H264:
      strcpy(pObj->algName, ALG_VID_CODEC_H264_ENC_NAME);
      break;
    case ALG_VID_CODEC_MPEG4:
      strcpy(pObj->algName, ALG_VID_CODEC_MPEG4_ENC_NAME);
      break;
    case ALG_VID_CODEC_MJPEG:
      strcpy(pObj->algName, ALG_VID_CODEC_JPEG_ENC_NAME);
      break;
    default:
      OSA_memFree(pObj);
      return NULL;
  }

  memcpy(&pObj->createPrm, create, sizeof(pObj->createPrm));

  if(create->codec==ALG_VID_CODEC_MJPEG)
  {
    ALG_JpgEncCreate jpgCreate;

    jpgCreate.dataFormat = create->dataFormat;
    jpgCreate.width      = create->width;
    jpgCreate.height     = create->height;
    jpgCreate.offsetH    = create->offsetH;
    jpgCreate.offsetV    = create->offsetV;
    jpgCreate.encDataFormat = ALG_VID_DATA_FORMAT_YUV420;
    jpgCreate.qValue     = create->qValue;

    pObj->hJpgEncode = ALG_jpgEncCreate(&jpgCreate);

    if(pObj->hJpgEncode==NULL) {
      OSA_memFree(pObj);
      return NULL;
    }

    return pObj;
  }

  pObj->params.maxHeight             = create->height;
  pObj->params.maxWidth              = create->width;
  pObj->params.maxFrameRate          = ALD_VID_ENC_MAX_FRAMERATE;
  pObj->params.maxBitRate            = ALD_VID_ENC_MAX_BITRATE;
  pObj->params.inputChromaFormat     = XDM_YUV_420SP;
  pObj->params.dataEndianness        = XDM_BYTE;
  pObj->params.maxInterFrameInterval = 1;
  pObj->params.inputContentType      = IVIDEO_PROGRESSIVE;
  pObj->params.reconChromaFormat     = XDM_YUV_420SP;

  if(create->codec==ALG_VID_CODEC_MPEG4)
  {
 		pObj->params.size       					= sizeof(IMP4VENC_Params);
      	pObj->params.encodingPreset					= XDM_USER_DEFINED;
		pObj->params.maxHeight  					= create->height;
  		pObj->params.maxWidth   					= (create->offsetH>create->width)?create->offsetH:create->width;
  		pObj->params_mpeg4.subWindowHeight 			= create->height;
  		pObj->params_mpeg4.subWindowWidth 			= create->width;
  		pObj->params_mpeg4.rotation					= 0;
  		pObj->params_mpeg4.vbvSize 					= 0;
  		pObj->params_mpeg4.svhMode 					= 0;
  		pObj->params_mpeg4.IFrameBitRateBiasFactor 	= 256;
  		pObj->params_mpeg4.PFrameBitRateBiasFactor 	= 256;
  		pObj->params_mpeg4.peakBufWindow 			= 2;
  		pObj->params_mpeg4.minBitRate				= 64000;	//set to 64kbps
		switch(create->rateControlType) {
			case ALG_VID_ENC_RATE_CONTROL_RCOFF:
			  pObj->params.rateControlPreset     = IVIDEO_NONE;
			  break;
			default:
			case ALG_VID_ENC_RATE_CONTROL_VBR:
			  pObj->params.rateControlPreset     = IVIDEO_STORAGE;
			  break;
			case ALG_VID_ENC_RATE_CONTROL_CBR:
			  pObj->params.rateControlPreset     = IVIDEO_LOW_DELAY;
			  break;
		}
  }
  else if(create->codec==ALG_VID_CODEC_H264)
  {
	  if(77 == create->profileIdc)
	  {
		  pObj->params_h264.profileIdc 					= 77;
		  pObj->params_h264.transform8x8FlagIntraFrame 	= 0;
		  pObj->params_h264.transform8x8FlagInterFrame 	= 0;
		  pObj->params_h264.seqScalingFlag 				= 0;
		  pObj->params_h264.entropyMode 				= 1;
		  printf("[alg_vidEnc]zeng Main profile --------------create->profileIdc=%d\n" , create->profileIdc);
		  
	  }
	  else if(66 == create->profileIdc)
	  {
		  pObj->params_h264.profileIdc 					= 66;
		  pObj->params_h264.transform8x8FlagIntraFrame 	= 0;
		  pObj->params_h264.transform8x8FlagInterFrame 	= 0;
		  pObj->params_h264.seqScalingFlag 				= 0;
		  pObj->params_h264.entropyMode 				= 0;
		  printf("[alg_vidEnc]zeng baseline profile--------------create->profileIdc=%d\n" , create->profileIdc);

	  }
	  else
	  {

		  pObj->params_h264.profileIdc 					= 100;
		  pObj->params_h264.transform8x8FlagIntraFrame 	= 0;
		  pObj->params_h264.transform8x8FlagInterFrame 	= (create->width>2048)?0:1;
		  pObj->params_h264.seqScalingFlag 				= 1;
		  pObj->params_h264.entropyMode 				= 1;	 	 	  	
	 	  printf("[alg_vidEnc]zeng hight profile--------------create->profileIdc=%d\n" , create->profileIdc);
		  
	  }

	  pObj->params.size      						= sizeof(IH264VENC_Params);
	  pObj->params.rateControlPreset				= IVIDEO_USER_DEFINED;
	  pObj->params_h264.levelIdc 					= IH264VENC_LEVEL_40;
	  pObj->params_h264.Log2MaxFrameNumMinus4		= 0;
	  pObj->params_h264.ConstraintSetFlag			= 0;
	  pObj->params_h264.enableVUIparams 			= 7;
	  pObj->params_h264.meAlgo 						= 0;
	  pObj->params_h264.enableARM926Tcm 			= 0;
	  pObj->params_h264.enableDDRbuff 				= (create->ddrEnable) ? 1 : 0;
	  pObj->params_h264.sliceMode 					= 3;
	  pObj->params_h264.EnableLongTermFrame			= 0;
	  pObj->params_h264.outputDataMode 				= IH264VENC_TI_ENTIREFRAME;
	  pObj->params_h264.sliceFormat					= IH264VENC_TI_BYTESTREAM; 
	  printf("[alg_vidEnc]zeng afdfafdasfdasf--------create->profileIdc=%d\n" , create->profileIdc);
	
	  if((create->encodePreset&ALG_VID_ENC_PRESET_MASK) == ALG_VID_ENC_PRESET_AUTO)
	  {
		  if((create->width<320)||(create->height<128))
		  {
			pObj->params.encodingPreset		= XDM_USER_DEFINED;
			pObj->params_h264.encQuality 	= 0;
		  }
		  else {
			if(create->mode == ALG_VID_ENC_MODE_STANDARD) {
				pObj->params.encodingPreset		= XDM_HIGH_QUALITY;
				pObj->params_h264.encQuality 	= 2;
			}
			else {
				pObj->params.encodingPreset		= XDM_HIGH_SPEED;
				pObj->params_h264.encQuality 	= 3;
			}
		  }
  	  }
  	  else {
			pObj->params.encodingPreset		= XDM_USER_DEFINED;
			pObj->params_h264.encQuality 	= 0;
	  }
	  if((create->encodePreset&ALG_VID_ENC_SVC_MASK) == ALG_VID_ENC_PRESET_SVC)
	  {
		  pObj->params_h264.numTemporalLayers			= 3;
		  pObj->params_h264.svcSyntaxEnable				= XDAS_FALSE;
		  /*
		  To disable svcSyntaxEnable as most player cant play this SVC syntax,
		  if player is comptatible, please change the value to XDAS_TRUE;
		  */
	  }
	  else	{
		  pObj->params_h264.numTemporalLayers			= 0;
		  pObj->params_h264.svcSyntaxEnable				= XDAS_FALSE;
	  }
  }

  /*enable mv when initialize for get memory information*/
  pObj->createPrm.mbMvOutEnable		 = 1;
  pObj->createPrm.forceKeyFrame 	 = FALSE;

  /* Create video encoder instance */
  pObj->hEncode = VIDENC1_create(gALG_hEngine, pObj->algName, &pObj->params);

  if (pObj->hEncode == NULL) {
    OSA_ERROR("Failed to open video encode algorithm: %s (0x%x)\n", pObj->algName, Engine_getLastError(gALG_hEngine));
    OSA_memFree(pObj);
    return NULL;
  }

  status = ALG_vidEncSetDynamicParams(pObj);
  if(status!=OSA_SOK) {
    ALG_vidEncDelete(pObj);
    return NULL;
  }

  return pObj;
}

XDAS_Int32 ALG_vidEncSetDynamicParams(ALG_VidEncObj *pObj)
{
  XDAS_Int32 status;

  if(pObj==NULL)
    return OSA_EFAIL;

  if(pObj->createPrm.codec==ALG_VID_CODEC_MJPEG) {

	OSA_mutexLock(&vnf_imcop_lock);
	OSA_mutexLock(&vicp_imcop_lock);
    ALG_jpgEncSetQvalue(pObj->hJpgEncode, pObj->createPrm.bitrate);
    OSA_mutexUnlock(&vnf_imcop_lock);
	OSA_mutexUnlock(&vicp_imcop_lock);

    return OSA_SOK;
  }

  if(pObj->createPrm.codec==ALG_VID_CODEC_H264)
  {
    memcpy(&pObj->dynamicParams_h264, &H264VENC_TI_IH264VENC_DYNAMICPARAMS, sizeof(pObj->dynamicParams_h264));
    pObj->dynamicParams.size = sizeof(pObj->dynamicParams_h264);
  }
  else if(pObj->createPrm.codec==ALG_VID_CODEC_MPEG4)
  {
	pObj->dynamicParams.size = sizeof(pObj->dynamicParams_mpeg4);
  }
  else
  {
    pObj->dynamicParams.size = sizeof(pObj->dynamicParams);
  }

  pObj->dynamicParams.inputHeight         = pObj->createPrm.height;
  pObj->dynamicParams.inputWidth          = pObj->createPrm.width;
  pObj->dynamicParams.targetBitRate       = pObj->createPrm.bitrate < 0 ? 0 : pObj->createPrm.bitrate;
  pObj->dynamicParams.generateHeader      = XDM_ENCODE_AU;
  pObj->dynamicParams.captureWidth        = pObj->createPrm.offsetH;
  pObj->dynamicParams.targetFrameRate     = pObj->createPrm.framerate;
  pObj->dynamicParams.refFrameRate        = pObj->createPrm.framerate;
  pObj->dynamicParams.interFrameInterval  = 0;
  pObj->dynamicParams.mbDataFlag		  = XDAS_FALSE;

  switch(pObj->createPrm.codec) {
    case ALG_VID_CODEC_H264:
 	  pObj->dynamicParams.intraFrameInterval  				  	= 0;
  	  pObj->dynamicParams.forceFrame 		  				  	= IVIDEO_NA_FRAME;//(pObj->createPrm.forceKeyFrame==TRUE)?IVIDEO_IDR_FRAME:IVIDEO_NA_FRAME;

	  pObj->dynamicParams_h264.sliceSize					  	= ((pObj->createPrm.height/16)*pObj->createPrm.packetsize)/100;
	  pObj->dynamicParams_h264.airRate						  	= 0;
      pObj->dynamicParams_h264.intraFrameQP					  	= pObj->createPrm.qpinit;
      pObj->dynamicParams_h264.interPFrameQP				  	= pObj->createPrm.qpinit;
      pObj->dynamicParams_h264.initQ						  	= pObj->createPrm.qpinit;
      pObj->dynamicParams_h264.rcQMax						  	= (pObj->createPrm.qpmax>51)?51:pObj->createPrm.qpmax;
      pObj->dynamicParams_h264.rcQMin						  	= (pObj->createPrm.qpmin<=0)?1:pObj->createPrm.qpmin;
      pObj->dynamicParams_h264.rcQMaxI						  	= (pObj->createPrm.qpmax>51)?51:pObj->createPrm.qpmax;
      pObj->dynamicParams_h264.rcQMinI						  	= (pObj->createPrm.qpmin<=0)?1:pObj->createPrm.qpmin;
	  pObj->dynamicParams_h264.maxDelay						  	= 2000;
	  pObj->dynamicParams_h264.aspectRatioX 				  	= 1;
	  pObj->dynamicParams_h264.aspectRatioY 				  	= 1;
	  pObj->dynamicParams_h264.lfDisableIdc					  	= 0;
   	  pObj->dynamicParams_h264.enableBufSEI					  	= 1;    /*!< Enable Buffering period SEI */
  	  pObj->dynamicParams_h264.enablePicTimSEI				  	= 1;  	/*!< Enable Picture Timing SEI */
 	  pObj->dynamicParams_h264.perceptualRC                   	= 1;
      pObj->dynamicParams_h264.idrFrameInterval               	= 100;//pObj->createPrm.keyFrameInterval;
      pObj->dynamicParams_h264.mvSADoutFlag 				  	= pObj->createPrm.mbMvOutEnable;
	  pObj->dynamicParams_h264.resetHDVICPeveryFrame		  	= pObj->createPrm.resetHDVICP;
      pObj->dynamicParams_h264.enableROI					  	= 1;
	  pObj->dynamicParams_h264.metaDataGenerateConsume        	= 0;
	  pObj->dynamicParams_h264.maxBitrateCVBR				  	= 0;
	  pObj->dynamicParams_h264.interlaceRefMode				  	= 0;
	  pObj->dynamicParams_h264.enableGDR						= 0;
	  pObj->dynamicParams_h264.GDRduration						= 0;
	  pObj->dynamicParams_h264.GDRinterval						= 0;
	  pObj->dynamicParams_h264.LongTermRefreshInterval			= 0;
	  pObj->dynamicParams_h264.UseLongTermFrame					= 0;
	  pObj->dynamicParams_h264.SetLongTermFrame					= 0;
	  pObj->dynamicParams_h264.CustomScaleMatrix_Buffer			= NULL;
	  pObj->dynamicParams_h264.CVBRsensitivity					= 0;
	  pObj->dynamicParams_h264.CVBRminbitrate					= 0;
	  pObj->dynamicParams_h264.LBRmaxpicsize					= 0;
	  pObj->dynamicParams_h264.LBRminpicsize					= 0;
	  pObj->dynamicParams_h264.LBRskipcontrol					= 0x050004;
	  pObj->dynamicParams_h264.maxHighCmpxIntCVBR			  	= 0;
	  pObj->dynamicParams_h264.disableMVDCostFactor			  	= 0;
	  pObj->dynamicParams_h264.putDataGetSpaceFxn			  	= NULL;
	  pObj->dynamicParams_h264.dataSyncHandle				  	= NULL;

	  pObj->dynamicParams_h264.VUI_Buffer->timingInfoPresentFlag= 1;
	  pObj->dynamicParams_h264.VUI_Buffer->timeScale 			= 240*(pObj->createPrm.framerate/1000);
	  pObj->dynamicParams_h264.VUI_Buffer->numUnitsInTicks 		= 120;

	  /* these three fields are not set as they are reserved */
	  /* intraSliceNum, meMultiPart and intraThrQF */
	  switch(pObj->createPrm.rateControlType) {
		case ALG_VID_ENC_RATE_CONTROL_CBR:
		  pObj->dynamicParams_h264.rcAlgo     = 5;
		  break;
		default:
		case ALG_VID_ENC_RATE_CONTROL_VBR:
		  pObj->dynamicParams_h264.rcAlgo     = 1;
		  break;
		case ALG_VID_ENC_RATE_CONTROL_RCOFF:
		  pObj->dynamicParams_h264.rcAlgo     = 2;
		  break;
	  }
	  if( pObj->dynamicParams_h264.sliceSize > (pObj->createPrm.height/16) )
	  {
		pObj->dynamicParams_h264.sliceSize = (pObj->createPrm.height/16);
	  }

      break;

    case ALG_VID_CODEC_MPEG4:

	  pObj->dynamicParams.inputHeight 		  	= pObj->createPrm.height;
	  pObj->dynamicParams.inputWidth		  	= pObj->createPrm.offsetH;
	  pObj->dynamicParams.captureWidth        	= 0;
 	  pObj->dynamicParams.intraFrameInterval  	= pObj->createPrm.keyFrameInterval;
  	  pObj->dynamicParams.forceFrame 		  	= (pObj->createPrm.forceKeyFrame==TRUE)?IVIDEO_I_FRAME:IVIDEO_NA_FRAME;

	  pObj->dynamicParams_mpeg4.numMBRows      	= ((pObj->createPrm.height/16)*pObj->createPrm.packetsize)/100;
	  pObj->dynamicParams_mpeg4.initQ          	= pObj->createPrm.qpinit;
	  pObj->dynamicParams_mpeg4.rcQMin         	= (pObj->createPrm.qpmin<=1)?2:pObj->createPrm.qpmin;
	  pObj->dynamicParams_mpeg4.rcQMax         	= (pObj->createPrm.qpmax>31)?31:pObj->createPrm.qpmax;
	  pObj->dynamicParams_mpeg4.rateFix        	= 0;
	  pObj->dynamicParams_mpeg4.rateFixRange   	= 2;
	  pObj->dynamicParams_mpeg4.interFrameQP   	= 0;
	  pObj->dynamicParams_mpeg4.intraFrameQP   	= 0;
	  if( pObj->dynamicParams_mpeg4.numMBRows > (pObj->createPrm.height/16) )
	  {
		pObj->dynamicParams_mpeg4.numMBRows = (pObj->createPrm.height/16);
	  }

	  if((pObj->createPrm.encodePreset&ALG_VID_ENC_PRESET_MASK) == ALG_VID_ENC_PRESET_AUTO)
	  {
		  pObj->dynamicParams_mpeg4.intraAlgo      	= IMP4VENC_INTRA_INTER_DECISION_LQ_HP;
		  pObj->dynamicParams_mpeg4.skipMBAlgo     	= IMP4VENC_SKIP_MB_LQ_HP;
		  pObj->dynamicParams_mpeg4.unrestrictedMV 	= IMP4VENC_UMV_LQ_HP; //(pObj->createPrm.mode == ALG_VID_ENC_MODE_FAST_FPS) ? IMP4VENC_UMV_LQ_HP:IMP4VENC_UMV_HQ_LP;
		  if(pObj->createPrm.width<=720) // for D1 or QVGA
			pObj->dynamicParams_mpeg4.meAlgo		= IMP4VENC_ME_MQ_MP;
		  else if(pObj->createPrm.width==1280) // for 720P or SXVGA
			pObj->dynamicParams_mpeg4.meAlgo		= IMP4VENC_ME_HQ_MP;
		  else // 1080P or others.
			pObj->dynamicParams_mpeg4.meAlgo		= IMP4VENC_ME_LQ_HP;
	  }
	  else {
		  pObj->dynamicParams_mpeg4.intraAlgo      	= IMP4VENC_INTRA_INTER_DECISION_HQ_LP;
		  pObj->dynamicParams_mpeg4.skipMBAlgo     	= IMP4VENC_SKIP_MB_HQ_LP;
		  pObj->dynamicParams_mpeg4.unrestrictedMV 	= IMP4VENC_UMV_LQ_HP; //switched off due to UMV bug
		  pObj->dynamicParams_mpeg4.meAlgo			= IMP4VENC_ME_HQ_LP;
	  }

	  pObj->dynamicParams_mpeg4.mvDataEnable = pObj->createPrm.mbMvOutEnable;

      break;
    case ALG_VID_CODEC_MJPEG:
			pObj->createPrm.mbMvOutEnable = 0;
      break;
  }

  #ifdef ALG_VID_ENC_DEBUG
  OSA_printf(" ALG: VidEnc: DynamicParams: bitrate = %ld bps, Key-frame-interval = %ld\n",
    	pObj->dynamicParams.targetBitRate,
    	pObj->dynamicParams_h264.idrFrameInterval
    );
  #endif

  if(pObj->createPrm.codec!=ALG_VID_CODEC_H264) {
	 OSA_mutexLock(&vnf_imcop_lock);
	 OSA_mutexLock(&vicp_imcop_lock);
  }
  else {
    if (pObj->params_h264.enableDDRbuff==0)
      OSA_mutexLock(&vicp_imcop_lock);
  }

  /* Set video encoder dynamic parameters */
  pObj->encStatus.size = sizeof(VIDENC1_Status);
  pObj->encStatus.data.buf = NULL;
  status = VIDENC1_control(pObj->hEncode, XDM_SETPARAMS, (VIDENC1_DynamicParams*)&pObj->dynamicParams, &pObj->encStatus);

  if (status != VIDENC1_EOK) {
    OSA_ERROR("XDM_SETPARAMS failed, status=%ld\n", status);
    return OSA_EFAIL;
  }

  pObj->encStatus.data.buf = NULL;

  status = VIDENC1_control(pObj->hEncode, XDM_GETBUFINFO, (VIDENC1_DynamicParams*)&pObj->dynamicParams, &pObj->encStatus);

  if (status != VIDENC1_EOK) {
      OSA_ERROR("XDM_GETBUFINFO failed, status=%ld\n", status);
      return OSA_EFAIL;
  }

  #ifdef ALG_VID_ENC_DEBUG
  	OSA_printf(" ALG: VidEnc: mbMvOutEnable =%d \n",(int)pObj->createPrm.mbMvOutEnable);
	OSA_printf(" ALG: VidEnc: encStatus.bufInfo.minNumOutBufs is %d\n", (int)pObj->encStatus.bufInfo.minNumOutBufs);					//2
	OSA_printf(" ALG: VidEnc: encStatus.bufInfo.minOutBufSize[0] is %d\n", (int)pObj->encStatus.bufInfo.minOutBufSize[0]);
	OSA_printf(" ALG: VidEnc: encStatus.bufInfo.minOutBufSize[1] is %d\n", (int)pObj->encStatus.bufInfo.minOutBufSize[1]);
  	OSA_printf(" ALG: VidEnc: XDM_GETBUFINFO: min in bufs:%ld buf(0):%ld buf(1):%ld\n", pObj->encStatus.bufInfo.minNumInBufs,  																																										pObj->encStatus.bufInfo.minInBufSize[0],  																																										pObj->encStatus.bufInfo.minInBufSize[1]);
  #endif

	if( pObj->createPrm.mbMvOutEnable )
	{
		if( pObj->encStatus.bufInfo.minNumOutBufs > 1 )
		{
			if( pObj->MV_Size == 0 )
			{

				pObj->MV_Size = pObj->encStatus.bufInfo.minOutBufSize[1];
				pObj->MV_Addr = OSA_cmemAlloc(pObj->MV_Size,32);
				if( pObj->MV_Addr == NULL )
				{
					OSA_ERROR("OSA_cmemAlloc failed \n");
		      		return OSA_EFAIL;
				}

				#ifdef ALG_VID_ENC_DEBUG
				OSA_printf(" ALG: VidEnc: pObj->MV algName =%s \n",pObj->algName);
			    OSA_printf(" ALG: VidEnc: pObj->MV_Size =%d \n",(int)pObj->MV_Size);
				OSA_printf(" ALG: VidEnc: Default is Disable MV data output \n");
				#endif
				/* Default is Disable MV data output */
 				pObj->createPrm.mbMvOutEnable = 0;
 				switch(pObj->createPrm.codec)
 				{
 					case ALG_VID_CODEC_H264:
						pObj->dynamicParams_h264.mvSADoutFlag = 0;
 					break;
 					case ALG_VID_CODEC_MPEG4:
 						pObj->dynamicParams_mpeg4.mvDataEnable = 0;
 					break;
 				}
 				status = VIDENC1_control(pObj->hEncode, XDM_SETPARAMS, (VIDENC1_DynamicParams*)&pObj->dynamicParams, &pObj->encStatus);
				if (status != VIDENC1_EOK) {
					OSA_ERROR("XDM_SETPARAMS failed, status=%ld\n", status);
					return OSA_EFAIL;
				}
			}
		}else{
			OSA_ERROR("pObj->encStatus.bufInfo.minNumOutBufs is incorrect for MV_DATA \n");
	    	return OSA_EFAIL;
		}
	}

	/* Reset Force Frame Flag */
	pObj->createPrm.forceKeyFrame = FALSE;

	if(pObj->createPrm.codec!=ALG_VID_CODEC_H264){
		OSA_mutexUnlock(&vnf_imcop_lock);
		OSA_mutexUnlock(&vicp_imcop_lock);
	} else {
	  if (pObj->params_h264.enableDDRbuff== 0)
      	OSA_mutexUnlock(&vicp_imcop_lock);
   }

   return OSA_SOK;
}

int ALG_vidEncSetKeyFrameInterval(void *hndl, int keyFrameInterval, int forceFrame)
{
  ALG_VidEncObj *pObj = (ALG_VidEncObj*)hndl;
  int status;

  if(pObj==NULL)
    return OSA_EFAIL;

  pObj->createPrm.keyFrameInterval = keyFrameInterval;
  pObj->createPrm.forceKeyFrame = (forceFrame==1)?TRUE:FALSE;

  pObj->createPrm.bitrate--; // since keyFrameInterval does not take effect unless there is change in bitrate
  status = ALG_vidEncSetDynamicParams(pObj);
  pObj->createPrm.bitrate++; // restore original bitrate
  status = ALG_vidEncSetDynamicParams(pObj);

  return status;
}

int ALG_vidEncSetQPSize(void *hndl, int qpinit, int qpmin, int qpmax, int packetsize)
{
  ALG_VidEncObj *pObj = (ALG_VidEncObj*)hndl;
  int status;

  if(pObj==NULL)
    return OSA_EFAIL;

  pObj->createPrm.qpinit 		= qpinit;
  pObj->createPrm.qpmin 		= qpmin;
  pObj->createPrm.qpmax 		= qpmax;
  pObj->createPrm.packetsize 	= packetsize;

  status = ALG_vidEncSetDynamicParams(pObj);

  return status;
}


int ALG_vidEncSetBitrate(void *hndl, int bitrate)
{
  ALG_VidEncObj *pObj = (ALG_VidEncObj*)hndl;

  if(pObj==NULL)
    return OSA_EFAIL;

  pObj->createPrm.bitrate = bitrate;

  return ALG_vidEncSetDynamicParams(pObj);
}
// add zeng
int ALG_vidEncSetRCType(void *hndl, int nRcType)
{
  ALG_VidEncObj *pObj = (ALG_VidEncObj*)hndl;

  if(pObj==NULL)
    return OSA_EFAIL;
  
  //pObj->createPrm.bitrate = bitrate;
  if(nRcType == ALG_VID_ENC_RATE_CONTROL_CBR ||
  	nRcType == ALG_VID_ENC_RATE_CONTROL_VBR  ||
  	nRcType == ALG_VID_ENC_RATE_CONTROL_RCOFF )
  {
  	pObj->createPrm.rateControlType = nRcType;

  }
  else
  {
	pObj->createPrm.rateControlType = ALG_VID_ENC_RATE_CONTROL_RCOFF;
  }

  printf("[ALG_vidEncSetRCType]rateControlType=%d \n" , pObj->createPrm.rateControlType);
  return ALG_vidEncSetDynamicParams(pObj);
}


int ALG_vidEncSetROI(void *hndl, int flag)
{
  ALG_VidEncObj *pObj = (ALG_VidEncObj*)hndl;

  if(pObj==NULL)
    return OSA_EFAIL;

  pObj->createPrm.enableROI = flag;

  return ALG_vidEncSetDynamicParams(pObj);
}

int ALG_vidEncSetFramerate(void *hndl, int framerate)
{
  ALG_VidEncObj *pObj = (ALG_VidEncObj*)hndl;

  if(pObj==NULL)
    return OSA_EFAIL;

  pObj->createPrm.framerate = framerate;

  return ALG_vidEncSetDynamicParams(pObj);
}


int ALG_vidEncSetMVdataOut(void *hndl, int mbMvOutEnable)
{
  ALG_VidEncObj *pObj = (ALG_VidEncObj*)hndl;

  if(pObj==NULL)
    return OSA_EFAIL;

  pObj->createPrm.mbMvOutEnable = mbMvOutEnable;

  return ALG_vidEncSetDynamicParams(pObj);
}


int ALG_vidEncGetMVdataSize(void *hndl)
{
  ALG_VidEncObj *pObj = (ALG_VidEncObj*)hndl;

  if(pObj==NULL)
    return OSA_EFAIL;

  return pObj->MV_Size;
}

void *ALG_vidEncGetMVdataAddr(void *hndl)
{
  ALG_VidEncObj *pObj = (ALG_VidEncObj*)hndl;

  if(pObj==NULL)
    return NULL;

  return pObj->MV_Addr;
}

int ALG_vidEncGetMVdataStatus(void *hndl)
{
  ALG_VidEncObj *pObj = (ALG_VidEncObj*)hndl;

  if(pObj==NULL)
    return OSA_EFAIL;

  return pObj->createPrm.mbMvOutEnable;
}

void CheckROIBoundary(XDM_Point *val, int xbound, int ybound)
{
	if(val->x<0) val->x=0;
	if(val->y<0) val->y=0;

	if(val->x>xbound) val->x=xbound;
	if(val->y>ybound) val->x=ybound;
}

XDAS_Int32 GetSVCtemporalId(XDAS_Int32 numTemporalLayers, XDAS_Int32 frameType, XDAS_Int32 frameBytes)
{
	XDAS_Int32 scalabilty;
	XDAS_Int32 maxRefFrames;
	static XDAS_Int32 uidrFrmNumInGOP = 0;
	XDAS_Int32 temporalId = 3;

	if(numTemporalLayers == 255 || numTemporalLayers <= 0 || numTemporalLayers > 3)
	{
		return 0;
	}
	else
	{
		maxRefFrames = numTemporalLayers + 1;

		if(frameType == IVIDEO_IDR_FRAME)
			uidrFrmNumInGOP = 0;
		else if(frameBytes != 0)
			uidrFrmNumInGOP++;

		switch(maxRefFrames)
		{
			case 2:
			{
				if (uidrFrmNumInGOP & 0x01)
					temporalId = (maxRefFrames - 1);
				else
					temporalId = 0;
			}
			break;

			case 3:
			{
				if (uidrFrmNumInGOP & 0x01)
					temporalId = (maxRefFrames - 1);
				else
					temporalId = (uidrFrmNumInGOP & 0x02)?1:0;
			}
			break;

			case 4:
			{
				if (uidrFrmNumInGOP & 0x01)
				{
					temporalId = (maxRefFrames - 1);
				}
				else
				{
					if ((uidrFrmNumInGOP & 0x03) == 0x02)
						temporalId = 2;
					else
						temporalId = ((uidrFrmNumInGOP & 0x07)==0x04)?1:0;
				}
			}
			break;

			default:
			{
			}
			break;
		}
	}

	return temporalId;
}
//changed by zeng
int ALG_vidEncRun(void *hndl, ALG_VidEncRunPrm *prm, ALG_VidEncRunStatus *runStatus, CallBackSetOSDPixel func)
{
  ALG_VidEncObj *pObj = (ALG_VidEncObj*)hndl;
  IVIDEO1_BufDescIn  inBufDesc;
  XDM_BufDesc        outBufDesc;
  XDAS_Int32         status;
  VIDENC1_InArgs   	 inArgs;
  VIDENC1_OutArgs    outArgs;
  IH264VENC_InArgs 	 h264InArgs;
  IH264VENC_OutArgs  h264OutArgs;
  XDAS_Int32         offset;
  XDAS_Int32         outBufSizeArray[2];
  XDAS_UInt8         *dst[2],i=0;
  static unsigned long h264Timestamp = 0;

  runStatus->bytesGenerated = 0;
  runStatus->isKeyFrame     = FALSE;
  runStatus->numMB          = 0;

  if(pObj==NULL)
    return OSA_EFAIL;

  if(pObj->createPrm.codec==ALG_VID_CODEC_MJPEG)
  {
    ALG_JpgEncRunPrm jpgRun;
    ALG_JpgEncRunStatus jpgStatus;

    jpgRun.inStartX = prm->inStartX;
    jpgRun.inStartY = prm->inStartY;
    jpgRun.inAddr   = prm->inAddr;
    jpgRun.outAddr  = prm->outAddr;
    jpgRun.outDataMaxSize = prm->outDataMaxSize;

    jpgRun.snapEnable = prm->snapEnable;
	strcpy(jpgRun.snapFilename, prm->snapFilename);

	OSA_mutexLock(&vnf_imcop_lock);
	OSA_mutexLock(&vicp_imcop_lock);

        status = ALG_jpgEncRun(pObj->hJpgEncode, &jpgRun, &jpgStatus, NULL);

	OSA_mutexUnlock(&vnf_imcop_lock);
	OSA_mutexUnlock(&vicp_imcop_lock);

    runStatus->isKeyFrame     = TRUE;

    if(status!=OSA_SOK) {

      return OSA_EFAIL;
    }

    runStatus->bytesGenerated = jpgStatus.bytesGenerated;

    return OSA_SOK;
  }

  offset = pObj->createPrm.offsetH * pObj->createPrm.offsetV;

  if(prm->outDataMaxSize==0)
    prm->outDataMaxSize = pObj->createPrm.width*pObj->createPrm.height*2;

  outBufSizeArray[0] = prm->outDataMaxSize;

  prm->inStartY = OSA_floor(prm->inStartY, 2);
  prm->inStartX = OSA_floor(prm->inStartX, 2);

  inBufDesc.numBufs       = 2;
  inBufDesc.frameWidth    = OSA_align(pObj->createPrm.width, 16);
  inBufDesc.frameHeight   = OSA_align(pObj->createPrm.height, 16);
  inBufDesc.framePitch    = pObj->createPrm.offsetH;

  inBufDesc.bufDesc[0].bufSize = pObj->createPrm.offsetH*inBufDesc.frameHeight ;
  inBufDesc.bufDesc[0].buf     = (XDAS_Int8*)(prm->inAddr + prm->inStartY*pObj->createPrm.offsetH + prm->inStartX);
  inBufDesc.bufDesc[0].accessMask = 0;

  inBufDesc.bufDesc[1].bufSize = pObj->createPrm.offsetH*inBufDesc.frameHeight/2;
  inBufDesc.bufDesc[1].buf     = (XDAS_Int8*)(prm->inAddr + offset + prm->inStartY*pObj->createPrm.offsetH/2 + prm->inStartX);
  inBufDesc.bufDesc[1].accessMask = 0;

  if(prm->snapEnable) {
  	OSA_fileWriteVideoData(prm->snapFilename, (Uint8 *)inBufDesc.bufDesc[0].buf, (Uint8 *)inBufDesc.bufDesc[1].buf,
  		inBufDesc.frameWidth, inBufDesc.frameHeight, inBufDesc.framePitch);
  }

  if( pObj->MV_Size == 0 )
  {
	  outBufDesc.numBufs  = 1;
	  outBufDesc.bufSizes = outBufSizeArray;
	  outBufDesc.bufs     = (XDAS_Int8 **) &prm->outAddr;
  }
  else {

	dst[0]	           = prm->outAddr;
	dst[1]             = pObj->MV_Addr;
	outBufSizeArray[1] = pObj->MV_Size;

	outBufDesc.numBufs  = 2;
	outBufDesc.bufSizes = outBufSizeArray;
	outBufDesc.bufs     = dst;
  }

	inArgs.size         		= sizeof(VIDENC1_InArgs);
	inArgs.inputID 				= 1;
	inArgs.topFieldFirstFlag	= 0;

	if(pObj->createPrm.codec==ALG_VID_CODEC_H264)
	{
		h264InArgs.videncInArgs 			= inArgs;
		h264InArgs.videncInArgs.size 		= sizeof(IH264VENC_InArgs);
		h264InArgs.roiParameters.numOfROI 	= (prm->roiPrm.numROI>=MAX_ROI)?MAX_ROI:prm->roiPrm.numROI;
		for(i=0;i<h264InArgs.roiParameters.numOfROI;i++)
		{
			h264InArgs.roiParameters.listROI[i].topLeft.x = prm->roiPrm.roiInfo[i].topLeft_x;
			h264InArgs.roiParameters.listROI[i].topLeft.y = prm->roiPrm.roiInfo[i].topLeft_y;
			h264InArgs.roiParameters.listROI[i].bottomRight.x = prm->roiPrm.roiInfo[i].bottomRight_x;
			h264InArgs.roiParameters.listROI[i].bottomRight.y = prm->roiPrm.roiInfo[i].bottomRight_y;
			h264InArgs.roiParameters.roiPriority[i] = prm->roiPrm.roiInfo[i].roiPriority;

			printf("ROI[%d]=(%d,%d,%d,%d)roiPriority=%d\n" , i, prm->roiPrm.roiInfo[i].topLeft_x , prm->roiPrm.roiInfo[i].topLeft_y ,
				prm->roiPrm.roiInfo[i].bottomRight_x , prm->roiPrm.roiInfo[i].bottomRight_y ,prm->roiPrm.roiInfo[i].roiPriority );
			CheckROIBoundary(&h264InArgs.roiParameters.listROI[i].topLeft, pObj->createPrm.width, pObj->createPrm.height);
			CheckROIBoundary(&h264InArgs.roiParameters.listROI[i].bottomRight, pObj->createPrm.width, pObj->createPrm.height);

			switch(prm->roiPrm.roiInfo[i].roiType)
			{
				case ALG_VID_ENC_ROI_TYPE_FACE:
					h264InArgs.roiParameters.roiType[i] = FACE_OBJECT;
					break;
				case ALG_VID_ENC_ROI_TYPE_PRIVACYMASK:
					h264InArgs.roiParameters.roiType[i] = PRIVACY_MASK;
					break;
				case ALG_VID_ENC_ROI_TYPE_FOREGROUND:
					h264InArgs.roiParameters.roiType[i] = FOREGROUND_OBJECT;
					break;
				default: // No other Types currently supported
					h264InArgs.roiParameters.roiType[i] = DEFAULT_OBJECT;
					break;
			}
		}
		h264InArgs.timeStamp 			= h264Timestamp;
		h264InArgs.insertUserData 		= XDAS_FALSE;
		h264InArgs.lengthUserData 		= 0;
		h264InArgs.numOutputDataUnits 	= 0;
		h264Timestamp = h264Timestamp + 2;
	}

	if(pObj->createPrm.codec == ALG_VID_CODEC_H264)
	{
    	h264OutArgs.videncOutArgs.size = sizeof(IH264VENC_OutArgs);
	}
    else {
    	outArgs.size = sizeof(VIDENC1_OutArgs);
    }

	if(pObj->createPrm.codec!=ALG_VID_CODEC_H264)
	{
  		OSA_mutexLock(&vnf_imcop_lock);
  		OSA_mutexLock(&vicp_imcop_lock);
	}
	else {
		/*
		Can be commented if pObj->params_h264.enableDDRbuff = 1;
		This way, VICP functions can be ran in parallel with H.264.
		But since DDR version of H.264 is slower, only useful
		if you need to hide VICP processing behind H.264 to
		meet system fps target.
		*/
		if (pObj->params_h264.enableDDRbuff== 0)
			OSA_mutexLock(&vicp_imcop_lock);
  	}

    //OSA_printf("\n Entering Encode process \n");
	if( NULL != func ){
		func(inBufDesc.frameWidth, inBufDesc.frameHeight, inBufDesc.bufDesc[0].buf, inBufDesc.bufDesc[1].buf);
	}

    if(pObj->createPrm.codec==ALG_VID_CODEC_H264)
        status = VIDENC1_process(pObj->hEncode, &inBufDesc, &outBufDesc, (VIDENC1_InArgs*)&h264InArgs, (IVIDENC1_OutArgs*)&h264OutArgs);
    else
        status = VIDENC1_process(pObj->hEncode, &inBufDesc, &outBufDesc, &inArgs, &outArgs);

    //OSA_printf("\n Exiting Encode process : %d %d \n", status, VIDENC1_EOK);

	if(pObj->createPrm.codec!=ALG_VID_CODEC_H264)
	{
		OSA_mutexUnlock(&vicp_imcop_lock);
		OSA_mutexUnlock(&vnf_imcop_lock);
	}
	else {
		/*   Can be commented if pObj->params_h264.enableDDRbuff = 1;
		This way, VICP functions can be ran in parallel with H.264.
		But since DDR version of H.264 is slower, only useful
		if you need to hide VICP processing behind H.264 to
		meet system fps target.
		*/
		if (pObj->params_h264.enableDDRbuff== 0)
			OSA_mutexUnlock(&vicp_imcop_lock);
	}

    if(pObj->createPrm.codec==ALG_VID_CODEC_H264) {
		if (status != VIDENC1_EOK){
			//OSA_printf("CODEC RUN FAIL ERROR CODE: %d %d %d\n", status, h264OutArgs.videncOutArgs.extendedError, h264OutArgs.videncOutArgs.bytesGenerated);
			return OSA_EFAIL;
		}
		runStatus->bytesGenerated 	= h264OutArgs.videncOutArgs.bytesGenerated;
		runStatus->temporalId 		= GetSVCtemporalId(pObj->params_h264.numTemporalLayers,
	                                  					h264OutArgs.videncOutArgs.encodedFrameType,
									  					h264OutArgs.videncOutArgs.bytesGenerated);

		if(h264OutArgs.videncOutArgs.encodedFrameType==IVIDEO_I_FRAME || h264OutArgs.videncOutArgs.encodedFrameType==IVIDEO_IDR_FRAME)
			runStatus->isKeyFrame = TRUE;
	}
	else {
		if (status != VIDENC1_EOK){
			//OSA_printf("CODEC RUN FAIL ERROR CODE: %d %d %d\n", status, outArgs.extendedError, outArgs.bytesGenerated);
			return OSA_EFAIL;
		}
		runStatus->bytesGenerated = outArgs.bytesGenerated;

		if(outArgs.encodedFrameType==IVIDEO_I_FRAME || outArgs.encodedFrameType==IVIDEO_IDR_FRAME)
			runStatus->isKeyFrame = TRUE;
	}

	return OSA_SOK;
}

int ALG_vidEncDelete(void *hndl)
{
  ALG_VidEncObj *pObj=(ALG_VidEncObj *)hndl;

  if(pObj==NULL)
    return OSA_EFAIL;

  if(pObj->createPrm.codec==ALG_VID_CODEC_MJPEG)
  {
    ALG_jpgEncDelete(pObj->hJpgEncode);

  }
  else {

    if(pObj->hEncode)
    {
    	if( pObj->MV_Size != 0 )
    	{
				OSA_cmemFree(pObj->MV_Addr);
    	}

      VIDENC1_delete(pObj->hEncode);
    }
  }

  OSA_memFree(pObj);

  return OSA_SOK;
}

int ALG_vidEncTestMain(int argc, char **argv)
{
  int status, i, k, numFrames=1000;
  Uint8 *inVirtAddr=NULL, *inPhysAddr, *outVirtAddr=NULL, *outPhysAddr, *curOutAddr;
  OSA_PrfHndl prfVidEnc[ALG_VID_ENC_TEST_CHANNELS];
  Uint32 size=4*MB, fileSize;
  void *algVidEncHndl[ALG_VID_ENC_TEST_CHANNELS];
  ALG_VidEncCreate createPrm;
  ALG_VidEncRunPrm runPrm;
  ALG_VidEncRunStatus runStatus;
  FILE *fileHndl[ALG_VID_ENC_TEST_CHANNELS];
  char filename[20];

  status = DRV_init();
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_init()\n");
    return status;
  }

  status = ALG_sysInit();
  if(status!=OSA_SOK)  {
    DRV_exit();
    return status;
  }

  inVirtAddr     = OSA_cmemAlloc(size, 32);
  outVirtAddr    = OSA_cmemAlloc(size, 32);

  inPhysAddr     = OSA_cmemGetPhysAddr(inVirtAddr);
  outPhysAddr    = OSA_cmemGetPhysAddr(outVirtAddr);

  memset(inVirtAddr, 0x0, size);
  memset(outVirtAddr, 0x0, size);

  if(   inVirtAddr==NULL || outVirtAddr == NULL
     || inPhysAddr==NULL || outPhysAddr == NULL
    ) {
    OSA_ERROR("OSA_cmemAlloc()\n");
    goto error_exit;
  }

  memset(fileHndl, 0, sizeof(fileHndl));

  createPrm.codec           = ALG_VID_CODEC_H264;
  createPrm.dataFormat      = DRV_DATA_FORMAT_YUV420;
  createPrm.width           = 1920;  //320
  createPrm.height          = 1088;  //240
  createPrm.offsetH         = 1920;  //320
  createPrm.offsetV         = 1088;  //240
  createPrm.bitrate         = 6*1000*1000;
  createPrm.mode            = ALG_VID_ENC_MODE_STANDARD;
  createPrm.keyFrameInterval= 30;
  createPrm.mbMvOutEnable   = FALSE;
  createPrm.qValue          = 75;
  createPrm.ddrEnable       = 0;
  createPrm.forceKeyFrame   = FALSE;

  if(createPrm.dataFormat==DRV_DATA_FORMAT_YUV420)
    status = OSA_fileReadFile("videoQVGA.yuv420", inVirtAddr, size, &fileSize);
  else
    status = OSA_fileReadFile("video.yuv422", inVirtAddr, size, &fileSize);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_fileReadFile()\n");
    goto error_exit;
  }

  memset(algVidEncHndl, 0, sizeof(algVidEncHndl));

  for(k=0; k<ALG_VID_ENC_TEST_CHANNELS; k++) {
    algVidEncHndl[k] = ALG_vidEncCreate(&createPrm);
    if(algVidEncHndl[k]==NULL) {
      OSA_ERROR("ALG_vidEncCreate()\n");
      goto error_exit;
    }

    OSA_prfReset(&prfVidEnc[k]);

    sprintf( filename, "%s/CH%02d.bits", ALG_VID_ENC_TEST_FILESYS, k);

    fileHndl[k] = fopen(filename, "wb");

    if(fileHndl[k]==NULL) {
      OSA_ERROR("fopen(%s)", filename);
      goto error_exit;
    }
  }

  curOutAddr = outVirtAddr;

  for(i=0; i<numFrames; i++) {

    runPrm.inStartX       = 0;
    runPrm.inStartY       = 0;
    runPrm.inAddr         = inVirtAddr;
    runPrm.outAddr        = curOutAddr;
    runPrm.mbMvInfo       = NULL;
    runPrm.outDataMaxSize = size;
    runPrm.roiPrm.numROI  = 0;

    for(k=0; k<ALG_VID_ENC_TEST_CHANNELS; k++) {

      #if 1
      OSA_prfBegin(&prfVidEnc[k]);
      status = ALG_vidEncRun(algVidEncHndl[k], &runPrm, &runStatus,NULL);
      OSA_prfEnd(&prfVidEnc[k], 1);
      #endif

      if(status==OSA_SOK) {
        #ifdef ALG_VID_ENC_TEST_DEBUG
        OSA_printf(" ALG: VidEnc: CH %d: %d: %ld bytes, key_frame = %ld, MB = %d\n", k, i, runStatus.bytesGenerated, runStatus.isKeyFrame, runStatus.numMB);
        #endif

        fwrite(curOutAddr, runStatus.bytesGenerated, 1, fileHndl[k]);

      } else {
        OSA_printf(" ALG: VidEnc: ALG_vidEncRun() ERROR !!!\n");
        break;
      }
    }
    #if 0
    if((i%11)==0) {
      ALG_vidDecTestMain(0, NULL);
    }
    #endif
  }

  #ifdef ALG_VID_ENC_TEST_DEBUG
  for(k=0; k<ALG_VID_ENC_TEST_CHANNELS; k++) {
    OSA_prfPrint(&prfVidEnc[k], "VID ENC", 0);
  }
  #endif

error_exit:

  for(k=0; k<ALG_VID_ENC_TEST_CHANNELS; k++) {
    ALG_vidEncDelete(algVidEncHndl[k]);
    if(fileHndl[k])
      fclose(fileHndl[k]);
  }

  if(inVirtAddr)
    OSA_cmemFree(inVirtAddr);

  if(outVirtAddr)
    OSA_cmemFree(outVirtAddr);

  ALG_sysExit();
  DRV_exit();

  return status;
}
