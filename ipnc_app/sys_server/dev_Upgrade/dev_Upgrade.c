/*************************************************
Copyright (C), 2001-2012, TMVIDEO. Co., Ltd.
File name: // 文件名f
Author: Version: Date: 刘纹高 2012-03-07 // 作者、版本及完成日期
Description: // 用于详细说明此程序文件完成的主要功能，与其他模块
// 或函数的接口，输出值、取值范围、含义及参数间的控
Others: // 其它内容的说明
Function List: // 主要函数列表，每条记录应包括函数名及功能简要说明
History: // 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改
内容简述
Date:
Author:
Modification:
*************************************************/




#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <termios.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <pthread.h>
#include <linux/sem.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <linux/route.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>


#include "dev_Upgrade.h"
#include <sys/stat.h> 


//获取文件长度
/*************************************************
Function: unsigned long get_file_size(const char *path)
Description: 获取文件长度。
Input: int size 为升级文件大小, char *buffer 升级文件数据
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 按照18M Rootfs空间设计
author: 刘纹高 2013-01-07
*************************************************/
unsigned long get_file_size(const char *path)  
{  
    unsigned long filesize = -1;      
    struct stat statbuff;  
    if(stat(path, &statbuff) < 0){  
        return filesize;  
    }else{  
        filesize = statbuff.st_size;  
    }  
    return filesize;  
}  






//查找升级文件进行升级
/*************************************************
Function: int Auto_Temp_Updata_File()  
Description: 首先发现升级文件，进行自动升级。
Input: 
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 
author: 刘纹高 2013-01-27
*************************************************/
int Auto_Temp_Updata_File()  
{  
	unsigned long nUpdatalength;


	

	//查找统一三个区升级文件,并自动升级
	printf("\n-------------------------------------------\n");
	nUpdatalength = 0;
	nUpdatalength = get_file_size("/sys_ext/sys_all_three_mtdblock.tar");
	printf("0 . ALL THREE sys_all_three_mtdblock.tar file size [%ld]\n" , nUpdatalength);
	if(nUpdatalength>1024&&nUpdatalength<0x3000000)
	{
		//首先关闭所有运行的主程序
		system("killall -9 ipcam");
		system("killall -9 av_server.out");
		sync();
		//将升级文件更新对于的文件路径区域
		system("/bin/tar -xf /sys_ext/sys_all_three_mtdblock.tar -C / ");
		//同步
		sync();
		//删除临时存储文件
		system("rm -f /sys_ext/sys_all_three_mtdblock.tar");
		sync();	
		return 1;
	}


   //查找APP升级文件,并自动升级
	printf("\n-------------------------------------------\n");
	nUpdatalength = 0;
	nUpdatalength = get_file_size("/sys_ext/sys_app_mtdblock5.tar");
	printf("1 . APP sys_lib_mtdblock5.tar file size [%ld]\n" , nUpdatalength);
	if(nUpdatalength>1024&&nUpdatalength<0x2000000)
	{
		//首先关闭所有运行的主程序
		system("killall -9 ipcam");
		system("killall -9 av_server.out");
		sync();
		//将升级文件更新对于的文件路径区域
		system("/bin/tar -xf /sys_ext/sys_app_mtdblock5.tar -C / ");
		//同步
		sync();
		//删除临时存储文件
		system("rm -f /sys_ext/sys_app_mtdblock5.tar");
		sync();	
		return 1;
	}

	//查找DEV升级文件,并自动升级
	nUpdatalength = 0;
	nUpdatalength = get_file_size("/sys_ext/sys_dev_mtdblock6.tar");
	
	printf("2 DEV sys_lib_mtdblock6.tar file size [%ld]\n" , nUpdatalength);
	if(nUpdatalength>1024&&nUpdatalength<0x2000000)
	{
		//首先关闭所有运行的主程序
		system("killall -9 ipcam");
		system("killall -9 av_server.out");
		sync();
		//将升级文件更新对于的文件路径区域
		system("/bin/tar -xf /sys_ext/sys_dev_mtdblock6.tar -C / ");
		//同步
		sync();
		//删除临时存储文件
		system("rm -f /sys_ext/sys_dev_mtdblock6.tar");
		sync();
		return 1;	
	}

	//查找LIB升级文件,并自动升级
	nUpdatalength = 0;
	nUpdatalength = get_file_size("/sys_ext/sys_lib_mtdblock7.tar");
	
	printf("3 . LIB sys_lib_mtdblock7.tar file size [%ld]\n" , nUpdatalength);
	if(nUpdatalength>1024&&nUpdatalength<0x2000000)
	{
		//首先关闭所有运行的主程序
		system("killall -9 ipcam");
		system("killall -9 av_server.out");
		sync();
		//将升级文件更新对于的文件路径区域
		system("/bin/tar -xf /sys_ext/sys_lib_mtdblock7.tar -C / ");
		//同步
		sync();
		//删除临时存储文件
		system("rm -f /sys_ext/sys_lib_mtdblock7.tar");
		sync();
		return 1;	
	} 

	//查找工厂出厂参数升级文件,并自动升级
	nUpdatalength = 0;
	nUpdatalength = get_file_size("/sys_ext/sys_fac_mtdblock4.tar");
	
	printf("4 . FACTORY sys_fac_mtdblock4.tar file size [%ld]\n" , nUpdatalength);
	if(nUpdatalength>1024&&nUpdatalength<0x2000000)
	{
		//首先关闭所有运行的主程序
		system("killall -9 ipcam");
		system("killall -9 av_server.out");
		sync();
		//将升级文件更新对于的文件路径区域
		system("/bin/tar -xf /sys_ext/sys_fac_mtdblock4.tar -C / ");
		//同步
		sync();
		//删除临时存储文件
		system("rm -f /sys_ext/sys_fac_mtdblock4.tar");
		sync();
		return 1;	
	} 
	return 0;
}  




