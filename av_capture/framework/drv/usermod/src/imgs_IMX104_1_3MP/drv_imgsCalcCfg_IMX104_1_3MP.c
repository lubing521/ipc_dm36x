
#include "drv_imgs_IMX104_1_3MP.h"
#include <math.h>

#define DRV_SHOW_INFO

extern int gCurrsensorMode;
int DRV_imgsCalcFrameRate(int fps)
{
  if(fps<5)
    fps=30;

  if(fps>30)
    fps=30;

  gDRV_imgsObj.curFrameTime.fps    = fps;

  gDRV_imgsObj.curFrameTime.t_frame = 1000.0*IMGS_MHZ/(gDRV_imgsObj.curFrameTime.fps);
  gDRV_imgsObj.curFrameTime.VB      = gDRV_imgsObj.curFrameTime.t_frame/gDRV_imgsObj.curFrameTime.t_row - gDRV_imgsObj.curFrameTime.H;

  if(gDRV_imgsObj.curFrameTime.VB < gDRV_imgsObj.curFrameTime.VBmin)
    gDRV_imgsObj.curFrameTime.VB = gDRV_imgsObj.curFrameTime.VBmin;

  return 0;
}

int DRV_imgsCalcFrameTime(Uint32 fps, Uint32 dataW, Uint32 dataH, Bool binEnable)
{
  DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;
  DRV_ImgsModeConfig *pModeCfg = &gDRV_imgsObj.curModeConfig;

  memset(pFrame, 0, sizeof(*pFrame));

  if(dataW>IMGS_W_MAX)
    dataW = IMGS_W_MAX;

  if(dataH>IMGS_H_MAX)
    dataH = IMGS_H_MAX;

  if(fps<5)
    fps=30;

  if(fps>30)
    fps=30;

  pFrame->col_size = dataW;
  pFrame->row_size = dataH;

  pFrame->col_bin  = 0;
  pFrame->row_bin  = 0;

  pFrame->H      = 2*OSA_ceil(dataH/2, 2);
  pFrame->W      = 2*OSA_ceil(dataW/2, 2);

  pFrame->col_start = IMGS_COL_START_MIN + (IMGS_W_MAX - pFrame->col_size)/2;
  pFrame->row_start = IMGS_ROW_START_MIN + (IMGS_H_MAX - pFrame->row_size)/2;

  if(pFrame->col_start < 0 )
    pFrame->col_start = 0;

  if(pFrame->row_start < 0 )
    pFrame->row_start = 0;

  pFrame->col_start = OSA_align(pFrame->col_start, 4*(pFrame->col_bin+1));
  pFrame->row_start = OSA_align(pFrame->row_start, 4*(pFrame->row_bin+1));

  pFrame->VBmin  = 9;
  pFrame->HBmin  = 346*(pFrame->row_bin+1)+64+(80/(pFrame->col_bin+1))/2;
  pFrame->HB     = pFrame->HBmin;
  pFrame->SW     = pFrame->H-1;
  pFrame->SD     = 0;

  if(dataW*dataH >= 1920*1080) {
    if(fps>24)
      fps = 24;
  }
  if(dataW*dataH >= 2048*1536) {
    if(fps>15)
      fps = 15;
  }

  pFrame->f_pclk = 48*IMGS_MHZ;
  if(dataW*dataH >= 720*480)
    pFrame->f_pclk = 64*IMGS_MHZ;
  if(dataW*dataH >= 1280*960) {
    if(pFrame->row_bin==0)
      pFrame->f_pclk = 64*IMGS_MHZ;
    else
      pFrame->f_pclk = 96*IMGS_MHZ;
  }
  if(dataW*dataH >= 1600*1200)
    pFrame->f_pclk = 96*IMGS_MHZ;

  pFrame->pll_N = (2-1);

  pFrame->pll_M = (16);
	if(fps < 8){
		pFrame->pll_DIV = 2;
		pFrame->pll_PREDIV = 4;
	} else if(fps <= 15){
		pFrame->pll_DIV = 1;
		pFrame->pll_PREDIV = 4;
	} else {
		pFrame->pll_DIV = 1;
		pFrame->pll_PREDIV = 2;
	}
  

  pFrame->t_pclk = 1000.0*IMGS_MHZ/pFrame->f_pclk;

  pFrame->t_row  = 2*pFrame->t_pclk*OSA_max(
                                    pFrame->W/2 + OSA_max(pFrame->HB, pFrame->HBmin),
                                    41+396*(pFrame->row_bin+1)+99
                                    );


  pFrame->shutterOverhead = 208 * (pFrame->row_bin+1) + 98 + (pFrame->SD+1) - 94;
  pFrame->shutterOverhead = 2 * pFrame->t_pclk * pFrame->shutterOverhead;

  DRV_imgsCalcFrameRate(fps);

  DRV_imgsCalcSW(pFrame->t_frame/1000);

  pModeCfg->sensorDataWidth   = pFrame->W;
  pModeCfg->sensorDataHeight  = pFrame->H;
	if(gCurrsensorMode==DRV_IMGS_SENSOR_MODE_2048x1536){  
		pModeCfg->validStartX       = IMGS_H_3M_PAD/2;
		pModeCfg->validStartY       = IMGS_V_3M_PAD/2;
		pModeCfg->validWidth        = pModeCfg->sensorDataWidth-IMGS_H_3M_PAD;
		pModeCfg->validHeight       = pModeCfg->sensorDataHeight-IMGS_V_3M_PAD;
	}else{
  pModeCfg->validStartX       = IMGS_H_PAD/2;
  pModeCfg->validStartY       = IMGS_V_PAD/2;
  pModeCfg->validWidth        = pModeCfg->sensorDataWidth-IMGS_H_PAD;
  pModeCfg->validHeight       = pModeCfg->sensorDataHeight-IMGS_V_PAD;
	}
  pModeCfg->binEnable         = binEnable;
  /*
  #if (0)
  pModeCfg->vdint0		= 0;
  pModeCfg->vdint1		= 0;
  pModeCfg->vdint2		= 0;
  #else
  pModeCfg->vdint0		= 1023;	//(5);
  pModeCfg->vdint1		= 0;		//800;
  pModeCfg->vdint2		= IMGS_H_MAX-1;
  #endif
*/
  #ifdef DRV_SHOW_INFO
  OSA_printf(" Sensor Mode Info,\n");
  OSA_printf(" Width      = %4d \n", pModeCfg->sensorDataWidth);
  OSA_printf(" Height     = %4d \n", pModeCfg->sensorDataHeight);
  OSA_printf(" fps        = %4d \n", fps);
  OSA_printf(" Bin Enable = %4d \n", pModeCfg->binEnable );
  OSA_printf(" \n");
  OSA_printf(" Sensor Frame Timing Info,\n");
  OSA_printf(" fps                  = %d\n", pFrame->fps              );
  OSA_printf(" t_frame (ns)         = %f\n", pFrame->t_frame          );
  OSA_printf(" t_row   (ns)         = %f\n", pFrame->t_row            );
  OSA_printf(" t_pclk  (ns)         = %f\n", pFrame->t_pclk           );
  OSA_printf(" W                    = %d\n", pFrame->W                );
  OSA_printf(" H                    = %d\n", pFrame->H                );
  OSA_printf(" SW                   = %d\n", pFrame->SW               );
  OSA_printf(" SD                   = %d\n", pFrame->SD               );
  OSA_printf(" HB                   = %d\n", pFrame->HB               );
  OSA_printf(" VB                   = %d\n", pFrame->VB               );
  OSA_printf(" HBmin                = %d\n", pFrame->HBmin            );
  OSA_printf(" VBmin                = %d\n", pFrame->VBmin            );
  OSA_printf(" f_pclk               = %d\n", pFrame->f_pclk           );
  OSA_printf(" shutterOverhead (ns) = %f\n", pFrame->shutterOverhead  );
  OSA_printf(" col_size             = %d\n", pFrame->col_size         );
  OSA_printf(" row_size             = %d\n", pFrame->row_size         );
  OSA_printf(" col_bin              = %d\n", pFrame->col_bin          );
  OSA_printf(" row_bin              = %d\n", pFrame->row_bin          );
  OSA_printf(" col_start            = %d\n", pFrame->col_start        );
  OSA_printf(" row_start            = %d\n", pFrame->row_start        );
  OSA_printf(" pll_M                = %d\n", pFrame->pll_M            );
  OSA_printf(" pll_N                = %d\n", pFrame->pll_N            );
  OSA_printf(" pll_DIV              = %d\n", pFrame->pll_DIV          );
  OSA_printf(" \n");
  #endif

  return OSA_SOK;
}

