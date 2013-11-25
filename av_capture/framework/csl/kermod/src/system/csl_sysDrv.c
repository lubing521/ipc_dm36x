/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_sysDrv.h>
#include <csl_sysIoctl.h>
#include <csl_dma.h>

CSL_VpssObj gCSL_drvVpssObj;
CSL_RszObj gCSL_drvRszObj;
CSL_IpipeObj gCSL_drvIpipeObj;
CSL_CcdcObj gCSL_drvCcdcObj;
CSL_IpipeifObj gCSL_drvIpipeifObj;
CSL_H3aObj gCSL_drvH3aObj;
CSL_LdcObj gCSL_drvLdcObj;
CSL_FaceDetectObj gCSL_drvFaceDetectObj;
CSL_GpioObj gCSL_drvGpioObj;

CSL_IntcRegsOvly gCSL_drvIntcRegs = CSL_INTC_REGS;

CSL_SysDrvObj gCSL_sysDrvObjList[CSL_MODULE_MAX] = {
  // CSL_MODULE_ISP5
  {
   (void *) CSL_ISP5_0_REGS,

   NULL,

   NULL,
   NULL,
   NULL,

   {NULL},
   },
  // CSL_MODULE_VPSS
  {
   (void *) CSL_VPSS_0_REGS,

   &gCSL_drvVpssObj,

   (CSL_DrvFunc) CSL_vpssInit,
   (CSL_DrvFunc) CSL_vpssExit,
   (CSL_DrvFuncIoctl) CSL_vpssHwControl,

   {NULL},
   },
  // CSL_MODULE_RSZ
  {
   (void *) CSL_RSZ_0_REGS,

   &gCSL_drvRszObj,

   (CSL_DrvFunc) CSL_rszInit,
   (CSL_DrvFunc) CSL_rszExit,
   (CSL_DrvFuncIoctl) CSL_rszHwControl,

   {NULL},
   },
  // CSL_MODULE_IPIPE
  {
   (void *) CSL_IPIPE_0_REGS,

   &gCSL_drvIpipeObj,

   (CSL_DrvFunc) CSL_ipipeInit,
   (CSL_DrvFunc) CSL_ipipeExit,
   (CSL_DrvFuncIoctl) CSL_ipipeHwControl,

   {NULL},
   },
  // CSL_MODULE_ISIF
  {
   (void *) CSL_ISIF_0_REGS,

   &gCSL_drvCcdcObj,

   (CSL_DrvFunc) CSL_ccdcInit,
   (CSL_DrvFunc) CSL_ccdcExit,
   (CSL_DrvFuncIoctl) CSL_ccdcHwControl,

   {NULL},
   },
  // CSL_MODULE_IPIPEIF
  {
   (void *) CSL_IPIPEIF_0_REGS,

   &gCSL_drvIpipeifObj,

   (CSL_DrvFunc) CSL_ipipeifInit,
   (CSL_DrvFunc) CSL_ipipeifExit,
   (CSL_DrvFuncIoctl) CSL_ipipeifHwControl,

   {NULL},
   },
  // CSL_MODULE_H3A
  {
   (void *) CSL_H3A_0_REGS,

   &gCSL_drvH3aObj,

   (CSL_DrvFunc) CSL_h3aInit,
   (CSL_DrvFunc) CSL_h3aExit,
   (CSL_DrvFuncIoctl) CSL_h3aHwControl,

   {NULL},
   },
  // CSL_MODULE_LDC
  {
   (void *) CSL_LDC_0_REGS,

   &gCSL_drvLdcObj,

   (CSL_DrvFunc) CSL_ldcInit,
   (CSL_DrvFunc) CSL_ldcExit,
   (CSL_DrvFuncIoctl) CSL_ldcHwControl,

   {NULL},
   },
  // CSL_MODULE_FACE_DETECT
  {
   (void *) CSL_FACE_DETECT_0_REGS,

   &gCSL_drvFaceDetectObj,

   (CSL_DrvFunc) CSL_faceDetectInit,
   (CSL_DrvFunc) CSL_faceDetectExit,
   (CSL_DrvFuncIoctl) CSL_faceDetectHwControl,

   {NULL},
   },
  // CSL_MODULE_GPIO
  {
   (void *) CSL_GPIO_REGS,

   &gCSL_drvGpioObj,

   (CSL_DrvFunc) CSL_gpioInit,
   (CSL_DrvFunc) CSL_gpioExit,
   (CSL_DrvFuncIoctl) CSL_gpioHwControl,

   {NULL},
   },   
};

