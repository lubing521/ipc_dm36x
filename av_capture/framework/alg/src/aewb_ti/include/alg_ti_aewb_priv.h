#ifndef _ALG_TI_AEWB_PRIV_H_
#define _ALG_TI_AEWB_PRIV_H_

#define NUM_OF_REF_1        30  //maximal number of reference 1, actual use 17                          
#define NUM_OF_REF_2        15  //maximal number of reference 2, actual use 7
#define NUM_OF_GRAY          4  //maximal number of gray 2 used, actual use 4

typedef struct
{
    // Sensor specific calibration data
    unsigned int    num_of_ref_1;
    unsigned int    num_of_ref_2;
    unsigned int    num_of_gray;

    unsigned int    color_temp_1[NUM_OF_REF_1];

    int wbReferenceCb[NUM_OF_REF_1][NUM_OF_GRAY];
    int wbReferenceCr[NUM_OF_REF_1][NUM_OF_GRAY];

    unsigned int    ref_gray_R_1[NUM_OF_REF_1][NUM_OF_GRAY];
    unsigned int    ref_gray_G_1[NUM_OF_REF_1][NUM_OF_GRAY];
    unsigned int    ref_gray_B_1[NUM_OF_REF_1][NUM_OF_GRAY];

    unsigned int    ref_index_2[NUM_OF_REF_2];
    unsigned int    color_temp_2[NUM_OF_REF_2];

    unsigned int    img_ref[NUM_OF_REF_2 * 1120];

    int referencesCb_2[NUM_OF_REF_2][NUM_OF_GRAY];
    int referencesCr_2[NUM_OF_REF_2][NUM_OF_GRAY];

    unsigned int    ref_gray_R_2[NUM_OF_REF_2][NUM_OF_GRAY];
    unsigned int    ref_gray_G_2[NUM_OF_REF_2][NUM_OF_GRAY];
    unsigned int    ref_gray_B_2[NUM_OF_REF_2][NUM_OF_GRAY];


    // Sensor specific tuning paramaters
    unsigned int    radius; 

    int luma_awb_min;
    int luma_awb_max;

    unsigned int    low_color_temp_thresh;

    unsigned int    apply_rgb_adjust;

    int R_adjust;
    int B_adjust;

    unsigned int    SB_1;
    unsigned int    SB_2;

    unsigned int    SB_low_bound;

    unsigned int    default_T_H;
    unsigned int    default_T_MH;
    unsigned int    default_T_ML;
    unsigned int    default_T_L;

    unsigned int    default_T_H_index;
    unsigned int    default_T_MH_index;
    unsigned int    default_T_ML_index;
    unsigned int    default_T_L_index;  

    unsigned int    best_gray_index_default;

} awb_calc_data_t;

extern awb_calc_data_t awb_calc_data_ov9712;
extern awb_calc_data_t awb_calc_data_ar130;
extern awb_calc_data_t awb_calc_data_MT9M034;
extern awb_calc_data_t awb_calc_data_ar0330;
extern awb_calc_data_t awb_calc_data_ar0331;

extern awb_calc_data_t awb_calc_data_MT9P031;
extern awb_calc_data_t awb_calc_data_imx122;

#endif

