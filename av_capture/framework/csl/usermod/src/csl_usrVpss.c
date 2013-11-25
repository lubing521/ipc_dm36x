#include <csl_usrDrv.h>
#include <csl_vpssIoctl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ANR- IT */
Uint32* csl_h3aRegs;
CSL_Status CSL_vpssOpen(CSL_VpssHandle hndl)
{
  Uint32 ispOffset;

  #if 0
  ispOffset = CSL_ISP5_BASE_PHYS_ADDR - CSL_SYS_BASE_PHYS_ADDR;  
  hndl->regBasePhysAddr = CSL_SYS_BASE_PHYS_ADDR;
  hndl->regBaseLen      = CSL_SYS_BASE_MAX_OFFSET;
  #else
  ispOffset = 0;  
  hndl->regBasePhysAddr = CSL_ISP5_BASE_PHYS_ADDR;
  hndl->regBaseLen      = CSL_ISP5_BASE_MAX_OFFSET;
  #endif

  
  hndl->regBaseVirtAddr = NULL;

  hndl->ispRegs         = NULL;  
  hndl->vpssRegs        = NULL;
  hndl->rszRegs         = NULL;            
  hndl->ipipeRegs       = NULL;          
  hndl->isifRegs        = NULL;  
  hndl->ipipeifRegs     = NULL;        
  hndl->h3aRegs         = NULL;    
  hndl->ldcRegs         = NULL;      
  hndl->faceDetectRegs  = NULL;            

  hndl->fd = CSL_usrDrvOpen(CSL_MODULE_VPSS);

  if (hndl->fd < 0)
    return CSL_EFAIL;
  
  hndl->regBaseVirtAddr = CSL_usrDrvMmap(hndl->fd, hndl->regBasePhysAddr, hndl->regBaseLen);    
  if(hndl->regBaseVirtAddr==NULL)
    return CSL_EFAIL;
    


  hndl->ispRegs         = ((CSL_Isp5RegsOvly)       ((Uint32)hndl->regBaseVirtAddr + ispOffset + CSL_ISP5_0_OFFSET       )); 
  hndl->vpssRegs        = ((CSL_VpssRegsOvly)       ((Uint32)hndl->regBaseVirtAddr + ispOffset + CSL_VPSS_0_OFFSET       ));
  hndl->rszRegs         = ((CSL_RszRegsOvly)        ((Uint32)hndl->regBaseVirtAddr + ispOffset + CSL_RSZ_0_OFFSET        ));            
  hndl->ipipeRegs       = ((CSL_IpipeRegsOvly)      ((Uint32)hndl->regBaseVirtAddr + ispOffset + CSL_IPIPE_0_OFFSET      ));            
  hndl->isifRegs        = ((CSL_IsifRegsOvly)       ((Uint32)hndl->regBaseVirtAddr + ispOffset + CSL_ISIF_0_OFFSET       ));  
  hndl->ipipeifRegs     = ((CSL_IpipeifRegsOvly)    ((Uint32)hndl->regBaseVirtAddr + ispOffset + CSL_IPIPEIF_0_OFFSET    ));        
  hndl->h3aRegs         = ((CSL_H3aRegsOvly)        ((Uint32)hndl->regBaseVirtAddr + ispOffset + CSL_H3A_0_OFFSET        ));    
  hndl->ldcRegs         = ((CSL_LdcRegsOvly)        ((Uint32)hndl->regBaseVirtAddr + ispOffset + CSL_LDC_0_OFFSET        ));      
  hndl->faceDetectRegs  = ((CSL_FaceDetectRegsOvly) ((Uint32)hndl->regBaseVirtAddr + ispOffset + CSL_FACE_DETECT_0_OFFSET));            

  /* ANR - IT */
  csl_h3aRegs = (Uint32*)((Uint32)hndl->regBaseVirtAddr + ispOffset + CSL_H3A_0_OFFSET);
  return CSL_SOK;
}

CSL_Status CSL_vpssClose(CSL_VpssHandle hndl)
{
  CSL_usrDrvUnmap(hndl->fd, hndl->regBaseVirtAddr, hndl->regBaseLen);

  return CSL_usrDrvClose(hndl->fd);
}

CSL_Status CSL_vpssHwSetup(CSL_VpssHandle hndl, CSL_VpssHwSetup * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_VPSS_CMD_HW_SETUP, data);
}

CSL_Status CSL_vpssHwReset(CSL_VpssHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_VPSS_CMD_HW_RESET, NULL);
}

CSL_Status CSL_vpssSetClkConfig(CSL_VpssHandle hndl, CSL_VpssClkConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_VPSS_CMD_SET_CLK_CONFIG, data);
}

CSL_Status CSL_vpssSetIntEvtConfig(CSL_VpssHandle hndl, CSL_VpssIntEvtConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_VPSS_CMD_SET_INT_EVT_CONFIG, data);
}

CSL_Status CSL_vpssSetPriConfig(CSL_VpssHandle hndl, CSL_VpssPriConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_VPSS_CMD_SET_PRI_CONFIG, data);
}

CSL_Status CSL_vpssSetWblSelConfig(CSL_VpssHandle hndl, CSL_VpssWblSelConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_VPSS_CMD_SET_WBL_SEL_CONFIG, data);
}

CSL_Status CSL_vpssLock(CSL_VpssHandle hndl, Uint32 timeout)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_LOCK, (void *) timeout);
}

CSL_Status CSL_vpssUnlock(CSL_VpssHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_UNLOCK, NULL);
}

CSL_Status CSL_vpssSetClkDiv(CSL_VpssHandle hndl, CSL_VpssClkDiv *prm)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_VPSS_CMD_SET_CLKDIV, prm);  
}

CSL_Status CSL_vpssGetClkDiv(CSL_VpssHandle hndl, CSL_VpssClkDiv *prm)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_VPSS_CMD_GET_CLKDIV, prm);  
}


