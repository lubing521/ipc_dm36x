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
TFC_IMAGE_TOOLS gTFC_Image_Tools;

void TFC_ImageToolsInit(void)
{
    memset(&gTFC_Image_Tools, 0, sizeof(gTFC_Image_Tools)); 

    gTFC_Image_Tools.Sharpness   = Sharpness;
    gTFC_Image_Tools.Contrast    = Contrast;
    gTFC_Image_Tools.Brightness  = Brightness;
    gTFC_Image_Tools.Saturation  = Saturation;
    gTFC_Image_Tools.NF2         = NF2;
    gTFC_Image_Tools.Metering    = Metering;
    gTFC_Image_Tools.AutoIRIS    = AutoIRIS;
    gTFC_Image_Tools.SavePara    = SavePara;
    gTFC_Image_Tools.FactorySet  = FactorySet;
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

