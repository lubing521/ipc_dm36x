/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/


#include <csl_gpio.h>


CSL_Status CSL_gpioSetMode(CSL_GpioHandle hndl, Uint32 gpioNum, Uint32 mode )
{
  Uint32 bank;
  Uint32 mask;
  
  if(gpioNum > CSL_GPIO_NUM_MAX )
    return CSL_EFAIL;
    
  bank =  gpioNum/(CSL_GPIO_NUM_PER_BANK*2); 
  gpioNum = gpioNum%(CSL_GPIO_NUM_PER_BANK*2);
  
  mask = (1<<gpioNum);

  if(mode & CSL_GPIO_OUTPUT) {
    hndl->regs->BANK[bank].DIR &= ~mask;
  } else 
  if(mode & CSL_GPIO_INPUT) {
  
    if(mode & CSL_GPIO_INTR_TRIG_RISE) 
      hndl->regs->BANK[bank].SET_RIS_TRIG = mask;
    else 
      hndl->regs->BANK[bank].CLR_RIS_TRIG = mask;
    

    if(mode & CSL_GPIO_INTR_TRIG_FALL) 
      hndl->regs->BANK[bank].SET_FAL_TRIG = mask;
    else 
      hndl->regs->BANK[bank].CLR_FAL_TRIG = mask;

    hndl->regs->BANK[bank].DIR |= mask;
  }
  
  return CSL_SOK;
}

CSL_Status CSL_gpioSet(CSL_GpioHandle hndl, Uint32 gpioNum )
{
  Uint32 bank;
  
  if(gpioNum > CSL_GPIO_NUM_MAX )
    return CSL_EFAIL;
    
  bank =  gpioNum/(CSL_GPIO_NUM_PER_BANK*2); 
  gpioNum = gpioNum%(CSL_GPIO_NUM_PER_BANK*2);

  hndl->regs->BANK[bank].SET_DATA = (1<<gpioNum);
  
  return CSL_SOK;
}

CSL_Status CSL_gpioClr(CSL_GpioHandle hndl, Uint32 gpioNum )
{
  Uint32 bank;
  
  if(gpioNum > CSL_GPIO_NUM_MAX )
    return CSL_EFAIL;
    
  bank =  gpioNum/(CSL_GPIO_NUM_PER_BANK*2); 
  gpioNum = gpioNum%(CSL_GPIO_NUM_PER_BANK*2);

  hndl->regs->BANK[bank].CLR_DATA = (1<<gpioNum);
  
  return CSL_SOK;
}


Uint32     CSL_gpioGet(CSL_GpioHandle hndl, Uint32 gpioNum )
{
  Uint32 bank;
  
  if(gpioNum > CSL_GPIO_NUM_MAX )
    return 0;
    
  bank =  gpioNum/(CSL_GPIO_NUM_PER_BANK*2); 
  gpioNum = gpioNum%(CSL_GPIO_NUM_PER_BANK*2);

  if( ( hndl->regs->BANK[bank].IN_DATA ) & (1<<gpioNum) )
    return 1;
  
  return 0;
}

CSL_Status CSL_gpioBankIntEnable(CSL_GpioHandle hndl, Uint32 bankNum, Bool32 enable )
{
  if(bankNum > CSL_GPIO_BANK_MAX)
    return CSL_EFAIL;
    
  if(enable)  
    hndl->regs->BINTEN |= (1<<bankNum);
  else
    hndl->regs->BINTEN &= ~(1<<bankNum);
    
  return CSL_SOK;
}

CSL_Status CSL_gpioBankIntGetStatus(CSL_GpioHandle hndl, Uint32 bankNum, Uint16 *intStatus )
{
  Uint32 tmpValue;
  
  if(bankNum > CSL_GPIO_BANK_MAX)
    return CSL_EFAIL;

  tmpValue = hndl->regs->BANK[bankNum/2].INTSTAT;
  
  if(bankNum & 0x1) 
    *intStatus = (Uint16)(tmpValue >> 16);
  else
    *intStatus = (Uint16)(tmpValue >> 0);
  
  return CSL_SOK;
}