CSL_Status CSL_sysDrvInit()
{
  CSL_Status status = CSL_SOK;
  int     i;
  CSL_SysDrvObj *pDrvObj;

  for (i = 0; i < CSL_MODULE_MAX; i++) {

    pDrvObj = &gCSL_sysDrvObjList[i];

    if (pDrvObj->pModObj != NULL) {

      status |= CSL_sysMutexCreate(&pDrvObj->mutex);

      if (pDrvObj->modInit != NULL)
        status |= pDrvObj->modInit(pDrvObj->pModObj);
    }
  }
  
  status |= CSL_dmaInit();

  return status;
}

CSL_Status CSL_sysDrvExit()
{
  CSL_Status status = CSL_SOK;
  int     i;
  CSL_SysDrvObj *pDrvObj;

  for (i = 0; i < CSL_MODULE_MAX; i++) {

    pDrvObj = &gCSL_sysDrvObjList[i];

    if (pDrvObj->pModObj != NULL) {
      if (pDrvObj->modExit != NULL)
        status |= pDrvObj->modExit(pDrvObj->pModObj);

      status |= CSL_sysMutexDelete(&pDrvObj->mutex);
    }
  }

  status |= CSL_dmaExit();

  return status;
}

CSL_Status CSL_sysDrvOpen(Uint32 moduleId)
{
  CSL_Status status = CSL_SOK;
  CSL_SysDrvObj *pDrvObj;

  if (moduleId >= CSL_MODULE_MAX)
    return CSL_EFAIL;

  pDrvObj = &gCSL_sysDrvObjList[moduleId];

  if (pDrvObj->pModObj == NULL)
    return CSL_EFAIL;

  return status;
}

CSL_Status CSL_sysDrvIoctl(Uint32 moduleId, Uint32 cmdId, void *prm)
{
  CSL_Status status = CSL_SOK;
  CSL_SysDrvObj *pDrvObj;

  if (moduleId >= CSL_MODULE_MAX)
    return CSL_EFAIL;

  pDrvObj = &gCSL_sysDrvObjList[moduleId];

  if (pDrvObj->pModObj == NULL)
    return CSL_EFAIL;

  if (!CSL_IOCTL_CMD_IS_VALID(cmdId))
    return CSL_EFAIL;

  cmdId = CSL_IOCTL_CMD_GET(cmdId);

  if (cmdId == CSL_SYS_CMD_LOCK) {
    status = CSL_sysMutexLock(&pDrvObj->mutex, (Uint32) prm);
  } else if (cmdId == CSL_SYS_CMD_UNLOCK) {
    status = CSL_sysMutexUnlock(&pDrvObj->mutex);
  } else {
    if (pDrvObj->modIoctl != NULL)
      status = pDrvObj->modIoctl(pDrvObj->pModObj, cmdId, prm);
  }

  return status;
}

CSL_Status CSL_sysDrvClose(Uint32 moduleId)
{
  CSL_Status status = CSL_SOK;
  CSL_SysDrvObj *pDrvObj;

  if (moduleId >= CSL_MODULE_MAX)
    return CSL_EFAIL;

  pDrvObj = &gCSL_sysDrvObjList[moduleId];

  if (pDrvObj->pModObj == NULL)
    return CSL_EFAIL;

  return status;
}

void   *CSL_sysGetBaseAddr(Uint16 moduleId)
{
  if (moduleId >= CSL_MODULE_MAX)
    return NULL;

  return gCSL_sysDrvObjList[moduleId].regBaseAddr;
}
