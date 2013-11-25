
#ifndef _DRV_H3A_H_
#define _DRV_H3A_H_

#include <drv_imgs.h>
#include <osa_buf.h>

#define DRV_H3A_MOD_AF    (CSL_H3A_MOD_AF)
#define DRV_H3A_MOD_AEWB  (CSL_H3A_MOD_AEWB)
#define DRV_H3A_MOD_MAX   (CSL_H3A_MOD_MAX)

typedef struct {

  int sensorMode;

  int numBufAewb;
  int numBufAf;

  int aewbVendor;
} DRV_H3aConfig;

typedef struct {

  Uint32 afVfEnable;
  Uint16 afFvAccMode;
  Uint16 afNumWinH;
  Uint16 afNumWinV;

  Uint16 aewbOutFormat;
  Uint16 aewbNumWinH;
  Uint16 aewbNumWinV;
  Uint16 aewbNumSamplesPerColorInWin;

} DRV_H3aInfo;

int DRV_h3aOpen(DRV_H3aConfig *config);
int DRV_h3aClose();

int DRV_h3aEnable(int h3aMod, Bool enable);
int DRV_h3aSetParams(void);
int DRV_h3aCalcParams(void);

int DRV_h3aGetInfo(DRV_H3aInfo *info);

int DRV_h3aGetBuf(int h3aMod, int *bufId, int timeout);
int DRV_h3aPutBuf(int h3aMod, int bufId);

OSA_BufInfo *DRV_h3aGetBufInfo(int h3aMod, int bufId);

#endif

