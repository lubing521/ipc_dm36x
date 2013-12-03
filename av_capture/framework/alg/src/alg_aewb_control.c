
#include "alg_aewb_priv.h"
#include "imageTunePriv.h"
#include <drv_capture.h>
#include <drv_display.h>

// add by fan
//#define ALG_AEWB_DEBUG

extern IMAGE_TUNE_Ctrl gIMAGE_TUNE_ctrl;

static Uint32 yee_table[] = {
	#include "alg_aewb_ee_table.txt"
};

#define ENABLE_COMPENSATION  (200) // 100 = 1.00x compensation i.e none, 150 = 1.5x compensation

short ALG_aewbSetSensorGain(int gain)
{
  static int prevValue = -1;
  int gain32;

  if(prevValue==gain)
    return 0;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: Sensor A Gain = %d\n", gain);
  #endif

  prevValue = gain;

  gain32 = (int)gain*1;

  if(gALG_aewbObj.vsEnable)
    gain32 = (int)(gain*ENABLE_COMPENSATION)/100;
  else if(gALG_aewbObj.vnfDemoCfg)
  	gain32 = (int)(gain*2);

  DRV_imgsSetAgain(gain32, 0);

  return 0;
}

short ALG_aewbSetSensorDGain(int gain)
{
  static int prevValue = -1;

  if(prevValue==gain)
    return 0;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: Sensor D Gain = %d\n", gain);
  #endif

  prevValue = gain;
  DRV_imgsSetDgain(gain);

  return 0;
}

short ALG_aewbSetSensorExposure(int shutter)
{
  static int prevValue = -1;
  int shutter32;

  if(prevValue==shutter)
    return 0;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: Sensor Exposure = %d\n", shutter);
  #endif

  prevValue = shutter;

  shutter32 = (int)(shutter*100)/gALG_aewbObj.reduceShutter;

  if(gALG_aewbObj.vsEnable)
    shutter32 = (shutter32*100)/ENABLE_COMPENSATION;
  /*else if(gALG_aewbObj.vnfDemoCfg)
    shutter32 = (shutter32*200)/ENABLE_COMPENSATION;*/

  DRV_imgsSetEshutter(shutter32, 0);

  return 0;
}

short ALG_aewbSetIpipeWb(AWB_PARAM  *pAwb_Data )
{
  DRV_IpipeWb ipipeWb;
  static AWB_PARAM PreAwb_Data;
  int dGain;

	if( memcmp( &PreAwb_Data, pAwb_Data, sizeof(AWB_PARAM))== 0 )
		return 0;

	PreAwb_Data = *pAwb_Data;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: R Gr Gb B = (%d, %d, %d, %d) DGAIN = %d\n",
    pAwb_Data->rGain, pAwb_Data->grGain, pAwb_Data->gbGain, pAwb_Data->bGain, pAwb_Data->dGain
    );
  #endif

  ipipeWb.gainR  = 4*pAwb_Data->rGain;
  ipipeWb.gainGr = 4*pAwb_Data->grGain;
  ipipeWb.gainGb = 4*pAwb_Data->gbGain;
  ipipeWb.gainB  = 4*pAwb_Data->bGain;

  DRV_ipipeSetWb(&ipipeWb);

  dGain = pAwb_Data->dGain*2;

  if(gALG_aewbObj.vnfDemoCfg)
	dGain = (512*8)-1;

  DRV_isifSetDgain(dGain, dGain, dGain, dGain, 0);


  return 0;
}

short ALG_aewbSetIpipeWb2(AWB_PARAM  *pAwb_Data )
{
  DRV_IpipeWb ipipeWb;
  static AWB_PARAM PreAwb_Data;
  pAwb_Data->dGain = 256;

  if( memcmp( &PreAwb_Data, pAwb_Data, sizeof(AWB_PARAM))== 0 )
	  return 0;

  PreAwb_Data = *pAwb_Data;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: R Gr Gb B = (%d, %d, %d, %d)\n",
    pAwb_Data->rGain, pAwb_Data->grGain, pAwb_Data->gbGain, pAwb_Data->bGain);
  #endif

  ipipeWb.gainR  = 4*pAwb_Data->rGain;
  ipipeWb.gainGr = 4*pAwb_Data->grGain;
  ipipeWb.gainGb = 4*pAwb_Data->gbGain;
  ipipeWb.gainB  = 4*pAwb_Data->bGain;

  DRV_ipipeSetWb(&ipipeWb);

  return 0;
}

