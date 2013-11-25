
#include <drv_ldcPriv.h>
#include <osa_cmem.h>
#include <osa_file.h>

DRV_LdcObj gDRV_ldcObj[DRV_LDC_STREAM_ID_MAX];

int DRV_ldcInit()
{
  memset(&gDRV_ldcObj, 0, sizeof(gDRV_ldcObj));
  
  return 0;
}

int DRV_ldcExit()
{

  return 0;
}

int DRV_ldcOpen(int streamId, DRV_LdcConfig *config)
{
  int status;
  DRV_ImgsLdcConfig *pImgsLdcConfig;  
  DRV_LdcObj *pObj;  

  if(streamId >= DRV_LDC_STREAM_ID_MAX )
    return OSA_EFAIL;
    
  pObj = &gDRV_ldcObj[streamId];  

  memset(pObj, 0, sizeof(*pObj));
  
  status = OSA_mutexCreate(&pObj->lock);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_mutexCreate()\n");
    return status;
  }
  
  if(config->pLdcConfig==NULL) {
    // get config from sensor driver
    pImgsLdcConfig = DRV_imgsGetLdcConfig(config->sensorMode, config->inWidth, config->inHeight);
    if(pImgsLdcConfig==NULL) {
      OSA_ERROR("DRV_imgsGetLdcConfig(%d, %d x %d)\n", config->sensorMode, config->inWidth, config->inHeight);
      return OSA_EFAIL;
    }
    
    memcpy(&pObj->ldcPrm, pImgsLdcConfig, sizeof(pObj->ldcPrm));
        
  } else {
    memcpy(&pObj->ldcPrm, config->pLdcConfig, sizeof(pObj->ldcPrm));
  }

  return OSA_SOK;
}

int DRV_ldcClose(int streamId)
{
  
  return OSA_SOK;
}

int DRV_ldcImageTuneCmdExecuteLdc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal;
  IMAGE_TUNE_Params imgsParams;
  DRV_LdcObj *pObj;
   
  pObj = &gDRV_ldcObj[DRV_LDC_TUNING_TOOL_CTRL_STREAM_ID];  

  retVal = IMAGE_TUNE_CmdExecuteLdcPrm(cmdInfo, statusInfo);

  if(retVal==OSA_SOK) {  
    memset(&imgsParams, 0, sizeof(imgsParams));
    
    OSA_mutexLock(&pObj->lock);
  
    imgsParams.pLdcPrm = &pObj->ldcPrm.ldcParams;

    IMAGE_TUNE_GetParams(&imgsParams);
    
    OSA_mutexUnlock(&pObj->lock);    
  }
  
  return retVal;
}

