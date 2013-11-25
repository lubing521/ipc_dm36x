
#include <drv_faceDetect.h>
#include <osa_cmem.h>
#include <osa_file.h>

#define DRV_FACE_DETECT_WORK_AREA_SIZE    (320*240*2)

typedef struct {

  Uint8 *workAreaVirtAddr;
  Uint8 *workAreaPhysAddr;  
  
  DRV_FaceDetectRunStatus prevFaceInfo;

} DRV_FaceDetectObj;

static DRV_FaceDetectObj gDRV_faceDetectObj;

int DRV_faceDetectOpen()
{
  memset(&gDRV_faceDetectObj, 0, sizeof(gDRV_faceDetectObj));
  
  gDRV_faceDetectObj.workAreaVirtAddr = OSA_cmemAlloc(DRV_FACE_DETECT_WORK_AREA_SIZE, 32);
  gDRV_faceDetectObj.workAreaPhysAddr = OSA_cmemGetPhysAddr(gDRV_faceDetectObj.workAreaVirtAddr);
    
  if(  gDRV_faceDetectObj.workAreaVirtAddr==NULL
   ||  gDRV_faceDetectObj.workAreaPhysAddr==NULL
    ) {
    OSA_ERROR("Insufficient memory\n");
    goto error_exit;
  }
  
  CSL_faceDetectIntClear(&gCSL_faceDetectHndl);  
  CSL_faceDetectIntEnable(&gCSL_faceDetectHndl, TRUE);
  
  return OSA_SOK;
  
error_exit:
  if(gDRV_faceDetectObj.workAreaVirtAddr)
    OSA_cmemFree(gDRV_faceDetectObj.workAreaVirtAddr);

  return OSA_EFAIL;  
}

int DRV_faceDetectClose()
{
  CSL_faceDetectIntEnable(&gCSL_faceDetectHndl, FALSE);
  CSL_faceDetectIntClear(&gCSL_faceDetectHndl);

  if(gDRV_faceDetectObj.workAreaVirtAddr)
    OSA_cmemFree(gDRV_faceDetectObj.workAreaVirtAddr);

  return OSA_SOK;
}

int DRV_faceDetectStabilizeFaceStatus(DRV_FaceDetectRunStatus *curInfo)
{
  DRV_FaceDetectRunStatus *prevInfo = &gDRV_faceDetectObj.prevFaceInfo;
  int thresX=6, thresY=6, i, numFaces;
  Bool sameFace;
 
  
  for(i=0; i<curInfo->numFaces; i++) {
    if(prevInfo->info[i].sizeConfidence!=0) {
      if( abs((int)prevInfo->info[i].centerX-(int)curInfo->info[i].centerX) < thresX)
        sameFace = TRUE;
      else
        sameFace = FALSE;
      if( abs((int)prevInfo->info[i].centerY-(int)curInfo->info[i].centerY) < thresY)
        sameFace = TRUE;
      else
        sameFace = FALSE;
        
      if(sameFace) {
        memcpy(&curInfo->info[i], &prevInfo->info[i], sizeof(curInfo->info[i]));
      }
    }
  }
  
  numFaces = curInfo->numFaces;
  
  for(i=0; i<prevInfo->numFaces; i++) {  
    if(i>curInfo->numFaces) {
      memcpy(&curInfo->info[i], &prevInfo->info[i], sizeof(curInfo->info[i]));      
      numFaces = i;
    }
  }

  memset(prevInfo, 0, sizeof(*prevInfo));
  
  prevInfo->numFaces = curInfo->numFaces;
  
  for(i=0; i<curInfo->numFaces; i++) {
    memcpy(&prevInfo->info[i], &curInfo->info[i], sizeof(curInfo->info[i]));  
  }
  
  curInfo->numFaces = numFaces;
  
  return OSA_SOK;
}

