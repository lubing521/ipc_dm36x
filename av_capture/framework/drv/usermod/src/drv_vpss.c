
#include <drv_csl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <sys/ioctl.h>

//#define DRV_VPSS_DEBUG

void DRV_SyncRst(Uint8 module, Uint8 domain, Uint8 state)
{
	void *pMem_map;
	int dev_fp;
	unsigned long phyAddr= 0x01C41000;
	unsigned long length = 0xAE0;
	unsigned int *pMDCTL = NULL;
	unsigned int *pPTCMD = NULL;
	unsigned int *pPTSTAT = NULL;
	unsigned int *pMDSTAT = NULL;

#ifdef DRV_VPSS_DEBUG
	OSA_printf("DRV_SyncRst: module = %d, domain = %d, state = %d\n", module, domain,state);
#endif

	if ((dev_fp=open("/dev/mem",O_RDWR|O_SYNC))==-1)
	{
		OSA_ERROR("dev_fp Fail!! \n");
		return ;
	}
	pMem_map=mmap(	(void	*)phyAddr,length,
					PROT_READ|PROT_WRITE|PROT_EXEC,MAP_SHARED,
					dev_fp,phyAddr);

	if (pMem_map!=(void *)phyAddr)
	{
		OSA_ERROR("pMem_map Fail!! \n");
		return ;
	}


	pPTCMD 		= (unsigned int *)( (char *)pMem_map+0x120);
	pPTSTAT		= (unsigned int *)( (char *)pMem_map+0x128);
	pMDSTAT		= (unsigned int *)( (char *)pMem_map+0x800);
	pMDCTL	 	= (unsigned int *)( (char *)pMem_map+0xA00);

	// Wait for any outstanding transition to complete
  while ( (*pPTSTAT) & (0x00000001 << domain) );

  // If we are already in that state, just return
  if (((pMDSTAT[module]) & 0x1F) == state) return;

  // Perform transition
  pMDCTL[module] = ((pMDCTL[module]) & (0xFFFFFFE0)) | (state);
  *pPTCMD |= (0x00000001 << domain);

  // Wait for transition to complete
  while ( (*pPTSTAT) & (0x00000001 << domain) );

  // Wait and verify the state
  while (((pMDSTAT[module]) & 0x1F) != state);


	if( pMem_map )
		munmap(pMem_map, length);

	if( dev_fp >= 0)
		close(dev_fp);

	return;
}

int DRV_vpssEnableBoxcar(Bool enable)
{
  int status;

  CSL_VpssWblSelConfig wblSelConfig;

  wblSelConfig.wblSelIsifIpipe = CSL_VPSS_BL_WBL_ISIF_OR_IPIPE_SEL_ISIF;

  if(enable)
    wblSelConfig.wblSelIpipeLdc  = CSL_VPSS_BL_WBL_LDC_OR_IPIPE_SEL_IPIPE;
  else
    wblSelConfig.wblSelIpipeLdc  = CSL_VPSS_BL_WBL_LDC_OR_IPIPE_SEL_LDC;

  status = CSL_vpssSetWblSelConfig(&gCSL_vpssHndl, &wblSelConfig);

  return status;
}


