/*
 *  Copyright 2007 by Texas Instruments Incorporated.
 *
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *
 */

/*
 *  ======== awb_ti.h ========
 */
#ifndef AWB_TI_
#define AWB_TI_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== AWB_TI_VIDENCCOPY ========
 *  Our implementation of the IAWB interface
 */
extern IAWB_Fxns AWB_TI_AWB;

#define TIAWB_CMD_CALIBRATION     0x3

#ifdef __cplusplus
}
#endif

#endif

