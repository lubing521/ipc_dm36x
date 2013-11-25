/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_faceDetect.h>


CSL_Status CSL_faceDetectHwSetup(CSL_FaceDetectHandle hndl, CSL_FaceDetectHwSetup * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  if (((Uint32) data->inputAddr) % 32)
    status = CSL_EFAIL;
  if (((Uint32) data->workAreaAddr) % 32)
    status = CSL_EFAIL;

  hndl->regs->FDIF_INTEN = 1;
    
  hndl->regs->FDIF_PICADDR = (Uint32) data->inputAddr    ;
  hndl->regs->FDIF_WKADDR  = (Uint32) data->workAreaAddr ;

  hndl->regs->FD_DCOND  = data->detectCondition;
  hndl->regs->FD_STARTX = data->inStartX;
  hndl->regs->FD_STARTY = data->inStartY;  
  hndl->regs->FD_SIZEX  = data->inWidth;
  hndl->regs->FD_SIZEY  = data->inHeight;  
  hndl->regs->FD_LHIT   = data->detectThres;

  hndl->regs->FD_CTRL    = 1;  

  // put delay
  {
    volatile int i;
    
    for(i=0; i<5000; i++)
      ;
  }
  
  hndl->regs->FD_CTRL   = 0x2; // enable FD

  return status;
}

CSL_Status CSL_faceDetectDisable(CSL_FaceDetectHandle hndl)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  hndl->regs->FD_CTRL = 0x4;  

  return CSL_SOK;
}

CSL_Status CSL_faceDetectGetStatus(CSL_FaceDetectHandle hndl, CSL_FaceDetectFaceStatus * faceList, Uint16 * numFaces)
{
  Uint16  i;
  CSL_FaceDetectFaceStatus *curFaceInfo;

  if (hndl == NULL || numFaces == NULL || faceList == NULL )
    return CSL_EFAIL;

  *numFaces = (Uint16) hndl->regs->FD_DNUM;

  curFaceInfo = (CSL_FaceDetectFaceStatus *)&hndl->regs->FD_CENTERX1;

  for(i=0; i<*numFaces; i++) {

    faceList->centerX = curFaceInfo->centerX;
    faceList->centerY = curFaceInfo->centerY;
    faceList->sizeConfidence = curFaceInfo->sizeConfidence;
    faceList->angle   = curFaceInfo->angle & CSL_FACE_DETECT_FD_ANGLE1_ANGLE1_MASK;

    faceList++;
    curFaceInfo++;
  }

  return CSL_SOK;
}
