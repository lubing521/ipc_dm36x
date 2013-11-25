#include "alg_aewb_priv.h"
#include "alg_ti_aewb_priv.h"
#include "alg_ti_flicker_detect.h"
#include "ae_ti.h"
#include "awb_ti.h"
#include "TI_aewb.h"


int TI_fd_get_config(int sensorMode, int* row_time, int* pinp, int* h3aWinHeight)
{
    int fd_res_support = 0;
    return fd_res_support;
}

#define FD_BRIGHTNESS_THRESHHOLD 	(8333.0*1000*1024) 	// threshold value to be crossed to trigger FD detection process

int TI_fd_trigger(IAEWB_Ae *curAe, IAEWB_Ae *nextAe)
{
    int fd_trigger = 0;
    return fd_trigger;
}

