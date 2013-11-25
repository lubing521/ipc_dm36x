/* 
 *  Copyright 2007
 *  Texas Instruments Incorporated 
 *
 *  All rights reserved.  Property of Texas Instruments Incorporated
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 * 
 */

/**
 *  @file       ti/xdais/dm/iae.h
 *
 *  @brief      This header defines all types, constants, and functions
 *              shared by all implementations of the auto exposure algorithms.
 */
/**
 *  @defgroup   ti_xdais_dm_IAE   xDM 1.0 extention Auto Exposure Interface
 *
 *  This is the xDM 1.0 extention Auto Exposure Interface.
 */

#ifndef ti_xdais_dm_IAE_
#define ti_xdais_dm_IAE_

#include <xdc/std.h>
#include <ti/xdais/ialg.h>
#include <ti/xdais/xdas.h>
#include <ti/xdais/dm/xdm.h>
#include <iaewb.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup    ti_xdais_dm_IAE */
/*@{*/

#define IAE_ERUNTIME  XDM_ERUNTIME        /**< @copydoc XDM_ERUNTIME */
#define IAE_EOK       XDM_EOK             /**< @copydoc XDM_EOK */
#define IAE_EFAIL     XDM_EFAIL           /**< @copydoc XDM_EFAIL */
#define IAE_EUNSUPPORTED XDM_EUNSUPPORTED /**< @copydoc XDM_EUNSUPPORTED */

#define IAE_MAX_RANGES 125
/**
 *  @brief      This must be the first field of all IAE
 *              instance objects.
 */
typedef struct IAE_Obj {
    struct IAE_Fxns *fxns;
} IAE_Obj;


/**
 *  @brief      Opaque handle to an IAE objects.
 */
typedef struct IAE_Obj  *IAE_Handle;

typedef struct IAE_Range{
    XDAS_Int32 min;
    XDAS_Int32 max;
}IAE_Range;

typedef enum IAE_Pri {
    IAE_PRI_0 = 0,    /** highest priority */
    IAE_PRI_1 = 1,
    IAE_PRI_2 = 2,
    IAE_PRI_3 = 3,
    IAE_PRI_4 = 4
} IAE_Pri;

/**
 *  @brief      Defines the creation time parameters for
 *              all IAE instance objects.
 *
 *  @extensibleStruct
 */

typedef struct IAE_Params{
    XDAS_Int32 size;            /**< @sizeField */
    XDAS_UInt32 numHistory;     /**< number of history results used
                                 *   in calculating the current result 
                                 */
    XDAS_UInt32 numSmoothSteps; /**< number of steps taken to make the
                                 *   transition smooth
                                 */

} IAE_Params;


/**
 *  @brief      This structure defines the algorithm parameters that can be
 *              modified after creation via control() calls.
 *
 *  @remarks    It is not necessary that given implementation support all
 *              dynamic parameters to be configurable at run time.  If a
 *              particular algorithm does not support run-time updates to
 *              a parameter that the application is attempting to change
 *              at runtime, it may indicate this as an error.
 *
 *  @extensibleStruct
 *
 *  @sa         IAE_Fxns::control()
 */

typedef struct IAE_DynamicParams {
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
    XDAS_UInt32 targetBrightness;    /**< target brightness value */
    XDAS_UInt32 thrld;               /**< threshold for not using history brightness information */

} IAE_DynamicParams;


/**
 *  @brief      Defines the input arguments for all IAE instance
 *              process function.
 *
 *  @extensibleStruct
 *
 *  @sa         IAE_Fxns::process()
 */

typedef struct IAE_InArgs {
    XDAS_Int32 		size;		/**< @sizeField */
    IAEWB_StatMat 	statMat;	/**< Statistics Matrix */
    IAEWB_Ae 		curAe;		/**< Auto Exposure setting for the current frame */
    IAEWB_Wb 		curWb;		/**< White Blance gain setting for the current frame */
} IAE_InArgs;

/**
 *  @brief      Defines instance status parameters.
 *
 *  @extensibleStruct
 *
 *  @sa         IAE_Fxns::control()
 */
typedef struct IAE_Status {
    XDAS_Int32 size;          /**< @sizeField */
} IAE_Status;


/**
 *  @brief      Defines the run time output arguments for all IAE
 *              instance objects.
 *
 *  @extensibleStruct
 *
 *  @sa         IAE_Fxns::process()
 */
typedef struct IAE_OutArgs {
    XDAS_Int32 	size;		/**< @sizeField */
    IAEWB_Ae 	nextAe;
} IAE_OutArgs;


/**
 *  @brief      Defines the control commands for the IAE module.
 *
 *  @remarks    This ID can be extended in IMOD interface for
 *              additional controls.
 *
 *  @sa         XDM_CmdId
 *
 *  @sa         IAE_Fxns::control()
 */
typedef  IALG_Cmd IAE_Cmd;

#define IAE_CMD_SET_CONFIG		0x1
#define IAE_CMD_GET_CONFIG		0x2

/**
 *  @brief      Defines all of the operations on IAE objects.
 */
typedef struct IAE_Fxns {
    IALG_Fxns   ialg;             /**< Traditional xDAIS algorithm interface. */

/**
 *  @brief      Basic auto exposure run call.
 *
 *  @param[in]  handle          Handle to an algorithm instance.
 *  @param[in]  inArgs          Input arguments.  This is a required
 *                              parameter.
 *  @param[out] outArgs         Ouput results.  This is a required parameter.
 *
 *  @pre        @c inArgs must not be NULL, and must point to a valid
 *              IAE_InArgs structure.
 *
 *  @pre        @c outArgs must not be NULL, and must point to a valid
 *              IAE_OutArgs structure.
 *
 *  @post       The algorithm <b>must not</b> modify the contents of
 *              @c inArgs.
 *              That is, the data and buffers pointed to by this parameter
 *              must be treated as read-only.
 *
 *
 *  @retval     IALG_EOK        Success.
 *  @retval     IALG_EFAIL      Failure.  See IAE_OutArgs#extendedError
 *                              for more detailed further error conditions.
 */
    XDAS_Int32 (*process)(IAE_Handle handle,IAE_InArgs *inArgs, 
        IAE_OutArgs *outArgs, IAEWB_Rgb *rgbData, XDAS_UInt8* weight, void *customData);

/**
 
 *
 *  @param[in]  handle          Handle to an algorithm instance.
 *  @param[in]  id              Command id.  See #XDM_CmdId.
 *  @param[in]  params          Dynamic parameters.  This is a required
 *                              parameter.
 *  @param[out] status          Output results.  This is a required parameter.
 *
 *  @pre        @c params must not be NULL, and must point to a valid
 *              IAE_DynamicParams structure.
 *
 *  @pre        @c status must not be NULL, and must point to a valid
 *              IAE_Status structure.
 *
 *  @post       The algorithm <b>must not</b> modify the contents of @c params.
 *              That is, the data pointed to by this parameter must be
 *              treated as read-only.
 *
 *  @retval     IALG_EOK        Success.
 *  @retval     IALG_EFAIL      Failure.  See IAE_Status#extendedError
 *                              for more detailed further error conditions.
 */
    XDAS_Int32 (*control)(IAE_Handle handle, IAE_Cmd id,IAE_DynamicParams *params, IAE_Status *status);

} IAE_Fxns;


/*@}*/

#ifdef __cplusplus
}
#endif

#endif  /* ti_xdais_dm_IAE_ */

