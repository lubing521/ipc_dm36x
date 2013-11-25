/** ===========================================================================
* @file Appro_aewb.h
*
* @path $(IPNCPATH)\multimedia\encode_stream\aew_xdm
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */

#ifndef _AEW_CONTROL_H_
#define _AEW_CONTROL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iae.h>
#include <iawb.h>
#include <iaewb.h>
#include <ae_appro.h>
#include <awb_appro.h>

typedef	struct _DRVIER_DEF{
	unsigned short	EST_SENDING;
	unsigned short	EST_NOW;
	unsigned short	AGC_SENDING;
	unsigned short	AGC_NOW;
	unsigned short B_GAIN_SENDING;
	unsigned short B_GAIN_NOW;
	unsigned short R_GAIN_SENDING;
	unsigned short R_GAIN_NOW;
	unsigned short DGAIN;
	short   AE_COUNTER;
	short 	UPDATE_FLAG;
	short   VD_COUNT;
	short   PAUSE_AWWB;			/*default  = 0*/
	short   SKIP_BINNING_MODE; 	/*default  = 0    binning mode */
}DRVIER_DEF;

enum{
	DRV_CMD_FRAMECNT = 0,
	DRV_CMD_RBGAIN_DISP,
	DRV_CMD_RB_HIGH_LIMIT,
	DRV_CMD_RB_LOW_LIMIT,
	DRV_CMD_AE_GAIN_INFO,
	DRV_CMD_MAX
};

typedef struct _DRV_SETUP{
	IAE_Handle 	handleAe;
	IAWB_Handle handleAwb;
	int 		IsUpdate;
	int 		IsCustomRB_limit;
	unsigned short r_high_limit;
	unsigned short r_low_limit;
	unsigned short b_high_limit;
	unsigned short b_low_limit;
	int 		Cmd;
	void 		*pData;
}DRV_SETUP;

extern IAE_Range APPRO_dgain_List_480P[IAE_MAX_RANGES];
extern IAE_Range APPRO_agc_List_480P[IAE_MAX_RANGES];
extern IAE_Range APPRO_shutter_List_480P[IAE_MAX_RANGES];
extern IAE_Range APPRO_shutter_List_480P_Bin[IAE_MAX_RANGES];

extern IAE_Range APPRO_dgain_List_720P[IAE_MAX_RANGES];
extern IAE_Range APPRO_agc_List_720P[IAE_MAX_RANGES];
extern IAE_Range APPRO_shutter_List_720P[IAE_MAX_RANGES];

extern IAE_Range APPRO_dgain_List_NF[IAE_MAX_RANGES];
extern IAE_Range APPRO_agc_List_NF[IAE_MAX_RANGES];
extern IAE_Range APPRO_shutter_List_NF[IAE_MAX_RANGES];

extern unsigned char APPRO_WEIGHTING_SPOT[];
extern unsigned char APPRO_WEIGHTING_CENTER[];
extern unsigned char APPRO_WEIGHTING_MATRIX[];

void ApproSend3A(	IAE_Handle handleAe,IAWB_Handle handleAwb,
						IAE_OutArgs *IAE_OutArg, IAWB_OutArgs *IAWB_OutArg);

void CONTROL_DRIVER_initial(	IAE_Handle handleAe,IAWB_Handle handleAwb,

									IAE_OutArgs *IAE_OutArg, IAWB_OutArgs *IAWB_OutArg);

void CONTROL_DRIVER_process( IAE_Handle handleAe,IAWB_Handle handleAwb,
										IAE_OutArgs *IAE_OutArg,IAWB_OutArgs *IAWB_OutArg,CONTROL3AS *CONTROL3A);

void CONTROL_DRIVER_Setup( DRV_SETUP *pParm);


extern DRVIER_DEF DRIVER;
extern DRV_SETUP CTRL_DRV_SETUP;
#endif
