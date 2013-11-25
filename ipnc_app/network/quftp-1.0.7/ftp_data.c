/* FTP Data Functions
   $Revision: 1.3 $
   $Date: 2002/06/24 04:04:32 $
*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "ftp.h"
#include "quftp.h"

int ftp_open_data_pasv(struct ftpconnection *connection, char *address, int port) {
	int response;
	struct sockaddr_in dataaddr;
	char *buffer = NULL, *line, *temp, *temp2;
	int length;
	if (!ftp_check_ready(connection, 1)) return 0;
	if (!address) {
		ftp_sendline(connection, "PASV\r\n");
		line = (char *)malloc(MAX_LINE_SIZE);
		response = ftp_getrc(connection, line, MAX_LINE_SIZE, 0);
		switch (response) {
			case 227 : temp = strstr(line, "(");
				   temp++;
				   temp2 = strstr(temp, ")");
				   length = temp2-temp;
				   buffer = (char *)malloc(length + 1);
				   memcpy(buffer, temp, length);
				   buffer[length] = 0;
				   address = (char *)malloc(20);
				   port = ftp_parse_pasv(buffer, address);
				   free(line);
				   free(buffer);
				   break;
			case 421 : log(LOG_ALERT, "Service unavailable\n");
				   free(line);
				   ftp_disconnect(connection);
				   return 0;
				   break;
			case 500 : log(LOG_ALERT, "Server doesn't understand PASV\n");
				   free(line);
				   return 0;
				   break;
			case 501 : log(LOG_ALERT, "Server doesn't understand PASV parameters\n");
				   free(line);
				   return 0;
				   break;
			case 502 : log(LOG_ALERT, "Server doesn't understand PASV\n");
				   free(line);
				   return 0;
				   break;
			case 530 : log(LOG_ALERT, "Not logged in\n");
				   free(line);
				   return 0;
				   break;
			default  : log(LOG_ALERT, "Unknown response to PASV: %d\n", response);
				   free(line);
				   return 0;
				   break;
		}
	}
	if ((connection->datafd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(0);
	}
	log(LOG_MESSAGE, "Opening a data connection to %s:%d",
			address, port);
	memset(&dataaddr, 0, sizeof(dataaddr));
	dataaddr.sin_family = AF_INET;
	dataaddr.sin_port = htons(port);
	if (inet_aton(address, &dataaddr.sin_addr) <= 0) {
		perror("inet_aton");
		exit(0);
	}
	if (connect(connection->datafd,(SOCKADDR *)&dataaddr, sizeof(dataaddr)) < 0){
		log(LOG_ALERT, "Can't open a data connection to %s\n", address);
		return -1;
	}
	connection->status = STATUS_TRANSFER;
	return 0;
}

int ftp_close_data(struct ftpconnection *connection) {
	if (connection->dataconfd) close(connection->dataconfd);
	if (connection->datafd) close(connection->datafd);
	connection->datafd = 0;
	connection->dataconfd = 0;
	connection->status = STATUS_IDLE;
	log(LOG_MESSAGE, "Data connection closed\n");
	return 0;
}

int ftp_read_data(struct ftpconnection *connection, char *buffer, int max_size) {
	int n;
	if (connection->status != STATUS_TRANSFER) return -1;
	memset(buffer, 0, max_size);
	n = read(connection->datafd, buffer, max_size);
	if (n < 0) perror("read");
	return n;
}

int ftp_read_data_line(struct ftpconnection *connection, char *buffer, int max_size) {
	int n;
	char *temp;
	if (connection->status != STATUS_TRANSFER) return -1;
	memset(buffer, 0, max_size);
	temp = buffer;
	while ((n = read(connection->datafd, temp, 1)) > 0) {
		if (temp[0] == '\r') continue;
		if (temp[0] == '\n') break;
		if (temp == (buffer + max_size)) break;
		temp++;
	}
	temp[0] = 0;
	if (n < 0) {
		perror("read");
		exit(0);
	}
	if (buffer[0] == 0) return 0;
	return n;
}

int ftp_write_data(struct ftpconnection *connection, char *buffer, int size) {
	int n;
	if (connection->status != STATUS_TRANSFER) return -1;
	if ((n = write(connection->datafd, buffer, size)) < 0) {
		perror("write");
		exit(0);
	}
	return n;
}

int ftp_open_data(struct ftpconnection *connection) {
	struct sockaddr_in servaddr;
	struct in_addr sin;
	char *line;
	int h1, h2, h3, h4, p1, p2, port;
	int response;
	SOCKLEN len;
	if ((connection->dataconfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(0);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = 0;
	if (bind(connection->dataconfd, (SOCKADDR *)&servaddr, sizeof(servaddr)) < 0) {
		perror("bind");
		exit(0);
	}
	if (listen(connection->dataconfd, 1) < 0) {
		perror("listen");
		exit(0);
	}
	len = 128;
	if (getsockname(connection->dataconfd, (SOCKADDR *)&servaddr, &len) < 0) {
		perror("getsockname");
		exit(0);
	}
	line = (char *)malloc(MAX_LINE_SIZE);
	port = servaddr.sin_port;
	sin.s_addr = connection->localip;
	sprintf(line, "%s.%d\r\n", inet_ntoa(sin), port);
	sscanf(line, "%d.%d.%d.%d.%d", &h1, &h2, &h3, &h4, &port);
	p1 = (port / 256);
	p2 = port - (256 * p1);
	sprintf(line, "PORT %d,%d,%d,%d,%d,%d\r\n", h1, h2, h3, h4, p2, p1);
	connection->status = STATUS_WAITING;
	ftp_sendline(connection, line);
	free(line);
	response = ftp_getrc(connection, NULL, 0, 0);
	switch (response) {
		case 200 : return 0;
		case 421 : log(LOG_ALERT, "Service unavailable\n");
			   ftp_disconnect(connection);
			   return response;
			   break;
		case 500 : log(LOG_ALERT, "Server doesn't understand PORT\n");
			   return response;
			   break;
		case 501 : log(LOG_ALERT, "Server doesn't understand PORT parameters\n");
			   return response;
			   break;
		case 502 : log(LOG_ALERT, "Server doesn't understand PORT\n");
			   return response;
			   break;
		case 530 : log(LOG_ALERT, "Not logged in\n");
			   return response;
			   break;
		default  : log(LOG_ALERT, "Unknown response to PORT: %d\n", response);
			   return response;
			   break;
	}
}

int ftp_wait_data(struct ftpconnection *connection) {
	struct sockaddr_in servaddr;
	int len = 128;
	if (connection->status != STATUS_WAITING) {
		return 0;
	}
	connection->datafd = accept(connection->dataconfd, (SOCKADDR *)&servaddr, &len);
	log(LOG_MESSAGE, "Data connection opened to %s",
		inet_ntoa(servaddr.sin_addr));
	connection->status = STATUS_TRANSFER;
	return 1;
}