/*************************************************
Function: Rootfs_UpdateFlash(int size, char *buffer)
Description: 打开FLASH文件写入数据。
Input: int size 为升级文件大小, char *buffer 升级文件数据
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 按照18M Rootfs空间设计
author: 刘纹高 2012-03-07
*************************************************/
int Rootfs_UpdateFlash(int size, char *buffer)
{

	int rootfs_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=0x17FFFFF;	//文件系统区分为18M

	nFileLength=size;
	//默认写入FLASH正确
	flash_err=FLASH_UPGRADE_OK;
	//检查文件大小，防止错误尺寸文件被写入
	if(nFileLength<=23*1024*1024||nFileLength>=24*1024*1024)
	{
		printf("file size err [%d] \n", nFileLength);
		//发现写入数据大小错误
		flash_err=FLASH_UPGRADE_DATA_SIZE_ERR;
		goto ErrDispose;
	}


# if D_WRITE_FLASH_TASK

	
	if ((rootfs_flashfs=open(MTD_ROOTFS_UPDATE_DEV, O_WRONLY))<0)
	{
		printf("Rootfs O_WRONLY open FLASH Block [%s] fail\n", MTD_ROOTFS_UPDATE_DEV);
		//打开FLASH设备失败
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Rootfs O_WRONLY Open Success [%s] rootfs_flashfs [%d]\n" , MTD_ROOTFS_UPDATE_DEV , rootfs_flashfs);
	}
	//写入数据
	FlashOffset=0 ;
	WriteBlock=64*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			//WriteBlock=nFileLength;

			//WriteBlock=32*1024;

			WriteBlock=nFileLength-(nFileLength%64);


			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
	
		//写入升级文件
		if(write(rootfs_flashfs , buffer+FlashOffset , WriteBlock)!=WriteBlock)
		{
			printf("Rootfs write error WriteBlock [%d]\n" , WriteBlock);
			//写入数据到FLASH设备失败
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(200000);
		FlashOffset+=WriteBlock;
		//通知网络进度
		SendWriteFlashMsg(0x17FFFFF , FlashOffset);
		printf("Rootfs Write ok WriteBlock [%d] FlashOffset [%d]\n" , WriteBlock , FlashOffset);
		
	}

/*
	//擦出去区域数据为0XFF		
	unsigned char flushbufer[64*1024];
	FlushBlock=64*1024;
	nRootfs_CountLen=0x17FFFFF;
	if(nRootfs_CountLen>FlashOffset)
	{
		nRootfs_CountLen-=FlashOffset;
	}
	else
	{
		nRootfs_CountLen=0;
	}
	printf("Rootfs Flush nRootfs_CountLen [%d]\n" , nRootfs_CountLen);
	while(nRootfs_CountLen>0)
	{
		if(nRootfs_CountLen<FlushBlock)
		{
			FlushBlock=nFileLength;
			nRootfs_CountLen=0;
		}
		else
		{
			nRootfs_CountLen-=FlushBlock;
		}
	
		//擦写FLASH
		memset(flushbufer , 0xFF , sizeof(flushbufer));
		if(write(rootfs_flashfs ,flushbufer , FlushBlock)!=FlushBlock)
		{
			printf("write error FlushBlock [%d]\n" , FlushBlock);
			//写入数据到FLASH设备失败
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(10);
		FlashOffset+=FlushBlock;
		//通知网络进度
		SendWriteFlashMsg(0x17FFFFF , FlashOffset);
		printf("Rootfs Flush Write ok FlushBlock [%d] FlashOffset [%d]\n" , FlushBlock , FlashOffset);
	}
*/

# endif

ErrDispose:
	
	sync();
	if (rootfs_flashfs>0)
	{
		close(rootfs_flashfs);
		rootfs_flashfs=-1;
	}

	/*这是测试写入FLASH是否成功，检验程序*/
# if D_READ_FLASH_TEST
	Read_Flash_Data(0x300000 , NULL , MTD_ROOTFS_UPDATE_DEV , "rRootfs.jfs2");
#endif
	return flash_err;
}



