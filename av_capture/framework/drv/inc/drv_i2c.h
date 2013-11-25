

#ifndef _DRV_I2C_H_
#define _DRV_I2C_H_

#include <osa.h>

#define I2C_TRANSFER_SIZE_MAX   (254)

typedef struct {

  int fd;

} DRV_I2cHndl;

int DRV_i2cOpen(DRV_I2cHndl *hndl, Uint8 devAddr);
int DRV_i2cRead8  (DRV_I2cHndl *hndl, Uint8 *reg, Uint8 *value, Uint8 count);
int DRV_i2cWrite8 (DRV_I2cHndl *hndl, Uint8 *reg, Uint8 *value, Uint8 count);
int DRV_i2cRead16 (DRV_I2cHndl *hndl, Uint8 *reg, Uint16 *value, Uint8 count);
int DRV_i2cWrite16(DRV_I2cHndl *hndl, Uint8 *reg, Uint16 *value, Uint8 count);

int DRV_i2c16Read8  (DRV_I2cHndl *hndl, Uint16 *reg, Uint8 *value, Uint8 count);
int DRV_i2c16Write8 (DRV_I2cHndl *hndl, Uint16 *reg, Uint8 *value, Uint8 count);
int DRV_i2c16Read16 (DRV_I2cHndl *hndl, Uint16 *reg, Uint16 *value, Uint8 count);
int DRV_i2c16Write16(DRV_I2cHndl *hndl, Uint16 *reg, Uint16 *value, Uint8 count);
int DRV_i2cClose(DRV_I2cHndl *hndl);

#endif
