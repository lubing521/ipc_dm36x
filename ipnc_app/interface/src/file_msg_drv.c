/* ===========================================================================
* @file file_msg_drv.c
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
#include <share_mem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sem_util.h>
#include <file_msg_drv.h>

#ifdef DEBUG
#define DBG(fmt, args...)	printf("FileMsgDrv: Debug\n" fmt, ##args)
#else
#define DBG(fmt, args...)
#endif

#define ERR(fmt, args...)	printf("FileMsgDrv: Error\n" fmt, ##args)

#define SWAP(a, b)  {a ^= b; b ^= a; a ^= b;}

static int qid,mid,gProcId, eCount = 0, gKey;
static void *pShareMem;
static SemHandl_t hndlDrvSem = NULL;
/**
* @brief Save message ID in file message driver

* Save a message ID to file message driver. This ID will be used to identify
*  who is communicating with file manager. This function is old method, please
*  use InitFileMsgDrv(int key,int iProcId) instead.
* @param iqid [I ] message ID
*/
void SaveQid(int iqid)
{
	qid = iqid;
}
/**
* @brief Send quit command to file manager.

* This command will make file manager stop running. After you called this, all
*  the other process can't get system information because file manager is down.
*/
void SendFileQuitCmd()
{
	FILE_MSG_BUF msgbuf;

	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = 0;
	msgbuf.cmd = FILE_MSG_QUIT;
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
}