int DRV_vpssInit()
{
  CSL_VpssClkConfig    	clkConfig;
  CSL_VpssIntEvtConfig 	intEvtConfig;
  CSL_VpssPriConfig    	priConfig;
  CSL_VpssWblSelConfig 	wblSelConfig;
  CSL_VpssHwSetup 		vpssSetup;

  Uint32 value32;
  int status;

  DRV_SyncRst(47, 0, 0);
  OSA_waitMsecs(10);
  DRV_SyncRst(47, 0, 3);
  OSA_waitMsecs(10);

  vpssSetup.clkConfig     = &clkConfig;
  vpssSetup.intEvtConfig  = &intEvtConfig;
  vpssSetup.priConfig     = &priConfig;
  vpssSetup.wblSelConfig  = NULL;

  clkConfig.pclkSelect          = CSL_VPSS_PCLK_SEL_VPSS;
  clkConfig.pclkSyncClkEnable   = TRUE;
  clkConfig.blClkEnable         = TRUE;
  clkConfig.isifClkEnable       = TRUE;
  clkConfig.h3aClkEnable        = TRUE;
  clkConfig.ipipeifClkEnable    = TRUE;
  clkConfig.ipipeClkEnable      = TRUE;
  clkConfig.rszClkEnable        = TRUE;
  clkConfig.ldcClkEnable        = TRUE;
  clkConfig.fdifClkEnable       = TRUE;
  clkConfig.ldcClkSelect        = CSL_VPSS_LDC_CLK_MODULE;

  intEvtConfig.intSel[0] = CSL_VPSS_INT_EVT_SEL_ISIF_INT0;
  intEvtConfig.intSel[1] = CSL_VPSS_INT_EVT_SEL_ISIF_INT1;
  intEvtConfig.intSel[2] = CSL_VPSS_INT_EVT_SEL_RSZ_INT_DMA;
  intEvtConfig.intSel[3] = CSL_VPSS_INT_EVT_SEL_LDC_INT;
  intEvtConfig.intSel[4] = CSL_VPSS_INT_EVT_SEL_FDIF_INT;
  intEvtConfig.intSel[5] = CSL_VPSS_INT_EVT_SEL_IPIPE_INT_BSC;
  intEvtConfig.intSel[6] = CSL_VPSS_INT_EVT_SEL_IPIPE_INT_DMA; // boxcar complete interrupt
  intEvtConfig.intSel[7] = CSL_VPSS_INT_EVT_SEL_OSD_INT;
  intEvtConfig.intSel[8] = CSL_VPSS_INT_EVT_SEL_VENC_INT;

  intEvtConfig.evtSel[0] = CSL_VPSS_INT_EVT_SEL_IPIPE_INT_BSC;
  intEvtConfig.evtSel[1] = CSL_VPSS_INT_EVT_SEL_IPIPE_INT_HST;
  intEvtConfig.evtSel[2] = CSL_VPSS_INT_EVT_SEL_IPIPE_INT_DPC_RNEW0;
  intEvtConfig.evtSel[3] = CSL_VPSS_INT_EVT_SEL_IPIPE_INT_DPC_RNEW1;

  priConfig.priVpssWr = 0;
  priConfig.priVpssRd = 0;

  status = CSL_vpssHwSetup(&gCSL_vpssHndl, &vpssSetup);
  if(status!=OSA_SOK) {
    OSA_ERROR("CSL_vpssHwSetup()\n");
    return status;
  }

  status = DRV_vpssEnableBoxcar(FALSE);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_vpssEnableBoxcar()\n");
    return status;
  }

  // enable other ISP related clocks
  gCSL_vpssHndl.ipipeRegs->GCK_MMR = 0x1;
  gCSL_vpssHndl.ipipeRegs->GCK_PIX = 0xF;
  gCSL_vpssHndl.rszRegs->GCK_MMR   = 0x1;
  gCSL_vpssHndl.rszRegs->GCK_SDR   = 0x1;

  gCSL_vpssHndl.ispRegs->INTSTAT = 0xFFFFFFFF; // clear interrupt status

  // init PINMUX0
  status = CSL_gpioGetPinmux(&gCSL_gpioHndl, 0, &value32);
  if(status==OSA_SOK) {
    value32 &= 0xFFFF0000; // enable Y0-Y7, HD, VD, FID

    status = CSL_gpioSetPinmux(&gCSL_gpioHndl, 0, value32);
  }

  // init PINMUX1
  status = CSL_gpioGetPinmux(&gCSL_gpioHndl, 1, &value32);
  if(status==OSA_SOK) {
    value32 &= 0xFFFFFFF0; // enable gio91 gio92
 
    status = CSL_gpioSetPinmux(&gCSL_gpioHndl, 1, value32);
  }
 
  // init PINMUX4
  status = CSL_gpioGetPinmux(&gCSL_gpioHndl, 4, &value32);
  if(status==OSA_SOK) {
    value32 &= 0xFFF3FC3F; // enable gio30 gio31 gio36
 
    status = CSL_gpioSetPinmux(&gCSL_gpioHndl, 4, value32);
  }

  if(status!=OSA_SOK) {
    OSA_ERROR("CSL_gpioGetPimux()\n");
    return status;
  }

  OSA_waitMsecs(10);

  DRV_clkGetClkHz(NULL, TRUE);

  return status;
}

