

#include <csl_gpioIoctl.h>

CSL_Status CSL_gpioHwControl(CSL_GpioHandle hndl, Uint32 cmd, void *prm)
{
  CSL_Status status = CSL_SOK;
  CSL_GpioPrm gpioPrm;
  Uint16 intStatus;

  switch (cmd) {

  case CSL_GPIO_CMD_SET_MODE:
  
    status = CSL_copyFromUser(&gpioPrm, prm, sizeof(gpioPrm));
    if(status==CSL_SOK) {
      status = CSL_gpioSetMode(hndl, gpioPrm.num, gpioPrm.value);
    }
    break;
  case CSL_GPIO_CMD_SET:
  
    status = CSL_gpioSet(hndl, (Uint32)prm );
    break;
  case CSL_GPIO_CMD_CLR:
  
    status = CSL_gpioClr(hndl, (Uint32)prm );
    break;
  case CSL_GPIO_CMD_GET:
  
    status = CSL_gpioGet(hndl, (Uint32)prm );
    break;
    
  case CSL_GPIO_CMD_BANK_INT_ENABLE:

    status = CSL_copyFromUser(&gpioPrm, prm, sizeof(gpioPrm));
    if(status==CSL_SOK) {
      status = CSL_gpioBankIntEnable(hndl, gpioPrm.num, gpioPrm.value);
    }
    break;
 
  case CSL_GPIO_CMD_BANK_INT_GET_STATUS:

    status = CSL_copyFromUser(&gpioPrm, prm, sizeof(gpioPrm));
    if(status==CSL_SOK) {
      status = CSL_gpioBankIntGetStatus(hndl, gpioPrm.num, &intStatus);
      
      if(status==CSL_SOK) {
        gpioPrm.value = intStatus;
        status = CSL_copyToUser(prm, &gpioPrm, sizeof(gpioPrm));
      }
    }
    break;
    
  case CSL_GPIO_CMD_BANK_INT_CLR_STATUS:
  
    status = CSL_copyFromUser(&gpioPrm, prm, sizeof(gpioPrm));
    if(status==CSL_SOK) {
      status = CSL_gpioBankIntClrStatus(hndl, gpioPrm.num, gpioPrm.value);
    }  
    break;
    
  case CSL_GPIO_CMD_PINMUX_SET:

    status = CSL_copyFromUser(&gpioPrm, prm, sizeof(gpioPrm));
    if(status==CSL_SOK) {
      status = CSL_gpioSetPinmux(hndl, gpioPrm.num, gpioPrm.value);
    }
    break;

  case CSL_GPIO_CMD_PINMUX_GET:

    status = CSL_copyFromUser(&gpioPrm, prm, sizeof(gpioPrm));
    if(status==CSL_SOK) {
      status = CSL_gpioGetPinmux(hndl, gpioPrm.num, &gpioPrm.value);
      
      if(status==CSL_SOK) {
        status = CSL_copyToUser(prm, &gpioPrm, sizeof(gpioPrm));
      }
    }
    break;
    
  case CSL_GPIO_CMD_PSC_SET:    

    status = CSL_copyFromUser(&gpioPrm, prm, sizeof(gpioPrm));
    if(status==CSL_SOK) {
      status = CSL_gpioSetPsc(hndl, gpioPrm.num, gpioPrm.value);
    }
    break;


  default:
    status = CSL_EFAIL;
    break;
  }

  return status;
}