#if 0
static int GetShareMemoryId()
{
	FILE_MSG_BUF msgbuf;

	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gProcId;
	msgbuf.cmd = FILE_GET_MID;
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
	msgrcv(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), gProcId, 0);
	return msgbuf.shmid;
}
#endif
/**
* @brief Initialize file message driver.

* Initialize file message driver. Please see \ref FILE_MSG_DRV_HOW to learn more.
* @note This API must be used before use any other file message driver API.
* @param key [I ] Key number for message queue and share memory.
* @param iProcId [I ] Message ID(Which was define in \ref File_Msg_Def.h) to initialize file message driver.
* @retval 0 Success.
* @retval -1 Fail.
*/
int InitFileMsgDrv(int key,int iProcId)
{
	if(hndlDrvSem == NULL)
		hndlDrvSem = MakeSem();
	if(hndlDrvSem == NULL){
		return -1;
	}
	DBG("Semaphore Addr: %p\n", hndlDrvSem);
	if((qid = Msg_Init(key)) < 0){
		ERR("Message init fail\n");
		DestroySem(hndlDrvSem);
		return -1;
	}
	DBG("Qid: %d\n",qid);
	gKey = key;
	mid = pShareMemInit(key);
	if(mid < 0){
		ERR("Share memory id error\n");
		DestroySem(hndlDrvSem);
		return -1;
	}
	DBG("Share Memory id : %d\n", mid);
	pShareMem = shmat(mid,0,0);
	DBG("Share Memory Address:%p\n", pShareMem);
	gProcId = iProcId;
	return 0;
}
/**
* @brief Cleanup file message driver.

* This API shoud be called at the end of process.
*/
void CleanupFileMsgDrv()
{
	shmdt(pShareMem);
	DestroySem(hndlDrvSem);
}
/**
* @brief Get system information.

* Get system information from file manager. In order to make every change can
*  update quickly, this function return the pointer to share memory.
* @warning Do NOT modify where this pointer point to.
* @return Pointer to where system information was stored.
* @retval NULL Can't get system information.
*/
SysInfo *GetSysInfo()
{
	FILE_MSG_BUF msgbuf;

	if(eCount >= 3){
		eCount = 0;
		qid = Msg_Init(gKey);
	}
	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gProcId;
	msgbuf.cmd = FILE_MSG_READ;
	msgbuf.shmid = mid;
	SemWait(hndlDrvSem);
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
	msgrcv(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), gProcId, 0);
	SemRelease(hndlDrvSem);
	if(msgbuf.length != sizeof(SysInfo)){
		ERR("Data read error\n");
		eCount++;
		return NULL;
	}
	return (SysInfo *) pShareMem;
}
/**
* @brief Get system log information.

* @param nPageNum [I ] Page number
* @param nItemNum [I ] Item number
* @param pBuf [O ] pointer to where log information stored.
* @retval 0 Success.
* @retval -1 Fail
*/
int GetSysLog(int nPageNum, int nItemNum, LogEntry_t * pBuf)
{
	FILE_MSG_BUF msgbuf;
	SysInfo *pSysInfo = (SysInfo *) pShareMem;

	if(eCount >= 3){
		eCount = 0;
		qid = Msg_Init(gKey);
	}
	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gProcId;
	msgbuf.cmd = FILE_MSG_READ;
	msgbuf.shmid = mid;
	msgbuf.shmidx = 1;
	msgbuf.nPageNum = nPageNum;
	msgbuf.nItemNum = nItemNum;
	SemWait(hndlDrvSem);
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
	msgrcv(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), gProcId, 0);
	SemRelease(hndlDrvSem);
	if(msgbuf.length != sizeof(SysInfo)){
		ERR("Data read error\n");
		eCount++;
		return -1;
	}
	memcpy(pBuf, &pSysInfo->tCurLog, sizeof(LogEntry_t));
	return 0;
}
/**
* @brief Save system information in file manager.
* @param nShmIndex [I ] Share memory index (0 or 1). 0 is used for system settings;
 and 1 is used for system log.
* @retval 0 Success.
* @retval -1 Fail
*/
static int SetSysInfo(int nShmIndex)
{
	FILE_MSG_BUF msgbuf;

	if(eCount >= 3){
		eCount = 0;
		qid = Msg_Init(gKey);
	}
	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gProcId;
	msgbuf.cmd = FILE_MSG_WRITE;
	msgbuf.shmid = mid;
	msgbuf.shmidx = nShmIndex;
	SemWait(hndlDrvSem);
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
	msgrcv(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), gProcId, 0);
	SemRelease(hndlDrvSem);
	if(msgbuf.length != sizeof(SysInfo)){
		ERR("Data write error\n");
		eCount++;
		return -1;
	}
	return 0;
}
/**
* @brief Save system information in file manager immediately
* @retval 0 Success.
* @retval -1 Fail
*/
static int SetSysInfoImmediately()
{
	FILE_MSG_BUF msgbuf;

	if(eCount >= 3){
		eCount = 0;
		qid = Msg_Init(gKey);
	}
	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gProcId;
	msgbuf.cmd = FILE_MSG_WRITE_I;
	msgbuf.shmid = mid;
	SemWait(hndlDrvSem);
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
	msgrcv(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), gProcId, 0);
	SemRelease(hndlDrvSem);
	if(msgbuf.length != sizeof(SysInfo)){
		ERR("Data write error\n");
		eCount++;
		return -1;
	}
	return 0;
}
/**
* @brief Save netmask in file manager

* The netmask here will be used in static IP mode.
* @param netmask [I ] New netmask in static IP mode.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetNetMask(struct in_addr netmask)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.netmask, &netmask, sizeof(netmask));
	return SetSysInfo(0);
}
/**
* @brief Save IP in file manager

* The IP here will be used in static IP mode.
* @param ip [I ] New IP in static IP mode.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetIp(struct in_addr ip)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.ip, &ip, sizeof(ip));
	return SetSysInfoImmediately();
}
/**
* @brief Set http port in file manager

* Http port of web site will be change when boa restart.
* @param port [I ] New http port.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetHttpPort(unsigned short port)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.http_port, &port, sizeof(port));
	return SetSysInfo(0);
}
/**
* @brief Save IPCAM title in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetTitle(void *buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->lan_config.title) < len+1)
		return -1;
	memcpy(pSysInfo->lan_config.title, buf, len);
	pSysInfo->lan_config.title[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save IPCAM title in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetRegUsr(void *buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->lan_config.regusr) < len+1)
		return -1;
	memcpy(pSysInfo->lan_config.regusr, buf, len);
	pSysInfo->lan_config.regusr[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save gateway in file manager
* @param gateway [I ] New gateway in static IP mode.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetGateway(struct in_addr gateway)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.gateway, &gateway, sizeof(gateway));
	return SetSysInfo(0);
}
/**
* @brief Save DHCP status in file manager
* @param enable [I ] 0 meets DHCP mode, otherwise static IP mode.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetDhcpEnable(int enable)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.dhcp_enable, &enable, sizeof(enable));
	return SetSysInfo(0);
}
/**
* @brief Save FTP FQDN in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetFtpFqdn(void *buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->ftp_config.servier_ip) < len + 1)
		return -1;
	memcpy(&pSysInfo->ftp_config.servier_ip, buf, len);
	pSysInfo->ftp_config.servier_ip[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save FTP user name in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetFtpUsername(void *buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->ftp_config.username) < len + 1)
		return -1;
	memcpy(pSysInfo->ftp_config.username, buf, len);
	pSysInfo->ftp_config.username[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save FTP user password in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetFtpPassword(void *buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->ftp_config.password) < len + 1)
		return -1;
	memcpy(pSysInfo->ftp_config.password, buf, len);
	pSysInfo->ftp_config.password[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save FTP folder in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetFtpFoldname(void *buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->ftp_config.foldername) < len + 1)
		return -1;
	memcpy(pSysInfo->ftp_config.foldername, buf, len);
	pSysInfo->ftp_config.foldername[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save FTP image acount in file manager

* This value controls that how many JPEG image will be sent in one FTP
*  connection.
* @param imageacount [I ] JPEG image count in one FTP connection.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetFtpImageacount(int imageacount)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->ftp_config.image_acount, &imageacount, sizeof(imageacount));
	return SetSysInfo(0);
}
/**
* @brief Save FTP port number in file manager
* @param port [I ] New FTP port.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetFtpPort(unsigned short port)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->ftp_config.port, &port, sizeof(port));
	return SetSysInfo(0);
}
/**
* @brief Save FTP client process ID in file manager
* @param pid [I ] FTP client process ID.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetFtpPid(int pid)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->ftp_config.pid, &pid, sizeof(pid));
	return SetSysInfo(0);
}
/**
* @brief Save SMTP FQDN in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSmtpServerIp(void * buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->smtp_config.servier_ip) < len + 1)
		return -1;
	memcpy(pSysInfo->smtp_config.servier_ip, buf, len);
	pSysInfo->smtp_config.servier_ip[len] = '\0';
	return SetSysInfo(0);
}

/**
* @brief Save SMTP port number in file manager
* @param port [I ] New SMTP port.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSmtpServerPort(unsigned short port)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->smtp_config.server_port, &port, sizeof(port));
	return SetSysInfo(0);
}

/**
* @brief Save SMTP Authentication in file manager.

* Determind whether use SMTP authentication to send e-mail or not.
* @param authentication [I ] SMTP authentication.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSmtpAuthentication(unsigned char authentication)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->smtp_config.authentication, &authentication,
		sizeof(authentication));
	return SetSysInfo(0);
}
/**
* @brief Save SMTP user name in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSmtpUsername(void * buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->smtp_config.username) < len + 1)
		return -1;
	memcpy(pSysInfo->smtp_config.username, buf, len);
	pSysInfo->smtp_config.username[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save SMTP user password in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSmtpPassword(void * buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->smtp_config.password) < len + 1)
		return -1;
	memcpy(pSysInfo->smtp_config.password, buf, len);
	pSysInfo->smtp_config.password[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save SMTP sender e-mail address in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSmtpSenderEmail(void * buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->smtp_config.sender_email) < len + 1)
		return -1;
	memcpy(pSysInfo->smtp_config.sender_email, buf, len);
	pSysInfo->smtp_config.sender_email[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save SMTP reveiver e-mail address in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSmtpReceiverEmail(void * buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->smtp_config.receiver_email) < len + 1)
		return -1;
	memcpy(pSysInfo->smtp_config.receiver_email, buf, len);
	pSysInfo->smtp_config.receiver_email[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save e-mail CC in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSmtpCC(void * buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->smtp_config.CC) < len + 1)
		return -1;
	memcpy(pSysInfo->smtp_config.CC, buf, len);
	pSysInfo->smtp_config.CC[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save e-mail subject in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSmtpSubject(void * buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->smtp_config.subject) < len + 1)
		return -1;
	memcpy(pSysInfo->smtp_config.subject, buf, len);
	pSysInfo->smtp_config.subject[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save e-mail contant text in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSmtpText(void * buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->smtp_config.text) < len + 1)
		return -1;
	memcpy(pSysInfo->smtp_config.text, buf, len);
	pSysInfo->smtp_config.text[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save e-mail attachments in file manager.
* @param attachments [I ] Count of JPEG images in one e-mail.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSmtpAttachments(unsigned char attachments)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->smtp_config.attachments, &attachments,
		sizeof(attachments));
	return SetSysInfo(0);
}
/**
* @brief Save e-mail view in file manager.
* @param view [I ] 0 meets text style, otherwise html style.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSmtpView(unsigned char view)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->smtp_config.view, &view, sizeof(view));
	return SetSysInfo(0);
}
/**
* @brief Save DNS ip in file manager.

* DNS IP here will be used under static IP mode.
* @param ip [I ] DNS ip.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetDns(struct in_addr ip)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.dns, &ip, sizeof(ip));
	return SetSysInfo(0);
}
/**
* @brief Save SNTP FQDN in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSntpServer(void *buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->lan_config.net.ntp_server) < len + 1)
		return -1;
	memcpy(pSysInfo->lan_config.net.ntp_server, buf, len);
	pSysInfo->lan_config.net.ntp_server[len] = '\0';
	return SetSysInfo(0);
}
/**
* @brief Save camera day/night mode in file manager.
* @param value [I ] Day/night mode.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetCamDayNight(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nDayNight, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save camera white balance mode in file manager.
* @param value [I ] White balance mode.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetCamWhiteBalance(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nWhiteBalance, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save camera backlight mode in file manager.
* @param value [I ] Backlight mode.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetCamBacklight(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nBackLight, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save camera brightness in file manager.
* @param value [I ] Brightness.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetCamBrightness(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nBrightness, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save camera contrast in file manager.
* @param value [I ] Contrast.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetCamContrast(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nContrast, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save camera saturation in file manager.
* @param value [I ] Saturation.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetCamSaturation(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nSaturation, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save camera sharpness in file manager.
* @param value [I ] Sharpness.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetCamSharpness(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nSharpness, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetMaxExposureValue(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.maxexposure, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetMaxGainValue(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.maxgain, &value, sizeof(value));
	return SetSysInfo(0);
}

/**
* @brief Save JPEG encode quality in file manager.
* @param value [I ] JPEG quality.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetJpegQuality(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.njpegquality, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetMirror(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.mirror, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetFaceDetect(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->face_config.fdetect, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetFDX(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->face_config.startX, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetFDY(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->face_config.startY, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetFDW(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->face_config.width, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetFDH(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->face_config.height, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetFDConfLevel(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->face_config.fdconflevel, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetFDDirection(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->face_config.fddirection, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetFRecognition(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->face_config.frecog, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetFRConfLevel(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->face_config.frconflevel, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetFRDatabase(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->face_config.frdatabase, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetPrivacyMask(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->face_config.pmask, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetMaskOptions(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->face_config.maskoption, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetAdvMode(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	memcpy(&pSysInfo->lan_config.AdvanceMode, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetDemoCfg(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.democfg, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetOSDWin(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.osdwin, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetHistogram(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.histogram, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetDynRange(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.dynRange, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetOSDStream(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.osdstream, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetOSDWinNum(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.osdwinnum, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save IPCAM title in file manager
* @param buf [I ] Pointer to string.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetOSDText(void *buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->lan_config.osdtext) < len+1)
		return -1;
	memcpy(pSysInfo->lan_config.osdtext, buf, len);
	pSysInfo->lan_config.osdtext[len] = '\0';

	return SetSysInfo(0);
}

/**
* @brief Save video codec mode in file manager.

* Video codec mode can be single codec, dual codec, triple codec.
* @param value [I ] Video codec mode.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetVideoMode(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nStreamType, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetVideoCodecMode(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nVideocodecmode, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetVideoCodecCombo(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nVideocombo, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetVideoCodecRes(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nVideocodecres, &value, sizeof(value));
	return SetSysInfo(0);
}

/**
* @brief Save time display format in file manager.
* @param value [I ] Time display format.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetTimeFormat(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.time_format, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save daylight time setting in file manager.
* @param value [I ] Use daylight or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetDaylightTime(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.daylight_time, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save time zone in file manager.
* @param value [I ] Time zone.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetTimeZone(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.ntp_timezone, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save image size in file manager.
* @param stream [I ] 0: JPEG stream. 1: MPEG1 stream. 2: MPEG2 stream.
* @param Xvalue [I ] Image width.
* @param Yvalue [I ] Image height.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetVideoSize(unsigned char stream, int Xvalue, int Yvalue)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	if (stream == 1 || stream == 4) {
		memcpy(&pSysInfo->lan_config.JpegXsize, &Xvalue, sizeof(Xvalue));
		memcpy(&pSysInfo->lan_config.JpegYsize, &Yvalue, sizeof(Yvalue));
	} else if (stream == 2) {
		memcpy(&pSysInfo->lan_config.Mpeg41Xsize, &Xvalue, sizeof(Xvalue));
		memcpy(&pSysInfo->lan_config.Mpeg41Ysize, &Yvalue, sizeof(Yvalue));
	} else if (stream == 3) {
		memcpy(&pSysInfo->lan_config.Mpeg42Xsize, &Xvalue, sizeof(Xvalue));
		memcpy(&pSysInfo->lan_config.Mpeg42Ysize, &Yvalue, sizeof(Yvalue));
	} else if (stream == 5) {
		memcpy(&pSysInfo->lan_config.Avc1Xsize, &Xvalue, sizeof(Xvalue));
		memcpy(&pSysInfo->lan_config.Avc1Ysize, &Yvalue, sizeof(Yvalue));
	} else if (stream == 6) {
		memcpy(&pSysInfo->lan_config.Avc2Xsize, &Xvalue, sizeof(Xvalue));
		memcpy(&pSysInfo->lan_config.Avc2Ysize, &Yvalue, sizeof(Yvalue));
	}

	return SetSysInfo(0);
}

int fSetStreamConfig(unsigned char stream, int width, int height, char *name, int portnum)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	fResetFDROIParam(stream, width, height);
	fResetCodecROIParam(stream, width, height);

	pSysInfo->stream_config[stream].width	= width;
	pSysInfo->stream_config[stream].height  = height;

	strcpy((char *)pSysInfo->stream_config[stream].name, name);

	pSysInfo->stream_config[stream].portnum  = portnum;

	if((portnum==H264_1_PORTNUM) || (portnum==H264_2_PORTNUM))
		strcpy((char *)pSysInfo->stream_config[stream].portname, "PSIA/Streaming/channels/2?videoCodecType=H.264");  //h264   PSIA/Streaming/channels/2?videoCodecType=H.264
	else if((portnum==MPEG4_1_PORTNUM) || (portnum==MPEG4_2_PORTNUM))
		strcpy((char *)pSysInfo->stream_config[stream].portname, "PSIA/Streaming/channels/1?videoCodecType=MPEG4");  //mpeg4   PSIA/Streaming/channels/1?videoCodecType=MPEG4
	else
		strcpy((char *)pSysInfo->stream_config[stream].portname, "PSIA/Streaming/channels/0?videoCodecType=MJPEG");  //mjpeg   PSIA/Streaming/channels/0?videoCodecType=MJPEG

	return SetSysInfo(0);
}

void fResetCodecROIParam(int stream, int width, int height)
{
	int i=0;

    SysInfo *pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return;

	for(i=0;i<pSysInfo->codec_roiconfig[stream].numROI;i++) {
		pSysInfo->codec_roiconfig[stream].roi[i].startx = (width*pSysInfo->codec_roiconfig[stream].roi[i].startx)/pSysInfo->codec_roiconfig[stream].prevwidth;
		pSysInfo->codec_roiconfig[stream].roi[i].starty = (height*pSysInfo->codec_roiconfig[stream].roi[i].starty)/pSysInfo->codec_roiconfig[stream].prevheight;
		pSysInfo->codec_roiconfig[stream].roi[i].width 	= (width*pSysInfo->codec_roiconfig[stream].roi[i].width)/pSysInfo->codec_roiconfig[stream].prevwidth;
		pSysInfo->codec_roiconfig[stream].roi[i].height = (height*pSysInfo->codec_roiconfig[stream].roi[i].height)/pSysInfo->codec_roiconfig[stream].prevheight;
	}

	pSysInfo->codec_roiconfig[stream].prevwidth 	= (width<=0)?1:width;
	pSysInfo->codec_roiconfig[stream].prevheight 	= (height<=0)?1:height;

	return;
}

void fResetFDROIParam(int stream, int width, int height)
{
    SysInfo *pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return;

	if(stream == 0) {
		pSysInfo->face_config.startX 		= (width*pSysInfo->face_config.startX)/pSysInfo->face_config.prevwidth;
		pSysInfo->face_config.startY 		= (height*pSysInfo->face_config.startY)/pSysInfo->face_config.prevheight;
		pSysInfo->face_config.width  		= (width*pSysInfo->face_config.width)/pSysInfo->face_config.prevwidth;
		pSysInfo->face_config.height 		= (height*pSysInfo->face_config.height)/pSysInfo->face_config.prevheight;
		pSysInfo->face_config.prevwidth  	= (width<=0)?1:width;
		pSysInfo->face_config.prevheight 	= (height<=0)?1:height;
	}

	return;
}

/**
* @brief Save stream active status in file manager.
* @param stream1 [I ] Active or not.
* @param stream2 [I ] Active or not.
* @param stream3 [I ] Active or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetStreamActive(unsigned char stream1, unsigned char stream2, unsigned char stream3,
                     unsigned char stream4, unsigned char stream5, unsigned char stream6)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.Supportstream1, &stream1, sizeof(stream1));
	memcpy(&pSysInfo->lan_config.Supportstream2, &stream2, sizeof(stream2));
	memcpy(&pSysInfo->lan_config.Supportstream3, &stream3, sizeof(stream3));
	memcpy(&pSysInfo->lan_config.Supportstream4, &stream4, sizeof(stream4));
	memcpy(&pSysInfo->lan_config.Supportstream5, &stream5, sizeof(stream5));
	memcpy(&pSysInfo->lan_config.Supportstream6, &stream6, sizeof(stream6));
	return SetSysInfo(0);
}
/**
* @brief Save TV out clock config in file manager
* @param value [I] NTSC or PAL mode.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetImageSource(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.imagesource, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save MPEG1 bitrate in file manager.
* @param value [I ] Bitrate.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetMP41bitrate(int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nMpeg41bitrate, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save MPEG2 bitrate in file manager.
* @param value [I ] Bitrate.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetMP42bitrate(int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nMpeg42bitrate, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save encode frame rate in file manager.
* @param stream [I ] Which stream.
* @param value [I ] Frame rate.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetFramerate1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	memcpy(&pSysInfo->lan_config.nFrameRate1, &value, sizeof(value));

	return SetSysInfo(0);
}

int fSetFramerate2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	memcpy(&pSysInfo->lan_config.nFrameRate2, &value, sizeof(value));

	return SetSysInfo(0);
}

int fSetFramerate3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	memcpy(&pSysInfo->lan_config.nFrameRate3, &value, sizeof(value));

	return SetSysInfo(0);
}

int fSetFramerateVal1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	memcpy(&pSysInfo->lan_config.frameRateVal1, &value, sizeof(value));

	return SetSysInfo(0);
}

int fSetFramerateVal2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	memcpy(&pSysInfo->lan_config.frameRateVal2, &value, sizeof(value));

	return SetSysInfo(0);
}

int fSetFramerateVal3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	memcpy(&pSysInfo->lan_config.frameRateVal3, &value, sizeof(value));

	return SetSysInfo(0);
}

/**
* @brief Save rate control in file manager.
*
* @param value [I ] 0: OFF, 1: VBR, 2: CBR
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetRateControl(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nRateControl, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetImageFormat(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.imageformat, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save JPEG resolution in file manager.
* @param value [I ] JPEG resolution.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetResolution(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.liveresolution, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save MPEG1 resolution in file manager.
* @param value [I ] MPEG1 resolution.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetMPEG4Res(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.mpeg4resolution, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save MPEG2 resolution in file manager.
* @param value [I ] MPEG2 resolution.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetMPEG42Res(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.mpeg42resolution, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save user schedule in file manager.

* Number of schedules was define as \ref SCHDULE_NUM . User can modify the
	front 7 schedules by web site.
* @param index [I ] User schedule index.
* @param pSchedule [I ] User schedule.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSchedule(int index,Schedule_t* pSchedule, int day, int year)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	memcpy(&pSysInfo->lan_config.aSchedules[index], pSchedule, sizeof(Schedule_t));
	memcpy(&pSysInfo->lan_config.schedCurDay, &day, sizeof(day));
	memcpy(&pSysInfo->lan_config.schedCurYear, &year, sizeof(year));

	return SetSysInfo(0);
}
/**
* @brief Save Ethernet lost status in file manager.
* @param value [I ] Ethernet lost or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetLostAlarm(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.lostalarm, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save SD card alarm enable status in file manager.
* @param value [I ] Enable alarm save to SD card or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSDAlarmEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.bSdaEnable, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save FTP alarm enable status in file manager.
* @param value [I ] Enable alarm save to FTP or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetFTPAlarmEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.bAFtpEnable, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save SMTP alarm enable status in file manager.
* @param value [I ] Enable alarm save to SMTP or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSMTPAlarmEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.bASmtpEnable, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save alarm duration in file manager.
* @param value [I ] New alarm duration.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetAlarmDuration(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nAlarmDuration, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save AVI duration in file manager.
* @param value [I ] New AVI duration.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetAVIDuration(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.aviduration, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save AVI format in file manager.
* @param value [I ] New AVI format.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetAVIFormat(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.aviformat, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save FTP file format in file manager.
* @param value [I ] 0:MPEG4 , 1:JPEG.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetFTPFileFormat(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->ftp_config.ftpfileformat, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save SD file format in file manager.
* @param value [I ] 0:MPEG4 , 1:JPEG.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSDFileFormat(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->sdcard_config.sdfileformat, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief SaveSMTP file format in file manager.
* @param value [I ] 0:MPEG4 , 1:JPEG.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetAttFileFormat(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->smtp_config.attfileformat, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save web site audio enable status in file manager.
* @param value [I ] Enable audio on web site or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetAudioON(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->audio_config.audioON, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetAudioEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->audio_config.audioenable, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetAudioMode(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->audio_config.audiomode, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetAudioEncode(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->audio_config.codectype, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetAudioSampleRate(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->audio_config.samplerate, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetAudioBitrateValue(int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->audio_config.bitrateValue, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetAudioBitrate(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->audio_config.bitrate, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetAudioAlarmLevel(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->audio_config.alarmlevel, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetAudioOutVolume(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->audio_config.audiooutvolume, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetASmtpAttach(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->smtp_config.asmtpattach, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save record to FTP status in file manager.
* @param value [I ] Enable record to FTP or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetRftpenable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->ftp_config.rftpenable, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save record to SD card status in file manager.
* @param value [I ] Enable record to SD card or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSdReEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->sdcard_config.sdrenable, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save 2A mode in file manager.
* @param value [I ] New 2A mode.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetImageAEW(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nAEWswitch, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save 2A mode in file manager.
* @param value [I ] New 2A mode.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetImageAEWType(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nAEWtype, &value, sizeof(value));
	return SetSysInfo(0);
}

/**
* @brief Save binning mode in file manager.
* @param value [I ] New binning mode.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetBinning(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(pSysInfo->lan_config.nBinning == value)
		return 0;
	memcpy(&pSysInfo->lan_config.nBinning, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save backlight control status in file manager.
* @param value [I ] New backlight control mode.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetBlc(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nBacklightControl, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save motion detect enable status in file manager.
* @param value [I ] Enable motion detection or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetMotionEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->motion_config.motionenable, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save motion detect choice status in file manager.
* @param value [I ] New motion detect choice status.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetMotionCEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->motion_config.motioncenable, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save motion detect level status in file manager.
* @param value [I ] New motion detect level status.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetMotionLevel(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->motion_config.motionlevel, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save motion detect user define value in file manager.
* @param value [I ] New motion detect user define value.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetMotionCValue(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->motion_config.motioncvalue, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save motion block in file manager.
* @param pValue [I ] New motion block value.
* @param len [I ] String length.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetMotionBlock(char *pValue,int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(MOTION_BLK_LEN < len)
		return -1;
	memcpy(&pSysInfo->motion_config.motionblock, pValue, len);
	return SetSysInfo(0);
}
/**
* @brief Save SD insert status in file manager.
* @param value [I ] SD insert status.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSDInsert(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->sdcard_config.sdinsert, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save user acount in file manager.
* @param index [I ] User acount index.
* @param acout [I ] User acount data.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetAcount(int index, Acount_t *acout)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->acounts[index], acout, sizeof(Acount_t));
	return SetSysInfo(0);
}
/**
* @brief Save IP netcam pan and zoom status in file manager.
* @param value [I ] Pan and zoom status.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetIpncPtz(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->ptz_config, &value, sizeof(value));

	return SetSysInfo(0);
}
/**
* @brief Save GIO input alarm status in file manager.
* @param value [I ] Enable GIO input alarm or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetGIOInEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.gioinenable, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save GIO input triger status in file manager.
* @param value [I ] High or low triger.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetGIOInType(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.giointype, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save GIO output alarm status in file manager.
* @param value [I ] Enable alarm GIO output or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetGIOOutEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.giooutenable, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save GIO outputput status in file manager.
* @param value [I ] output high or low.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetGIOOutType(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.gioouttype, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save time stamp status in file manager.
* @param value [I ] Enable time stamp or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetTStampEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.tstampenable, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Save time stamp format in file manager.
* @param value [I ] Time stamp format.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetDateFormat(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.dateformat, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetTStampFormat(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.tstampformat, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetDatePosition(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.dateposition, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetTimePosition(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.timeposition, &value, sizeof(value));
	return SetSysInfo(0);
}

/**
* @brief Save alarm audio volume in file manager.
* @param value [I ] Alarm audio volume.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetAudioInVolume(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->audio_config.audioinvolume, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Reset system to default.
* @retval 0 Success.
* @retval -1 Fail
*/
int fResetSysInfo()
{
	FILE_MSG_BUF msgbuf;

	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gProcId;
	msgbuf.cmd = FILE_MSG_RESET;
	msgbuf.shmid = mid;
	SemWait(hndlDrvSem);
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
	msgrcv(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), gProcId, 0);
	SemRelease(hndlDrvSem);
	if(msgbuf.length != sizeof(SysInfo)){
		ERR("FILE_MSG_RESET fail\n");
		return -1;
	}
	return 0;
}
/**
* @brief Save a system log in file manager.
* @param value [I ] System log.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetSysLog(LogEntry_t* value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL || value == NULL)
		return -1;
	memcpy(&pSysInfo->tCurLog, value, sizeof(LogEntry_t));
	DBG("LOG DEBUG MSG: event: %s\n", pSysInfo->tCurLog.event);
	DBG("LOG DEBUG MSG: time: %s\n", asctime(&pSysInfo->tCurLog.time));
	return SetSysInfo(1);
}
/**
* @brief Update alarm status in file manager.

* This function will change alarm status in file manager.
* @param value [I ] New alarm status
* @retval -1 Fail to update.
* @retval 0 Update successfully.
*/
int fSetAlarmStatus(unsigned short value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.alarmstatus, &value, sizeof(value));
	return SetSysInfo(0);
}
/**
* @brief Set whether to support PTZ or not.
* @param value [I ] support PTZ or not.
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetPtzSupport(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if (value == pSysInfo->lan_config.net.supportptzpage)
		return 0;
	memcpy(&pSysInfo->lan_config.net.supportptzpage, &value, sizeof(value));
	return SetSysInfo(0);
}

/**
* @brief Save .
* @param value [I ] .
* @retval 0 Success.
* @retval -1 Fail
*/
int fSetNetMulticast(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if (value == pSysInfo->lan_config.net.multicast_enable)
		return 0;
	memcpy(&pSysInfo->lan_config.net.multicast_enable, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetHttpsPort(unsigned short value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.net.https_port, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetRS485Port(unsigned short value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.rs485config, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetRateControl1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nRateControl1, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetRateControl2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nRateControl2, &value, sizeof(value));
	return SetSysInfo(0);
}


int fSetDateStampEnable1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[0].dstampenable, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetDateStampEnable2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[1].dstampenable, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetDateStampEnable3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[2].dstampenable, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetTimeStampEnable1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[0].tstampenable, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetTimeStampEnable2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[1].tstampenable, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetTimeStampEnable3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[2].tstampenable, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetLogoEnable1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[0].nLogoEnable, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetLogoEnable2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[1].nLogoEnable, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetLogoEnable3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[2].nLogoEnable, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetLogoPosition1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[0].nLogoPosition, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetLogoPosition2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[1].nLogoPosition, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetLogoPosition3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[2].nLogoPosition, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetTextPosition1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[0].nTextPosition, &value, sizeof(value));
	return SetSysInfo(0);
}


