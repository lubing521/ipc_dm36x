/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_rsz.h>
#include <csl_sysDrv.h>


CSL_Status CSL_rszSetFlip(CSL_RszHandle hndl, Uint8 rszMod, Bool32 flipH, Bool32 flipV)
{
  CSL_Status status = CSL_EFAIL;

  if(rszMod >=CSL_RSZ_CH_MAX)
    return status;
    
  hndl->flipH[rszMod] = flipH;
  hndl->flipV[rszMod] = flipV;
  
  return status;
}

CSL_Status CSL_rszSetOutAddr(CSL_RszHandle hndl, Uint8 rszMod, Uint8 * addr, Uint8 *yuv420CoutAddr)
{
  Uint32  addr32, caddr32;

  if (hndl == NULL)
    return CSL_EFAIL;

  addr32 = (Uint32) addr;
  caddr32 = (Uint32) yuv420CoutAddr;

  if (rszMod == CSL_RSZ_A) {
  
    hndl->regs->RZA_SDR_Y_SAD_L = (Uint16) addr32;
    hndl->regs->RZA_SDR_Y_SAD_H = (Uint16) (addr32 >> 16);
    hndl->regs->RZA_SDR_Y_BAD_L = (Uint16) addr32;
    hndl->regs->RZA_SDR_Y_BAD_H = (Uint16) (addr32 >> 16);

    hndl->regs->RZA_SDR_C_SAD_L = (Uint16) caddr32;
    hndl->regs->RZA_SDR_C_SAD_H = (Uint16) (caddr32 >> 16);
    hndl->regs->RZA_SDR_C_BAD_L = (Uint16) caddr32;
    hndl->regs->RZA_SDR_C_BAD_H = (Uint16) (caddr32 >> 16);
    
  } else if (rszMod == CSL_RSZ_B) {
  
    hndl->regs->RZB_SDR_Y_SAD_L = (Uint16) addr32;
    hndl->regs->RZB_SDR_Y_SAD_H = (Uint16) (addr32 >> 16);
    hndl->regs->RZB_SDR_Y_BAD_L = (Uint16) addr32;
    hndl->regs->RZB_SDR_Y_BAD_H = (Uint16) (addr32 >> 16);
    
    hndl->regs->RZB_SDR_C_SAD_L = (Uint16) caddr32;
    hndl->regs->RZB_SDR_C_SAD_H = (Uint16) (caddr32 >> 16);
    hndl->regs->RZB_SDR_C_BAD_L = (Uint16) caddr32;
    hndl->regs->RZB_SDR_C_BAD_H = (Uint16) (caddr32 >> 16);
  }

  return CSL_SOK;
}

/* this function will just writes the passed vlaues to global handle structure and Actual register
    write will done else where after ISR is happend */
CSL_Status CSL_rszSetRuntimeConfigInHndl(CSL_RszHandle hndl,Uint8 rszMod,CSL_RszRuntimeConfig *rtCfg)
{
    CSL_RszRuntimeConfig *rtCfgGblHdl;

#if 0
    printk(KERN_INFO "resizeMod = %x outH=%d outW=%d hrsz =%d vrsz=%d inStX=%d, inStY=%d\n", 
		   rszMod, rtCfg->outHeight, rtCfg->outWidth, rtCfg->hrsz, rtCfg->vrsz, rtCfg->inStartX, rtCfg->inStartY);
#endif
    if (hndl == NULL)
      return CSL_EFAIL;

    /* update parameters to global structure */
    rtCfgGblHdl = &hndl->rszRtCfg[rszMod];
    rtCfgGblHdl->outWidth = rtCfg->outWidth;
    rtCfgGblHdl->outHeight= rtCfg->outHeight;
    rtCfgGblHdl->inStartX = rtCfg->inStartX;
    rtCfgGblHdl->inStartY = rtCfg->inStartY;
    rtCfgGblHdl->hrsz     = rtCfg->hrsz;
    rtCfgGblHdl->vrsz     = rtCfg->vrsz;
    // Memory addresses and offsets update for SDRAM Y and CbCr output data??

    /* set flag so that these values are written to registers in Next ISR */
    hndl->rszRuntimeConfigFlag[rszMod] = TRUE;
	
	return CSL_SOK;
}

CSL_Status CSL_rszApplyRuntimeCfg(CSL_RszHandle hndl,Uint8 rszMod)
{
    CSL_RszRuntimeConfig *rtCfg;
	
	if (hndl == NULL)
	  return CSL_EFAIL;

	if (rszMod >= CSL_RSZ_CH_MAX)
	  return CSL_EFAIL;

	/* update resizer runtime parameters if required and reset the flag */
	if (TRUE == hndl->rszRuntimeConfigFlag[rszMod]) 
	{
		rtCfg = &hndl->rszRtCfg[rszMod];
		
		if (rszMod == CSL_RSZ_A) {
		  hndl->regs->RZA_O_HSZ = rtCfg->outWidth - 1;
		  hndl->regs->RZA_O_VSZ = rtCfg->outHeight - 1;
		  hndl->regs->RZA_I_HPS = rtCfg->inStartX;
		  hndl->regs->RZA_I_VPS = rtCfg->inStartY;
		  hndl->regs->RZA_H_DIF = rtCfg->hrsz;
		  hndl->regs->RZA_V_DIF = rtCfg->vrsz;
		} else if (rszMod == CSL_RSZ_B) {
		  hndl->regs->RZB_O_HSZ = rtCfg->outWidth - 1;
		  hndl->regs->RZB_O_VSZ = rtCfg->outHeight - 1;
		  hndl->regs->RZA_I_HPS = rtCfg->inStartX;
		  hndl->regs->RZA_I_VPS = rtCfg->inStartY;
		  hndl->regs->RZB_H_DIF = rtCfg->hrsz;
		  hndl->regs->RZB_V_DIF = rtCfg->vrsz;
		}

		hndl->rszRuntimeConfigFlag[rszMod] = FALSE;
	}

	return CSL_SOK;
}