short ALG_aewbSetIsifDGain(int dGain)
{
  static int pre_dGain = -1;
  if(dGain == pre_dGain) return 0;

  pre_dGain = dGain;
  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: ISIF D Gain = %d\n", dGain);
  #endif

  dGain /= 2;
  DRV_isifSetDgain(dGain, dGain, dGain, dGain, 0);

  return 0;
}



short ALG_aewbSetSensorDcsub(int dcsub)
{
	static int prevValue=-1;

  if( prevValue == dcsub )
  	return 0;

	#ifdef ALG_AEWB_DEBUG
		OSA_printf(" AEWB: Sensor DCSUB = %d\n", dcsub);
  #endif

  prevValue = dcsub;

  dcsub = -dcsub;

  DRV_isifSetDcSub(dcsub);

  return 0;
}

short ALG_aewbSetSensorBin(int bin)
{
  static int prevValue=-1;

  if(prevValue==bin)
    return 0;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: Sensor BIN = %d!!!!!!!!!!!!!!!!!Switch\n", bin);
  #endif

  prevValue = bin;

  if(gALG_aewbObj.vnfDemoCfg)
   	DRV_imgsBinMode(bin=0x20);
  else
   	DRV_imgsBinMode(bin);

  return 0;
}

short ALG_aewbSetRgb2Rgb(RGB2RGB_PARAM  *pRgb2Rgb )
{
  CSL_IpipeRgb2RgbConfig rgb2rgb;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: RGB2RGB \n");
  #endif

  rgb2rgb.matrix[0][0] = pRgb2Rgb->rgb_mul_rr;
  rgb2rgb.matrix[0][1] = pRgb2Rgb->rgb_mul_gr;
  rgb2rgb.matrix[0][2] = pRgb2Rgb->rgb_mul_br;

  rgb2rgb.matrix[1][0] = pRgb2Rgb->rgb_mul_rg;
  rgb2rgb.matrix[1][1] = pRgb2Rgb->rgb_mul_gg;
  rgb2rgb.matrix[1][2] = pRgb2Rgb->rgb_mul_bg;

  rgb2rgb.matrix[2][0] = pRgb2Rgb->rgb_mul_rb;
  rgb2rgb.matrix[2][1] = pRgb2Rgb->rgb_mul_gb;
  rgb2rgb.matrix[2][2] = pRgb2Rgb->rgb_mul_bb;

  rgb2rgb.offset[0] = pRgb2Rgb->rgb_oft_or;
  rgb2rgb.offset[1] = pRgb2Rgb->rgb_oft_og;
  rgb2rgb.offset[2] = pRgb2Rgb->rgb_oft_ob;

  if(gALG_aewbObj.vnfDemoCfg == 0)
	DRV_ipipeSetRgb2Rgb(&rgb2rgb);

  return 0;
}
short ALG_aewbSetRgb2Rgb2(RGB2RGB_PARAM  *pRgb2Rgb )
{
  CSL_IpipeRgb2RgbConfig rgb2rgb;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: RGB2RGB2 \n");
  #endif

  rgb2rgb.matrix[0][0] = pRgb2Rgb->rgb_mul_rr;
  rgb2rgb.matrix[0][1] = pRgb2Rgb->rgb_mul_gr;
  rgb2rgb.matrix[0][2] = pRgb2Rgb->rgb_mul_br;

  rgb2rgb.matrix[1][0] = pRgb2Rgb->rgb_mul_rg;
  rgb2rgb.matrix[1][1] = pRgb2Rgb->rgb_mul_gg;
  rgb2rgb.matrix[1][2] = pRgb2Rgb->rgb_mul_bg;

  rgb2rgb.matrix[2][0] = pRgb2Rgb->rgb_mul_rb;
  rgb2rgb.matrix[2][1] = pRgb2Rgb->rgb_mul_gb;
  rgb2rgb.matrix[2][2] = pRgb2Rgb->rgb_mul_bb;

  rgb2rgb.offset[0] = pRgb2Rgb->rgb_oft_or;
  rgb2rgb.offset[1] = pRgb2Rgb->rgb_oft_og;
  rgb2rgb.offset[2] = pRgb2Rgb->rgb_oft_ob;

  DRV_ipipeSetRgb2Rgb2(&rgb2rgb);

  return 0;
}

