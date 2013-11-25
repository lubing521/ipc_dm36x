/* FTP Functions
   $Revision: 1.3 $
   $Date: 2002/06/24 04:04:32 $
*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "rc.h"
#include "ftp.h"
#include "quftp.h"

int ftp_login(struct ftpconnection *connection) {
	int response, show_motd = 0;
	char line[4096];
	log(LOG_CALLS, "%p\n", connection);
	if (connection->status == STATUS_IDLE) {
		ftp_sendline(connection, "REIN\r\n");
		response = ftp_getrc(connection, NULL, 0, 0);
		switch (response) {
			case 220 : break; /* OK REIN */
			case 120 : log(LOG_ALERT, "Service not ready... try again later");
				   ftp_disconnect(connection);
				   return response;
				   break;
			case 421 : log(LOG_ALERT, "Service unavailable\n");
				   ftp_disconnect(connection);
				   return response;
				   break;
			case 500 : log(LOG_ALERT, "This server doesn't allow you to re-login\n");
				   return response;
				   break;
			case 502 : log(LOG_ALERT, "This server doesn't allow you to re-login\n");
				   return response;
				   break;
			default	 : log(LOG_CRITICAL, "Unknown response %d\n", response);
				   return response;
				   break;
		}
	} else if (connection->status != STATUS_WAIT_LOGIN) {
		log(LOG_CRITICAL, "ftp_login: Server is not waiting for login\n");
		return -1;
	}
	log(LOG_MESSAGE, "Send username %s", connection->username);
	sprintf(line, "USER %s\r\n", connection->username);
	response = ftp_sendline(connection, line);
	response = ftp_getrc(connection, NULL, 0, 0);
	switch (response) {
		case 230 : log(LOG_ERROR, "Don't need to send password\n");
			   connection->status = STATUS_IDLE;
			   break;
		case 530 : log(LOG_ALERT, "Not logged in as %s\n", connection->username);
			   return response;
			   break;
		case 500 : log(LOG_ALERT, "Server doesn't understand USER\n");
			   ftp_disconnect(connection);
			   return response;
			   break;
		case 501 : log(LOG_ALERT, "Server can't understand USER command\n");
			   ftp_disconnect(connection);
			   return response;
			   break;
		case 421 : log(LOG_ALERT, "Service unavailable\n");
			   ftp_disconnect(connection);
			   return response;
			   break;
		case 332 : log(LOG_ALERT, "This program does not support user accounts\n");
			   return response;
			   break;
		case 331 : log(LOG_MESSAGE, "Send password xxxxxxx");
			   sprintf(line, "PASS %s\r\n", connection->password);
			   response = ftp_sendline(connection, line);
			   if (log_get_level() >= 5) show_motd = 1;
			   response = ftp_getrc(connection, NULL, 0, show_motd);
			   switch (response) {
				   case 230 :
				   case 202 : break;
				   case 530 : log(LOG_CRITICAL, "Login incorrect\n");
					      ftp_disconnect(connection);
					      connection->status = STATUS_WAIT_CONNECT;
					      return response;
					      break;
				   case 500 :
				   case 503 :
				   case 501 : log(LOG_ALERT, "Server doesn't understand PASS\n");
					      ftp_disconnect(connection);
					      return response;
					      break;
				   case 421 : log(LOG_ALERT, "ftp_login: Service is unavailable\n");
					      ftp_disconnect(connection);
					      return response;
					      break;
				   case 332 :
				   default  : log(LOG_ALERT, "Unknown response to PASS: %d\n", response);
					      return response;
					      break;
			   }
			   break;
		default  : log(LOG_ALERT, "Unknown response to USER: %d\n", response);
			   return response;
			   break;
	}
	connection->status = STATUS_IDLE;
	log(LOG_MESSAGE, "Getting remote system type");
	ftp_sendline(connection, "SYST\r\n");
	response = ftp_getrc(connection, connection->system, 128, 0);
	if (response == 215) {
		log(LOG_MESSAGE, "Remote system type is %s\n", connection->system);
	} else {
		memset(connection->system, 0, sizeof(connection->system));
	}
	log(LOG_WARNING, "Logged in to %s as %s\n", connection->hostname,
			connection->username);
	connection->status = STATUS_IDLE;
	ftp_pwd(connection);
	return 0;
}

