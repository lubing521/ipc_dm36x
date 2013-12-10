#include "alg_aewb_priv.h"
#include "TI_aewb.h"
#include "drv_ipipe.h"
#include "TI_aewb_ISP.h"
#include "alg_aewb.h"

static struct rgb2rgb_index rgb_matrixes_0[]= 
{
    //U30 (T=2830K)
    {
        .color_temp = 2000,//2830,
        .rgb2rgbparam = {
            382,  -65, -61,
            -95,  353, -2,
            -77, -439, 772,
            0,    0,   0
        },
        .rgb2rgb2param = {
            251,   -6,  11,
            -6,  263,  -1,
            -13,  -11, 280,
            0,    0,   0
        }
    },
    //TL84 (T=4000K) 
    {
        .color_temp = 4000,
        .rgb2rgbparam = {
            395,  -152,  12,
            -78,   379, -45,
            -41,  -257, 554,
            0,     0,    0
        },
        .rgb2rgb2param = {
            251,    -7,   12,
            1,   256,   -2,
            -6,   -10,  271,
            0,     0,    0
        }
    },
    //D50 (T=5237K)
    {
        .color_temp = 5237,
        .rgb2rgbparam = {
            407, -181,  30,
            -72,  354, -25,
            -24, -213, 493,
            0,    0,   0
        },
        .rgb2rgb2param = {
            260,  -15,   11,
            -1,  259,   -2,
            -2,  -12,  270,
            0,    0,    0
        }
    },
    //D65 (T=6500K)
    {
        .color_temp = 6500,
        .rgb2rgbparam = {
            455, -210,  10,
            -75,  364, -33,
            -20, -205, 481,
            0,    0,   0
        },
        .rgb2rgb2param = {
            256,  -13,   13,
            -2,  259,   -1,
            -5,  -9,   271,
            0,    0,    0
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
    rgb_matrixes_1,
};

static void ISP_ipipeSetNf2(int aGain, int dGain)
{
    static int value_bak = -1;
    static CSL_IpipeNfConfig nf2 =
    {
    .enable             = 1,
    .spreadVal          = 3,
    .spreadValSrc       = 0,
    .lutAddrShift       = 0,
    .greenSampleMethod  = 0,
    .lscGainEnable      = 0,
    .edgeDetectThresMin = 0,
    .edgeDetectThresMax = 2047,
    .lutThresTable      = {20, 20, 20, 20, 20, 20, 20, 20},
    .lutIntensityTable  = {31, 31, 31, 31, 31, 31, 31, 31},
    .lutSpreadTable     = { 0,  0,  0,  0,  0,  0,  0,  0},
    };
    
    int ag = (aGain > 10000) ? 10000 : aGain;
    int dg = (dGain >  2048) ?  2048 : dGain;
    int value = 0;
    
    if (ag < 2000)
    {
        value = 20;
    }
    else
    {
        value = 20 + (ag - 2000) / 75 + (dg - 1000) / 10;
    }

    value += gALG_AewbDebug.ipipeNf2.Nf2Value_adjust + ALG_aewbGetNoiseLevel();
    value = (value > 200) ? 200 : value;
    value = (value <  20) ?  20 : value;
    
    if (value == value_bak)
    {
        return;
    }
    value_bak = value;
    
    int i;
    for (i = 0; i < 8; i++)
    {
        nf2.lutThresTable[i] = (Uint32)value + (Uint32)(i * value / 10);
    }

    gALG_AewbDebug.ipipeNf2.Nf2Value = value;
    
#if ISP_DEBUG
    printf("ISP_ipipeSetNf2 value = %3d\n", value);
#endif

    DRV_ipipeSetNf2(&nf2);
}

static int getSetIndex(int eTime, int aGain, int dGain, int cTemp)
{
    if (aGain >= 8000 && dGain >= 1536)
    {
        return 2; 
    }
    else if (aGain >= 5000)
    {  
        return 1;
    }
    else
    {
        return 0;
    }
}

int TI_2A_ISP_control_OV9712(int eTime, int aGain, int dGain, int cTemp)
{
    static int steaySet = -1;
    static int tempSet = -1, tempCnt = 0;
    int tempCountLimit = 10;

    int i = getSetIndex(eTime, aGain, dGain, cTemp);
    int j = 0;

    ISP_ipipeSetNf2(aGain, dGain);
    
    if (steaySet == i)
    {
        j = switch_rgb2rgb_matrixes(cTemp, rgb_matrixes[i], 0);
        tempSet = -1;
        tempCnt = 0;
    }
    else if (tempSet == i && tempCnt >= tempCountLimit)
    {
        steaySet = i; 
        tempSet = -1;
        tempCnt = 0;
        j = switch_rgb2rgb_matrixes(cTemp, rgb_matrixes[i], 1);
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

