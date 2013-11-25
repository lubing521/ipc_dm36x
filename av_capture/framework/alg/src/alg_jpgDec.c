
#include <alg_priv.h>
#include <alg_jpgDec.h>

typedef struct {

  IMGDEC1_Handle   hDecode;
  ALG_JpgDecCreate createPrm;
  char             algName[20];
  IMGDEC1_Params  params;
  IMGDEC1_DynamicParams dynamicParams;

  IMGDEC1_Status  decStatus;

} ALG_JpgDecObj;

void *ALG_jpgDecCreate(ALG_JpgDecCreate *create)
{
  ALG_JpgDecObj *pObj;

  if(gALG_hEngine==NULL)
    return NULL;

  pObj = OSA_memAlloc(sizeof(ALG_JpgDecObj));
  if(pObj==NULL)
    return NULL;

  memset(pObj, 0, sizeof(*pObj));

  strcpy(pObj->algName, ALG_VID_CODEC_JPEG_DEC_NAME);

  memcpy(&pObj->createPrm, create, sizeof(pObj->createPrm));

  pObj->params.size = sizeof(pObj->params);
  pObj->params.maxHeight = create->maxHeight;
  pObj->params.maxWidth  = create->maxWidth;
  pObj->params.maxScans  = 1;
  pObj->params.dataEndianness = XDM_BYTE;

  if(create->dataFormat==ALG_VID_DATA_FORMAT_YUV422)
    pObj->params.forceChromaFormat = XDM_YUV_422ILE;
  else
    pObj->params.forceChromaFormat = XDM_YUV_420SP;

  pObj->hDecode = IMGDEC1_create(gALG_hEngine, pObj->algName, &pObj->params);

  if (pObj->hDecode == NULL) {
    OSA_ERROR("Failed to open image decode algorithm: %s (0x%x)\n", pObj->algName, Engine_getLastError(gALG_hEngine));
    OSA_memFree(pObj);
    return NULL;
  }

  return pObj;
}

int ALG_jpgDecSetDynamicParams(ALG_JpgDecObj *pObj, Bool decodeHeader, Uint16 offsetH)
{
  XDAS_Int32 status;

  pObj->dynamicParams.size   = sizeof(pObj->dynamicParams);
  pObj->dynamicParams.numAU  = XDM_DEFAULT;

  if(decodeHeader)
    pObj->dynamicParams.decodeHeader = XDM_PARSE_HEADER;
  else
    pObj->dynamicParams.decodeHeader = XDM_DECODE_AU;

  pObj->dynamicParams.displayWidth = offsetH;

  /* Set video decoder dynamic parameters */
  pObj->decStatus.size = sizeof(pObj->decStatus);
  pObj->decStatus.data.buf = NULL;

  status = IMGDEC1_control(pObj->hDecode, XDM_SETPARAMS, &pObj->dynamicParams, &pObj->decStatus);

  if (status != IMGDEC1_EOK) {
    OSA_ERROR("XDM_SETPARAMS failed, status=%ld\n", status);
    return OSA_EFAIL;
  }

  pObj->decStatus.data.buf = NULL;

  status = IMGDEC1_control(pObj->hDecode, XDM_GETBUFINFO, &pObj->dynamicParams, &pObj->decStatus);

  if (status != IMGDEC1_EOK) {
      OSA_ERROR("XDM_GETBUFINFO failed, status=%ld\n", status);
      return OSA_EFAIL;
  }

  #ifdef ALG_DEBUG
  OSA_printf(" ALG: ImgDec: XDM_GETBUFINFO: min in bufs:%ld buf(0):%ld buf(1):%ld\n",pObj->decStatus.bufInfo.minNumInBufs,pObj->decStatus.bufInfo.minInBufSize[0], pObj->decStatus.bufInfo.minInBufSize[1]);
  #endif

  return OSA_SOK;
}

int ALG_jpgDecRun(void *hndl, ALG_JpgDecRunPrm *prm, ALG_JpgDecRunStatus *runStatus)
{
  ALG_JpgDecObj  *pObj = (ALG_JpgDecObj*)hndl;
  XDM1_BufDesc    inBufDesc;
  XDM1_BufDesc    outBufDesc;
  XDAS_Int32      status;
  IMGDEC1_InArgs  inArgs;
  IMGDEC1_OutArgs outArgs;
  Uint32          bytesPerPixel, offset;

  runStatus->bytesUsed = 0;
  runStatus->frameWidth = 0;
  runStatus->frameHeight = 0;

  if(pObj==NULL)
    return OSA_EFAIL;

  status = ALG_jpgDecSetDynamicParams(pObj, FALSE, prm->outOffsetH);
  if(status!=OSA_SOK)
    return status;

  offset = prm->outOffsetH * prm->outOffsetV;

  bytesPerPixel = 1;
  if(pObj->createPrm.dataFormat==ALG_VID_DATA_FORMAT_YUV422) {
    bytesPerPixel = 2;
  }

  inBufDesc.numBufs       = 1;

  inBufDesc.descs[0].bufSize    = prm->inDataSize;
  inBufDesc.descs[0].buf        = (XDAS_Int8*)prm->inAddr;
  inBufDesc.descs[0].accessMask = 0;

  prm->outStartX = OSA_align(prm->outStartX, 2);
  prm->outStartY = OSA_align(prm->outStartY, 2);

  outBufDesc.numBufs              = 1;
  outBufDesc.descs[0].buf         = (XDAS_Int8*)(prm->outAddr + (prm->outStartY*prm->outOffsetH + prm->outStartX)*bytesPerPixel);
  outBufDesc.descs[0].bufSize     = prm->outOffsetH*prm->outOffsetV*bytesPerPixel;
  outBufDesc.descs[0].accessMask  = 0;

  if(pObj->createPrm.dataFormat==ALG_VID_DATA_FORMAT_YUV420) {
    outBufDesc.numBufs = 2;

    outBufDesc.descs[1].bufSize    = outBufDesc.descs[0].bufSize/2;
    outBufDesc.descs[1].buf        = (XDAS_Int8*)(prm->outAddr + offset + (prm->outStartY*prm->outOffsetH/2 + prm->outStartX));
    outBufDesc.descs[1].accessMask = 0;
  }


  inArgs.size         = sizeof(IMGDEC1_InArgs);
  inArgs.numBytes     = inBufDesc.descs[0].bufSize;
  outArgs.size        = sizeof(IMGDEC1_OutArgs);

  status = IMGDEC1_process(pObj->hDecode, &inBufDesc, &outBufDesc, &inArgs, &outArgs);

  if (status != IMGDEC1_EOK)
    return OSA_EFAIL;

  runStatus->bytesUsed = outArgs.bytesConsumed;

  pObj->decStatus.data.buf = NULL;

  status = IMGDEC1_control(pObj->hDecode, XDM_GETSTATUS, &pObj->dynamicParams, &pObj->decStatus);

  if (status != IMGDEC1_EOK) {
      OSA_ERROR("XDM_GETSTATUS failed, status=%ld\n", status);
      return OSA_EFAIL;
  }

  runStatus->frameWidth = pObj->decStatus.outputWidth;
  runStatus->frameHeight = pObj->decStatus.outputHeight;

  return OSA_SOK;
}

int ALG_jpgDecDelete(void *hndl)
{
  ALG_JpgDecObj *pObj=(ALG_JpgDecObj *)hndl;

  if(pObj==NULL)
    return OSA_EFAIL;

  if(pObj->hDecode)
    IMGDEC1_delete(pObj->hDecode);

  OSA_memFree(pObj);

  return OSA_SOK;
}

