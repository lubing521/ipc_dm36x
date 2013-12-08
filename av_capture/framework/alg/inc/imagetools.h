#ifndef _IMAGETOOLS_H_
#define _IMAGETOOLS_H_

#include <alg.h>
#include <drv_h3a.h>
#include "aewb_xdm.h"
#include "alg_aewb.h"

typedef struct
{
    int Nf2Value;
    int Nf2Value_adjust;
} ipipeNf2Prm;

typedef struct
{
    int ClampValue;
    int ClampValue_adjust;
} CcdcClampPrm;

typedef struct
{
    Bool32 DebugEnable; 
    ALG_AewbStatus AewbStatus;
    ALG_AewbRunPrm AewbRunPrm;
    AWB_PARAM awb_gain;
    ipipeNf2Prm ipipeNf2;
    CcdcClampPrm CcdcClamp; 
} ALG_AEWB_DEBUG;

enum 
{
    READ_MODE = 0,
    WRITE_MODE,
};

typedef struct
{
    int (*Sharpness)(int wr, int *value);
    int (*Contrast)(int wr, int *value);
    int (*Brightness)(int wr, int *value);
    int (*Saturation)(int wr, int *value);
    int (*NF2)(int wr, int *value);
    int (*Metering)(int wr, int *value);
    int (*AutoIRIS)(int wr, int *value);

    int (*SavePara)(void);
    int (*FactorySet)(void);
    
} TFC_IMAGE_TOOLS;

int SavePara(void);
int FactorySet(void);
int Sharpness(int wr, int *value);
int Contrast(int wr, int *value);
int Brightness(int wr, int *value);
int Saturation(int wr, int *value);
int NF2(int wr, int *value);
int Metering(int wr, int *value);
int AutoIRIS(int wr, int *value);
void TFC_ImageToolsInit(void);

extern ALG_AEWB_DEBUG gALG_AewbDebug;
extern TFC_IMAGE_TOOLS gTFC_Image_Tools;

#endif

