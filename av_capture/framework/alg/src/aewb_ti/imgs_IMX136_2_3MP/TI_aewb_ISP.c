
#include "alg_aewb_priv.h"
#include "TI_aewb.h"
#include "drv_ipipe.h"
#include "TI_aewb_ISP.h"
#include "alg_vnf.h"

struct rgb2rgb_index rgb_matrixes_0[]= {
    {
        //U30-1080 (T=3000K)
        .color_temp = 3000,
        .rgb2rgbparam = {
            372, -85, -31,  //364,  -49, -59,
            -129, 395, -10, //-120,  408, -32,
            34, -276, 498,  //-7, -225, 488,
            0, 0, 0
        },
        .rgb2rgb2param = {
            251, -5, 10,   //253,   4, -1,
            0, 255, 1,     //-8, 272, -8,
            -11, 5, 262,   //-11,   7, 260,
            0, 0, 0
        }
    },
    {
        //TL84  (T=4000K)
        .color_temp = 4000,
        .rgb2rgbparam = {
            382, -146, 20,  //463,-172,-35,
            -104, 407, -47, //-125, 401,-20,
            11, -175, 420,  //-18,-132,406,
            0,     0,   0
        },
        .rgb2rgb2param = {
            252, -6, 10,  //267, -8, -3,
            1, 255, 0,    //-5,268, -6,
            -4, 0, 260,   //0,  2,253,
            0, 0, 0
        }
    },
    //DAY-1080 (T=5000K)
    {
        .color_temp = 5000,
        .rgb2rgbparam = {  
            380, -150, 26,  //438,-163, -19,
            -82, 368, -30,  //-106, 411, -49,
            10, -166, 412,  //-12,-162, 430,
            0,    0,   0
        },
        .rgb2rgb2param = {  
            256, -14, 14, //272,-19,   4,
            1, 256, -1,   //-11,281, -14,
            -3, 0, 259,   //-6, -3, 265,
            0,  0,   0
        }
    },
    // Day - 1080 (T=6500K)
    {
        .color_temp = 6500,
        .rgb2rgbparam = {
           394, -168,  30,  //441, -163, -22,
           -75,  356, -25,  //-94,  412, -63,
            11, -149, 394,  //-13, -121, 390,
            0,    0,   0
        },
        .rgb2rgb2param = {
            259, -14, 11, //268,   -13,   1,
            3, 250, 3,    //-4,   266,  -6,
            -3, -3, 262,  //-4,    -5, 264,
            0,  0,   0
        }
    },
    //end
    {
        .color_temp = 0,
    }
};
static struct rgb2rgb_index rgb_matrixes_1[] = {
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
            0, 0, 0
			}
    },
    //end
    {
        .color_temp = 0,
    }
};
static struct rgb2rgb_index rgb_matrixes_2[] = {
    {
        .color_temp = 3000,
        .rgb2rgbparam = {
            77, 150, 29,
            77, 150, 29,
            77, 150, 29,
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

static CSL_IpipeEdgeEnhanceConfig ee_0 = {
    .enable = 1,
    .hpfShift = 3,
    .table = TI_YEE_TABLE,
    .lowerThres = 32,
    .hpfCoeff = {{80, -10, 0}, {-10,-10,0}, {0,0,0}},
    .edgeSharpGain = 0,
};
static CSL_IpipeEdgeEnhanceConfig ee_1 = {
    .enable = 0,
    .hpfShift = 4,
    .table = TI_YEE_TABLE,
    .lowerThres = 32,
    .hpfCoeff = {{80, -10, 0}, {-10,-10,0}, {0,0,0}},
    .edgeSharpGain = 0,
};

static CSL_IpipeNfConfig nf2_0 = {
    .enable = 1,
    .spreadVal = 3,
    .lutAddrShift = 2,
    .greenSampleMethod = 0,
    .lscGainEnable = 0,
    .edgeDetectThresMin = 0,
    .edgeDetectThresMax = 2047,
    .lutThresTable = {14,26,36,44, 50,56,64,70},
    .lutIntensityTable = {20,20,18,18, 16,16,16,16},
};
static CSL_IpipeNfConfig nf2_1 = {
    .enable = 1,
    .spreadVal = 3,
    .lutAddrShift = 2,
    .greenSampleMethod = 0,
    .lscGainEnable = 0,
    .edgeDetectThresMin = 0,
    .edgeDetectThresMax = 2047,
    .lutThresTable = {20,20,20,20, 20,20,20,20},
    .lutIntensityTable = {16,16,16,16, 16,16,16,16},
}; 

static void ISP_config(int i)
{
    CSL_IpipeEdgeEnhanceConfig* ee[] = { &ee_0, &ee_1, &ee_1, };
    DRV_ipipeSetEdgeEnhance(ee[i]);

    CSL_IpipeNfConfig* nf2[] = { &nf2_0, &nf2_1, &nf2_1, };
    DRV_ipipeSetNf2(nf2[i]);
}

static int getSetIndex(int eTime, int aGain, int dGain, int cTemp)
{
    if (aGain >= 125000 && dGain >= 4000)
    {
        return 2;
    }
    else if (aGain >= 125000)
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

    struct rgb2rgb_index* rgb_matrixes[]= {rgb_matrixes_0, rgb_matrixes_1, rgb_matrixes_2};
    int i = getSetIndex(eTime, aGain, dGain, cTemp);
    int j = 0;

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