/*************************************************
Function: Kernel_UpdateFlash(int size, char *buffer)
Description: 打开Kernel 分区 FLASH文件写入数据。
Input: int size 为升级文件大小, char *buffer 升级文件数据
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 按照2M Kernel空间设计
author: 刘纹高 2012-03-07
*************************************************/
int Kernel_UpdateFlash(int size, char *buffer)
{

	int kernel_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=0x1FFFFF;	//Kernel 为2M

	nFileLength=size;
	//默认写入FLASH正确
	flash_err=FLASH_UPGRADE_OK;
	//检查文件大小，防止错误尺寸文件被写入
	if(nFileLength<=1*1024*1024||nFileLength>=2*1024*1024)
	{
		printf("file size err [%d] \n", nFileLength);
		//发现写入数据大小错误
		flash_err=FLASH_UPGRADE_DATA_SIZE_ERR;
		goto ErrDispose;
	}

# if D_WRITE_FLASH_TASK

	
	if ((kernel_flashfs=open(MTD_KERNEL_UPDATE_DEV, O_WRONLY))<0)
	{
		printf("Kernel O_WRONLY open FLASH Block [%s] fail\n", MTD_KERNEL_UPDATE_DEV);
		//打开FLASH设备失败
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Kernel O_WRONLY Open Success [%s] kernel_flashfs [%d]\n" , MTD_KERNEL_UPDATE_DEV , kernel_flashfs);
	}
	//写入数据
	FlashOffset=0 ;
	WriteBlock=64*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			//WriteBlock=nFileLength;
			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
	
		//写入升级文件
		if(write(kernel_flashfs , buffer+FlashOffset , WriteBlock)!=WriteBlock)
		{
			printf("write error WriteBlock [%d]\n" , WriteBlock);
			//写入数据到FLASH设备失败
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(200000);
		FlashOffset+=WriteBlock;
		//通知网络进度
		SendWriteFlashMsg(0x1FFFFF , FlashOffset);
		printf("Kernel Write ok WriteBlock [%d] FlashOffset [%d]\n" , WriteBlock , FlashOffset);
		
	}

/*
	//擦出去区域数据为0XFF		
	unsigned char flushbufer[64*1024];
	FlushBlock=64*1024;
	nRootfs_CountLen=0x1FFFFF;
	if(nRootfs_CountLen>FlashOffset)
	{
		nRootfs_CountLen-=FlashOffset;
	}
	else
	{
		nRootfs_CountLen=0;
	}
	printf("Kernel Flush nRootfs_CountLen [%d]\n" , nRootfs_CountLen);
	while(nRootfs_CountLen>0)
	{
		if(nRootfs_CountLen<FlushBlock)
		{
			FlushBlock=nFileLength;
			nRootfs_CountLen=0;
		}
		else
		{
			nRootfs_CountLen-=FlushBlock;
		}
	
		//擦写FLASH
		memset(flushbufer , 0xFF , sizeof(flushbufer));
		if(write(kernel_flashfs ,flushbufer , FlushBlock)!=FlushBlock)
		{
			printf("write error FlushBlock [%d]\n" , FlushBlock);
			//写入数据到FLASH设备失败
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(10);
		FlashOffset+=FlushBlock;
		//通知网络进度
		SendWriteFlashMsg(0x1FFFFF , FlashOffset);
		printf("Kernel Flush Write ok FlushBlock [%d] FlashOffset [%d]\n" , FlushBlock , FlashOffset);
	}
*/

# endif


ErrDispose:
	
	sync();
	if (kernel_flashfs>0)
	{
		close(kernel_flashfs);
		kernel_flashfs=-1;
	}

/*这是测试写入FLASH是否成功，检验程序*/
# if D_READ_FLASH_TEST
	Read_Flash_Data(0x200000 , NULL , MTD_KERNEL_UPDATE_DEV , "rkernel.image");
#endif

	return flash_err;
			
}




