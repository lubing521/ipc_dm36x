#ifndef _ALG_TI_AEWB_PRIV_H_
#define _ALG_TI_AEWB_PRIV_H_

#define NUM_OF_REF_1		30  //maximal number of reference 1, actual use 17                          
#define NUM_OF_REF_2       	15  //maximal number of reference 2, actual use 7
#define NUM_OF_GRAY      	 4  //maximal number of gray 2 used, actual use 4

#if 1
typedef struct
{
	// Sensor specific calibration data
	
	unsigned int	num_of_ref_1;
	unsigned int	num_of_ref_2;
	unsigned int	num_of_gray;

	unsigned int	color_temp_1[NUM_OF_REF_1];

	int	wbReferenceCb[NUM_OF_REF_1][NUM_OF_GRAY];
	int	wbReferenceCr[NUM_OF_REF_1][NUM_OF_GRAY];

	unsigned int	ref_gray_R_1[NUM_OF_REF_1][NUM_OF_GRAY];
	unsigned int	ref_gray_G_1[NUM_OF_REF_1][NUM_OF_GRAY];
	unsigned int	ref_gray_B_1[NUM_OF_REF_1][NUM_OF_GRAY];

	unsigned int	ref_index_2[NUM_OF_REF_2];
	unsigned int	color_temp_2[NUM_OF_REF_2];

	unsigned int	img_ref[NUM_OF_REF_2 * 1120];

	int	referencesCb_2[NUM_OF_REF_2][NUM_OF_GRAY];
	int	referencesCr_2[NUM_OF_REF_2][NUM_OF_GRAY];

	unsigned int	ref_gray_R_2[NUM_OF_REF_2][NUM_OF_GRAY];
	unsigned int	ref_gray_G_2[NUM_OF_REF_2][NUM_OF_GRAY];
	unsigned int	ref_gray_B_2[NUM_OF_REF_2][NUM_OF_GRAY];


	// Sensor specific tuning paramaters

	unsigned int	radius; 

	int	luma_awb_min;
	int	luma_awb_max;

	unsigned int	low_color_temp_thresh;
	
	unsigned int	apply_rgb_adjust;

	int	R_adjust;
	int	B_adjust;

	unsigned int	SB_1;
	unsigned int	SB_2;

	unsigned int	SB_low_bound;

	unsigned int	default_T_H;
	unsigned int	default_T_MH;
	unsigned int	default_T_ML;
	unsigned int	default_T_L;

	unsigned int	default_T_H_index;
	unsigned int	default_T_MH_index;
	unsigned int	default_T_ML_index;
	unsigned int	default_T_L_index;	

	unsigned int	best_gray_index_default;

} awb_calc_data_t;

#endif 

#if 0

typedef struct
{
	// Sensor specific calibration data
	
	unsigned char	num_of_ref_1;
	unsigned char	num_of_ref_2;
	unsigned char	num_of_gray;

	unsigned short	color_temp_1[NUM_OF_REF_1];

	short	wbReferenceCb[NUM_OF_REF_1][NUM_OF_GRAY];
	short	wbReferenceCr[NUM_OF_REF_1][NUM_OF_GRAY];

	unsigned short	ref_gray_R_1[NUM_OF_REF_1][NUM_OF_GRAY];
	unsigned short	ref_gray_G_1[NUM_OF_REF_1][NUM_OF_GRAY];
	unsigned short	ref_gray_B_1[NUM_OF_REF_1][NUM_OF_GRAY];

	unsigned char	ref_index_2[NUM_OF_REF_2];
	unsigned short	color_temp_2[NUM_OF_REF_2];

	unsigned short	img_ref[NUM_OF_REF_2 * 1120];

	short	referencesCb_2[NUM_OF_REF_2][NUM_OF_GRAY];
	short	referencesCr_2[NUM_OF_REF_2][NUM_OF_GRAY];

	unsigned short	ref_gray_R_2[NUM_OF_REF_2][NUM_OF_GRAY];
	unsigned short	ref_gray_G_2[NUM_OF_REF_2][NUM_OF_GRAY];
	unsigned short	ref_gray_B_2[NUM_OF_REF_2][NUM_OF_GRAY];


	// Sensor specific tuning paramaters

	unsigned short	radius; 

	int	luma_awb_min;
	int	luma_awb_max;

	unsigned short	low_color_temp_thresh;
	
	unsigned char	apply_rgb_adjust;

	short	R_adjust;
	short	B_adjust;

	unsigned short	SB_1;
	unsigned short	SB_2;

	unsigned short	SB_low_bound;

	unsigned short	default_T_H;
	unsigned short	default_T_MH;
	unsigned short	default_T_ML;
	unsigned short	default_T_L;

	unsigned char	default_T_H_index;
	unsigned char	default_T_MH_index;
	unsigned char	default_T_ML_index;
	unsigned char	default_T_L_index;	

	unsigned char	best_gray_index_default;

} awb_calc_data_t;
#endif

extern awb_calc_data_t awb_calc_data;


#endif

