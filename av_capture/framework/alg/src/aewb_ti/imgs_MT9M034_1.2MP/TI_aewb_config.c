#include "alg_aewb_priv.h"
#include "alg_ti_aewb_priv.h"
#include "alg_ti_flicker_detect.h"
#include "ae_ti.h"
#include "awb_ti.h"
#include "TI_aewb.h"

static void TIAE_config_flicker_none(IAE_DynamicParams * aeDynamicParams, int min_step)
{
    int i = 0;

    aeDynamicParams->exposureTimeRange[i].min = min_step;
    aeDynamicParams->exposureTimeRange[i].max = 33333; 
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 1000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 1024;
    i++;

    aeDynamicParams->exposureTimeRange[i].min = 33333;
    aeDynamicParams->exposureTimeRange[i].max = 33333;
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 1024;
    aeDynamicParams->sensorGainRange[i].max = 1024;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;
    i++;

    aeDynamicParams->exposureTimeRange[i].min = 33333;
    aeDynamicParams->exposureTimeRange[i].max = 33333;
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 2000;
    aeDynamicParams->sensorGainRange[i].max = 2000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;
    i++;

    aeDynamicParams->exposureTimeRange[i].min = 33333;
    aeDynamicParams->exposureTimeRange[i].max = 33333;
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 4000;
    aeDynamicParams->sensorGainRange[i].max = 4000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;
    i++;

    aeDynamicParams->exposureTimeRange[i].min = 33333;
    aeDynamicParams->exposureTimeRange[i].max = 33333;
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 8000;
    aeDynamicParams->sensorGainRange[i].max = 8000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 4096;
    i++;

    aeDynamicParams->numRanges = i;
    aeDynamicParams->exposureTimeStepSize = 1;

    return;
}

static void TIAE_config_flicker_yes(IAE_DynamicParams * aeDynamicParams, int min_step, int step_size)
{
    int i = 0;

    if (min_step < step_size)
    {
        aeDynamicParams->exposureTimeRange[i].min = min_step;
        aeDynamicParams->exposureTimeRange[i].max = step_size;
        aeDynamicParams->apertureLevelRange[i].min = 1;
        aeDynamicParams->apertureLevelRange[i].max = 1;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 1000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 1024;
        i++;
    }
    else
    {
        aeDynamicParams->exposureTimeRange[i].min = step_size;
        aeDynamicParams->exposureTimeRange[i].max = step_size;
        aeDynamicParams->apertureLevelRange[i].min = 1;
        aeDynamicParams->apertureLevelRange[i].max = 1;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 1000;
        aeDynamicParams->ipipeGainRange[i].min = 256;
        aeDynamicParams->ipipeGainRange[i].max = 1024;
        i++;
    }
    aeDynamicParams->exposureTimeRange[i].min = step_size;
    aeDynamicParams->exposureTimeRange[i].max = step_size;
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 1000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;
    i++;

    aeDynamicParams->exposureTimeRange[i].min = step_size * 2;
    aeDynamicParams->exposureTimeRange[i].max = step_size * 2;
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 1000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 1536;
    i++;

    int k = 3;
    if (step_size == 8333)
    {
        aeDynamicParams->exposureTimeRange[i].min = step_size * 3;
        aeDynamicParams->exposureTimeRange[i].max = step_size * 3;
        aeDynamicParams->apertureLevelRange[i].min = 1;
        aeDynamicParams->apertureLevelRange[i].max = 1;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 1000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 1365;
        i++;
        k = 4;
    }

    aeDynamicParams->exposureTimeRange[i].min = step_size * k;
    aeDynamicParams->exposureTimeRange[i].max = step_size * k;
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 1000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;
    i++;
    aeDynamicParams->exposureTimeRange[i].min = step_size * k;
    aeDynamicParams->exposureTimeRange[i].max = step_size * k;
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 2000;
    aeDynamicParams->sensorGainRange[i].max = 2000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;
    i++;
    aeDynamicParams->exposureTimeRange[i].min = step_size * k;
    aeDynamicParams->exposureTimeRange[i].max = step_size * k;
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 4000;
    aeDynamicParams->sensorGainRange[i].max = 4000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;
    i++;
    aeDynamicParams->exposureTimeRange[i].min = step_size * k;
    aeDynamicParams->exposureTimeRange[i].max = step_size * k;
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 8000;
    aeDynamicParams->sensorGainRange[i].max = 8000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 4096;
    i++;

    aeDynamicParams->numRanges = i;
    aeDynamicParams->exposureTimeStepSize = 1;
    return;
}


void TI_2A_AE_config(IAE_DynamicParams * aeDynamicParams, int min_exp, int step_size)
{

    aeDynamicParams->size = sizeof(*aeDynamicParams);
    aeDynamicParams->targetBrightnessRange.min = 35;
    aeDynamicParams->targetBrightnessRange.max = 45;
    aeDynamicParams->targetBrightness = 40;
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

