/** ===========================================================================
* @file main.c
*
* @path $(IPNCPATH)\sys_adm\boot_proc
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2010
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <Sys_Msg_Def.h>
#include <sys_msg_drv.h>
#include <signal.h>

#ifdef DEBUG
#define DBG(fmt, args...)	printf("Debug " fmt, ##args)
#else
#define DBG(fmt, args...)
#endif

#define ERR(fmt, args...)	printf("Error " fmt, ##args)

void usage()
{
	printf("This program should be used only once in the boot flow.\n");
	printf("Only one parameter is allowed which is a number of interger.\n");
	printf("This number will be passed to system server to control boot.\n");
}

void alarm_handler(int sig)
{
	printf("TimeOut occure in boot_proc.\n");
	printf("Program exit.\n");
	exit(1);
}

/**
 * @brief	main entry
 * @param	command line argument
 * @return	error code
 */
int main(int argc,char *argv[]) {
	int value;
	if(SysDrvInit(SYS_BOOT_MSG) < 0){
		ERR("System message driver initialize failed.\n");
		exit(1);
	}
	if(argc != 2){
		usage();
	}else{
		value = atoi(argv[1]);
		if(signal(SIGALRM, alarm_handler) != SIG_ERR){
			alarm(10);
			if(sizeof(value) != SysDoBootProc(&value, sizeof(value)))
				ERR("SysDoBootProc control fail.\n");
			else
				DBG("SysDoBootProc control success.\n");
		}
	}
	SysDrvExit();
	return 0;
}
