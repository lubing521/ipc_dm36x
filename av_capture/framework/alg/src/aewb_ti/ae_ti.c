
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iae.h"

#include "ae_ti.h"
#include "ae_ti_priv.h"

//#define AE_DEBUG

extern IALG_Fxns AE_TI_IALG;
//#define __DEBUG
#ifdef __DEBUG
#define AE_DEBUG_PRINTS
#define dprintf printf
#else
#define dprintf
#endif
#define IALGFXNS  \
    &AE_TI_IALG,        /* module ID */                         \
    NULL,               /* activate */                          \
    AE_TI_alloc,        /* alloc */                             \
    NULL,               /* control (NULL => no control ops) */  \
    NULL,               /* deactivate */                        \
    AE_TI_free,         /* free */                              \
    AE_TI_init,         /* init */                              \
    NULL,               /* moved */                             \
    NULL                        /* numAlloc (NULL => IALG_MAXMEMRECS) */

/*
 *  ======== AE_TI_IAE ========
 *  This structure defines TI's implementation of the IAE interface
 *  for the AE_TI module.
 */
IAE_Fxns AE_TI_AE =             /* module_vendor_interface */
{
    {IALGFXNS},
    AE_TI_process,
    AE_TI_control,
};

/*
 *  ======== AE_TI_IALG ========
 *  This structure defines TI's implementation of the IALG interface
 *  for the AE_TI module.
 */
IALG_Fxns AE_TI_IALG =          /* module_vendor_interface */
{
    IALGFXNS
};

/*
 *  ======== AE_TI_alloc ========
 */
Int AE_TI_alloc(const IALG_Params *algParams,
                IALG_Fxns **pf, IALG_MemRec memTab[])
{
    IAE_Params *params = (IAE_Params *) algParams;
    int numTabs = 1;

    /* Request memory for my object */
    memTab[0].size      = sizeof(AE_TI_Obj);
    memTab[0].alignment = 0;
    memTab[0].space     = IALG_EXTERNAL;
    memTab[0].attrs     = IALG_PERSIST;

    memTab[1].size      = sizeof(XDAS_UInt32) * (params->numHistory + 1);
    memTab[1].alignment = 0;
    memTab[1].space     = IALG_EXTERNAL;
    memTab[1].attrs     = IALG_PERSIST;
    numTabs++;
    return (numTabs);
}

/*
 *  ======== AE_TI_free ========
 */
Int AE_TI_free(IALG_Handle handle, IALG_MemRec memTab[])
{
    AE_TI_Obj *h = (AE_TI_Obj *) handle;
    int numTabs = 0;
    /* Request memory for my object */
    memTab[0].size      = sizeof(AE_TI_Obj);
    memTab[0].alignment = 0;
    memTab[0].space     = IALG_EXTERNAL;
    memTab[0].attrs     = IALG_PERSIST;

    memTab[1].size      = sizeof(XDAS_UInt32) * h->numHistory + 1;
    memTab[1].alignment = 0;
    memTab[1].space     = IALG_EXTERNAL;
    memTab[1].attrs     = IALG_PERSIST;
    numTabs++;
    return (numTabs);
}

/*
 *  ======== AE_TI_initObj ========
 */
Int AE_TI_init(IALG_Handle handle,
               const IALG_MemRec memTab[], IALG_Handle p,
               const IALG_Params *algParams)
{
    AE_TI_Obj *h = (AE_TI_Obj *) handle;
    IAE_Params *params = (IAE_Params *) algParams;

    if (handle == NULL)
    {
        return (IAE_EFAIL);
    }
    if (params == NULL)
    {
        /* no static parameters passed in, use default */
        h->numHistory = 1;
        h->numSmoothSteps = 1;
        h->historyBrightness = NULL;
    }
    else if (params->size != sizeof(IAE_Params))
    {
        return (IAE_EUNSUPPORTED);
    }
    else
    {
        int i = 0;
        h->numHistory = params->numHistory + 1;
        h->numSmoothSteps = params->numSmoothSteps;
        if (h->numSmoothSteps < 0) h->numSmoothSteps = 1;
        if (h->numSmoothSteps > 10) h->numSmoothSteps = 10;
        h->historyBrightness = memTab[1].base;
        for (i = 0; i < h->numHistory; i++)
        {
            h->historyBrightness[i] = -1;
        }
        h->avgY = -1;
    }
    h->numRanges = 0;
    h->exposureTimeStepSize = 1;
    h->targetBrightness = 40;
    h->targetBrightnessRange.min = h->targetBrightness - 5;
    h->targetBrightnessRange.max = h->targetBrightness + 5;
    h->thrld = 10;
    h->locked = FALSE;
    h->lockCnt = 0;

    return (IAE_EOK);
}

