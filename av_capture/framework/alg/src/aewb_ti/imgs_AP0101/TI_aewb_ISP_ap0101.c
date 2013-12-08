
#include "alg_aewb_priv.h"
#include "TI_aewb.h"
#include "drv_ipipe.h"
//#include "TI_aewb_ISP.h"

static struct rgb2rgb_index rgb_matrixes_0[]= 
{
    {
        //U30-1080 (T=2830K)
        .color_temp = 2830,
        .rgb2rgbparam = {
            434, -191, 13,
            -112, 358, 10,
            -138, -451, 845,
            0, 0, 0
        },
        .rgb2rgb2param = {
            257, -8, 7,
            -3, 262, -4,
            -1, -13, 270,
            0, 0, 0
        }
    },
    {
        //CWF-1080  (T=4077K)
        .color_temp = 4077,
        .rgb2rgbparam = {
            507, -286,  35,
            -131, 401, -14,
            -68, -321, 644,
            0,     0,   0
        },
        .rgb2rgb2param = {
            258,  -10,  8,
            4,    247,  5,
            -3,     7,  253,
            0,     0,   0
        }
    },
    //DAY-1080 (T=5237K)
    {
        .color_temp = 5237,
        .rgb2rgbparam = {
            489, -312, 69,
            -89,  402, -57,
            -36, -289, 582,
            0,    0,   0
        },
        .rgb2rgb2param = {
            259, -1, -2,
            3, 259, -6,
            -4, -3, 263,
            0,  0,   0
        }
    },
    //end
    {
        .color_temp = 0,
    }
};

static struct rgb2rgb_index rgb_matrixes_1[] = 
{
    {
        .color_temp = 3000,
        .rgb2rgbparam = {
            256, 0, 0,
            0, 256, 0,
            0, 0, 256,
            0, 0, 0},
        .rgb2rgb2param = {
            256, 0, 0,
            0, 256, 0,
            0, 0, 256,
            0, 0, 0}
    },
    //end
    {
        .color_temp = 0,
    }
};

static struct rgb2rgb_index* rgb_matrixes[]= 
{
    rgb_matrixes_0, 
    rgb_matrixes_1,
};

static void ISP_config(int i)
{
}

static int getSetIndex(int eTime, int aGain, int dGain, int cTemp)
{
    if (aGain >= 8000 && dGain >= 2048)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int TI_2A_ISP_control(int eTime, int aGain, int dGain, int cTemp)
{
    static int steaySet = -1;
    static int tempSet = -1, tempCnt = 0;
    int tempCountLimit = 10;

    int i = getSetIndex(eTime, aGain, dGain, cTemp);
    int j = 0;

    if (steaySet == i) 
    {
        tempSet = -1;
        tempCnt = 0;
    }
    else if (tempSet == i && tempCnt >= tempCountLimit) 
    {
        steaySet = i; 
        tempSet = -1;
        tempCnt = 0;
        ISP_config(i);
    }
    else if (tempSet == i)
    {
        tempCnt++;
    }
    else
    {
        tempSet = i;
        tempCnt = 0;
    }
    return 100 * steaySet + j;
}

