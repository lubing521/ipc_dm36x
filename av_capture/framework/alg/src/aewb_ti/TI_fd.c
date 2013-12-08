#include "alg_aewb_priv.h"
#include "alg_ti_aewb_priv.h"
#include "alg_ti_flicker_detect.h"
#include "ae_ti.h"
#include "awb_ti.h"
#include "TI_aewb.h"

//#define FD_DEBUG

#define FD_FRAME_STEPS (5) //number of frames to wait between FD stages

int TI_fd_get_config(int sensorMode, int* row_time, int* pinp, int* h3aWinHeight);
int TI_fd_trigger(IAEWB_Ae *curAe, IAEWB_Ae *nextAe);

static int TI_fd_init(int h3a_w, int h3a_h, int row_time, int h3aWinHeight, int *fd_mem, int fd_mem_size)
{
    int w = h3a_w, h = h3a_h; //H3A AEWB has w x h windows
    //(1) first API call to get persistent memory size
    int ms = flicker_alloc(w, h);
#ifdef FD_DEBUG
    printf("FD debug: ms=%d, fd_mem=%d\n", ms, fd_mem_size);
#endif

    if (ms > fd_mem_size)
    {
        return 0;
    }

    //(2) second API call; fail if ret!=0
    int ret = flicker_init(fd_mem, w, h, row_time, h3aWinHeight, 100); //threshold default is 100
#ifdef FD_DEBUG
    printf("FD debug: %d, %d, %d, %d\n", w, h, row_time, h3aWinHeight);
#endif
    return(ret == 0);
}

//convert H3A RGB data into the luma image (int16) the FD algorithm needed
static void GETTING_RGB_BLOCK_VALUE_Y(unsigned short * BLOCK_DATA_ADDR, short *y, int shift,
                                      int aew_win_hz_cnt, int aew_win_vt_cnt)
{
    unsigned short i,j, numWin;
    Uint8 *curAewbAddr;
    CSL_H3aAewbOutSumModeOverlay *pAewbWinData;
    int r, g, b;
    short *py = y;

    curAewbAddr = (Uint8*)BLOCK_DATA_ADDR;
    numWin=0;

    for (i = 0; i < aew_win_vt_cnt; i++)
    {
        for (j = 0; j < aew_win_hz_cnt; j++)
        {
            pAewbWinData = (CSL_H3aAewbOutSumModeOverlay *)curAewbAddr;
            g = (pAewbWinData->subSampleAcc[0] + pAewbWinData->subSampleAcc[3]) >> (1+shift);
            r = pAewbWinData->subSampleAcc[1] >> shift;
            b = pAewbWinData->subSampleAcc[2] >> shift;
            *py++ = ((0x4D * r) + (0x96 * g) + (0x1D * b) + 128 ) / 256;

            curAewbAddr += sizeof(CSL_H3aAewbOutSumModeOverlay);

            numWin++;

            if (numWin % 8 == 0)
            {
                curAewbAddr += sizeof(CSL_H3aAewbOutUnsatBlkCntOverlay);
            }
        }
        curAewbAddr = (Uint8*)OSA_align((Uint32)curAewbAddr, 32);
    }
}

