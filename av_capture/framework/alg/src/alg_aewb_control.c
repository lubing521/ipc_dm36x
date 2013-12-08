
#include "alg_aewb_priv.h"
#include "imageTunePriv.h"
#include <drv_capture.h>
#include <drv_display.h>

// add by fan
//#define ALG_AEWB_DEBUG

extern IMAGE_TUNE_Ctrl gIMAGE_TUNE_ctrl;

static Uint32 TI_YEE_TABLE[1024] = 
{
    0   ,0   ,0   ,1   ,1   ,1   ,1   ,1   ,2   ,2   ,2   ,2   ,2   ,3   ,3   ,3   ,
    3   ,3   ,4   ,4   ,4   ,4   ,4   ,5   ,5   ,5   ,5   ,5   ,6   ,6   ,6   ,6   ,
    6   ,7   ,7   ,7   ,7   ,7   ,8   ,8   ,8   ,8   ,8   ,9   ,9   ,9   ,9   ,9   ,
    10  ,10  ,10  ,10  ,10  ,11  ,11  ,11  ,11  ,11  ,12  ,12  ,12  ,12  ,12  ,13  ,
    13  ,13  ,13  ,13  ,14  ,14  ,14  ,14  ,14  ,15  ,15  ,15  ,15  ,15  ,16  ,16  ,
    16  ,16  ,16  ,17  ,17  ,17  ,17  ,17  ,18  ,18  ,18  ,18  ,18  ,19  ,19  ,19  ,
    19  ,19  ,20  ,20  ,20  ,20  ,20  ,21  ,21  ,21  ,21  ,21  ,22  ,22  ,22  ,22  ,
    22  ,23  ,23  ,23  ,23  ,23  ,24  ,24  ,24  ,24  ,24  ,25  ,25  ,25  ,25  ,25  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,26  ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,-26 ,
    -26 ,-25 ,-25 ,-25 ,-25 ,-25 ,-24 ,-24 ,-24 ,-24 ,-24 ,-23 ,-23 ,-23 ,-23 ,-23 ,
    -22 ,-22 ,-22 ,-22 ,-22 ,-21 ,-21 ,-21 ,-21 ,-21 ,-20 ,-20 ,-20 ,-20 ,-20 ,-19 ,
    -19 ,-19 ,-19 ,-19 ,-18 ,-18 ,-18 ,-18 ,-18 ,-17 ,-17 ,-17 ,-17 ,-17 ,-16 ,-16 ,
    -16 ,-16 ,-16 ,-15 ,-15 ,-15 ,-15 ,-15 ,-14 ,-14 ,-14 ,-14 ,-14 ,-13 ,-13 ,-13 ,
    -13 ,-13 ,-12 ,-12 ,-12 ,-12 ,-12 ,-11 ,-11 ,-11 ,-11 ,-11 ,-10 ,-10 ,-10 ,-10 ,
    -10 ,-9  ,-9  ,-9  ,-9  ,-9  ,-8  ,-8  ,-8  ,-8  ,-8  ,-7  ,-7  ,-7  ,-7  ,-7  ,
    -6  ,-6  ,-6  ,-6  ,-6  ,-5  ,-5  ,-5  ,-5  ,-5  ,-4  ,-4  ,-4  ,-4  ,-4  ,-3  ,
    -3  ,-3  ,-3  ,-3  ,-2  ,-2  ,-2  ,-2  ,-2  ,-1  ,-1  ,-1  ,-1  ,-1  ,0   ,0   ,
};

#define ENABLE_COMPENSATION  (200) // 100 = 1.00x compensation i.e none, 150 = 1.5x compensation

CSL_IpipeRgb2YuvConfig rgb2yuv_color = 
{
    .matrix = {
        { 0x004d, 0x0096, 0x001d },
        { 0x0fd5, 0x0fab, 0x0080 },
        { 0x0080, 0x0f95, 0x0feb },
    },
    .offset = {
        0x00, 0x80, 0x80
    },

    .cLpfEnable = FALSE,
    .cPos = CSL_IPIPE_YUV_CHROMA_POS_LUM,
};

CSL_IpipeRgb2YuvConfig rgb2yuv_mono = 
{
    .matrix = {
        { 0x004d, 0x0096, 0x001d },
        { 0x0, 0x0, 0x0 },
        { 0x0, 0x0, 0x0 },
    },
    .offset = {
        0x00, 0x80, 0x80
    },

    .cLpfEnable = FALSE,
    .cPos = CSL_IPIPE_YUV_CHROMA_POS_LUM,
};

