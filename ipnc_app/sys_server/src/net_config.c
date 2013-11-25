/** ===========================================================================
* @file net_config.c
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
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <unistd.h>
#include <signal.h>
#include <asm/types.h>
#include <linux/sockios.h>
//#include <asm/arch/v4l_ioctl.h>

//#include "lancam.h"
#include "net_config.h"
#include "davinci_emac_priv.h"

#define NET_CONFIG_DEBUG

#ifdef NET_CONFIG_DEBUG
#define DBG_ERR(x...)		perror(x)
#define DBG_NET(x...)		printf(x)
#define DBG(x...)			printf(x)
#else
#define DBG_ERR(x...)
#define DBG_NET(x...)
#define DBG(x...)
#endif

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
static struct sockaddr_in sa = {
	sin_family:	PF_INET,
	sin_port:	0
};
/**
* @brief Davinci emac private control type.
*/
typedef struct {
	unsigned int cmd;
	void *data;
} emac_drv_priv_ioctl;
/**
* @brief Mii data.
*/
struct mii_data
{
	unsigned short phy_id;
	unsigned short reg_num;
	unsigned short val_in;
	unsigned short val_out;
};

/**
 * @brief	Set a certain interface flag.
 * @param	"char *ifname" : interface name
 * @param	"short flag" : flag
 * @retval	0 : success ; -1 : fail
 */
int net_set_flag(char *ifname, short flag)
{
	struct ifreq ifr;
	int skfd;

	if ( (skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		DBG_ERR("socket error");
		return -1;
	}
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0) {
		DBG_ERR("net_set_flag: ioctl SIOCGIFFLAGS");
		close(skfd);
		return (-1);
	}
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	ifr.ifr_flags |= flag;
	if (ioctl(skfd, SIOCSIFFLAGS, &ifr) < 0) {
		DBG_ERR("net_set_flag: ioctl SIOCSIFFLAGS");
		close(skfd);
		return -1;
	}
	close(skfd);
	return (0);
}

/**
 * @brief	Clear a certain interface flag.
 * @param	"char *ifname" : interface name
 * @param	"short flag" : flag
 * @retval	0 : success ; -1 : fail
 */
int net_clr_flag(char *ifname, short flag)
{
	struct ifreq ifr;
	int skfd;

	if ( (skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		DBG_ERR("socket error");
		return -1;
	}
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0) {
		DBG_ERR("net_clr_flag: ioctl SIOCGIFFLAGS");
		close(skfd);
		return -1;
	}
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	ifr.ifr_flags &= ~flag;
	if (ioctl(skfd, SIOCSIFFLAGS, &ifr) < 0) {
		DBG_ERR("net_clr_flag: ioctl SIOCSIFFLAGS");
		close(skfd);
		return -1;
	}
	close(skfd);
	return (0);
}

/**
 * @brief	Get an interface flag.
 * @param	"char *ifname" : interface name
 * @retval	ifr.ifr_flags
 * @retval	-1 : fail
 */
int net_get_flag(char *ifname)
{
	struct ifreq ifr;
	int skfd;

	if ( (skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		DBG_ERR("socket error");
		return -1;
	}
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0) {
		DBG_ERR("net_get_flag: ioctl SIOCGIFFLAGS");
		close(skfd);
		return -1;
	}
	close(skfd);
	return ifr.ifr_flags;
}

/**
 * @brief	Set an interface up & running
 * @param	"char *ifname" : interface name
 * @retval	0 : success ; -1 : fail
 */
int net_nic_up(char *ifname)
{
	return net_set_flag(ifname, (IFF_UP | IFF_RUNNING));
}

/**
 * @brief	Set an interface down
 * @param	"char *ifname" : interface name
 * @retval	0 : success ; -1 : fail
 */
int net_nic_down(char *ifname)
{
	return net_clr_flag(ifname, IFF_UP);
}

