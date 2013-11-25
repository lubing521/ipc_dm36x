/** ===========================================================================
* @file upnp-scanip.c
*
* @path $(IPNCPATH)\network\ScanIP_Upnp
*
* @desc 
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <net/if.h>
#include <net/route.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <asm/types.h>
#include <arpa/inet.h>


#include <sys_msg_drv.h>
#include <sysctrl.h>
//#include <Msg_Def.h>
//#include <share_mem.h>

#include <upnp/upnp.h>
#define randomize() srand((unsigned)time(NULL))
#define IPV4_LEN 15

UpnpDevice_Handle device_handle;
int rc;
unsigned short upnpPort = 0;
int port,test,code;
unsigned int expTime = 360;
char desc_doc_url[200],*desc_doc_name = "tvdevicedesc.xml", *ip_address;

int DM350CallbackEventHandler( Upnp_EventType EventType, void* , void* );

void ipToNum(char ipString[IPV4_LEN],unsigned short ipaddr[4]){
	int index = 0;
	int tmp_index = 0;
	int ipClass = 0;
	char tmp[4];

	while(index < IPV4_LEN){
		if(ipString[index] != '.'){
			tmp[tmp_index++] = ipString[index++];
		}else{
			tmp[tmp_index] = '\0';
			ipaddr[ipClass++] = atoi(tmp);
			tmp_index = 0;
			index++;
		}
	}
	tmp[tmp_index] = '\0';
	ipaddr[ipClass] = atoi(tmp);
	//printf("ip = %d.%d.%d.%d\n",ipaddr[0],ipaddr[1],ipaddr[2],ipaddr[3]);
}

void stop(int sig){
	UpnpUnRegisterRootDevice( device_handle );
	UpnpFinish( );
	printf("Shutting down libupnp.\n");
}

void start(int sig){
	//UpnpUnRegisterRootDevice( device_handle );
	//UpnpFinish( );

/**	if( ControlSystemData(SFIELD_GET_UPNP_PORT, (void *)&upnpPort, sizeof(upnpPort)) < 0 ){
		printf("ControlSystemData error.\n");
	}*/
/*
	if( ControlSystemData(SFIELD_SET_UPNP_PID, (void *)&pid, sizeof(pid)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	ControlSystemData(SFIELD_GET_UPNP_PORT, (void *)&upnpPort, sizeof(upnpPort)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	ControlSystemData(SFIELD_GET_UPNP_EXPTIME, (void *)&expTime, sizeof(expTime)) < 0 ){
		printf("ControlSystemData error.\n");
	}
*/

	rc = UpnpInit(NULL, upnpPort);  //upnpPort = 0:autoset
	if ( UPNP_E_SUCCESS == rc ) {
		ip_address = UpnpGetServerIpAddress();
		if(upnpPort == 0)
			port = UpnpGetServerPort();
		else
			port = upnpPort;
		printf ("UPnP Initialized OK ip=%s, port=%d\n",
			(ip_address ? ip_address : "UNKNOWN"), port);
		sprintf(desc_doc_url ,"http://%s/%s", ip_address ,desc_doc_name );
		printf("desc_doc_url = %s\n", desc_doc_url );
	} else {
		printf ("** ERROR UpnpInit(): %d", rc);
		printf ("\n");
		exit (EXIT_FAILURE);
	}

	if( UPNP_E_SUCCESS == (test = UpnpRegisterRootDevice( desc_doc_url
						,DM350CallbackEventHandler ,&device_handle , &device_handle)) ) {
	}else printf("UpnpRegisterRootDevice failure.\n");
printf("error code = %d\n",test);

	if( UPNP_E_SUCCESS == UpnpSendAdvertisement( device_handle, expTime ) ){
		printf("device advertisement succeed.\n");
	}else printf("device advertisement failure.\n");
}

