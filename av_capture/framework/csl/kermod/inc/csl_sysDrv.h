/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#ifndef _CSL_SYS_DRV_H
#define _CSL_SYS_DRV_H


#include <csl_sys.h>
#include <csl_ccdcIoctl.h>
#include <csl_ipipeifIoctl.h>
#include <csl_ipipeIoctl.h>
#include <csl_rszIoctl.h>
#include <csl_ldcIoctl.h>
#include <csl_vpssIoctl.h>
#include <csl_faceDetectIoctl.h>
#include <csl_h3aIoctl.h>
#include <csl_gpioIoctl.h>

typedef CSL_Status(*CSL_DrvFunc) (void *hndl);
typedef CSL_Status(*CSL_DrvFuncIoctl) (void *hndl, Uint32 cmdId, void *prm);

typedef struct {

  void   *regBaseAddr;
  void   *pModObj;

  CSL_DrvFunc modInit;
  CSL_DrvFunc modExit;
  CSL_DrvFuncIoctl modIoctl;

  CSL_SysMutex mutex;

} CSL_SysDrvObj;


extern CSL_VpssObj gCSL_drvVpssObj;
extern CSL_RszObj gCSL_drvRszObj;
extern CSL_IpipeObj gCSL_drvIpipeObj;
extern CSL_CcdcObj gCSL_drvCcdcObj;
extern CSL_IpipeifObj gCSL_drvIpipeifObj;
extern CSL_H3aObj gCSL_drvH3aObj;
extern CSL_LdcObj gCSL_drvLdcObj;
extern CSL_FaceDetectObj gCSL_drvFaceDetectObj;
extern CSL_GpioObj gCSL_drvGpioObj;
extern CSL_IntcRegsOvly gCSL_drvIntcRegs;

CSL_Status CSL_sysDrvInit(void);
CSL_Status CSL_sysDrvExit(void);

CSL_Status CSL_sysDrvOpen(Uint32 moduleId);
CSL_Status CSL_sysDrvIoctl(Uint32 moduleId, Uint32 cmdId, void *prm);
CSL_Status CSL_sysDrvClose(Uint32 moduleId);


#endif