CSL_IpipeRgb2YuvConfig* rgb2yuv[] = 
{
    &rgb2yuv_color,
    &rgb2yuv_mono,
};

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

  drvImgsFunc->imgsSetAgain(gain32, 0);

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
  drvImgsFunc->imgsSetDgain(gain);

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

  drvImgsFunc->imgsSetEshutter(shutter32, 0);

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
   	drvImgsFunc->imgsBinMode(bin=0x20);
  else
   	drvImgsFunc->imgsBinMode(bin);

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

short ALG_aewbSetEdgeEnhancement(EDGE_PARAM *pParm)
{
    static int es_gain = -1;
    static CSL_IpipeEdgeEnhanceConfig config =
    {
    .enable                   = TRUE,
    .haloReduceEnable         = TRUE,

    .mergeMethod              = CSL_IPIPE_YEE_EE_ES_MERGING_METHOD_SUMMATION,
    .hpfShift                 = 3,//shift=3 for 1080P/720P, shift=4 for D1
    .hpfCoeff[0][0]           = 48,
    .hpfCoeff[0][1]           = 12,
    .hpfCoeff[0][2]           = 1024-10,
    .hpfCoeff[1][0]           = 12,
    .hpfCoeff[1][1]           = 0,
    .hpfCoeff[1][2]           = 1024-6,
    .hpfCoeff[2][0]           = 1024-10,
    .hpfCoeff[2][1]           = 1024-6,
    .hpfCoeff[2][2]           = 1024-2,

    .lowerThres               = 8,//32; //changed based on Gang Comment
    .edgeSharpGain            = 0,
    .edgeSharpHpValLowThres   = 255,
    .edgeSharpHpValUpLimit    = 255,
    .edgeSharpGainGradient    = 64, 
    .edgeSharpOffsetGradient  = 32,  
    .table                    = TI_YEE_TABLE,//NULL;
    };

    if (pParm->es_gain == es_gain)
    {
        return 0;
    }
    es_gain = pParm->es_gain; 
    
    config.edgeSharpGain = (pParm->es_gain > 0xE0) ? 0xE0 : pParm->es_gain;
    config.edgeSharpGain = (pParm->es_gain < 0x10) ? 0x10 : pParm->es_gain;
    
//#ifdef ALG_AEWB_DEBUG
    OSA_printf(" AEWB: EDGE ENHANCEMENTEE_Param es_gain = %d\n", config.edgeSharpGain);
//#endif

    if (gALG_aewbObj.vnfDemoCfg == 0)
    {
        DRV_ipipeSetEdgeEnhance(&config);
    }

    return 0;
}

int g_bEnableTurnColor = 0;
void ALG_aewbSetDayNight(void)
{
    static int daynight_bak = -1;
    int daynight = 0;

    daynight = g_bEnableTurnColor; ////0 代表彩色，1代表黑色 

    if (daynight == daynight_bak || daynight > 1 || daynight < 0)
    {
        return;      
    }
    daynight_bak = daynight;
    
//#if ALG_AEWB_DEBUG
    OSA_printf("AEWB: daynight = %s\n", daynight == 0 ? "COLOR_IRCUT" : "MONO_IRCUT");
//#endif
    rgb2yuv[daynight]->offset[0] = Aew_ext_parameter.brightness-128;
    DRV_ipipeSetRgb2Yuv(rgb2yuv[daynight]);
}

short ALG_aewbSetBrightness(int Yoffset)
{
    static int YoffsetBak = -1;
    
    if (YoffsetBak == Yoffset)
    {
        return 0;
    }
    
    YoffsetBak = Yoffset;

    int daynight = g_bEnableTurnColor;
//#ifdef ALG_AEWB_DEBUG   
OSA_printf("AEWB: Brightness = %5d\n", Yoffset-128);
//#endif 
    rgb2yuv[daynight]->offset[0] = Yoffset-128;
    DRV_ipipeSetRgb2Yuv(rgb2yuv[daynight]);
    return 0;
}

short ALG_aewbSetContrast(int Contrast)
{
    static int ContrastBak = -1;

    if (ContrastBak == Contrast)
    {
        return 0;
    }

    ContrastBak = Contrast;

//#ifdef ALG_AEWB_DEBUG
    OSA_printf(" AEWB: Contrast = %5d", Contrast);
//#endif
    DRV_ipipeSetContrastBrightness(Contrast, 0x0);
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

  drvImgsFunc->imgsBinEnable(Is_binning);

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

  drvImgsFunc->imgsSet50_60Hz(Is50Hz);

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

  //drvImgsFunc->imgsSetFramerate(frame_rate_mode);

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

