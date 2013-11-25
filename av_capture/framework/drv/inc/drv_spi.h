

#ifndef _DRV_SPI_H_
#define _DRV_SPI_H_

#include <osa.h>

typedef struct {

  int fd;

} DRV_SPIHndl;

int DRV_SPIOpen(DRV_SPIHndl *hndl, Uint8 devAddr);
int DRV_SPIRead8(DRV_SPIHndl *hndl, Uint8 *buf,  Uint32 count, Uint8 *Obuf);
int DRV_SPIWrite8(DRV_SPIHndl *hndl, Uint8 *buf, Uint32 count);
int DRV_SPIClose(DRV_SPIHndl *hndl);

#endif
