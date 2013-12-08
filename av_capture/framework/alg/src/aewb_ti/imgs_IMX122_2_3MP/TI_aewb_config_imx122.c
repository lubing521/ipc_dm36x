#include "alg_aewb_priv.h"
#include "alg_ti_aewb_priv.h"
#include "alg_ti_flicker_detect.h"
#include "ae_ti.h"
#include "awb_ti.h"
#include "TI_aewb.h"

static void TIAE_config_flicker_none(IAE_DynamicParams * aeDynamicParams, int min_exp)
{
    int i = 0;
    aeDynamicParams->exposureTimeRange[i].min = min_exp;
    aeDynamicParams->exposureTimeRange[i].max = 33333;
    aeDynamicParams->apertureLevelRange[i].min = 255;
    aeDynamicParams->apertureLevelRange[i].max = 255;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 125000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 8191;
    i++;

    aeDynamicParams->numRanges = i;
    aeDynamicParams->exposureTimeStepSize = min_exp;

    return;
}

static void TIAE_config_flicker_yes(IAE_DynamicParams * aeDynamicParams, int min_step, int step_size)
{
    int i = 0;
    
    //printf("----ALG_aewbCheckAutoIris = %d\n", ALG_aewbCheckAutoIris());
    if (ALG_aewbCheckAutoIris() > 0)
    {
        //adjust aperture Level 10%-100%
        for (i = 0; i < 90; i++)
        {
            aeDynamicParams->apertureLevelRange[i].min = 10 + i;
            aeDynamicParams->apertureLevelRange[i].max = 10 + i;
            aeDynamicParams->exposureTimeRange[i].min = step_size;
            aeDynamicParams->exposureTimeRange[i].max = step_size;
            aeDynamicParams->sensorGainRange[i].min = 1000;
            aeDynamicParams->sensorGainRange[i].max = 1000;
            aeDynamicParams->ipipeGainRange[i].min = 1024;
            aeDynamicParams->ipipeGainRange[i].max = 1024;
        }
    }

    if (ALG_aewbGetBLC() > 0)
    {
        aeDynamicParams->apertureLevelRange[i].min = 100;
        aeDynamicParams->apertureLevelRange[i].max = 100;
        aeDynamicParams->exposureTimeRange[i].min = step_size;
        aeDynamicParams->exposureTimeRange[i].max = step_size;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 1000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 1024;
        i++;
    }
    else if (min_step < step_size)
    {
        aeDynamicParams->apertureLevelRange[i].min = 100;
        aeDynamicParams->apertureLevelRange[i].max = 100;
        aeDynamicParams->exposureTimeRange[i].min = min_step;
        aeDynamicParams->exposureTimeRange[i].max = step_size;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 1000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 1024;
        i++;
    }
    else
    {
        aeDynamicParams->apertureLevelRange[i].min = 100;
        aeDynamicParams->apertureLevelRange[i].max = 100;
        aeDynamicParams->exposureTimeRange[i].min = step_size;
        aeDynamicParams->exposureTimeRange[i].max = step_size;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 1000;
        aeDynamicParams->ipipeGainRange[i].min = 256;
        aeDynamicParams->ipipeGainRange[i].max = 1024;
        i++;
    }
    
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = step_size * 1;
    aeDynamicParams->exposureTimeRange[i].max = step_size * 1;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 2000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 1024;
    i++;

    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = step_size * 2;
    aeDynamicParams->exposureTimeRange[i].max = step_size * 2;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 1500;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 1024;
    i++;

    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = step_size * 3;
    aeDynamicParams->exposureTimeRange[i].max = step_size * 3;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 20000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 1280;
    i++;
   
    if (step_size == 8333) //60Hz
    {   
        aeDynamicParams->apertureLevelRange[i].min = 100;
        aeDynamicParams->apertureLevelRange[i].max = 100;
        aeDynamicParams->exposureTimeRange[i].min = step_size * 4;
        aeDynamicParams->exposureTimeRange[i].max = step_size * 4;
        aeDynamicParams->sensorGainRange[i].min = 10000;
        aeDynamicParams->sensorGainRange[i].max = 50000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 2048;
        i++;
    }
    else if (step_size == 10000) //50 Hz
    {  
        aeDynamicParams->apertureLevelRange[i].min = 100;
        aeDynamicParams->apertureLevelRange[i].max = 100;
        aeDynamicParams->exposureTimeRange[i].min = step_size * 3 + 3333;
        aeDynamicParams->exposureTimeRange[i].max = step_size * 3 + 3333;
        aeDynamicParams->sensorGainRange[i].min = 10000;
        aeDynamicParams->sensorGainRange[i].max = 50000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 2048;
        i++;
    }

    aeDynamicParams->numRanges = i;
    aeDynamicParams->exposureTimeStepSize = 1;
    return;
}

void TI_2A_AE_config_imx122(IAE_DynamicParams * aeDynamicParams, int min_exp, int step_size)
{
    aeDynamicParams->size = sizeof(*aeDynamicParams);
    aeDynamicParams->targetBrightnessRange.min = 35;
    aeDynamicParams->targetBrightnessRange.max = 45;
    aeDynamicParams->targetBrightness          = 40;
    aeDynamicParams->thrld = 5;

    if (min_exp == 8333 || min_exp == 10000)
    {
        TIAE_config_flicker_yes(aeDynamicParams, min_exp, min_exp);
    }
    else if (step_size == 8333 || step_size == 10000) 
    {
        TIAE_config_flicker_yes(aeDynamicParams, min_exp, step_size);
    }
    else if (step_size == 0) //no flicker at all
    {
        TIAE_config_flicker_none(aeDynamicParams, min_exp);
    }
    else
    {
        TIAE_config_flicker_yes(aeDynamicParams, min_exp, 8333);
    }
}

