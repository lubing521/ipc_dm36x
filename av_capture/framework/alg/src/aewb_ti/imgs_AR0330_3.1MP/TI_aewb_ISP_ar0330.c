#include "alg_aewb_priv.h"
#include "TI_aewb.h"
#include "drv_ipipe.h"
#include "TI_aewb_ISP.h"
#include "alg_aewb.h"
#include <imageTunePriv.h>
#include "imagetools.h"

#define ISP_DEBUG 0

static struct rgb2rgb_index rgb_matrixes_0[]= 
{
  {
//U30 
    .color_temp = 2800,
    .rgb2rgbparam = {
      343,  -50, -37,
      -111,  323, 44,
      -24, -266, 546,
        0,    0,   0
    },
    .rgb2rgb2param = {
      252,   -8,  13,
       -2,  258,  0,
      -25,   26, 255,
        0,    0,   0
    }
  },
  {
//TL84
    .color_temp = 4000,
    .rgb2rgbparam = {
         362,  -111,   5,
         -90,   338,   8,
         -22,  -138, 416,
           0,     0,    0
    },
    .rgb2rgb2param = {
        250,    -6,   12,
         -2,   258,    0,
         -9,     3,  262,
          0,     0,    0
    }
  },
//D50
  {
    .color_temp = 5000,
    .rgb2rgbparam = {
      360, -112,   8,
      -76,  308,  25,
      -10, -121, 387,
        0,    0,   0
    },
    .rgb2rgb2param = {
      251,   -8,   12,
       -3,  256,    3,
       -4,    2,  258,
        0,    0,    0
    }
  },
//D65
  {
    .color_temp = 6500,
    .rgb2rgbparam = {
      357, -109,   7,
      -66,  282,  40,
       -5, -113, 374,
        0,    0,   0
    },
    .rgb2rgb2param = {
      256,  -10,   10,
       -3,  256,    3,
       -3,  -3,   262,
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

#if 0
static CSL_IpipeEdgeEnhanceConfig ee_0 = 
{
    .enable = 1,
    .haloReduceEnable = 1,
    .mergeMethod = 0,    
    .hpfShift = 15,
    .table = NULL,
    .edgeSharpGain = 64,
    .edgeSharpGainGradient = 96,
    .edgeSharpHpValLowThres = 250,
    .edgeSharpOffsetGradient = 24, 
    .edgeSharpHpValUpLimit = 32,
};

static CSL_IpipeEdgeEnhanceConfig ee_1 = 
{
    .enable = 1,
    .haloReduceEnable = 1,
    .mergeMethod = 0,    
    .hpfShift = 15,      
    .table = NULL, 
    .edgeSharpGain = 32,
    .edgeSharpGainGradient = 48,
    .edgeSharpHpValLowThres = 350,
    .edgeSharpOffsetGradient = 24, 
    .edgeSharpHpValUpLimit = 32,
}; 

static CSL_IpipeEdgeEnhanceConfig* ee[] = 
{
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
        .dcOffset = -200,         
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
        value = -200;
    }
    else
    {
        value = -200 - (aGain - 3000) / 500.0 - (dGain - 1000) / 300.0;
    }

    value += gALG_AewbDebug.CcdcClamp.ClampValue_adjust;
    value = (value > -180) ? -180 : value;
    value = (value < -210) ? -210 : value;
    
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
    if (aGain >= 40000 && dGain >= 1280)
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

int TI_2A_ISP_control_ar0330(int eTime, int aGain, int dGain, int cTemp)
{
    static int steaySet = -1;
    static int tempSet = -1, tempCnt = 0;
    int tempCountLimit = 100;

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

