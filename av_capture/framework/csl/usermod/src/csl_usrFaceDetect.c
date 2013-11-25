#include <csl_usrDrv.h>
#include <csl_faceDetectIoctl.h>

CSL_Status CSL_faceDetectOpen(CSL_FaceDetectHandle hndl)
{
  hndl->fd = CSL_usrDrvOpen(CSL_MODULE_FACE_DETECT);

  if (hndl->fd < 0)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_faceDetectClose(CSL_FaceDetectHandle hndl)
{
  return CSL_usrDrvClose(hndl->fd);
}

CSL_Status CSL_faceDetectHwSetup(CSL_FaceDetectHandle hndl, CSL_FaceDetectHwSetup * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_FACE_DETECT_CMD_HW_SETUP, data);
}

CSL_Status CSL_faceDetectDisable(CSL_FaceDetectHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_FACE_DETECT_CMD_DISABLE, NULL);
}

CSL_Status CSL_faceDetectGetStatus(CSL_FaceDetectHandle hndl, CSL_FaceDetectFaceStatus * faceList, Uint16 * numFaces)
{
  CSL_FaceDetectGetStatusPrm getStatusPrm;

  getStatusPrm.faceList = faceList;
  getStatusPrm.numFaces = numFaces;

  return CSL_usrDrvIoctl(hndl->fd, CSL_FACE_DETECT_CMD_GET_STATUS, &getStatusPrm);
}

CSL_Status CSL_faceDetectIntEnable(CSL_FaceDetectHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_FACE_DETECT_CMD_INT_ENABLE, (void *) enable);
}

CSL_Status CSL_faceDetectIntClear(CSL_FaceDetectHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_FACE_DETECT_CMD_INT_CLEAR, NULL);
}

CSL_Status CSL_faceDetectIntWait(CSL_FaceDetectHandle hndl, int timeout)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_FACE_DETECT_CMD_INT_WAIT, (void*)timeout);
}

CSL_Status CSL_faceDetectLock(CSL_FaceDetectHandle hndl, Uint32 timeout)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_LOCK, (void *) timeout);
}

CSL_Status CSL_faceDetectUnlock(CSL_FaceDetectHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_UNLOCK, NULL);
}