int DRV_faceDetectRun(DRV_FaceDetectRunPrm *prm, DRV_FaceDetectRunStatus *pStatus)
{
  CSL_FaceDetectHwSetup setup;
  int status;
  int retry;
  
  CSL_faceDetectIntClear(&gCSL_faceDetectHndl);  

  setup.inputAddr         = prm->inPhysAddr;
  setup.workAreaAddr      = gDRV_faceDetectObj.workAreaPhysAddr;
  setup.inStartX          = 0;
  setup.inStartY          = 0;
  setup.inWidth           = prm->inWidth;
  setup.inHeight          = prm->inHeight;
  setup.detectThres       = prm->detectThres;
  setup.detectCondition   = prm->detectCondition;
    
  status = CSL_faceDetectHwSetup(&gCSL_faceDetectHndl, &setup);
  if(status!=OSA_SOK) {
    OSA_ERROR("CSL_faceDetectHwSetup()\n");
    return status;
  }
 
  CSL_faceDetectIntWait(&gCSL_faceDetectHndl, 200);
  
  retry=50;
  
  #if 0
  while( gCSL_vpssHndl.faceDetectRegs->FD_CTRL != 0x4 ) {
    OSA_waitMsecs(1);
    retry--;
    if(retry==0)
      break;
  }
  #endif
    
  CSL_faceDetectDisable(&gCSL_faceDetectHndl);  
  
  pStatus->numFaces = 0;
  
  if(retry==0) {
    return OSA_EFAIL;
  }
  
  status = CSL_faceDetectGetStatus(&gCSL_faceDetectHndl, pStatus->info, &pStatus->numFaces);
  
  //DRV_faceDetectStabilizeFaceStatus(pStatus);

  return status;
}

int DRV_faceDetectTestMain(int argc, char **argv)
{
  int status, i, count=1;
  Uint32 readSize;
  Uint8 *inBufVirtAddr=NULL;
  static DRV_FaceDetectRunStatus faceStatus;
  DRV_FaceDetectRunPrm facePrm;
  OSA_PrfHndl prfFd;
  
  status = DRV_init();
  if(status!=OSA_SOK)
    return status;
    
  OSA_prfReset(&prfFd);

  facePrm.inWidth         = 320;
  facePrm.inHeight        = 240;    
  facePrm.detectThres     = 4;
  facePrm.detectCondition = DRV_FACE_DETECT_CONDITION_DEFAULT;

  inBufVirtAddr = OSA_cmemAlloc(facePrm.inWidth*facePrm.inHeight, 32);    

  facePrm.inPhysAddr = OSA_cmemGetPhysAddr(inBufVirtAddr);
  
  if(inBufVirtAddr==NULL||facePrm.inPhysAddr==NULL) {
    OSA_ERROR("OSA_cmemAlloc()\n");
    goto error_exit;
  }
  
  status = OSA_fileReadFile("videoQVGA.yuv420", inBufVirtAddr, facePrm.inWidth*facePrm.inHeight, &readSize);
  if(status!=OSA_SOK)
    goto error_exit;
  
  status = DRV_faceDetectOpen();
  if(status!=OSA_SOK)
    goto error_exit;

  OSA_prfBegin(&prfFd);
  
  for(i=0;i<count; i++)
    status = DRV_faceDetectRun(&facePrm,  &faceStatus);
  
  OSA_prfEnd(&prfFd, count);
  OSA_prfPrint(&prfFd, "Face Detect", 0);
    
  if(status==OSA_SOK) {
    OSA_printf(" \n");
    OSA_printf(" %d Face(s) detected\n", faceStatus.numFaces);
    OSA_printf(" \n");    
    for(i=0; i<faceStatus.numFaces; i++) {
      OSA_printf(" #%2d : centre(X,Y) = (%3d,%3d),  angle = %3d, size = %d, confidence = %d \n",
             1+i, 
             faceStatus.info[i].centerX,
             faceStatus.info[i].centerY,
             faceStatus.info[i].angle,
             CSL_FACE_DETECT_GET_SIZE(faceStatus.info[i].sizeConfidence),
             CSL_FACE_DETECT_GET_CONFIDENCE(faceStatus.info[i].sizeConfidence)             
             );                              
    }
    OSA_printf("\n");    
  } else {
    OSA_printf(" FD: ERROR: Face detect could not complete, it timed out !!!\n")
  }

  DRV_faceDetectClose();

error_exit:
  if(inBufVirtAddr)
    OSA_cmemFree(inBufVirtAddr);
    
  DRV_exit();
  
  return status;
}

