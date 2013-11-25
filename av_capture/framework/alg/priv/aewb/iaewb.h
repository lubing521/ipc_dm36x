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
 *  @file       ti/xdais/dm/iaewb.h
 *
 *  @brief      This header defines all types, constants, and functions
 *              shared by all implementations of the auto exposure and auto 
 *              white balance algorithms.
 */
/**
 *  @defgroup   ti_xdais_dm_IAEWB   xDM 1.0 extention Auto Exposure & 
 *                                  Auto White Balance Interface
 *
 *  This is the xDM 1.0 extention Auto Exposure & Auto White Balance Interface.
 */

#ifndef ti_xdais_dm_IAEWB_
#define ti_xdais_dm_IAEWB_

#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup    ti_xdais_dm_IAEWB */
/*@{*/

/**
 *  @brief typedef for RGB count: 16-bit for HW 3A, 32-bit otherwise 
 */

#ifdef H3A
typedef XDAS_UInt16 IAEWB_RGB_COUNT;  /**< H3A available: 16-bit unsigned */
#else
typedef XDAS_UInt32 IAEWB_RGB_COUNT; /**< SW Implementation: 32-bit unsigned */
#endif

/**
 *  @brief structure for non-saturated RGB counter values for each sub-window
 */
typedef struct IAEWB_Rgb{
    IAEWB_RGB_COUNT r; /**< Red value 16 bit */
    IAEWB_RGB_COUNT g; /**< Green value 16 bit */
    IAEWB_RGB_COUNT b; /**< Blue value 16 bit */
} IAEWB_Rgb;

/**
 *  @brief structure for statistics matrix
 */

typedef struct IAEWB_StatMat{
	    XDAS_Int32 winCtHorz; /**< number of windows in horizontal direction */
	    XDAS_Int32 winCtVert; /**< number of windows in vertical direction */
	    XDAS_Int32 pixCtWin; /**< number pixels per window */
	} IAEWB_StatMat;

/**
 *  @brief structure for Auto Exposure Setting
 */
typedef struct {
  XDAS_Int32 exposureTime;  /**< exposure Time  */
  XDAS_Int32 apertureLevel; /**< aperture Level */
  XDAS_Int32 sensorGain;    /**< sensor gain, Q10 */
  XDAS_Int32 ipipeGain;     /**< IPIPE gain, Q10  
                             *   e.g. 1.0 is 1024, 2.0 is 2048 and 0.5 is 512
			     */
} IAEWB_Ae;

/**
  \brief structure for Auto Whilte Balance Setting
*/
typedef struct IAEWB_Wb{
  XDAS_UInt32 rGain; /**< Red Gain, Q10, 
                      *   e.g. 1.0 is 1024, 2.0 is 2048 and 0.5 is 512
		      */
  XDAS_UInt32 gGain; /**< Green Gain, Q10, 
                      *   e.g. 1.0 is 1024, 2.0 is 2048 and 0.5 is 512
		      */
  XDAS_UInt32 bGain; /**< Blue Gain, Q10, 
                       *  e.g. 1.0 is 1024, 2.0 is 2048 and 0.5 is 512
		       */
  XDAS_UInt32 colorTemp; /**< color Temperature, 
                          *   unit is not yet decided (tentative)
			  */
} IAEWB_Wb;



/*@}*/  /* ingroup */

#ifdef __cplusplus
}
#endif


#endif  /* ti_xdais_dm_IAEWB_ */

