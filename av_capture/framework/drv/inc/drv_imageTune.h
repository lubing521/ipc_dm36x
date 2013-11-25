
#ifndef _DRV_IMAGE_TUNE_H_
#define _DRV_IMAGE_TUNE_H_

#include <osa.h>
#include <osa_thr.h>
#include <imageTune.h>

#define DRV_IMAGE_TUNE_THR_PRI          (99)

int DRV_imageTuneInit();
int DRV_imageTuneExit();

Bool DRV_imageTuneIsSaveRawData();
Bool DRV_imageTuneIsSaveYuvData();
Bool DRV_imageTuneIsSendRawData();
Bool DRV_imageTuneIsSendYuvData();

int DRV_imageTuneSaveData(IMAGE_TUNE_SaveDataInfo *info);

int DRV_imageTuneLoadParams(int mode, int paramSetId);
int DRV_imageTuneLoadParamSet(int mode, int paramSetId);

int DRV_imageTuneSaveParams(int mode, int paramSetId, Bool saveDefaultParams);

int DRV_imageTuneSaveDataToFile(int format, int fileIndex);

#endif /* _DRV_IMAGE_TUNE_H_  */

