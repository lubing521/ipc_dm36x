/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_gpioIoctl.h>

CSL_Status CSL_gpioOpen(CSL_GpioHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  hndl->regs = CSL_sysGetBaseAddr(CSL_MODULE_GPIO);
  
  if (hndl->regs == NULL)
    return CSL_EFAIL;
    
  hndl->sysRegs = CSL_SYS_REGS;    

  if (hndl->sysRegs == NULL)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_gpioClose(CSL_GpioHandle hndl)
{
  return CSL_SOK;
}

CSL_Status CSL_gpioInit(CSL_GpioHandle hndl)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL)
    return CSL_EINVPARAMS;

  status |= CSL_gpioOpen(hndl);

  return status;
}

CSL_Status CSL_gpioExit(CSL_GpioHandle hndl)
{
  if (hndl == NULL)
    return CSL_EINVPARAMS;

  CSL_gpioClose(hndl);

  return CSL_SOK;
}