int DRV_ldcRun(int streamId, DRV_LdcRunPrm *prm)
{
  CSL_LdcFrameConfig frameConfig;
  int status, format, i;
  int OBH, OBV, PIXEL_PAD, inBytesPerPixel;
  int maxOBH, minOBH, maxOBV, minOBV;
  DRV_LdcObj *pObj;

  if(streamId >= DRV_LDC_STREAM_ID_MAX )
    return OSA_EFAIL;
    
  pObj = &gDRV_ldcObj[streamId];  

  maxOBH = 64;
  maxOBV = 32;
  if(prm->inFormat==DRV_DATA_FORMAT_YUV422) {
    minOBH = 16;
    minOBV = 16;
  } else {
    minOBH = 32;
    minOBV = 32;
  }
  
  OBH=1;
  OBV=1;
  
  if(prm->OBH==0) {

    for(i=maxOBH; i>=minOBH; i-=32) {
      if((prm->inWidth%i)==0) {
        OBH=i;
        break;
      }
    }
             
  } else {
    OBH = prm->OBH;
  }

  if(prm->OBV==0) {
  
    for(i=maxOBV; i>=minOBV; i-=16) {
      if((prm->inHeight%i)==0) {
        OBV=i;
        break;
      }
    }
    
  } else {
    OBV = prm->OBV;
  }
    
  if(prm->PIXEL_PAD==0) {
    PIXEL_PAD = 8;
    if(prm->inFormat==DRV_DATA_FORMAT_RAW)
      PIXEL_PAD += 3;    
  } else {
    PIXEL_PAD = prm->PIXEL_PAD;
  }
  
  if(prm->inFormat==DRV_DATA_FORMAT_YUV422) {
    inBytesPerPixel = 2;
  } else 
  if(prm->inFormat==DRV_DATA_FORMAT_YUV420) {  
    inBytesPerPixel = 1;
  } else {
    inBytesPerPixel = 2;  
  }

  frameConfig.outBlkH           = OBH;  
  frameConfig.outBlkV           = OBV;  
  frameConfig.pixelPad          = PIXEL_PAD;    
  frameConfig.frameWidth        = prm->inWidth;
  frameConfig.frameHeight       = prm->inHeight;
  frameConfig.initX             = 0;
  frameConfig.initY             = 0;
  frameConfig.inputAddr         = prm->inPhysAddr + (prm->inOffsetH*prm->inStartY + prm->inStartX)*inBytesPerPixel;
  frameConfig.inputLineOffset   = prm->inOffsetH*inBytesPerPixel;
  frameConfig.inputAddr420C     = prm->inPhysAddr + prm->inOffsetH*prm->inOffsetV + (prm->inOffsetH*prm->inStartY/2 + prm->inStartX);
  frameConfig.outputAddr        = prm->outPhysAddr;
  frameConfig.outputLineOffset  = prm->outOffsetH*inBytesPerPixel;
  frameConfig.outputAddr420C    = prm->outPhysAddr + prm->outOffsetH*prm->outOffsetV;
  
  if(     frameConfig.outBlkH%16
      ||  frameConfig.outBlkV%2
      ||  frameConfig.frameWidth%frameConfig.outBlkH
      ||  frameConfig.frameHeight%frameConfig.outBlkV
      ||  (Uint32)frameConfig.inputAddr%32
      ||  (Uint32)frameConfig.outputAddr%32      
      ||  frameConfig.inputLineOffset%32
      ||  frameConfig.outputLineOffset%32      
    ) {
    OSA_ERROR("Illegal parameters\n");  
    return OSA_EFAIL;
  }

  if(prm->inFormat==DRV_DATA_FORMAT_YUV422)
    format = CSL_LDC_INPUT_MODE_YUV422_LD;
  else
  if(prm->inFormat==DRV_DATA_FORMAT_YUV420)  
    format = CSL_LDC_INPUT_MODE_YUV420_LD;  
  else
    format = CSL_LDC_INPUT_MODE_BAYER_CAC;  

  OSA_mutexLock(&pObj->lock);

  pObj->ldcPrm.ldcParams.ldcSetup.frameConfig = &frameConfig;
  pObj->ldcPrm.ldcParams.ldcSetup.table       = pObj->ldcPrm.ldcParams.ldcTable;
  pObj->ldcPrm.ldcParams.ldcSetup.enable      = FALSE;
  pObj->ldcPrm.ldcParams.ldcSetup.ldMapEnable = TRUE;
  pObj->ldcPrm.ldcParams.ldcSetup.inputType   = format;
  
  status = CSL_ldcHwSetup(&gCSL_ldcHndl, &pObj->ldcPrm.ldcParams.ldcSetup);
  
  OSA_mutexUnlock(&pObj->lock);  
  
  if(status!=OSA_SOK) {
    OSA_ERROR("CSL_ldcHwSetup()\n");
    return OSA_EFAIL;
  }  

  CSL_ldcIntClear(&gCSL_ldcHndl);
  CSL_ldcIntEnable(&gCSL_ldcHndl, TRUE);
  
  status = CSL_ldcEnable(&gCSL_ldcHndl, TRUE);

  if(status==OSA_SOK) {
    CSL_ldcIntWait(&gCSL_ldcHndl);
  }
  
  CSL_ldcIntEnable(&gCSL_ldcHndl, FALSE);  
  
  return status;
}