//return: -1 on going; otherwise as fd lib return value 
static int TI_fd_run(int16* h3aDataVirtAddr, int pinp, int* fd_mem, int exp, int gain, int h3a_w, int h3a_h)
{
    flicker_ret_st ret_st;
    int16 h3a[512]; //h3a luma image for FD algorithm
    int16 *pAddr = h3aDataVirtAddr;
    static int frame_cnt_fd = 0;
    int exp1, gain1;

    /* begin Flicker Detection process */
    if (frame_cnt_fd == 0)
    {
        //1st exposure
        exp1 = 10000 * ((exp + 5000) / 10000);
        if (exp1 <10000)
        {
            exp1 = 10000;
        }
        gain1 = gain * exp / exp1;

        /* Set sensor exposure time and analog gain for 1st stage FD */
#ifdef FD_DEBUG
        printf("\n\nFD debug exp0=%d, gain0=%d\n", exp, gain);
        printf("\n\nFD debug exp1=%d, gain1=%d\n", exp1, gain1);
#endif
        ALG_aewbSetSensorExposure(exp1);
        DRV_isifSetDgain(gain1/2, gain1/2, gain1/2, gain1/2, 0);
    }
    else if (frame_cnt_fd == FD_FRAME_STEPS)
    {
        //2nd exposure
        exp1 = 10000 * ((exp + 5000) / 10000);
        if (exp1 <10000)
        {
            exp1 = 10000;
        }
        exp1 -= 5000;
        gain1 = gain * exp / exp1;

        //(3) API call for detection
        /* Pass H3A buffer to data conversion function */
        GETTING_RGB_BLOCK_VALUE_Y(pAddr, h3a, 2, h3a_w, h3a_h);
        ret_st = flicker_detect(fd_mem, h3a, pinp, FLICKER_STATE_STAT);

        /* Set sensor exposure time and analog gain for 2nd stage FD (add 5 ms to 1st stage exposure time) */
#ifdef FD_DEBUG
        printf("\n\nFD debug exp2=%d, gain2=%d\n", exp1, gain1);
#endif
        ALG_aewbSetSensorExposure(exp1);
        DRV_isifSetDgain(gain1 / 2, gain1 / 2, gain1 / 2, gain1 / 2, 0);
    }
    else if (frame_cnt_fd == FD_FRAME_STEPS * 2)
    {
        //3rd exposure
        exp1 = 8333 * ((exp + 4167) / 8333);
        if (exp1 < 8333) 
        {
            exp1 = 8333;
        }
        gain1 = gain * exp / exp1;

        //(3) API call for detection
        /* Pass H3A buffer to data conversion function */
        GETTING_RGB_BLOCK_VALUE_Y(pAddr, h3a, 2, h3a_w, h3a_h);
        ret_st = flicker_detect(fd_mem, h3a, pinp, FLICKER_STATE_STAT);

        /* Set sensor exposure time and analog gain for 3rd stage FD */
#ifdef FD_DEBUG
        printf("\n\nFD debug exp3=%d, gain3=%d\n", exp1, gain1);
#endif
        ALG_aewbSetSensorExposure(exp1);
        DRV_isifSetDgain(gain1 / 2, gain1 / 2, gain1 / 2, gain1 / 2, 0);
    }
    else if (frame_cnt_fd == FD_FRAME_STEPS * 3)
    {
        //4th exposure
        exp1 = 8333 * ((exp + 4167) / 8333);
        if (exp1 <8333) 
        {
            exp1 = 8333;
        }
        exp1 -= 4167;
        gain1 = gain * exp / exp1;

        //(3) API call for detection
        /* Pass H3A buffer to data conversion function */
        GETTING_RGB_BLOCK_VALUE_Y(pAddr, h3a, 2, h3a_w, h3a_h);
        ret_st = flicker_detect(fd_mem, h3a, pinp, FLICKER_STATE_STAT);

        /* Set sensor exposure time and analog gain for 4th stage FD (add 4.166 ms to 3rd stage exposure time) */
#ifdef FD_DEBUG
        printf("\n\nFD debug exp4=%d, gain4=%d\n", exp1, gain1);
#endif
        ALG_aewbSetSensorExposure(exp1);
        DRV_isifSetDgain(gain1 / 2, gain1 / 2, gain1 / 2, gain1 / 2, 0);
    }
    else if (frame_cnt_fd == FD_FRAME_STEPS * 4)
    {
        //(3) API call for detection
        /* Pass H3A buffer to data conversion function */
        GETTING_RGB_BLOCK_VALUE_Y(pAddr, h3a, 2, h3a_w, h3a_h);
        ret_st = flicker_detect(fd_mem, h3a, pinp, FLICKER_STATE_CALC);

#ifdef FD_DEBUG
        printf("\n\nFD debug exp5=%d, gain5=%d\n", exp, gain);
#endif
        ALG_aewbSetSensorExposure(exp);
        DRV_isifSetDgain(gain / 2, gain / 2, gain / 2, gain / 2, 0);

#ifdef FD_DEBUG_MSG
        OSA_printf("\n ret_st=%d\n", ret_st);
#endif
        frame_cnt_fd = 0;
        return ret_st;
    }

    frame_cnt_fd++;
    return -1;
}

