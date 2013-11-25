

#ifndef _DRV_ISIF_PRIV_H_
#define _DRV_ISIF_PRIV_H_

#include <drv_isif.h>
#include <imageTuneCmdHandler.h>

typedef struct {

  DRV_IsifConfig config;
  int numBuf;
  OSA_BufInfo bufInfo[CSL_BUF_NUM_MAX];
  
  int numLscBuf;
  OSA_BufInfo lscBufInfo[CSL_BUF_NUM_MAX];
  
  DRV_IsifInfo info;
  
  DRV_ImgsModeConfig imgsModeInfo;
  
  DRV_ImgsIsifConfig *pImgsIsifConfig;
  
  Uint8 *lscGainTableVirtAddr;
  Uint8 *lscGainTablePhysAddr;
  Uint8 *lscOffsetTableVirtAddr;
  Uint8 *lscOffsetTablePhysAddr;
      
} DRV_IsifObj;

int DRV_isifSetParams();
int DRV_isifImageTuneCmdExecuteLsc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);

#endif /* _DRV_LDC_PRIV_H_  */
