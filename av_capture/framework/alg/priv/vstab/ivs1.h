/*
 *  Copyright 2008
 *  Texas Instruments Incorporated
 *
 *  All rights reserved.  Property of Texas Instruments Incorporated
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *
 */

/**
 *  @file       ti/xdais/dm/ivs1.h
 *
 *  @brief      This header defines all types, constants, and functions
 *              shared by all implementations of the video stabilization
 *              algorithms.
 */
/**
 *  @defgroup   ti_xdais_dm_IVS1   xDM 1.00 Video Stabilization Interface
 *
 *  This is the xDM 1.00 Video Stabilization Interface.
 */

#ifndef ti_xdais_dm_IVS1_
#define ti_xdais_dm_IVS1_

#include <xdc/std.h>
#include <ti/xdais/ialg.h>
#include <ti/xdais/xdas.h>
#include <ti/xdais/dm/xdm.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup    ti_xdais_dm_IVS1 */
/*@{*/

#define IVS1_EOK            XDM_EOK             /**< @copydoc XDM_EOK */
#define IVS1_EFAIL          XDM_EFAIL           /**< @copydoc XDM_EFAIL */
#define IVS1_EUNSUPPORTED   XDM_EUNSUPPORTED    /**< @copydoc XDM_EUNSUPPORTED */

/**
 *  @brief      This must be the first field of all IVS1
 *              instance objects.
 */
typedef struct IVS1_Obj {
    struct IVS1_Fxns *fxns;
} IVS1_Obj;


/**
 *  @brief      Opaque handle to an IVS1 objects.
 */
typedef struct IVS1_Obj  *IVS1_Handle;


/**
 *  @brief      Defines the creation time parameters for
 *              all IVS1 instance objects.
 *
 *  @extensibleStruct
 */
typedef struct IVS1_Params {
    XDAS_Int32  size;             /**< @sizeField */

    XDAS_UInt16 vertImgSizeBeforeBSC;       /**< Vertical size of frame that is input to BSC  */
    XDAS_UInt16 horzImgSizeBeforeBSC;       /**< Horizontal size of frame that is input to BSC */

    XDAS_UInt16 vertDownsampleRatioInImgSensor;      /**< Vertical downsampling ratio in image sensor in video mode */
    XDAS_UInt16 horzDownSampleRatioInImgSensor;      /**< Horizontal downsampling ratio in image sensor in video mode */

    XDAS_UInt16 minPanCoeffHorz;            /**< Minimum panning coefficient in horizontal dirn must be between 0 - 1 in Q15 */
                                            /**< Typically choose a value between 0.8 - 0.9 */
    XDAS_UInt16 maxPanCoeffHorz;            /**< Maximum panning coefficient in horizontal direction must be between 0 -1 in Q15 */
                                            /**< Typically choose a value between 0.9 - 1.0 */
    XDAS_UInt16 minPanCoeffVert;            /**< Minimum panning coefficient in vertical dirn must be between 0 - 1 in Q15 */
                                            /**< Typically choose a value between 0.8 - 0.9 */
    XDAS_UInt16 maxPanCoeffVert;            /**< Maximum panning coefficient in vertical direction must be between 0 -1 in Q15 */
                                            /**< Typically choose a value between 0.9 - 1.0 */

    /* BSC parameters */
    XDAS_UInt32 bsc_row_vct;
    XDAS_UInt32 bsc_row_vpos;
    XDAS_UInt32 bsc_row_vnum;
    XDAS_UInt32 bsc_row_vskip;
    XDAS_UInt32 bsc_row_hpos;
    XDAS_UInt32 bsc_row_hnum;
    XDAS_UInt32 bsc_row_hskip;
    XDAS_UInt32 bsc_row_shf;
    XDAS_UInt32 bsc_col_vct;
    XDAS_UInt32 bsc_col_vpos;
    XDAS_UInt32 bsc_col_vnum;
    XDAS_UInt32 bsc_col_vskip;
    XDAS_UInt32 bsc_col_hpos;
    XDAS_UInt32 bsc_col_hnum;
    XDAS_UInt32 bsc_col_hskip;
    XDAS_UInt32 bsc_col_shf;

} IVS1_Params;


