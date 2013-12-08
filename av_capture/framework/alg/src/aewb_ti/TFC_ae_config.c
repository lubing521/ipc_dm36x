#include "alg_aewb_priv.h"
#include "alg_ti_aewb_priv.h"
#include "alg_ti_flicker_detect.h"
#include "ae_ti.h"
#include "awb_ti.h"
#include "TI_aewb.h"
#include "alg_aewb.h"

static void TFCAE_config_flicker_none(IAE_DynamicParams * aeDynamicParams, int min_step)
{
    int exposure_value = 36000;

    int i = 0;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = min_step;
    aeDynamicParams->exposureTimeRange[i].max = exposure_value; 
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 1000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 1024;

    i++;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = exposure_value;
    aeDynamicParams->exposureTimeRange[i].max = exposure_value;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 1000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;

    i++;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = exposure_value;
    aeDynamicParams->exposureTimeRange[i].max = exposure_value;
    aeDynamicParams->sensorGainRange[i].min = 2000;
    aeDynamicParams->sensorGainRange[i].max = 2000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;

    i++;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = exposure_value;
    aeDynamicParams->exposureTimeRange[i].max = exposure_value;
    aeDynamicParams->sensorGainRange[i].min = 4000;
    aeDynamicParams->sensorGainRange[i].max = 4000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;

    i++;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = exposure_value;
    aeDynamicParams->exposureTimeRange[i].max = exposure_value;
    aeDynamicParams->sensorGainRange[i].min = 6000;
    aeDynamicParams->sensorGainRange[i].max = 6000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 4096;
    
    i++;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = exposure_value;
    aeDynamicParams->exposureTimeRange[i].max = exposure_value;
    aeDynamicParams->sensorGainRange[i].min = 8000;
    aeDynamicParams->sensorGainRange[i].max = 8000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 4096;

    i++;
    aeDynamicParams->numRanges = i;
    aeDynamicParams->exposureTimeStepSize = 1;

    return;
}

