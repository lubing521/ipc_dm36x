

#ifndef _DEV_I2C_H_
#define _DEV_I2C_H_

#include <linux/ioctl.h>


#define I2C_DRV_NAME  "dev_i2c"
//#define I2C_DEBUG

#define I2C_DEV_MAX_ADDR  (0xFF)

#define I2C_DRV_MAGICNUM      (0x2C)

#define I2C_CMD_SET_DEV_ADDR        (0x01)
#define I2C_CMD_READ                (0x02)
#define I2C_CMD_WRITE               (0x03)

#define I2C_IOCTL_CMD_MAKE(cmd)     ( _IO( I2C_DRV_MAGICNUM, cmd) )
#define I2C_IOCTL_CMD_GET(cmd)      ( _IOC_NR(cmd) )
#define I2C_IOCTL_CMD_IS_VALID(cmd) ( (_IOC_TYPE(cmd) == I2C_DRV_MAGICNUM ) ? 1 : 0)

#define I2C_TRANSFER_BUF_SIZE_MAX   (256)

typedef struct {
  unsigned char dataSize;
  unsigned char count;
  unsigned char regSize;  
  unsigned char *reg;  
  unsigned short *reg16;  
  void          *value;
} I2C_TransferPrm;


int  I2C_devInit(void);
void I2C_devExit(void);

#endif

