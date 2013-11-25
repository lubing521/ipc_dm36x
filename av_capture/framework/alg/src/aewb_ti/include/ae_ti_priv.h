/*
 *  Copyright 2007 by Texas Instruments Incorporated.
 *
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *
 */

/*
 *  ======== ae_ti_priv.h ========
 *  Internal vendor specific (TI) interface header for AE
 *  algorithm. Only the implementation source files include
 *  this header; this header is not shipped as part of the
 *  algorithm.
 *
 *  This header contains declarations that are specific to
 *  this implementation and which do not need to be exposed
 *  in order for an application to use the AE algorithm.
 */
#ifndef AE_TI_PRIV_
#define AE_TI_PRIV_

#define RY    0x4d
#define GY    0x96
#define BY    0x1d

typedef struct AE_TI_Obj {
    IALG_Obj   alg;            /* MUST be first field of all XDAS algs */
    XDAS_Int32 numRanges;
    IAE_Range exposureTimeRange[IAE_MAX_RANGES];
    XDAS_UInt32 exposureTimeStepSize;
    IAE_Range apertureLevelRange[IAE_MAX_RANGES];
    IAE_Range sensorGainRange[IAE_MAX_RANGES];
    IAE_Range ipipeGainRange[IAE_MAX_RANGES];
    IAE_Range targetBrightnessRange;
    XDAS_UInt32 targetBrightness;
    XDAS_UInt32 thrld;
    XDAS_UInt32 avgY; 
    XDAS_UInt32 numHistory;
    XDAS_UInt32 numSmoothSteps;
    XDAS_Int32 *historyBrightness;
    XDAS_UInt32 curBrightness;
    XDAS_Int32  curExposureTime;
    XDAS_Int32  curApertureLevel;
    XDAS_Int32  curSensorGain;
    XDAS_Int32  curIpipeGain;
    XDAS_Bool   locked;
    XDAS_Int32  lockCnt;
} AE_TI_Obj;

extern Int AE_TI_alloc(const IALG_Params *algParams, IALG_Fxns **pf,
    IALG_MemRec memTab[]);

extern Int AE_TI_free(IALG_Handle handle, IALG_MemRec memTab[]);

extern Int AE_TI_init(IALG_Handle handle,
    const IALG_MemRec memTab[], IALG_Handle parent,
    const IALG_Params *algParams);

extern XDAS_Int32 AE_TI_process(IAE_Handle h, IAE_InArgs *inArgs, IAE_OutArgs *outArgs,
    IAEWB_Rgb *rgbData, XDAS_UInt8 *weight, void *customData);

extern XDAS_Int32 AE_TI_control(IAE_Handle handle,
    IAE_Cmd id, IAE_DynamicParams *params, IAE_Status *status);

#endif

