/** ===========================================================================
* @file main.c
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <Msg_Def.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <system_control.h>
#include "share_mem.h"
#include <pthread.h>
#include <file_msg_drv.h>
#include <alarm_msg_drv.h>
#include <schedule_mng.h>
#include <av_server_ctrl.h>
#include <sd_config.h>


#define SYSTEM_SERVER_BUFFER_LENGTH	256
#define FILE_THR_CREATED	0x1
#define ALARM_THR_CREATED	0x2
#ifdef DEBUG
#define DBG(fmt, args...)	printf("Debug " fmt, ##args)
#else
#define DBG(fmt, args...)
#endif

#define ERR(fmt, args...)	printf("Error " fmt, ##args)
static int IsSysQuit = 0;
extern void *FileMngThread(void *arg);
extern void *AlramThread(void *arg);

/**
 * @brief	receive and process system message
 * @param	"SYS_MSG_BUF* pMsg"	[IN]buffer of message
 * @retval	1: Message is proccessed
 * @retval	0: Message has some error
 */
int ProcSysMsg(SYS_MSG_BUF* pMsg)
{
	int ret = 0;
	void *pData = NULL;
	char buffer[SYSTEM_SERVER_BUFFER_LENGTH];
	switch(pMsg->cmd){
		case SYS_MSG_GET_DHCP_EN:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetDhcpEn()) == NULL)
				break;
			pMsg->length = sizeof(int);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_HTTP_PORT:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetHttpPort()) == NULL)
				break;
			pMsg->length = sizeof(unsigned short);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SNTP_FQDN:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetFqdn()) == NULL)
				break;
			pMsg->length = strlen(pData) + 1;
			ret = 2;
			break;
		}
		case SYS_MSG_SET_NETMASK:
		{
			in_addr_t net_mask;
			ShareMemRead(pMsg->offset, &net_mask, pMsg->length);
			if(SetNetMask(net_mask) != 0){
				printf("Fail at SYS_MSG_SET_NETMASK\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TIMEFORMAT:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TIMEFORMAT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTimeFormat(value) != 0){
				printf("\nSystemServer:Fail at SYS_MSG_SET_TIMEFORMAT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DAYLIGHT:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_DAYLIGHT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetDaylightTime(value) != 0){
				printf("\nSystemServer:Fail at SYS_MSG_SET_DAYLIGHT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TIMEZONE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TIMEZONE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTimeZone(value) != 0){
				printf("\nSystemServer:Fail at SYS_MSG_SET_TIMEZONE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_GET_DEVICE_TYPE:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetDeviceType()) == NULL)
				break;
			pMsg->length = sizeof(unsigned short);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_TITLE:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetTitle()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_REGUSR:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetRegUsr()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_OSDTEXT:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetOSDText()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_MAC:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetMac()) == NULL)
				break;
			pMsg->length = MAC_LENGTH;
			ret = 2;
			break;
		}
		case SYS_MSG_GET_IP:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetIp()) == NULL)
				break;
			pMsg->length = sizeof(in_addr_t);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_GATEWAY:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetGateWay()) == NULL)
				break;
			pMsg->length = sizeof(in_addr_t);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_NETMASK:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetNetmask()) == NULL)
				break;
			pMsg->length = sizeof(in_addr_t);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_FTP_SERVER_IP:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetFtpServerIp()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_FTP_USERNAME:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetFtpUsername()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_FTP_PASSWORD:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetFtpPassword()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_FTP_FOLDNAME:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetFtpFoldername()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_FTP_IMAGEAMOUNT:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetFtpImageAmount()) == NULL)
				break;
			pMsg->length = sizeof(int);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_FTP_PORT:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetFtpPort()) == NULL)
				break;
			pMsg->length = sizeof(__u16);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_FTP_PID:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetFtpPid()) == NULL)
				break;
			pMsg->length = sizeof(int);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SMTP_SERVER_IP:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetSmtpServerIp()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SMTP_SERVER_PORT:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetSmtpServerPort()) == NULL)
				break;
			pMsg->length = sizeof(__u16);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SMTP_AUTHENTICATION:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetSmtpAuthentication()) == NULL)
				break;
			pMsg->length = sizeof(char);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SMTP_USERNAME:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetsmtpUsername()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SMTP_PASSWORD:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetSmtpPassword()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SMTP_SENDER_EMAIL_ADDRESS:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetSmtpSenderEmail()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SMTP_RECEIVER_EMAIL_ADDRESS:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetSmtpReceiverEmail()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SMTP_CC:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetSmtpCC()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SD_FILENAME:
		{
			/* Get current recording file name in SD card */
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetSdFileName()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SMTP_SUBJECT:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetSmtpSubject()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SMTP_TEXT:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetSmtpText()) == NULL)
				break;
			pMsg->length = strlen(pData);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SMTP_ATTACHMENTS:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetSmtpAttachments()) == NULL)
				break;
			pMsg->length = sizeof(char);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_SMTP_VIEW:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetSmtpView()) == NULL)
				break;
			pMsg->length = sizeof(char);
			ret = 2;
			break;
		}
		case SYS_MSG_GET_DHCP_CONFIG:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetDhcpCinfig()) == NULL)
				break;
			pMsg->length = sizeof(int);
			ret = 2;
			break;
		}
	/**	case SYS_MSG_GET_UPNP_PORT:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetUpnpPort()) == NULL)
				break;
			pMsg->length = sizeof(unsigned short);
			ret = 2;
			break;
		}*/
		case SYS_MSG_SET_IP:
		{
			in_addr_t ip;
			if(pMsg->length != sizeof(ip))
				break;
			ShareMemRead(pMsg->offset, &ip, pMsg->length);
			if(SetIp(ip) != 0){
				printf("Fail at SYS_MSG_SET_IP\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_HTTPPORT:
		{
			unsigned short port;
			if(pMsg->length != sizeof(port))
				break;
			ShareMemRead(pMsg->offset, &port, pMsg->length);
			if(SetHttpPort(port) != 0){
				printf("Fail at SYS_MSG_SET_HTTPPORT\n");
				break;
			}
			/* No result to boa */
			if(pMsg->src == SYS_BOA_MSG)
				pMsg->src = 0;
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TITLE:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetTitle(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_TITLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_REGUSR:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetRegUsrName(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_REGUSR\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_OSDTEXT:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetOSDTextInfo(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_OSDTEXT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_GATEWAY:
		{
			in_addr_t gateway;
			if(pMsg->length != sizeof(gateway))
				break;
			ShareMemRead(pMsg->offset, &gateway, pMsg->length);
			if(SetGateway(gateway) != 0){
				printf("Fail at SYS_MSG_SET_GATEWAY\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DHCPC_ENABLE:
		{
			int enable;
			if(pMsg->length != sizeof(enable))
				break;
			ShareMemRead(pMsg->offset, &enable, pMsg->length);
			if(SetDhcpEnable(enable) != 0){
				printf("Fail at SYS_MSG_SET_DHCPC_ENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FTP_FQDN:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetFtpFqdn(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_FTP_FQDN\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FTP_USERNAME:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetFtpUsername(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_FTP_USERNAME\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FTP_PASSWORD:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetFtpPassword(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_FTP_PASSWORD\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FTP_FOLDNAME:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetFtpFoldname(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_FTP_FOLDNAME\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FTP_IMAGEAMOUNT:
		{
			int imageacount;
			if(pMsg->length != sizeof(imageacount))
				break;
			ShareMemRead(pMsg->offset, &imageacount, pMsg->length);
			if(SetFtpImageacount(imageacount) != 0){
				printf("Fail at SYS_MSG_SET_FTP_IMAGEAMOUNT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FTP_PORT:
		{
			unsigned short pid;
			if(pMsg->length != sizeof(pid))
				break;
			ShareMemRead(pMsg->offset, &pid, pMsg->length);
			if(SetFtpPort(pid) != 0){
				printf("Fail at SYS_MSG_SET_FTP_PORT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FTP_PID:
		{
			int pid;
			if(pMsg->length != sizeof(pid))
				break;
			ShareMemRead(pMsg->offset, &pid, pMsg->length);
			if(SetFtpPid(pid) != 0){
				printf("Fail at SYS_MSG_SET_FTP_PID\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SMTP_SERVER_IP:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetSmtpServerIp(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_SMTP_USERNAME\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SMTP_SERVER_PORT:
		{
			unsigned short pid;
			if(pMsg->length != sizeof(pid))
				break;
			ShareMemRead(pMsg->offset, &pid, pMsg->length);
			if(SetSmtpServerPort(pid) != 0){
				printf("Fail at SYS_MSG_SET_SMTP_SERVER_PORT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SMTP_AUTHENTICATION:
		{
			unsigned char authentication;
			if(pMsg->length != sizeof(authentication))
				break;
			ShareMemRead(pMsg->offset, &authentication, pMsg->length);
			if(SetSmtpAuthentication(authentication) != 0){
				printf("Fail at SYS_MSG_SET_SMTP_AUTHENTICATION\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SMTP_USERNAME:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetSmtpUsername(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_SMTP_USERNAME\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SMTP_PASSWORD:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetSmtpPassword(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_SMTP_PASSWORD\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SMTP_SENDER_EMAIL_ADDRESS:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetSmtpSenderEmail(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_SMTP_SENDER_EMAIL_ADDRESS\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SMTP_RECEIVER_EMAIL_ADDRESS:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetSmtpReceiverEmail(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_SMTP_RECEIVER_EMAIL_ADDRESS\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SMTP_CC:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetSmtpCC(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_SMTP_CC\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SMTP_SUBJECT:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetSmtpSubject(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_SMTP_SUBJECT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SMTP_TEXT:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetSmtpText(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_SMTP_TEXT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SMTP_ATTACHMENTS:
		{
			unsigned char attachments;
			if(pMsg->length != sizeof(attachments))
				break;
			ShareMemRead(pMsg->offset, &attachments, pMsg->length);
			if(SetSmtpAttachments(attachments) != 0){
				printf("Fail at SYS_MSG_SET_SMTP_ATTACHMENTS\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SMTP_VIEW:
		{
			unsigned char view;
			if(pMsg->length != sizeof(view))
				break;
			ShareMemRead(pMsg->offset, &view, pMsg->length);
			if(SetSmtpView(view) != 0){
				printf("Fail at SYS_MSG_SET_SMTP_VIEW\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_GET_DNS:
		{
			if(pMsg->src <= MSG_TYPE_MSG1)
				break;
			if((pData = GetDns()) == NULL)
				break;
			pMsg->length = sizeof(in_addr_t);
			ret = 2;
			break;
		}
		case SYS_MSG_SET_DNS:
		{
			in_addr_t ip;
			if(pMsg->length != sizeof(ip))
				break;
			ShareMemRead(pMsg->offset, &ip, pMsg->length);
			if(SetDns(ip) != 0){
				printf("Fail at SYS_MSG_SET_DNS\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SNTP_SERVER_IP:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetSntpServer(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_SNTP_SERVER_IP\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DAY_NIGHT:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_DAY_NIGHT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetCamDayNight(value) != 0){
				printf("Fail at SYS_MSG_SET_DAY_NIGHT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_WHITE_BALANCE:
		{
			unsigned char value;
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			DBG("SYS_MSG_SET_WHITE_BALANCE :%d\n", value);
			if(SetCamWhiteBalance(value) != 0){
				printf("Fail at SYS_MSG_SET_WHITE_BALANCE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_BACKLIGHT:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_BACKLIGHT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetCamBacklight(value) != 0){
				printf("Fail at SYS_MSG_SET_BACKLIGHT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_BRIGHTNESS:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_BRIGHTNESS\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetCamBrightness(value) != 0){
				printf("Fail at SYS_MSG_SET_BRIGHTNESS\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_CONTRAST:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_CONTRAST\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetCamContrast(value) != 0){
				printf("Fail at SYS_MSG_SET_CONTRAST\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SATURATION:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_SATURATION\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetCamSaturation(value) != 0){
				printf("Fail at SYS_MSG_SET_SATURATION\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SHARPNESS:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_SHARPNESS\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetCamSharpness(value) != 0){
				printf("Fail at SYS_MSG_SET_SHARPNESS\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_CLEARSYSLOG:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_CLEARSYSLOG\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetClearSysLog(value) != 0){
				printf("Fail at SYS_MSG_SET_CLEARSYSLOG\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_CLEARACCESSLOG:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_CLEARACCESSLOG\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetClearAccessLog(value) != 0){
				printf("Fail at SYS_MSG_SET_CLEARACCESSLOG\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_VSTAB:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_VSTAB\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetVstabValue(value) != 0){
				printf("Fail at SYS_MSG_SET_VSTAB\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_LDC:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_LDC\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetLdcValue(value) != 0){
				printf("Fail at SYS_MSG_SET_LDC\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MAXEXPOSURE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MAXEXPOSURE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMaxExposureValue(value) != 0){
				printf("Fail at SYS_MSG_SET_MAXEXPOSURE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MAXGAIN:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MAXGAIN\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMaxGainValue(value) != 0){
				printf("Fail at SYS_MSG_SET_MAXGAIN\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SNF:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_SNF\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetSnfValue(value) != 0){
				printf("Fail at SYS_MSG_SET_SNF\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TNF:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TNF\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTnfValue(value) != 0){
				printf("Fail at SYS_MSG_SET_TNF\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_AUDIOMODE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_AUDIOMODE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAudioMode(value) != 0){
				printf("Fail at SYS_MSG_SET_AUDIOMODE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_AUDIO_ENCODE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_AUDIO_ENCODE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAudioEncode(value) != 0){
				printf("Fail at SYS_MSG_SET_AUDIO_ENCODE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_AUDIO_SAMPLERATE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_AUDIO_SAMPLERATE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAudioSampleRate(value) != 0){
				printf("Fail at SYS_MSG_SET_AUDIO_SAMPLERATE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_AUDIO_BITRATE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_AUDIO_BITRATE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAudioBitrate(value) != 0){
				printf("Fail at SYS_MSG_SET_AUDIO_BITRATE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_AUDIO_ALARMLEVEL:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_AUDIO_ALARMLEVEL\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAudioAlarmLevel(value) != 0){
				printf("Fail at SYS_MSG_SET_AUDIO_ALARMLEVEL\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_AUDIOOUTVOLUME:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_AUDIOOUTVOLUME\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAudioOutVolume(value) != 0){
				printf("Fail at SYS_MSG_SET_AUDIOOUTVOLUME\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DEFAULT:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_DEFAULT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetSystemDefault(value) != 0){
				printf("Fail at SYS_MSG_SET_DEFAULT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_JPEG_QUALITY:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_JPEG_QUALITY\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMJPEGQuality(value) != 0){
				printf("Fail at SYS_MSG_SET_JPEG_QUALITY\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MIRROR:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MIRROR\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMirror(value) != 0){
				printf("Fail at SYS_MSG_SET_MIRROR\n");
				break;
			}
			ret = 1;
			break;
		}

		case SYS_MSG_SET_ADVMODE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_ADVMODE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAdvMode(value) != 0){
				printf("Fail at SYS_MSG_SET_ADVMODE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DEMOCFG:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_DEMOCFG\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetDemoCfg(value) != 0){
				printf("Fail at SYS_MSG_SET_DEMOCFG\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_EXPPRIORITY:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_EXPPRIORITY\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetExpPriority(value) != 0){
				printf("Fail at SYS_MSG_SET_EXPPRIORITY\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_OSDWIN:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_OSDWIN\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetOSDWin(value) != 0){
				printf("Fail at SYS_MSG_SET_OSDWIN\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_ALARMLOCAL:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_ALARMLOCAL\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAlarmStorage(value) != 0){
				printf("Fail at SYS_MSG_SET_ALARMLOCAL\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_RECORDLOCAL:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_RECORDLOCAL\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetRecordStorage(value) != 0){
				printf("Fail at SYS_MSG_SET_RECORDLOCAL\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_CLICK_SNAP_STORAGE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_CLICK_SNAP_STORAGE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetClickSnapStorage(value) != 0){
				printf("Fail at SYS_MSG_SET_CLICK_SNAP_STORAGE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_CLICK_SNAP_FILENAME:
		{
			DBG("SYS_MSG_SET_CLICK_SNAP_FILENAME\n");
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetClickSnapFilename(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_CLICK_SNAP_FILENAME\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_RATE_CONTROL1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_RATE_CONTROL1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetRateControl1(value) != 0){
				printf("Fail at SYS_MSG_SET_RATE_CONTROL1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_RATE_CONTROL2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_RATE_CONTROL2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetRateControl2(value) != 0){
				printf("Fail at SYS_MSG_SET_RATE_CONTROL2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DATE_STAMP_ENABLE1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_DATE_STAMP_ENABLE1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetDateStampEnable1(value) != 0){
				printf("Fail at SYS_MSG_SET_DATE_STAMP_ENABLE1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DATE_STAMP_ENABLE2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_DATE_STAMP_ENABLE2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetDateStampEnable2(value) != 0){
				printf("Fail at SYS_MSG_SET_DATE_STAMP_ENABLE2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DATE_STAMP_ENABLE3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_DATE_STAMP_ENABLE3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetDateStampEnable3(value) != 0){
				printf("Fail at SYS_MSG_SET_DATE_STAMP_ENABLE3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TIME_STAMP_ENABLE1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TIME_STAMP_ENABLE1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTimeStampEnable1(value) != 0){
				printf("Fail at SYS_MSG_SET_TIME_STAMP_ENABLE1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TIME_STAMP_ENABLE2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TIME_STAMP_ENABLE2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTimeStampEnable2(value) != 0){
				printf("Fail at SYS_MSG_SET_TIME_STAMP_ENABLE2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TIME_STAMP_ENABLE3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TIME_STAMP_ENABLE3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTimeStampEnable3(value) != 0){
				printf("Fail at SYS_MSG_SET_TIME_STAMP_ENABLE3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_LOGO_ENABLE1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_LOGO_ENABLE1\n");
//			printf("\r\nDEBUG: Inside main.c   SYS_MSG_SET_LOGO_ENABLE1--------------------------------\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetLogoEnable1(value) != 0){
				printf("Fail at SYS_MSG_SET_LOGO_ENABLE1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_LOGO_ENABLE2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_LOGO_ENABLE2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetLogoEnable2(value) != 0){
				printf("Fail at SYS_MSG_SET_LOGO_ENABLE2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_LOGO_ENABLE3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_LOGO_ENABLE3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetLogoEnable3(value) != 0){
				printf("Fail at SYS_MSG_SET_LOGO_ENABLE3\n");
				break;
			}
			ret = 1;
			break;
		}
              case SYS_MSG_SET_LOGO_POSITION1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_LOGO_POSITION1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetLogoPosition1(value) != 0){
				printf("Fail at SYS_MSG_SET_LOGO_POSITION1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_LOGO_POSITION2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_LOGO_POSITION2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetLogoPosition2(value) != 0){
				printf("Fail at Fail at  SYS_MSG_SET_LOGO_POSITION2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_LOGO_POSITION3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_LOGO_POSITION3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetLogoPosition3(value) != 0){
				printf("Fail at  SYS_MSG_SET_LOGO_POSITION3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TEXT_ENABLE1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TEXT_ENABLE1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTextEnable1(value) != 0){
				printf("Fail at SYS_MSG_SET_TEXT_ENABLE1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TEXT_ENABLE2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TEXT_ENABLE2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTextEnable2(value) != 0){
				printf("Fail at SYS_MSG_SET_TEXT_ENABLE2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TEXT_ENABLE3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TEXT_ENABLE3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTextEnable3(value) != 0){
				printf("Fail at SYS_MSG_SET_TEXT_ENABLE3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_OVERLAY_TEXT1:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("SYS_MSG_SET_OVERLAY_TEXT1 :String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetOverlayText1(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_OVERLAY_TEXT1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_OVERLAY_TEXT2:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("SYS_MSG_SET_OVERLAY_TEXT2 :String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetOverlayText2(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_OVERLAY_TEXT2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_OVERLAY_TEXT3:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("SYS_MSG_SET_OVERLAY_TEXT3 :String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetOverlayText3(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_OVERLAY_TEXT3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TEXT_POSITION1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TEXT_POSITION1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTextPosition1(value) != 0){
				printf("Fail at SYS_MSG_SET_TEXT_POSITION1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TEXT_POSITION2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TEXT_POSITION2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTextPosition2(value) != 0){
				printf("Fail at SYS_MSG_SET_TEXT_POSITION2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TEXT_POSITION3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TEXT_POSITION3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTextPosition3(value) != 0){
				printf("Fail at SYS_MSG_SET_TEXT_POSITION3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_ENCRYPT_VIDEO:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_ENCRYPT_VIDEO\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetEncryptVideo(value) != 0){
				printf("Fail at SYS_MSG_SET_ENCRYPT_VIDEO\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_LOCAL_DISPLAY:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_LOCAL_DISPLAY\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetLocalDisplay(value) != 0){
				printf("Fail at SYS_MSG_SET_LOCAL_DISPLAY\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_IPRATIO1:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_IPRATIO1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			value = 100;
			if(SetIpratio1(value) != 0){
				printf("Fail at SYS_MSG_SET_IPRATIO1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_IPRATIO2:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_IPRATIO2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			value = 100;
			if(SetIpratio2(value) != 0){
				printf("Fail at SYS_MSG_SET_IPRATIO2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_IPRATIO3:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_IPRATIO3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetIpratio3(value) != 0){
				printf("Fail at SYS_MSG_SET_IPRATIO3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FORCE_IFRAME1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FORCE_IFRAME1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetForceIframe1(value) != 0){
				printf("Fail at SYS_MSG_SET_FORCE_IFRAME1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FORCE_IFRAME2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FORCE_IFRAME2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetForceIframe2(value) != 0){
				printf("Fail at SYS_MSG_SET_FORCE_IFRAME2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FORCE_IFRAME3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FORCE_IFRAME3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetForceIframe3(value) != 0){
				printf("Fail at SYS_MSG_SET_FORCE_IFRAME3\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_QPINIT1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_QPINIT1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetQPInit1(value) != 0){
				printf("Fail at SYS_MSG_SET_QPINIT1\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_QPINIT2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_QPINIT2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetQPInit2(value) != 0){
				printf("Fail at SYS_MSG_SET_QPINIT2\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_QPINIT3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_QPINIT3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetQPInit3(value) != 0){
				printf("Fail at SYS_MSG_SET_QPINIT3\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_QPMIN1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_QPMIN1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetQPMin1(value) != 0){
				printf("Fail at SYS_MSG_SET_QPMIN1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_QPMIN2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_QPMIN2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetQPMin2(value) != 0){
				printf("Fail at SYS_MSG_SET_QPMIN2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_QPMIN3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_QPMIN3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetQPMin3(value) != 0){
				printf("Fail at SYS_MSG_SET_QPMIN3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_QPMAX1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_QPMAX1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetQPMax1(value) != 0){
				printf("Fail at SYS_MSG_SET_QPMAX1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_QPMAX2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_QPMAX2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetQPMax2(value) != 0){
				printf("Fail at SYS_MSG_SET_QPMAX2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_QPMAX3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_QPMAX3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetQPMax3(value) != 0){
				printf("Fail at SYS_MSG_SET_QPMAX3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MECONFIG1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MECONFIG1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMEConfig1(value) != 0){
				printf("Fail at SYS_MSG_SET_MECONFIG1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MECONFIG2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MECONFIG2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMEConfig2(value) != 0){
				printf("Fail at SYS_MSG_SET_MECONFIG2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MECONFIG3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MECONFIG3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMEConfig3(value) != 0){
				printf("Fail at SYS_MSG_SET_MECONFIG3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_PACKET_SIZE1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_PACKET_SIZE1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetPacketSize1(value) != 0){
				printf("Fail at SYS_MSG_SET_PACKET_SIZE1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_PACKET_SIZE2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_PACKET_SIZE2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetPacketSize2(value) != 0){
				printf("Fail at SYS_MSG_SET_PACKET_SIZE2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_PACKET_SIZE3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_PACKET_SIZE3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetPacketSize3(value) != 0){
				printf("Fail at SYS_MSG_SET_PACKET_SIZE3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_ROI_ENABLE1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_ROI_ENABLE1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetROIEnable1(value) != 0){
				printf("Fail at SYS_MSG_SET_ROI_ENABLE1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_ROI_ENABLE2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_ROI_ENABLE2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetROIEnable2(value) != 0){
				printf("Fail at SYS_MSG_SET_ROI_ENABLE2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_ROI_ENABLE3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_ROI_ENABLE3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetROIEnable3(value) != 0){
				printf("Fail at SYS_MSG_SET_ROI_ENABLE3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR1X1:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR1X1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr1X1(value) != 0){
				printf("Fail at SYS_MSG_SET_STR1X1\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_STR1Y1:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR1Y1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr1Y1(value) != 0){
				printf("Fail at SYS_MSG_SET_STR1Y1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR1W1:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR1W1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr1W1(value) != 0){
				printf("Fail at SYS_MSG_SET_STR1W1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR1H1:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR1H1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr1H1(value) != 0){
				printf("Fail at SYS_MSG_SET_STR1H1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR1X2:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR1X2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr1X2(value) != 0){
				printf("Fail at SYS_MSG_SET_STR1X2\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_STR1Y2:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR1Y2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr1Y2(value) != 0){
				printf("Fail at SYS_MSG_SET_STR1Y2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR1W2:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR1W2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr1W2(value) != 0){
				printf("Fail at SYS_MSG_SET_STR1W2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR1H2:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR1H2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr1H2(value) != 0){
				printf("Fail at SYS_MSG_SET_STR1H2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR1X3:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR1X3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr1X3(value) != 0){
				printf("Fail at SYS_MSG_SET_STR1X3\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_STR1Y3:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR1Y3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr1Y3(value) != 0){
				printf("Fail at SYS_MSG_SET_STR1Y3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR1W3:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR1W3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr1W3(value) != 0){
				printf("Fail at SYS_MSG_SET_STR1W3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR1H3:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR1H3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr1H3(value) != 0){
				printf("Fail at SYS_MSG_SET_STR1H3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR2X1:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR2X1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr2X1(value) != 0){
				printf("Fail at SYS_MSG_SET_STR2X1\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_STR2Y1:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR2Y1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr2Y1(value) != 0){
				printf("Fail at SYS_MSG_SET_STR2Y1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR2W1:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR2W1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr2W1(value) != 0){
				printf("Fail at SYS_MSG_SET_STR2W1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR2H1:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR2H1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr2H1(value) != 0){
				printf("Fail at SYS_MSG_SET_STR2H1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR2X2:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR2X2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr2X2(value) != 0){
				printf("Fail at SYS_MSG_SET_STR2X2\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_STR2Y2:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR2Y2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr2Y2(value) != 0){
				printf("Fail at SYS_MSG_SET_STR2Y2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR2W2:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR2W2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr2W2(value) != 0){
				printf("Fail at SYS_MSG_SET_STR2W2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR2H2:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR2H2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr2H2(value) != 0){
				printf("Fail at SYS_MSG_SET_STR2H2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR2X3:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR2X3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr2X3(value) != 0){
				printf("Fail at SYS_MSG_SET_STR2X3\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_STR2Y3:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR2Y3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr2Y3(value) != 0){
				printf("Fail at SYS_MSG_SET_STR2Y3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR2W3:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR2W3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr2W3(value) != 0){
				printf("Fail at SYS_MSG_SET_STR2W3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR2H3:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR2H3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr2H3(value) != 0){
				printf("Fail at SYS_MSG_SET_STR2H3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR3X1:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR3X1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr3X1(value) != 0){
				printf("Fail at SYS_MSG_SET_STR3X1\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_STR3Y1:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR3Y1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr3Y1(value) != 0){
				printf("Fail at SYS_MSG_SET_STR3Y1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR3W1:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR3W1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr3W1(value) != 0){
				printf("Fail at SYS_MSG_SET_STR3W1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR3H1:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR3H1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr3H1(value) != 0){
				printf("Fail at SYS_MSG_SET_STR3H1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR3X2:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR3X2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr3X2(value) != 0){
				printf("Fail at SYS_MSG_SET_STR3X2\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_STR3Y2:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR3Y2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr3Y2(value) != 0){
				printf("Fail at SYS_MSG_SET_STR3Y2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR3W2:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR3W2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr3W2(value) != 0){
				printf("Fail at SYS_MSG_SET_STR3W2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR3H2:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR3H2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr3H2(value) != 0){
				printf("Fail at SYS_MSG_SET_STR3H2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR3X3:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR3X3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr3X3(value) != 0){
				printf("Fail at SYS_MSG_SET_STR3X3\n");
				break;
			}
			ret = 1;
			break;
		}
        case SYS_MSG_SET_STR3Y3:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR3Y3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr3Y3(value) != 0){
				printf("Fail at SYS_MSG_SET_STR3Y3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR3W3:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR3W3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr3W3(value) != 0){
				printf("Fail at SYS_MSG_SET_STR3W3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_STR3H3:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_STR3H3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetStr3H3(value) != 0){
				printf("Fail at SYS_MSG_SET_STR3H3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DETAIL_INFO1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_DETAIL_INFO1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetDetailInfo1(value) != 0){
				printf("Fail at SYS_MSG_SET_DETAIL_INFO1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DETAIL_INFO2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_DETAIL_INFO2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetDetailInfo2(value) != 0){
				printf("Fail at SYS_MSG_SET_DETAIL_INFO2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DETAIL_INFO3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_DETAIL_INFO3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetDetailInfo3(value) != 0){
				printf("Fail at SYS_MSG_SET_DETAIL_INFO3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_HISTOGRAM:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_HISTOGRAM\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetHistogram(value) != 0){
				printf("Fail at SYS_MSG_SET_HISTOGRAM\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DYNRANGE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_DYNRANGE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetDynRange(value) != 0){
				printf("Fail at SYS_MSG_SET_DYNRANGE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_OSDSTREAM:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_OSDSTREAM\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetOSDStream(value) != 0){
				printf("Fail at SYS_MSG_SET_OSDSTREAM\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_OSDWINNUM:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_OSDWINNUM\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetOSDWinNum(value) != 0){
				printf("Fail at SYS_MSG_SET_OSDWINNUM\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_VIDEO_MODE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_VIDEO_MODE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);

			int server_status = SetVideoMode(value);

			if(server_status < 0){
				printf("\nSystemServer:Fail at SYS_MSG_SET_VIDEO_MODE\n");
				break;
			}else if(server_status == 1){
				pMsg->length = 0xFF;
				ret = 1;
			}else{
				ret = 1;
			}
			break;
		}
		case SYS_MSG_SET_VIDEOCODEC_COMBO:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_VIDEOCODEC_COMBO\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);

			int server_status = SetVideoCodecCombo(value);

			if(server_status < 0){
				printf("\nSystemServer:Fail at SYS_MSG_SET_VIDEOCODEC_COMBO\n");
				break;
			}else if(server_status == 1){
				pMsg->length = 0xFF;
				ret = 1;
			}else{
				ret = 1;
			}
			break;
		}
		case SYS_MSG_SET_VIDEOCODEC_RES:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_VIDEOCODEC_RES\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			int server_status = SetVideoCodecRes(value);
			if( server_status < 0){
				printf("\nSystemServer:Fail at SYS_MSG_SET_VIDEOCODEC_RES\n");
				break;
			}else if(server_status == 1){
				pMsg->length = 0xFF;
				ret = 1;
			}else{
				ret = 1;
			}
			break;
		}
		case SYS_MSG_SET_IMAGE_SOURCE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_IMAGE_SOURCE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetImagesource(value) != 0){
				printf("Fail at SYS_MSG_SET_IMAGE_SOURCE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_IMAGE_FORMAT:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_IMAGE_FORMAT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetImageFormat(value) != 0){
				printf("Fail at SYS_MSG_SET_IMAGE_FORMAT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_RESOLUTION:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_RESOLUTION\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetResolution(value) != 0){
				printf("Fail at SYS_MSG_SET_RESOLUTION\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MPEG4_RES:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MPEG4_RES\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMPEG4Res(value) != 0){
				printf("Fail at SYS_MSG_SET_MPEG4_RES\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SCHEDULE:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetSchedule(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_SCHEDULE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MPEG42_RES:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MPEG42_RES\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMPEG42Res(value) != 0){
				printf("Fail at SYS_MSG_SET_MPEG42_RES\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_DEL_SCHEDULE:
		{
			int bEnable;
			DBG("SYS_MSG_DEL_SCHEDULE\n");
			if(pMsg->length != sizeof(bEnable))
				break;
			ShareMemRead(pMsg->offset, &bEnable, pMsg->length);
			if(DelSchedule(bEnable) != 0){
				printf("Fail at SYS_MSG_DEL_SCHEDULE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_AVIDURATION:
		{
			__u8 value;
			DBG("SYS_MSG_SET_AVIDURATION\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAVIDuration(value) != 0){
				printf("Fail at SYS_MSG_SET_AVIDURATION\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_AVIFORMAT:
		{
			__u8 value;
			DBG("SYS_MSG_SET_AVIFORMAT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAVIFormat(value) != 0){
				printf("Fail at SYS_MSG_SET_AVIFORMAT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FTPFILEFORMAT:
		{
			__u8 value;
			DBG("SYS_MSG_SET_FTPFILEFORMAT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFTPFileFormat(value) != 0){
				printf("Fail at SYS_MSG_SET_FTPFILEFORMAT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SDFILEFORMAT:
		{
			__u8 value;
			DBG("SYS_MSG_SET_SDFILEFORMAT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetSDFileFormat(value) != 0){
				printf("Fail at SYS_MSG_SET_SDFILEFORMAT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_ATTFILEFORMAT:
		{
			__u8 value;
			DBG("SYS_MSG_SET_ATTFILEFORMAT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAttFileFormat(value) != 0){
				printf("Fail at SYS_MSG_SET_ATTFILEFORMAT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_AUDIOENABLE:
		{
			__u8 value;
			DBG("SYS_MSG_SET_AUDIOENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAudioON(value) != 0){
				printf("Fail at SYS_MSG_SET_AUDIOENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_ASMTPATTACH:
		{
			__u8 value;
			DBG("SYS_MSG_SET_ASMTPATTACH\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetASmtpAttach(value) != 0){
				printf("Fail at SYS_MSG_SET_ASMTPATTACH\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FTP_RFTPENABLE:
		{
			__u8 value;
			DBG("SYS_MSG_SET_FTP_RFTPENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetRftpenable(value) != 0){
				printf("Fail at SYS_MSG_SET_FTP_RFTPENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SD_REENABLE:
		{
			__u8 value;
			DBG("SYS_MSG_SET_SD_REENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetSdReEnable(value) != 0){
				printf("Fail at SYS_MSG_SET_SD_REENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_LOSTALARM:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_LOSTALARM\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetLostAlarm(value) != 0){
				printf("Fail at SYS_MSG_SET_LOSTALARM\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SDAENABLE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_SDAENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetSDAlarmEnable(value) != 0){
				printf("Fail at SYS_MSG_SET_SDAENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MOTIONENABLE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MOTIONENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMotionDetect(value) != 0){
				printf("Fail at SYS_MSG_SET_MOTIONENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MOTIONCENABLE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MOTIONCENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMotionDetCEnable(value) != 0){
				printf("Fail at SYS_MSG_SET_MOTIONCENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MOTIONLEVEL:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MOTIONLEVEL\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMotionDetLevel(value) != 0){
				printf("Fail at SYS_MSG_SET_MOTIONLEVEL\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MOTIONCVALUE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MOTIONCVALUE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMotionDetCValue(value) != 0){
				printf("Fail at SYS_MSG_SET_MOTIONCVALUE\n");
				break;
			}
			ret = 1;
			break;
		}

		case SYS_MSG_SET_MOTIONBLOCK:
		{
			__u8 tempBuff[MOTION_BLK_LEN];
			int i;
			DBG("SYS_MSG_SET_MOTIONBLOCK\n");
			if(pMsg->length >  MOTION_BLK_LEN) {
				printf("Fail at SYS_MSG_SET_MOTIONBLOCK: %d %d\n",pMsg->length, MOTION_BLK_LEN);
				break;
			}
			ShareMemRead(pMsg->offset, tempBuff, pMsg->length);
			if(SetMotionDetectBlock((char *)tempBuff,pMsg->length) != 0){
				for(i =0;i<pMsg->length;i++){
					printf("%x,", tempBuff[i]);
				}
				printf("Fail at SYS_MSG_SET_MOTIONBLOCK\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FDETECT:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FDETECT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFaceDetect(value) != 0){
				printf("Fail at SYS_MSG_SET_FDETECT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FDX:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_FDX\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFDX(value) != 0){
				printf("Fail at SYS_MSG_SET_FDX\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FDY:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_FDY\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFDY(value) != 0){
				printf("Fail at SYS_MSG_SET_FDY\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FDW:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_FDW\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFDW(value) != 0){
				printf("Fail at SYS_MSG_SET_FDW\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FDH:
		{
			unsigned int value;
			DBG("SYS_MSG_SET_FDH\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFDH(value) != 0){
				printf("Fail at SYS_MSG_SET_FDH\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FDCONF_LEVEL:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FDCONF_LEVEL\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFDConfLevel(value) != 0){
				printf("Fail at SYS_MSG_SET_FDCONF_LEVEL\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FD_DIRECTION:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FD_DIRECTION\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFDDirection(value) != 0){
				printf("Fail at SYS_MSG_SET_FD_DIRECTION\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FRECOGNITION:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FRECOGNITION\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFRecognition(value) != 0){
				printf("Fail at SYS_MSG_SET_FRECOGNITION\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FRCONF_LEVEL:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FRCONF_LEVEL\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFRConfLevel(value) != 0){
				printf("Fail at SYS_MSG_SET_FRCONF_LEVEL\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FR_DATABASE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FR_DATABASE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFRDatabase(value) != 0){
				printf("Fail at SYS_MSG_SET_FR_DATABASE\n");
				break;
			}
			ret = 1;
			break;
		}
	    case SYS_MSG_SET_PRIVACY_MASK:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_PRIVACY_MASK\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetPrivacyMask(value) != 0){
				printf("Fail at SYS_MSG_SET_PRIVACY_MASK\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MASK_OPTIONS:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MASK_OPTIONS\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMaskOptions(value) != 0){
				printf("Fail at SYS_MSG_SET_MASK_OPTIONS\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FTPAENABLE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FTPAENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFTPAlarmEnable(value) != 0){
				printf("Fail at SYS_MSG_SET_FTPAENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SMTPAENABLE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_SMTPAENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetSMTPAlarmEnable(value) != 0){
				printf("Fail at SYS_MSG_SET_SMTPAENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_2A_SWITCH:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_2A_SWITCH\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			int server_status = SetImageAEW(value);
			if(server_status < 0){
				printf("Fail at SYS_MSG_SET_2A_SWITCH\n");
				break;
			}else if(server_status == 1){
				pMsg->length = 0xFF;
				ret = 1;
			}else{
				ret = 1;
			}
			break;
		}
		case SYS_MSG_SET_2A_TYPE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_2A_TYPE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetImageAEWType(value) != 0){
				printf("Fail at SYS_MSG_SET_2A_TYPE\n");
				break;
			}
			ret = 1;
			break;
		}

		case SYS_MSG_SET_BINNING:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_BINNING\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			int server_status = SetBinning(value);
			if(server_status < 0){
				printf("Fail at SYS_MSG_SET_BINNING\n");
				break;
			}else if(server_status == 1){
				pMsg->length = 0xFF;
				ret = 1;
			}else{
				ret = 1;
			}
			break;
		}
		case SYS_MSG_SET_BLC:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_BLC\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetBlc(value) != 0){
				printf("Fail at SYS_MSG_SET_BLC\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_ALARMDURATION:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_ALARMDURATION\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAlarmDuration(value) != 0){
				printf("Fail at SYS_MSG_SET_ALARMDURATION\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MPEG41_BITRATE:
		{
			int value;
			DBG("SYS_MSG_SET_MPEG41_BITRATE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMP41bitrate(value) != 0){
				printf("Fail at SYS_MSG_SET_MPEG41_BITRATE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MPEG42_BITRATE:
		{
			int value;
			DBG("SYS_MSG_SET_MPEG42_BITRATE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetMP42bitrate(value) != 0){
				printf("Fail at SYS_MSG_SET_MPEG42_BITRATE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_DEL_USER:
		{
			DBG("SYS_MSG_DEL_USER\n");
			if(pMsg->length > USER_LEN){
				ERR("Invalid user name\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(buffer[pMsg->length - 1] != '\0'){
				if(USER_LEN == pMsg->length){
					ERR("String isn't null terminated\n");
					break;
				} else
					buffer[pMsg->length] = '\0';
			}
			if(DelUser(buffer) != 0){
				ERR("Fail at SYS_MSG_DEL_USER\n");
				break;
			}
			pMsg->length = 0;
			ret = 1;
			break;
		}
		case SYS_MSG_ADD_USER:
		{
			Acount_t value;
			DBG("SYS_MSG_ADD_USER\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			DBG("User:%s\n", value.user);
			DBG("Password:%s\n", value.password);
			DBG("authority:%d\n", value.authority);
			if(AddUser(&value) != 0){
				printf("Fail at SYS_MSG_ADD_USER\n");
				break;
			}
			pMsg->length = 0;
			ret = 1;
			break;
		}
		case SYS_MSG_SET_GIOINENABLE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_GIOINENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetGIOInEnable(value) != 0){
				printf("Fail at SYS_MSG_SET_GIOINENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_GIOINTYPE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_GIOINTYPE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetGIOInType(value) != 0){
				printf("Fail at SYS_MSG_SET_GIOINTYPE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_GIOOUTENABLE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_GIOOUTENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetGIOOutEnable(value) != 0){
				printf("Fail at SYS_MSG_SET_GIOOUTENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_GIOOUTTYPE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_GIOOUTTYPE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetGIOOutType(value) != 0){
				printf("Fail at SYS_MSG_SET_GIOOUTTYPE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TSTAMPENABLE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TSTAMPENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTStampEnable(value) != 0){
				printf("Fail at SYS_MSG_SET_TSTAMPENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DATEFORMAT:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_DATEFORMAT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetDateFormat(value) != 0){
				printf("Fail at SYS_MSG_SET_DATEFORMAT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TSTAMPFORMAT:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TSTAMPFORMAT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTStampFormat(value) != 0){
				printf("Fail at SYS_MSG_SET_TSTAMPFORMAT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_DATEPPOS:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_DATEPPOS\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetDatePosition(value) != 0){
				printf("Fail at SYS_MSG_SET_DATEPPOS\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_TIMEPOS:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_TIMEPOS\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetTimePosition(value) != 0){
				printf("Fail at SYS_MSG_SET_TIMEPOS\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_AUDIOINVOLUME:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_AUDIOINVOLUME\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAudioInVolume(value) != 0){
				printf("Fail at SYS_MSG_SET_AUDIOINVOLUME\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_IMAGE_DEFAULT:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_IMAGE_DEFAULT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetImageParaDefault(value) != 0){
				printf("Fail at SYS_MSG_SET_IMAGE_DEFAULT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_DO_LOGIN:
		{
			login_data_t	prm;
			DBG("SYS_MSG_DO_LOGIN\n");
			if(pMsg->length != sizeof(prm))
				break;
			ShareMemRead(pMsg->offset, &prm, pMsg->length);
			if(DoLogin(&prm))
				break;
			ret = 1;
			break;
		}
		case SYS_MSG_SD_FORMAT:
		{
			int prm;
			DBG("SYS_MSG_SD_FORMAT\n");
			if(pMsg->length != sizeof(prm))
				break;
			ShareMemRead(pMsg->offset, &prm, pMsg->length);
			if(prm != 1)
				break;
			if(SdFormat())
				break;
			ret = 1;
			break;
		}
		case SYS_MSG_SD_UNMOUNT:
		{
			unsigned char value;
			DBG("SYS_MSG_SD_UNMOUNT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SdUnmount(value) != 0){
				printf("\nSystemServer:Fail at SYS_MSG_SD_UNMOUNT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_GET_USER_AUTHORITY:
		{
			get_user_authority_t	prm;
			DBG("SYS_MSG_GET_USER_AUTHORITY\n");
			if(pMsg->length != sizeof(prm))
				break;
			ShareMemRead(pMsg->offset, &prm, pMsg->length);
			prm.authority= GetUserAuthority(prm.user_id);
			ShareMemWrite(pMsg->offset, &prm, pMsg->length);
			ret = 1;
			break;
		}

		case SYS_MSG_SCHEDULE_STOP:
		{
			ScheduleStop();
			DBG("SYS_MSG_SCHEDULE_STOP\n");
			ret = 1;
			break;
		}

		case SYS_MSG_SCHEDULE_RESUME:
		{
			ResumeSchedule();
			DBG("SYS_MSG_SCHEDULE_RESUME\n");
			ret = 1;
			break;
		}

		case SYS_MSG_QUIT:
		{
			IsSysQuit = 1;
			pMsg->src = 0;
			break;
		}


		case SYS_MSG_IPNCPTZ:
		{
			Ptz_Data value;
			char ptz_command[10];
			char *a = "zoomin";
			char *b = "zoomou";
			char *c = "up%000";
			char *d = "down%0";
			char *e = "left%0";
			int ptz_command_num;


			DBG("SYS_MSG_IPNCPTZ\n");

			ShareMemRead(pMsg->offset, &value, pMsg->length);

			strncpy(ptz_command, value.ptzdata, 6);
			if(strncmp(ptz_command, a, 6) == 0)
			{
				DBG("ZOOMIN!!\n");
				ptz_command_num = 0;
			}else if(strncmp(ptz_command, b, 6) == 0){
				DBG("ZOOMOUT!!\n");
				ptz_command_num = 1;
			}else if(strncmp(ptz_command, c, 6) == 0){
				DBG("UP!!\n");
				ptz_command_num = 2;
			}else if(strncmp(ptz_command, d, 6) == 0){
				DBG("DOWN!!\n");
				ptz_command_num = 3;
			}else if(strncmp(ptz_command, e, 6) == 0){
				DBG("LEFT!!\n");
				ptz_command_num = 4;
			}else{
				DBG("RIGHT!!\n");
				ptz_command_num = 5;
			}

			if(SetCamPtz(ptz_command_num) != 0){
				printf("Fail at SYS_MSG_IPNCPTZ\n");
				break;
			}

			pMsg->length = 0;
			ret = 1;
			break;
		}

		case SYS_MSG_INIT_IPNCPTZ:
		{
			int ptz_command_num;

			ptz_command_num = 6;
			SetCamPtz(ptz_command_num);

			break;
		}
		case SYS_MSG_SET_RATE_CONTROL:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_RATE_CONTROL\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			int server_status = SetRateControl(value);
			if(server_status < 0){
				printf("Fail at SYS_MSG_SET_RATE_CONTROL\n");
				break;
			}else if(server_status == 1){
				pMsg->length = 0xFF;
				ret = 1;
			}else{
				ret = 1;
			}
			break;
		}
		case SYS_MSG_SET_FRAMERATE1:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FRAMERATE1\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFramerate1(value) != 0){
				printf("Fail at SYS_MSG_SET_FRAMERATE1\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FRAMERATE2:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FRAMERATE2\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFramerate2(value) != 0){
				printf("Fail at SYS_MSG_SET_FRAMERATE2\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FRAMERATE3:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FRAMERATE3\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFramerate3(value) != 0){
				printf("Fail at SYS_MSG_SET_FRAMERATE3\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_MULTICAST:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_MULTICAST\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetNetMulticast(value) != 0){
				printf("\nSystemServer:Fail at SYS_MSG_SET_MULTICAST\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_DO_BOOT_PROC:
		{
			int value;
			DBG("SYS_MSG_DO_BOOT_PROC\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(DoBootProc(value) != 0){
				printf("\nSystemServer:Fail at SYS_MSG_DO_BOOT_PROC\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_HTTPSPORT:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_HTTPSPORT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetHttpsPort(value) != 0){
				printf("\nSystemServer:Fail at SYS_MSG_SET_HTTPSPORT\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_RS485:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_RS485\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetRS485Port(value) != 0){
				printf("\nSystemServer:Fail at SYS_MSG_SET_RS485\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_ALARM_ENABLE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_ALARM_ENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAlarmEnable(value) != 0){
				printf("Fail at SYS_MSG_SET_ALARM_ENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_EXT_ALARM:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_EXT_ALARM\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(Avs_SetExtAlarm(value) != 0){
				printf("Fail at SYS_MSG_SET_EXT_ALARM\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_AUDIOALARM:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_AUDIOALARM\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(Avs_SetAudioAlarm(value) != 0){
				printf("Fail at SYS_MSG_SET_AUDIOALARM\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_ALARM_AUDIOPLAY:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_ALARM_AUDIOPLAY\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(Avs_SetAlarmAudioPlay(value) != 0){
				printf("Fail at SYS_MSG_SET_ALARM_AUDIOPLAY\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_ALARM_AUDIOFILE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_ALARM_AUDIOFILEY\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(Avs_SetAlarmAudioFile(value) != 0){
				printf("Fail at SYS_MSG_SET_ALARM_AUDIOFILE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SCHEDULE_REPEAT_ENABLE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_SCHEDULE_REPEAT_ENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(Avs_SetScheduleRepeatEnable(value) != 0){
				printf("Fail at SYS_MSG_SET_SCHEDULE_REPEAT_ENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SCHEDULE_NUM_WEEKS:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_SCHEDULE_NUM_WEEKS\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(Avs_SetScheduleNumWeeks(value) != 0){
				printf("Fail at SYS_MSG_SET_SCHEDULE_NUM_WEEKS\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_SCHEDULE_INFINITE_ENABLE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_SCHEDULE_INFINITE_ENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(Avs_SetScheduleInfiniteEnable(value) != 0){
				printf("Fail at SYS_MSG_SET_SCHEDULE_INFINITE_ENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_AUDIORECEIVER_ENABLE:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_AUDIORECEIVER_ENABLE\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetAudioReceiverEnable(value) != 0){
				printf("Fail at SYS_MSG_SET_AUDIORECEIVER_ENABLE\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_AUDIOSERVER_IP:
		{
			if(pMsg->length > SYSTEM_SERVER_BUFFER_LENGTH){
				printf("SYS_MSG_SET_AUDIOSERVER_IP :String length bigger then System Server Buffer\n");
				break;
			}
			ShareMemRead(pMsg->offset, buffer, pMsg->length);
			if(SetAudioSeverIp(buffer, pMsg->length) != 0){
				printf("Fail at SYS_MSG_SET_AUDIOSERVER_IP\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_LINEAR_WDR:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_LINEAR_WDR\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetLinearWdr(value) != 0){
				printf("Fail at SYS_MSG_SET_LINEAR_WDR\n");
				break;
			}
			ret = 1;
			break;
		}
		case SYS_MSG_SET_FLICKER_DETECT:
		{
			unsigned char value;
			DBG("SYS_MSG_SET_FLICKER_DETECT\n");
			if(pMsg->length != sizeof(value))
				break;
			ShareMemRead(pMsg->offset, &value, pMsg->length);
			if(SetFlickerDetect(value) != 0){
				printf("Fail at SYS_MSG_SET_FLICKER_DETECT\n");
				break;
			}
			ret = 1;
			break;
		}


		default:
			printf("Unknown message. Quit\n");
			IsSysQuit = 1;
			pMsg->src = 0;
			break;
	}
	if(ret == 2){
		pMsg->offset = (pMsg->src - MSG_TYPE_MSG1 - 1) * PROC_MEM_SIZE;
		ShareMemWrite(pMsg->offset, pData, pMsg->length);
		ret = 1;
	}
	return ret;
}

static int gSysMsgId;

/**
 * @brief	Set system message id
 * @param	"int qid"	[IN]queue id
 * @return	void
 */
void SetSysMsgId(int qid)
{
	gSysMsgId = qid;
}

/**
 * @brief	Get system message id
 * @param	none
 * @return	gSysMsgId
 */
int GetSysMsgId()
{
	return gSysMsgId;
}

/**
 * @brief	Clean system message queue
 * @param	none
 * @return	void
 */
void CleanSysMsg()
{
	SYS_MSG_BUF msgbuf;
	int i;
	for(i = MSG_TYPE_START; i < MSG_TYPE_END;i++)
		while(msgrcv( GetSysMsgId(), &msgbuf, sizeof(msgbuf) - sizeof(long),
					i, IPC_NOWAIT)>=0);
}

/**
 * @brief	System message thread
 * @param	"int qid" : [IN]QID
 * @param	"int mid" : [IN]MID
 * @return	0
 */
int SysMsgThread(int qid,int mid)
{
	SYS_MSG_BUF msgbuf;
	int msg_size, e_count = 0;
	while(IsSysQuit == 0){
		msg_size = msgrcv( qid, &msgbuf, sizeof(msgbuf) - sizeof(long),
				MSG_TYPE_MSG1, 0);
		if( msg_size < 0 ){
			if(e_count++ >= 3){
				ERR("System server receive msg fail \n");
				perror("ERROR:");
#if 0
				sleep(20);
				Msg_Kill(qid);
				qid = Msg_Init(SYS_MSG_KEY);
#endif
				break;
			}
		}else if(msgbuf.src == MSG_TYPE_MSG1 || msgbuf.src < 0){
			ERR("Got Error message\n");
			break;
		} else {
			e_count = 0;
			if(ProcSysMsg(&msgbuf) == 1)
				msgbuf.shmid = mid;
			else
				msgbuf.length = -1;
			if(msgbuf.src != 0){
				msgbuf.Des = msgbuf.src;
				msgbuf.src = MSG_TYPE_MSG1;
				msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);
			}
		}
	}
	IsSysQuit = 1;
	return 0;
}

/**
 * @brief	main entry
 * @param	command line argument
 * @return	error code
 */
int main(int argc,char *argv[]) {
	int qid, mid,bThreadStatus = 0;
	struct shmid_ds buf;
	pthread_t tFileMngThread,tAlarmThread;
	/* System message queue initial */
	qid = Msg_Init(SYS_MSG_KEY);
	SetSysMsgId(qid);
	if(pthread_create(&tFileMngThread, NULL, FileMngThread, NULL)){
		IsSysQuit = 1;
		ERR("FileMngThread create fail\n");
	}
	printf("FileMngThread created\n");
	bThreadStatus |= FILE_THR_CREATED;
	/* Create communication between file manager and system server */
	if(InitFileMsgDrv(FILE_MSG_KEY, FILE_SYS_MSG) != 0){
		if(bThreadStatus & FILE_THR_CREATED){
			SendFileQuitCmd();
			pthread_join (tFileMngThread, NULL);
		}
		ERR("File message driver init fail\n");
		return -1;
	}
	if(pthread_create(&tAlarmThread, NULL, AlramThread, NULL)){
		IsSysQuit = 1;
		ERR("AlramThread create fail\n");
	}
	printf("AlramThread created\n");
	bThreadStatus |= ALARM_THR_CREATED;
	/* Share memory initial (for system server) */
	if((mid = ShareMemInit(SYS_MSG_KEY)) < 0){
		IsSysQuit = 1;
		ERR("Share memory init fail\n");
	}
	printf("Share memory init success\n");
	while(IsSysQuit == 0){
		if(SystemInit() != 0){
			IsSysQuit = 1;
			break;
		}
		printf("SystemInit success\n");
		if(ScheduleMngInit(GetSysInfo()))
		{
			IsSysQuit = 1;
			break;
			ERR("Schedule manager init fail\n");
		}
		printf("Schedule manager init success\n");
		/* Process all messages which was sent to system server */
		SysMsgThread(qid,mid);
	}
	if(bThreadStatus & ALARM_THR_CREATED){
		SendAlarmQuitCmd();
		pthread_join (tAlarmThread, NULL);
	}
	if(bThreadStatus & FILE_THR_CREATED){
		SendFileQuitCmd();
		pthread_join (tFileMngThread, NULL);
	}
	SystemCleanup();
	if(mid >= 0)
		shmctl(mid, IPC_RMID, &buf);
	Msg_Kill(qid);
	return 0;
}