int fSetTextPosition2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[1].nTextPosition, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetTextPosition3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[2].nTextPosition, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetTextEnable1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[0].nTextEnable, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetTextEnable2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[1].nTextEnable, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetTextEnable3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[2].nTextEnable, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetOverlayText1(void *buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	memcpy(pSysInfo->osd_config[0].overlaytext, buf, len);
	pSysInfo->osd_config[0].overlaytext[len] = '\0';

	return SetSysInfo(0);
}

int fSetOverlayText2(void *buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	memcpy(pSysInfo->osd_config[1].overlaytext, buf, len);
	pSysInfo->osd_config[1].overlaytext[len] = '\0';

	return SetSysInfo(0);
}

int fSetOverlayText3(void *buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	memcpy(pSysInfo->osd_config[2].overlaytext, buf, len);
	pSysInfo->osd_config[2].overlaytext[len] = '\0';

	return SetSysInfo(0);
}
int fSetDetailInfo1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[0].nDetailInfo, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetDetailInfo2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[1].nDetailInfo, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetDetailInfo3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->osd_config[2].nDetailInfo, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetEncryptVideo(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nEncryptVideo, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetLocalDisplay(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nLocalDisplay, &value, sizeof(value));
	return SetSysInfo(0);
}

 int fSetIpratio1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[0].ipRatio, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetIpratio2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[1].ipRatio, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetIpratio3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[2].ipRatio, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetForceIframe1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[0].fIframe, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetForceIframe2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[1].fIframe, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetForceIframe3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[2].fIframe, &value, sizeof(value));
	return SetSysInfo(0);
}

