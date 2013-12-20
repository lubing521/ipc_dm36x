
#ifndef _DRV_GPIO_H_
#define _DRV_GPIO_H_

#include <drv.h>
#include <drv_csl.h>

#define DRV_GPIO_DIR_OUT        (CSL_GPIO_OUTPUT)
#define DRV_GPIO_DIR_IN         (CSL_GPIO_INPUT)
#define DRV_GPIO_INT_TRIG_RISE  (CSL_GPIO_INTR_TRIG_RISE)
#define DRV_GPIO_INT_TRIG_FALL  (CSL_GPIO_INTR_TRIG_FALL)
#define DRV_GPIO_INT_TRIG_DUAL  (CSL_GPIO_INTR_TRIG_DUAL)

int DRV_gpioSetMode(Uint32 num, Uint16 mode);
int DRV_gpioSet(Uint32 num);
int DRV_gpioClr(Uint32 num);
int DRV_gpioGet(Uint32 num);

#endif


