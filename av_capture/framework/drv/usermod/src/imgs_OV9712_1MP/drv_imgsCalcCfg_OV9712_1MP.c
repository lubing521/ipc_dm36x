
#include "drv_imgs_OV9712_1MP.h"

int DRV_imgsCalcFrameRate_OV9712(Uint32 fps)
{
    DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;

    pFrame->fps = fps;
    pFrame->rows = 828;
    pFrame->pll_pre_div = 2;
    pFrame->pll_mul = 7;
    pFrame->master_clk = IMGS_PLL_REF_CLK;
    pFrame->pll_clk = pFrame->master_clk / pFrame->pll_pre_div * pFrame->pll_mul;

    fps = fps * 100;
    switch (fps)
    {
    case 2500://25fps
        pFrame->cols = 2282;//2032;
        pFrame->pll_div = 2;
        break;
    case 3000://30 fps
    default:
        pFrame->cols = 1902;//1692;
        pFrame->pll_div = 2;
        break;
    }

    pFrame->f_pclk = pFrame->pll_clk / pFrame->pll_div;
    pFrame->t_pclk = 1.0e9 / pFrame->f_pclk;
    pFrame->t_row = pFrame->t_pclk * pFrame->cols;
    pFrame->t_frame = pFrame->t_row * pFrame->rows;
    return 0;
}

int DRV_imgsCalcFrameTimes_OV9712(Uint32 fps, Uint32 dataW, Uint32 dataH)
{
    DRV_ImgsModeConfig *pModeCfg = &gDRV_imgsObj.curModeConfig;
    DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;

    pFrame->fps = fps;
    pFrame->rows = 828;
    pFrame->pll_pre_div = 2;
    pFrame->pll_mul = 7;
    pFrame->master_clk = IMGS_PLL_REF_CLK;
    pFrame->pll_clk = pFrame->master_clk / pFrame->pll_pre_div * pFrame->pll_mul;

    fps = fps * 100;
    switch (fps)
    {
    case 1500://15 fps
        pFrame->cols = 1692;
        pFrame->pll_div = 4;
        break;
    case 1000://10 fps
        pFrame->cols = 1692;
        pFrame->pll_div = 6;
        break;
    case 750://7.5fps
        pFrame->cols = 1692;
        pFrame->pll_div = 8;
        break;
    case 500://5fps
        pFrame->cols = 1692;
        pFrame->pll_div = 12;
        break;
    case 2500://25fps
        pFrame->cols = 2282;//2032;
        pFrame->pll_div = 2;
        break;
    case 1250://12.5fps
        pFrame->cols = 2032;
        pFrame->pll_div = 4;
        break;
    case 833://8.33fps
        pFrame->cols = 2032;
        pFrame->pll_div = 6;
        break;
    case 625://6.25fps
        pFrame->cols = 2032;
        pFrame->pll_div = 8;
        break;
    case 2000://20fps
        pFrame->cols = 2540;
        pFrame->pll_div = 2;
        break;
    case 3000://30 fps
    default:
        pFrame->cols = 1902;//1692;
        pFrame->pll_div = 2;
        break;
    }

    pFrame->f_pclk = pFrame->pll_clk / pFrame->pll_div;
    pFrame->t_pclk = 1.0e9 / pFrame->f_pclk;
    pFrame->t_row = pFrame->t_pclk * pFrame->cols;
    pFrame->t_frame = pFrame->t_row * pFrame->rows;

    pModeCfg->sensorDataWidth   = 2 * OSA_ceil(dataW/2, 2);
    pModeCfg->sensorDataHeight  = 2 * OSA_ceil(dataH/2, 2);
    pModeCfg->validStartX       = IMGS_H_PAD/2;
    pModeCfg->validStartY       = IMGS_V_PAD/2;
    pModeCfg->validWidth        = pModeCfg->sensorDataWidth-IMGS_H_PAD;
    pModeCfg->validHeight       = pModeCfg->sensorDataHeight-IMGS_V_PAD;
    pModeCfg->binEnable         = 0;

#if 1
    OSA_printf("\n");
    OSA_printf("[Sensor Mode Info]\n");
    OSA_printf(" sensorDataWidth      = %d\n", pModeCfg->sensorDataWidth);
    OSA_printf(" sensorDataHeight     = %d\n", pModeCfg->sensorDataHeight);
    OSA_printf(" validStartX          = %d\n", pModeCfg->validStartX);
    OSA_printf(" validStartY          = %d\n", pModeCfg->validStartY);
    OSA_printf(" validWidth           = %d\n", pModeCfg->validWidth);
    OSA_printf(" validHeight          = %d\n", pModeCfg->validHeight);
    OSA_printf(" binEnable            = %d\n", pModeCfg->binEnable );
    OSA_printf(" \n");
    OSA_printf(" Sensor Frame Timing Info,\n");
    OSA_printf(" fps                  = %d\n", pFrame->fps           );
    OSA_printf(" t_frame (ns)         = %f\n", pFrame->t_frame       );
    OSA_printf(" t_row   (ns)         = %f\n", pFrame->t_row         );
    OSA_printf(" t_pclk  (ns)         = %f\n", pFrame->t_pclk        );
    OSA_printf(" cols                 = %d\n", pFrame->cols          );
    OSA_printf(" rows                 = %d\n", pFrame->rows          );
    OSA_printf(" master_clk           = %d\n", pFrame->master_clk    );
    OSA_printf(" pll_clk              = %d\n", pFrame->pll_clk       );
    OSA_printf(" f_pclk               = %d\n", pFrame->f_pclk        );
    OSA_printf(" pll_pre_div          = %d\n", pFrame->pll_pre_div   );
    OSA_printf(" pll_mul              = %d\n", pFrame->pll_mul       );
    OSA_printf(" pll_div              = %d\n", pFrame->pll_div       );
    OSA_printf(" \n");
#endif
  
    return 0;
}

