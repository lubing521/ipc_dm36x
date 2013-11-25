#include "alg_aewb_priv.h"
#include "alg_ti_aewb_priv.h"
#include "alg_ti_flicker_detect.h"
#include "ae_ti.h"
#include "awb_ti.h"
#include "TI_aewb.h"


int TI_fd_get_config(int sensorMode, int* row_time, int* pinp, int* h3aWinHeight)
{
    //return 0;
    int fd_res_support = 1;

    /* multi-resolution flicker detection support */
    if (sensorMode == DRV_IMGS_SENSOR_MODE_1920x1080)
    {
        /* 1080p */
        *row_time = 30;
        *pinp = 42;
        *h3aWinHeight = 34;
    }
    else
    {
        /* FD resolution not currently supported, turn off FD */
        fd_res_support = 0;
    }

    return fd_res_support;
}

#define FD_BRIGHTNESS_THRESHHOLD 	(8333.0*1000*1024) 	// threshold value to be crossed to trigger FD detection process

int TI_fd_trigger(IAEWB_Ae *curAe, IAEWB_Ae *nextAe)
{
    int fd_trigger = 0;
    float FD_brightness_cur;
    float FD_brightness_next;

    FD_brightness_cur = (float)curAe->exposureTime * curAe->sensorGain * curAe->ipipeGain;
    FD_brightness_next = (float)nextAe->exposureTime * nextAe->sensorGain * nextAe->ipipeGain;

    /* Trigger Flicker detection process based on brightness threshold being crossed */
    if(FD_brightness_next < FD_BRIGHTNESS_THRESHHOLD && FD_brightness_cur >= FD_BRIGHTNESS_THRESHHOLD)
    {
        fd_trigger = 1; //Go bright
    }

    return fd_trigger;
}

