#include "alg_aewb_priv.h"
#include "TI_aewb.h"
#include "drv_ipipe.h"
#include "TI_aewb_ISP.h"
#include "alg_aewb.h"
#include "imagetools.h"

#define ISP_DEBUG 0

static struct rgb2rgb_index rgb_matrixes_0[] = 
{
    //U30
    {
        .color_temp = 3000,
        .rgb2rgbparam = {
            365, -62, -47,
           -115, 323,  48,
            -44,-306, 605,
            0, 0, 0},
        .rgb2rgb2param = {
            249, -4,  11,
            -3, 259,   0,
           -25,  27, 254,
            0, 0, 0}
    },
    //TL84
    {
        .color_temp = 4000,
        .rgb2rgbparam = {
            367, -119,   8,
            -94, 336,   15,
            -26, -157, 439,
            0, 0, 0},
        .rgb2rgb2param = {
            251, -6, 11,
            -1, 256, 1,
            -5, 1, 260,
            0, 0, 0}
    },
    //D50
    {
        .color_temp = 5000,
        .rgb2rgbparam = {
            378, -124,   2,
            -82,  301,  37,
            -12, -136, 404,
            0, 0, 0},
        .rgb2rgb2param = {
            250, -8,   14,
              0, 254,   2,
             -2,  -1, 259,
            0, 0, 0}
    },
    //D65
    {
        .color_temp = 6500,
        .rgb2rgbparam = {
            384, -127, -1,
            -74, 288,  42,
            -5, -118, 380,
            0, 0, 0},
        .rgb2rgb2param = {
            255, -9, 10,
            0, 251, 5,
            -4, -3, 263,
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

static void ISP_SetCcdcClamp(int aGain, int dGain)
{
    static int value_bak = -1;
    int value;
    int i;
    static CSL_CcdcClampConfig pPrm = 
    {
        .enable = 1,         
        .mode = 0,              
        .dcOffset = -176,         
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
    
    int ag = (aGain > 20000) ? 20000 : aGain;
    int dg = (dGain >  1280) ?  1280 : dGain;
    int value = 0;
    
    if (ag < 4000)
    {
        value = 20;
    }
    else
    {
        value = 20 + (ag - 4000) / 250 + (dg - 1000) / 10;
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

int TI_2A_ISP_control_MT9M034(int eTime, int aGain, int dGain, int cTemp)
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
        //auto_contrast(0);
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
        //auto_contrast(1);
    }
    return 100 * steaySet + j;
}

