
#include "alg_aewb_priv.h"
#include "TI_aewb.h"
#include "drv_ipipe.h"
#include "TI_aewb_ISP.h"

int g_bEnableTurnColor;

struct rgb2rgb_index rgb_matrixes_0[]= {
 {
    //U30-1080 (T=3000K)
    .color_temp = 3000,
    .rgb2rgbparam = {
      365,  -74, -35,
      -98, 371,  -17,
      -11, -179, 446,
        0, 0, 0
    },
    .rgb2rgb2param = {
      254,  6, -4,
      -9, 271, -6,
       1, -13, 268,
      0, 0, 0
    }
  },
  {
//CWF-1080  (T=4200K)
    .color_temp = 4200,
    .rgb2rgbparam = {
        470,-204,-10,
       -101, 376,-19,
        -13,-119,388,
         0,     0,   0
    },
    .rgb2rgb2param = {
        269, -8, -5,
         -4,262, -3,
          5,  1,250,
         0,     0,   0
    }
  },
//DAY-1080 (T=5000K)
  {
    .color_temp = 5000,
    .rgb2rgbparam = {
       445,-187,  -1,
       -94, 406, -56,
        -5,-149, 410,
        0,    0,   0
    },
    .rgb2rgb2param = {
      276,-23,  3,
       -8,272, -8,
       -2, -9,267,
      0,  0,   0
    }
  },
  // Day - 1080 (T=6500K)
  {
    .color_temp = 6500,
        .rgb2rgbparam = {
      445,-199, 10,
      -79, 401,-66,
       -2,-115,373,
        0,    0,   0
    },
    .rgb2rgb2param = {
        272,-15, -1,
         -4,262, -2,
         -2, -5,263,
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
    CSL_IpipeNfConfig* nf2[] = { &nf2_0, &nf2_1, &nf2_1, };
    CSL_IpipeEdgeEnhanceConfig* ee[] = { &ee_0, &ee_1, &ee_1, };
    DRV_ipipeSetEdgeEnhance(ee[i]);
    DRV_ipipeSetNf2(nf2[i]);
}

static int getSetIndex(int eTime, int aGain, int dGain, int cTemp)
{
    if (aGain >= 125000 && dGain >= 4096)
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

    int i = getSetIndex(eTime, aGain, dGain, cTemp);
    int j = 0;

    struct rgb2rgb_index* rgb_matrixes[]= {rgb_matrixes_0, rgb_matrixes_1, rgb_matrixes_2};
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




