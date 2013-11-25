/*************************************************
Copyright (C), 2001-2012, TMVIDEO. Co., Ltd.
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
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <linux/route.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>

#ifndef _TM_UPGRADE_H__
#define _TM_UPGRADE_H__



/***********Kernel Flash分区空间文件*********************/
#define MTD_KERNEL_UPDATE_DEV				"/dev/mtdblock1"

/***********Rootfs Flash分区空间文件*********************/
#define MTD_ROOTFS_UPDATE_DEV				"/dev/mtdblock2"

/***********应用程序 Flash分区空间文件*********************/
#define MTD_APP_UPDATE_DEV				"/dev/mtdblock3"

/***********参数区 Flash分区空间文件*********************/
#define MTD_PARAMETER_UPDATE_DEV				"/dev/mtdblock4"



/******
是否打开"写"FLASH功能，正常情况下必须打开，
该定义主要用于升级模块功能调试。
********/
#define D_WRITE_FLASH_TASK			1

/******
是否打开"读"FLASH功能，正常情况下必须关闭，
该定义主要用于升级模块功能调试。
******/
#define D_READ_FLASH_TEST			0

/***
如果打开读，读取FLASH保存的文件名定义，
用于调试
***/
#define D_READ_FLASH_SAVE_FILENAME	"read_rootfs5.jffs2"
/***
如果打开读，读取FLASH保存的文件的大小，
用于调试
***/
#define D_READ_FLASH_FILE_SZIE      0x300000
/***
升级测试文件名
用于调试
***/
#define UPDATE_FILE_NAME				"read_rootfs1_ok.jffs2"

/***
升级TCP\UDP端口，固定为60001
***/
#define UPDATE_TCP_UDP_LISTEN_PORT			60001


#define		SOCK_SNDRCV_LEN		(1024*32)

#define     SOCK_TIME_OVER      10


#ifndef DWORD
#define DWORD unsigned long
#endif

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef LPVOID
#define LPVOID void *
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef True
#define True	1
#endif

#ifndef TRUE
#define TRUE	1
#endif


#ifndef False
#define False	0
#endif

#ifndef FALSE
#define FALSE	0
#endif



/***
升级数据检查
***/
typedef enum _file_upgrade_datacheck_enum
{
/***升级文件包头效验情况***/
		FILE_UPGRADE_DATACHECK_OK=0	,			//数据正确
		FILE_UPGRADE_DATACHECK_BUFFER_ERR,		//buffer为NULL错误
		FILE_UPGRADE_DATACHECK_FLAG_ERR,		//FLAG错误
		FILE_UPGRADE_DATACHECK_IMAGE_TYPE_ERR,	//升级类型错误
		FILE_UPGRADE_DATACHECK_CPU_TYPE_ERR ,	//CPU类型错误
		FILE_UPGRADE_DATACHECK_VIDEOCOUNT_ERR ,	//视频通道错误
		FILE_UPGRADE_DATACHECK_VERSION_ERR ,	//版本错误
		FILE_UPGRADE_DATACHECK_DATALEN_ERR ,	//数据实际长度错误
		FILE_UPGRADE_DATACHECK_CRC_ERR ,		//CRC数据效验错误
/***升级文件读写状态情况***/		
		FILE_UPGRADE_OPEN_FAILURE ,				//打开文件失败
		FILE_UPGRADE_COUNTLENGHT_FAILURE ,		//打开文件总长度小于升级包头160个字节
		FILE_UPGRADE_MALLOC_MEMERY_FAILURE ,	//申请系统内存失败
		FILE_UPGRADE_USBDISK_DONTFIND ,			//在U盘中未找到升级文件
/***读写Flash状态情况***/	
		FLASH_UPGRADE_OK	,				//FLASH写入正常,升级成功
		FLASH_UPGRADE_OPEN_DEVICE_FAIL,		//打开FLASH设备失败
		FLASH_UPGRADE_WRITE_DATA_FAIL,		//写入数据到FLASH设备失败
		FLASH_UPGRADE_READ_DATA_FAIL,		//读取数据到FLASH设备失败
		FLASH_UPGRADE_DATA_SIZE_ERR	,		//发现写入数据大小错误
/***网络升级状态情况***/
		NETWORK_STATUS_CHECK_OK ,					//网络升级状态正常
		NETWORK_STATUS_CHECK_BLOCKE_ERROR ,			//网络升级检查分包错误
		NETWORK_STATUS_CHECK_MALLOC_MEMERY_FAILURE , //网络升级申请系统内存失败
		NETWORK_STATUS_RECV_ERROR                  //网络升级数据接收异常


} FILE_UPGRADE_DATACHECK_E;









