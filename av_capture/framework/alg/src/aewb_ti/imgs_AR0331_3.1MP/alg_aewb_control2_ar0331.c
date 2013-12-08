
#include "alg_aewb_priv.h"

extern awb_calc_data_t awb_calc_data_ar0331;
extern int   TI_2A_config_ar0331(int flicker_detection, int saldre);
extern short TI_2A_SetEEValues_ar0331(int shift_val);
extern int   TI_2A_ISP_control_ar0331(int eTime, int aGain, int dGain, int cTemp);
extern void  TI_2A_AE_config_ar0331(IAE_DynamicParams * aeDynamicParams, int min_exp, int step_size);
extern int   TI_fd_get_config_ar0331(int sensorMode, int* row_time, int* pinp, int* h3aWinHeight);
extern int   TI_fd_trigger_ar0331(IAEWB_Ae *curAe, IAEWB_Ae *nextAe);

void ALG_aewbSetExposureGain_ar0331(int EX, int AG, int DG, int init)
{
    if (init)
    {
        ALG_aewbSetSensorExposure(EX);
        ALG_aewbSetSensorGain(AG);
        ALG_aewbSetIsifDGain(DG);
    }
}  

static void TFC_Awb_ar0331(AWB_PARAM * awb_gain, int awb_mode)
{
    if (awb_mode == AWB_AUTO)
    {    
    }
    else if (awb_mode == OUTDOOR)
    {
        awb_gain->grGain += 6;
        awb_gain->gbGain += 6;
        awb_gain->rGain  += 6;
    }
    else if (awb_mode == INDOOR)
    {
        awb_gain->grGain -= 12;
        awb_gain->gbGain -= 12;
        awb_gain->bGain  -= 12;
    }
    else if (awb_mode == FLUORESCENT)
    {
        awb_gain->grGain -= 6;
        awb_gain->gbGain -= 6;
        awb_gain->rGain  -= 6;
    }
    else if(awb_mode == MANUAL)
    {
        awb_gain->rGain += (Aew_ext_parameter.iRedGain - 50)*3/2;
        awb_gain->bGain += (Aew_ext_parameter.iBlueGain - 50)*3/2;
    }

}

ALG_TI_Aewb_Priv TI_AWEB_ar0331 =
{
    .SetExposureGain = ALG_aewbSetExposureGain_ar0331,
    .SetEEValues     = TI_2A_SetEEValues_ar0331,
    .AE_config       = TI_2A_AE_config_ar0331,
    .ISP_control     = TI_2A_ISP_control_ar0331,
    .fd_get_config   = TI_fd_get_config_ar0331,
    .fd_trigger      = TI_fd_trigger_ar0331,
    .awb_adjust      = TFC_Awb_ar0331,
    .calc_data       = &awb_calc_data_ar0331
};

