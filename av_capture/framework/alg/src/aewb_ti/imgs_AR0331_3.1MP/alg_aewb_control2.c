
#include "alg_aewb_priv.h"
void ALG_aewbSetExposureGain(int EX, int AG, int DG, int init)
{
    if (init)
    {
        ALG_aewbSetSensorExposure(EX);
        ALG_aewbSetSensorGain(AG);
        ALG_aewbSetIsifDGain(DG);
    }
}  

