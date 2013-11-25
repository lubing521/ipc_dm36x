
#ifndef _CSL_USR_DRV_H_
#define _CSL_USR_DRV_H_

#include <csl_soc.h>

int        CSL_usrDrvOpen(Uint32 module);
CSL_Status CSL_usrDrvClose(int fd);
CSL_Status CSL_usrDrvIoctl(int fd, Uint32 cmd, void *prm);
void      *CSL_usrDrvMmap(int fd, unsigned long physAddr, unsigned int len);
CSL_Status CSL_usrDrvUnmap(int fd, void *virtAddr, unsigned int len);

#endif
