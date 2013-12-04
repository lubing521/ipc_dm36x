/** ===========================================================================
* @file Appro_aew_xdm.h
*
* @path $(IPNCPATH)\multimedia\encode_stream\aew_xdm
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */

#ifndef _APPRO_AEW_XDM_H_
#define _APPRO_AEW_XDM_H_

#ifdef AEW_ENABLE
#undef AEW_ENABLE
#endif

#ifdef AEW_DISABLE
#undef AEW_DISABLE
#endif

enum {
	MANUAL_IRIS = 0,
	AUTO_IRIS
};

enum {
	AE_NIGHT = 0,
	AE_DAY
};

enum {

	INDOOR = 0,
	OUTDOOR,
	AWB_AUTO
};

enum {
	SATURATION_LOW = 0,
	SATURATION_NORMAL,
	SATURATION_HIGH
};

enum {
	BACKLIGHT_LOW = 0,
	BACKLIGHT_LOW2,
	BACKLIGHT_NORMAL,
	BACKLIGHT_NORMAL2,
	BACKLIGHT_HIGH,
	BACKLIGHT_HIGH2
};

enum {
	BRIGHTNESS_LOW,
	BRIGHTNESS_NORMAL,
	BRIGHTNESS_HIGH
};

enum {
	CONTRAST_LOW,
	CONTRAST_NORMAL,
	CONTRAST_HIGH
};

enum {
	SHARPNESS_LOW,
	SHARPNESS_NORMAL,
	SHARPNESS_HIGH
};

enum {
  	SENSOR_BINNING=0,
  	SENSOR_SKIP
};

enum {
  	VIDEO_NTSC=0,
  	VIDEO_PAL,
	VIDEO_NONE,
};

enum {
	AEW_DISABLE = 0,
	AEW_ENABLE
};


typedef struct {
	unsigned int rgb_mul_rr;
	unsigned int rgb_mul_gr;
	unsigned int rgb_mul_br;
    unsigned int rgb_mul_rg;
    unsigned int rgb_mul_gg;
    unsigned int rgb_mul_bg;
	unsigned int rgb_mul_rb;
	unsigned int rgb_mul_gb;
	unsigned int rgb_mul_bb;
	unsigned int rgb_oft_or;
	unsigned int rgb_oft_og;
	unsigned int rgb_oft_ob;

} RGB2RGB_PARAM;

typedef struct _AWB_GAIN_PARAM{
	unsigned int rGain;
	unsigned int grGain;
	unsigned int gbGain;
	unsigned int bGain;
	unsigned int dGain;
} AWB_PARAM;

typedef struct _BRT_CRT_PARAM{
	unsigned int yuv_adj_ctr;
	unsigned int yuv_adj_brt;
}BRT_CRT_PARAM;

typedef struct _EDGE_PARAM{
	int yee_en;
	int en_halo_red;
	int merge_meth;
	int yee_shf;
	int yee_mul_00;
	int yee_mul_01;
	int yee_mul_02;
	int yee_mul_10;
	int yee_mul_11;
	int yee_mul_12;
	int yee_mul_20;
	int yee_mul_21;
	int yee_mul_22;
	int	yee_thr;
	int es_gain;
	int es_thr1;
	int es_thr2;
	int es_gain_grad;
	int es_ofst_grad;
	unsigned int *ee_table;
}EDGE_PARAM;

typedef struct _AEW_EXT_PARAM{
	short 			(*GAIN_SETUP)(int);
	short 			(*SHUTTER_SETUP)(int);
	short 			(*AWB_SETUP)(AWB_PARAM *);
	short 			(*DCSUB_SETUP)(int);
	short 			(*BIN_SETUP)(int);
	short 			(*RGB2RGB_SETUP)(RGB2RGB_PARAM  *);
	short			(*MEDIAN_SETUP)(int);
	short			(*EDGE_SETUP)(EDGE_PARAM *);
	short			(*BRT_CTR_SET)(BRT_CRT_PARAM *);
	short			(*BINNING_SKIP_SETUP)(int);
	short			(*ENV_50_60HZ_SETUP)(int);
	short			(*SENSOR_FRM_RATE_SETUP)(int);
	unsigned short 	*H3ABuffer;
	int				auto_iris;
	int				day_night;
	int				awb_mode;
	int				saturation;
	int				blc;
	int				sharpness;
	int				brightness;
	int				contrast;
	int				binning_mode;
	int				env_50_60Hz;
	int				aew_enable;
	int				special_mode;
	int				sensor_frame_rate;
	int 			ipipe_dgain_base;
} AEW_EXT_PARAM;

typedef struct _aewDataEntry {
	unsigned short window_data[8][8];
	unsigned short unsat_block_ct[8];
}aewDataEntry;

int xDM_2A_Init(void);
void xDM_2A_Free(void);
int ApproWBSetPrm_xDM(int mode);
void DRVApproSend3A_xDM(void);
void Appro3Afunction_xDM(void);

extern AEW_EXT_PARAM 		Aew_ext_parameter;
extern int					g_bEnableTurnColor;

#endif

