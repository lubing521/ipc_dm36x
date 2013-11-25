#ifndef AEWB_TI_
#define AEWB_TI_

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_STEPS             6
#define NUM_RGB2RGB_MATRIXES  10


struct rgb2rgb_index {
    int color_temp;
    RGB2RGB_PARAM rgb2rgbparam;
    RGB2RGB_PARAM rgb2rgb2param;
};

extern unsigned char TI_WEIGHTING_SPOT[];
extern unsigned char TI_WEIGHTING_CENTER[];
extern unsigned char TI_WEIGHTING_MATRIX[];
extern Uint32 TI_YEE_TABLE[];


int TI_2A_config(int flicker_detection, int saldre);
void TI_2A_init_tables(int width, int height);
void TI2A_applySettings(IAEWB_Ae *curAe, IAEWB_Ae *nextAe, IAEWB_Wb* nextWb, int numSmoothSteps, int step);
short TI_2A_SetEEValues(int shift_val);
int TI_2A_ISP_control(int eTime, int aGain, int dGain, int cTemp);
void TI_2A_AE_config(AE_TIAR0331_DynamicParams *aeDynamicParams, int min_exp, int step_size);

#ifdef __cplusplus
}
#endif

#endif
