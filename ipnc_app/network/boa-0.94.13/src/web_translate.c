
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <file_list.h>
#include <web_translate.h>
#include <sys_env_type.h>
#include <file_msg_drv.h>
#include <net_config.h>
#include <ipnc_ver.h>

static char *nicname = "eth0";

ARG_HASH_TABLE *arg_hash;

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_timeformat(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.time_format);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_osdtextinfo(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.osdtext);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_regusr(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.regusr);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxtitlelen(char *data, char *arg)
{
	return sprintf(data, "%d", MAX_LANCAM_TITLE_LEN);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4resolution(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.mpeg4resolution);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_liveresolution(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.liveresolution);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4quality(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.mpeg4quality);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxfqdnlen(char *data, char *arg)
{
	return sprintf(data, "%d", MAX_FQDN_LENGTH - 1);
}
int para_maxdomainname(char *data, char *arg)
{
	return sprintf(data, "%d", MAX_DOMAIN_NAME);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ftpip(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->ftp_config.servier_ip);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ftpipport(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->ftp_config.port);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ftpuser(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->ftp_config.username);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ftppassword(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->ftp_config.password);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ftppapath(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->ftp_config.foldername);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxftpuserlen(char *data, char *arg)
{
	return sprintf(data, "%d", USER_LEN - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxftppwdlen(char *data, char *arg)
{
	return sprintf(data, "%d", PASSWORD_LEN - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxftppathlen(char *data, char *arg)
{
	return sprintf(data, "%d", MAX_FILE_NAME - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtpip(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->smtp_config.servier_ip);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtpport(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->smtp_config.server_port);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtpauth(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->smtp_config.authentication);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtpuser(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->smtp_config.username);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxsmtpuser(char *data, char *arg)
{
	return sprintf(data, "%d", USER_LEN - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtppwd(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->smtp_config.password);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxsmtppwd(char *data, char *arg)
{
	return sprintf(data, "%d", PASSWORD_LEN - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtpsender(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->smtp_config.sender_email);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxsmtpsender(char *data, char *arg)
{
	return sprintf(data, "%d", MAX_NAME_LENGTH - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_emailuser(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->smtp_config.receiver_email);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxemailuserlen(char *data, char *arg)
{
	return sprintf(data, "%d", MAX_NAME_LENGTH - 1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportmpeg4(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportmpeg4);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_format(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.imageformat);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 * NEW FEATURE                                                             *
 ***************************************************************************/
int para_advancemode(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.AdvanceMode);
}
int para_advfeaturename(char *data, char *arg)
{

	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "LDC");
	else if ( *arg == '2' )
		return sprintf(data, "VS");
	else if ( *arg == '3')
		return sprintf(data, "NF");
	else if ( *arg == '4')
		return sprintf(data, "LDC+VS");
	else if ( *arg == '5')
		return sprintf(data, "LDC+NF");
	else if ( *arg == '6')
		return sprintf(data, "VS+NF");
	else if ( *arg == '7')
		return sprintf(data, "LDC+VS+NF");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;LDC;VS;NF;LDC+VS;LDC+NF;VS+NF;LDC+VS+NF");

	return -1;
}

int para_preprocess(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	//return sprintf(data, "%d", pSysInfo->lan_config.AdvanceMode);
	unsigned char retv = (pSysInfo->lan_config.AdvanceMode) & 0x03;
	return sprintf(data, "%d", retv);
}
int para_noisefilt(char *data, char *arg)
{
	//unsigned char retv = (AdvmodeX >> 2) & 0x03;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	unsigned char retv = ((pSysInfo->lan_config.AdvanceMode) >> 2) & 0x03;
	return sprintf(data, "%d", retv);
}
int para_preprocessname(char *data, char *arg)
{

	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "VS");
	else if ( *arg == '2' )
		return sprintf(data, "LDC");
	else if ( *arg == '3')
		return sprintf(data, "VS+LDC");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;VS;LDC;VS+LDC");

	return -1;
}
int para_noisefiltname(char *data, char *arg)
{

	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "SNF");
	else if ( *arg == '2' )
		return sprintf(data, "TNF");
	else if ( *arg == '3')
		return sprintf(data, "SNF+TNF");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;SNF;TNF;SNF+TNF");

	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_osdwin(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.osdwin);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_defaultstorage(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.defaultstorage);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_cfinsert(char *data, char *arg)
{
	return sprintf(data, "%d", 0);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_defaultcardgethtm(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.net.defaultcardgethtm);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_brandurl(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.net.brandurl);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_brandname(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.net.brandname);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supporttstamp(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supporttstamp);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4xsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Mpeg41Xsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4ysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Mpeg41Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_jpegxsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.JpegXsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_jpegysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.JpegYsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_socketauthority(char *data, char *arg)
{
	return sprintf(data, "%d", 0);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_authoritychange(char *data, char *arg)
{
	return sprintf(data, "%d", 0);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportmotion(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportmotion);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportwireless(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportwireless);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_serviceftpclient(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.serviceftpclient);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_servicesmtpclient(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.servicesmtpclient);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_servicepppoe(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.servicepppoe);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_servicesntpclient(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.servicesntpclient);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_serviceddnsclient(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.serviceddnsclient);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportmaskarea(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportmaskarea);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_machinecode(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "0x%x", pSysInfo->DeviceType);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxchannel(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.maxchannel);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportrs485(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportrs485);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportrs232(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportrs232);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ftppath(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->ftp_config.foldername);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_layoutnum(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	if ( *arg == '0' )
		return sprintf(data, "1");
	return sprintf(data, "%d", pSysInfo->lan_config.net.layoutnum);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportmui(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportmui);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mui(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.mui);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportsequence(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportsequence);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_quadmodeselect(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.quadmodeselect);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_serviceipfilter(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.serviceipfilter);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_oemflag0(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.oemflag0);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportdncontrol(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportdncontrol);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportavc(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportavc);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportaudio(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportaudio);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportptzpage(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.supportptzpage);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_dhcpconfig(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.dhcp_config);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_pppoeenable(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4desired(char *data, char *arg)
{
	return sprintf(data, "1");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4cenable(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4resname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "1280x720");
	else if ( *arg == '1' )
		return sprintf(data, "640x480");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "1280x720;640x480");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg42resname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "640x360");
	else if ( *arg == '1' )
		return sprintf(data, "352x192");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "640x360;352x192");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_resolutionname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "720");
	else if ( *arg == '1' )
		return sprintf(data, "VGA");
	else if ( *arg == '2' )
		return sprintf(data, "CIF");
	else if ( *arg == '3' )
		return sprintf(data, "640x360");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "720;VGA;CIF;640x360");
	return -1;
}
/***************************************************************************
 *here                                                                     *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg4qualityname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "high");
	else if ( *arg == '1' )
		return sprintf(data, "normal");
	else if ( *arg == '2' )
		return sprintf(data, "low");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "high;normal;low");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motionylimit(char *data, char *arg)
{
	return sprintf(data, "720");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_ratecontrol(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nRateControl);
}


/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_quadmodeselectname(char *data, char *arg)
{
	return sprintf(data, "30fps");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportagc(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_agc(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_colorkiller(char *data, char *arg)
{
	return sprintf(data, "128");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_fluorescent(char *data, char *arg)
{
	return sprintf(data, "128");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mirror(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_kelvin(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supporthue(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportexposure(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportfluorescent(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportmirros(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportkelvin(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_senseup(char *data, char *arg)
{
	return sprintf(data, "1");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportsenseup(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportmaxagcgain(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supporthspeedshutter(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_hspeedshutter(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxagcgainname(char *data, char *arg)
{
	return sprintf(data, "dark");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_maxagcgain(char *data, char *arg)
{
	return sprintf(data, "0");
}

int para_stream1xsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.JpegXsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream1ysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.JpegYsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream2xsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Mpeg41Xsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream2ysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Mpeg41Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream3xsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Mpeg42Xsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream3ysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Mpeg42Ysize);
}
/***************************************************************************
 * Added in DM365 IPCAM                                                    *
 ***************************************************************************/
int para_stream5xsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Avc1Xsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream5ysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Avc1Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream6xsize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Avc2Xsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream6ysize(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Avc2Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream1name(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "JPG(%dx%d)",	pSysInfo->lan_config.JpegXsize,
										pSysInfo->lan_config.JpegYsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream2name(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "MPEG4(%dx%d)",	pSysInfo->lan_config.Mpeg41Xsize,
											pSysInfo->lan_config.Mpeg41Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream3name(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "MPEG4(%dx%d)",	pSysInfo->lan_config.Mpeg42Xsize,
											pSysInfo->lan_config.Mpeg42Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream4name(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "JPG-2(%dx%d)",	pSysInfo->lan_config.JpegXsize,
											pSysInfo->lan_config.JpegYsize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream5name(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "H.264(%dx%d)",	pSysInfo->lan_config.Avc1Xsize,
											pSysInfo->lan_config.Avc1Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_stream6name(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "H.264(%dx%d)",	pSysInfo->lan_config.Avc2Xsize,
											pSysInfo->lan_config.Avc2Ysize);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_timeformatname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "YYYY/MM/DD");
	else if ( *arg == '1' )
		return sprintf(data, "MM/DD/YYYY");
	else if ( *arg == '2' )
		return sprintf(data, "DD/MM/YYYY");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "YYYY/MM/DD;MM/DD/YYYY;DD/MM/YYYY");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_authorityadmin(char *data, char *arg)
{
	return sprintf(data, "%d", AUTHORITY_ADMIN);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_authorityoperator(char *data, char *arg)
{
	return sprintf(data, "%d", AUTHORITY_OPERATOR);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_authorityviewer(char *data, char *arg)
{
	return sprintf(data, "%d", AUTHORITY_VIEWER);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_user(char *data, char *arg)
{
	int i, count = 0;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	if ( *arg >= '0' && *arg <= '9' )
		return sprintf(data, "%s", pSysInfo->acounts[*arg - '0'].user);
	else if ( *arg == 'a' || *arg == '\0' ){
		for(i = 0; i< ACOUNT_NUM;i++)
			count += sprintf(data + count, "user%d:%s\n", i + 1, pSysInfo->acounts[i].user);
		return count;
	}
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_authority(char *data, char *arg)
{
	int i, count = 0;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	if ( *arg >= '0' && *arg <= '9' )
		return sprintf(data, "%d", pSysInfo->acounts[*arg - '0'].authority);
	else if ( *arg == 'a' || *arg == '\0' ){
		for(i = 0; i< ACOUNT_NUM;i++)
			count += sprintf(data + count, "authority%d:%d\n", i+1,
							pSysInfo->acounts[i].authority);
		return count;
	}
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_mpeg42resolution(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.mpeg42resolution);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_smtpminattach(char *data, char *arg)
{
  return sprintf(data, "1");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
 int para_smtpmaxattach(char *data, char *arg)
{
  return sprintf(data, "20");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motionxlimit(char *data, char *arg)
{
	return sprintf(data, "%d", 1280);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motionxblock(char *data, char *arg)
{
	return sprintf(data, "%d", 4);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_motionyblock(char *data, char *arg)
{
	return sprintf(data, "%d", 3);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportstream1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Supportstream1);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportstream2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Supportstream2);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportstream3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Supportstream3);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportstream4(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Supportstream4);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportstream5(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Supportstream5);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportstream6(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.Supportstream6);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 ***************************************************************************/


/***************************************************************************
 *                                                                         *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int para_osdwinnum(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.osdwinnum);
}

int para_osdwinnumname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Logo Window");
	else if ( *arg == '1' )
		return sprintf(data, "Text Window");
#if 0
	else if ( *arg == '2' )
		return sprintf(data, "Window_3");
	else if ( *arg == '3')
		return sprintf(data, "Window_4");
	else if ( *arg == '4')
		return sprintf(data, "Window_5");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Window_1;Window_2;Window_3;Window_4;Window_5");
#else
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Logo Window;Text Window");
#endif

	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int para_osdstream(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.osdstream);
}

int para_osdstreamname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Stream_1");
	else if ( *arg == '1' )
		return sprintf(data, "Stream_2");
	else if ( *arg == '2' )
		return sprintf(data, "Stream_3");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Stream_1;Stream_2;Stream_3");

	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_waitserver(char *data, char *arg)
{
	return sprintf(data, "0");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_supportcolorkiller(char *data, char *arg)
{
  return sprintf(data, "1");
}
int para_supportAWB(char *data, char *arg)
{
  return sprintf(data, "1");
}
int para_supportbrightness(char *data, char *arg)
{
  return sprintf(data, "1");
}
int para_supportcontrast(char *data, char *arg)
{
  return sprintf(data, "1");
}
int para_supportsaturation(char *data, char *arg)
{
  return sprintf(data, "1");
}
int para_supportbacklight(char *data, char *arg)
{
  return sprintf(data, "1");
}
int para_supportsharpness(char *data, char *arg)
{
  return sprintf(data, "1");
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_tstampenable(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.tstampenable);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
  int para_tstampcolorname(char *data, char *arg)
{
  if ( *arg == '0' )
		return sprintf(data, "white");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "white");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
  int para_tstamplocname(char *data, char *arg)
{
  if ( *arg == '0' )
		return sprintf(data, "leftup");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "leftup");
	return -1;
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_event_location(char *data, char *arg)
{
	extern unsigned int event_location;

	return sprintf(data, "%d", event_location);
}
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_get_event(char *data, char *arg)
{
	extern unsigned int event_location;
	LogEntry_t tLog;
	int i = atoi(arg) - 1 + event_location;
	struct tm* ptm;
	if(GetSysLog(0, i, &tLog)){
		/* No event */
		return sprintf(data, "");
	}
	ptm = &tLog.time;
	return sprintf(data, "%d-%02d-%02d %02d:%02d:%02d %s",
					ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
					ptm->tm_hour, ptm->tm_min, ptm->tm_sec, tLog.event);
}
/***************** Live Video Page ***********************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_democfg(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.democfg);
}

int para_democfgname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "NONE");
	else if ( *arg == '1' )
		return sprintf(data, "VS DEMO");
	else if ( *arg == '2' )
		return sprintf(data, "VNF DEMO");
	else if ( *arg == '3' )
		return sprintf(data, "LDC DEMO");
	else if ( *arg == '4' )
		return sprintf(data, "FD DEMO");
	else if ( *arg == '5' )
		return sprintf(data, "FD ROI DEMO");
	else if ( *arg == '6' )
		return sprintf(data, "ROI CENTER DEMO");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "NONE;VS DEMO;VNF DEMO;LDC DEMO;FD DEMO;FD ROI DEMO;ROI CENTER DEMO");

	return -1;
}
 int para_clicksnapfilename(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%s", pSysInfo->lan_config.nClickSnapFilename);
}
 int para_clicksnapstorage(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nClickSnapStorage);
}
int para_streamname1(char *data, char *arg)
{
	NET_IPV4 ip;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	ip.int32 = net_get_ifaddr(nicname);

	return sprintf(data, "%s(%dx%d)@rtsp://%d.%d.%d.%d:%d/%s", pSysInfo->stream_config[0].name,
		pSysInfo->stream_config[0].width,pSysInfo->stream_config[0].height,
		ip.str[0], ip.str[1], ip.str[2], ip.str[3],
		pSysInfo->stream_config[0].portnum, pSysInfo->stream_config[0].portname);
}
int para_streamname2(char *data, char *arg)
{
	NET_IPV4 ip;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	ip.int32 = net_get_ifaddr(nicname);

	return sprintf(data, "%s(%dx%d)@rtsp://%d.%d.%d.%d:%d/%s", pSysInfo->stream_config[1].name,
		pSysInfo->stream_config[1].width,pSysInfo->stream_config[1].height,
		ip.str[0], ip.str[1], ip.str[2], ip.str[3],
		pSysInfo->stream_config[1].portnum, pSysInfo->stream_config[1].portname);
}
int para_streamname3(char *data, char *arg)
{
	NET_IPV4 ip;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	ip.int32 = net_get_ifaddr(nicname);

	return sprintf(data, "%s(%dx%d)@rtsp://%d.%d.%d.%d:%d/%s", pSysInfo->stream_config[2].name,
		pSysInfo->stream_config[2].width,pSysInfo->stream_config[2].height,
		ip.str[0], ip.str[1], ip.str[2], ip.str[3],
		pSysInfo->stream_config[2].portnum, pSysInfo->stream_config[2].portname);
}
int para_streamwidth1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->stream_config[0].width);
}
int para_streamwidth2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->stream_config[1].width);
}
int para_streamwidth3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->stream_config[2].width);
}

int para_streamheight1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->stream_config[0].height);
}
int para_streamheight2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->stream_config[1].height);
}
int para_streamheight3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->stream_config[2].height);
}


/***************** VIDEO / IMAGE PAGE ***********************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_title(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.title);
}
int para_videocodec(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nStreamType);
}

int para_videocodecname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Single");
	else if ( *arg == '1' )
		return sprintf(data, "Dual");
	else if ( *arg == '2' )
		return sprintf(data, "Tri-Stream");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Single;Dual;Tri-Stream");
	return -1;
}
int para_videocodecmode(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nVideocodecmode);
}
int para_videocodecmodename(char *data, char *arg)
{
	char *modename_list[] =
	{
		"H.264", "MPEG4", "MegaPixel JPEG",
		"H.264 + JPEG", "MPEG4 + JPEG",
		"Dual H.264", "Dual MPEG4", "H264 + MPEG4",
		"Dual H264 + JPEG", "Dual MPEG4 + JPEG",
	};
	int tblsize = sizeof(modename_list)/sizeof(modename_list[0]);
	int i = 0;
	char rtn_msg[512] = "\0";
	if ( *arg == 'a' || *arg == '\0' )
	{
		for (i = 0; i < tblsize; i++)
		{
			if (i > 0)
				strcat(rtn_msg, ";");
			strcat(rtn_msg, modename_list[i]);
		}
		return sprintf(data, "%s", rtn_msg);
	}
	else if (*arg >= '0' && *arg <= '9')
	{
		i = atoi(arg);
		if (i < tblsize)
			return sprintf(data, "%s", modename_list[i]);
	}
	return -1;
}
int para_videocodeccombo(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nVideocombo);
}
int para_videocodeccomboname(char *data, char *arg)
{
	char *modename_list[] =
	{
		"H.264;MPEG4;MegaPixel",
		"H.264 + JPEG;MPEG4 + JPEG;Dual H.264;Dual MPEG4;H264 + MPEG4",
		"Dual H264 + JPEG;Dual MPEG4 + JPEG",
	};
	int tblsize = sizeof(modename_list)/sizeof(modename_list[0]);
	int i = 0;
	char rtn_msg[512] = "\0";
	if ( *arg == 'a' || *arg == '\0' )
	{
		for (i = 0; i < tblsize; i++)
		{
			if (i > 0)
				strcat(rtn_msg, "@");
			strcat(rtn_msg, modename_list[i]);
		}
		return sprintf(data, "%s", rtn_msg);
	}
	else if (*arg >= '0' && *arg <= '9')
	{
		i = atoi(arg);
		if (i < tblsize)
			return sprintf(data, "%s", modename_list[i]);
	}

	return -1;
}

int para_videocodecres(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nVideocodecres);
}
int para_videocodecresname(char *data, char *arg)
{
	char *resname_list[] =
	{
		"H264:720;H264:D1;H264:SXVGA;H264:1080;H264:720MAX60",
		"MPEG4:720;MPEG4:D1;MPEG4:SXVGA;MPEG4:1080;MPEG4:720MAX60",
		"H264:2MP;JPG:2MP;H264:3MP;JPG:3MP;H264:5MP;JPG:5MP",
		"H264:720,JPEG:VGA;H264:D1,JPEG:D1;H264:720,JPEG:720",
		"MPEG4:720,JPEG:VGA;MPEG4:D1,JPEG:D1;MPEG4:720,JPEG:720",
		"H264:720,H264:QVGA;H264:D1,H264:D1;H264:D1,H264:QVGA;H264:1080,H264:QVGA",
		"MPEG4:720,MPEG4:QVGA;MPEG4:D1,MPEG4:D1;MPEG4:D1,MPEG4:QVGA;MPEG4:1080,MPEG4:QVGA",
		"H264:D1,MPEG4:D1",
		"H264:720,JPEG:VGA,H264:QVGA", "MPEG4:720,JPEG:VGA,MPEG4:QVGA",
	};
	int tblsize = sizeof(resname_list)/sizeof(resname_list[0]);
	int i = 0;
	char rtn_msg[1024] = "\0";
	if ( *arg == 'a' || *arg == '\0' )
	{
		for (i = 0; i < tblsize; i++)
		{
			if (i > 0)
				strcat(rtn_msg, "@");
			strcat(rtn_msg, resname_list[i]);
		}
		return sprintf(data, "%s", rtn_msg);
	}
	else if (*arg >= '0' && *arg <= '9')
	{
		i = atoi(arg);
		if (i < tblsize)
			return sprintf(data, "%s", resname_list[i]);
	}

	return -1;
}

int para_mpeg4cvalue(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nMpeg41bitrate/1000);
}
int para_mpeg42cvalue(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nMpeg42bitrate/1000);
}
int para_livequality(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.njpegquality);
}
int para_qualityname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "high");
	else if ( *arg == '1' )
		return sprintf(data, "normal");
	else if ( *arg == '2' )
		return sprintf(data, "low");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "high;normal;low");
	return -1;
}

int para_framerate1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.nFrameRate1);
}
int para_framerate2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nFrameRate2);
}
int para_framerate3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nFrameRate3);
}

#define MAX_FRAMERATE_SIZE	(10)
const char *dayfrmrate_list1_dm368[MAX_FRAMERATE_SIZE] =
{
	"30,24,15,8;30,24,15,8;30,24,15,8;30,24,15,8;Auto",
	"30,24,15,8;30,24,15,8;30,24,15,8;30,24,15,8;Auto",
	"30,24,15,8;30,24,15,8;20,15,8;20,15,8;Auto;Auto",
	"30,24,15,8;30,24,15,8;30,24,15,8",
	"30,24,15,8;30,24,15,8;30,24,15,8",
	"30,24,15,8;30,24,15,8;30,24,15,8;30,24,15,8",
	"30,24,15,8;30,24,15,8;30,24,15,8;30,24,15,8",
	"30,24,15,8",
	"30,24,15,8", "30,24,15,8"
};
const char *dayfrmrate_list2_dm368[MAX_FRAMERATE_SIZE] =
{
	"NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA;NA",
	"30,24,15,8;30,24,15,8;30,24,15,8",
	"30,24,15,8;30,24,15,8;30,24,15,8",
	"30,24,15,8;30,24,15,8;30,24,15,8;30,24,15,8",
	"30,24,15,8;30,24,15,8;30,24,15,8;30,24,15,8",
	"30,24,15,8",
	"30,24,15,8", "30,24,15,8"
};
const char *dayfrmrate_list3_dm368[MAX_FRAMERATE_SIZE] =
{
	"NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA;NA",
	"NA;NA;NA", "NA;NA;NA",
	"NA;NA;NA;NA", "NA;NA;NA;NA",
	"NA",
	"30,24,15,8", "30,24,15,8"
};
const char *nightfrmrate_list1_dm368[MAX_FRAMERATE_SIZE] =
{
	"30,15,5;30,15,5;30,15,5;30,15,5;Auto",
	"30,15,5;30,15,5;30,15,5;30,15,5;Auto",
	"30,15,5;30,15,5;20,15,5;20,15,5;Auto;Auto",
	"30,15,5;30,15,5;30,15,5",
	"30,15,5;30,15,5;30,15,5",
	"30,15,5;30,15,5;30,15,5;30,15,5",
	"30,15,5;30,15,5;30,15,5;30,15,5",
	"30,15,5",
	"30,15,5", "30,15,5"
};
const char *nightfrmrate_list2_dm368[MAX_FRAMERATE_SIZE] =
{
	"NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA;NA",
	"30,15,5;30,15,5;30,15,5",
	"30,15,5;30,15,5;30,15,5",
	"30,15,5;30,15,5;30,15,5;30,15,5",
	"30,15,5;30,15,5;30,15,5;30,15,5",
	"30,15,5",
	"30,15,5", "30,15,5"
};
const char *nightfrmrate_list3_dm368[MAX_FRAMERATE_SIZE] =
{
	"NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA;NA",
	"NA;NA;NA", "NA;NA;NA",
	"NA;NA;NA;NA", "NA;NA;NA;NA",
	"NA",
	"30,15,5", "30,15,5"
};
const char *dayfrmrate_list1_dm365[MAX_FRAMERATE_SIZE] =
{
	"30,24,15,8;30,24,15,8;30,24,15,8;23,15,8;Auto",
	"30,24,15,8;30,24,15,8;30,24,15,8;23,15,8;Auto",
	"30,24,15,8;30,24,15,8;15,8;15,8;Auto;Auto",
	"30,24,15,8;30,24,15,8;30,24,15,8",
	"30,24,15,8;30,24,15,8;30,24,15,8",
	"30,24,15,8;30,24,15,8;30,24,15,8;23,15,8",
	"30,24,15,8;30,24,15,8;30,24,15,8;23,15,8",
	"30,24,15,8",
	"30,24,15,8", "30,24,15,8"
};
const char *dayfrmrate_list2_dm365[MAX_FRAMERATE_SIZE] =
{
	"NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA;NA",
	"30,24,15,8;30,24,15,8;30,24,15,8",
	"30,24,15,8;30,24,15,8;30,24,15,8",
	"30,24,15,8;30,24,15,8;30,24,15,8;23,15,8",
	"30,24,15,8;30,24,15,8;30,24,15,8;23,15,8",
	"30,24,15,8",
	"30,24,15,8", "30,24,15,8"
};
const char *dayfrmrate_list3_dm365[MAX_FRAMERATE_SIZE] =
{
	"NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA;NA",
	"NA;NA;NA", "NA;NA;NA",
	"NA;NA;NA;NA", "NA;NA;NA;NA",
	"NA",
	"30,24,15,8", "30,24,15,8"
};
const char *nightfrmrate_list1_dm365[MAX_FRAMERATE_SIZE] =
{
	"30,15,5;30,15,5;30,15,5;23,15,5;Auto",
	"30,15,5;30,15,5;30,15,5;23,15,5;Auto",
	"30,24,15,5;30,24,15,5;15,5;15,5;Auto;Auto",
	"30,15,5;30,15,5;30,15,5",
	"30,15,5;30,15,5;30,15,5",
	"30,15,5;30,15,5;30,15,5;23,15,5",
	"30,15,5;30,15,5;30,15,5;23,15,5",
	"30,15,5",
	"30,15,5", "30,15,5"
};
const char *nightfrmrate_list2_dm365[MAX_FRAMERATE_SIZE] =
{
	"NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA;NA",
	"30,15,5;30,15,5;30,15,5",
	"30,15,5;30,15,5;30,15,5",
	"30,15,5;30,15,5;30,15,5;23,15,5",
	"30,15,5;30,15,5;30,15,5;23,15,5",
	"30,15,5",
	"30,15,5", "30,15,5"
};
const char *nightfrmrate_list3_dm365[MAX_FRAMERATE_SIZE] =
{
	"NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA", "NA;NA;NA;NA;NA;NA",
	"NA;NA;NA", "NA;NA;NA",
	"NA;NA;NA;NA", "NA;NA;NA;NA",
	"NA",
	"30,15,5", "30,15,5"
};

int para_frameratenameall1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	int i = 0;
	int tblsize;
	char rtn_msg[1024] = "\0";
	const char **nightfrmrate_list1 = pSysInfo->lan_config.chipConfig ? nightfrmrate_list1_dm368:nightfrmrate_list1_dm365;
	const char **dayfrmrate_list1 = pSysInfo->lan_config.chipConfig ? dayfrmrate_list1_dm368:dayfrmrate_list1_dm365;

	if (pSysInfo->lan_config.nDayNight == 0) {
		tblsize = MAX_FRAMERATE_SIZE;
		if ( *arg == 'a' || *arg == '\0' )
		{
			for (i = 0; i < tblsize; i++)
			{
				if (i > 0)
					strcat(rtn_msg, "@");
				strcat(rtn_msg, nightfrmrate_list1[i]);
			}
			return sprintf(data, "%s", rtn_msg);
		}
		else if (*arg >= '0' && *arg <= '9')
		{
			i = atoi(arg);
			if (i < tblsize)
				return sprintf(data, "%s", nightfrmrate_list1[i]);
		}
	}
	else {
		tblsize = MAX_FRAMERATE_SIZE;
		if ( *arg == 'a' || *arg == '\0' )
		{
			for (i = 0; i < tblsize; i++)
			{
				if (i > 0)
					strcat(rtn_msg, "@");
				strcat(rtn_msg, dayfrmrate_list1[i]);
			}
			return sprintf(data, "%s", rtn_msg);
		}
		else if (*arg >= '0' && *arg <= '9')
		{
			i = atoi(arg);
			if (i < tblsize)
				return sprintf(data, "%s", dayfrmrate_list1[i]);
		}
	}

	return -1;
}

int para_frameratenameall2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	int i = 0;
	int tblsize;
	char rtn_msg[1024] = "\0";
	const char **nightfrmrate_list2 = pSysInfo->lan_config.chipConfig ? nightfrmrate_list2_dm368:nightfrmrate_list2_dm365;
	const char **dayfrmrate_list2 = pSysInfo->lan_config.chipConfig ? dayfrmrate_list2_dm368:dayfrmrate_list2_dm365;

	if (pSysInfo->lan_config.nDayNight == 0) {
		tblsize = MAX_FRAMERATE_SIZE;
		if ( *arg == 'a' || *arg == '\0' )
		{
			for (i = 0; i < tblsize; i++)
			{
				if (i > 0)
					strcat(rtn_msg, "@");
				strcat(rtn_msg, nightfrmrate_list2[i]);
			}
			return sprintf(data, "%s", rtn_msg);
		}
		else if (*arg >= '0' && *arg <= '9')
		{
			i = atoi(arg);
			if (i < tblsize)
				return sprintf(data, "%s", nightfrmrate_list2[i]);
		}
	}
	else {
		tblsize = MAX_FRAMERATE_SIZE;
		if ( *arg == 'a' || *arg == '\0' )
		{
			for (i = 0; i < tblsize; i++)
			{
				if (i > 0)
					strcat(rtn_msg, "@");
				strcat(rtn_msg, dayfrmrate_list2[i]);
			}
			return sprintf(data, "%s", rtn_msg);
		}
		else if (*arg >= '0' && *arg <= '9')
		{
			i = atoi(arg);
			if (i < tblsize)
				return sprintf(data, "%s", dayfrmrate_list2[i]);
		}
	}


	return -1;
}

int para_frameratenameall3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	int i = 0;
	int tblsize;
	char rtn_msg[1024] = "\0";
	const char **nightfrmrate_list3 = pSysInfo->lan_config.chipConfig ? nightfrmrate_list3_dm368:nightfrmrate_list3_dm365;
	const char **dayfrmrate_list3 = pSysInfo->lan_config.chipConfig ? dayfrmrate_list3_dm368:dayfrmrate_list3_dm365;

	if (pSysInfo->lan_config.nDayNight == 0) {
		tblsize = MAX_FRAMERATE_SIZE;
		if ( *arg == 'a' || *arg == '\0' )
		{
			for (i = 0; i < tblsize; i++)
			{
				if (i > 0)
					strcat(rtn_msg, "@");
				strcat(rtn_msg, nightfrmrate_list3[i]);
			}
			return sprintf(data, "%s", rtn_msg);
		}
		else if (*arg >= '0' && *arg <= '9')
		{
			i = atoi(arg);
			if (i < tblsize)
				return sprintf(data, "%s", nightfrmrate_list3[i]);
		}
	}
	else {
		tblsize = MAX_FRAMERATE_SIZE;
		if ( *arg == 'a' || *arg == '\0' )
		{
			for (i = 0; i < tblsize; i++)
			{
				if (i > 0)
					strcat(rtn_msg, "@");
				strcat(rtn_msg, dayfrmrate_list3[i]);
			}
			return sprintf(data, "%s", rtn_msg);
		}
		else if (*arg >= '0' && *arg <= '9')
		{
			i = atoi(arg);
			if (i < tblsize)
				return sprintf(data, "%s", dayfrmrate_list3[i]);
		}
	}

	return -1;
}

int para_ratecontrol1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nRateControl1);
}
int para_ratecontrol2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nRateControl2);
}
int para_ratecontrolname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "VBR");
	else if ( *arg == '2' )
		return sprintf(data, "CBR");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;VBR;CBR");
	return -1;
}

 int para_datestampenable1(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[0].dstampenable);
}
 int para_datestampenable2(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[1].dstampenable);
}
 int para_datestampenable3(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[2].dstampenable);
}
 int para_timestampenable1(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[0].tstampenable);
}
 int para_timestampenable2(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[1].tstampenable);
}
 int para_timestampenable3(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[2].tstampenable);
}
  int para_logoenable1(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[0].nLogoEnable);
}
 int para_logoenable2(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[1].nLogoEnable);
}
 int para_logoenable3(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[2].nLogoEnable);
}
  int para_logoposition1(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[0].nLogoPosition);
}
 int para_logoposition2(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[1].nLogoPosition);
}
 int para_logoposition3(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[2].nLogoPosition);
}
 int para_logopositionname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Top-Left");
	else if ( *arg == '1' )
		return sprintf(data, "Top-Right");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Top-Left;Top-Right");
	return -1;
}
 int para_textenable1(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[0].nTextEnable);
}
 int para_textenable2(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[1].nTextEnable);
}
 int para_textenable3(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[2].nTextEnable);
}
  int para_textposition1(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[0].nTextPosition);
}
 int para_textposition2(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[1].nTextPosition);
}
 int para_textposition3(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[2].nTextPosition);
}
 int para_textpositionname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Top-Left");
	else if ( *arg == '1' )
		return sprintf(data, "Top-Right");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Top-Left;Top-Right");
	return -1;
}
 int para_overlaytext1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->osd_config[0].overlaytext);
}
  int para_overlaytext2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->osd_config[1].overlaytext);
}
int para_overlaytext3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->osd_config[2].overlaytext);
}
 int para_detailinfo1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[0].nDetailInfo);
}
  int para_detailinfo2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[1].nDetailInfo);
}
  int para_detailinfo3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->osd_config[2].nDetailInfo);
}
int para_mirctrl(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.mirror);
}

int para_mirctrlname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "HORIZONTAL");
	else if ( *arg == '2' )
		return sprintf(data, "VERTICAL");
	else if ( *arg == '3')
		return sprintf(data, "BOTH");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;HORIZONTAL;VERTICAL;BOTH");

	return -1;
}
 int para_encryptvideo(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nEncryptVideo);
}
 int para_localdisplay(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nLocalDisplay);
}
int para_localdisplayname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "NTSC");
	else if ( *arg == '2' )
		return sprintf(data, "PAL");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;NTSC;PAL");

	return -1;
}

 int para_aviformat(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.aviformat);
}
 int para_aviformatname(char *data, char *arg)
{
  	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	int supportMpeg4 = pSysInfo->lan_config.Supportstream2;
	int supportMpeg4cif = pSysInfo->lan_config.Supportstream3;
	int supportH264 = pSysInfo->lan_config.Supportstream5;
	int supportH264cif = pSysInfo->lan_config.Supportstream6;
	int mpeg41Xsize= pSysInfo->lan_config.Mpeg41Xsize;
	int mpeg41Ysize= pSysInfo->lan_config.Mpeg41Ysize;
	int mpeg42Xsize=pSysInfo->lan_config.Mpeg42Xsize;
	int mpeg42Ysize=pSysInfo->lan_config.Mpeg42Ysize;
	int avc1Xsize=pSysInfo->lan_config.Avc1Xsize;
	int avc1Ysize=pSysInfo->lan_config.Avc1Ysize;
	int avc2Xsize=pSysInfo->lan_config.Avc2Xsize;
	int avc2Ysize=pSysInfo->lan_config.Avc2Ysize;

	if ( supportH264 ) {
		if (supportH264cif){
			return sprintf(data,"H.264(%d x %d);H.264(%d x %d)", avc1Xsize,avc1Ysize,avc2Xsize,avc2Ysize);
		}
		else if(supportMpeg4cif){
			return sprintf(data,"H.264(%d x %d);MPEG4(%d x %d)",avc1Xsize,avc1Ysize,mpeg42Xsize,mpeg42Ysize);
		}
		else {
			return sprintf(data,"H.264(%d x %d)",avc1Xsize,avc1Ysize);
		}
	}
	else if (supportMpeg4){
		if (supportH264cif){
			return sprintf(data,"MPEG4(%d x %d);H.264(%d x %d)",mpeg41Xsize,mpeg41Ysize,avc2Xsize,avc2Ysize);
		}
		else if(supportMpeg4cif){
			return sprintf(data,"MPEG4(%d x %d);MPEG4(%d x %d)",mpeg41Xsize,mpeg41Ysize,mpeg42Xsize,mpeg42Ysize);
		}
		else {
			return sprintf(data,"MPEG4(%d x %d)",mpeg41Xsize,mpeg41Ysize);
		}
	}
	else{
		return sprintf(data,"NA");
	}

	return -1;
}

 int para_aviduration(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.aviduration);
}
 int para_avidurationname(char *data, char *arg)
{
  	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	if(pSysInfo->sdcard_config.sdinsert==3){
		if( *arg == '0' )
			return sprintf(data, "AUTO");
		else if( *arg == 'a' || *arg == '\0')
			return sprintf(data, "AUTO");
	}
	else {
		if ( *arg == '0' )
			return sprintf(data, "5 sec");
		else if ( *arg == '1' )
			return sprintf(data, "10 sec");
		else if ( *arg == 'a' || *arg == '\0' )
			return sprintf(data, "5 sec;10 sec");
	}

	return -1;
}

/***************** VIDEO ADVANCED PAGE ***********************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

 int para_ipratio1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_advconfig[0].ipRatio);
}
  int para_ipratio2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_advconfig[1].ipRatio);
}
  int para_ipratio3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_advconfig[2].ipRatio);
}
  int para_forceIframe1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_advconfig[0].fIframe);
}
  int para_forceIframe2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_advconfig[1].fIframe);
}
  int para_forceIframe3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_advconfig[2].fIframe);
}
  int para_qpinit1(char *data, char *arg)
{
	unsigned char value;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	value = pSysInfo->codec_advconfig[0].qpInit;
	if(pSysInfo->lan_config.codectype1==H264_CODEC)
		value = (value>51) ? 51:value;
	else if(pSysInfo->lan_config.codectype1==MPEG4_CODEC)
		value = (value>31) ? 31:value;

	value = (value==0) ? 1: value;

	return sprintf(data, "%d", value);
}
  int para_qpinit2(char *data, char *arg)
{
	unsigned char value;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	value = pSysInfo->codec_advconfig[1].qpInit;
	if(pSysInfo->lan_config.codectype1==H264_CODEC)
		value = (value>51) ? 51:value;
	else if(pSysInfo->lan_config.codectype1==MPEG4_CODEC)
		value = (value>31) ? 31:value;

	value = (value==0) ? 1: value;

	return sprintf(data, "%d", value);
}
  int para_qpinit3(char *data, char *arg)
{
	unsigned char value;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	value = pSysInfo->codec_advconfig[2].qpInit;
	if(pSysInfo->lan_config.codectype1==H264_CODEC)
		value = (value>51) ? 51:value;
	else if(pSysInfo->lan_config.codectype1==MPEG4_CODEC)
		value = (value>31) ? 31:value;

	value = (value==0) ? 1: value;

	return sprintf(data, "%d", value);
}
  int para_qpmin1(char *data, char *arg)
{
	unsigned char value;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	value = pSysInfo->codec_advconfig[0].qpMin;
	if(pSysInfo->lan_config.codectype1==H264_CODEC)
		value = (value>51) ? 51:value;
	else if(pSysInfo->lan_config.codectype1==MPEG4_CODEC)
		value = (value>31) ? 31:value;

	value = (value==0) ? 1: value;

	return sprintf(data, "%d", value);
}
  int para_qpmin2(char *data, char *arg)
{
	unsigned char value;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	value = pSysInfo->codec_advconfig[1].qpMin;
	if(pSysInfo->lan_config.codectype2==H264_CODEC)
		value = (value>51) ? 51:value;
	else if(pSysInfo->lan_config.codectype2==MPEG4_CODEC)
		value = (value>31) ? 31:value;

	value = (value==0) ? 1: value;

	return sprintf(data, "%d", value);
}
  int para_qpmin3(char *data, char *arg)
{
	unsigned char value;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	value = pSysInfo->codec_advconfig[2].qpMin;
	if(pSysInfo->lan_config.codectype3==H264_CODEC)
		value = (value>51) ? 51:value;
	else if(pSysInfo->lan_config.codectype3==MPEG4_CODEC)
		value = (value>31) ? 31:value;

	value = (value==0) ? 1: value;

	return sprintf(data, "%d", value);
}
 int para_qpmax1(char *data, char *arg)
{
	unsigned char value;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	value = pSysInfo->codec_advconfig[0].qpMax;
	if(pSysInfo->lan_config.codectype1==H264_CODEC)
		value = (value>51) ? 51:value;
	else if(pSysInfo->lan_config.codectype1==MPEG4_CODEC)
		value = (value>31) ? 31:value;

	value = (value==0) ? 1: value;

	return sprintf(data, "%d", value);
}
  int para_qpmax2(char *data, char *arg)
{
	unsigned char value;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	value = pSysInfo->codec_advconfig[1].qpMax;
	if(pSysInfo->lan_config.codectype2==H264_CODEC)
		value = (value>51) ? 51:value;
	else if(pSysInfo->lan_config.codectype2==MPEG4_CODEC)
		value = (value>31) ? 31:value;

	value = (value==0) ? 1: value;

	return sprintf(data, "%d", value);
}
  int para_qpmax3(char *data, char *arg)
{
	unsigned char value;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	value = pSysInfo->codec_advconfig[2].qpMax;
	if(pSysInfo->lan_config.codectype3==H264_CODEC)
		value = (value>51) ? 51:value;
	else if(pSysInfo->lan_config.codectype3==MPEG4_CODEC)
		value = (value>31) ? 31:value;

	value = (value==0) ? 1: value;

	return sprintf(data, "%d", value);
}
  int para_meconfig1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_advconfig[0].meConfig);
}
  int para_meconfig2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_advconfig[1].meConfig);
}
  int para_meconfig3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_advconfig[2].meConfig);
}
 int para_meconfigname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "AUTO");
	else if ( *arg == '1' )
		return sprintf(data, "CUSTOM");
	else if ( *arg == '2' )
		return sprintf(data, "SVC");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "AUTO;CUSTOM;SVC");
	return -1;
}
 int para_packetsize1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_advconfig[0].packetSize);
}
  int para_packetsize2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_advconfig[1].packetSize);
}
  int para_packetsize3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_advconfig[2].packetSize);
}
 int para_regionofinterestenable1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[0].numROI);
}
  int para_regionofinterestenable2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[1].numROI);
}
  int para_regionofinterestenable3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[2].numROI);
}
 int para_str1x1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[0].roi[0].startx);
}
 int para_str1y1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[0].roi[0].starty);
}
 int para_str1w1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[0].roi[0].width);
}
 int para_str1h1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[0].roi[0].height);
}
 int para_str1x2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[0].roi[1].startx);
}
 int para_str1y2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[0].roi[1].starty);
}
 int para_str1w2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[0].roi[1].width);
}
 int para_str1h2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[0].roi[1].height);
}
 int para_str1x3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[0].roi[2].startx);
}
 int para_str1y3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[0].roi[2].starty);
}
 int para_str1w3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[0].roi[2].width);
}
 int para_str1h3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[0].roi[2].height);
}
 int para_str2x1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[1].roi[0].startx);
}
 int para_str2y1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[1].roi[0].starty);
}
 int para_str2w1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[1].roi[0].width);
}
 int para_str2h1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[1].roi[0].height);
}
 int para_str2x2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[1].roi[1].startx);
}
 int para_str2y2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[1].roi[1].starty);
}
 int para_str2w2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[1].roi[1].width);
}
 int para_str2h2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[1].roi[1].height);
}
 int para_str2x3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[1].roi[2].startx);
}
 int para_str2y3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[1].roi[2].starty);
}
 int para_str2w3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[1].roi[2].width);
}
 int para_str2h3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[1].roi[2].height);
}
 int para_str3x1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[2].roi[0].startx);
}
 int para_str3y1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[2].roi[0].starty);
}
 int para_str3w1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[2].roi[0].width);
}
 int para_str3h1(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[2].roi[0].height);
}
 int para_str3x2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[2].roi[1].startx);
}
 int para_str3y2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[2].roi[1].starty);
}
 int para_str3w2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[2].roi[1].width);
}
 int para_str3h2(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[2].roi[1].height);
}
 int para_str3x3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[2].roi[2].startx);
}
 int para_str3y3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[2].roi[2].starty);
}
 int para_str3w3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[2].roi[2].width);
}
 int para_str3h3(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->codec_roiconfig[2].roi[2].height);
}


/***************** VIDEO ANALYTICS PAGE ***********************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int para_motionblock(char *data, char *arg)
{
  	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->motion_config.motionblock);
}
int para_motion_blk(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;

	strncpy(data, pSysInfo->motion_config.motionblock, MOTION_BLK_LEN);
	return strlen(data);
}
int para_motionsensitivity(char *data, char *arg)
{
  SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->motion_config.motionlevel);

}
int para_motionname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "LOW");
	else if ( *arg == '1' )
		return sprintf(data, "MEDIUM");
	else if ( *arg == '2' )
		return sprintf(data, "HIGH");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "LOW;MEDIUM;HIGH");

	return -1;
}
int para_motioncenable(char *data, char *arg)
{
  SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->motion_config.motioncenable);
}
int para_motioncvalue(char *data, char *arg)
{
  SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->motion_config.motioncvalue);
}
int para_fdetect(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->face_config.fdetect);
}
int para_fdetectname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "DETECT");
	else if ( *arg == '2' )
		return sprintf(data, "ENHANCED DETECT");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;DETECT;ENHANCED DETECT");

	return -1;
}
int para_fdx(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->face_config.startX);
}
 int para_fdy(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->face_config.startY);
}
 int para_fdw(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->face_config.width);
}
 int para_fdh(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->face_config.height);
}
 int para_fdconflevel(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->face_config.fdconflevel);
}
 int para_fddirection(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->face_config.fddirection);
}
int para_fddirectionname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "UP");
	else if ( *arg == '1' )
		return sprintf(data, "LEFT");
	else if ( *arg == '2' )
		return sprintf(data, "RIGHT");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "UP;LEFT;RIGHT");
	return -1;
}
int para_frecognition(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->face_config.frecog);
}
int para_frecognitionname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "RECOGNIZE USER");
	else if ( *arg == '2' )
		return sprintf(data, "REGISTER USER");
	else if ( *arg == '3' )
		return sprintf(data, "CLEAR ALL USERS");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;RECOGNIZE USER;REGISTER USER;CLEAR ALL USERS");

	return -1;
}
int para_frconflevel(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->face_config.frconflevel);
}
int para_frdatabase(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->face_config.frdatabase);
}
int para_privacymask(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->face_config.pmask);
}
int para_maskoptions(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->face_config.maskoption);
}
int para_maskoptionsname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "BLACK BOX");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "BLACK BOX");
	return -1;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

/***************** CAMERA PAGE ***********************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_brightness(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nBrightness);
}
int para_contrast(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nContrast);
}
int para_saturation(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nSaturation);
}
int para_sharpness(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nSharpness);
}
int para_blc(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nBacklightControl);
}
int para_backlight(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nBackLight);
}
int  para_backlightname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Min");
	else if ( *arg == '1' )
		return sprintf(data, "Mid");
	else if ( *arg == '2' )
		return sprintf(data, "Max");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Min;Mid;Max");
	return -1;
}
int para_dynrange(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.dynRange);
}
int para_dynrangename(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "None");
	else if ( *arg == '1' )
		return sprintf(data, "Low Global Enhancement");
	else if ( *arg == '2' )
		return sprintf(data, "Medium Global Enhancement");
	else if ( *arg == '3' )
		return sprintf(data, "High Global Enhancement");
	else if ( *arg == '4' )
		return sprintf(data, "Low Local Enhancement");
	else if ( *arg == '5' )
		return sprintf(data, "Medium Local Enhancement");
	else if ( *arg == '6' )
		return sprintf(data, "High Local Enhancement");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "None;Low Global Enhancement;Medium Global Enhancement;High Global Enhancement;Low Local Enhancement;Medium Local Enhancement;High Local Enhancement");
	return -1;
}
int para_awb(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nWhiteBalance);
}

int para_awbname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Auto");
#if 0
	else if ( *arg == '1' )
		return sprintf(data, "Indoor");
	else if ( *arg == '2' )
		return sprintf(data, "Outdoor");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Auto;Indoor;Outdoor");
#else
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Auto");
#endif

	return -1;
}
int para_daynight(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nDayNight);
}
int para_daynightname(char *data, char *arg)
{
	char *namelist[] = {"night","day"};
	if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "%s;%s", namelist[0], namelist[1]);
	else if (*arg >= '0' && *arg <= '1') {
		int idx = atoi(arg);
		return sprintf(data, "%s", namelist[idx]);
	}
	return -1;
}
int para_histogram(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.histogram);
}
 int para_vidstb(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", (pSysInfo->lan_config.AdvanceMode&FFLAG_VS)?1:0);
}
int para_ldc(char *data, char *arg)
 {
 	SysInfo* pSysInfo = GetSysInfo();
 	if(pSysInfo == NULL)
 		return -1;
 	return sprintf(data, "%d", (pSysInfo->lan_config.AdvanceMode&FFLAG_LDC)?1:0);
 }

int para_binning(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nBinning);
}
int para_binningname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "%s", "BINNING");
	else if ( *arg == '1' )
		return sprintf(data, "%s", "SKIPPING");
	else if ( *arg == '2' )
		return sprintf(data, "%s", "WINDOW");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "%s;%s;%s", "BINNING", "SKIPPING", "WINDOW");
	return -1;
}
int para_image2a(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.nAEWswitch);
}

int para_img2aname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "NONE");
	else if ( *arg == '2' || *arg == '1')
		return sprintf(data, "TI");
	else if ( *arg == 'a' || *arg == '\0' ) {
		return sprintf(data, "NONE;TI");
	}
	return -1;
}
int para_image2atype(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.nAEWtype);
}

int para_img2atypename(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "Auto Exposure");
	else if ( *arg == '2' )
		return sprintf(data, "Auto White Balance");
	else if ( *arg == '3' )
		return sprintf(data, "Auto Exposure + Auto White Balance");
	else if ( *arg == 'a' || *arg == '\0' ) {
		return sprintf(data, "OFF;Auto Exposure;Auto White Balance;Auto Exposure + Auto White Balance");
	}
	return -1;
}

  int para_maxexposuretime(char *data, char *arg)
 {
 	SysInfo* pSysInfo = GetSysInfo();
 	if(pSysInfo == NULL)
 		return -1;
 	return sprintf(data, "%d", pSysInfo->lan_config.maxexposure);
 }
   int para_maxexposuretimename(char *data, char *arg)
 {
 	if ( *arg == 'AUTO' )
 		return sprintf(data, "0");
 #if 0
 	else if ( *arg == 'LOW' )
 		return sprintf(data, "1");
 	else if ( *arg == 'MED' )
 		return sprintf(data, "2");
 	else if ( *arg == 'HIGH' )
 		return sprintf(data, "3");
 	else if ( *arg == 'a' || *arg == '\0' )
 		return sprintf(data, "AUTO;LOW;MED;HIGH");
#else
 	else if ( *arg == 'a' || *arg == '\0' )
 		return sprintf(data, "AUTO");
#endif
 	return -1;
 }
  int para_maxgain(char *data, char *arg)
 {
 	SysInfo* pSysInfo = GetSysInfo();
 	if(pSysInfo == NULL)
 		return -1;
 	return sprintf(data, "%d", pSysInfo->lan_config.maxgain);
 }
   int para_maxgainname(char *data, char *arg)
 {
 	if ( *arg == 'AUTO' )
 		return sprintf(data, "0");
 #if 0
 	else if ( *arg == 'LOW' )
 		return sprintf(data, "1");
 	else if ( *arg == 'MED' )
 		return sprintf(data, "2");
 	else if ( *arg == 'HIGH' )
 		return sprintf(data, "3");
 	else if ( *arg == 'a' || *arg == '\0' )
 		return sprintf(data, "AUTO;LOW;MED;HIGH");
#else
 	else if ( *arg == 'a' || *arg == '\0' )
 		return sprintf(data, "AUTO");
#endif
 	return -1;
 }
int para_imagesource(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.imagesource);
}
 int para_exposurename(char *data, char *arg)
{
	if ( *arg == '2' )
		return sprintf(data, "No flicker");
	if ( *arg == '0' )
		return sprintf(data, "60hz flicker");
	if ( *arg == '1' )
		return sprintf(data, "50hz flicker");
	if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "60hz flicker;50hz flicker;No flicker");

	return -1;
}
int para_priority(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.expPriority);
}

 int para_priorityname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Priortize FrameRate");
	else if ( *arg == '1' )
		return sprintf(data, "Priortize Exposure");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Priortize FrameRate;Priortize Exposure");
	return -1;
}
  int para_snfltctrl(char *data, char *arg)
 {
     SysInfo* pSysInfo = GetSysInfo();

 	if(pSysInfo == NULL)
 		return -1;
 	return sprintf(data, "%d", (pSysInfo->lan_config.AdvanceMode&FFLAG_SNF)?1:0);
 }
    int para_snfltctrlname(char *data, char *arg)
 {
#if 0
	if ( *arg == '0' )
 		return sprintf(data, "OFF");
 	else if ( *arg == '1' )
 		return sprintf(data, "LOW");
 	else if ( *arg == '2' )
 		return sprintf(data, "MEDIUM");
 	else if ( *arg == '3' )
 		return sprintf(data, "HIGH");
 	else if ( *arg == 'a' || *arg == '\0' )
 		return sprintf(data, "OFF;LOW;MEDIUM;HIGH");
#else
	if ( *arg == '0' )
 		return sprintf(data, "OFF");
 	else if ( *arg == '1' )
 		return sprintf(data, "ON");
 	else if ( *arg == 'a' || *arg == '\0' )
 		return sprintf(data, "OFF;ON");
#endif

 	return -1;
 }
   int para_tnfltctrl(char *data, char *arg)
 {
     SysInfo* pSysInfo = GetSysInfo();

 	if(pSysInfo == NULL)
 		return -1;

 	return sprintf(data, "%d", (pSysInfo->lan_config.AdvanceMode&FFLAG_TNF)?1:0);
 }

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

/***************** AUDIO PAGE ***********************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_audioenable(char *data, char *arg)
{
  SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->audio_config.audioON);
}
 int para_audiomode(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->audio_config.audiomode);
}
 int para_audiomodename(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Only Mic");
	else if ( *arg == '1' )
		return sprintf(data, "Only Speaker");
	else if ( *arg == '2' )
		return sprintf(data, "Both");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Only Mic;Only Speaker;Both Mic & Speaker");
	return -1;
}
  int para_audioinvolume(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->audio_config.audioinvolume);
}
 int para_encoding(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->audio_config.codectype);
}
  int para_encodingname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "G711");
#if 1
	if ( *arg == '1' )
		return sprintf(data, "AAC-LC");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "G711;AAC-LC");
#else
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "G711");
#endif
	return -1;
}
 int para_samplerate(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->audio_config.samplerate);
}
  int para_sampleratename(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "8Khz");
	else if ( *arg == '1' )
		return sprintf(data, "16Khz");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "8Khz;16Khz");

	return -1;
}
 int para_audiobitrate(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->audio_config.bitrate);
}
  int para_audiobitratename(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	if(pSysInfo->audio_config.codectype==0) {
		if(pSysInfo->audio_config.samplerate==0) {
			if ( *arg == '0' )
				return sprintf(data, "AUTO: 64Kbps");
			else if ( *arg == 'a' || *arg == '\0' )
				return sprintf(data, "AUTO: 64Kbps");
		}
		else if(pSysInfo->audio_config.samplerate==1) {
			if ( *arg == '0' )
				return sprintf(data, "AUTO: 128Kbps");
			else if ( *arg == 'a' || *arg == '\0' )
				return sprintf(data, "AUTO: 128Kbps");
		}
	}
	else if(pSysInfo->audio_config.codectype==1) {
		if(pSysInfo->audio_config.samplerate==0) {
			if ( *arg == '0' )
				return sprintf(data, "24Kbps");
			else if ( *arg == '1' )
				return sprintf(data, "36Kbps");
			else if ( *arg == '2' )
				return sprintf(data, "48Kbps");
			else if ( *arg == 'a' || *arg == '\0' )
				return sprintf(data, "24Kbps;36Kbps;48Kbps");
		}
		else if(pSysInfo->audio_config.samplerate==1) {
			if ( *arg == '0' )
				return sprintf(data, "32Kbps");
			else if ( *arg == '1' )
				return sprintf(data, "48Kbps");
			else if ( *arg == '2' )
				return sprintf(data, "64Kbps");
			else if ( *arg == 'a' || *arg == '\0' )
				return sprintf(data, "32Kbps;48Kbps;64Kbps");
		}
	}

	return -1;
}
int para_audiobitratenameall(char *data, char *arg)
{
	char *resname_list[] =
	{
		"AUTO: 64Kbps",
		"24Kbps;48Kbps;64Kbps",
		"AUTO: 128Kbps",
		"40Kbps;64Kbps;96Kbps",
	};
	int tblsize = sizeof(resname_list)/sizeof(resname_list[0]);
	int i = 0;
	char rtn_msg[512] = "\0";
	if ( *arg == 'a' || *arg == '\0' )
	{
		for (i = 0; i < tblsize; i++)
		{
			if (i > 0)
				strcat(rtn_msg, "@");
			strcat(rtn_msg, resname_list[i]);
		}
		return sprintf(data, "%s", rtn_msg);
	}
	else if (*arg >= '0' && *arg <= '1')
	{
		i = atoi(arg);
		if (i < tblsize)
			return sprintf(data, "%s", resname_list[i]);
	}

	return -1;
}

 int para_alarmlevel(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->audio_config.alarmlevel);
}
  int para_audiooutvolume(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->audio_config.audiooutvolume);
}

  int para_audioreceiverenable(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->audio_config.audiorecvenable);
}

  int para_audioserverip(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->audio_config.audioServerIp);
}

/***************** DATE TIME PAGE ***********************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_date(char *data, char *arg)
{
	time_t tnow;
	struct tm *tmnow;

	tzset();
	time(&tnow);
	tmnow = localtime(&tnow);

	if ( *arg == '0' ) {
		return sprintf(data, "%d", tmnow->tm_year+1900);
	}
	else if ( *arg == '1' ) {
		return sprintf(data, "%d", tmnow->tm_mon+1);
	}
	else if ( *arg == '2' ) {
		return sprintf(data, "%d", tmnow->tm_mday);
	}
	else if ( *arg == '\0' )
		return sprintf(data, "%04d/%02d/%02d", tmnow->tm_year+1900, tmnow->tm_mon+1, tmnow->tm_mday);
	return -1;
}
int para_time(char *data, char *arg)
{
	time_t tnow;
	struct tm *tmnow;

	tzset();
	time(&tnow);
	tmnow = localtime(&tnow);

	if ( *arg == '0' ) {
		return sprintf(data, "%d", tmnow->tm_hour);
	}
	else if ( *arg == '1' ) {
		return sprintf(data, "%d", tmnow->tm_min);
	}
	else if ( *arg == '2' ) {
		return sprintf(data, "%d", tmnow->tm_sec);
	}
	else if ( *arg == '\0' )
		return sprintf(data, "%02d:%02d:%02d", tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec);
	return -1;
}
int para_sntptimezone(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.ntp_timezone);
}

char *TZname[] = {
	"GMT-12 Eniwetok, Kwajalein",
	"GMT-11 Midway Island, Samoa",
	"GMT-10 Hawaii",
	"GMT-09 Alaska",
	"GMT-08 Pacific Time (US & Canada), Tijuana",
	"GMT-07 Mountain Time (US & Canada), Arizona",
	"GMT-06 Central Time (US & Canada), Mexico City, Tegucigalpa, Saskatchewan",
	"GMT-05 Eastern Time (US & Canada), Indiana(East), Bogota, Lima",
	"GMT-04 Atlantic Time (Canada), Caracas, La Paz",
	"GMT-03 Brasilia, Buenos Aires, Georgetown",
	"GMT-02 Mid-Atlantic",
	"GMT-01 Azores, Cape Verdes Is.",
	"GMT+00 GMT, Dublin, Edinburgh, London, Lisbon, Monrovia, Casablanca",
	"GMT+01 Berlin, Stockholm, Rome, Bern, Brussels, Vienna, Paris, Madrid, Amsterdam, Prague, Warsaw, Budapest",
	"GMT+02 Athens, Helsinki, Istanbul, Cairo, Eastern Europe, Harare, Pretoria, Israel",
	"GMT+03 Baghdad, Kuwait, Nairobi, Riyadh, Moscow, St. Petersburg, Kazan, Volgograd",
	"GMT+04 Abu Dhabi, Muscat, Tbilisi",
	"GMT+05 Islamabad, Karachi, Ekaterinburg, Tashkent",
	"GMT+06 Alma Ata, Dhaka",
	"GMT+07 Bangkok, Jakarta, Hanoi",
	"GMT+08 Taipei, Beijing, Chongqing, Urumqi, Hong Kong, Perth, Singapore",
	"GMT+09 Tokyo, Osaka, Sapporo, Seoul, Yakutsk",
	"GMT+10 Brisbane, Melbourne, Sydney, Guam, Port Moresby, Vladivostok, Hobart",
	"GMT+11 Magadan, Solomon Is., New Caledonia",
	"GMT+12 Fiji, Kamchatka, Marshall Is., Wellington, Auckland"
};

int para_timezonename(char *data, char *arg)
{
#if 1
	if ( *arg == '\0' ) {
		SysInfo* pSysInfo = GetSysInfo();
		if(pSysInfo == NULL)
			return -1;
		if (pSysInfo->lan_config.net.ntp_timezone <= 24)
			return sprintf(data, "%s", TZname[pSysInfo->lan_config.net.ntp_timezone]);
		return -1;
	}
	else {
		int tz = atoi(arg);
		if ((tz >= 0) && (tz <= 24))
			return sprintf(data, "%s", TZname[tz]);
		return -1;
	}
#else

	int i, tblsize = sizeof(TZname)/sizeof(TZname[0]);
	char timezone_msg[2048] = "\0";

	if ( *arg == 'a' || *arg == '\0' )
	{
		for (i = 0; i < tblsize; i++)
		{
			if (i > 0)
				strcat(timezone_msg, "@");

			strcat(timezone_msg, TZname[i]);

		}

		return sprintf(data, "%s", timezone_msg);
	}

	return -1;

#endif
}

int para_daylight(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.daylight_time);
}
int para_dateformat(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.dateformat);
}
int para_dateformatname(char *data, char *arg)
{
  if ( *arg == '0' )
		return sprintf(data, "YYYY/MM/DD");
	else if ( *arg == '1' )
		return sprintf(data, "MM/DD/YYYY");
	else if ( *arg == '2' )
		return sprintf(data, "DD/MM/YYYY");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "YYYY/MM/DD;MM/DD/YYYY;DD/MM/YYYY");
	return -1;
}
int para_tstampformat(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.tstampformat);
}
int para_tstampformatname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "12Hrs");
	else if ( *arg == '1' )
		return sprintf(data, "24Hrs");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "12Hrs;24Hrs");
	return -1;
}
int para_dateposition(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.dateposition);
}
 int para_timeposition(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.timeposition);
}
 int para_datetimepositionname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Bottom-Left");
	else if ( *arg == '1' )
		return sprintf(data, "Bottom-Right");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Bottom-Left;Bottom-Right");
	return -1;
}
/***************** NETWORK & PORT ***********************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
 int para_dhcpenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.dhcp_enable);
}

int para_netip(char *data, char *arg)
{
	NET_IPV4 ip;
	ip.int32 = net_get_ifaddr(nicname);

	if ( *arg == '0' )
		return sprintf(data, "%d", ip.str[0]);
	else if ( *arg == '1' )
		return sprintf(data, "%d", ip.str[1]);
	else if ( *arg == '2' )
		return sprintf(data, "%d", ip.str[2]);
	else if ( *arg == '3' )
		return sprintf(data, "%d", ip.str[3]);
	else if ( *arg == '\0' )
		return sprintf(data, "%03d.%03d.%03d.%03d", ip.str[0], ip.str[1], ip.str[2], ip.str[3]);
	return -1;
}

int para_netmask(char *data, char *arg)
{
	NET_IPV4 mask;
	mask.int32 = net_get_netmask(nicname);

	if ( *arg == '0' )
		return sprintf(data, "%d", mask.str[0]);
	else if ( *arg == '1' )
		return sprintf(data, "%d", mask.str[1]);
	else if ( *arg == '2' )
		return sprintf(data, "%d", mask.str[2]);
	else if ( *arg == '3' )
		return sprintf(data, "%d", mask.str[3]);
	else if ( *arg == '\0' )
		return sprintf(data, "%03d.%03d.%03d.%03d", mask.str[0], mask.str[1], mask.str[2], mask.str[3]);
	return -1;
}

int para_gateway(char *data, char *arg)
{
	NET_IPV4 gateway;
	gateway.int32 = net_get_gateway();

	if ( *arg == '0' )
		return sprintf(data, "%d", gateway.str[0]);
	else if ( *arg == '1' )
		return sprintf(data, "%d", gateway.str[1]);
	else if ( *arg == '2' )
		return sprintf(data, "%d", gateway.str[2]);
	else if ( *arg == '3' )
		return sprintf(data, "%d", gateway.str[3]);
	else if ( *arg == '\0' )
		return sprintf(data, "%03d.%03d.%03d.%03d", gateway.str[0], gateway.str[1], gateway.str[2], gateway.str[3]);
	return -1;
}

int para_dnsip(char *data, char *arg)
{
	NET_IPV4 dns;
	dns.int32 = net_get_dns();

	if ( *arg == '0' )
		return sprintf(data, "%d", dns.str[0]);
	else if ( *arg == '1' )
		return sprintf(data, "%d", dns.str[1]);
	else if ( *arg == '2' )
		return sprintf(data, "%d", dns.str[2]);
	else if ( *arg == '3' )
		return sprintf(data, "%d", dns.str[3]);
	else if ( *arg == '\0' )
		return sprintf(data, "%03d.%03d.%03d.%03d", dns.str[0], dns.str[1], dns.str[2], dns.str[3]);
	return -1;
}

int para_multicast(char *data, char *arg)
{
  SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.multicast_enable);
}

int para_sntpip(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%s", pSysInfo->lan_config.net.ntp_server);
}

int para_httpport(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.http_port);
}

int para_httpsport(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.net.https_port);
}
int para_giointype(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.giointype);
}
 int para_gioinname(char *data, char *arg)
{
  if ( *arg == '0' )
		return sprintf(data, "LOW");
	else if ( *arg == '1' )
		return sprintf(data, "HIGH");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "LOW;HIGH");
	return -1;
}
int para_gioouttype(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.gioouttype);
}
 int para_giooutname(char *data, char *arg)
{
  if ( *arg == '0' )
		return sprintf(data, "LOW");
	else if ( *arg == '1' )
		return sprintf(data, "HIGH");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "LOW;HIGH");
	return -1;
}
 int para_rs485(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.rs485config);
}
 int para_rs485name(char *data, char *arg)
{
#if 0
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "PTZ");
	else if ( *arg == '1' )
		return sprintf(data, "ALARM");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;PTZ;ALARM");
#else
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF");
#endif
	return -1;
}

/*************************** ALARM PAGE ***********************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
 int para_alarmenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nAlarmEnable);
}
int para_alarmduration(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nAlarmDuration);
}
int para_recordduration(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "10 seconds");
	else if ( *arg == '1' )
		return sprintf(data, "30 seconds");
	else if ( *arg == '2' )
		return sprintf(data, "1 minute");
	else if ( *arg == '3' )
		return sprintf(data, "5 minutes");
	else if ( *arg == '4' )
		return sprintf(data, "10 minutes");
	else if ( *arg == '5' )
		return sprintf(data, "Non-Stop");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "10 seconds;30 seconds;1 minute;5 minutes;10 minutes;Non-Stop");
	return -1;
}
int para_motionenable(char *data, char *arg)
{
  SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->motion_config.motionenable);
}
int para_lostalarm(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.lostalarm);
}
 int para_audioalarm(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->audio_config.alarmON);
}

int para_extalarm(char *data, char *arg)
{
  SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nExtAlarm);
}
int para_gioinenable(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.gioinenable);
}
 int para_giooutenable(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.giooutenable);
}

  int para_exttriggername(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "OFF");
	else if ( *arg == '1' )
		return sprintf(data, "ON");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "OFF;ON");
	return -1;
}
int para_aftpenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.bAFtpEnable);
}
 int para_ftpfileformat(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->ftp_config.ftpfileformat);
}

int para_asmtpenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.bASmtpEnable);
}
  int para_attfileformat(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->smtp_config.attfileformat);
}
int para_asmtpattach(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->smtp_config.attachments);
}
 int para_formatName(char *data, char *arg)
{
	int supportMpeg4=0,supportMpeg4cif=0 ,supportH264=0, supportH264cif=0, supportJPG=0;
	int supportAVI=0;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	supportJPG 		= pSysInfo->lan_config.Supportstream1;
	supportMpeg4 	= pSysInfo->lan_config.Supportstream2;
	supportMpeg4cif = pSysInfo->lan_config.Supportstream3;
	supportH264 	= pSysInfo->lan_config.Supportstream5;
	supportH264cif 	= pSysInfo->lan_config.Supportstream6;

	supportAVI = supportH264|supportH264cif|supportMpeg4|supportMpeg4cif;

	if (pSysInfo->sdcard_config.sdinsert==3) {
		if(supportJPG) {
			if ( *arg == '0' )
				return sprintf(data, "JPEG");
			else if ( *arg == 'a' || *arg == '\0' )
				return sprintf(data, "JPEG");
		}
		else {
			return sprintf(data, "NA");
		}
	}
	else {
		if ((supportAVI&&supportJPG)){
			if ( *arg == '0' )
				return sprintf(data, "AVI");
			else if(*arg =='1')
				return sprintf(data,"JPEG");
			else if ( *arg == 'a' || *arg == '\0' )
				return sprintf(data, "AVI;JPEG");
		}
		else if (supportAVI){
			if ( *arg == '0' )
				return sprintf(data, "AVI");
			else if ( *arg == 'a' || *arg == '\0' )
				return sprintf(data, "AVI");
		}
		else if (supportJPG){
			if ( *arg == '0' )
				return sprintf(data, "JPEG");
			else if ( *arg == 'a' || *arg == '\0' )
				return sprintf(data, "JPEG");
		}
	}

	return -1;
}

int para_sdaenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.bSdaEnable);
}
 int para_sdfileformat(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->sdcard_config.sdfileformat);
}
 int para_sdformatName(char *data, char *arg)
{
	int supportMpeg4=0,supportMpeg4cif=0 ,supportH264=0, supportH264cif=0, supportJPG=0;
	int supportAVI=0;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	supportJPG 		= pSysInfo->lan_config.Supportstream1;
	supportMpeg4 	= pSysInfo->lan_config.Supportstream2;
	supportMpeg4cif = pSysInfo->lan_config.Supportstream3;
	supportH264 	= pSysInfo->lan_config.Supportstream5;
	supportH264cif 	= pSysInfo->lan_config.Supportstream6;

	supportAVI = supportH264|supportH264cif|supportMpeg4|supportMpeg4cif;

    if ((supportAVI&&supportJPG)){
		if ( *arg == '0' )
			return sprintf(data, "AVI");
		else if(*arg =='1')
			return sprintf(data,"JPEG");
		else if ( *arg == 'a' || *arg == '\0' )
			return sprintf(data, "AVI;JPEG");
	}
	else if (supportAVI){
		if ( *arg == '0' )
			return sprintf(data, "AVI");
		else if ( *arg == 'a' || *arg == '\0' )
			return sprintf(data, "AVI");
	}
	else if (supportJPG){
		if ( *arg == '0' )
			return sprintf(data, "JPEG");
		else if ( *arg == 'a' || *arg == '\0' )
			return sprintf(data, "JPEG");
	}

	return -1;
}
 int para_alarmlocalstorage(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.alarmlocal);
}
 int para_alarmaudioplay(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nAlarmAudioPlay);
}
 int para_alarmaudiofile(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nAlarmAudioFile);
}
  int para_alarmaudiofilename(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "alarm_1.wav");
	if ( *arg == '1' )
		return sprintf(data, "alarm_2.wav");
	else if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "alarm_1.wav;alarm_2.wav");
	return -1;
}

/***************** STORAGE PAGE ***********************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int para_rftpenable(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->ftp_config.rftpenable);
}
 int para_sdrenable(char *data, char *arg)
{
    SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->sdcard_config.sdrenable);
}
int para_schedule(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	Schedule_t *pSchedule;
	int ret = 0, i, arg_i = atoi(arg);

	if(pSysInfo == NULL)
		return -1;
	 if ( *arg == 'a' || *arg == '\0' ){
		for(i = 0; i < SCHDULE_NUM; i ++){
			pSchedule = &(pSysInfo->lan_config.aSchedules[i]);
			ret += sprintf(data + ret,
				"%02d%d%02d%02d%02d%02d%02d%02d%02d\n", i,
				pSchedule -> bStatus, pSchedule -> nDay,
				pSchedule -> tStart.nHour, pSchedule -> tStart.nMin,
				pSchedule -> tStart.nSec, pSchedule -> tDuration.nHour,
				pSchedule -> tDuration.nMin, pSchedule -> tDuration.nSec);
		}
		return ret;
	} else if (arg_i >= 0 && arg_i < SCHDULE_NUM){
		pSchedule = &(pSysInfo->lan_config.aSchedules[arg_i]);
		return sprintf(data, "%d%02d%02d%02d%02d%02d%02d%02d",
				pSchedule -> bStatus, pSchedule -> nDay,
				pSchedule -> tStart.nHour, pSchedule -> tStart.nMin,
				pSchedule -> tStart.nSec, pSchedule -> tDuration.nHour,
				pSchedule -> tDuration.nMin, pSchedule -> tDuration.nSec);
	}
	return -1;
}
 int para_recordlocalstorage(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.recordlocal);
}

 int para_schedulerepeatenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nScheduleRepeatEnable);
}
 int para_schedulenumweeks(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nScheduleNumWeeks);
}
 int para_scheduleinfiniteenable(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.nScheduleInfiniteEnable);
}

/***************** Support Page ***********************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_kernelversion(char *data, char *arg)
{
	FILE *fptr;
	char TempBuff[80];
	char *pStr[3];
	int buffsize = sizeof(TempBuff);
	int cnt = 0;

	fptr=fopen("/proc/version","r");
	if (fptr==NULL){
	fprintf(stderr,"\n Can't get Version information ");
	return -1;
	}
	fread( TempBuff, buffsize-1, 1,fptr);
	TempBuff[buffsize-1] = '\0';

	pStr[0] = strtok(TempBuff, " ");

	for( cnt = 1;cnt < 3;cnt++ )
	{
		pStr[cnt] = strtok(NULL, " ");
	}
	fclose(fptr);
	return sprintf(data, "%s %s %s ",pStr[0],pStr[1],pStr[2]);

}

int para_biosversion(char *data, char *arg)
{
	int fd0;
	size_t size, cnt = 0, length;
	char readbuff[1024], *strptr, *target;
	fd0 = open("/dev/mtd0", O_RDONLY);
	if (fd0 == -1) {
		dbg("=======Open mtd0 failed========\n");
		return sprintf(data, "Open mtd0 failed");
	}

	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	if(pSysInfo->lan_config.chipConfig)
		size = lseek(fd0, 0x240000, SEEK_SET);
	else
		size = lseek(fd0, 0x6c000, SEEK_SET);

	if (size == -1) {
		dbg("=======lseek failed========\n");
		return sprintf(data, "lseek failed");
	}
	size = read(fd0, readbuff, 1024);
	dbg("#######size = %d\n", size);
	readbuff[1023] = '\0';
	strptr = readbuff;
	while (cnt < size) {
		length = strlen(strptr) + 1;
		if (strncmp(strptr, "ver", 3) == 0)
			break;
		strptr += length;
		cnt += length;
	}
	target = strtok(strptr, "=");
//	fprintf(stderr,"====stringA: %s\n", target);
	target = strtok(NULL, "=");
//	fprintf(stderr,"====stringB: %s\n", target);
	close(fd0);
	return sprintf(data, "%s", target);
}
int para_softwareversion(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	if(pSysInfo->lan_config.chipConfig)
		return sprintf(data, "%s", DM368_APP_VERSION);
	else
		return sprintf(data, "%s", DM365_APP_VERSION);
}

/************************** SDCARD EXPLORER PAGE **************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_sdinsert(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->sdcard_config.sdinsert);
}
 int para_sdleft(char *data, char *arg)
{
  long long freespace;
  freespace=GetDiskfreeSpace("/mnt/mmc/");

  return sprintf(data, "%lld",freespace*1024);
}
 int para_sdused(char *data, char *arg)
{
  long long usedspace;
  usedspace=GetDiskusedSpace("/mnt/mmc/");
  return sprintf(data,"%lld",usedspace*1024);
}
/************************** Others ****************************************/
/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int para_reloadflag(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;

	return sprintf(data, "%d", pSysInfo->lan_config.reloadFlag);
}

int para_reloadtime(char *data, char *arg)
{
	return sprintf(data, "%d", 20);
}

int para_dmvaenable(char *data, char *arg)
{
	return sprintf(data, "%d", 0);
}

int para_maxaccount(char *data, char *arg)
{
	return sprintf(data, "%d", ACOUNT_NUM);
}

int para_minnamelen(char *data, char *arg)
{
	return sprintf(data, "%d", 4);
}

int para_maxnamelen(char *data, char *arg)
{
	return sprintf(data, "%d", USER_LEN);
}

int para_minpwdlen(char *data, char *arg)
{
	return sprintf(data, "%d", 4);
}

int para_maxpwdlen(char *data, char *arg)
{
	return sprintf(data, "%d", PASSWORD_LEN);
}

int para_bkupfirmware(char *data, char *arg)
{
	return sprintf(data, "%d", 0);
}

 int para_sensorlinearwdr(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.linearwdr);
}

 int para_sensorlinearwdrname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "Linear");
	if ( *arg == '1' )
		return sprintf(data, "WDR");
	if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "Linear;WDR");

	return -1;
}

 int para_flickerdetect(char *data, char *arg)
{
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	return sprintf(data, "%d", pSysInfo->lan_config.flickerdetect);
}

 int para_flickerdetectname(char *data, char *arg)
{
	if ( *arg == '0' )
		return sprintf(data, "None");
	if ( *arg == '1' )
		return sprintf(data, "Unknow");
	if ( *arg == '2' )
		return sprintf(data, "50Hz");
	if ( *arg == '3' )
		return sprintf(data, "60Hz");
	if ( *arg == 'a' || *arg == '\0' )
		return sprintf(data, "None;Unknow;50Hz;60Hz");

	return -1;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
#define HASH_TABLE_SIZE	(sizeof(HttpArgument)/sizeof(HTML_ARGUMENT))

HTML_ARGUMENT HttpArgument [] =
{
	{ "regusrname"      , para_regusr               , AUTHORITY_VIEWER   	, NULL },
	{ "osdtextinfo"    	, para_osdtextinfo          , AUTHORITY_VIEWER   	, NULL },
	{ "maxtitlelen"     , para_maxtitlelen          , AUTHORITY_VIEWER 	, NULL },
	{ "liveresolution"  , para_liveresolution       , AUTHORITY_VIEWER   	, NULL },
	{ "mpeg4quality"    , para_mpeg4quality         , AUTHORITY_VIEWER 	, NULL },
	{ "maxfqdnlen"      , para_maxfqdnlen           , AUTHORITY_VIEWER 	, NULL },
	{ "maxdomainname"   , para_maxdomainname        , AUTHORITY_VIEWER 	, NULL },

	{ "supportmpeg4"    , para_supportmpeg4         , AUTHORITY_VIEWER   	, NULL },
	{ "format"          , para_format               , AUTHORITY_VIEWER   	, NULL },
	{ "imagesource"     , para_imagesource          , AUTHORITY_VIEWER   	, NULL },
	{ "advmode"     	, para_advancemode         	, AUTHORITY_VIEWER	, NULL },
	{ "advfeaturename"	, para_advfeaturename		, AUTHORITY_VIEWER	, NULL },
	{ "preprocess"		, para_preprocess			, AUTHORITY_VIEWER	, NULL },
	{ "preprocessname"	, para_preprocessname		, AUTHORITY_VIEWER	, NULL },
	{ "noisefilt"		, para_noisefilt			, AUTHORITY_VIEWER	, NULL },
	{ "noisefiltname"	, para_noisefiltname		, AUTHORITY_VIEWER	, NULL },
	{ "osdwin"     		, para_osdwin          		, AUTHORITY_VIEWER   	, NULL },
	{ "defaultstorage"  , para_defaultstorage       , AUTHORITY_VIEWER   	, NULL },
	{ "cfinsert"        , para_cfinsert             , AUTHORITY_VIEWER   	, NULL },
	{ "defaultcardgethtm", para_defaultcardgethtm   , AUTHORITY_VIEWER   	, NULL },
	{ "brandurl"         , para_brandurl            , AUTHORITY_VIEWER   	, NULL },
	{ "brandname"        , para_brandname           , AUTHORITY_VIEWER   	, NULL },
	{ "supporttstamp"    , para_supporttstamp       , AUTHORITY_VIEWER   	, NULL },
	{ "mpeg4xsize"       , para_mpeg4xsize          , AUTHORITY_VIEWER   	, NULL },
	{ "mpeg4ysize"       , para_mpeg4ysize          , AUTHORITY_VIEWER   	, NULL },
	{ "jpegxsize"        , para_jpegxsize           , AUTHORITY_VIEWER   	, NULL },
	{ "jpegysize"        , para_jpegysize           , AUTHORITY_VIEWER   	, NULL },
	{ "socketauthority"  , para_socketauthority     , AUTHORITY_VIEWER   	, NULL },
	{ "authoritychange"  , para_authoritychange     , AUTHORITY_VIEWER   	, NULL },
	{ "supportmotion"    , para_supportmotion       , AUTHORITY_VIEWER   	, NULL },
	{ "supportwireless"  , para_supportwireless     , AUTHORITY_VIEWER   	, NULL },
	{ "serviceftpclient" , para_serviceftpclient    , AUTHORITY_VIEWER   	, NULL },
	{ "servicesmtpclient", para_servicesmtpclient   , AUTHORITY_VIEWER   	, NULL },
	{ "servicepppoe"     , para_servicepppoe        , AUTHORITY_VIEWER   	, NULL },
	{ "servicesntpclient", para_servicesntpclient   , AUTHORITY_VIEWER   	, NULL },
	{ "serviceddnsclient", para_serviceddnsclient   , AUTHORITY_VIEWER   	, NULL },
	{ "supportmaskarea"  , para_supportmaskarea     , AUTHORITY_VIEWER   	, NULL },
	{ "machinecode"      , para_machinecode         , AUTHORITY_VIEWER   	, NULL },
	{ "maxchannel"       , para_maxchannel          , AUTHORITY_VIEWER   	, NULL },
	{ "supportrs485"     , para_supportrs485        , AUTHORITY_VIEWER   	, NULL },
	{ "supportrs232"     , para_supportrs232        , AUTHORITY_VIEWER   	, NULL },
	{ "layoutnum"        , para_layoutnum           , AUTHORITY_VIEWER   	, NULL },
	{ "supportmui"       , para_supportmui          , AUTHORITY_VIEWER   	, NULL },
	{ "mui"              , para_mui                 , AUTHORITY_VIEWER   	, NULL },
	{ "supportsequence"  , para_supportsequence     , AUTHORITY_VIEWER   	, NULL },
	{ "quadmodeselect"   , para_quadmodeselect      , AUTHORITY_VIEWER   	, NULL },
	{ "serviceipfilter"  , para_serviceipfilter     , AUTHORITY_VIEWER   	, NULL },
	{ "oemflag0"         , para_oemflag0            , AUTHORITY_VIEWER   	, NULL },
	{ "supportdncontrol" , para_supportdncontrol    , AUTHORITY_VIEWER   	, NULL },
	{ "supportavc"       , para_supportavc          , AUTHORITY_VIEWER   	, NULL },
	{ "supportaudio"     , para_supportaudio        , AUTHORITY_VIEWER   	, NULL },
	{ "supportptzpage"   , para_supportptzpage      , AUTHORITY_VIEWER   	, NULL },

	{ "dhcpconfig"			, para_dhcpconfig			, AUTHORITY_VIEWER 	, NULL },
	{ "timeformat"			, para_timeformat			, AUTHORITY_VIEWER   	, NULL },
	{ "pppoeenable"			, para_pppoeenable			, AUTHORITY_VIEWER 	, NULL },
	{ "mpeg4desired"		, para_mpeg4desired			, AUTHORITY_VIEWER 	, NULL },
	{ "mpeg4cenable"		, para_mpeg4cenable			, AUTHORITY_VIEWER 	, NULL },
	{ "mpeg4cvalue"			, para_mpeg4cvalue			, AUTHORITY_VIEWER 	, NULL },
	{ "mpeg42cvalue"		, para_mpeg42cvalue			, AUTHORITY_VIEWER 	, NULL },
	{ "mpeg4resname"		, para_mpeg4resname			, AUTHORITY_VIEWER 	, NULL },
	{ "mpeg4resolution"		, para_mpeg4resolution      , AUTHORITY_VIEWER 	, NULL },
	{ "mpeg42resname"		, para_mpeg42resname		, AUTHORITY_VIEWER 	, NULL },
	{ "mpeg42resolution"	, para_mpeg42resolution		, AUTHORITY_VIEWER 	, NULL },
	{ "resolutionname"		, para_resolutionname		, AUTHORITY_VIEWER 	, NULL },
	{ "mpeg4qualityname"	, para_mpeg4qualityname		, AUTHORITY_VIEWER 	, NULL },
	{ "waitserver"			, para_waitserver			, AUTHORITY_VIEWER 	, NULL },
	{ "supportcolorkiller"	, para_supportcolorkiller	, AUTHORITY_VIEWER   	, NULL },
	{ "supportAWB"			, para_supportAWB			, AUTHORITY_VIEWER   	, NULL },
	{ "supportbrightness"	, para_supportbrightness	, AUTHORITY_VIEWER   	, NULL },
	{ "supportcontrast"		, para_supportcontrast		, AUTHORITY_VIEWER   	, NULL },
	{ "supportsaturation"	, para_supportsaturation	, AUTHORITY_VIEWER   	, NULL },
	{ "supportbacklight"	, para_supportbacklight		, AUTHORITY_VIEWER   	, NULL },
	{ "supportsharpness"	, para_supportsharpness		, AUTHORITY_VIEWER   	, NULL },

	{ "quadmodeselectname"	, para_quadmodeselectname	, AUTHORITY_VIEWER , NULL },
    { "supportagc"		, para_supportagc				, AUTHORITY_VIEWER , NULL },
	{ "agc"					, para_agc					, AUTHORITY_VIEWER , NULL },
    { "fluorescent"		, para_fluorescent				, AUTHORITY_VIEWER , NULL },
	{ "mirror"			    , para_mirror				, AUTHORITY_VIEWER , NULL },
    { "kelvin"			, para_kelvin					, AUTHORITY_VIEWER , NULL },
    { "supporthue"		, para_supporthue				, AUTHORITY_VIEWER , NULL },
    { "supportexposure"		, para_supportexposure		, AUTHORITY_VIEWER , NULL },
    { "supportfluorescent"	, para_supportfluorescent	, AUTHORITY_VIEWER , NULL },
    { "supportmirros"		, para_supportmirros		, AUTHORITY_VIEWER , NULL },
    { "supportkelvin"		, para_supportkelvin		, AUTHORITY_VIEWER , NULL },
	{ "senseup"			, para_senseup					, AUTHORITY_VIEWER , NULL },
	{ "supportsenseup"		, para_supportsenseup		, AUTHORITY_VIEWER , NULL },
	{ "supportmaxagcgain"	, para_supportmaxagcgain		, AUTHORITY_VIEWER , NULL },
	{ "supporthspeedshutter"	, para_supporthspeedshutter	, AUTHORITY_VIEWER , NULL },
	{ "hspeedshutter"		, para_hspeedshutter			, AUTHORITY_VIEWER , NULL },
	{ "maxagcgainname"		, para_maxagcgainname		, AUTHORITY_VIEWER , NULL },
	{ "maxagcgain"		, para_maxagcgain				, AUTHORITY_VIEWER , NULL },

	{ "timeformatname"		, para_timeformatname		, AUTHORITY_VIEWER , NULL },

	{ "motionxlimit"			, para_motionxlimit	    	, AUTHORITY_VIEWER , NULL },
	{ "motionylimit"	    	, para_motionylimit			, AUTHORITY_VIEWER , NULL },
	{ "motionxblock"			, para_motionxblock		    , AUTHORITY_VIEWER , NULL },
	{ "motionyblock"			, para_motionyblock		    , AUTHORITY_VIEWER , NULL },
	{ "authorityadmin"		, para_authorityadmin		, AUTHORITY_VIEWER , NULL },
	{ "authorityoperator"		, para_authorityoperator	, AUTHORITY_VIEWER , NULL },
	{ "authorityviewer"		, para_authorityviewer		, AUTHORITY_VIEWER , NULL },

	{ "user"			    , para_user				    , AUTHORITY_VIEWER , NULL },
	{ "authority"		    , para_authority	   	    , AUTHORITY_VIEWER , NULL },
    { "gioinenable"		    , para_gioinenable 		, AUTHORITY_VIEWER , NULL },
    { "giooutenable"	    , para_giooutenable 	 , AUTHORITY_VIEWER , NULL },
	{ "tstampenable"	    , para_tstampenable	     , AUTHORITY_VIEWER , NULL },
	{ "tstampcolorname"	    , para_tstampcolorname	 , AUTHORITY_VIEWER , NULL },
	{ "tstamplocname"	    , para_tstamplocname	 , AUTHORITY_VIEWER , NULL },
    { "eventlocation"	    , para_event_location  		, AUTHORITY_VIEWER , NULL },
    { "event"	    		, para_get_event  		, AUTHORITY_VIEWER , NULL },

	{ "stream1xsize"		, para_stream1xsize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream1ysize"		, para_stream1ysize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream2xsize"		, para_stream2xsize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream2ysize"		, para_stream2ysize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream3xsize"		, para_stream3xsize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream3ysize"		, para_stream3ysize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream4xsize"		, para_stream1xsize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream4ysize"		, para_stream1ysize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream5xsize"		, para_stream5xsize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream5ysize"		, para_stream5ysize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream6xsize"		, para_stream6xsize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream6ysize"		, para_stream6ysize	  	, AUTHORITY_VIEWER , NULL },
	{ "stream1name"			, para_stream1name	  	, AUTHORITY_VIEWER , NULL },
	{ "stream2name"			, para_stream2name	  	, AUTHORITY_VIEWER , NULL },
	{ "stream3name"			, para_stream3name	  	, AUTHORITY_VIEWER , NULL },
	{ "stream4name"			, para_stream4name	  	, AUTHORITY_VIEWER , NULL },
	{ "stream5name"			, para_stream5name	  	, AUTHORITY_VIEWER , NULL },
	{ "stream6name"			, para_stream6name	  	, AUTHORITY_VIEWER , NULL },

	{ "supportstream1"		, para_supportstream1	, AUTHORITY_VIEWER , NULL },
	{ "supportstream2"		, para_supportstream2	, AUTHORITY_VIEWER , NULL },
	{ "supportstream3"		, para_supportstream3	, AUTHORITY_VIEWER , NULL },
	{ "supportstream4"		, para_supportstream4	, AUTHORITY_VIEWER , NULL },
	{ "supportstream5"		, para_supportstream5	, AUTHORITY_VIEWER , NULL },
	{ "supportstream6"		, para_supportstream6	, AUTHORITY_VIEWER , NULL },
	{ "osdstream"			, para_osdstream   		, AUTHORITY_VIEWER , NULL },
	{ "osdstreamname"		, para_osdstreamname	, AUTHORITY_VIEWER , NULL },
	{ "osdwinnum"			, para_osdwinnum   		, AUTHORITY_VIEWER , NULL },
	{ "osdwinnumname"		, para_osdwinnumname	, AUTHORITY_VIEWER , NULL },
	{ "ratecontrol"			, para_ratecontrol		, AUTHORITY_VIEWER , NULL },

	/*		LIVE VIDEO SCREEN 	*/
	{ "democfg"				, para_democfg 				, AUTHORITY_VIEWER , NULL },
	{ "democfgname"			, para_democfgname			, AUTHORITY_VIEWER , NULL },
    { "clicksnapfilename"   , para_clicksnapfilename 	, AUTHORITY_VIEWER , NULL },
    { "clicksnapstorage"    , para_clicksnapstorage  	, AUTHORITY_VIEWER , NULL },
	{ "streamname1"			, para_streamname1			, AUTHORITY_VIEWER , NULL },
	{ "streamname2"			, para_streamname2			, AUTHORITY_VIEWER , NULL },
	{ "streamname3"			, para_streamname3			, AUTHORITY_VIEWER , NULL },
	{ "streamwidth1"		, para_streamwidth1			, AUTHORITY_VIEWER , NULL },
	{ "streamwidth2"		, para_streamwidth2			, AUTHORITY_VIEWER , NULL },
	{ "streamwidth3"		, para_streamwidth3			, AUTHORITY_VIEWER , NULL },
	{ "streamheight1"		, para_streamheight1		, AUTHORITY_VIEWER , NULL },
	{ "streamheight2"		, para_streamheight2		, AUTHORITY_VIEWER , NULL },
	{ "streamheight3"		, para_streamheight3		, AUTHORITY_VIEWER , NULL },

	/*		VIDEO / IMAGE SCREEN 	*/
	{ "title"           	, para_title                , AUTHORITY_VIEWER , NULL },
    { "videocodec"		    , para_videocodec		    , AUTHORITY_VIEWER , NULL },
    { "videocodecname"		, para_videocodecname		, AUTHORITY_VIEWER , NULL },
    { "videocodeccombo"		, para_videocodeccombo		, AUTHORITY_VIEWER , NULL },
	{ "videocodeccomboname"	, para_videocodeccomboname	, AUTHORITY_VIEWER , NULL },
	{ "videocodecres"		, para_videocodecres		, AUTHORITY_VIEWER , NULL },
	{ "videocodecresname"	, para_videocodecresname	, AUTHORITY_VIEWER , NULL },
	{ "bitrate1"			, para_mpeg4cvalue			, AUTHORITY_VIEWER , NULL },
	{ "bitrate2"			, para_mpeg42cvalue			, AUTHORITY_VIEWER , NULL },
	{ "livequality"     	, para_livequality          , AUTHORITY_VIEWER , NULL },
	{ "qualityname"			, para_qualityname			, AUTHORITY_VIEWER , NULL },
	{ "framerate1"			, para_framerate1			, AUTHORITY_VIEWER , NULL },
	{ "framerate2"			, para_framerate2			, AUTHORITY_VIEWER , NULL },
	{ "framerate3"			, para_framerate3			, AUTHORITY_VIEWER , NULL },
	{ "frameratenameall1"	, para_frameratenameall1	, AUTHORITY_VIEWER , NULL },
	{ "frameratenameall2"	, para_frameratenameall2	, AUTHORITY_VIEWER , NULL },
	{ "frameratenameall3"	, para_frameratenameall3	, AUTHORITY_VIEWER , NULL },
    { "ratecontrol1"        , para_ratecontrol1   		, AUTHORITY_VIEWER , NULL },
    { "ratecontrol2"        , para_ratecontrol2   		, AUTHORITY_VIEWER , NULL },
	{ "ratecontrolname"		, para_ratecontrolname		, AUTHORITY_VIEWER , NULL },
	{ "datestampenable1"    , para_datestampenable1    	, AUTHORITY_VIEWER , NULL },
	{ "datestampenable2"    , para_datestampenable2    	, AUTHORITY_VIEWER , NULL },
	{ "datestampenable3"    , para_datestampenable3    	, AUTHORITY_VIEWER , NULL },
	{ "timestampenable1"    , para_timestampenable1    	, AUTHORITY_VIEWER , NULL },
	{ "timestampenable2"    , para_timestampenable2    	, AUTHORITY_VIEWER , NULL },
	{ "timestampenable3"    , para_timestampenable3    	, AUTHORITY_VIEWER , NULL },
	{ "logoenable1"         , para_logoenable1      	, AUTHORITY_VIEWER , NULL },
	{ "logoenable2"         , para_logoenable2      	, AUTHORITY_VIEWER , NULL },
	{ "logoenable3"         , para_logoenable3      	, AUTHORITY_VIEWER , NULL },
	{ "logoposition1"       , para_logoposition1    	, AUTHORITY_VIEWER , NULL },
	{ "logoposition2"       , para_logoposition2    	, AUTHORITY_VIEWER , NULL },
	{ "logoposition3"       , para_logoposition3    	, AUTHORITY_VIEWER , NULL },
	{ "logopositionname"    , para_logopositionname 	, AUTHORITY_VIEWER , NULL },
	{ "textenable1"         , para_textenable1      	, AUTHORITY_VIEWER , NULL },
	{ "textenable2"         , para_textenable2      	, AUTHORITY_VIEWER , NULL },
	{ "textenable3"         , para_textenable3      	, AUTHORITY_VIEWER , NULL },
    { "textposition1"       , para_textposition1    	, AUTHORITY_VIEWER , NULL },
    { "textposition2"       , para_textposition2    	, AUTHORITY_VIEWER , NULL },
    { "textposition3"       , para_textposition3    	, AUTHORITY_VIEWER , NULL },
	{ "textpositionname"    , para_textpositionname 	, AUTHORITY_VIEWER , NULL },
	{ "overlaytext1"    	, para_overlaytext1     	, AUTHORITY_VIEWER , NULL },
	{ "overlaytext2"    	, para_overlaytext2     	, AUTHORITY_VIEWER , NULL },
	{ "overlaytext3"    	, para_overlaytext3     	, AUTHORITY_VIEWER , NULL },
	{ "detailinfo1"         , para_detailinfo1      	, AUTHORITY_VIEWER , NULL },
	{ "detailinfo2"         , para_detailinfo2      	, AUTHORITY_VIEWER , NULL },
	{ "detailinfo3"         , para_detailinfo3      	, AUTHORITY_VIEWER , NULL },
	{ "encryptvideo"        , para_encryptvideo     	, AUTHORITY_VIEWER , NULL },
	{ "mirctrl"				, para_mirctrl 				, AUTHORITY_VIEWER , NULL },
	{ "mirctrlname"			, para_mirctrlname			, AUTHORITY_VIEWER , NULL },
	{ "localdisplay"        , para_localdisplay     	, AUTHORITY_VIEWER , NULL },
	{ "localdisplayname"    , para_localdisplayname		, AUTHORITY_VIEWER , NULL },
 	{ "aviformat"	        , para_aviformat			, AUTHORITY_VIEWER , NULL },
	{ "aviformatname"	    , para_aviformatname		, AUTHORITY_VIEWER , NULL },
    { "aviduration"	        , para_aviduration			, AUTHORITY_VIEWER , NULL },
	{ "avidurationname"	    , para_avidurationname		, AUTHORITY_VIEWER , NULL },

	/*	Video Advanced Setting Page    */
	{ "ipratio1"                 , para_ipratio1               , AUTHORITY_VIEWER , NULL },
	{ "ipratio2"                 , para_ipratio2               , AUTHORITY_VIEWER , NULL },
	{ "ipratio3"                 , para_ipratio3               , AUTHORITY_VIEWER , NULL },
	{ "forceiframe1"             , para_forceIframe1           , AUTHORITY_VIEWER , NULL },
	{ "forceiframe2"             , para_forceIframe2           , AUTHORITY_VIEWER , NULL },
	{ "forceiframe3"             , para_forceIframe3           , AUTHORITY_VIEWER , NULL },
	{ "qpinit1"          , para_qpinit1          , AUTHORITY_VIEWER , NULL },
	{ "qpinit2"          , para_qpinit2          , AUTHORITY_VIEWER , NULL },
	{ "qpinit3"          , para_qpinit3          , AUTHORITY_VIEWER , NULL },
	{ "qpmin1"           , para_qpmin1           , AUTHORITY_VIEWER , NULL },
	{ "qpmin2"           , para_qpmin2           , AUTHORITY_VIEWER , NULL },
	{ "qpmin3"           , para_qpmin3           , AUTHORITY_VIEWER , NULL },
	{ "qpmax1"           , para_qpmax1           , AUTHORITY_VIEWER , NULL },
	{ "qpmax2"           , para_qpmax2           , AUTHORITY_VIEWER , NULL },
	{ "qpmax3"           , para_qpmax3           , AUTHORITY_VIEWER , NULL },
	{ "meconfig1"        , para_meconfig1        , AUTHORITY_VIEWER , NULL },
	{ "meconfig2"        , para_meconfig2        , AUTHORITY_VIEWER , NULL },
	{ "meconfig3"        , para_meconfig3        , AUTHORITY_VIEWER , NULL },
	{ "meconfigname"     , para_meconfigname     , AUTHORITY_VIEWER , NULL },
	{ "packetsize1"      , para_packetsize1      , AUTHORITY_VIEWER , NULL },
	{ "packetsize2"      , para_packetsize2      , AUTHORITY_VIEWER , NULL },
	{ "packetsize3"      , para_packetsize3      , AUTHORITY_VIEWER , NULL },
	{ "regionofinterestenable1"    , para_regionofinterestenable1     , AUTHORITY_VIEWER , NULL },
	{ "regionofinterestenable2"    , para_regionofinterestenable2     , AUTHORITY_VIEWER , NULL },
	{ "regionofinterestenable3"    , para_regionofinterestenable3     , AUTHORITY_VIEWER , NULL },
	{ "str1x1"            , para_str1x1          , AUTHORITY_VIEWER , NULL },
      { "str1y1"            , para_str1y1          , AUTHORITY_VIEWER , NULL },
	{ "str1w1"            , para_str1w1          , AUTHORITY_VIEWER , NULL },
      { "str1h1"            , para_str1h1          , AUTHORITY_VIEWER , NULL },
	{ "str1x2"            , para_str1x2          , AUTHORITY_VIEWER , NULL },
      { "str1y2"            , para_str1y2          , AUTHORITY_VIEWER , NULL },
	{ "str1w2"            , para_str1w2          , AUTHORITY_VIEWER , NULL },
      { "str1h2"            , para_str1h2          , AUTHORITY_VIEWER , NULL },
	{ "str1x3"            , para_str1x3          , AUTHORITY_VIEWER , NULL },
      { "str1y3"            , para_str1y3          , AUTHORITY_VIEWER , NULL },
	{ "str1w3"            , para_str1w3          , AUTHORITY_VIEWER , NULL },
      { "str1h3"            , para_str1h3          , AUTHORITY_VIEWER , NULL },
	{ "str2x1"            , para_str2x1          , AUTHORITY_VIEWER , NULL },
      { "str2y1"            , para_str2y1          , AUTHORITY_VIEWER , NULL },
	{ "str2w1"            , para_str2w1          , AUTHORITY_VIEWER , NULL },
      { "str2h1"            , para_str2h1          , AUTHORITY_VIEWER , NULL },
	{ "str2x2"            , para_str2x2          , AUTHORITY_VIEWER , NULL },
      { "str2y2"            , para_str2y2          , AUTHORITY_VIEWER , NULL },
	{ "str2w2"            , para_str2w2          , AUTHORITY_VIEWER , NULL },
      { "str2h2"            , para_str2h2          , AUTHORITY_VIEWER , NULL },
	{ "str2x3"            , para_str2x3          , AUTHORITY_VIEWER , NULL },
      { "str2y3"            , para_str2y3          , AUTHORITY_VIEWER , NULL },
	{ "str2w3"            , para_str2w3          , AUTHORITY_VIEWER , NULL },
      { "str2h3"            , para_str2h3          , AUTHORITY_VIEWER , NULL },
	{ "str3x1"            , para_str3x1          , AUTHORITY_VIEWER , NULL },
      { "str3y1"            , para_str3y1          , AUTHORITY_VIEWER , NULL },
	{ "str3w1"            , para_str3w1          , AUTHORITY_VIEWER , NULL },
      { "str3h1"            , para_str3h1          , AUTHORITY_VIEWER , NULL },
	{ "str3x2"            , para_str3x2          , AUTHORITY_VIEWER , NULL },
      { "str3y2"            , para_str3y2          , AUTHORITY_VIEWER , NULL },
	{ "str3w2"            , para_str3w2          , AUTHORITY_VIEWER , NULL },
      { "str3h2"            , para_str3h2          , AUTHORITY_VIEWER , NULL },
	{ "str3x3"            , para_str3x3          , AUTHORITY_VIEWER , NULL },
      { "str3y3"            , para_str3y3          , AUTHORITY_VIEWER , NULL },
	{ "str3w3"            , para_str3w3          , AUTHORITY_VIEWER , NULL },
      { "str3h3"            , para_str3h3          , AUTHORITY_VIEWER , NULL },

	/*      VIDEO ANALYTICS SCREEN 				*/
	{ "motionblock"		    , para_motionblock			, AUTHORITY_VIEWER 	, NULL },
	{ "motionsensitivity"	, para_motionsensitivity	, AUTHORITY_VIEWER 	, NULL },
	{ "motionname"		    , para_motionname			, AUTHORITY_VIEWER 	, NULL },
	{ "motioncenable"		, para_motioncenable		, AUTHORITY_VIEWER 	, NULL },
	{ "motioncvalue"      	, para_motioncvalue			, AUTHORITY_VIEWER 	, NULL },
	{ "fdetect"				, para_fdetect 				, AUTHORITY_VIEWER	, NULL },
	{ "fdetectname"			, para_fdetectname			, AUTHORITY_VIEWER	, NULL },
	{ "frecognition"        , para_frecognition       	, AUTHORITY_VIEWER 	, NULL },
	{ "frecognitionname"    , para_frecognitionname   	, AUTHORITY_VIEWER 	, NULL },
	{ "privacymask"         , para_privacymask       	, AUTHORITY_VIEWER 	, NULL },
	{ "maskoptions"         , para_maskoptions       	, AUTHORITY_VIEWER 	, NULL },
	{ "maskoptionsname"     , para_maskoptionsname   	, AUTHORITY_VIEWER 	, NULL },
	{ "fdconflevel"         , para_fdconflevel  		, AUTHORITY_VIEWER 	, NULL },
	{ "fdx"                 , para_fdx          		, AUTHORITY_VIEWER 	, NULL },
	{ "fdy"                 , para_fdy          		, AUTHORITY_VIEWER 	, NULL },
	{ "fdw"                 , para_fdw          		, AUTHORITY_VIEWER 	, NULL },
	{ "fdh"                 , para_fdh          		, AUTHORITY_VIEWER 	, NULL },
	{ "frconflevel"         , para_frconflevel  		, AUTHORITY_VIEWER 	, NULL },
	{ "fddirection"         , para_fddirection      	, AUTHORITY_VIEWER 	, NULL },
	{ "fddirectionname"     , para_fddirectionname  	, AUTHORITY_VIEWER 	, NULL },
	{ "frdatabase"          , para_frdatabase       	, AUTHORITY_VIEWER 	, NULL },

	/*      CAMERA SCREEN 				*/
    { "brightness"				, para_brightness			, AUTHORITY_VIEWER 		, NULL },
    { "contrast"				, para_contrast				, AUTHORITY_VIEWER 		, NULL },
 	{ "sharpness"				, para_sharpness			, AUTHORITY_VIEWER 		, NULL },
    { "saturation"				, para_saturation			, AUTHORITY_VIEWER 		, NULL },
  	{ "blc"						, para_blc					, AUTHORITY_VIEWER 		, NULL },
 	{ "backlight"				, para_backlight			, AUTHORITY_VIEWER 		, NULL },
 	{ "backlightname"			, para_backlightname		, AUTHORITY_VIEWER 		, NULL },
	{ "dynrange"    			, para_dynrange        		, AUTHORITY_VIEWER   	, NULL },
	{ "dynrangename"   			, para_dynrangename    		, AUTHORITY_VIEWER   	, NULL },
 	{ "awb"				    	, para_awb					, AUTHORITY_VIEWER 		, NULL },
    { "awbname"					, para_awbname				, AUTHORITY_VIEWER 		, NULL },
	{ "colorkiller"				, para_daynight				, AUTHORITY_VIEWER 		, NULL },
    { "daynightname"			, para_daynightname			, AUTHORITY_VIEWER 		, NULL },
    { "histogram"     			, para_histogram          	, AUTHORITY_VIEWER   	, NULL },
	{ "vidstb1"  				, para_vidstb				, AUTHORITY_VIEWER 		, NULL },
	{ "lensdistortcorrection"  	, para_ldc					, AUTHORITY_VIEWER 		, NULL },
    { "binning"					, para_binning				, AUTHORITY_VIEWER 		, NULL },
    { "binningname"				, para_binningname			, AUTHORITY_VIEWER 		, NULL },
	{ "img2a"					, para_image2a				, AUTHORITY_VIEWER 		, NULL },
	{ "img2aname"				, para_img2aname			, AUTHORITY_VIEWER 		, NULL },
	{ "img2atype"				, para_image2atype			, AUTHORITY_VIEWER 		, NULL },
	{ "img2atypename"			, para_img2atypename		, AUTHORITY_VIEWER 		, NULL },
	{ "maxexposuretime"         , para_maxexposuretime      , AUTHORITY_VIEWER 	, NULL },
    { "maxexposuretimename"     , para_maxexposuretimename  , AUTHORITY_VIEWER 	, NULL },
	{ "maxgain"                 , para_maxgain              , AUTHORITY_VIEWER 	, NULL },
    { "maxgainname"             , para_maxgainname          , AUTHORITY_VIEWER 	, NULL },
	{ "exposurectrl"		    , para_imagesource	    	, AUTHORITY_VIEWER 		, NULL },
	{ "exposurename"		    , para_exposurename		    , AUTHORITY_VIEWER 		, NULL },
	{ "priority"		    	, para_priority		    	, AUTHORITY_VIEWER 		, NULL },
	{ "priorityname"		    , para_priorityname	    	, AUTHORITY_VIEWER 		, NULL },
   	{ "nfltctrl"                , para_snfltctrl            , AUTHORITY_VIEWER 	, NULL },
   	{ "nfltctrlname"            , para_snfltctrlname        , AUTHORITY_VIEWER 	, NULL },
	{ "tnfltctrl"               , para_tnfltctrl            , AUTHORITY_VIEWER 	, NULL },

	/*      AUDIO SCREEN 				*/
	{ "audioenable"				, para_audioenable			, AUTHORITY_VIEWER   , NULL },
	{ "audiomode"              	, para_audiomode            , AUTHORITY_VIEWER , NULL },
	{ "audiomodename"          	, para_audiomodename        , AUTHORITY_VIEWER , NULL },
	{ "audioinvolume"          	, para_audioinvolume        , AUTHORITY_VIEWER , NULL },
	{ "encoding"               	, para_encoding             , AUTHORITY_VIEWER , NULL },
	{ "encodingname"           	, para_encodingname         , AUTHORITY_VIEWER , NULL },
	{ "samplerate"             	, para_samplerate           , AUTHORITY_VIEWER , NULL },
	{ "sampleratename"         	, para_sampleratename       , AUTHORITY_VIEWER , NULL },
	{ "audiobitrate"           	, para_audiobitrate         , AUTHORITY_VIEWER , NULL },
	{ "audiobitratename"       	, para_audiobitratename     , AUTHORITY_VIEWER , NULL },
	{ "audiobitratenameall"    	, para_audiobitratenameall  , AUTHORITY_VIEWER , NULL },
	{ "alarmlevel"             	, para_alarmlevel           , AUTHORITY_VIEWER , NULL },
	{ "audiooutvolume"       	, para_audiooutvolume     	, AUTHORITY_VIEWER , NULL },
	{ "audioreceiverenable"     , para_audioreceiverenable  , AUTHORITY_VIEWER , NULL },
	{ "audioserverip"           , para_audioserverip        , AUTHORITY_VIEWER , NULL },

	/*      DATE TIME SCREEN				*/
	{ "date"            		, para_date                 , AUTHORITY_VIEWER  , NULL },
	{ "time"            		, para_time                 , AUTHORITY_VIEWER  , NULL },
	{ "timezone"    			, para_sntptimezone         , AUTHORITY_VIEWER 	, NULL },
	{ "timezonename"			, para_timezonename			, AUTHORITY_VIEWER 	, NULL },
	{ "daylight"				, para_daylight				, AUTHORITY_VIEWER  , NULL },
    { "dateformat"				, para_dateformat			, AUTHORITY_VIEWER  , NULL },
    { "dateformatname"         	, para_dateformatname       , AUTHORITY_VIEWER 	, NULL },
    { "tstampformat"        	, para_tstampformat      	, AUTHORITY_VIEWER 	, NULL },
	{ "tstampformatname"    	, para_tstampformatname  	, AUTHORITY_VIEWER 	, NULL },
	{ "dateposition"           	, para_dateposition         , AUTHORITY_VIEWER 	, NULL },
	{ "timeposition"           	, para_timeposition         , AUTHORITY_VIEWER 	, NULL },
	{ "datetimepositionname"   	, para_datetimepositionname , AUTHORITY_VIEWER 	, NULL },

	/*      NETWORK PORT				*/
	{ "dhcpenable"      , para_dhcpenable           , AUTHORITY_VIEWER , NULL },
	{ "netip"           , para_netip                , AUTHORITY_VIEWER , NULL },
	{ "netmask"         , para_netmask              , AUTHORITY_VIEWER , NULL },
	{ "gateway"         , para_gateway              , AUTHORITY_VIEWER , NULL },
	{ "dnsip"           , para_dnsip                , AUTHORITY_VIEWER , NULL },

	{ "ftpip"           , para_ftpip                , AUTHORITY_VIEWER , NULL },
	{ "ftpipport"       , para_ftpipport            , AUTHORITY_VIEWER , NULL },
	{ "ftpuser"         , para_ftpuser              , AUTHORITY_VIEWER , NULL },
	{ "ftppassword"     , para_ftppassword          , AUTHORITY_VIEWER , NULL },
	{ "ftppath"         , para_ftppath              , AUTHORITY_VIEWER , NULL },
	{ "maxftpuserlen"   , para_maxftpuserlen        , AUTHORITY_VIEWER , NULL },
	{ "maxftppwdlen"    , para_maxftppwdlen         , AUTHORITY_VIEWER , NULL },
	{ "maxftppathlen"   , para_maxftppathlen        , AUTHORITY_VIEWER , NULL },

	{ "smtpauth"        , para_smtpauth             , AUTHORITY_VIEWER , NULL },
	{ "smtpuser"        , para_smtpuser             , AUTHORITY_VIEWER , NULL },
	{ "maxsmtpuser"     , para_maxsmtpuser          , AUTHORITY_VIEWER , NULL },
	{ "smtppwd"         , para_smtppwd              , AUTHORITY_VIEWER , NULL },
	{ "maxsmtppwd"      , para_maxsmtppwd           , AUTHORITY_VIEWER , NULL },
	{ "smtpsender"      , para_smtpsender           , AUTHORITY_VIEWER , NULL },
	{ "maxsmtpsender"   , para_maxsmtpsender        , AUTHORITY_VIEWER , NULL },
	{ "smtpip"          , para_smtpip               , AUTHORITY_VIEWER , NULL },
	{ "smtpport"        , para_smtpport             , AUTHORITY_VIEWER , NULL },
	{ "emailuser"       , para_emailuser            , AUTHORITY_VIEWER , NULL },
	{ "maxemailuserlen" , para_maxemailuserlen      , AUTHORITY_VIEWER , NULL },

	{ "multicast"		, para_multicast  			, AUTHORITY_VIEWER , NULL },

	{ "sntpip"          , para_sntpip               , AUTHORITY_VIEWER , NULL },

	{ "httpport"        , para_httpport             , AUTHORITY_VIEWER , NULL },
	{ "httpsport"       , para_httpsport         	, AUTHORITY_VIEWER , NULL },
    { "portinput"		, para_giointype  		 	, AUTHORITY_VIEWER , NULL },
    { "portinputname"	, para_gioinname  		 	, AUTHORITY_VIEWER , NULL },
    { "portoutput"		, para_gioouttype  		 	, AUTHORITY_VIEWER , NULL },
    { "portoutputname"	, para_giooutname  		 	, AUTHORITY_VIEWER , NULL },
	{ "rs485"           , para_rs485             	, AUTHORITY_VIEWER , NULL },
	{ "rs485name"       , para_rs485name         	, AUTHORITY_VIEWER , NULL },

	/*      ALARM PAGE				*/
	{ "alarmenable"         , para_alarmenable      		, AUTHORITY_VIEWER , NULL },
    { "alarmduration"		, para_alarmduration			, AUTHORITY_VIEWER , NULL },
    { "recordduration"		, para_recordduration			, AUTHORITY_VIEWER , NULL },
	{ "motionenable"		, para_motionenable				, AUTHORITY_VIEWER , NULL },
    { "lostalarm"		    , para_lostalarm				, AUTHORITY_VIEWER , NULL },
    { "darkblankalarm"      , para_audioalarm   			, AUTHORITY_VIEWER , NULL },
    { "audioalarm"      	, para_audioalarm   			, AUTHORITY_VIEWER , NULL },
	{ "extalarm"            , para_extalarm         		, AUTHORITY_VIEWER , NULL },
	{ "exttriggerinput"     , para_gioinenable  			, AUTHORITY_VIEWER , NULL },
    { "exttriggeroutput"    , para_giooutenable 			, AUTHORITY_VIEWER , NULL },
 	{ "exttriggername"      , para_exttriggername   		, AUTHORITY_VIEWER , NULL },
    { "aftpenable"			, para_aftpenable				, AUTHORITY_VIEWER , NULL },
 	{ "ftpfileformat"	    , para_ftpfileformat			, AUTHORITY_VIEWER , NULL },
	{ "ftpfileformatname"   , para_formatName				, AUTHORITY_VIEWER , NULL },
    { "asmtpenable"			, para_asmtpenable				, AUTHORITY_VIEWER , NULL },
    { "attfileformat"	    , para_attfileformat			, AUTHORITY_VIEWER , NULL },
	{ "attfileformatname"   , para_formatName				, AUTHORITY_VIEWER , NULL },
	{ "asmtpattach"		    , para_asmtpattach				, AUTHORITY_VIEWER , NULL },
	{ "smtpminattach"       , para_smtpminattach			, AUTHORITY_VIEWER , NULL },
	{ "smtpmaxattach"       , para_smtpmaxattach			, AUTHORITY_VIEWER , NULL },
    { "sdaenable"		    , para_sdaenable				, AUTHORITY_VIEWER , NULL },
    { "sdfileformat"	    , para_sdfileformat				, AUTHORITY_VIEWER , NULL },
	{ "sdfileformatname"    , para_sdformatName				, AUTHORITY_VIEWER , NULL },
	{ "alarmlocalstorage"   , para_alarmlocalstorage        , AUTHORITY_VIEWER , NULL },
	{ "alarmaudioplay"      , para_alarmaudioplay           , AUTHORITY_VIEWER , NULL },
	{ "alarmaudiofile"      , para_alarmaudiofile           , AUTHORITY_VIEWER , NULL },
	{ "alarmaudiofilename"  , para_alarmaudiofilename       , AUTHORITY_VIEWER , NULL },

	/*      SCHEDULE PAGE				*/
	{ "rftpenable"          , para_rftpenable		, AUTHORITY_VIEWER , NULL },
	{ "sdrenable"		    , para_sdrenable		, AUTHORITY_VIEWER , NULL },
	{ "schedule"			, para_schedule			, AUTHORITY_VIEWER , NULL },
	{ "recordlocalstorage"		 , para_recordlocalstorage		 , AUTHORITY_VIEWER , NULL },
    { "schedulerepeatenable"     , para_schedulerepeatenable     , AUTHORITY_VIEWER , NULL },
	{ "schedulenumweeks"         , para_schedulenumweeks         , AUTHORITY_VIEWER , NULL },
    { "scheduleinfiniteenable"   , para_scheduleinfiniteenable   , AUTHORITY_VIEWER , NULL },

	/*       SUPPORT PAGE       */
	{ "kernelversion"		, para_kernelversion		, AUTHORITY_VIEWER 	, NULL },
	{ "biosversion"			, para_biosversion			, AUTHORITY_VIEWER 	, NULL },
    { "softwareversion"	    , para_softwareversion  	, AUTHORITY_VIEWER 	, NULL },

	/*       SDCARD EXPLORER PAGE       */
	{ "sdinsert"        	, para_sdinsert         , AUTHORITY_VIEWER   , NULL },
	{ "sdleft"		        , para_sdleft			, AUTHORITY_VIEWER , NULL },
	{ "sdused"		        , para_sdused			, AUTHORITY_VIEWER , NULL },

	/*       Others       */
	{ "reloadflag"			, para_reloadflag			, AUTHORITY_VIEWER 		, NULL },
	{ "reloadtime"			, para_reloadtime			, AUTHORITY_VIEWER 		, NULL },
	{ "dmvaenable"			, para_dmvaenable			, AUTHORITY_VIEWER 		, NULL },
    { "maxaccount"		    , para_maxaccount			, AUTHORITY_VIEWER 		, NULL },
    { "minnamelen"		    , para_minnamelen			, AUTHORITY_VIEWER 		, NULL },
    { "maxnamelen"	    	, para_maxnamelen			, AUTHORITY_VIEWER 		, NULL },
    { "minpwdlen"		    , para_minpwdlen			, AUTHORITY_VIEWER 		, NULL },
    { "maxpwdlen"		    , para_maxpwdlen			, AUTHORITY_VIEWER 		, NULL },
    { "bkupfirmware"		, para_bkupfirmware			, AUTHORITY_VIEWER 		, NULL },


    { "sensorlinearwdr"		, para_sensorlinearwdr			, AUTHORITY_VIEWER 		, NULL },
    { "sensorlinearwdrname"	, para_sensorlinearwdrname			, AUTHORITY_VIEWER 		, NULL },

    { "flickerdetect"		, para_flickerdetect			, AUTHORITY_VIEWER 		, NULL },
    { "flickerdetectname"	, para_flickerdetectname			, AUTHORITY_VIEWER 		, NULL },
};

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
unsigned int arg_hash_cal_value(char *name)
{
	unsigned int value = 0;

	while (*name)
		value = value * 37 + (unsigned int)(*name++);
	return value;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void arg_hash_insert_entry(ARG_HASH_TABLE *table, HTML_ARGUMENT *arg)
{
	if (table->harg) {
		arg->next = table->harg;
	}
	table->harg = arg;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int arg_hash_table_init(void)
{
	int i;

	if ( (arg_hash = (ARG_HASH_TABLE *)calloc(sizeof(ARG_HASH_TABLE), MAX_ARG_HASH_SIZE)) == NULL) {
		return -1;
	}
	for (i=0; i<HASH_TABLE_SIZE; i++) {
		arg_hash_insert_entry(arg_hash+(arg_hash_cal_value(HttpArgument[i].name)%MAX_ARG_HASH_SIZE), HttpArgument+i);
	}
	return 0;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int TranslateWebPara(AUTHORITY authority, char *target, char *para, char *subpara)
{
	HTML_ARGUMENT *htmp;
	htmp = arg_hash[arg_hash_cal_value(para)%MAX_ARG_HASH_SIZE].harg;

	while (htmp) {
		if ( strcasecmp(htmp->name, para) == 0 ) {
			if (authority > htmp->authority) {
				dbg("[%s.%s] permission denied!!!\n", para, subpara);
				return -1;
			}
			return (*htmp->handler) (target, subpara);
		}
		htmp = htmp->next;
	}
	dbg("[%s.%s] not found\n", para, subpara);
	return -1;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

void arg_hash_table_cleanup()
{
	free(arg_hash);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int ShowAllWebValue(char *data, int max_size, AUTHORITY authority)
{
	HTML_ARGUMENT *htmp;
	int i, total_size = 0, size;
	char buf[1024];
	for(i = 0;i < HASH_TABLE_SIZE; i++){
		htmp = &HttpArgument[i];
		if (authority > htmp->authority)
			continue;
		if(htmp ->handler == NULL)
			continue;
		size = sprintf(buf, "%s=", htmp->name);
		if(total_size + size + 1 > max_size){
			total_size = sprintf(data, "Not enogh size to show");
			break;
		}
		total_size += sprintf(data+total_size, "%s", buf);
		size = (*htmp->handler) (buf, "");
		if(size < 0){
			size = sprintf(buf, "Error return");
		}
		if(total_size + size + 1 > max_size){
			total_size = sprintf(data, "Not enogh size to show");
			break;
		}
		total_size += sprintf(data+total_size, "%s<br>", buf);
	}
	return total_size;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

static int ShowPara(char* buf, int index, char* name, AUTHORITY authority)
{
	char strAuthority[5][20] = {"ADMINISTRATOR","OPERATOR","VIEWER","NONE","ERROR"};
	int a_index;
	switch(authority){
		case AUTHORITY_ADMIN:
			a_index = 0;
			break;
		case AUTHORITY_OPERATOR:
			a_index = 1;
			break;
		case AUTHORITY_VIEWER:
			a_index = 2;
			break;
		case AUTHORITY_NONE:
			a_index = 3;
			break;
		default:
			a_index = 4;
			break;
	}
	return sprintf(buf, "%3d.%-25s%s\n", index, name, strAuthority[a_index]);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

int ShowAllPara(char *data, int max_size, AUTHORITY authority)
{
	HTML_ARGUMENT *htmp;
	int i, total_size = 0, size,count = 0;
	char buf[1024];
	for(i = 0;i < HASH_TABLE_SIZE; i++){
		htmp = &HttpArgument[i];
		if(htmp ->handler == NULL)
			continue;
		size = ShowPara(buf, ++count, htmp->name, htmp->authority);
		if(total_size + size + 1 > max_size){
			total_size = sprintf(data, "Not enogh size to show");
			break;
		}
		total_size += sprintf(data+total_size, "%s", buf);
	}
	return total_size;
}