int DRV_imgsCalcSW_OV9712(int exposureTimeInUsecs, int *SW)
{
    DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;
    //*SW = (exposureTimeInUsecs*1000.0) /pFrame->t_row;
    *SW = (exposureTimeInUsecs*1000.0) /pFrame->t_row + 0.5;

    if (*SW<1)
        *SW = 1;

    //if (*SW>0x338)
        //*SW = 0x338;
    
    //printf("exposureTimeInUsecs = %d, *SW = %d = 0x%x\n", exposureTimeInUsecs, *SW, *SW);    

    return OSA_SOK;
}

int DRV_imgsCalcAgain_OV9712(int again)
{
    Uint16 gainRegVal=0;
#if 0
    if (aGain<1000)
        aGain = 1000;

    if (aGain>7999)
        aGain = 7999;

    if (aGain >= 8000)
    {
        gainRegVal |= (1<<6);
        aGain >>= 1;
    }
    if (aGain >= 4000)
    {
        gainRegVal |= (1<<5);
        aGain >>= 1;
    }
    if (aGain >= 2000)
    {
        gainRegVal |= (1<<4);
        aGain >>= 1;
    }
    gainRegVal += ((aGain-1000)*16 + 500)/1000;
#else
	float gain_times;
    double gain_db;
    unsigned int gain_reg;
    int i =0;

    if (again <= 1000) again = 1000;
    if (again >= 60000) again = 60000;

    gain_times = again / 1000.0;
    gain_db = 20.0 * log10(gain_times);
    gain_reg = (unsigned char) (gain_db / 0.3 + 0.5);    //for 0.3db/step

    //gain_reg = gain_reg > 48 ? 48 : gain_reg;
    //gain_reg = 80;
    gainRegVal = gain_reg;
    printf("aGain = %d, gainRegVal = %d\n", again, gainRegVal);
#endif

    //printf("aGain = %d, gainRegVal = %d\n", aGain, gainRegVal);
    return gainRegVal;
}


int DRV_imgsInvertSW_OV9712(int regVal)
{
    DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;
    int exposureTimeInUsecs;

    exposureTimeInUsecs = (regVal*pFrame->t_row)/1000;
    return exposureTimeInUsecs;
}

