
#ifndef _DRV_ISIF_H_
#define _DRV_ISIF_H_

#include <drv_imgs.h>
#include <osa_buf.h>

#define DRV_ISIF_INT_VD0  (CSL_CCDC_INT_TYPE_VD0)
#define DRV_ISIF_INT_VD1  (CSL_CCDC_INT_TYPE_VD1)
#define DRV_ISIF_INT_LSC  (CSL_CCDC_INT_TYPE_LSC)

typedef struct {

  Uint32  paxelWidth;         ///< Paxel Width, in pixels, valid values 8, 16, 32, 64, 128
  Uint32  paxelHeight;        ///< Paxel Width, in lines, valid values 8, 16, 32, 64, 128
  Uint32  gainFormat;         ///< Gain format 0..8, refer to register manual for details

  Bool32  offsetEnable;       ///< Offset control, TRUE: enable, FALSE: disable
  Uint32  offsetScale;        ///< Offset scale factor (U8Q7)
  Uint32  offsetShift;        ///< Offset left shift, 0..5 bit shift

  Uint32  initX;              ///< Init position X
  Uint32  initY;              ///< Init position Y

} DRV_IsifLscConfig;

typedef struct {

  int    sensorMode;
  int    vnfDemoCfg;
  Bool   alawEnable;
  Bool   dpcmEnable;

  int    numBuf;
  int    numLscBuf;
  
  DRV_IsifLscConfig lscConfig;

} DRV_IsifConfig;

typedef struct {

  Uint16 ddrOutDataWidth;
  Uint16 ddrOutDataHeight;
  Uint16 ddrOutDataOffsetH; // in bytes

  Uint16 lscDataWidth;
  Uint16 lscDataHeight;
  Uint16 lscDataOffsetH; // in bytes     
  Uint32 lscOffsetTableOffsetInBuf; // in bytes

} DRV_IsifInfo;

int DRV_isifOpen(DRV_IsifConfig *config);

int DRV_isifClose();

int DRV_isifGetInfo(DRV_IsifInfo *info);

int DRV_isifOutEnable(Bool enable);
int DRV_isifEnable(Bool enable);

int DRV_isifSetDcSub(Int16 dcSub);
int DRV_isifSetDgain(Uint16 gainR, Uint16 gainGb, Uint16 gainGr, Uint16 gainB, Uint16 offset);

int DRV_isifWaitInt(Uint16 intId, Uint16 numIntsToWait);

int DRV_isifGetOutBuf(int *bufId, int timeout);
int DRV_isifPutOutBuf(int bufId);

OSA_BufInfo *DRV_isifGetOutBufInfo(int bufId);

int DRV_isifLscEnable(Bool enable);

int DRV_isifLscGetBuf(int *bufId, int timeout);
int DRV_isifLscPutBuf(int bufId);

OSA_BufInfo *DRV_isifLscGetBufInfo(int bufId);

int DRV_isifGetBayerPhase(int sensorMode);

//int DRV_isifLock();
//int DRV_isifUnlock();

#endif
