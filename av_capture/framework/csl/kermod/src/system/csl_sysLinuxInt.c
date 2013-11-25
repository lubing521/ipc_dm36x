/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_sys.h>
#include <csl_sysDrv.h>
#include <linux/interrupt.h>

typedef irqreturn_t(*CSL_SysIntLinuxIsr) (int irq, void *dev_id, struct pt_regs * regs);

CSL_Status CSL_sysIntClear(Uint32 intId)
{
  if(intId>=64)
    return CSL_EFAIL;
    
  if(intId < 32)
    gCSL_drvIntcRegs->IRQ0 = (1 << intId);
  else  
    gCSL_drvIntcRegs->IRQ1 = (1 << (intId-32));
    
  return CSL_SOK;    
}


CSL_Status CSL_sysIntEnable(Uint32 intId, Bool32 enable)
{
  unsigned long intState;
  
  if(intId>=64)
    return CSL_EFAIL;

  CSL_intGlobalDisable(intState);

  if (enable) {

    if(intId < 32)
      gCSL_drvIntcRegs->EINT0 |= (1 << intId);
    else  
      gCSL_drvIntcRegs->EINT1 |= (1 << (intId-32));

  } else {

    if(intId < 32)
      gCSL_drvIntcRegs->EINT0 &= ~(1 << intId);
    else  
      gCSL_drvIntcRegs->EINT1 &= ~(1 << (intId-32));
  }

  CSL_intGlobalRestore(intState);

  return CSL_SOK;
}

CSL_Status CSL_sysIntAttachIsr(Uint32 intId, CSL_SysIntIsr isr, void *prm)
{
  int     status = 0, intType;

  if (isr == NULL) {
    free_irq(intId, prm);
  } else {
  
    intType = SA_INTERRUPT;
      
    status = request_irq(intId, (CSL_SysIntLinuxIsr) isr, intType, CSL_DRV_NAME, prm);
  }

  if (status<0) {
    printk( KERN_ERR "request_irq(%d)\n", intId);
    return CSL_EFAIL;
  }

  return CSL_SOK;
}