int DRV_ldcTestMain(int argc, char **argv)
{
  int status, i, count=30;
  Uint8 *inVirtAddr=NULL, *inPhysAddr, *outVirtAddr=NULL, *outPhysAddr;
  DRV_LdcConfig ldcConfig;
  DRV_LdcRunPrm ldcPrm;
  OSA_PrfHndl prfLdc;
  
  Uint32 size=2*MB, fileSize;
  
  status = DRV_init();
  if(status!=OSA_SOK)
    return status;
    
  OSA_prfReset(&prfLdc);
    
  inVirtAddr     = OSA_cmemAlloc(size, 32); 
  outVirtAddr    = OSA_cmemAlloc(size, 32);
  
  inPhysAddr     = OSA_cmemGetPhysAddr(inVirtAddr);
  outPhysAddr    = OSA_cmemGetPhysAddr(outVirtAddr);  
  
  memset(outVirtAddr, 0xAA, size);
  
  if(   inVirtAddr==NULL || outVirtAddr == NULL 
     || inPhysAddr==NULL || outPhysAddr == NULL 
    ) {
    OSA_ERROR("OSA_cmemAlloc()\n");  
    goto error_exit;
  }
  
  status = OSA_fileReadFile("video.yuv420", inVirtAddr, size, &fileSize);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_fileReadFile()\n");    
    goto error_exit;
  }
  
  ldcConfig.sensorMode = DRV_IMGS_SENSOR_MODE_DEFAULT;
  ldcConfig.inWidth    = 640;
  ldcConfig.inHeight   = 480;  
  ldcConfig.pLdcConfig = NULL;
  
  status = DRV_ldcOpen(0, &ldcConfig);
  if(status!=OSA_SOK)
    goto error_exit;
    
  ldcPrm.inFormat     = DRV_DATA_FORMAT_YUV420;
  ldcPrm.inStartX     = 0;
  ldcPrm.inStartY     = 0;  
  ldcPrm.inPhysAddr   = inPhysAddr;
  ldcPrm.inOffsetH    = ldcConfig.inWidth;
  ldcPrm.inOffsetV    = ldcConfig.inHeight;
  ldcPrm.inWidth      = ldcConfig.inWidth;
  ldcPrm.inHeight     = ldcConfig.inHeight;
  ldcPrm.outPhysAddr  = outPhysAddr;
  ldcPrm.outOffsetH   = ldcPrm.inWidth;
  ldcPrm.outOffsetV   = ldcPrm.inHeight;
  ldcPrm.OBH          = 0;
  ldcPrm.OBV          = 0;
  ldcPrm.PIXEL_PAD    = 0;

  OSA_prfBegin(&prfLdc);
  
  for(i=0; i<count; i++)
    status = DRV_ldcRun(0, &ldcPrm);
    
  OSA_prfEnd(&prfLdc, count);  
    
  OSA_prfPrint(&prfLdc, "LDC", 0);

  if(status==OSA_SOK) {
    OSA_printf(" Writing size=(%dx%d) offset=(%dx%d)\n", ldcPrm.inWidth, ldcPrm.inHeight, ldcPrm.outOffsetH, ldcPrm.outOffsetV);  
    OSA_fileWriteFile("outputLDC.yuv", outVirtAddr, ldcPrm.outOffsetH*ldcPrm.outOffsetV*2);
  }

  DRV_ldcClose(0);

error_exit:
  if(inVirtAddr)  
    OSA_cmemFree(inVirtAddr);

  if(outVirtAddr)  
    OSA_cmemFree(outVirtAddr);

  DRV_exit();
    
  return status;
}