static int fd_trigger = 1;
static int fd_trigger_ret = 0;

int TI_fd_sequence(int sensorMode, void* h3aDataVirtAddr, 
                   int h3a_w, int h3a_h, int saldre, 
                   IAEWB_Ae *curAe, IAEWB_Ae *nextAe, int env_50_60Hz,
                   int *fd_mem, int fd_mem_size)
{
    if (env_50_60Hz != VIDEO_NTSC && 
        env_50_60Hz != VIDEO_PAL)
    {
        return 0;
    }

    if (fd_trigger == 0)
    {
        //int trigger = TI_fd_trigger(curAe, nextAe);
        int trigger = 0;
        if (trigger == 1)
        {
            fd_trigger = 1;
#ifdef FD_DEBUG
            printf("FD debug 0: trigger\n");
#endif
        }
    }

    static int row_time, pinp, h3aWinHeight;
    flicker_ret_st ret_st;
    if (fd_trigger == 1)
    {
        int res_support;
        fd_trigger = 0;
        res_support = gALG_aewbObj.algTiAEWB->fd_get_config(sensorMode, &row_time, &pinp, &h3aWinHeight);
        if (res_support)
        {
            int init = TI_fd_init(h3a_w, h3a_h, row_time, h3aWinHeight, fd_mem, fd_mem_size);
            if (init)
            {
                fd_trigger = 2;
            }
        }
#ifdef FD_DEBUG
//        printf("FD debug 1: res=%d, init=%d\n", res_support, init);
#endif
    }

    if (fd_trigger == 2)
    {
        static int stab_count = 0;
        /* Wait for 2A to stabailize to enable FD */
        if (nextAe->exposureTime == curAe->exposureTime && nextAe->sensorGain == curAe->sensorGain)
        {
            stab_count++;
            if (stab_count==5)
            {
                fd_trigger = 3;
                stab_count = 0;
            }
        }
        else
        {
            if (stab_count) stab_count = 0;
        }

#ifdef FD_DEBUG
        printf("FD debug 2: stab_count=%d\n", stab_count);
#endif
    }

    if (fd_trigger == 3)
    {
        int16* pAddr = h3aDataVirtAddr;
        int orig_exp = nextAe->exposureTime;
        int orig_gain = nextAe->ipipeGain;
        int ret = TI_fd_run(pAddr, pinp, fd_mem, orig_exp, orig_gain, h3a_w, h3a_h);
        if (ret >= 0)
        {
            ret_st = ret;
            fd_trigger = 4;
//#ifdef FD_DEBUG
            printf("FD debug 3: ret=%d\n", ret);
            char* info[] = {"none", "unknow", "50Hz", "60Hz"};
            printf("FD result: %s\n", info[ret]);
//#endif
            fd_trigger_ret = ret;
        }
    }

    if (fd_trigger == 4)
    {
        static int fd_count = 0;
        static int nf_count = 0;
        fd_count++;

        if (ret_st == 0)
        {
            nf_count++;
        }

        if (((ret_st == 2) && (env_50_60Hz == VIDEO_PAL))  ||
            ((ret_st == 3) && (env_50_60Hz == VIDEO_NTSC)))
        {
            fd_count = nf_count = fd_trigger = 0;
            TI_2A_config(ret_st, saldre);
#ifdef FD_DEBUG
            printf("FD debug 4: ret_st=%d, trigger=%d\n", ret_st, fd_trigger);
#endif
        }
        else if (fd_count == 5)
        {
            if (nf_count >= 3)
            {
                TI_2A_config(0, saldre);
            }
            else
            {
                TI_2A_config(1, saldre);
            }
#ifdef FD_DEBUG
            printf("FD debug 4: nf_count=%d, trigger=%d\n", nf_count, 0);
#endif
            fd_count = nf_count = fd_trigger = 0;
        }
        else
        {
            fd_trigger = 2;
            fd_trigger = 0; //Gang debug: detect only once
#ifdef FD_DEBUG
            printf("FD debug 4: trigger=%d\n", fd_trigger);
#endif
        }

    }

    return fd_trigger;
}

int TI_fd_enable()
{
    fd_trigger = 1;
    sleep(1);
    return fd_trigger_ret;
}

