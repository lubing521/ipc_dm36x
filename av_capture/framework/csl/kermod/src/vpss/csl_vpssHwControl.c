#include <csl_vpssIoctl.h>

CSL_Status CSL_vpssHwControl(CSL_VpssHandle hndl, Uint32 cmd, void *prm)
{
  CSL_Status status = CSL_SOK;
  
  static CSL_VpssClkConfig clkConfig;
  static CSL_VpssIntEvtConfig intEvtConfig;
  static CSL_VpssPriConfig priConfig;
  static CSL_VpssWblSelConfig wblSelConfig;
  static CSL_VpssHwSetup data;
  static CSL_VpssClkDiv clkDiv;

  switch (cmd) {

  case CSL_VPSS_CMD_HW_SETUP:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&data, prm, sizeof(data));

    if (status == CSL_SOK) {
      if(data.clkConfig) {
        status = CSL_copyFromUser(&clkConfig, data.clkConfig, sizeof(clkConfig));
        
        data.clkConfig = &clkConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.intEvtConfig) {
        status = CSL_copyFromUser(&intEvtConfig, data.intEvtConfig, sizeof(intEvtConfig));
        
        data.intEvtConfig = &intEvtConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.priConfig) {
        status = CSL_copyFromUser(&priConfig, data.priConfig, sizeof(priConfig));
        
        data.priConfig = &priConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.wblSelConfig) {
        status = CSL_copyFromUser(&wblSelConfig, data.wblSelConfig, sizeof(wblSelConfig));
        
        data.wblSelConfig = &wblSelConfig;
      }
    }

    if (status == CSL_SOK)
      status = CSL_vpssHwSetup(hndl, &data);

    break;

  case CSL_VPSS_CMD_HW_RESET:

    status = CSL_vpssHwReset(hndl);

    break;

  case CSL_VPSS_CMD_SET_CLK_CONFIG:

    status = CSL_copyFromUser(&clkConfig, prm, sizeof(clkConfig));

    if (status == CSL_SOK)
      status = CSL_vpssSetClkConfig(hndl, &clkConfig);

    break;

  case CSL_VPSS_CMD_SET_INT_EVT_CONFIG:

    status = CSL_copyFromUser(&intEvtConfig, prm, sizeof(intEvtConfig));

    if (status == CSL_SOK)
      status = CSL_vpssSetIntEvtConfig(hndl, &intEvtConfig);

    break;

  case CSL_VPSS_CMD_SET_PRI_CONFIG:

    status = CSL_copyFromUser(&priConfig, prm, sizeof(priConfig));

    if (status == CSL_SOK)
      status = CSL_vpssSetPriConfig(hndl, &priConfig);

    break;

  case CSL_VPSS_CMD_SET_WBL_SEL_CONFIG:

    status = CSL_copyFromUser(&wblSelConfig, prm, sizeof(wblSelConfig));

    if (status == CSL_SOK)
      status = CSL_vpssSetWblSelConfig(hndl, &wblSelConfig);

    break;
    
  case CSL_VPSS_CMD_SET_CLKDIV:
    status = CSL_copyFromUser(&clkDiv, prm, sizeof(clkDiv));
    
    if(status==CSL_SOK) 
      status = CSL_vpssSetClkDiv(hndl, &clkDiv);
      
    break;    

  case CSL_VPSS_CMD_GET_CLKDIV:
   
    status = CSL_vpssGetClkDiv(hndl, &clkDiv);

    if(status==CSL_SOK) 
      status = CSL_copyToUser(prm, &clkDiv, sizeof(clkDiv));
      
    break;    
    
  default:
    status = CSL_EFAIL;
    break;
  }

  return status;
}
