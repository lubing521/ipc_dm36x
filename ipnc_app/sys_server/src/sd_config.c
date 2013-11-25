/** ===========================================================================
* @file sd_config.c
*
* @path $(IPNCPATH)\sys_server\src\
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2009
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */

#include <asm/types.h>
#include <dm355_gio_util.h>
#include <stdio.h>
#include <stdlib.h>
#include <system_default.h>
#include <file_msg_drv.h>
#include <unistd.h>
#include "schedule_mng.h"
#include "sd_config.h"

#define SD_INSERTED 0x01
#define SD_MOUNTED  0x02
#define SD_INSERT_AND_MOUNT 0x03

#define GIO_SD_DETECT			86	/*Low True*/
#define GIO_SD_WRITE_PROTECT	85

#define __E(fmt, args...) fprintf(stderr, "Error: " fmt, ## args)

extern int gAS_Pause;

char gbSD_WriteProtect = 1;
static __u8 gSdStatus = 0x00;

/**
* @brief Check if SD card is inserted
*
* @param isInit [I] if now is system initial call
* @param bEnable [I] if really do check or not
* @return Value to determind if system environment update
* @retval 1 status update
* @retval 0 status not update
*/
int CheckSDCard(int isInit, int bEnable)
{
	__u8 bSdInsert;
	static int count = 0;
	char cmd[80];
	if(bEnable == 0)
		bSdInsert = 1;
	else
		bSdInsert = dm355_gio_read(GIO_SD_DETECT) ? 0 : 1;
	if (bSdInsert != (gSdStatus & SD_INSERTED) || isInit)
	{
		if(bSdInsert)
		{
			if(isInit){
				/* Because schedule manager is not ready, 
				we don't nead to pause and resume it. */
				gAS_Pause = 1;
				system("umount /mnt/mmc\n");
				system("mount -t vfat /dev/mmcblk0 /mnt/mmc\n");
				system("mount -t vfat /dev/mmcblk0p1 /mnt/mmc\n");
				sprintf(cmd, "mkdir -p %s\n", SD_PATH);
				system(cmd);
				count = 0;
				gSdStatus |= SD_INSERT_AND_MOUNT;
				gbSD_WriteProtect = dm355_gio_read(GIO_SD_WRITE_PROTECT);
				fSetSDInsert(gSdStatus);
				gAS_Pause = 0;
				return 1;
			} else if(count < 7){
				count ++;
				return 0;
			}else{
				gAS_Pause = 1;
				PauseSchedule();
				system("umount /mnt/mmc\n");
				system("mount -t vfat /dev/mmcblk0 /mnt/mmc\n");
				system("mount -t vfat /dev/mmcblk0p1 /mnt/mmc\n");
				sprintf(cmd, "mkdir -p %s\n", SD_PATH);
				system(cmd);
				count = 0;
				gSdStatus |= SD_INSERT_AND_MOUNT;
				gbSD_WriteProtect = dm355_gio_read(GIO_SD_WRITE_PROTECT);
				fSetSDInsert(gSdStatus);
				gAS_Pause = 0;
				ResumeSchedule();
				return 1;
			}
		}
		else
		{
			if ((gSdStatus & SD_MOUNTED) || isInit)
				system("umount /mnt/mmc\n");
			gSdStatus &= ~(SD_INSERT_AND_MOUNT);
			fSetSDInsert(gSdStatus);
		}
		return 1;
	}
	return 0;
}

/**
* @brief Unmount SD card
* @retval 0 Success
*/
int SdUnmount(unsigned char value)
{
	/* Stop SD card recording */
	gAS_Pause = 1;
	PauseSchedule();
	int ret = 0;
	if (value == 1) {
		ret = system("umount /mnt/mmc\n");
		if (ret == 0) {
			gSdStatus &= ~(SD_MOUNTED);
			fSetSDInsert(gSdStatus);
		}
		sleep(1);
	} else if (value == 8) {
		char cmd[80];
		__u8 bSdInsert = dm355_gio_read(GIO_SD_DETECT) ? 0 : 1;
		if (bSdInsert == 1) {
			system("umount /mnt/mmc\n");
			system("mount -t vfat /dev/mmcblk0 /mnt/mmc\n");
			system("mount -t vfat /dev/mmcblk0p1 /mnt/mmc\n");
			sprintf(cmd, "mkdir -p %s\n", SD_PATH);
			ret = system(cmd);
			gSdStatus |= SD_INSERT_AND_MOUNT;
			gbSD_WriteProtect = dm355_gio_read(GIO_SD_WRITE_PROTECT);
			fSetSDInsert(gSdStatus);
		} else {
			ret = -1;
		}
	}
	gAS_Pause = 0;
	ResumeSchedule();
	return ret;
}
/**
* @brief Format SD card
* @retval -1 Error
*/
int SdFormat()
{
	int ret = 0;
	if(gbSD_WriteProtect)
		return -1;
	if(system("umount /mnt/mmc\n")){
		__E("SD umount fail\n");
		return -1;
	}
	if(system("mkdosfs /dev/mmcblk0p1\n") && system("mkdosfs /dev/mmcblk0\n")){
		__E("SD format fail\n");
		ret = -1;
	}
	if(system("mount -t vfat /dev/mmcblk0p1 /mnt/mmc/\n") &&
			system("mount -t vfat /dev/mmcblk0 /mnt/mmc/\n")){
		__E("SD mount fail\n");
		ret = -1;
	}
	if(system("mkdir /mnt/mmc/ipnc/\n")){
		__E("SD make dir ipnc fail\n");
		return -1;
	}
	return ret;
}
