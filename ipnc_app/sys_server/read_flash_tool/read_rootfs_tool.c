/*************************************************
Copyright (C), 2001-2012,  Co., Ltd.
File name: // 文件名
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




int main()
{
	int err_msg;
	
	printf("******************************************\n");
	printf("*********LiuWenGao 2013-01-23-Write**********\n");
	printf("Read Flash DM368 /dev/mtdblock Create\n");
	printf("******************************************\n");

#if 0
	/*********读取Bootleader区域**********/
	system("rm -r Bootleader_mtdblock0.fs");
	err_msg=Read_Flash_Data(0x2FFFFF , NULL , "/dev/mtd0", "Bootleader_mtdblock0.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock0 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock0 success! \n");
	}


	/*********读取Params区域**********/
	system("rm -r Params_mtdblock1.fs");
	err_msg=Read_Flash_Data(0x1FFFFF , NULL , "/dev/mtd1", "Params_mtdblock1.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock1 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock1 success! \n");
	}



	/*********读取Kernel区域**********/
	system("rm -r Kernel_mtdblock2.fs");
	err_msg=Read_Flash_Data(0x1FFFFF , NULL , "/dev/mtd2", "Kernel_mtdblock2.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock2 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock2 success! \n");
	}


	/*********读取filesystem区域**********/
	system("rm -r filesystem_mtdblock3.fs");
	err_msg=Read_Flash_Data(0x17FFFFF , NULL , "/dev/mtd3", "filesystem_mtdblock3.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtd3 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtd3 success! \n");
	}


	/*********读取factory_info区域**********/
	system("rm -r factory_info_mtdblock4.fs");
	err_msg=Read_Flash_Data(0x1FFFFF , NULL , "/dev/mtd4", "factory_info_mtdblock4.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock4 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock4 success! \n");
	}
	/*********读取sys_app区域**********/
	system("rm -r sys_app_mtdblock5.fs");
	err_msg=Read_Flash_Data(0xFFFFFF , NULL , "/dev/mtd5", "sys_app_mtdblock5.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock5 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock5 success! \n");
	}

	/*********读取sys_dev区域**********/
	system("rm -r sys_dev_mtdblock6.fs");
	err_msg=Read_Flash_Data(0xFFFFFF , NULL , "/dev/mtd6", "sys_dev_mtdblock6.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock6 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock6 success! \n");
	}


	/*********读取sys_lib区域**********/
	system("rm -r sys_lib_mtdblock7.fs");
	err_msg=Read_Flash_Data(0xFFFFFF , NULL , "/dev/mtd7", "sys_lib_mtdblock7.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock7 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock7 success! \n");
	}

#endif

	//首先关闭所有运行的主程序
	system("killall -9 ipcam");
	system("killall -9 av_server.out");
	sync();

	//DM368高清IPC，升级文件读取
	//2013年1月23日，刘纹高编写
	unsigned long nUpdataLen;
	float fLen;


	/*********读取三个区域统一升级包**********/
	system("rm -r sys_all_three_mtdblock.fs");
	//制作三个区域统一升级包升级文件，必须要求进行挂着，只针对曾庆魏设定目录
	printf("----------------------------------------\n");
	system("/bin/tar -cf sys_all_three_mtdblock.fs /sys_app /opt /sys_lib /tmp ");
	sync();
	nUpdataLen =0;
	fLen =0;
	nUpdataLen = get_file_size("sys_all_three_mtdblock.fs");
	fLen = (float)((float)(nUpdataLen/1024)/(float)1024);
	printf("sys_all_three_mtdblock.fs Length [%f M]\n" , fLen);
	printf("----------------------------------------\n");


	/*********读取sys_fac区域,出厂设置区域**********/
	system("rm -r sys_fac_mtdblock4.fs");
	//制作APP区域升级文件，必须要求进行挂着，只针对曾庆魏设定目录
	printf("----------------------------------------\n");
	system("/bin/tar -cf sys_fac_mtdblock4.fs /sys_fac ");
	sync();
	nUpdataLen =0;
	fLen =0;
	nUpdataLen = get_file_size("sys_fac_mtdblock4.fs");
	fLen = (float)((float)(nUpdataLen/1024)/(float)1024);
	printf("sys_fac_mtdblock4.fs Length [%f M]\n" , fLen);
	printf("----------------------------------------\n");


#if 0
	/*********读取sys_app区域**********/
	system("rm -r sys_app_mtdblock5.fs");
	//制作APP区域升级文件，必须要求进行挂着，只针对曾庆魏设定目录
	printf("----------------------------------------\n");
	system("/bin/tar -cf sys_app_mtdblock5.fs /sys_app ");
	sync();
	nUpdataLen =0;
	fLen =0;
	nUpdataLen = get_file_size("sys_app_mtdblock5.fs");
	fLen = (float)((float)(nUpdataLen/1024)/(float)1024);
	printf("sys_app_mtdblock5.fs Length [%f M]\n" , fLen);
	printf("----------------------------------------\n");





	/*********读取sys_dev区域**********/
	system("rm -r sys_dev_mtdblock6.fs");
	//制作DEV区域升级文件，必须要求进行挂着，只针对曾庆魏设定目录
	printf("----------------------------------------\n");
	system("/bin/tar -cf sys_dev_mtdblock6.fs /opt ");
	sync();
	nUpdataLen =0;
	fLen =0;
	nUpdataLen = get_file_size("sys_dev_mtdblock6.fs");
	fLen = (float)((float)(nUpdataLen/1024)/(float)1024);
	printf("sys_dev_mtdblock6.fs Length [%f M]\n" , fLen);
	printf("----------------------------------------\n");


	/*********读取sys_lib区域**********/
	system("rm -r sys_lib_mtdblock7.fs");
	//制作LIB区域升级文件，必须要求进行挂着，只针对曾庆魏设定目录
	printf("----------------------------------------\n");
	system("/bin/tar -cf sys_lib_mtdblock7.fs /sys_lib ");
	nUpdataLen =0;
	fLen =0;
	nUpdataLen = get_file_size("sys_lib_mtdblock7.fs");
	fLen = (float)((float)(nUpdataLen/1024)/(float)1024);
	printf("sys_lib_mtdblock7.fs Length [%f M]\n" , fLen);
	printf("----------------------------------------\n");

#endif

	sync();

	printf("********************complete**********************\n");

	return 0;
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
	int i , nCheckValue ;
	unsigned long nBlockValue;
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

		nBlockValue = 0;
		for (i = 0 ; i<ReadBlock ; i++)
		{
			nCheckValue = 0;
			nCheckValue = 0xFF - readbuffer[i];
			nBlockValue += nCheckValue;
		}

		printf("read nBlockValue [%ld]\n" , nBlockValue);
		if (nBlockValue == 0)
		{
			
			printf("read ok 0xFF black\n");
			//goto ErrDispose;
		}

		printf("read ok Count [%d] ReadBlock [%d] ReadFlashOffset [%d]\n" , nReadsize ,ReadBlock , ReadFlashOffset);
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






