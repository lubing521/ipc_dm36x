
#include <csl_hndl.h>
#include <csl_usrDrv.h>
#include <csl_sysIoctl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

CSL_VpssObj       gCSL_vpssHndl;
CSL_RszObj        gCSL_rszHndl;
CSL_IpipeObj      gCSL_ipipeHndl;
CSL_CcdcObj       gCSL_ccdcHndl;
CSL_IpipeifObj    gCSL_ipipeifHndl;
CSL_H3aObj        gCSL_h3aHndl;
CSL_LdcObj        gCSL_ldcHndl;
CSL_FaceDetectObj gCSL_faceDetectHndl;
CSL_GpioObj       gCSL_gpioHndl;

int CSL_sysInit()
{
  int status=CSL_SOK;

  status |= CSL_vpssOpen(&gCSL_vpssHndl);  
  status |= CSL_rszOpen(&gCSL_rszHndl);
  status |= CSL_ipipeOpen(&gCSL_ipipeHndl);
  status |= CSL_ccdcOpen(&gCSL_ccdcHndl);
  status |= CSL_ipipeifOpen(&gCSL_ipipeifHndl);
  status |= CSL_h3aOpen(&gCSL_h3aHndl);
  status |= CSL_ldcOpen(&gCSL_ldcHndl);
  status |= CSL_faceDetectOpen(&gCSL_faceDetectHndl);
  status |= CSL_gpioOpen(&gCSL_gpioHndl);
      
  return status;
}

int CSL_sysExit()
{
  int status=CSL_SOK;

  status |= CSL_gpioClose(&gCSL_gpioHndl);
  status |= CSL_faceDetectClose(&gCSL_faceDetectHndl);  
  status |= CSL_ldcClose(&gCSL_ldcHndl);
  status |= CSL_h3aClose(&gCSL_h3aHndl);
  status |= CSL_ipipeifClose(&gCSL_ipipeifHndl);
  status |= CSL_ccdcClose(&gCSL_ccdcHndl);
  status |= CSL_ipipeClose(&gCSL_ipipeHndl);
  status |= CSL_rszClose(&gCSL_rszHndl);
  status |= CSL_vpssClose(&gCSL_vpssHndl);
    
  return status;
}


int CSL_usrDrvOpen(Uint32 module)
{
  char    deviceName[20];

  if (module >= CSL_MODULE_MAX)
    return -1;

  sprintf(deviceName, "/dev/%s%d", CSL_DRV_NAME, module);

  return open(deviceName, O_RDWR);
}

CSL_Status CSL_usrDrvClose(int fd)
{
  return close(fd);
}

CSL_Status CSL_usrDrvIoctl(int fd, Uint32 cmd, void *prm)
{
  cmd = CSL_IOCTL_CMD_MAKE(cmd);
  return ioctl(fd, cmd, prm);
}

void *CSL_usrDrvMmap(int fd, unsigned long physAddr, unsigned int len)
{
  void *virtAddr;
  
  virtAddr = mmap (0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd,	physAddr);    
  
  if(virtAddr==NULL || virtAddr==MAP_FAILED)
    return NULL;
  
  return virtAddr;
}

CSL_Status CSL_usrDrvUnmap(int fd, void *virtAddr, unsigned int len)
{
  return munmap (virtAddr, len);
}