int  fSetQPInit1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[0].qpInit, &value, sizeof(value));
	return SetSysInfo(0);
}

int  fSetQPInit2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[1].qpInit, &value, sizeof(value));
	return SetSysInfo(0);
}

int  fSetQPInit3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[2].qpInit, &value, sizeof(value));
	return SetSysInfo(0);
}

int  fSetQPMin1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[0].qpMin, &value, sizeof(value));
	return SetSysInfo(0);
}

int  fSetQPMin2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[1].qpMin, &value, sizeof(value));
	return SetSysInfo(0);
}

int  fSetQPMin3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[2].qpMin, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetQPMax1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[0].qpMax, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetQPMax2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[1].qpMax, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetQPMax3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[2].qpMax, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetMEConfig1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[0].meConfig, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetMEConfig2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[1].meConfig, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetMEConfig3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[2].meConfig, &value, sizeof(value));
	return SetSysInfo(0);
}

int  fSetPacketSize1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[0].packetSize, &value, sizeof(value));
	return SetSysInfo(0);
}

int  fSetPacketSize2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[1].packetSize, &value, sizeof(value));
	return SetSysInfo(0);
}

int  fSetPacketSize3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_advconfig[2].packetSize, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetROIEnable1(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[0].numROI, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetROIEnable2(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[1].numROI, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetROIEnable3(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[2].numROI, &value, sizeof(value));
	return SetSysInfo(0);
}


int fSetStr1X1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[0].roi[0].startx, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr1Y1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[0].roi[0].starty, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr1W1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[0].roi[0].width, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr1H1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[0].roi[0].height, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr1X2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[0].roi[1].startx, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr1Y2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[0].roi[1].starty, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr1W2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[0].roi[1].width, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr1H2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[0].roi[1].height, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr1X3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[0].roi[2].startx, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr1Y3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[0].roi[2].starty, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr1W3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[0].roi[2].width, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr1H3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[0].roi[2].height, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr2X1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[1].roi[0].startx, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr2Y1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[1].roi[0].starty, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr2W1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[1].roi[0].width, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr2H1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[1].roi[0].height, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr2X2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[1].roi[1].startx, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr2Y2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[1].roi[1].starty, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr2W2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[1].roi[1].width, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr2H2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[1].roi[1].height, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr2X3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[1].roi[2].startx, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr2Y3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[1].roi[2].starty, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr2W3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[1].roi[2].width, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr2H3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[1].roi[2].height, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr3X1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[2].roi[0].startx, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr3Y1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[2].roi[0].starty, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr3W1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[2].roi[0].width, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr3H1(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[2].roi[0].height, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr3X2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[2].roi[1].startx, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr3Y2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[2].roi[1].starty, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr3W2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[2].roi[1].width, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr3H2(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[2].roi[1].height, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr3X3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[2].roi[2].startx, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr3Y3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[2].roi[2].starty, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr3W3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[2].roi[2].width, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetStr3H3(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->codec_roiconfig[2].roi[2].height, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetExpPriority(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.expPriority, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetClickSnapFilename(void *buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;

	memcpy(pSysInfo->lan_config.nClickSnapFilename, buf, len);
	pSysInfo->lan_config.nClickSnapFilename[len] = '\0';

	return SetSysInfo(0);
}

int fSetClickSnapStorage(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nClickSnapStorage, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetAlarmEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nAlarmEnable, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetExtAlarm(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nExtAlarm, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetAudioAlarm(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->audio_config.alarmON, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetAlarmAudioPlay(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nAlarmAudioPlay, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetAlarmAudioFile(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nAlarmAudioFile, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetScheduleRepeatEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nScheduleRepeatEnable, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetScheduleNumWeeks(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nScheduleNumWeeks, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetScheduleInfiniteEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.nScheduleInfiniteEnable, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetAlarmStorage(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.alarmlocal, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetRecordStorage(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.recordlocal, &value, sizeof(value));
	return SetSysInfo(0);
}
int fSetChipConfig(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.chipConfig, &value, sizeof(value));
	return SetSysInfo(0);
}
/* System and Access log clear */
int fSetClearSysLog(unsigned char value)
{
	FILE_MSG_BUF msgbuf;

	if(eCount >= 3){
		eCount = 0;
		qid = Msg_Init(gKey);
	}
	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gProcId;
	msgbuf.cmd = FILE_MSG_CLEAR_SYSLOG;
	msgbuf.shmid = mid;
	SemWait(hndlDrvSem);
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
	msgrcv(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), gProcId, 0);
	SemRelease(hndlDrvSem);
	if(msgbuf.length != sizeof(SysInfo)){
		ERR("Data read error\n");
		eCount++;
		return -1;
	}
	return 0;
}

int fSetClearAccessLog(unsigned char value)
{
	FILE_MSG_BUF msgbuf;

	if(eCount >= 3){
		eCount = 0;
		qid = Msg_Init(gKey);
	}
	memset(&msgbuf, 0, sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.src = gProcId;
	msgbuf.cmd = FILE_MSG_CLEAR_ACCESSLOG;
	msgbuf.shmid = mid;
	SemWait(hndlDrvSem);
	msgsnd(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);/*send msg1*/
	msgrcv(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), gProcId, 0);
	SemRelease(hndlDrvSem);
	if(msgbuf.length != sizeof(SysInfo)){
		ERR("Data read error\n");
		eCount++;
		return -1;
	}
	return 0;
}

int fSetAudioReceiverEnable(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->audio_config.audiorecvenable, &value, sizeof(value));
	return SetSysInfo(0);
}


int fSetAudioSeverIp(void *buf, int len)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	if(sizeof(pSysInfo->audio_config.audioServerIp) < len+1)
		return -1;
	memcpy(pSysInfo->audio_config.audioServerIp, buf, len);
	pSysInfo->audio_config.audioServerIp[len] = '\0';

	return SetSysInfo(0);
}

int fSetLinearWdr(unsigned char value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.linearwdr, &value, sizeof(value));
	return SetSysInfo(0);
}

int fSetFlickerDetect(unsigned int value)
{
	SysInfo *pSysInfo = (SysInfo *)pShareMem;
	if(pSysInfo == NULL)
		return -1;
	memcpy(&pSysInfo->lan_config.flickerdetect, &value, sizeof(value));
	return SetSysInfo(0);
}
