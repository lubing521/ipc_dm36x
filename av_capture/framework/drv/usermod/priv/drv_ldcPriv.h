

#ifndef _DRV_LDC_PRIV_H_
#define _DRV_LDC_PRIV_H_

#include <drv_ldc.h>
#include <imageTuneCmdHandler.h>

typedef struct {
  
  DRV_ImgsLdcConfig ldcPrm;
  
  OSA_MutexHndl lock;

} DRV_LdcObj;

int DRV_ldcImageTuneCmdExecuteLdc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);

#endif /* _DRV_LDC_PRIV_H_  */