/*************************************************
Function: App_UpdateFlash(int size, char *buffer)
Description: 打开Kernel 分区 FLASH文件写入数据。
Input: int size 为升级文件大小, char *buffer 升级文件数据
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 按照28M Kernel空间设计
author: 刘纹高 2012-03-07
升级位置：sys_app_mtdblock5
*************************************************/
int App_UpdateFlash(int size, char *buffer)
{

	//DM368,升级文件
	
	int app_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock ,readywrite;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=size;	//应用程序区
	char cmd[80];
	unsigned long nUpdatalength;
	
	nFileLength=size;
	//默认写入FLASH正确
	flash_err=FLASH_UPGRADE_OK;
	
	system("rm -f /sys_ext/sys_app_mtdblock5.tar");
	sync();
	
	if ((app_flashfs=open("/sys_ext/sys_app_mtdblock5.tar", O_WRONLY|O_CREAT))<0)
	{
		printf("Kernel O_WRONLY open Create /sys_ext/sys_app_mtdblock5.tar [%d] fail\n", app_flashfs);
		//打开FLASH设备失败
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Kernel O_WRONLY Open Success Create /sys_ext/sys_app_mtdblock5.tar app_flashfs [%d]\n" , app_flashfs);
	}
	//写入数据
	FlashOffset=0 ;
	WriteBlock=8*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			WriteBlock=nFileLength;
			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
		
		//写入升级文件
		readywrite=write(app_flashfs , buffer+FlashOffset , WriteBlock);
		if(readywrite!=WriteBlock)
		{
			printf("write error WriteBlock [%d] readywrite [%d]\n" , WriteBlock ,readywrite);
			//写入数据到FLASH设备失败
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(100);
		FlashOffset+=WriteBlock;
		//通知网络进度
		SendWriteFlashMsg(nRootfs_CountLen , FlashOffset);
		printf("Kernel Write ok WriteBlock [%d] FlashOffset [%d] nFileLength [%d]\n" , WriteBlock , FlashOffset , nFileLength);
		
	}
	
	
ErrDispose:
	
	sync();
	if (app_flashfs>0)
	{
		close(app_flashfs);
		app_flashfs=-1;
	}
	sync();
	//如果没有发生任何错误，进行升级
	if(flash_err==FLASH_UPGRADE_OK)
	{
		nUpdatalength = 0;
		nUpdatalength = get_file_size("/sys_ext/sys_app_mtdblock5.tar");
		
		printf("Network size [%d] sys_lib_mtdblock5.tar file size [%ld]\n" , size , nUpdatalength);
		if(nUpdatalength>1024&&(size==nUpdatalength))
		{
			//同步
			sync();
			system("killall -9 ipcam");
			system("killall -9 av_server.out");
			sync();
			system("/usr/sbin/flash_eraseall /dev/mtd5 ");
			/*
			//首先关闭所有运行的主程序
			system("killall -9 ipcam");
			//将升级文件更新对于的文件路径区域
			system("/bin/tar -xf /sys_ext/sys_app_mtdblock5.tar -C / ");
			//同步
			sync();
			//删除临时存储文件
			system("rm -f /sys_ext/sys_app_mtdblock5.tar");
			sync();
			*/
			
		}
		else
		{
			//删除临时存储文件
			system("rm -f /sys_ext/sys_app_mtdblock5.tar");
			sync();
		}
	}
	
	
	return flash_err;
	
			
}




