
#include "drv_imgs_OV9712_1MP.h"
int DRV_imgsCalcFrameRate(Uint32 fps)
{

  DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;
  fps = 25;
  pFrame->fps = fps;
  pFrame->rows = 828;
  pFrame->pll_pre_div = 2;
  pFrame->pll_mul = 7;
  pFrame->master_clk = 24 * IMGS_MHZ;
  pFrame->pll_clk = pFrame->master_clk 
      / pFrame->pll_pre_div * pFrame->pll_mul;

  fps = fps * 100;
  switch (fps) {
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
          pFrame->cols = 2283;
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
          pFrame->cols = 1692;
          pFrame->pll_div = 2;
          break;
 }

  pFrame->f_pclk = pFrame->pll_clk / pFrame->pll_div;
  pFrame->t_pclk = 1.0e9 / pFrame->f_pclk;
  pFrame->t_row = pFrame->t_pclk * pFrame->cols;
  pFrame->t_frame = pFrame->t_row * pFrame->rows;
  return 0;
}
int DRV_imgsCalcFrameTimes(Uint32 fps, Uint32 dataW, Uint32 dataH)
{

  DRV_ImgsModeConfig *pModeCfg = &gDRV_imgsObj.curModeConfig;
  DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;
  pFrame->fps = fps;
  pFrame->rows = 828;
  pFrame->pll_pre_div = 2;
  pFrame->pll_mul = 6;
  pFrame->master_clk = 24 * IMGS_MHZ;
  pFrame->pll_clk = pFrame->master_clk 
      / pFrame->pll_pre_div * pFrame->pll_mul;
  switch (fps) {
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
       case 5://5fps
          pFrame->cols = 1692;
          pFrame->pll_div = 12;
          break;
     case 2500://25fps
          pFrame->cols = 2032;
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
          pFrame->cols = 1692;
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

  printf("fps= %d, t_frame = %f, t_row = %f, t_pclk = %f\ncols = %d, rows = %d, master_clk = %d,"
          "pll_clk = %d, f_pclk = %d\n pll_pre_div = %d, pll_mul = %d pll_div = %d\n", 
          pFrame->fps, pFrame->t_frame, pFrame->t_row, pFrame->t_pclk, pFrame->cols, pFrame->rows,
          pFrame->master_clk, pFrame->pll_clk,pFrame->f_pclk, pFrame->pll_pre_div, pFrame->pll_mul, pFrame->pll_div);  
          

     return 0;
}

int DRV_imgsCalcSW(int exposureTimeInUsecs, int *SW)
{

  int reg_tem32;
  DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;
  //*SW = (exposureTimeInUsecs*1000.0) /pFrame->t_row;
  *SW = (exposureTimeInUsecs*1000.0) /pFrame->t_row + 0.5;
  //reg_tem32 = exposureTimeInUsecs;
  //*SW = reg_tem32 *128;
  if(*SW<1)
    *SW = 1;

  if(*SW>0x338)
    *SW = 0x338 ;
  //printf("\n\n shutter=%d, SW=%d\n", exposureTimeInUsecs, SW);

  return OSA_SOK;
}


int DRV_imgsCalcAgain(int aGain)
{
  Uint16 gainRegVal=0;
  //printf("\n\naGain = %d\n\n", aGain);

  if(aGain<1000)
    aGain = 1000;

  if(aGain>15500)
    aGain = 15500;
 
  if(aGain >= 8000){
      gainRegVal |= (1<<6);
      aGain >>= 1;
  }
  if(aGain >= 4000){
      gainRegVal |= (1<<5);
      aGain >>= 1;
  }
  if(aGain >= 2000){
      gainRegVal |= (1<<4);
      aGain >>= 1;
  }
  gainRegVal += ((aGain-1000)*16 + 500)/1000;

  //printf("\n\ngainRegVal = %d\n\n", gainRegVal);
  return gainRegVal;
}

int DRV_imgsInvertSW(int regVal)
{
  DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;
  int exposureTimeInUsecs;

  exposureTimeInUsecs = (regVal*pFrame->t_row)/1000;

  return exposureTimeInUsecs;
}

