#include "alg_aewb_priv.h"
#include "TI_aewb.h"
#include "drv_ipipe.h"
#include "TI_aewb_ISP.h"
#include "alg_aewb.h"
#include <imageTunePriv.h>
#include "imagetools.h"

#define ISP_DEBUG 0

#if 0
struct rgb2rgb_index rgb_matrixes_0[]= {
    //U30-1080 (T=2830K)
    {
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
    //CWF-1080  (T=4077K)
    {
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
            0, 0, 0}
    },
    //end
    {
        .color_temp = 0,
    }
};

static struct rgb2rgb_index* rgb_matrixes[]= {
    rgb_matrixes_0, 
    rgb_matrixes_0,
    rgb_matrixes_1,
};

#else
static struct rgb2rgb_index rgb_matrixes_0[] = 
{
    //F
    {
        .color_temp = 2700,
        .rgb2rgbparam = {
            327, 2, -74,
            -126, 328, 54,
            -28, -255, 539,
            0, 0, 0},
        .rgb2rgb2param = {
            225, 16, 15,
           20, 208, 27,
            -4, 22, 238,
            0, 0, 0}
    },
    //TL84
    {
        .color_temp = 4000,
        .rgb2rgbparam = {
            367, -76, -35,
            -103, 331, 28,
            -20, -141, 417,
            0, 0, 0},
        .rgb2rgb2param = {
            239, 9, 8,
            17, 219, 20,
            9, 7, 241,
            0, 0, 0}
    },
    //D50
    {
        .color_temp = 5000,
        .rgb2rgbparam = {
            387, -102, -29,
            -88, 334, 10,
            2, -109, 363,
            0, 0, 0},
        .rgb2rgb2param = {
            242, 7, 7,
            15, 223, 18,
            6, 5, 245,
            0, 0, 0}
    },
    //D65
    {
        .color_temp = 6500,
        .rgb2rgbparam = {
            387, -102, -29,
            -88, 334, 10,
            2, -109, 363,
            0, 0, 0},
        .rgb2rgb2param = {
            242, 7, 7,
            15, 223, 18,
            6, 5, 245,
            0, 0, 0}
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
static struct rgb2rgb_index rgb_matrixes_2[] = 
{
    {
        .color_temp = 3000,
        .rgb2rgbparam = {
            256, 0, 0,
            0, 256, 0,
            0, 0, 256,
            0, 0, 0},
        .rgb2rgb2param = {
            77, 150, 29,
            77, 150, 29,
            77, 150, 29,
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

#endif


#if 0
static CSL_IpipeEdgeEnhanceConfig ee_0 = {
    .enable = 1,
    .hpfShift = 3,
    .table = TI_YEE_TABLE_MT9P031,
    .lowerThres = 32,
    .hpfCoeff = {{80, -10, 0}, {-10,-10,0}, {0,0,0}},
    .edgeSharpGain = 0,
};
static CSL_IpipeEdgeEnhanceConfig ee_1 = {
    .enable = 0,
    .hpfShift = 4,
    .table = TI_YEE_TABLE_MT9P031,
    .lowerThres = 32,
    .hpfCoeff = {{80, -10, 0}, {-10,-10,0}, {0,0,0}},
    .edgeSharpGain = 0,
};

static CSL_IpipeEdgeEnhanceConfig* ee[] = {
    &ee_0,
    &ee_1,
    &ee_1,
};
#endif

static void ISP_SetCcdcClamp(int aGain, int dGain)
{
    static int value_bak = -1;
    int value;
    int i;
    static CSL_CcdcClampConfig pPrm = 
    {
        .enable = 1,         
        .mode = 0,              
        .dcOffset = -148,         
        .clampStartV = 0, 
        
        .horzClampMode = 0,     
        .horzClampBlack = 0, 
        .horzClampWinStartH = 0, 
        .horzClampWinStartV = 0, 
        .horzClampWinSizeV = 16, 
        .horzClampWinSizeH = 256,  
        .horzClampBaseWin = 0,  
        .horzClampNumWin = 0,   

        .vertClampResetValue = 0,   
        .vertClampResetValueSource = 0,  
        .vertClampLineAvgCoeff = 0,
        .vertClampWinStartH = 0,
        .vertClampWinStartV = 0, 
        .vertClampWinSizeH = 0,  
        .vertClampNumWin = 0,   
        .vertClampBlackOptical = 16, 
    };

    if (aGain < 3000)
    {
        value = -148;
    }
    else
    {
        value = -148 - (aGain - 3000) / 500.0 - (dGain - 1000) / 300.0;
    }
    
    value += gALG_AewbDebug.CcdcClamp.ClampValue_adjust;
    value = (value > -140) ? -140 : value;
    value = (value < -170) ? -170 : value;
    
    if (value == value_bak)
    {
        return;
    }
    value_bak = value;
    
#if ISP_DEBUG
    printf("----ISP_SetCcdcClamp value =%d\n", value);
#endif

    gALG_AewbDebug.CcdcClamp.ClampValue = value;
    pPrm.dcOffset = (Uint32)value;

    DRV_isifLock();  //ANR - ISIF
    CSL_ccdcSetClampConfig(&gCSL_ccdcHndl, &pPrm);
    DRV_isifUnlock();  //ANR - ISIF
}

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
    
    int ag = (aGain > 8000) ? 8000 : aGain;
    int dg = (dGain > 4096) ? 4096 : dGain;
    int value = 0;
    
    if (ag < 4000)
    {
        value = 20;
    }
    else
    {
        value = 20 + (ag - 4000) / 100 + (dg - 1000) / 30;
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
    if (aGain >= 8000 && dGain >= 4096)
    {
        return 2; 
    }
    else if (aGain >= 8000)
    {  
        return 1;
    }
    else
    {
        return 0;
    }
}

int TI_2A_ISP_control_MT9P031(int eTime, int aGain, int dGain, int cTemp)
{
    static int steaySet = -1;
    static int tempSet = -1, tempCnt = 0;
    int tempCountLimit = 3;
   
    int j = 0;
    int i = getSetIndex(eTime, aGain, dGain, cTemp);

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