#define upgrade_err()\
{             \
	printf(" %s(%d) - Line %d , %s \n", __FILE__, __LINE__, errno, strerror(errno));\
}

//升级包头160个字节
typedef struct tagTMUPDATAFILE_HEADER
{
	DWORD	dwFlag;			//1.升级标识头"tmup",
	DWORD	dwDataCRC;		//2.CRC头文件效验
	DWORD   dwCPUType;	    //3.服务器CPU类型，海思HI3515\HI3520\HI3530\HI3531
	DWORD	dwImageType;    //4.Image类型，升级内核、或者rootfs文件系统
	DWORD	dwChCount;	    //5.服务器视频通道数量	
	DWORD	dwServerType;   //6.服务器类型
	DWORD	dwMainVer;		//7.升级文件主版本号,1-8
	DWORD	dwChildVer;		//8.升级文件子版本号,1-10
	DWORD	dwModityVer;	//9.升级文件修改标识版本号,填写手动填写，1-99
	DWORD	dwRawWriteFileLen;   //10.升级写入文件长度
	char	ReserveBuf[120];  //作为附加中文信息，预留字节，为了方便扩展
	
}TMUPDATAFILE_HEADER,*PTMUPDATAFILE_HEADER;


//服务器结构
typedef struct tagTMUPDATANET_SERVER_INFO
{
	int					hTcpListenSock;		//TCP SOCKET
	int					hUdpPortSock;		//UDP SOCKET
	struct sockaddr_in	Udpaddr;
	
	
}TMUPDATANET_SERVER_INFO,*PTMUPDATANET_SERVER_INFO;






//车载SDK网络消息结构,徐志强定义2012年3月8日
#define		NET_FLAG 0x64766d74

#define		BLOCK_NETWORK_UPDATA_BLOCKSIZE	1024  //按照满足3G,MTU值设计，加上16字节头，满包总数据量为1040个字节

#define		BLOCK_NETWORK_UPDATA_COUNTBLOCKSIZE	1040  //按照满足3G,MTU值设计，加上16字节头，满包总数据量为1040个字节		

#define		NETCMD_USBUPDATA_START		0x00A0		//本机UI-->升级服务器，
													//命令其进行USB升级，U盘路径为/mnt/usb_disk

#define		NETCMD_USBUPDATA_RETURN		0x00A1		//升级服务器-->本机UI，
													//回复UI发出升级命令，开始进行升级

#define		NETCMD_USBUPDATA_FAILURE	0x00A2		//升级服务器-->本机UI，
													//回复UI发出升级命令，升级失败

#define		NETCMD_USBUPDATA_SUCCESS	0x00A3		//升级服务器-->本机UI，
													//回复UI发出升级命令，升级成功


#define		NETCMD_NETWORK_UPDATA_START	0x00B0	//网络-->升级服务器，
												//命令其进行网络升级升级，并准备接收升级数据包
												//填写dwReserved为分包总数
												//填写dwLength为升级包总数据长度

#define		NETCMD_NETWORK_UPDATA_RETURN	0x00B1	//升级服务器-->网络
													//回复升级服务器发出升级命令，开始接收升级数据包
													//dwReserved填写为分包数量
													//dwLength填写为升级包数据大小

#define		NETCMD_NETWORK_UPDATA_PACKET	0x00B2	//网络-->升级服务器
													//回复升级服务器发出升级命令，开始接收升级数据包
													//填写dwReserved为分包序号，从0开始计算
													//填写dwLength为升级分包数据长度

#define		NETCMD_NETWORK_UPDATA_MESSAGE	0x00B3	//升级服务器-->网络
													//回复升级服务器发出升级命令，开始接收升级数据包
													//dwReserved填写为信息
													//

#define		NETCMD_NETWORK_UPDATA_PROGRESS	0x00B4	//升级服务器-->网络
													//回复升级服务器发出升级命令，开始接收升级数据包
													//填写dwReserved为0-100写入进度,100代表完成写FLASH

typedef struct tagNET_MESSAGE
{
	DWORD dwNetFlag;
	DWORD dwCMD;
	DWORD dwReserved;
	DWORD dwLength;
	char  lpData[];
}NET_MESSAGE,*LPNET_MESSAGE;


//私用部分
#define UPDATE_TCP_BUFFER_MESSAGE_NUMBER	10


