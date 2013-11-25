
#include "alg_aewb_priv.h"
#if 1
void ALG_aewbSetExposureGain(int EX, int AG, int DG, int init)
{
    static int ag = 0, dg = 0, cnt = -1;
    if (init) 
    {
        cnt = 2;
        ALG_aewbSetSensorExposure(EX);
        ag = AG;
        dg = DG;
    }
    if (cnt == 0)
    {
        ALG_aewbSetSensorGain(ag);
        ALG_aewbSetIsifDGain(dg);
        cnt = -1;
    }
    if (cnt > 0) --cnt;
}
#else
void ALG_aewbSetExposureGain(int EX, int AG, int DG, int init)
{
    static int ag = 0, dg = 0, cnt = -1;
    if (init) 
    {
        cnt = 1;
        DRV_imgsSetEshutterAGain(EX, AG);
        ag = AG;
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