static int clip_int(int v, int vmin, int vmax)
{
    if (v < vmin)
    {
        v = vmin;
    }
    if (v > vmax)
    {
        v = vmax;
    }
    return v;
}


static void search_in_range(float ex,
                            int Amin, int Amax, int *pA,
                            int Tmin, int Tmax, int Tstep, int *pT,
                            int AGmin, int AGmax, int *pAG,
                            int DGmin, int DGmax, int *pDG)
{
    ex *= Amin;
    *pA = clip_int(ex, Amin, Amax);
    ex /= (*pA);

    ex *= Tmin;
    *pT = clip_int((int)(ex / Tstep) * Tstep, Tmin, Tmax);
    ex /= (*pT);

    ex *= AGmin;
    *pAG = clip_int(ex, AGmin, AGmax);
    ex /= *pAG;

    ex *= DGmin;
    *pDG = clip_int(ex, DGmin, DGmax);
    ex /= (*pDG);

}

static int search_range_ATG_dec_gain(int adjRatio,
                                     int cA, int cT, int cAG, int cDG,
                                     IAE_Range rA, IAE_Range rT, IAE_Range rAG,
                                     IAE_Range rDG, int Tstep, int *pA, int *pT,
                                     int *pAG, int *pDG)
{
    int Tmin = (rT.min + Tstep - 1) / Tstep * Tstep;
    int Tmax = rT.max / Tstep * Tstep;
    if (Tmin > Tmax || Tmin <= 0)
    {
        Tmin = Tmax;
    }
#ifdef AE_DEBUG
    printf("\nAE debug: dec Tmin=%d, Tmax=%d, Tstep=%d\n", Tmin, Tmax, Tstep);
#endif
    float tm = (float)rA.min * Tmin * rAG.min * rDG.min;
    float tc = (float)cA * cT * cAG * cDG;

    if (tm * 1024 <= adjRatio * tc)
    {
        float ex = tc * adjRatio / 1024 / tm;
        search_in_range(ex,
                        rA.min, rA.max, pA,
                        Tmin, Tmax, Tstep, pT,
                        rAG.min, rAG.max, pAG,
                        rDG.min, rDG.max, pDG);
        return 1;
    }
    *pA = rA.min;
    *pT = Tmin;
    *pAG = rAG.min;
    *pDG = rDG.min;

    return 0;
}

static int search_range_ATG_inc_gain(int adjRatio,
                                     int cA, int cT, int cAG, int cDG,
                                     IAE_Range rA, IAE_Range rT, IAE_Range rAG,
                                     IAE_Range rDG, int Tstep, int *pA, int *pT,
                                     int *pAG, int *pDG)
{
    int Tmin = (rT.min + Tstep - 1) / Tstep * Tstep;
    int Tmax = rT.max / Tstep * Tstep;
    if (Tmin > Tmax || Tmin <= 0)
    {
        Tmin = Tmax;
    }
#ifdef AE_DEBUG
    printf("\nAE debug: inc Tmin=%d, Tmax=%d, Tstep=%d\n", Tmin, Tmax, Tstep);
#endif
    float tm = (float)rA.max * Tmax * rAG.max * rDG.max;
    float tc = (float)cA * cT * cAG * cDG;

    if (tm * 1024 >= adjRatio * tc)
    {
        float ex = tc * adjRatio / 1024 / rA.min / Tmin / rAG.min / rDG.min;
        search_in_range(ex,
                        rA.min, rA.max, pA,
                        Tmin, Tmax, Tstep, pT,
                        rAG.min, rAG.max, pAG,
                        rDG.min, rDG.max, pDG);
        return 1;
    }
    *pA = rA.max;
    *pT = Tmax;
    *pAG = rAG.max;
    *pDG = rDG.max;

    return 0;
}

/*
 *  ======== AE_TI_process ========
 */
