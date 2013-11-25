/*
 * ifconfig   This file contains an implementation of the function
 *              that either displays or sets the characteristics of
 *              one or more of the system's networking interfaces.
 *
 * Author:    Javier Hsiao <javier@approtech.com>
 *              and others.  Copyright 2003 Appro Technology Inc.
 *
 */
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
//#include <asm/arch/v4l_ioctl.h>

//#include "lancam.h"
#include "net_config.h"

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

/***************************************************************************
 * Set a certain interface flag.                                           *
 ***************************************************************************/
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

/***************************************************************************
 * Clear a certain interface flag.                                         *
 ***************************************************************************/
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

/***************************************************************************
 * Get a interface flag.                                                   *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int net_nic_up(char *ifname)
{
	return net_set_flag(ifname, (IFF_UP | IFF_RUNNING));
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int net_nic_down(char *ifname)
{
	return net_clr_flag(ifname, IFF_UP);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void net_enable_dhcpcd(void)
{
/*	FILE *fp;
	pid_t pid;
	char pidfile[64];
	
	snprintf(pidfile, sizeof(pidfile), DHCPC_PID_FILE_PATH, "eth0");
	fp=fopen(pidfile, "r");
	if ( fp ) {
		fscanf(fp, "%u", &pid);
		fclose(fp);
		unlink(pidfile);
		kill(pid, SIGHUP);
	}*/
	system("killall -2 "DHCPC_EXEC);
	system(DHCPC_EXEC_PATH);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
void net_disable_dhcpcd(void)
{
	system("killall -2 "DHCPC_EXEC);
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
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
