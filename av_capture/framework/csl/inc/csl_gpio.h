/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#ifndef _CSL_GPIO_H_
#define _CSL_GPIO_H_

#ifdef __cplusplus
extern  "C" {
#endif

#include <csl_soc.h>

/**
  \file csl_gpio.h
  \brief GPIO APIs
*/

/**
 \ingroup  CSL_VPSS
 \defgroup CSL_GPIO GPIO
 \brief General purpose IO (GPIO)

 @{
*/

// define's
#define CSL_GPIO_OUTPUT         (0x1)                                             ///< GPIO mode : Output
#define CSL_GPIO_INPUT          (0x2)                                             ///< GPIO mode : Input
#define CSL_GPIO_INTR_TRIG_RISE (CSL_GPIO_INPUT|0x4)                              ///< GPIO mode : Interrupt, rising trigger 
#define CSL_GPIO_INTR_TRIG_FALL (CSL_GPIO_INPUT|0x8)                              ///< GPIO mode : Interrupt, falling trigger 
#define CSL_GPIO_INTR_TRIG_DUAL (CSL_GPIO_INTR_TRIG_RISE|CSL_GPIO_INTR_TRIG_FALL) ///< GPIO mode : Interrupt, rising + falling trigger

#define CSL_GPIO_BANK_MAX       (7)         ///< GPIO max banks
#define CSL_GPIO_NUM_PER_BANK   (16)        ///< GPIO per bank  
#define CSL_GPIO_NUM_MAX        (103)       ///< GPIO max 


#define CSL_PSC_MOD_VPSS          (47)      ///< PSC Module: VPSS
#define CSL_PSC_MOD_MAX           (52)      ///< PSC Module: Max 

#define CSL_PSC_MOD_STATE_SWRESET_DISABLE (0)                         ///< PSC State: Sw reset disable
#define CSL_PSC_MOD_STATE_SYNC_RESET      (1)                         ///< PSC State: sync reset
#define CSL_PSC_MOD_STATE_DISABLE         (2)                         ///< PSC State: disable
#define CSL_PSC_MOD_STATE_ENABLE          (3)                         ///< PSC State: enable
#define CSL_PSC_MOD_STATE_MAX             (CSL_PSC_MOD_STATE_ENABLE)  ///< PSC State: Max


/**
  \brief Module Object Structure
*/
  typedef struct {

#ifdef CSL_KERNEL_API
    CSL_GpioRegsOvly regs;       ///< Register overlay
    CSL_SysRegsOvly  sysRegs;    ///< Register overlay    
#else
    int     fd;
#endif

  } CSL_GpioObj;

  typedef CSL_GpioObj *CSL_GpioHandle;  ///< Module Handle

// function's

/**
  \brief Open Module Handle

  \param hndl   [ O]  Module Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_gpioOpen(CSL_GpioHandle hndl);

/**
  \brief Close Module Handle

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_gpioClose(CSL_GpioHandle hndl);

/**
  \brief Module lock : Mutex Lock
  
  \param hndl     [I ] hndl
  \param timeout  [I ] CSL_SYS_TIMEOUT_NONE or CSL_SYS_TIMEOUT_FOREVER
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
  CSL_Status CSL_gpioLock(CSL_GpioHandle hndl, Uint32 timeout);

/**
  \brief Module unlock : Mutex Unlock
  
  \param hndl     [I ] hndl
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
  CSL_Status CSL_gpioUnlock(CSL_GpioHandle hndl);

/**
  \brief Set GPIO pin config
  
  \param hndl     [I ] hndl
  \param gpioNum  [I ] GPIO pin number
  \param mode     [I ] GPIO config mode \n CSL_GPIO_OUTPUT \n CSL_GPIO_INPUT \n CSL_GPIO_INTR_TRIG_RISE \n CSL_GPIO_INTR_TRIG_FALL \n CSL_GPIO_INTR_TRIG_DUAL
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
CSL_Status CSL_gpioSetMode(CSL_GpioHandle hndl, Uint32 gpioNum, Uint32 mode );

/**
  \brief Set GPIO pin to HIGH
  
  \param hndl     [I ] hndl
  \param gpioNum  [I ] GPIO pin number
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
CSL_Status CSL_gpioSet(CSL_GpioHandle hndl, Uint32 gpioNum );

/**
  \brief Set GPIO pin to LOW
  
  \param hndl     [I ] hndl
  \param gpioNum  [I ] GPIO pin number
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
CSL_Status CSL_gpioClr(CSL_GpioHandle hndl, Uint32 gpioNum );

/**
  \brief Get GPIO pin value
  
  \param hndl     [I ] hndl
  \param gpioNum  [I ] GPIO pin number
  
  \return 0 pin is LOW, 1 pin is HIGH
*/
Uint32     CSL_gpioGet(CSL_GpioHandle hndl, Uint32 gpioNum );

/**
  \brief Enable/Disable GPIO bank interrupt 
  
  \param hndl     [I ] hndl
  \param bankNum  [I ] GPIO bank number
  \param enable   [I ] TRUE: enable interrupt, FALSE: disable interrupt
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
CSL_Status CSL_gpioBankIntEnable(CSL_GpioHandle hndl, Uint32 bankNum, Bool32 enable );

/**
  \brief Get GPIO bank interrupt status
  
  \param hndl      [I ] hndl
  \param bankNum   [I ] GPIO bank number
  \param intStatus [ O] Interrupt status of GPIOs in the bank
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
CSL_Status CSL_gpioBankIntGetStatus(CSL_GpioHandle hndl, Uint32 bankNum, Uint16 *intStatus );

/**
  \brief Clear GPIO bank interrupt status
  
  \param hndl      [I ] hndl
  \param bankNum   [I ] GPIO bank number
  \param intStatus [I ] Interrupt status to clear of GPIOs in the bank
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
CSL_Status CSL_gpioBankIntClrStatus(CSL_GpioHandle hndl, Uint32 bankNum, Uint16 intStatus );


/**
  \brief Set pin mux register
  
  \param hndl        [I ] hndl
  \param pinMuxNum   [I ] 0..4
  \param pinMuxValue [I ] Pin mux value
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
CSL_Status CSL_gpioSetPinmux(CSL_GpioHandle hndl, Uint32 pinMuxNum, Uint32 pinMuxValue);

/**
  \brief Get pin mux register value
  
  \param hndl        [I ] hndl
  \param pinMuxNum   [I ] 0..4
  \param pinMuxValue [ O] Pin mux value
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
CSL_Status CSL_gpioGetPinmux(CSL_GpioHandle hndl, Uint32 pinMuxNum, Uint32 *pinMuxValue);


/**
  \brief Set PSC module state
  
  \param hndl        [I ] hndl
  \param pscModNum   [I ] PSC module num
  \param pscModState [I ] PSC module state
  
  \return CSL_SOK on success, else CSL_Exxxx  
*/
CSL_Status CSL_gpioSetPsc(CSL_GpioHandle hndl, Uint32 pscModNum, Uint32 pscModState);


#ifdef __cplusplus
}
#endif
/*@}*/
#endif