#define SAT_Y              180
XDAS_Int32 AE_TI_process(IAE_Handle handle, IAE_InArgs *inArgs,
                         IAE_OutArgs *outArgs, IAEWB_Rgb *rgbData,
                         XDAS_UInt8 *weight, void *customData)
{
#ifdef AE_DEBUG_PRINTS
    static int frames = 0;
#endif
    int width = inArgs->statMat.winCtHorz;
    int height = inArgs->statMat.winCtVert;
    int numPixels = inArgs->statMat.pixCtWin;
    int i;
    int j;
    unsigned int redSum = 0;
    unsigned int blueSum = 0;
    unsigned int greenSum = 0;
    unsigned int redSum_unsat = 0;
    unsigned int blueSum_unsat = 0;
    unsigned int greenSum_unsat = 0;
    unsigned int totalY, totalY_unsat;
    unsigned int avgY;
    unsigned int weightSum = 0;
    unsigned int weightSum_unsat = 0;
    unsigned int rY, bY, gY;
    unsigned int nT, cT = inArgs->curAe.exposureTime;
    unsigned int nAG, cAG = inArgs->curAe.sensorGain;
    unsigned int nA, cA = inArgs->curAe.apertureLevel;
    unsigned int nDG, cDG = inArgs->curAe.ipipeGain;
    unsigned int curY, curY_unsat, curY_sat;
    AE_TI_Obj *h = (AE_TI_Obj *) handle;
    int adjRatio;
    unsigned int y, max_y = 0, min_y = 255 * numPixels;

    int reset = ((h->avgY == -1) || (inArgs->curAe.exposureTime % h->exposureTimeStepSize != 0));

    /* now calculate the average weighted luminance value */
    rY = (RY * inArgs->curWb.rGain + 512) >> 10;
    gY = (GY * inArgs->curWb.gGain + 512) >> 10;
    bY = (BY * inArgs->curWb.bGain + 512) >> 10;

#ifdef AE_DEBUG_PRINTS
    dprintf("ae: awb gains = %d, %d, %d, numPixels = %d, %d, %d\n",
            inArgs->curWb.rGain, inArgs->curWb.gGain, inArgs->curWb.bGain,
            numPixels, width, height);
    dprintf("ae: ipipe gains = %d, %d, %d\n", inArgs->curAe.ipipeGain,
            inArgs->curAe.exposureTime, inArgs->curAe.sensorGain);
#endif

    /* first calculate sumation of all R, G, B values */
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            redSum +=
                ((unsigned int) rgbData[i * width + j].r *
                 weight[i * width + j]);
            greenSum +=
                ((unsigned int) rgbData[i * width + j].g *
                 weight[i * width + j]);
            blueSum +=
                ((unsigned int) rgbData[i * width + j].b *
                 weight[i * width + j]);
            weightSum += weight[i * width + j];

            y = (rgbData[i * width + j].r * rY
                 + rgbData[i * width + j].g * gY
                 + rgbData[i * width + j].b * bY + 128) >> 8;
            if (max_y < y)
                max_y = y;
            if (min_y > y)
                min_y = y;
            if (y < SAT_Y * numPixels)
            {
                redSum_unsat +=
                    ((unsigned int) rgbData[i * width + j].r *
                     weight[i * width + j]) >> 8;
                greenSum_unsat +=
                    ((unsigned int) rgbData[i * width + j].g *
                     weight[i * width + j]) >> 8;
                blueSum_unsat +=
                    ((unsigned int) rgbData[i * width + j].b *
                     weight[i * width + j]) >> 8;
                weightSum_unsat += weight[i * width + j];
            }
        }
    }
    weightSum_unsat *= numPixels;
    totalY_unsat =
        redSum_unsat * rY + greenSum_unsat * gY + blueSum_unsat * bY;
    curY_unsat = (totalY_unsat + (weightSum_unsat >> 1)) / weightSum_unsat;

    weightSum *= numPixels;

#ifdef AE_DEBUG_PRINTS
    dprintf("min_y = %d\n", min_y);
    dprintf("R = %d, G = %d, B = %d\n", redSum / weightSum,
            greenSum / weightSum, blueSum / weightSum);
#endif

#if 1
    redSum >>= 8;
    greenSum >>= 8;
    blueSum >>= 8;
    totalY = redSum * rY + greenSum * gY + blueSum * bY;
#else
    totalY = redSum;
    if (totalY < greenSum)
    {
        totalY = greenSum;
    }
    if (totalY < blueSum)
    {
        totalY = blueSum;
    }
