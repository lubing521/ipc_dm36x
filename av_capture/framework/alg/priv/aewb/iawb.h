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
 *  @file       ti/xdais/dm/iawb.h
 *
 *  @brief      This header defines all types, constants, and functions
 *              shared by all implementations of the auto white balance 
 *              algorithms.
 */
/**
 *  @defgroup   ti_xdais_dm_IAWB   xDM 1.0 extention Auto White Balance 
 *                                 Interface
 *
 *  This is the xDM 1.0 extention Auto White Balance Interface.
 */

#ifndef ti_xdais_dm_IAWB_
#define ti_xdais_dm_IAWB_

#include <xdc/std.h>
#include <ti/xdais/ialg.h>
#include <ti/xdais/xdas.h>
#include <ti/xdais/dm/xdm.h>
#include <iaewb.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup    ti_xdais_dm_IAWB */
/*@{*/

#define IAWB_ERUNTIME  XDM_ERUNTIME        /**< @copydoc XDM_ERUNTIME */
#define IAWB_EOK       XDM_EOK             /**< @copydoc XDM_EOK */
#define IAWB_EFAIL     XDM_EFAIL           /**< @copydoc XDM_EFAIL */
#define IAWB_EUNSUPPORTED XDM_EUNSUPPORTED /**< @copydoc XDM_EUNSUPPORTED */

typedef enum IAWB_Mode{
  IAWB_MODE_AUTO,   /**< Automatic mode */
  IAWB_MODE_MANUAL, /**< Manual Mode, Algorithm try to find the nearest values
                     *   with given values in this mode 
		     */
  IAWB_MODE_NUM     /**< number of mode of Auto White Balance Algorithm 
                     *   Runing Mechanism
		     */
} IAWB_Mode;

/**
 *  @brief      This must be the first field of all IAWB
 *              instance objects.
 */
typedef struct IAWB_Obj {
    struct IAWB_Fxns *fxns;
} IAWB_Obj;

/**
 *  @brief      Opaque handle to an IAWB objects.
 */
typedef struct IAWB_Obj  *IAWB_Handle;

/**
 *  @brief      Defines the creation time parameters for
 *              all IAWB instance objects.
 *
 *  @extensibleStruct
 */

typedef struct IAWB_Params{
    XDAS_Int32 size;            /**< @sizeField */

    XDAS_UInt32 numHistory;     /**< number of history results used
                                 *   in calculating the current result 
                                 */
    XDAS_UInt32 numSmoothSteps; /**< number of steps taken to make the
                                 *   transition smooth
                                 */

} IAWB_Params;

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
 *  @sa         IAWB_Fxns::control()
 */
typedef struct IAWB_DynamicParams {
    XDAS_Int32 	size;                 /**< @sizeField */
    XDAS_Bool 	strobeOn;         /**< strobe is on/off,Boolean 0: off, 1: on */
    XDAS_Bool 	antiFlickerOn;    /**< Anti Flickering mechanism on/off, 
                                 *   Boolean 0: off, 1: on
				 */
    IAWB_Mode 	mode;             /**< Auto White Balance Algorithm Running Mode
                                 *
				 */
    IAEWB_Wb  	target;           /**< Target value. It has meaning only
                                 *   in Manual mode, can be NULL in auto mode 
				 *   but cannot be NULL in manual mode
				 */
}IAWB_DynamicParams;


/**
 *  @brief      Defines the input arguments for all IAWB instance
 *              process function.
 *
 *  @extensibleStruct
 *
 *  @sa         IAWB_Fxns::process()
 */
typedef struct IAWB_InArgs {
    XDAS_Int32 		size;       /**< @sizeField */
    IAEWB_StatMat 	statMat;   	/**< Statistics Matrix */
    IAEWB_Ae 		curAe;      /**< Auto Exposure setting for the current frame */
    IAEWB_Wb 		curWb;     /**< AWB setting for the current frame */
} IAWB_InArgs;

/**
 *  @brief      Defines instance status parameters.
 *
 *  @extensibleStruct
 *
 *  @sa         IAWB_Fxns::control()
 */
typedef struct IAWB_Status {
    XDAS_Int32 size;          /**< @sizeField */
} IAWB_Status;


/**
 *  @brief      Defines the run time output arguments for all IAWB
 *              instance objects.
 *
 *  @extensibleStruct
 *
 *  @sa         IAWB_Fxns::process()
 */
typedef struct IAWB_OutArgs {
    XDAS_Int32 size;            /**< @sizeField */
    IAEWB_Wb nextWb;        /**< White Blance gain setting for the current frame */
} IAWB_OutArgs;


/**
 *  @brief      Defines the control commands for the IAWB module.
 *
 *  @remarks    This ID can be extended in IMOD interface for
 *              additional controls.
 *
 *  @sa         XDM_CmdId
 *
 *  @sa         IAWB_Fxns::control()
 */
typedef  IALG_Cmd IAWB_Cmd;

#define IAWB_CMD_SET_CONFIG		0x1
#define IAWB_CMD_GET_CONFIG		0x2


/**
 *  @brief      Defines all of the operations on IAWB objects.
 */
typedef struct IAWB_Fxns {
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
 *              IAWB_InArgs structure.
 *
 *  @pre        @c outArgs must not be NULL, and must point to a valid
 *              IAWB_OutArgs structure.
 *
 *  @post       The algorithm <b>must not</b> modify the contents of
 *              @c inArgs.
 *              That is, the data and buffers pointed to by this parameter
 *              must be treated as read-only.
 *
 *
 *  @retval     IALG_EOK        Success.
 *  @retval     IALG_EFAIL      Failure.  See IAWB_OutArgs#extendedError
 *                              for more detailed further error conditions.
 */
    XDAS_Int32 (*process)(IAWB_Handle handle, IAWB_InArgs *inArgs,
        IAWB_OutArgs *outArgs, IAEWB_Rgb *rgbData, 
		void *customData);

/**
 *  @brief      Control behaviour of an algorithm.
 *
 *  @param[in]  handle          Handle to an algorithm instance.
 *  @param[in]  id              Command id.  See #XDM_CmdId.
 *  @param[in]  params          Dynamic parameters.  This is a required
 *                              parameter.
 *  @param[out] status          Output results.  This is a required parameter.
 *
 *  @pre        @c params must not be NULL, and must point to a valid
 *              IAWB_DynamicParams structure.
 *
 *  @pre        @c status must not be NULL, and must point to a valid
 *              IAWB_Status structure.
 *
 *  @post       The algorithm <b>must not</b> modify the contents of @c params.
 *              That is, the data pointed to by this parameter must be
 *              treated as read-only.
 *
 *  @retval     IALG_EOK        Success.
 *  @retval     IALG_EFAIL      Failure.  See IAWB_Status#extendedError
 *                              for more detailed further error conditions.
 */
    XDAS_Int32 (*control)(IAWB_Handle handle, IAWB_Cmd id,
        IAWB_DynamicParams *params, IAWB_Status *status);

} IAWB_Fxns;


/*@}*/

#ifdef __cplusplus
}
#endif

#endif  /* ti_xdais_dm_IAWB_ */

