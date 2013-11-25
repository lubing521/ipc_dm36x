/*
 *  Copyright 2007 by Texas Instruments Incorporated.
 *
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *
 */

/*
 *  ======== ae_tiar0331.h ========
 */
#ifndef AE_TI_
#define AE_TI_

#ifdef __cplusplus
extern "C" {
#endif
/*
 *  Our implementation of the IAE interface
 */
extern IAE_Fxns AE_TIAR0331_AE;

typedef struct AE_TIAR0331_DynamicParams {
    XDAS_Int32 	size;                 /**< @sizeField */
    XDAS_Int32 	numRanges;	     /**< number of control ranges */
    IAE_Range 	exposureTimeRange[IAE_MAX_RANGES];     /**< range of exposure time, 
                                     *   unit is same with exposure time
				     */
    XDAS_UInt32 exposureTimeStepSize;/**< step size of exposure time 
                                      *   adjustmnent */
    IAE_Range 	apertureLevelRange[IAE_MAX_RANGES];    /**< range of aperture level, 
                                      *   unit is same with aperture level
				      */
    IAE_Range 	sensorGainRange[IAE_MAX_RANGES];       /**< range of sensor gain, 
                                      *   unit is same with sensor gain 
				      */
    IAE_Range 	ipipeGainRange[IAE_MAX_RANGES];        /**< range of IPIPE gain, 
                                      *   unit is same with IPIPE gain
			              */
    IAE_Range 	targetBrightnessRange; /**< range of target brightnes */
    XDAS_UInt32 targetBrightness;   /**< target brightness value */
    XDAS_UInt32 targetBrightnessHigh;  /**< target brightness value */
    XDAS_UInt32 thrld;                 /**< threshold for not using history brightness information */

} AE_TIAR0331_DynamicParams;


typedef struct AE_TIAR0331_InArgs {
    XDAS_Int32  size;		/**< @sizeField */
   IAEWB_StatMat       statMat;        /**< Statistics Matrix */

    XDAS_Int32  curYLow;        /* mean brightness */
    XDAS_Int32  curYHigh;        /* low mean brightness */
    IAEWB_Ae    curAe;		/**< Auto Exposure setting for the current frame */
    IAEWB_Wb    curWb;		/**< White Blance gain setting for the current frame */
    XDAS_Bool   curWdr;
    XDAS_Int32  curRatioT1T2;
} AE_TIAR0331_InArgs;


typedef struct AE_TIAR0331_OutArgs {
    XDAS_Int32 	size;		/**< @sizeField */
    IAEWB_Ae 	nextAe;
    XDAS_Bool   nextWdr;
    XDAS_Int32  nextRatioT1T2;
} AE_TIAR0331_OutArgs;
#ifdef __cplusplus
}
#endif

#endif

