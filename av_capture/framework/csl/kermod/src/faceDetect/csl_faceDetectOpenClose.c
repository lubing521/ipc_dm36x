/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_faceDetectIoctl.h>

CSL_IntRet_t CSL_faceDetectIsr(int intId, void *prm, void *reserved);

CSL_Status CSL_faceDetectOpen(CSL_FaceDetectHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  hndl->regs = CSL_sysGetBaseAddr(CSL_MODULE_FACE_DETECT);

  if (hndl->regs == NULL)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_faceDetectClose(CSL_FaceDetectHandle hndl)
{
  return CSL_SOK;
}

CSL_Status CSL_faceDetectInit(CSL_FaceDetectHandle hndl)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL)
    return CSL_EINVPARAMS;

  status |= CSL_sysFlagCreate(&hndl->intFlag);
  status |= CSL_faceDetectOpen(hndl);

  CSL_sysIntAttachIsr(CSL_SYS_INT_FD, CSL_faceDetectIsr, hndl);

  CSL_sysIntEnable(CSL_SYS_INT_FD, FALSE);

  return status;
}

CSL_Status CSL_faceDetectExit(CSL_FaceDetectHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  CSL_sysIntEnable(CSL_SYS_INT_FD, FALSE);

  CSL_sysIntAttachIsr(CSL_SYS_INT_FD, NULL, hndl);

  CSL_faceDetectClose(hndl);

  CSL_sysFlagDelete(&hndl->intFlag);



  return CSL_SOK;
}