/*************************************************
Function: MTD_SYS_DEV
Description: 打开Kernel 分区 FLASH文件写入数据。
Input: int size 为升级文件大小, char *buffer 升级文件数据
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 按照16M Kernel空间设计
author: 刘纹高 2012-03-07
升级位置：sys_app_mtdblock5
*************************************************/
int SYS_Dev_Flash(int size, char *buffer)
{

	//DM368,升级文件
	
	int app_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock ,readywrite;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=size;	//应用程序区
	char cmd[80];
	unsigned long nUpdatalength;
	
	nFileLength=size;
	//默认写入FLASH正确
	flash_err=FLASH_UPGRADE_OK;
	
	system("rm -f /sys_ext/sys_dev_mtdblock6.tar");
	sync();
	
	if ((app_flashfs=open("/sys_ext/sys_dev_mtdblock6.tar", O_WRONLY|O_CREAT))<0)
	{
		printf("Kernel O_WRONLY open Create /sys_ext/sys_dev_mtdblock6.tar [%d] fail\n", app_flashfs);
		//打开FLASH设备失败
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Kernel O_WRONLY Open Success Create /sys_ext/sys_dev_mtdblock6.tar app_flashfs [%d]\n" , app_flashfs);
	}
	//写入数据
	FlashOffset=0 ;
	WriteBlock=8*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			WriteBlock=nFileLength;
			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
		
		//写入升级文件
		readywrite=write(app_flashfs , buffer+FlashOffset , WriteBlock);
		if(readywrite!=WriteBlock)
		{
			printf("write error WriteBlock [%d] readywrite [%d]\n" , WriteBlock ,readywrite);
			//写入数据到FLASH设备失败
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(100);
		FlashOffset+=WriteBlock;
		//通知网络进度
		SendWriteFlashMsg(nRootfs_CountLen , FlashOffset);
		printf("Kernel Write ok WriteBlock [%d] FlashOffset [%d] nFileLength [%d]\n" , WriteBlock , FlashOffset , nFileLength);
		
	}
	
	
ErrDispose:
	
	sync();
	if (app_flashfs>0)
	{
		close(app_flashfs);
		app_flashfs=-1;
	}
	sync();
	//如果没有发生任何错误，进行升级
	if(flash_err==FLASH_UPGRADE_OK)
	{
		nUpdatalength = 0;
		nUpdatalength = get_file_size("/sys_ext/sys_dev_mtdblock6.tar");
		
		printf("Network size [%d] sys_lib_mtdblock6.tar file size [%ld]\n" , size , nUpdatalength);
		if(nUpdatalength>1024&&(size==nUpdatalength))
		{
			//同步
			sync();
			system("killall -9 ipcam");
			system("killall -9 av_server.out");
			sync();
			system("/usr/sbin/flash_eraseall /dev/mtd6 ");
			/*
			//首先关闭所有运行的主程序
			system("killall -9 ipcam");
			//将升级文件更新对于的文件路径区域
			system("/bin/tar -xf /sys_ext/sys_dev_mtdblock6.tar -C / ");
			//同步
			sync();
			//删除临时存储文件
			system("rm -f /sys_ext/sys_dev_mtdblock6.tar");
			sync();
			*/	
		}
		else
		{
			//删除临时存储文件
			system("rm -f /sys_ext/sys_dev_mtdblock6.tar");
			sync();
		}
	}
	
	
	return flash_err;
	
	
}