int main()
{
//	int intrface, retn = 0,nread;
	int sockfd;
	int sock_len;
	//unsigned char mac_addr[6];
	struct sockaddr_in address;
	int result/*,qid*/;
	fd_set /*inputs ,*/ readfds , testfds;
	char buffer[88];
	char machine_code[2] = {0,0};
	char mac[6] = {0,0,0,0,0,0};
	int dhcp_stat = 0,pid;
	unsigned short ipv4[4];
	unsigned short ipv4_netmask[4];
	unsigned short ipv4_gateway[4];
	struct in_addr ip;
	struct in_addr submask;
	struct in_addr gateway_addr;
	__u16 http_port = 0;

	char /**paddr,*/title[12];
	struct timeval /*timeout*/;
	struct rtentry rttable;
	struct sigaction act,act1;

	act.sa_handler = start;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;

	act1.sa_handler = stop;
	sigemptyset(&act1.sa_mask);
	act1.sa_flags = SA_RESTART;

	sigaction(SIGUSR1 , &act , 0);
	sigaction(SIGUSR2 , &act1 , 0);
	
	if(SysDrvInit(SYS_UPNP_MSG) < 0){
		printf("SysDrvInit Fail\n");
		return 0;
	}
	pid = getpid();
/**	if( ControlSystemData(SFIELD_GET_UPNP_PORT, (void *)&upnpPort, sizeof(upnpPort)) < 0 ){
		printf("ControlSystemData error.\n");
	}*/
/*
	if( ControlSystemData(SFIELD_SET_UPNP_PID, (void *)&pid, sizeof(pid)) < 0 ){
		printf("ControlSystemData error.\n");
	}
	ControlSystemData(SFIELD_GET_UPNP_EXPTIME, (void *)&expTime, sizeof(expTime)) < 0 ){
		printf("ControlSystemData error.\n");
	}
*/
	sockfd = socket( AF_INET , SOCK_DGRAM , 0 );

	
	//change tvdevicedesc.xml
	system("cp -f /var/www/tvdevicedesc.xml.file /tmp/tvdevicedesc.xml");
    FILE *fptr;
//	char ch;
	//int count=0;
	char UDPString[100];
	char endWord[20];
	char *temp1="uuid:Upnp-TVEmulator-1_0-00";
	char *temp2="</UDN>\n";
    fptr=fopen("/tmp/tvdevicedesc.xml","r+");
    if(fptr==NULL)
	{
	   printf("Can't open file 'tvdevicedesc.xml'");
	   return -1;
	}
	/*
	if(fseek(fptr,619,0)!=0)
	{
	   printf("move ptr failed");
	   return -1;
	}
	//printf("move ptr success\n");
	
	while(count!=44)
	{
	   ch=getc(fptr);
	   printf("%c",ch);
	   count++;	
	}
	*/
	if(fseek(fptr,619,0)!=0)
	{
	   printf("move ptr failed");
	   return -1;
	}
	//printf("\nmove ptr success\n");
	
	
	   strcpy(UDPString,temp1);
	   strcpy(endWord,temp2);
	   unsigned long udp=0;
	   randomize();
	   udp=rand()%100000000;	   	
	   char UDPNumber[50];
	   sprintf(UDPNumber,"_%010d",(int)udp);
	   strcat(UDPString,UDPNumber);	   
	   strcat(UDPString,endWord);
	   printf("UDPString=%s\n",UDPString);
	
       fputs(UDPString,fptr);
	   	
	fclose(fptr);
	//printf("\ncount=%d\n",count);
			
		
	//UPnP program
	rc = UpnpInit(NULL, upnpPort);
	printf("Upnp port is %d\n",upnpPort);
	if ( UPNP_E_SUCCESS == rc ) {
		ip_address = UpnpGetServerIpAddress();
		if(upnpPort == 0)
			port = UpnpGetServerPort();
		else
			port = upnpPort;
		printf ("UPnP Initialized OK ip=%s, port=%d\n",
			(ip_address ? ip_address : "UNKNOWN"), port);
		sprintf(desc_doc_url ,"http://%s/%s", UpnpGetServerIpAddress() ,desc_doc_name );
		printf("desc_doc_url = %s\n", desc_doc_url );
	} else {
		printf ("** ERROR UpnpInit(): %d", rc);
		printf ("\n");
		exit (EXIT_FAILURE); // ---------->
	}

	if( UPNP_E_SUCCESS == (test = UpnpRegisterRootDevice( desc_doc_url
						,DM350CallbackEventHandler ,&device_handle , &device_handle)) ) {
	}else printf("UpnpRegisterRootDevice failure.\n");
printf("error code = %d\n",test);

	if( UPNP_E_SUCCESS == UpnpSendAdvertisement( device_handle, expTime ) ){
		printf("device advertisement succeed.\n");
	}else printf("device advertisement failure.\n");

	//ScanIP
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl( INADDR_ANY );
	address.sin_port = htons(8992);
	sock_len = sizeof( address );

	rttable.rt_gateway.sa_family = AF_INET;


	bind( sockfd , (struct sockaddr *)&address , sock_len );
	listen( sockfd , 5 );

	FD_ZERO( &readfds );
	FD_SET( sockfd , &readfds );

	while( 1 ) {
		int fd;
		testfds = readfds;
//		int nread;
		result = select( FD_SETSIZE , &testfds , 0 , 0 , 0 );
		if( result < 1 ) {
			if (errno != EINTR){
				perror("scanip");
				exit(1);
			}
		}

		for( fd = 0;fd < FD_SETSIZE ; fd++ ) {
			if( FD_ISSET(fd,&testfds) ) {
				if( fd == sockfd ) {
					sock_len = sizeof(address);
					result = recvfrom( sockfd , buffer , 88 ,0 , (struct sockaddr *)&address , &sock_len );
					if(result != -1){
						int x = 0,dev_title = 48;
						if( ControlSystemData(SFIELD_GET_IP, (void *)&ip, sizeof(ip)) < 0 ){
						printf("SFIELD_GET_IP error.\n");
						}
						if( ControlSystemData(SFIELD_GET_DEVICE_TYPE, (void *)&machine_code, sizeof(machine_code)) < 0 ){
						printf("SFIELD_GET_DEVICE_TYPE error.\n");
						}
						if( ControlSystemData(SFIELD_GET_NETMASK, (void *)&submask.s_addr, sizeof(submask.s_addr)) < 0 ){
						printf("SFIELD_GET_NETMASK error.\n");
						}
						if( ControlSystemData(SFIELD_GET_HTTPPORT, (void *)&http_port, sizeof(http_port)) < 0 ){
						printf("SFIELD_GET_HTTPPORT error.\n");
						}
						if( ControlSystemData(SFIELD_GET_DHCPC_ENABLE, (void *)&dhcp_stat, sizeof(dhcp_stat)) < 0 ){
						printf("SFIELD_GET_DHCPC_ENABLE error.\n");
						}
						if( ControlSystemData(SFIELD_GET_GATEWAY, (void *)&gateway_addr, sizeof(gateway_addr)) < 0 ){
						printf("SFIELD_GET_GATEWAY error.\n");
						}
						if( ControlSystemData(SFIELD_GET_MAC, (void *)&mac, sizeof(mac)) < 0 ){
						printf("SFIELD_GET_MAC error.\n");
						}
						if( ControlSystemData(SFIELD_GET_TITLE, (void *)&title, sizeof(title)) < 0 ){
						printf("SFIELD_GET_TITLE.\n");
						}
						buffer[4] = machine_code[0];
						buffer[5] = machine_code[1];
/*						
inet_aton("10.2.1.233",&ip);
inet_aton("255.255.255.0",&submask);
inet_aton("10.2.1.254",&gateway_addr);

printf("ip = %s\n",inet_ntoa(ip));
printf("netmask ip = %s\n",inet_ntoa(submask));
printf("gateway ip = %s\n",inet_ntoa(gateway_addr));
*/
						ipToNum(inet_ntoa(ip),ipv4);
						buffer[20] = ipv4[0];
						buffer[21] = ipv4[1];
						buffer[22] = ipv4[2];
						buffer[23] = ipv4[3];

						ipToNum(inet_ntoa(submask),ipv4_netmask);
						buffer[28] = ipv4_netmask[0];
						buffer[29] = ipv4_netmask[1];
						buffer[30] = ipv4_netmask[2];
						buffer[31] = ipv4_netmask[3];

						buffer[84] = http_port;
						buffer[85] = 0;
						buffer[17] = dhcp_stat;


						while( title[x] != '\0' ){
							buffer[dev_title++] = title[x];
							x++;
						}
						buffer[dev_title] = '\0';

						/*Get gateway address */
						ipToNum(inet_ntoa(gateway_addr),ipv4_gateway);
						buffer[24] = ipv4_gateway[0];
						buffer[25] = ipv4_gateway[1];
						buffer[26] = ipv4_gateway[2];
						buffer[27] = ipv4_gateway[3];

						buffer[6] = mac[0];
						buffer[7] = mac[1];
						buffer[8] = mac[2];
						buffer[9] = mac[3];
						buffer[10] = mac[4];
						buffer[11] = mac[5];
						printf("In scanip loop.\n");
						result = sendto( sockfd , buffer , 88 ,0 , (struct sockaddr *)&address , sock_len );
					}
				}
			}
		}
	}
}

int DM350CallbackEventHandler( Upnp_EventType EventType, void* Event, void* Cookie )
{
printf("in callback function...");

	switch( EventType ) {
		default:
			printf("UNKOWN EVENT TYPE\n");
	}

	return 0;
}
