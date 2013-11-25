/** ===========================================================================
* @file checkPlatform.c
*
* @path $(IPNCPATH)\sys_server\src\
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

/**
 * @brief	CheckCpuSpeed
 * @param	none
 * @return	0 : DM365 \n
 *			1 : DM368 \n
 *			-1: error
 */
int CheckCpuSpeed( void )
{
	static int CurrentStatus = -255;
	int fd_cpu = -1;
	int ret = -1;
	char StrBuffer[300];
	char TmpBuf[50];
	char *pStr = NULL;
	float cpu_mips = 0;
	char delima_buf[] = ":\r\n ";

	if( CurrentStatus >= -1 )
	{
		fprintf(stderr,"CPU CurrentStatus is = %d \n", CurrentStatus);
		return CurrentStatus;
	}

	fd_cpu = open("/proc/cpuinfo", O_RDONLY);
	if( fd_cpu < 0 )
	{
		fprintf(stderr,"CheckCpuSpeed open file fail \n");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = read(fd_cpu,  StrBuffer, sizeof(StrBuffer)-1);
	if( ret <= 0 )
	{
		fprintf(stderr,"read device error !!");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = -1;
	StrBuffer[sizeof(StrBuffer)-1] = '\0';
	pStr = strtok(StrBuffer,delima_buf );

	while( pStr != NULL )
	{
		sscanf( pStr,"%s",TmpBuf);

		if( strncmp(TmpBuf, "BogoMIPS", sizeof("BogoMIPS")) == 0 )
		{

			pStr = strtok(NULL, delima_buf);

			sscanf( pStr,"%f",&cpu_mips);

			//fprintf(stderr,"CPU MIPS = %f \n", cpu_mips);

			if( cpu_mips > 100 && cpu_mips <= 150 )
			{
				fprintf(stderr,"\nCPU is DM365\n");
				ret = 0;
				goto CHECK_CPU_END;
			}
			else if( cpu_mips > 150)
			{
				fprintf(stderr,"\nCPU is DM368\n");
				ret = 1;
				goto CHECK_CPU_END;
			}
		}

		pStr = strtok(NULL, delima_buf);

	}

CHECK_CPU_END:

	if( fd_cpu >= 0 )
		close(fd_cpu);


	CurrentStatus = ret;

	return ret;
}


