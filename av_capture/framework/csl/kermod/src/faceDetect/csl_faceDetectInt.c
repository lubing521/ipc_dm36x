/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_faceDetect.h>
#include <csl_sysDrv.h>

CSL_IntRet_t CSL_faceDetectIsr(int intId, void *prm, void *reserved)
{
  CSL_FaceDetectHandle hndl = &gCSL_drvFaceDetectObj;

  CSL_sysFlagSet(&hndl->intFlag);

  return CSL_INT_RET_SOK;
}

CSL_Status CSL_faceDetectIntEnable(CSL_FaceDetectHandle hndl, Bool32 enable)
{
  CSL_Status status;

  if (hndl == NULL)
    return CSL_EFAIL;

  status = CSL_sysIntEnable(CSL_SYS_INT_FD, enable);

  return status;
}

CSL_Status CSL_faceDetectIntClear(CSL_FaceDetectHandle hndl)
{
  CSL_Status status;

  if (hndl == NULL)
    return CSL_EFAIL;

  CSL_sysIntClear(CSL_SYS_INT_FD);

  status = CSL_sysFlagClear(&hndl->intFlag);

  return status;
}

CSL_Status CSL_faceDetectIntWait(CSL_FaceDetectHandle hndl, int timeout)
{
  CSL_Status status;

  if (hndl == NULL)
    return CSL_EFAIL;

  status = CSL_sysFlagWait(&hndl->intFlag, timeout);

  return status;
}