short ALG_aewbSetOtfCorrect( int level )
{
	static int prevalue = -1;
	CSL_IpipeDpcConfig config;
	int levelD = 64;
	int levelC = 255;

	if( level == prevalue )
		return 0;

	prevalue = level;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: Sensor OTF Level = %d\n", level);
  #endif

  config.lutEnable						= 0;
  config.lutType							= 0;
  config.lutOption0CorMethod	= 0;
  config.lutStartAddr					= 0;
  config.lutNumEntries				= 0;
  config.lutAddr							= 0;

  config.otfEnable						= 1;
  config.otfType							= 0;
  config.otfAlg								= 0;
  config.otf2DetThres[0]			= level*levelD;
  config.otf2DetThres[1]			= level*levelD;
  config.otf2DetThres[2]			= level*levelD;
  config.otf2DetThres[3]			= level*levelD;

  config.otf2CorThres[0]			= level*levelC;
  config.otf2CorThres[1]			= level*levelC;
  config.otf2CorThres[2]			= level*levelC;
  config.otf2CorThres[3]			= level*levelC;


	config.otf3ActAdjust			= 0;
	config.otf3DetThres				= 0;
	config.otf3DetThresSlope	    = 0;
	config.otf3DetThresMin		    = 0;
	config.otf3DetThresMax		    = 0;
	config.otf3CorThres				= 0;
	config.otf3CorThresSlope	    = 0;
	config.otf3CorThresMin		    = 0;
	config.otf3CorThresMax		    = 0;

	DRV_ipipeSetDpcConfig(&config);
  return 0;
}

short ALG_aewbSetEdgeEnhancement(EDGE_PARAM  *pParm )
{
  CSL_IpipeEdgeEnhanceConfig config;
  static CSL_IpipeEdgeEnhanceConfig prev_config = {0};



  config.enable                   = pParm->yee_en;
  config.haloReduceEnable         = pParm->en_halo_red;
  config.mergeMethod              = pParm->merge_meth;
  config.hpfShift                 = pParm->yee_shf;
  config.hpfCoeff[0][0]           = pParm->yee_mul_00;
  config.hpfCoeff[0][1]           = pParm->yee_mul_01;
  config.hpfCoeff[0][2]           = pParm->yee_mul_02;
  config.hpfCoeff[1][0]           = pParm->yee_mul_10;
  config.hpfCoeff[1][1]           = pParm->yee_mul_11;
  config.hpfCoeff[1][2]           = pParm->yee_mul_12;
  config.hpfCoeff[2][0]           = pParm->yee_mul_20;
  config.hpfCoeff[2][1]           = pParm->yee_mul_21;
  config.hpfCoeff[2][2]           = pParm->yee_mul_22;
  config.lowerThres               = pParm->yee_thr;
  config.edgeSharpGain            = pParm->es_gain;
  config.edgeSharpHpValLowThres   = (pParm->es_thr1&0x3F);
  config.edgeSharpHpValUpLimit    = (pParm->es_thr2&0x3F);
  config.edgeSharpGainGradient    = pParm->es_gain_grad;
  config.edgeSharpOffsetGradient  = pParm->es_ofst_grad;
  config.table                    = yee_table;

  if(memcmp(&prev_config, &config, sizeof(config))==0)
    return 0;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: EDGE ENHANCEMENT \n");
  #endif

  memcpy(&prev_config, &config, sizeof(config));

  if(gALG_aewbObj.vnfDemoCfg == 0)
	DRV_ipipeSetEdgeEnhance(&config);

  return 0;
}

short ALG_aewbSetContrastBrightness(BRT_CRT_PARAM  *pParm )
{
	static BRT_CRT_PARAM  ParmSet;

	if( memcmp(&ParmSet, pParm, sizeof(BRT_CRT_PARAM))== 0 )
		return 0;

	ParmSet = *pParm;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: Contrast = %5d, Brightness = %5d\n", pParm->yuv_adj_ctr, pParm->yuv_adj_brt);
  #endif
  DRV_ipipeSetYoffet((pParm->yuv_adj_brt-128));

  DRV_ipipeSetContrastBrightness(pParm->yuv_adj_ctr, 0x0);

  return 0;
}