static void TFCAE_config_flicker_yes(IAE_DynamicParams * aeDynamicParams, int min_step, int step_size)
{
    int i = 0;

#if 0
    if (ALG_aewbCheckAutoIris() > 0)
    {
        //adjust aperture Level 50%-100%
        for (i = 0; i <= 50; i++)
        {
            aeDynamicParams->apertureLevelRange[i].min = 50 + i;
            aeDynamicParams->apertureLevelRange[i].max = 50 + i;
            aeDynamicParams->exposureTimeRange[i].min = step_size;
            aeDynamicParams->exposureTimeRange[i].max = step_size;
            aeDynamicParams->sensorGainRange[i].min = 1000;
            aeDynamicParams->sensorGainRange[i].max = 1000;
            aeDynamicParams->ipipeGainRange[i].min = 1024;
            aeDynamicParams->ipipeGainRange[i].max = 1024;
        }
    }
    printf("\n------ALG_aewbCheckAutoIris =%d", ALG_aewbCheckAutoIris());
#endif

    if (min_step < step_size)
    {
        aeDynamicParams->apertureLevelRange[i].min = 100;
        aeDynamicParams->apertureLevelRange[i].max = 100;
        aeDynamicParams->exposureTimeRange[i].min = min_step;
        aeDynamicParams->exposureTimeRange[i].max = step_size;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 1000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 1024;
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
    }

    i++;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = step_size;
    aeDynamicParams->exposureTimeRange[i].max = step_size;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 1000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;

    i++;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = step_size * 2;
    aeDynamicParams->exposureTimeRange[i].max = step_size * 2;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 1000;
    aeDynamicParams->ipipeGainRange[i].min = 256;
    aeDynamicParams->ipipeGainRange[i].max = 2048;
    
    i++;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = step_size * 3;
    aeDynamicParams->exposureTimeRange[i].max = step_size * 3;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 1000;
    aeDynamicParams->ipipeGainRange[i].min = 256;
    aeDynamicParams->ipipeGainRange[i].max = 2048;
    
    i++;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = step_size * 3;
    aeDynamicParams->exposureTimeRange[i].max = step_size * 3;
    aeDynamicParams->sensorGainRange[i].min = 2000;
    aeDynamicParams->sensorGainRange[i].max = 2000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;
    
    i++;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = step_size * 3;
    aeDynamicParams->exposureTimeRange[i].max = step_size * 3;
    aeDynamicParams->sensorGainRange[i].min = 3000;
    aeDynamicParams->sensorGainRange[i].max = 3000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;
    
    i++;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = step_size * 3;
    aeDynamicParams->exposureTimeRange[i].max = step_size * 3;
    aeDynamicParams->sensorGainRange[i].min = 4000;
    aeDynamicParams->sensorGainRange[i].max = 4000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;

    if (step_size == 10000)
    {
        i++;
        aeDynamicParams->apertureLevelRange[i].min = 100;
        aeDynamicParams->apertureLevelRange[i].max = 100;
        aeDynamicParams->exposureTimeRange[i].min = step_size * 3;
        aeDynamicParams->exposureTimeRange[i].max = step_size * 3;
        aeDynamicParams->sensorGainRange[i].min = 6000;
        aeDynamicParams->sensorGainRange[i].max = 6000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 2048;
        
        i++;
        aeDynamicParams->apertureLevelRange[i].min = 100;
        aeDynamicParams->apertureLevelRange[i].max = 100;
        aeDynamicParams->exposureTimeRange[i].min = step_size * 3;
        aeDynamicParams->exposureTimeRange[i].max = step_size * 3;
        aeDynamicParams->sensorGainRange[i].min = 8000;
        aeDynamicParams->sensorGainRange[i].max = 8000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 4096;
    }
    else
    {   
        i++;
        aeDynamicParams->apertureLevelRange[i].min = 100;
        aeDynamicParams->apertureLevelRange[i].max = 100;
        aeDynamicParams->exposureTimeRange[i].min = step_size * 4;
        aeDynamicParams->exposureTimeRange[i].max = step_size * 4;
        aeDynamicParams->sensorGainRange[i].min = 4000;
        aeDynamicParams->sensorGainRange[i].max = 4000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 4096;

        i++;
        aeDynamicParams->apertureLevelRange[i].min = 100;
        aeDynamicParams->apertureLevelRange[i].max = 100;
        aeDynamicParams->exposureTimeRange[i].min = step_size * 4;
        aeDynamicParams->exposureTimeRange[i].max = step_size * 4;
        aeDynamicParams->sensorGainRange[i].min = 8000;
        aeDynamicParams->sensorGainRange[i].max = 8000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 4096;
    }
    
#if 0
    i++;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = step_size * 6;
    aeDynamicParams->exposureTimeRange[i].max = step_size * 6;
    aeDynamicParams->sensorGainRange[i].min = 8000;
    aeDynamicParams->sensorGainRange[i].max = 8000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 4096;
#endif

    i++;
    aeDynamicParams->numRanges = i;
    aeDynamicParams->exposureTimeStepSize = 1;
    
    return;
}

void TFC_2A_AE_config(IAE_DynamicParams *aeDynamicParams, int min_exp, int step_size)
{
    aeDynamicParams->size = sizeof(*aeDynamicParams);
    aeDynamicParams->targetBrightnessRange.min = 50;
    aeDynamicParams->targetBrightnessRange.max = 60;
    aeDynamicParams->targetBrightness = 55;
    aeDynamicParams->thrld = 5;

    if (min_exp == 8333 || min_exp == 10000)
    {
        TFCAE_config_flicker_yes(aeDynamicParams, min_exp, min_exp);
    }
    else if (step_size == 8333 || step_size == 10000) 
    {
        TFCAE_config_flicker_yes(aeDynamicParams, min_exp, step_size);
    }
    else if (step_size == 0) //no flicker at all
    {
        TFCAE_config_flicker_none(aeDynamicParams, min_exp);
    }
    else
    {
        TFCAE_config_flicker_yes(aeDynamicParams, min_exp, 8333);
    }
}

