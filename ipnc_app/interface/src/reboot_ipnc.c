#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <sys/ioctl.h>

void restart_ipcam(void)
{
	char cmd[256];

	int dev_fp;
	void *pMem_map;
	unsigned long phyAddr= 0x01C41000;
	unsigned long length = 0xAE0;
	unsigned int *pPSCCtrl = NULL;
	unsigned int *pPTCMD = NULL;


	if ((dev_fp=open("/dev/mem",O_RDWR|O_SYNC))==-1)
	{
		printf("dev_fp Fail!! \n");
		return ;
	}

	pMem_map=mmap(	(void	*)phyAddr,length/*0x10000000*/,
					PROT_READ|PROT_WRITE|PROT_EXEC,MAP_SHARED,
					dev_fp,phyAddr);

	if (pMem_map!=(void *)phyAddr)
	{
		printf("pMem_map Fail!! \n");
		return ;
	}


	pPTCMD 		= (unsigned int *)( (char *)pMem_map+0x120);
	pPSCCtrl 	= (unsigned int *)( (char *)pMem_map+0xA00);

	/*EDMA */
	*(pPSCCtrl+0) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*EDMA */
	*(pPSCCtrl+1) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*EDMA */
	*(pPSCCtrl+2) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*EDMA */
	*(pPSCCtrl+3) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*EDMA */
	*(pPSCCtrl+4) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*VPSS */
	*(pPSCCtrl+47) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*MJCP */
	*(pPSCCtrl+50) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);
	/*HDVICP */
	*(pPSCCtrl+51) 	= 2;
	*pPTCMD 		= 1;
	usleep(5000);

	if( pMem_map )
		munmap(pMem_map, length);

	if( dev_fp >= 0)
		close(dev_fp);

	sprintf(cmd, "shutdown -r now\n");
	system(cmd);

	return;
}


int main(void)
{
	restart_ipcam();
	return 1;
}
