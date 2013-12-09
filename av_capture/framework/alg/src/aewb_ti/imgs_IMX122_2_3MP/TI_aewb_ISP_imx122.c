#include "alg_aewb_priv.h"
#include "TI_aewb.h"
#include "drv_ipipe.h"
#include "TI_aewb_ISP.h"
#include <imageTunePriv.h>
#include "imagetools.h"

#define ISP_DEBUG 0

static struct rgb2rgb_index rgb_matrixes_0[]= 
{
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
            0, 0, 0
			}
    },
    //end
    {
        .color_temp = 0,
    }
};

static struct rgb2rgb_index rgb_matrixes_2[] = 
{
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

static struct rgb2rgb_index* rgb_matrixes[]= 
{
    rgb_matrixes_0, 
    rgb_matrixes_1,
    rgb_matrixes_1,
};

#if 0
static CSL_IpipeEdgeEnhanceConfig ee_0 = {
    .enable = 1,
    .hpfShift = 3,
    .table = TI_YEE_TABLE_IMX122,
    .lowerThres = 32,
    .hpfCoeff = {{80, -10, 0}, {-10,-10,0}, {0,0,0}},
    .edgeSharpGain = 0,
};
static CSL_IpipeEdgeEnhanceConfig ee_1 = {
    .enable = 0,
    .hpfShift = 4,
    .table = TI_YEE_TABLE_IMX122,
    .lowerThres = 32,
    .hpfCoeff = {{80, -10, 0}, {-10,-10,0}, {0,0,0}},
    .edgeSharpGain = 0,
};
#endif

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
    .lutIntensityTable  = {24, 25, 26, 26, 27, 28, 29, 30},
    .lutSpreadTable     = { 0,  0,  0,  0,  0,  0,  0,  0},
    };
    
    int ag = (aGain > 40000) ? 40000 : aGain;
    int dg = (dGain >  1536) ?  1536 : dGain;
    int value = 0;
    
    if (ag < 10000)
    {
        value = 20;
    }
    else
    {
        value = 20 + (ag - 10000) / 300 + (dg - 1000) / 10;
    }

    value += gALG_AewbDebug.ipipeNf2.Nf2Value_adjust + ALG_aewbGetNoiseLevel();
    value = (value > 200) ? 200 : value;
    value = (value <  16) ?  16 : value;
    
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
    if (aGain >= 40000 && dGain >= 2048)
    {
        return 2; 
    }
    else if (aGain >= 20000)
    {  
        return 1;
    }
    else
    {
        return 0;
    }
}

int TI_2A_ISP_control_imx122(int eTime, int aGain, int dGain, int cTemp)
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