/**
 *  @brief      This structure defines the algorithm parameters that can be
 *              modified after creation via control() calls.
 *
 *  @remarks    It is not necessary that a given implementation support all
 *              dynamic parameters to be configurable at run time.  If a
 *              particular algorithm does not support run-time updates to
 *              a parameter that the application is attempting to change
 *              at runtime, it may indicate this as an error.
 *
 *  @extensibleStruct
 *
 *  @sa         IVS1_Fxns::control()
 */
typedef struct IVS1_DynamicParams {
    XDAS_Int32 size;            /**< @sizeField */
} IVS1_DynamicParams;


/**
 *  @brief      Defines the input arguments for all IVS1 instance
 *              process function.
 *
 *  @extensibleStruct
 *
 *  @sa         IVS1_Fxns::process()
 */
typedef struct IVS1_InArgs {
    XDAS_Int32  size;             /**< @sizeField */

    XDAS_UInt32 bscBufferCurrV;    /**< Memory Address where the vertical BSC data for the current frame is present */
    XDAS_UInt32 bscBufferCurrH;    /**< Memory Address where the horizontal BSC data for the current frame is present */
    XDAS_UInt32 bscBufferPrevV;    /**< Memory Address where the vertical BSC data for the previous frame is present */
    XDAS_UInt32 bscBufferPrevH;    /**< Memory Address where the horizontal BSC data for the previous frame is present */

} IVS1_InArgs;


/**
 *  @brief      Defines instance status parameters.
 *
 *  @extensibleStruct
 *
 *  @sa         IVS1_Fxns::control()
 */
typedef struct IVS1_Status {
    XDAS_Int32 size;            /**< @sizeField */
    XDAS_Int32 extendedError;   /**< @extendedErrorField */
} IVS1_Status;


/**
 *  @brief      Defines the run time output arguments for all IVS1
 *              instance objects.
 *
 *  @extensibleStruct
 *
 *  @sa         IVS1_Fxns::process()
 */
typedef struct IVS1_OutArgs {
    XDAS_Int32  size;             /**< @sizeField */
    XDAS_Int32  extendedError;    /**< @extendedErrorField */

    XDAS_UInt32 startX;           /**< horizontal offset required to stabilize the video frame */
    XDAS_UInt32 startY;           /**< vertcal offset required to stabilize the video frame */

    XDAS_UInt32 winWidth;         /**< width of the stabilized video frame */
    XDAS_UInt32 winHeight;        /**< Height of the stabilized video frame */

} IVS1_OutArgs;


/**
 *  @brief      Defines the control commands for the IVS1 module.
 *
 *  @remarks    This ID can be extended in IMOD interface for
 *              additional controls.
 *
 *  @sa         XDM_CmdId
 *
 *  @sa         IVS1_Fxns::control()
 */
typedef  IALG_Cmd IVS1_Cmd;


/**
 *  @brief      Defines all of the operations on IVS1 objects.
 */
typedef struct IVS1_Fxns {
    IALG_Fxns   ialg;             /**< xDAIS algorithm interface.
                                   *
                                   *   @sa      IALG_Fxns
                                   */

    XDAS_Int32 (*process)(IVS1_Handle handle,IVS1_InArgs *inArgs,IVS1_OutArgs *outArgs);


    XDAS_Int32 (*control)(IVS1_Handle handle, IVS1_Cmd id,
                          IVS1_DynamicParams *params, IVS1_Status *status);

} IVS1_Fxns;

extern IVS1_Fxns VS_TI_IVS;

/*@}*/

#ifdef __cplusplus
}
#endif

#endif  /* ti_xdais_dm_IVS1_ */
/*
 *  @(#) ti.xdais.dm; 1, 0, 4,150; 11-25-2007 20:44:08; /db/wtree/library/trees/dais-i23x/src/
 */