short ALG_aewbSetSensorBinSkip(int Is_binning)
{
  static int prevValue=-1;

  if(prevValue==Is_binning)
    return 0;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: Sensor BinOrSkip = %d\n", Is_binning);
  #endif

  prevValue = Is_binning;

  DRV_imgsBinEnable(Is_binning);

  return 0;
}

short ALG_aewbSetSensor50_60Hz(int Is50Hz)
{
  static int prevValue = -1;



  if(prevValue==Is50Hz)
    return 0;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: Sensor 50_60Hz = %d\n", Is50Hz);
  #endif

  prevValue = Is50Hz;

  DRV_imgsSet50_60Hz(Is50Hz);

  if( Is50Hz )
  {
		DRV_displaySetMode(DRV_DISPLAY_MODE_PAL);
  }else{
		DRV_displaySetMode(DRV_DISPLAY_MODE_NTSC);
  }

  return 0;
}

short ALG_aewbSetSensorFrameRate(int frame_rate_mode)
{
  static int prevValue=-1;

  if(prevValue==frame_rate_mode)
    return 0;

  #ifdef ALG_AEWB_DEBUG
  OSA_printf(" AEWB: Sensor frame-rate = %d\n", frame_rate_mode);
  #endif

  prevValue = frame_rate_mode;

  //DRV_imgsSetFramerate(frame_rate_mode);

  return 0;
}

void ALG_aewbGetAEValues(Int32 *exposureTime, Int32 *apertureLevel, Int32 *sensorGain, Int32 *ipipeGain)
{
    *exposureTime 	= gALG_aewbObj.AE_OutArgs.nextAe.exposureTime;
    *apertureLevel 	= gALG_aewbObj.AE_OutArgs.nextAe.apertureLevel;
    *sensorGain 	= gALG_aewbObj.AE_OutArgs.nextAe.sensorGain;
    *ipipeGain 		= gALG_aewbObj.AE_OutArgs.nextAe.ipipeGain;
}

void ALG_aewbGetAWBGains(Uint16 *rGain, Uint16 *grGain, Uint16 *gbGain, Uint16 *bGain)
{
	if(gALG_aewbObj.aewbVendor == ALG_AEWB_ID_APPRO) {
		*rGain= gALG_aewbObj.AWB_OutArgs.nextWb.rGain;
		*grGain= gALG_aewbObj.AWB_OutArgs.nextWb.gGain;
		*gbGain= gALG_aewbObj.AWB_OutArgs.nextWb.gGain;
		*bGain= gALG_aewbObj.AWB_OutArgs.nextWb.bGain;
	}
	else {
		*rGain= gIMAGE_TUNE_ctrl.curPrm.ipipePrm.wb.gain[0];
		*grGain= gIMAGE_TUNE_ctrl.curPrm.ipipePrm.wb.gain[1];
		*gbGain= gIMAGE_TUNE_ctrl.curPrm.ipipePrm.wb.gain[2];
		*bGain= gIMAGE_TUNE_ctrl.curPrm.ipipePrm.wb.gain[3];
	}
}

void ALG_aewbGetRgb2Rgb(Int16*matrix)
{
	Uint32 i;
	static Int16 approMatrix[9]={
			  551, -324, 29,
			 -39, 383, -88,
			  9, -207, 454
			};

	if(gALG_aewbObj.aewbVendor==ALG_AEWB_ID_APPRO) {
		for (i=0;i<9;i++)
     		matrix[i]= approMatrix[i];
	}
	else {
		matrix[0]= gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2rgb1.matrix[0][0];
		matrix[1]= gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2rgb1.matrix[0][1];
		matrix[2]= gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2rgb1.matrix[0][2];
		matrix[3]= gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2rgb1.matrix[1][0];
		matrix[4]= gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2rgb1.matrix[1][1];
		matrix[5]= gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2rgb1.matrix[1][2];
		matrix[6]= gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2rgb1.matrix[2][0];
		matrix[7]= gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2rgb1.matrix[2][1];
		matrix[8]= gIMAGE_TUNE_ctrl.curPrm.ipipePrm.rgb2rgb1.matrix[2][2];
	}

}