/*************************************************
Function: MTD_SYS_LIB
Description: 打开Kernel 分区 FLASH文件写入数据。
Input: int size 为升级文件大小, char *buffer 升级文件数据
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 按照16M Kernel空间设计
author: 刘纹高 2012-03-07
升级位置：sys_app_mtdblock5
*************************************************/
int SYS_Dev_LIB(int size, char *buffer)
{


	//DM368,升级文件
	
	int app_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock ,readywrite;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=size;	//应用程序区
	char cmd[80];
	unsigned long nUpdatalength;
	
	nFileLength=size;
	//默认写入FLASH正确
	flash_err=FLASH_UPGRADE_OK;

	system("rm -f /sys_ext/sys_lib_mtdblock7.tar");
	sync();

	if ((app_flashfs=open("/sys_ext/sys_lib_mtdblock7.tar", O_WRONLY|O_CREAT))<0)
	{
		printf("Kernel O_WRONLY open Create /sys_ext/sys_lib_mtdblock7.tar [%d] fail\n", app_flashfs);
		//打开FLASH设备失败
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Kernel O_WRONLY Open Success Create /sys_ext/sys_lib_mtdblock7.tar app_flashfs [%d]\n" , app_flashfs);
	}
	//写入数据
	FlashOffset=0 ;
	WriteBlock=8*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			WriteBlock=nFileLength;
			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
		
		//写入升级文件
		readywrite=write(app_flashfs , buffer+FlashOffset , WriteBlock);
		if(readywrite!=WriteBlock)
		{
			printf("write error WriteBlock [%d] readywrite [%d]\n" , WriteBlock ,readywrite);
			//写入数据到FLASH设备失败
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(100);
		FlashOffset+=WriteBlock;
		//通知网络进度
		SendWriteFlashMsg(nRootfs_CountLen , FlashOffset);
		printf("Kernel Write ok WriteBlock [%d] FlashOffset [%d] nFileLength [%d]\n" , WriteBlock , FlashOffset , nFileLength);
		
	}

	
ErrDispose:
	
	sync();
	if (app_flashfs>0)
	{
		close(app_flashfs);
		app_flashfs=-1;
	}
	sync();
	//如果没有发生任何错误，进行升级
	if(flash_err==FLASH_UPGRADE_OK)
	{
		nUpdatalength = 0;
		nUpdatalength = get_file_size("/sys_ext/sys_lib_mtdblock7.tar");

		printf("Network size [%d] sys_lib_mtdblock7.tar file size [%ld]\n" , size , nUpdatalength);
		if(nUpdatalength>1024&&(size==nUpdatalength))
		{
			sync();
			system("killall -9 ipcam");
			system("killall -9 av_server.out");
			sync();
			system("/usr/sbin/flash_eraseall /dev/mtd7 ");
			/*
			//首先关闭所有运行的主程序
			system("killall -9 ipcam");
			//将升级文件更新对于的文件路径区域
			system("/bin/tar -xf /sys_ext/sys_lib_mtdblock7.tar -C / ");
			//同步
			sync();
			//删除临时存储文件
			system("rm -f /sys_ext/sys_lib_mtdblock7.tar");
			sync();
			*/

		}
		else
		{
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			//删除临时存储文件
			system("rm -f /sys_ext/sys_lib_mtdblock7.tar");
			sync();
		}
	}
	
	
	return flash_err;
	
}





