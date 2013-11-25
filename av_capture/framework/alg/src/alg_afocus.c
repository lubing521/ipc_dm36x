#include <alg_priv.h>
#include <csl_ipipe.h>
#include <drv_csl.h>
#include <osa_cmem.h>
#include <osa_file.h>

#include "alg_aewb_priv.h"

int ALG_afRun(ALG_AfRunPrm * prm, int *focus_value)
{
	CSL_H3aAfOutVfDisableOverlay *pAfPaxData;
	long int *phyCurAddr;
	unsigned short i,j;
  	Uint8 *curAfAddr;
	Uint32 accAfVal[9] = {0,0,0,0,0,0,0,0,0};
	float avgAfVal[9];
	int weighti, weightj, fweight;
	int af_pax_vt_cnt, af_pax_hz_cnt;

	af_pax_vt_cnt = prm->pH3aInfo->afNumWinV;
	af_pax_hz_cnt = prm->pH3aInfo->afNumWinH;

    curAfAddr = (Uint8* ) prm->h3aDataVirtAddr;

    phyCurAddr = OSA_cmemGetPhysAddr(curAfAddr);
    if(phyCurAddr==NULL) {
      OSA_ERROR("OSA_cmemGetPhysAddr() failed\n");
    }

   for(i=1;i<af_pax_vt_cnt-1; i++) {
    for(j=1;j<af_pax_hz_cnt-1; j++) {

      pAfPaxData = (CSL_H3aAfOutVfDisableOverlay *)curAfAddr;

	  weighti = (i > (af_pax_vt_cnt-1 - i))?(af_pax_vt_cnt-1 - i):i;
	  weightj = (j > (af_pax_hz_cnt-1 - j))?(af_pax_hz_cnt-1 - j):j;
	  fweight = weighti*weightj;

	  accAfVal[0] += fweight*pAfPaxData->hfvSum_0;
 	  accAfVal[1] += fweight*pAfPaxData->hfv1_0;
	  accAfVal[2] += fweight*pAfPaxData->hfv2_0;
	  accAfVal[3] += fweight*pAfPaxData->hfvSum_1;
 	  accAfVal[4] += fweight*pAfPaxData->hfv1_1;
	  accAfVal[5] += fweight*pAfPaxData->hfv2_1;
	  accAfVal[6] += fweight*pAfPaxData->hfvSum_2;
 	  accAfVal[7] += fweight*pAfPaxData->hfv1_2;
	  accAfVal[8] += fweight*pAfPaxData->hfv2_2;

	  curAfAddr += sizeof(CSL_H3aAfOutVfDisableOverlay);

    }

	curAfAddr = (Uint8*)OSA_align( (Uint32)curAfAddr, 32);
   }

   for(i=0;i<9;i++){
   	  avgAfVal[i] = (float)accAfVal[i]/(af_pax_vt_cnt*af_pax_hz_cnt);
      //if((i==4)||(i==5)) // 4 & 5 indices for green value in RGB space
      //	OSA_printf("accAfVal[%d]: %d \t avgAfVal[%d]: %f \n", i, accAfVal[i], i, avgAfVal[i]);
   }

   *focus_value = accAfVal[4];

   return 0;
}
