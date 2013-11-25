
#ifndef _DRV_GPIO_H_
#define _DRV_GPIO_H_

#include <drv.h>
#include <drv_csl.h>

#define DRV_GPIO_DIR_OUT        (CSL_GPIO_OUTPUT)
#define DRV_GPIO_DIR_IN         (CSL_GPIO_INPUT)
#define DRV_GPIO_INT_TRIG_RISE  (CSL_GPIO_INTR_TRIG_RISE)
#define DRV_GPIO_INT_TRIG_FALL  (CSL_GPIO_INTR_TRIG_FALL)
#define DRV_GPIO_INT_TRIG_DUAL  (CSL_GPIO_INTR_TRIG_DUAL)

static inline int DRV_gpioSetMode(Uint32 num, Uint16 mode)
{
  return CSL_gpioSetMode(&gCSL_gpioHndl, num, mode);
}

static inline int DRV_gpioSet(Uint32 num)
{
  return CSL_gpioSet(&gCSL_gpioHndl, num);
}

static inline int DRV_gpioClr(Uint32 num)
{
  return CSL_gpioClr(&gCSL_gpioHndl, num);
}

static inline int DRV_gpioGet(Uint32 num)
{
  return CSL_gpioGet(&gCSL_gpioHndl, num);
}

#endif