typedef struct tagTCP_SEND_NET_MESSAGE
{
	BOOL bUse;
	int hConSock;
	NET_MESSAGE szMsg;
}TCP_SEND_NET_MESSAGE,*LTCP_SEND_NET_MESSAGE;


typedef struct tagNET_listen_ret_t
{
	struct sockaddr_in			client_addr;
	int					accept_sock;
	int					resverd;
}listen_ret_t, *plisten_ret_t;

/***********升级标识头"tmup"**************/
#define UPDATA_D_FLAG  0x746d7570

/***********服务器CPU类型**************/
#define UPDATA_D_HI3515  0x00001001
#define UPDATA_D_HI3520  0x00001002
#define UPDATA_D_HI3530  0x00001003
#define UPDATA_D_HI3531  0x00001004
#define UPDATA_D_CPUUNKOWN  0x00001010

/***********升级Image类型**************/
#define UPDATA_D_ROOTFS  0x00001101    //升级文件系统
#define UPDATA_D_KERNEL  0x00001102	   //升级内核系统
#define UPDATA_D_APPAREA  0x00001103	   //应用程序系统
#define UPDATA_D_IMAGEUNKOWN  0x00001110	   //未知文件

/***********服务器视频通道数量**************/
#define UPDATA_D_4CHANNEL  4      //4路
#define UPDATA_D_5CHANNEL  5	   //5路
#define UPDATA_D_6CHANNEL  6	   //6路
#define UPDATA_D_7CHANNEL  7	   //7路
#define UPDATA_D_8CHANNEL  8	   //8路
#define UPDATA_D_CHANNELUNKOWN  40	   //未知通道

/***********服务器类型**************/
#define UPDATA_D_BASETYPE  0x00001301      //基础版本






/*************************************************
Function: Rootfs_UpdateFlash(int size, char *buffer)
Description: 打开FLASH文件写入数据。
Input: int size 为升级文件大小, char *buffer 升级文件数据
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 按照12M Rootfs空间设计
author: 刘纹高 2012-03-07
*************************************************/

int Rootfs_UpdateFlash(int size, char *buffer);



/*************************************************
Function: Kernel_UpdateFlash(int size, char *buffer)
Description: 打开Kernel 分区 FLASH文件写入数据。
Input: int size 为升级文件大小, char *buffer 升级文件数据
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 按照2M Kernel空间设计
author: 刘纹高 2012-03-07
*************************************************/
int Kernel_UpdateFlash(int size, char *buffer);


/*************************************************
Function: App_UpdateFlash(int size, char *buffer)
Description: 打开Kernel 分区 FLASH文件写入数据。
Input: int size 为升级文件大小, char *buffer 升级文件数据
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 按照2M Kernel空间设计
author: 刘纹高 2012-03-07
*************************************************/
int App_UpdateFlash(int size, char *buffer);


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
int Read_Flash_Data(int nReadsize , char *buffer , char *dev , char *savename);

/*************************************************
Function: mount_mtdblock4_Parameter()
Description: 挂载应用程序、参数区
Input: 
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 
author: 刘纹高 2012-03-07
*************************************************/
//调用函数挂载函数，必须在读取参数文件直接调用
void mount_mtdblock_Parameter();

/*************************************************
Function: Check_Upgrade_Data_Whether_Correct(TMUPDATAFILE_HEADER *updatakitheader , 
unsigned char *UpgradeBuf ,int  Upgradelen )   
Description: 打开选择升级文件，进行效验
Input: 消息信息
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: // 其它说明
*************************************************/
int Check_Upgrade_Data_Whether_Correct(TMUPDATAFILE_HEADER *updatakitheader , \
									   unsigned char *UpgradeBuf ,	\
									   int  Upgradelen);



/*************************************************
Function: int USBDisk_Auto_UpdateFlash()
Description: carDVR主控界面程序，发送自动升级消息，自动寻找升级文件进行升级
Input: char *UpgradePath 升级文件完整路径 /mnt/usb_disk/*.hibom.auto
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: 
author: 刘纹高 2012-03-07
*************************************************/
int USBDisk_Auto_UpdateFlash();


/*************************************************
Function: SendWriteFlashMsg(DWORD nWirteLen , DWORD nCorrentLen)
Description: 发送写FLASH消息进度,
DWORD nWirteLen 写FLASH总数 DWORD nCorrentLen 当前写入数据量
Input: 消息信息
Output: 带升级信息文件
Return: // 函数返回值的说明
Others: // 其它说明
*************************************************/
void SendWriteFlashMsg(DWORD nWirteLen , DWORD nCorrentLen);


#endif