CSL_Status CSL_gpioBankIntClrStatus(CSL_GpioHandle hndl, Uint32 bankNum, Uint16 intStatus )
{
  Uint32 tmpValue;
  
  if(bankNum > CSL_GPIO_BANK_MAX)
    return CSL_EFAIL;

  if(bankNum & 0x1) 
    tmpValue = (Uint32)intStatus << 16;
  else
    tmpValue = (Uint32)intStatus << 0;
    
  hndl->regs->BANK[bankNum/2].INTSTAT = tmpValue;
  
  return CSL_SOK;
}

CSL_Status CSL_gpioSetPinmux(CSL_GpioHandle hndl, Uint32 pinMuxNum, Uint32 pinMuxValue)
{
  if(pinMuxNum > 4)
    return CSL_EFAIL;
    
  hndl->sysRegs->PINMUX[pinMuxNum] = pinMuxValue;
  
  return CSL_SOK;
}

CSL_Status CSL_gpioGetPinmux(CSL_GpioHandle hndl, Uint32 pinMuxNum, Uint32 *pinMuxValue)
{
  if(pinMuxNum > 4)
    return CSL_EFAIL;
    
  *pinMuxValue = hndl->sysRegs->PINMUX[pinMuxNum];
  
  return CSL_SOK;
}

#define CSL_PSC_REG_VIRT_ADDR     CSL_PHYS_TO_VIRT(0x01c41000)

#define CSL_PSC_REG_PTCMD         (CSL_PSC_REG_VIRT_ADDR + 120)
#define CSL_PSC_REG_PTSTAT        (CSL_PSC_REG_VIRT_ADDR + 128)  
#define CSL_PSC_REG_MDSTAT        (CSL_PSC_REG_VIRT_ADDR + 0x800)
#define CSL_PSC_REG_MDCTL         (CSL_PSC_REG_VIRT_ADDR + 0xa00)

CSL_Status CSL_gpioSetPsc(CSL_GpioHandle hndl, Uint32 pscModNum, Uint32 pscModState)
{
  volatile Uint32 *mdstat = ( volatile Uint32 * )( CSL_PSC_REG_MDSTAT + ( 4 * pscModNum ) );
  volatile Uint32 *mdctl  = ( volatile Uint32 * )( CSL_PSC_REG_MDCTL  + ( 4 * pscModNum ) );
  volatile Uint32 *ptstat = ( volatile Uint32 * )( CSL_PSC_REG_PTSTAT);
  volatile Uint32 *ptcmd  = ( volatile Uint32 * )( CSL_PSC_REG_PTCMD );  
  volatile int i;
  unsigned long intState;
  int status=CSL_SOK;
 
  if(pscModNum>CSL_PSC_MOD_MAX) 
    return CSL_EFAIL;

  if(pscModState>CSL_PSC_MOD_STATE_MAX) 
    return CSL_EFAIL;
 
  CSL_intGlobalDisable(intState); 
  
  /*
    *  Step 0 - Ignore request if the state is already set as is
    */
  if ( ( ( *mdstat ) & 0x1f ) == pscModState ) {
      status = CSL_EFAIL;
      goto error_exit;
  }

  /*
    *  Step 1 - Wait for PTSTAT.GOSTAT to clear
    */
  while( *ptstat & 1 );

  /*
    *  Step 2 - Set MDCTLx.NEXT to new state
    */
  *mdctl &= ~0x1f;
  *mdctl |= pscModState;

  /*
    *  Step 3 - Start power transition ( set PTCMD.GO to 1 )
    */
  *ptcmd |= 1;

  /*
    *  Step 4 - Wait for PTSTAT.GOSTAT to clear
    */
  while( *ptstat & 1 );

  for(i=0;i<20000;i++);

  if ( ( ( *mdstat ) & 0x1f ) != pscModState ) {
      status = CSL_EFAIL;
      goto error_exit;
  }

error_exit:
  CSL_intGlobalRestore(intState); 

  if(status==CSL_SOK) 
    printk(" CSL: PSC SET %d, %d OK\n", pscModNum, pscModState);    
  else
    printk(" CSL: PSC SET %d, %d ERROR\n", pscModNum, pscModState);    
 
  return status;
}