int DRV_imgsCalcSW(int exposureTimeInUsecs)
{

	int Lw;
	DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;
	if(gCurrsensorMode==DRV_IMGS_SENSOR_MODE_2048x1536){
	if(pFrame->Is50Hz==0){
		Lw = IMGS_SENSOR_LINEWIDTH;
	}else{
		Lw = IMGS_SENSOR_LINEWIDTH;	//PAL, TBD	
		}
	}else{
		if(pFrame->Is50Hz==0){
			Lw = IMGS_SENSOR_LINEWIDTH;
		}else{
			Lw = IMGS_SENSOR_LINEWIDTH;	//PAL, TBD	
		}
	}
	pFrame->SW = (int)(((double)exposureTimeInUsecs*(double)Lw)/((double)33333) );	
	if(pFrame->SW<1){
		pFrame->SW = 1;	
	}
	
	return OSA_SOK;
}

int DRV_imgsCalcAgain(int aGain)
{
  Uint16 gainRegVal=0;
  int temp1, temp2;
  /* Bit[7]: digital gain */
  /* Bit[6:0]: analog gain */
  /* 1000 = 1x gain */
  if(aGain < 10000)
    aGain = 10000;

  if(aGain > 310000)
    aGain = 310000;

	/*bit[7:4]*/
	temp1 = (Uint16)(log(aGain/10000)/log(2));
	gainRegVal = ((1<<temp1)-1)<<4; 

	/*
	((aGain/1000)/(2^temp1)-1)*16
	= ((aGain/1000)*16/(2^temp1)-16)
	= ((aGain/1000)<<4>>temp1-16)
	= ((aGain/1000)<<(4-temp1)-16)
	= ((aGain<<(4-temp1)/1000)-16)
	*/
	/*bit[3:0]*/
	temp2 = (((aGain<<(4-temp1))/10000)-16);
	gainRegVal |= temp2;

	//printf(" aGain = %d temp1 = %d temp2 = %d gainRegVal = %d \n",aGain,temp1,temp2,gainRegVal);

  return gainRegVal;
}
