

#ifndef _DRV_IMAGE_TUNE_PRIV_H_
#define _DRV_IMAGE_TUNE_PRIV_H_

#include <drv_imageTune.h>
#include <imageTune.h>
#include <osa_flg.h>

#define DRV_IMAGE_TUNE_FILENAME_LEN_MAX   (256)

#define DRV_IMAGE_TUNE_THR_STACK_SIZE   (20*KB)

#define DRV_IMAGE_TUNE_DAVE_SAVE_FLAG_RAW       (0x1)
#define DRV_IMAGE_TUNE_DAVE_SAVE_FLAG_YUV        (0x2)

#define DRV_IMAGE_TUNE_DAVE_SEND_FLAG_RAW       (0x4)  //ANR - ETH
#define DRV_IMAGE_TUNE_DAVE_SEND_FLAG_YUV        (0x8)

typedef struct {

  Bool isRawDataSave;
  Bool isYuvDataSave;  
  Bool isRawDataSend;
  Bool isYuvDataSend;  
  char rawDataSaveFileName[DRV_IMAGE_TUNE_FILENAME_LEN_MAX];
  char yuvDataSaveFileName[DRV_IMAGE_TUNE_FILENAME_LEN_MAX];  
  
  OSA_FlgHndl    dataSaveFlg;
  
  OSA_ThrHndl thrHndl;

  IMAGE_TUNE_Params imgsDefaultPrm;

} DRV_ImageTuneObj;

int DRV_imageTuneCmdExecuteSaveRawData(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int DRV_imageTuneCmdExecuteSaveYuvData(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int DRV_imageTuneCmdExecuteSendRawData(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int DRV_imageTuneCmdExecuteSendYuvData(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);


#endif /* _DRV_IMAGE_TUNE_PRIV_H_ */