int DRV_vpssExit()
{
  CSL_VpssClkConfig    	clkConfig;
  CSL_VpssIntEvtConfig 	intEvtConfig;
  CSL_VpssPriConfig    	priConfig;
  CSL_VpssWblSelConfig 	wblSelConfig;
  CSL_VpssHwSetup 		vpssSetup;

  Uint32 value32;
  int status;

  vpssSetup.clkConfig     = &clkConfig;
  vpssSetup.intEvtConfig  = &intEvtConfig;
  vpssSetup.priConfig     = &priConfig;
  vpssSetup.wblSelConfig  = &wblSelConfig;

  clkConfig.pclkSelect          = CSL_VPSS_PCLK_SEL_VPSS;
  clkConfig.pclkSyncClkEnable   = FALSE;
  clkConfig.blClkEnable         = FALSE;
  clkConfig.isifClkEnable       = FALSE;
  clkConfig.h3aClkEnable        = FALSE;
  clkConfig.ipipeifClkEnable    = FALSE;
  clkConfig.ipipeClkEnable      = FALSE;
  clkConfig.rszClkEnable        = FALSE;
  clkConfig.ldcClkEnable        = FALSE;
  clkConfig.fdifClkEnable       = FALSE;
  clkConfig.ldcClkSelect        = CSL_VPSS_LDC_CLK_MODULE;

  intEvtConfig.intSel[0] = CSL_VPSS_INT_EVT_SEL_ISIF_INT0;
  intEvtConfig.intSel[1] = CSL_VPSS_INT_EVT_SEL_ISIF_INT1;
  intEvtConfig.intSel[2] = CSL_VPSS_INT_EVT_SEL_RSZ_INT_DMA;
  intEvtConfig.intSel[3] = CSL_VPSS_INT_EVT_SEL_LDC_INT;
  intEvtConfig.intSel[4] = CSL_VPSS_INT_EVT_SEL_FDIF_INT;
  intEvtConfig.intSel[5] = CSL_VPSS_INT_EVT_SEL_IPIPE_INT_BSC;
  intEvtConfig.intSel[6] = CSL_VPSS_INT_EVT_SEL_IPIPE_INT_HST;
  intEvtConfig.intSel[7] = CSL_VPSS_INT_EVT_SEL_OSD_INT;
  intEvtConfig.intSel[8] = CSL_VPSS_INT_EVT_SEL_VENC_INT;

  intEvtConfig.evtSel[0] = CSL_VPSS_INT_EVT_SEL_IPIPE_INT_BSC;
  intEvtConfig.evtSel[1] = CSL_VPSS_INT_EVT_SEL_IPIPE_INT_HST;
  intEvtConfig.evtSel[2] = CSL_VPSS_INT_EVT_SEL_IPIPE_INT_DPC_RNEW0;
  intEvtConfig.evtSel[3] = CSL_VPSS_INT_EVT_SEL_IPIPE_INT_DPC_RNEW1;

  priConfig.priVpssWr = 0;
  priConfig.priVpssRd = 0;

  wblSelConfig.wblSelIsifIpipe = CSL_VPSS_BL_WBL_ISIF_OR_IPIPE_SEL_ISIF;
  wblSelConfig.wblSelIpipeLdc  = CSL_VPSS_BL_WBL_LDC_OR_IPIPE_SEL_LDC;

  status = CSL_vpssHwSetup(&gCSL_vpssHndl, &vpssSetup);
  if(status!=OSA_SOK) {
    OSA_ERROR("CSL_vpssHwSetup()\n");
    return status;
  }

  // enable other ISP related clocks
  gCSL_vpssHndl.ipipeRegs->GCK_MMR = 0x0;
  gCSL_vpssHndl.ipipeRegs->GCK_PIX = 0x0;
  gCSL_vpssHndl.rszRegs->GCK_MMR   = 0x0;
  gCSL_vpssHndl.rszRegs->GCK_SDR   = 0x0;

  gCSL_vpssHndl.ispRegs->INTSTAT = 0xFFFFFFFF; // clear interrupt status

  DRV_SyncRst(47, 0, 2);

  OSA_waitMsecs(10);

  return status;
}