#endif
    curY_sat = (totalY + (weightSum >> 1)) / weightSum;

    if (0) //Gang:use 1 for  CPSE demo from Tuff
    {
        //(weightSum - weightSum_unsat)* 10  < weightSum){
        curY = curY_unsat;
    }
    else
    {
        curY = curY_sat;
    }
    if (curY == 0)
    {
        curY = 1;
    }

    /* now update the history brightnesss data */
    /* check if current brightness is within range of the history average */
    if (reset)
    {
        h->avgY = curY * h->numHistory;
        for (i = 0; i < h->numHistory; i++)
        {
            h->historyBrightness[i] = curY;
        }
        h->locked = FALSE;
        h->lockCnt = 0;
    }
    else
    {
        /* update avgY */
        h->avgY = h->avgY + curY - h->historyBrightness[h->numHistory - 1];

        /* update history */
        for (i = h->numHistory - 1; i > 0; i--)
        {
            h->historyBrightness[i] = h->historyBrightness[i - 1];
        }
        h->historyBrightness[0] = curY;
    }

    avgY = h->avgY;
    if (curY == 0)
    {
        curY = 1;
    }
    if (avgY == 0)
    {
        avgY = 1;
    }

#ifdef AE_DEBUG
    printf("AE debug: curY=%d, avgY=%d, locked=%d, lockcnt=%d\n", curY, avgY / h->numHistory, h->locked, h->lockCnt);
#endif
    if (!reset)
    {
        if (abs((int) avgY - (int) h->targetBrightness * h->numHistory) < h->thrld * h->numHistory && h->locked)
        {
            outArgs->nextAe = inArgs->curAe;
            return (IAE_EOK);
        }
        if ((curY > h->targetBrightnessRange.min && curY < h->targetBrightnessRange.max) ||
                (avgY > h->targetBrightnessRange.min * h->numHistory && avgY < h->targetBrightnessRange.max * h->numHistory))
        {
            h->lockCnt++;
        }
        else if (h->lockCnt > 0)
        {
            h->lockCnt--;
        }

        if (h->lockCnt >= 3)
        {
            h->lockCnt = 3;
            h->locked = TRUE;
            outArgs->nextAe = inArgs->curAe;
            return (IAE_EOK);
        }
        else if (h->lockCnt > 0 && h->locked)
        {
            outArgs->nextAe = inArgs->curAe;
            return (IAE_EOK);
        }
    }

    h->locked = FALSE;
    /* At this point, the average Y value and target Y value are
     * used to calculate the adjustment ratio to cur AE settings
     * Q10 format is used here to allow enouth accuracy
     */
    adjRatio = h->targetBrightness * 1024 / curY;
    //adjRatio = (((adjRatio - 1024)*7)>>3) + 1024;
#ifdef AE_DEBUG
    printf("\nAE debug: target=%d, curY=%d, adjRatio=%d", h->targetBrightness, curY, adjRatio);
#endif

    // adjust the ratio
    int delta = 100 * (11 - h->numSmoothSteps) / 10;
    if (adjRatio > 4096)
    {
        adjRatio = 4096;
    }
    else if (adjRatio < 256)
    {
        adjRatio = 256;
    }
    else if (adjRatio > 2048)
    {
        adjRatio = 2048;
    }
    else if (adjRatio < 512)
    {
        adjRatio = 512;
    }
    else if (adjRatio > 1536)
    {
        adjRatio = 1178;
    }
    else if (adjRatio < 682)
    {
        adjRatio = 890;
    }
    else if (adjRatio > 1024 + delta)
    {
        adjRatio = 1024 + delta;
    }
    else if (adjRatio < 1024 - delta)
    {
        adjRatio = 1024 - delta;
    }

#ifdef AE_DEBUG
    printf("\nAE debug: numH=%d, numS=%d, adjRatio=%d", h->numHistory, h->numSmoothSteps, adjRatio);
