
#ifndef _CSL_GPIO_IOCTL_
#define _CSL_GPIO_IOCTL_

#include <csl_gpio.h>

#define CSL_GPIO_CMD_SET_MODE             (0x00)  ///< prm: [I ] CSL_GpioPrm *
#define CSL_GPIO_CMD_SET                  (0x01)  ///< prm: [I ] Uint32
#define CSL_GPIO_CMD_CLR                  (0x02)  ///< prm: [I ] Uint32
#define CSL_GPIO_CMD_GET                  (0x03)  ///< prm: [I ] Uint32
#define CSL_GPIO_CMD_BANK_INT_ENABLE      (0x04)  ///< prm: [I ] CSL_GpioPrm *
#define CSL_GPIO_CMD_BANK_INT_GET_STATUS  (0x05)  ///< prm: [I ] CSL_GpioPrm *
#define CSL_GPIO_CMD_BANK_INT_CLR_STATUS  (0x06)  ///< prm: [I ] CSL_GpioPrm *
#define CSL_GPIO_CMD_PINMUX_SET           (0x07)  ///< prm: [I ] CSL_GpioPrm *
#define CSL_GPIO_CMD_PINMUX_GET           (0x08)  ///< prm: [I ] CSL_GpioPrm *
#define CSL_GPIO_CMD_PSC_SET              (0x09)  ///< prm: [I ] CSL_GpioPrm *

typedef struct {

  Uint32 num;
  Uint32 value;

} CSL_GpioPrm;

CSL_Status CSL_gpioInit(CSL_GpioHandle hndl);
CSL_Status CSL_gpioExit(CSL_GpioHandle hndl);
CSL_Status CSL_gpioHwControl(CSL_GpioHandle hndl, Uint32 cmd, void *prm);

#endif /* _CSL_GPIO_IOCTL_ */