/*************************************************
Function: SYS_All_Three_block
Description: 升级三个区域。
Input: int size 为升级文件大小, char *buffer 升级文件数据
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 按照16M Kernel空间设计
author: 刘纹高 2012-03-07
升级位置：sys_app_mtdblock5
*************************************************/
int SYS_All_Three_block(int size, char *buffer)
{


	//DM368,升级文件
	
	int app_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock ,readywrite;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=size;	//应用程序区
	char cmd[80];
	unsigned long nUpdatalength;
	
	nFileLength=size;
	//默认写入FLASH正确
	flash_err=FLASH_UPGRADE_OK;

	system("rm -f /sys_ext/sys_all_three_mtdblock.tar");
	sync();

	if ((app_flashfs=open("/sys_ext/sys_all_three_mtdblock.tar", O_WRONLY|O_CREAT))<0)
	{
		printf("Kernel O_WRONLY open Create /sys_ext/sys_all_three_mtdblock.tar [%d] fail\n", app_flashfs);
		//打开FLASH设备失败
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Kernel O_WRONLY Open Success Create /sys_ext/sys_all_three_mtdblock.tar app_flashfs [%d]\n" , app_flashfs);
	}
	//写入数据
	FlashOffset=0 ;
	WriteBlock=8*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			WriteBlock=nFileLength;
			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
		
		//写入升级文件
		readywrite=write(app_flashfs , buffer+FlashOffset , WriteBlock);
		if(readywrite!=WriteBlock)
		{
			printf("write error WriteBlock [%d] readywrite [%d]\n" , WriteBlock ,readywrite);
			//写入数据到FLASH设备失败
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(100);
		FlashOffset+=WriteBlock;
		//通知网络进度
		SendWriteFlashMsg(nRootfs_CountLen , FlashOffset);
		printf("Kernel Write ok WriteBlock [%d] FlashOffset [%d] nFileLength [%d]\n" , WriteBlock , FlashOffset , nFileLength);
		
	}

	
ErrDispose:
	
	sync();
	if (app_flashfs>0)
	{
		close(app_flashfs);
		app_flashfs=-1;
	}
	sync();
	//如果没有发生任何错误，进行升级
	if(flash_err==FLASH_UPGRADE_OK)
	{
		nUpdatalength = 0;
		nUpdatalength = get_file_size("/sys_ext/sys_all_three_mtdblock.tar");

		printf("Network size [%d] sys_all_three_mtdblock.tar file size [%ld]\n" , size , nUpdatalength);
		if(nUpdatalength>1024&&(size==nUpdatalength))
		{
			sync();
			usleep(100);

			system("killall -9 ipcam");
			system("killall -9 av_server.out");
			sync();
			usleep(100);
			printf("kill ipcam and av_server.out \n");
			
			system("/usr/sbin/flash_eraseall /dev/mtd5 ");
			sync();
			usleep(100);
			printf("/usr/sbin/flash_eraseall /dev/mtd5  \n");

			system("/usr/sbin/flash_eraseall /dev/mtd6 ");
			sync();
			usleep(100);
			printf("/usr/sbin/flash_eraseall /dev/mtd6  \n");

			system("/usr/sbin/flash_eraseall /dev/mtd7 ");
			sync();
			usleep(100);
			printf("/usr/sbin/flash_eraseall /dev/mtd7 \n");

			/*
			//首先关闭所有运行的主程序
			system("killall -9 ipcam");
			//将升级文件更新对于的文件路径区域
			system("/bin/tar -xf /sys_ext/sys_lib_mtdblock7.tar -C / ");
			//同步
			sync();
			//删除临时存储文件
			system("rm -f /sys_ext/sys_lib_mtdblock7.tar");
			sync();
			*/

		}
		else
		{
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			//删除临时存储文件
			system("rm -rf /sys_ext/sys_all_three_mtdblock.tar");
			sync();
		}
	}
	
	
	return flash_err;
	
}




/*************************************************
Function:  MTD_PARAMETER_UPDATE_DEV
Description: 参数区 Flash分区空间文件。
Input: int size 为升级文件大小, char *buffer 升级文件数据
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 按照16M Kernel空间设计
author: 刘纹高 2012-03-07
升级位置：sys_app_mtdblock5
*************************************************/
int SYS_Dev_PARAMETER(int size, char *buffer)
{
	
	//DM368,升级文件
	
	int app_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock ,readywrite;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=size;	//应用程序区
	char cmd[80];
	unsigned long nUpdatalength;
	
	nFileLength=size;
	//默认写入FLASH正确
	flash_err=FLASH_UPGRADE_OK;

	system("rm -f /sys_ext/sys_fac_mtdblock4.tar");
	sync();

	if ((app_flashfs=open("/sys_ext/sys_fac_mtdblock4.tar", O_WRONLY|O_CREAT))<0)
	{
		printf("Kernel O_WRONLY open Create /sys_ext/sys_fac_mtdblock4.tar [%d] fail\n", app_flashfs);
		//打开FLASH设备失败
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Kernel O_WRONLY Open Success Create /sys_ext/sys_fac_mtdblock4.tar app_flashfs [%d]\n" , app_flashfs);
	}
	//写入数据
	FlashOffset=0 ;
	WriteBlock=8*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			WriteBlock=nFileLength;
			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
		
		//写入升级文件
		readywrite=write(app_flashfs , buffer+FlashOffset , WriteBlock);
		if(readywrite!=WriteBlock)
		{
			printf("write error WriteBlock [%d] readywrite [%d]\n" , WriteBlock ,readywrite);
			//写入数据到FLASH设备失败
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(100);
		FlashOffset+=WriteBlock;
		//通知网络进度
		SendWriteFlashMsg(nRootfs_CountLen , FlashOffset);
		printf("Kernel Write ok WriteBlock [%d] FlashOffset [%d] nFileLength [%d]\n" , WriteBlock , FlashOffset , nFileLength);
		
	}

	
ErrDispose:
	
	sync();
	if (app_flashfs>0)
	{
		close(app_flashfs);
		app_flashfs=-1;
	}
	sync();
	//如果没有发生任何错误，进行升级
	if(flash_err==FLASH_UPGRADE_OK)
	{
		nUpdatalength = 0;
		nUpdatalength = get_file_size("/sys_ext/sys_fac_mtdblock4.tar");

		printf("Network size [%d] sys_fac_mtdblock4.tar file size [%ld]\n" , size , nUpdatalength);
		if(nUpdatalength>1024&&(size==nUpdatalength))
		{
			sync();
			system("killall -9 ipcam");
			system("killall -9 av_server.out");
			sync();
			system("/usr/sbin/flash_eraseall /dev/mtd4 ");
			/*
			//首先关闭所有运行的主程序
			system("killall -9 ipcam");
			//将升级文件更新对于的文件路径区域
			system("/bin/tar -xf /sys_ext/sys_lib_mtdblock4.tar -C / ");
			//同步
			sync();
			//删除临时存储文件
			system("rm -f /sys_ext/sys_lib_mtdblock4.tar");
			sync();
			*/

		}
		else
		{
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			//删除临时存储文件
			system("rm -f /sys_ext/sys_fac_mtdblock4.tar");
			sync();
		}
	}
	
	
	return flash_err;
	
}





