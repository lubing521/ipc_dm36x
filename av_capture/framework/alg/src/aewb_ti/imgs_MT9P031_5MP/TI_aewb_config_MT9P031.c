#include "alg_aewb_priv.h"
#include "alg_ti_aewb_priv.h"
#include "alg_ti_flicker_detect.h"
#include "ae_ti.h"
#include "awb_ti.h"
#include "TI_aewb.h"
#include "alg_aewb.h"

static void TIAE_config_flicker_none(IAE_DynamicParams * aeDynamicParams, int min_step)
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

#if 0
static void TIAE_config_flicker_yes(IAE_DynamicParams * aeDynamicParams, int min_step, int step_size)
{
    int i = 0;

#if 1
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
    //printf("----ALG_aewbCheckAutoIris = %d\n", ALG_aewbCheckAutoIris());
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
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;
    
    i++;
    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = step_size * 3;
    aeDynamicParams->exposureTimeRange[i].max = step_size * 3;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 1000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
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
        aeDynamicParams->ipipeGainRange[i].max = 1024;

        i++;
        aeDynamicParams->apertureLevelRange[i].min = 100;
        aeDynamicParams->apertureLevelRange[i].max = 100;
        aeDynamicParams->exposureTimeRange[i].min = step_size * 4;
        aeDynamicParams->exposureTimeRange[i].max = step_size * 4;
        aeDynamicParams->sensorGainRange[i].min = 8000;
        aeDynamicParams->sensorGainRange[i].max = 8000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 1024;
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
#else
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
    aeDynamicParams->sensorGainRange[i].max = 2000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 1024;
    i++;

    aeDynamicParams->apertureLevelRange[i].min = 100;
    aeDynamicParams->apertureLevelRange[i].max = 100;
    aeDynamicParams->exposureTimeRange[i].min = step_size * 3;
    aeDynamicParams->exposureTimeRange[i].max = step_size * 3;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 4000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 2048;
    i++;
   
    if (step_size == 8333) //60Hz
    {   
        aeDynamicParams->apertureLevelRange[i].min = 100;
        aeDynamicParams->apertureLevelRange[i].max = 100;
        aeDynamicParams->exposureTimeRange[i].min = step_size * 4;
        aeDynamicParams->exposureTimeRange[i].max = step_size * 4;
        aeDynamicParams->sensorGainRange[i].min = 4000;
        aeDynamicParams->sensorGainRange[i].max = 8000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 4096;
        i++;
    }
    else if (step_size == 10000) //50 Hz
    {  
        aeDynamicParams->apertureLevelRange[i].min = 100;
        aeDynamicParams->apertureLevelRange[i].max = 100;
        aeDynamicParams->exposureTimeRange[i].min = step_size * 3 + 8333;
        aeDynamicParams->exposureTimeRange[i].max = step_size * 3 + 8333;
        aeDynamicParams->sensorGainRange[i].min = 4000;
        aeDynamicParams->sensorGainRange[i].max = 8000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 4096;
        i++;
    }

    aeDynamicParams->numRanges = i;
    aeDynamicParams->exposureTimeStepSize = 1;
    return;
}

#endif

void TI_2A_AE_config_MT9P031(IAE_DynamicParams *aeDynamicParams, int min_exp, int step_size)
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

