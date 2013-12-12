
#include "alg_aewb_priv.h"

extern awb_calc_data_t awb_calc_data_imx122;
extern int   TI_2A_config_imx122(int flicker_detection, int saldre);
extern short TI_2A_SetEEValues_imx122(int shift_val);
extern int   TI_2A_ISP_control_imx122(int eTime, int aGain, int dGain, int cTemp);
extern void  TI_2A_AE_config_imx122(IAE_DynamicParams * aeDynamicParams, int min_exp, int step_size);
extern int   TI_fd_get_config_imx122(int sensorMode, int* row_time, int* pinp, int* h3aWinHeight);
extern int   TI_fd_trigger_imx122(IAEWB_Ae *curAe, IAEWB_Ae *nextAe);

void ALG_aewbSetExposureGain_imx122(int EX, int AG, int DG, int init)
{
    if (init)
    {
        ALG_aewbSetSensorExposure(EX);
        ALG_aewbSetSensorGain(AG);
        ALG_aewbSetIsifDGain(DG);
    }
} 

static void TI_Awb_imx122(AWB_PARAM * awb_gain, int awb_mode)
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
        awb_gain->grGain -= 6;
        awb_gain->gbGain -= 6;
        awb_gain->bGain  -= 6;
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

ALG_TI_Aewb_Priv TI_AWEB_IMX122 =
{
    .SetExposureGain = ALG_aewbSetExposureGain_imx122,
    .SetEEValues     = TI_2A_SetEEValues_imx122,
    .AE_config       = TI_2A_AE_config_imx122,
    .ISP_control     = TI_2A_ISP_control_imx122,
    .fd_get_config   = TI_fd_get_config_imx122,
    .fd_trigger      = TI_fd_trigger_imx122,
    .awb_adjust      = TI_Awb_imx122,
    .calc_data       = &awb_calc_data_imx122
};

