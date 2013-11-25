
#include <csl_usrDrv.h>
#include <csl_gpioIoctl.h>

CSL_Status CSL_gpioOpen(CSL_GpioHandle hndl)
{
  hndl->fd = CSL_usrDrvOpen(CSL_MODULE_GPIO);

  if (hndl->fd < 0)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_gpioClose(CSL_GpioHandle hndl)
{
  return CSL_usrDrvClose(hndl->fd);
}

CSL_Status CSL_gpioLock(CSL_GpioHandle hndl, Uint32 timeout)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_LOCK, (void *) timeout);
}

CSL_Status CSL_gpioUnlock(CSL_GpioHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_UNLOCK, NULL);
}


CSL_Status CSL_gpioSetMode(CSL_GpioHandle hndl, Uint32 gpioNum, Uint32 mode )
{
  CSL_GpioPrm prm;
  
  prm.num = gpioNum;
  prm.value = mode;
  
  return CSL_usrDrvIoctl(hndl->fd, CSL_GPIO_CMD_SET_MODE, &prm);
}

CSL_Status CSL_gpioSet(CSL_GpioHandle hndl, Uint32 gpioNum )
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_GPIO_CMD_SET, (void*)gpioNum);
}

CSL_Status CSL_gpioClr(CSL_GpioHandle hndl, Uint32 gpioNum )
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_GPIO_CMD_CLR, (void*)gpioNum);
}


Uint32     CSL_gpioGet(CSL_GpioHandle hndl, Uint32 gpioNum )
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_GPIO_CMD_GET, (void*)gpioNum);
}

CSL_Status CSL_gpioBankIntEnable(CSL_GpioHandle hndl, Uint32 bankNum, Bool32 enable )
{
  CSL_GpioPrm prm;
  
  prm.num = bankNum;
  prm.value = enable;
  
  return CSL_usrDrvIoctl(hndl->fd, CSL_GPIO_CMD_BANK_INT_ENABLE, &prm);
}

CSL_Status CSL_gpioBankIntGetStatus(CSL_GpioHandle hndl, Uint32 bankNum, Uint16 *intStatus )
{
  CSL_GpioPrm prm;
  CSL_Status status;
  
  *intStatus = 0;
  
  prm.num = bankNum;
  prm.value = 0;
  
  status = CSL_usrDrvIoctl(hndl->fd, CSL_GPIO_CMD_BANK_INT_GET_STATUS , &prm);
  
  if(status==CSL_SOK) {
    *intStatus = prm.value;
  }
  
  return status;
}

CSL_Status CSL_gpioBankIntClrStatus(CSL_GpioHandle hndl, Uint32 bankNum, Uint16 intStatus )
{
  CSL_GpioPrm prm;
  
  prm.num = bankNum;
  prm.value = intStatus;
  
  return CSL_usrDrvIoctl(hndl->fd, CSL_GPIO_CMD_BANK_INT_CLR_STATUS , &prm);
}

CSL_Status CSL_gpioSetPinmux(CSL_GpioHandle hndl, Uint32 pinMuxNum, Uint32 pinMuxValue)
{
  CSL_GpioPrm prm;
  
  prm.num = pinMuxNum;
  prm.value = pinMuxValue;
  
  return CSL_usrDrvIoctl(hndl->fd, CSL_GPIO_CMD_PINMUX_SET, &prm);
}

CSL_Status CSL_gpioGetPinmux(CSL_GpioHandle hndl, Uint32 pinMuxNum, Uint32 *pinMuxValue)
{
  CSL_GpioPrm prm;
  CSL_Status status;

  *pinMuxValue=0;
  
  prm.num = pinMuxNum;
  prm.value = 0;
  
  status = CSL_usrDrvIoctl(hndl->fd, CSL_GPIO_CMD_PINMUX_GET, &prm);
  
  if(status==CSL_SOK) {
    *pinMuxValue = prm.value;
  }
  
  return status;
}

CSL_Status CSL_gpioSetPsc(CSL_GpioHandle hndl, Uint32 pscModNum, Uint32 pscModState)
{
  CSL_GpioPrm prm;
  
  prm.num = pscModNum;
  prm.value = pscModState;
  
  return CSL_usrDrvIoctl(hndl->fd, CSL_GPIO_CMD_PSC_SET, &prm);
}