/*************************************************
Function: Read_Flash_Data(int nReadsize , char *buffer , char *dev)
Description: 读取 FLASH文件写入数据。
Input: int nReadsize 为读取文件大小, char *buffer 读取文件数据 ，
char *dev 读取设备 ,char *savename 保存文件名
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 按照远程调试设计
author: 刘纹高 2012-03-07
*************************************************/
int Read_Flash_Data(int nReadsize , char *buffer , char *dev , char *savename)
{

	
	int read_flashfs=-1;
	int  flash_err ;	
	FILE *fp=NULL;
	int fpwrlen;
	//默认读取FLASH正确
	flash_err=FLASH_UPGRADE_OK;
	sync();
	if ((read_flashfs=open(dev,  O_RDONLY))<0)
	{
		printf("O_RDONLY open FLASH Block [%s] fail\n", dev);
		//打开FLASH设备失败
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("O_RDONLY Open Success [%s] read_flashfs [%d]\n" , dev , read_flashfs);
	}
	
	//如果buffer为空,则保持读取数据到文件中
	if (buffer==NULL&&savename!=NULL)
	{
		fp=fopen(savename,"wb");
		if (fp==NULL)
		{
			printf("create file fail  \n");
			goto ErrDispose;
		}
	}

	int ReadFlashOffset=0 ;
	int ReadBlock=64*1024;
	int ReadnFileLength = nReadsize;
	unsigned char readbuffer[64*1024];
	while(ReadnFileLength>0)
	{	
		if(ReadnFileLength<ReadBlock)
		{
			ReadBlock=ReadnFileLength;
			ReadnFileLength=0;
		}
		else
		{
			ReadnFileLength-=ReadBlock;
		}
		memset(readbuffer , 0 , sizeof(readbuffer));
		//读取Flash文件块
		if(read(read_flashfs , readbuffer , ReadBlock)!=ReadBlock)
		{
			printf("read error ReadBlock [%d]\n" , ReadBlock);
			flash_err=FLASH_UPGRADE_READ_DATA_FAIL;
			goto ErrDispose;
		}		
		sync();

		//如果buffer不为空，将读取Flash数据写入buffer中
		if (buffer!=NULL)
		{
			memcpy(buffer+ReadFlashOffset , readbuffer , ReadBlock);
		}
		else
		{
			//将读取Flash数据写入文件中
			if (fp!=NULL)
			{
				fpwrlen=fwrite(readbuffer,ReadBlock,1,fp);//把字符串内容写入到文件 
				if (fpwrlen!=1)
				{
					printf("fwrite error fpwrlen [%d]\n" , fpwrlen);
				}
			}
		}		
		ReadFlashOffset+=ReadBlock;
		printf("read ok ReadBlock [%d] ReadFlashOffset [%d]\n" , ReadBlock , ReadFlashOffset);
	}
		
ErrDispose:
		
	sync();
	if (read_flashfs>0)
	{
		close(read_flashfs);
		read_flashfs=-1;
	}
	if (fp!=NULL)
	{
		fclose(fp);
	}
	return flash_err;
	
}






