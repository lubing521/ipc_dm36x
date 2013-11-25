
#include "alg_aewb_priv.h"
#if 0
void ALG_aewbSetExposureGain(int EX, int AG, int DG, int init)
{
    ALG_aewbSetSensorExposure(EX);
    ALG_aewbSetSensorGain(AG);
    ALG_aewbSetSensorDGain(DG);
}  
#else
void ALG_aewbSetExposureGain(int EX, int AG, int DG, int init)
{
    static int ag = 0, dg = 0, cnt = -1;
    if (init) 
    {
        cnt = 1;
        ALG_aewbSetSensorExposure(EX);
        ALG_aewbSetSensorGain(AG);
        dg = DG;
    }
    if (cnt == 0)
    {
        ALG_aewbSetIsifDGain(dg);
        cnt = -1;
    }
    if (cnt > 0) --cnt;
}
#endif
