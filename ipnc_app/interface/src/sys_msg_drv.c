/* ===========================================================================
* @file sys_msg_drv.c
*
* @path $(IPNCPATH)\util
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
#include "sys_msg_drv.h"
#include "share_mem.h"
#include "sem_util.h"

/* Functions to communicate with system server */
static int gSysMsgId,gSysProcId;
static SemHandl_t hndlDrvSem = NULL;

#define SYS_MSG_ERR(fmt, args...) fprintf(stderr, "Error:" fmt, ## args)

/**
* @brief Save process ID into global variable
* @param iProcId [I ] process ID
*/
void SetSysProcId(int iProcId)
{
	gSysProcId = iProcId;
}

/**
* @brief Save message ID into global variable
* @param qid [I ] message ID
*/
void SaveSysMsgId(int qid)
{
	gSysMsgId = qid;
}
/**
* @brief System message driver initialize.
* @param proc_id [I ] Message ID in \ref Sys_Msg_Def.h.
* @retval 0 Success
* @retval -1 Fail
*/
int SysDrvInit(int proc_id)
{
	if(proc_id < MSG_TYPE_MSG1 || proc_id >= MSG_TYPE_END){
		gSysProcId = MSG_TYPE_MSG1;
		SYS_MSG_ERR("Invalid Parameter\n");
		return -1;
	}
	gSysProcId = proc_id;
	if(ShareMemInit(SYS_MSG_KEY) < 0){
		gSysProcId = MSG_TYPE_MSG1;
		SYS_MSG_ERR("Share Memory init fail\n");
		return -1;
	}
	if(hndlDrvSem == NULL)
		hndlDrvSem = MakeSem();
	if(hndlDrvSem == NULL){
		gSysProcId = MSG_TYPE_MSG1;
		SYS_MSG_ERR("No more memory to create semaphore\n");
		return -1;
	}
	if((gSysMsgId=Msg_Init(SYS_MSG_KEY)) < 0){
		DestroySem(hndlDrvSem);
		hndlDrvSem = NULL;
		gSysProcId = MSG_TYPE_MSG1;
		SYS_MSG_ERR("Msg_Init fail\n");
		return -1;
	}
	return 0;
}
/**
* @brief System message driver exit.

* This API shoud be called at the end of process.
* @retval 0 Success.
*/
int SysDrvExit()
{
	gSysProcId = MSG_TYPE_MSG1;
	DestroySem(hndlDrvSem);
	hndlDrvSem = NULL;
	return 0;
}

