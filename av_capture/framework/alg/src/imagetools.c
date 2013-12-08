#include "alg_aewb_priv.h"
#include "alg_ti_aewb_priv.h"
#include "alg_ti_flicker_detect.h"

#ifdef AR0331_WDR
#include "ae_tiar0331.h"
#include <drv_imgs.h>
#else
#include "ae_ti.h"
#endif
#include <drv_gpio.h>

#include "awb_ti.h"
#include "ae_appro.h"
#include "awb_appro.h"
#include "Appro_aewb.h"
#include "TI_aewb.h"
#include "imageTune.h"
#include "imageTuneCmdHandler.h"
#include "imagetools.h"

ALG_AEWB_DEBUG gALG_AewbDebug;
IMAGE_TOOLS gImage_Tools;

void ImageToolsInit(void)
{
    memset(&gImage_Tools, 0, sizeof(gImage_Tools)); 

    gImage_Tools.Sharpness   = Sharpness;
    gImage_Tools.Contrast    = Contrast;
    gImage_Tools.Brightness  = Brightness;
    gImage_Tools.Saturation  = Saturation;
    gImage_Tools.NF2         = NF2;
    gImage_Tools.Metering    = Metering;
    gImage_Tools.AutoIRIS    = AutoIRIS;
    gImage_Tools.SavePara    = SavePara;
    gImage_Tools.FactorySet  = FactorySet;
}

int SavePara(void)
{
    return 0;
}

int FactorySet(void)
{
    return 0;
}

//value : 0 - 100
int Sharpness(int wr, int *value)
{
    return 0;
}

//value : 0 - 100
int Contrast(int wr, int *value)
{
    return 0;
}

//value : 0 - 100
int Brightness(int wr, int *value)
{
    return 0;
}

//value : 0 - 100
int Saturation(int wr, int *value)
{
    return 0;
}

//value : 0 - 100
int NF2(int wr, int *value)
{
    return 0;
}

int Metering(int wr, int *value)
{

    return 0;
}

//value : 0  disable 1:enable
int AutoIRIS(int wr, int *value)
{

    return 0;
}