#endif
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /* Use the range values to calculate the actual adjustment needed */

    nA = cA;
    nT = cT;
    nAG = cAG;
    nDG = cDG;
    int r = 0;


    if (adjRatio >= 1024)  //increase gain
    {
        for (i = 0; i < h->numRanges; i++)
        {
            r = search_range_ATG_inc_gain(adjRatio,
                                          cA, cT, cAG, cDG,
                                          h->apertureLevelRange[i],
                                          h->exposureTimeRange[i],
                                          h->sensorGainRange[i],
                                          h->ipipeGainRange[i],
                                          h->exposureTimeStepSize,
                                          &nA, &nT, &nAG, &nDG);
#ifdef AE_DEBUG
            printf("\nAE debug: i=%d, r=%d, num=%d\n", i, r, h->numRanges);
#endif
            if (r == 1)
            {
                break;
            }
        }
    }
    else if (adjRatio < 1024) //decrease gain
    {
        for (i = h->numRanges - 1; i >= 0; i--)
        {
            r = search_range_ATG_dec_gain(adjRatio,
                                          cA, cT, cAG, cDG,
                                          h->apertureLevelRange[i],
                                          h->exposureTimeRange[i],
                                          h->sensorGainRange[i],
                                          h->ipipeGainRange[i],
                                          h->exposureTimeStepSize,
                                          &nA, &nT, &nAG, &nDG);
#ifdef AE_DEBUG
            printf("\nAE debug: i=%d, r=%d, num=%d\n", i, r, h->numRanges);
#endif
            if (r == 1)
            {
                break;
            }
        }
    }

#ifdef AE_DEBUG
    printf("\nAE debug: cA, cT, cAG, cDG = %d, %d, %d, %d", cA, cT, cAG, cDG);
    printf("\nAE debug: nA, nT, nAG, nDG = %d, %d, %d, %d, r=%d\n", nA, nT, nAG, nDG, r);
#endif
    /* now output the AE settings for next frame(s) */
    outArgs->nextAe.exposureTime = nT;
    outArgs->nextAe.apertureLevel = nA;
    outArgs->nextAe.sensorGain = nAG;
    outArgs->nextAe.ipipeGain = nDG;

    return (IAE_EOK);
}

/*
 *  ======== AE_TI_control ========
 */
XDAS_Int32 AE_TI_control(IAE_Handle handle, IAE_Cmd id,
                         IAE_DynamicParams *params, IAE_Status *status)
{
    XDAS_Int32 retVal;
    AE_TI_Obj *h = (AE_TI_Obj *) handle;
    int i;
    /* validate arguments - this codec only supports "base" xDM. */
    if (params->size != sizeof(*params))
    {

        return (IAE_EUNSUPPORTED);
    }

    switch (id)
    {
    case IAE_CMD_SET_CONFIG:
        if (params->numRanges > IAE_MAX_RANGES)
        {
            retVal = IAE_EFAIL;
        }
        else
        {
            h->numRanges = params->numRanges;
            for (i = 0; i < h->numRanges; i++)
            {
                h->exposureTimeRange[i] = params->exposureTimeRange[i];
                h->apertureLevelRange[i] = params->apertureLevelRange[i];
                h->sensorGainRange[i] = params->sensorGainRange[i];
                h->ipipeGainRange[i] = params->ipipeGainRange[i];
            }
            h->targetBrightnessRange = params->targetBrightnessRange;
            h->thrld = params->thrld;
            h->targetBrightness = params->targetBrightness;
            h->exposureTimeStepSize = params->exposureTimeStepSize;
            h->locked = FALSE;
            h->lockCnt = 0;
            for (i = 0; i < h->numHistory; i++)
            {
                h->historyBrightness[i] = -1;
            }
            h->avgY = -1;
#ifdef AE_DEBUG_PRINTS
            dprintf("exposureTime Range = %d, %d, %d\n",
                    h->exposureTimeRange[0].min, h->exposureTimeRange[0].max,
                    h->exposureTimeStepSize);
            dprintf("setup dynamic parameters \n");
#endif
            retVal = IAE_EOK;
        }
        break;
    case IAE_CMD_GET_CONFIG:
        params->numRanges = h->numRanges;
        for (i = 0; i < h->numRanges; i++)
        {
            params->exposureTimeRange[i] = h->exposureTimeRange[i];
            params->apertureLevelRange[i] = h->apertureLevelRange[i];
            params->sensorGainRange[i] = h->sensorGainRange[i];
            params->ipipeGainRange[i] = h->ipipeGainRange[i];
        }
        params->targetBrightnessRange = h->targetBrightnessRange;
        params->thrld = h->thrld;
        params->targetBrightness = h->targetBrightness;
        params->exposureTimeStepSize = h->exposureTimeStepSize;
        retVal = IAE_EOK;
        break;

    default:
        /* unsupported cmd */
        retVal = IAE_EUNSUPPORTED;
        break;
    }
    return (retVal);
}