/**
* @brief get dhcp status.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetDhcpEn(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_DHCP_EN;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0){
		ShareMemRead(msgbuf.offset, data, length);
	}
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get http port number.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetHttpPort(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_HTTP_PORT;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get SNTP server FQDN.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSntpFqdn(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SNTP_FQDN;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get device type.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetDeviceType(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_DEVICE_TYPE;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get camera title.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetTitle(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_TITLE;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get camera title.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetRegUsr(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_REGUSR;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get camera title.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetOSDText(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_OSDTEXT;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get hardware MAC address.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetMAC(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_MAC;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get IPCAM's IP address.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetIP(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_IP;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get gateway address.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetGateway(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_GATEWAY;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get netmask setting.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetNetmask(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_NETMASK;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get FTP server IP address.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetFtpServerIp(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_FTP_SERVER_IP;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get FTP login username.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetFtpUsername(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_FTP_USERNAME;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get FTP login password.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetFtpPassword(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_FTP_PASSWORD;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get FTP folder name.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetFtpFoldername(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_FTP_FOLDNAME;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get FTP image amount.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetFtpImageAmount(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_FTP_IMAGEAMOUNT;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get FTP PID.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetFtpPid(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_FTP_PID;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get FTP port.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSysFtpPort(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_FTP_PORT;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get SMTP server IP address.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSmtpServerIp(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SMTP_SERVER_IP;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get SMTP server Port.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSmtpServerPort(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SMTP_SERVER_PORT;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get SMTP authentication.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSmtpAuthentication(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SMTP_AUTHENTICATION;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get SMTP username.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetsmtpUsername(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SMTP_USERNAME;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get SMTP password.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSmtpPassword(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SMTP_PASSWORD;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get SMTP sender email.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSmtpSenderEmail(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SMTP_SENDER_EMAIL_ADDRESS;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get SMTP receiver email.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSmtpReceiverEmail(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SMTP_RECEIVER_EMAIL_ADDRESS;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get current recording file name in SD card.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @return String lengh (not include '\0').
* @retval -1 Fail.
*/
int SysGetSdFileName(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SD_FILENAME;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get SMTP CC email address.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSmtpCC(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SMTP_CC;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get SMTP email subject.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSmtpSubject(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SMTP_SUBJECT;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get SMTP email text.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSmtpText(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SMTP_TEXT;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length > msgbuf.length)
		length = msgbuf.length;
	if(length > 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return length;
}

/**
* @brief get SMTP email attachments.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSmtpAttachments(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SMTP_ATTACHMENTS;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief get SMTP view.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSmtpView(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_SMTP_VIEW;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/*int GetSysUpnpPort(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_UPNP_PORT;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}*/


/**
* @brief get DHCP config.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSysDhcpConfig(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_DHCP_CONFIG;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief set net mask.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetNetMask(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_NETMASK;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set IP address.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetIp(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_IP;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set HTTP port.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetHttpPort(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_HTTPPORT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set camera title.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetTitle(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TITLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set camera title.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetRegUsr(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_REGUSR;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set camera title.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetOSDText(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_OSDTEXT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set gateway address.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetGateway(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_GATEWAY;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set DHCP enable.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetDhcpEnable(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_DHCPC_ENABLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set FTP FQDN.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysFtpFqdn(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FTP_FQDN;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set FTP username.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetFtpUsername(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FTP_USERNAME;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set FTP password.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetFtpPassword(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FTP_PASSWORD;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set FTP folder name.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetFtpFoldname(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FTP_FOLDNAME;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set FTP folder image amount.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetFtpImageacount(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FTP_IMAGEAMOUNT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set FTP port.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysFtpPort(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FTP_PORT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set FTP PID.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetFtpPid(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FTP_PID;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set SMTP server IP address.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSmtpServerIp(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SMTP_SERVER_IP;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
/**
* @brief set SMTP server Port.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSmtpServerPort(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SMTP_SERVER_PORT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief schedule stop

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SysScheduleStop(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SCHEDULE_STOP;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
/**
* @brief schedule resume

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SysScheduleResume(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SCHEDULE_RESUME;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set SMTP server authentication.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSmtpAuthentication(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SMTP_AUTHENTICATION;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set SMTP username.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSmtpUsername(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SMTP_USERNAME;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set SMTP password.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSmtpPassword(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SMTP_PASSWORD;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set SMTP sender address.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSmtpSenderEmail(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SMTP_SENDER_EMAIL_ADDRESS;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set SMTP receiver address.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSmtpReceiverEmail(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SMTP_RECEIVER_EMAIL_ADDRESS;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set SMTP C.C. address.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSmtpCC(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SMTP_CC;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set E-mail subject.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSmtpSubject(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SMTP_SUBJECT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set E-mail text.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSmtpText(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SMTP_TEXT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set SMTP email attachments.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSmtpAttachments(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SMTP_ATTACHMENTS;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set SMTP view.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSmtpView(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SMTP_VIEW;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set time format.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysTimeFormat(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TIMEFORMAT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set dyalight saving time.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysDaylight(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_DAYLIGHT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set time zone.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysTimeZone(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TIMEZONE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief send Quit command to system server.

*/
void SendSysQuitCmd()
{
	SYS_MSG_BUF msgbuf;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = 0;
	msgbuf.cmd = SYS_MSG_QUIT;
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
}

/**
* @brief get DNS.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetDns(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int length,ret = 0;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_DNS;
	SemWait(hndlDrvSem);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	length = len;
	if(length != msgbuf.length)
		ret = -1;
	if(ret == 0)
		ShareMemRead(msgbuf.offset, data, length);
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief set DNS.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetDns(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_DNS;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set SNTP server.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSntpServer(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SNTP_SERVER_IP;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set sensor Day/Night mode.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysDayNight(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_DAY_NIGHT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set Auto White Balance mode.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysWhiteBalance(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_WHITE_BALANCE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set Backlight value.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysBacklight(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_BACKLIGHT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set Brightness value.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysBrightness(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_BRIGHTNESS;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set Contrast value.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysContrast(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_CONTRAST;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set Saturation value.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysSaturation(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SATURATION;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set Sharpness value.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysSharpness(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SHARPNESS;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief clear system log.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysClearSysLog(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_CLEARSYSLOG;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief clear access log

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysClearAccessLog(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_CLEARACCESSLOG;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}


/**
* @brief set Jpeg Quality.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysJpegQuality(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_JPEG_QUALITY;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysMirror(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MIRROR;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysfdetect(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FDETECT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysfdx(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FDX;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysfdy(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FDY;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysfdw(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FDW;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysfdh(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FDH;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysfdconf(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FDCONF_LEVEL;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysfddir(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FD_DIRECTION;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysfrecog(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FRECOGNITION;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysfrconf(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FRCONF_LEVEL;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysfrdbase(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FR_DATABASE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSyspmask(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_PRIVACY_MASK;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSyspmaskopt(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MASK_OPTIONS;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/* NEW FEATURE */
int SetSysAdvMode(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_ADVMODE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysDemoCfg(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_DEMOCFG;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysOSDWin(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_OSDWIN;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysHistogram(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_HISTOGRAM;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysDynRange(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_DYNRANGE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysOSDWinNum(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_OSDWINNUM;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysOSDStream(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_OSDSTREAM;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysVideoMode(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_VIDEO_MODE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set Video Codec Combo selection.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysVideoCodecCombo(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_VIDEOCODEC_COMBO;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set Video Codec Resolution selection.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysVideoCodecRes(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_VIDEOCODEC_RES;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief NTSC/PAL Select.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysImageSource(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_IMAGE_SOURCE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysIMGFormat(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_IMAGE_FORMAT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysResolution(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_RESOLUTION;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysMPEG4Res(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MPEG4_RES;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysMPEG42Res(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MPEG42_RES;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set recording schedule.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysSchedule(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SCHEDULE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief delete recording schedule.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int DelSysSchedule(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_DEL_SCHEDULE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set lost ethernet alarm.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysLostAlarm(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_LOSTALARM;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set alarm enable - upload via SD card.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysSDAlarmEnable(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SDAENABLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set alarm enable - upload via ftp.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysFtpAlarmEnable(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FTPAENABLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set alarm enable - upload via ftp.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysSmtpAlarmEnable(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SMTPAENABLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set alarm duration.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysAlarmDuration(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_ALARMDURATION;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set avi duration.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetAVIDuration(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_AVIDURATION;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
/**
* @brief set avi format.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetAVIFormat(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_AVIFORMAT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set file format to save via FTP.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetFTPFileFormat(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FTPFILEFORMAT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set file format to save via SD.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSDFileFormat(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SDFILEFORMAT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set file format to save via SMTP.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetAttFileFormat(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_ATTFILEFORMAT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set audio status.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetAudioEnable(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_AUDIOENABLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set smtp attatched file numbers.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetASmtpAttach(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_ASMTPATTACH;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief enable record - upload via ftp.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetRftpEnable(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FTP_RFTPENABLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief enable record - save into SD card.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSdReEnable(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_SD_REENABLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief sitch 2A engine.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysImage2A(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_2A_SWITCH;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief sitch 2A engine.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysImage2AType(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_2A_TYPE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set Binning mode.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysBinning(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_BINNING;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief back light control on/off.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysBLC(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_BLC;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set motion detection on or off.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetMotionEnable(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MOTIONENABLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set motion c enable.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetMotionCEnable(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MOTIONCENABLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set motion detection sensitivity by select.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetMotionLevel(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MOTIONLEVEL;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief motion detection sensitivity value.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetMotionCValue(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MOTIONCVALUE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set blocks selected to do motion detection.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetMotionBlock(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MOTIONBLOCK;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set bitrate of MPEG4 stream1.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetMPEG41Bitrate(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MPEG41_BITRATE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set bitrate of MPEG4 stream2.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetMPEG42Bitrate(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MPEG42_BITRATE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set image settings to default.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetImageDefault(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_IMAGE_DEFAULT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief web interface login check.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int DoSysLogin(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int ret = -1, length = len;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_DO_LOGIN;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	if(msgbuf.length == length)
		ret = 0;
	return ret;
}

/**
* @brief Add user account.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SysAddUser(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_ADD_USER;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief Delete user account.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SysDelUser(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_DEL_USER;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief Set GIO-in enable.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetGIOInEnable(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_GIOINENABLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief PTZ control data input.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetIpncPtz(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_IPNCPTZ;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief PTZ control data input.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int InitIpncPtz(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_INIT_IPNCPTZ;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}


/**
* @brief SD card formatting command.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SysSdFormat(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SD_FORMAT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief Send command to unmount sd card.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SysSdUnmount(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SD_UNMOUNT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set GIO-in type.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetGIOInType(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_GIOINTYPE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief Set GIO-out enable.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetGIOOutEnable(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_GIOOUTENABLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set GIO-out type.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetGIOOutType(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_GIOOUTTYPE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set timestamp on/off.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetTStampEnable(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TSTAMPENABLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set timestamp format.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetTStampFormat(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TSTAMPFORMAT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set audio volume.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetAudioinVolume(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_AUDIOINVOLUME;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief get user Authority.

* @param data [ O] pointer to data  buffer.
* @param len [I ] data buffer length.
* @retval 0 Success.
* @retval -1 Fail.
*/
int GetSysUserAuthority(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	int ret = -1;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_GET_USER_AUTHORITY;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	if(msgbuf.length == len){
		ShareMemRead(msgbuf.offset, data, msgbuf.length);
		ret = 0;
	}
	SemRelease(hndlDrvSem);
	return ret;
}

/**
* @brief set bitrate control.(OFF/VBR/CBR, selected from webpage)

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysRateControl(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_RATE_CONTROL;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set Mpeg4 stream1 frame rate.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysFramerate1(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FRAMERATE1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set Mpeg4 stream2 frame rate.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysFramerate2(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FRAMERATE2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief set Jpeg frame rate.

* @param data [I ] pointer to input data.
* @param len [I ] data length.
* @return message buffer length.
*/
int SetSysFramerate3(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FRAMERATE3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysCommon(void * data, unsigned int len, int cmd)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = cmd;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysMulticast(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MULTICAST;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

/**
* @brief Do boot control.

* @param data [I ] pointer to input data.
* @param len [I ] data length. (should be sizeof(int). )
* @return message buffer length. (should be sizeof(int). )
*/
int SysDoBootProc(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_DO_BOOT_PROC;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysRateControl1(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_RATE_CONTROL1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysRateControl2(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_RATE_CONTROL2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysDateStampEnable1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_DATE_STAMP_ENABLE1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysDateStampEnable2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_DATE_STAMP_ENABLE2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysDateStampEnable3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_DATE_STAMP_ENABLE3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysTimeStampEnable1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TIME_STAMP_ENABLE1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysTimeStampEnable2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TIME_STAMP_ENABLE2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysTimeStampEnable3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TIME_STAMP_ENABLE3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysLogoEnable1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_LOGO_ENABLE1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysLogoEnable2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_LOGO_ENABLE2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysLogoEnable3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_LOGO_ENABLE3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysLogoPosition1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_LOGO_POSITION1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysLogoPosition2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_LOGO_POSITION2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysLogoPosition3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_LOGO_POSITION3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysTextEnable1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TEXT_ENABLE1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysTextEnable2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TEXT_ENABLE2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysTextEnable3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TEXT_ENABLE3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysTextPosition1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TEXT_POSITION1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysTextPosition2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TEXT_POSITION2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysTextPosition3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_TEXT_POSITION3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysEncryptVideo(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_ENCRYPT_VIDEO;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysLocalDisplay(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_LOCAL_DISPLAY;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysOverlayText1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_OVERLAY_TEXT1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysOverlayText2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_OVERLAY_TEXT2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysOverlayText3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_OVERLAY_TEXT3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysIpratio1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_IPRATIO1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysIpratio2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_IPRATIO2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysIpratio3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_IPRATIO3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysForceIframe1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FORCE_IFRAME1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysForceIframe2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FORCE_IFRAME2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysForceIframe3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FORCE_IFRAME3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysQPInit1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_QPINIT1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysQPInit2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_QPINIT2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysQPInit3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_QPINIT3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysQPMin1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_QPMIN1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysQPMin2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_QPMIN2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysQPMin3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_QPMIN3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysQPMax1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_QPMAX1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysQPMax2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_QPMAX2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysQPMax3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_QPMAX3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysMEConfig1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MECONFIG1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysMEConfig2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MECONFIG2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysMEConfig3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_MECONFIG3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysPacketSize1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_PACKET_SIZE1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysPacketSize2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_PACKET_SIZE2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysPacketSize3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_PACKET_SIZE3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysROIEnable1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_ROI_ENABLE1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysROIEnable2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_ROI_ENABLE2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysROIEnable3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_ROI_ENABLE3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr1X1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR1X1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr1Y1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR1Y1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr1W1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR1W1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr1H1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR1H1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr1X2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR1X2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr1Y2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR1Y2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr1W2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR1W2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr1H2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR1H2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr1X3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR1X3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr1Y3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR1Y3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr1W3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR1W3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr1H3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR1H3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr2X1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR2X1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr2Y1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR2Y1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr2W1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR2W1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr2H1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR2H1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr2X2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR2X2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr2Y2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR2Y2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr2W2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR2W2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr2H2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR2H2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr2X3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR2X3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr2Y3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR2Y3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr2W3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR2W3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr2H3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR2H3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr3X1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR3X1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr3Y1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR3Y1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr3W1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR3W1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr3H1(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR3H1;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr3X2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR3X2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr3Y2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR3Y2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr3W2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR3W2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr3H2(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR3H2;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr3X3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR3X3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr3Y3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR3Y3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
int SetSysStr3W3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR3W3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysStr3H3(void * data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_STR3H3;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysClickSnapFilename(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_CLICK_SNAP_FILENAME;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysClickSnapStorage(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_CLICK_SNAP_STORAGE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysAudioReceiverEnable(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_AUDIORECEIVER_ENABLE;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysAudioServerIp(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_AUDIOSERVER_IP;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysLinearWdr(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_LINEAR_WDR;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}

int SetSysFlickerDetect(void *data, unsigned int len)
{
	SYS_MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gSysProcId;
	msgbuf.cmd = SYS_MSG_SET_FLICKER_DETECT;
	msgbuf.length = len;
	msgbuf.offset = (gSysProcId - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
	SemWait(hndlDrvSem);
	ShareMemWrite(msgbuf.offset, data, len);
	msgsnd( gSysMsgId,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( gSysMsgId, &msgbuf,sizeof(msgbuf)-sizeof(long), gSysProcId, 0);
	SemRelease(hndlDrvSem);
	return msgbuf.length;
}