/**
 * @brief	get address of an interface
 * @param	"char *ifname" : interface name
 * @retval	net address
 */
in_addr_t net_get_ifaddr(char *ifname)
{
	struct ifreq ifr;
	int skfd;
	struct sockaddr_in *saddr;

	if ( (skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		DBG_ERR("socket error");
		return -1;
	}

	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	if (ioctl(skfd, SIOCGIFADDR, &ifr) < 0) {
		DBG_ERR("net_get_ifaddr: ioctl SIOCGIFADDR");
		close(skfd);
		return -1;
	}
	close(skfd);

	saddr = (struct sockaddr_in *) &ifr.ifr_addr;
	return saddr->sin_addr.s_addr;
}

/**
 * @brief	get ip of an interface
 * @param	"int skfd" :
 * @param	"char *ifname" : interface name
 * @retval	ip
 */
struct in_addr net_get_ip(int skfd, char *ifname)
{
	struct ifreq ifr;

	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	if (ioctl(skfd, SIOCGIFADDR, &ifr) < 0) {
		DBG_ERR("net_get_ip: ioctl SIOCGIFADDR");
		return (struct in_addr){-1};
	}
	return ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
}

/**
 * @brief	set ip of an interface
 * @param	"char *ifname" : interface name
 * @param	"in_addr_t addr" : ip address
 * @retval	0 : success ; -1 : fail
 */
int net_set_ifaddr(char *ifname, in_addr_t addr)
{
	struct ifreq ifr;
	int skfd;

	if ( (skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		DBG_ERR("socket error");
		return -1;
	}
	sa.sin_addr.s_addr = addr;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	memcpy((char *) &ifr.ifr_addr, (char *) &sa, sizeof(struct sockaddr));
	if (ioctl(skfd, SIOCSIFADDR, &ifr) < 0) {
		DBG_ERR("net_set_ifaddr: ioctl SIOCSIFADDR");
		close(skfd);
		return -1;
	}
	close(skfd);
	return 0;
}

/**
 * @brief	get address of an interface
 * @param	"char *ifname" : interface name
 * @retval	address
 */
in_addr_t net_get_netmask(char *ifname)
{
	struct ifreq ifr;
	int skfd;
	struct sockaddr_in *saddr;

	if ( (skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		DBG_ERR("socket error");
		return -1;
	}

	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	if (ioctl(skfd, SIOCGIFNETMASK, &ifr) < 0) {
		DBG_ERR("net_get_netmask: ioctl SIOCGIFNETMASK");
		close(skfd);
		return -1;
	}
	close(skfd);

	saddr = (struct sockaddr_in *) &ifr.ifr_addr;
	return saddr->sin_addr.s_addr;
}

/**
 * @brief	get netmask of an interface
 * @param	"char *ifname" : [IN]interface name
 * @param	"in_addr_t addr" : [OUT]netmask
 * @retval	0 : success ; -1 : fail
 */
int net_set_netmask(char *ifname, in_addr_t addr)
{
	struct ifreq ifr;
	int skfd;

	if ( (skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		DBG_ERR("socket error");
		return -1;
	}
	sa.sin_addr.s_addr = addr;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	memcpy((char *) &ifr.ifr_addr, (char *) &sa, sizeof(struct sockaddr));
	if (ioctl(skfd, SIOCSIFNETMASK, &ifr) < 0) {
		DBG_ERR("net_set_netmask: ioctl SIOCSIFNETMASK");
		close(skfd);
		return -1;
	}
	close(skfd);
	return 0;
}

/**
 * @brief	get mac address of an interface
 * @param	"char *ifname" : [IN]interface name
 * @param	"unsigned char *mac" : [OUT]mac address
 * @retval	0 : success ; -1 : fail
 */
int net_get_hwaddr(char *ifname, unsigned char *mac)
{
	struct ifreq ifr;
	int skfd;

	if ( (skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		DBG_ERR("socket error");
		return -1;
	}

	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	if (ioctl(skfd, SIOCGIFHWADDR, &ifr) < 0) {
		DBG_ERR("net_get_hwaddr: ioctl SIOCGIFHWADDR");
		close(skfd);
		return -1;
	}
	close(skfd);

	memcpy(mac, ifr.ifr_ifru.ifru_hwaddr.sa_data, IFHWADDRLEN);
	return 0;
}

/**
 * @brief	add a gateway
 * @param	"in_addr_t addr" : [IN]address of gateway
 * @retval	0 : success ; -1 : fail
 */
int net_add_gateway(in_addr_t addr)
{
	struct rtentry rt;
	int skfd;

	/* Clean out the RTREQ structure. */
	memset((char *) &rt, 0, sizeof(struct rtentry));

	/* Fill in the other fields. */
	rt.rt_flags = (RTF_UP | RTF_GATEWAY);

	rt.rt_dst.sa_family = PF_INET;
	rt.rt_genmask.sa_family = PF_INET;

	sa.sin_addr.s_addr = addr;
	memcpy((char *) &rt.rt_gateway, (char *) &sa, sizeof(struct sockaddr));

	/* Create a socket to the INET kernel. */
	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		DBG_ERR("socket error");
		return -1;
	}
	/* Tell the kernel to accept this route. */
	if (ioctl(skfd, SIOCADDRT, &rt) < 0) {
		DBG_ERR("net_add_gateway: ioctl SIOCADDRT");
		close(skfd);
		return -1;
	}
	/* Close the socket. */
	close(skfd);
	return (0);
}

/**
 * @brief	delete a gateway
 * @param	"in_addr_t addr" : [IN]address of gateway
 * @retval	0 : success ; -1 : fail
 */
int net_del_gateway(in_addr_t addr)
{
	struct rtentry rt;
	int skfd;

	/* Clean out the RTREQ structure. */
	memset((char *) &rt, 0, sizeof(struct rtentry));

	/* Fill in the other fields. */
	rt.rt_flags = (RTF_UP | RTF_GATEWAY);

	rt.rt_dst.sa_family = PF_INET;
	rt.rt_genmask.sa_family = PF_INET;

	sa.sin_addr.s_addr = addr;
	memcpy((char *) &rt.rt_gateway, (char *) &sa, sizeof(struct sockaddr));

	/* Create a socket to the INET kernel. */
	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		DBG_ERR("socket error");
		return -1;
	}
	/* Tell the kernel to accept this route. */
	if (ioctl(skfd, SIOCDELRT, &rt) < 0) {
		DBG_ERR("net_del_gateway: ioctl SIOCDELRT");
		close(skfd);
	return -1;
	}
	/* Close the socket. */
	close(skfd);
	return (0);
}

/**
 * @brief	set domain name server.
 * @param	"char *dnsname" : [IN]dns name
 * @retval	0 : success ; -1 : fail
 */
int net_set_dns(char *dnsname)
{
	FILE *fp;

	fp = fopen(RESOLV_CONF, "w");
	if ( fp ) {
		fprintf(fp, "nameserver %s\n", dnsname);
		fclose(fp);
		DBG_NET("dns=%s\n", dnsname);
		return 0;
	}
	DBG("net_set_dns: file \"%s\" opened for writing error!\n", RESOLV_CONF);
	return -1;
}

/**
 * @brief	get domain name server.
 * @param	none
 * @retval	dns address
 */
in_addr_t net_get_dns(void)
{
	FILE *fp;
	char dnsname[80];

	fp = fopen(RESOLV_CONF, "r");
	if ( fp ) {
		if (fscanf(fp, "nameserver %s\n", dnsname) != EOF) {
			fclose(fp);
			DBG_NET("dns=%s\n", dnsname);
			return inet_addr(dnsname);
		}
		DBG("net_get_dns: scan fail!\n");
	}
	DBG("net_get_dns: file \"%s\" opened for reading error!\n", RESOLV_CONF);
	return INADDR_ANY;
}

/**
 * @brief	start dhcp
 * @param	"char *ifname" : [IN]interface name
 * @return	pid
 */
pid_t net_start_dhcpcd(char *ifname)
{
	pid_t pid;

	if( (pid = vfork()) == 0 ) {
		/* the child */
		execlp(DHCPC_EXEC_PATH, DHCPC_EXEC_PATH, ifname, NULL);
		DBG_ERR("exec dhcpcd failed");
		_exit(0);
	}
	else if(pid < 0) {
		DBG_ERR("fork dhcpcd failed");
		return -1;
	}
	return pid;
}

/**
 * @brief	renew dhcp.
 * @param	"pid_t pid" : pid
 * @retval	0 : success ; -1 : fail
 */
int net_renew_dhcpcd(pid_t pid)
{
	if ( pid > 0 ) {
		if ( kill(pid, SIGALRM) == 0 )
			return 0;
		DBG_ERR("net_disable_dhcpcd, kill error");
	}
	DBG("net_disable_dhcpcd, pid error\n");
	return -1;
}

/**
 * @brief	enable dhcp.
 * @param	none
 * @retval	none
 */
void net_enable_dhcpcd(void)
{
	system("killall -9 "DHCPC_EXEC);
	system(DHCPC_EXEC_PATH);
}

/**
 * @brief	disable dhcp.
 * @param	none
 * @retval	none
 */
void net_disable_dhcpcd(void)
{
	system("killall -9 "DHCPC_EXEC);
	system("rm /var/run/dhcpcd-eth0.pid\n");
}

/**
 * @brief	search gateway
 * @param	"char *buf" : [IN]buffer
 * @param	"in_addr_t *gate_addr" : [OUT]gateway address
 * @return	0 : success ; -1 : fail
 */
int net_search_gateway(char *buf, in_addr_t *gate_addr)
{
	char iface[16];
	unsigned long dest, gate;
	int iflags;

	sscanf(buf, "%s\t%08lX\t%08lX\t%8X\t", iface, &dest, &gate, &iflags);
	DBG_NET("net_search_gateway:%s, %lX, %lX, %X\n", iface, dest, gate, iflags);
	if ( (iflags & (RTF_UP | RTF_GATEWAY)) == (RTF_UP | RTF_GATEWAY) ) {
		*gate_addr = gate;
		return 0;
	}
	return -1;
}

/**
 * @brief	set gateway
 * @param	"in_addr_t addr" : [IN]gateway address
 * @return	0 : success ; -1 : fail
 */
int net_set_gateway(in_addr_t addr)
{
	in_addr_t gate_addr;
	char buff[132];
	FILE *fp = fopen(PROCNET_ROUTE_PATH, "r");

	if (!fp) {
		DBG_ERR(PROCNET_ROUTE_PATH);
		DBG("INET (IPv4) not configured in this system.\n");
		return -1;
	}
	fgets(buff, 130, fp);
	while (fgets(buff, 130, fp) != NULL) {
		if (net_search_gateway(buff, &gate_addr) == 0) {
			net_del_gateway(gate_addr);
		}
	}
	fclose(fp);

	return net_add_gateway(addr);
}

/**
 * @brief	clean gateway
 * @param	none
 * @return	0 : success ; -1 : fail
 */
int net_clean_gateway(void)
{
	in_addr_t gate_addr;
	char buff[132];
	FILE *fp = fopen(PROCNET_ROUTE_PATH, "r");

	if (!fp) {
		DBG_ERR(PROCNET_ROUTE_PATH);
		DBG("INET (IPv4) not configured in this system.\n");
		return -1;
	}
	fgets(buff, 130, fp);
	while (fgets(buff, 130, fp) != NULL) {
		if (net_search_gateway(buff, &gate_addr) == 0) {
			net_del_gateway(gate_addr);
		}
	}
	fclose(fp);

	return 0;
}

/**
 * @brief	get gateway
 * @param	none
 * @return	gatewat address
 */
in_addr_t net_get_gateway(void)
{
	in_addr_t gate_addr;
	char buff[132];
	FILE *fp = fopen(PROCNET_ROUTE_PATH, "r");

	if (!fp) {
		DBG_ERR(PROCNET_ROUTE_PATH);
		DBG("INET (IPv4) not configured in this system.\n");
		return (INADDR_ANY);
	}
	fgets(buff, 130, fp);
	while (fgets(buff, 130, fp) != NULL) {
		if (net_search_gateway(buff, &gate_addr) == 0) {
			fclose(fp);
			return gate_addr;
		}
	}
	fclose(fp);
	return (INADDR_ANY);
}

/**
 * @brief	get net info
 * @param	"char *ifname" : [IN]interface name
 * @param	"struct NET_CONFIG *netcfg" : [OUT]net config
 * @return	0 : success ; -1 : fail
 */
int net_get_info(char *ifname, struct NET_CONFIG *netcfg)
{
	struct ifreq ifr;
	int skfd;
	struct sockaddr_in *saddr;

	if ( (skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		DBG_ERR("socket error");
		return -1;
	}

	saddr = (struct sockaddr_in *) &ifr.ifr_addr;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	if (ioctl(skfd, SIOCGIFADDR, &ifr) < 0) {
		DBG_ERR("net_get_info: ioctl SIOCGIFADDR");
		close(skfd);
		return -1;
	}
	netcfg->ifaddr = saddr->sin_addr.s_addr;
	DBG_NET("ifaddr=0x%x\n", netcfg->ifaddr);

	if (ioctl(skfd, SIOCGIFNETMASK, &ifr) < 0) {
		DBG_ERR("net_get_info: ioctl SIOCGIFNETMASK");
		close(skfd);
		return -1;
	}
	netcfg->netmask = saddr->sin_addr.s_addr;
	DBG_NET("netmask=0x%x\n", netcfg->netmask);

	if (ioctl(skfd, SIOCGIFHWADDR, &ifr) < 0) {
		DBG_ERR("net_get_info: ioctl SIOCGIFHWADDR");
		close(skfd);
		return -1;
	}
	memcpy(netcfg->mac, ifr.ifr_ifru.ifru_hwaddr.sa_data, IFHWADDRLEN);
	DBG_NET("hwaddr=%02x:%02x:%02x:%02x:%02x:%02x\n", netcfg->mac[0],netcfg->mac[1],
					netcfg->mac[2],netcfg->mac[3],netcfg->mac[4],netcfg->mac[5]);

	close(skfd);
	netcfg->gateway = net_get_gateway();
	DBG_NET("gateway=0x%x\n", netcfg->gateway);
	netcfg->dns = net_get_dns();
	return 0;

}
/**
* @brief check ethernet status
*
* @param pInterface [I] ethernt device name. Ex: "eth0" "eth1"
* @return Link status
* @retval 0  Linkdown
* @retval 1  Linkup
* @retvl -1  Error
*/
int Check_Link_status( char *pInterface )
{
	int skfd = 0;
	struct ifreq ifr;
#if 0
	struct mii_data* mii = NULL;
#else
	emac_drv_priv_ioctl priv_ioctl;
	emac_phy_params phy_params;
#endif

	if( ( skfd = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 )
	{
			perror( "socket" );
			return -1;
	}


	bzero( &ifr, sizeof( ifr ) );
	strncpy( ifr.ifr_name, pInterface, IFNAMSIZ - 1 );
	ifr.ifr_name[IFNAMSIZ - 1] = 0;
#if 0
	if( ioctl( skfd, SIOCGMIIPHY, &ifr ) < 0 )
	{
			perror( "ioctl" );
			close( skfd );
			return -1;
	}

	mii = (struct mii_data*)&ifr.ifr_data;
	mii->reg_num = 0x01;
	if( ioctl( skfd, SIOCGMIIREG, &ifr ) < 0 )
	{
			perror( "ioctl2" );
			close( skfd );
			return -1;
	}

	close( skfd );

	if( mii->val_out & 0x0004 )
	{

	//	printf( "Linkup\n" );
		return 1;
	}
	else
	{
	//	printf( "Linkdown\n" );
		return 0;
	}
#else
	/* DM365 private use */
	phy_params.reg_addr = 1;
	phy_params.phy_num = 1;
	priv_ioctl.cmd = EMAC_PRIV_MII_READ;
	priv_ioctl.data = &phy_params;
	ifr.ifr_data = (char *)&priv_ioctl;
	if( ioctl( skfd, SIOCDEVPRIVATE, &ifr ) < 0 )
	{
			perror( "ioctl" );
			close( skfd );
			return -1;
	}
	close( skfd );
	if(phy_params.data & 4)
		return 1;
	else
		return 0;
#endif
}
/**
* @brief check ethernet speed
*
* @param pInterface [I] ethernt device name. Ex: "eth0" "eth1"
* @return Ethernet speed
* @retval 0  10M
* @reval 1  100M
* @retval -1  Error
*/
int Check_Link_speed( char *pInterface )
{
	int skfd = 0;
	struct ifreq ifr;
	struct mii_data* mii = NULL;

	if( ( skfd = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 )
	{
			perror( "socket" );
			return -1;
	}


	bzero( &ifr, sizeof( ifr ) );
	strncpy( ifr.ifr_name, pInterface, IFNAMSIZ - 1 );
	ifr.ifr_name[IFNAMSIZ - 1] = 0;
	if( ioctl( skfd, SIOCGMIIPHY, &ifr ) < 0 )
	{
			perror( "ioctl" );
			return -1;
	}

	mii = (struct mii_data*)&ifr.ifr_data;
	mii->reg_num = 0; //sPHY_register 0
	if( ioctl( skfd, SIOCGMIIREG, &ifr ) < 0 )
	{
		  perror( "ioctl2" );
		  return -1;
	}
	close( skfd );
	if( mii->val_out & (1<<13) ) //scheck bit13 of PHY_REG0
	{
		//printf( "100M\n" );
		return 1;
	}
	else
	{
		//printf( "10M\n" );
		return 0;
	}

}

/**
 * @brief	main entry
 * @param	command line argument
 * @return	error code
 */
//#define NET_TEST
#ifdef NET_TEST
int main(int argc, char **argv)
{
	struct NET_CONFIG netcfg;
	int i;

	if (argc < 3) {
		net_get_info("eth0", &netcfg);
		return 0;
	}

	for (i=1; i<argc; i++)
		printf("arg[%d]=%s\n", i, argv[i]);

	if (!strcmp(argv[2], "up"))
		net_set_flag(argv[1], (IFF_UP | IFF_RUNNING));
	else if (!strcmp(argv[2], "down"))
		net_clr_flag(argv[1], IFF_UP);
	else if (!strcmp(argv[2], "ip")) {
		net_set_ifaddr(argv[1], inet_addr(argv[3]));
	}
	else if (!strcmp(argv[2], "netmask")) {
		net_set_netmask(argv[1], inet_addr(argv[3]));
	}
	else if (!strcmp(argv[2], "gateway")) {
		if (!strcmp(argv[1], "add"))
			net_add_gateway(inet_addr(argv[3]));
		else
			net_del_gateway(inet_addr(argv[3]));
	}
	else if (!strcmp(argv[2], "dhcpcd")) {
		if (!strcmp(argv[3], "enable"))
			net_enable_dhcpcd(argv[1]);
		else
			net_disable_dhcpcd(argv[1]);
	}
	else if (!strcmp(argv[1], "dns"))
		net_set_dns(argv[2]);
	else
		printf("unknown argument!\n");
	return 0;
}
#endif
