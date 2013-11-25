/* FTP File Transfer Functions
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
#include <sys/time.h>
#include "ftp.h"
#include "text.h"
#include "rc.h"
#include "quftp.h"

int ftp_get_file(struct ftpconnection *connection, struct qitem *item) {
	int fd, response, n, counter;
	unsigned long resume = 0;
	char line[MAX_LINE_SIZE], *buffer;
	struct stat statbuf;
	struct timeval tv;
	int updateinterval = 1;
	int updatetime = 1, bytesthissecond = 0;
	double starttime = 0.0, timelength, speed = 0.0;
	if (!item) return 0;
	log(LOG_INFO, "Retrieving \"%s/%s\" from %s\n", connection->remotedir, item->remotefilename,
			connection->hostname);
	if (!ftp_check_ready(connection, 1)) return -1;
	ftp_sendline(connection, "TYPE I\r\n");
	response = ftp_getrc(connection, NULL, 0, 0);
	switch (response) {
		case 200 : log(LOG_INFO, "Binary mode\r");
			   break;
		case 421 : log(LOG_ALERT, "Service unavailable\n");
			   ftp_disconnect(connection);
			   return -response;
			   break;
		case 500 : log(LOG_ALERT, "Server doesn't understand TYPE\r");
			   break;
		case 501 : log(LOG_ALERT, "Server doesn't understand TYPE parameters\r");
			   break;
		case 502 : log(LOG_ALERT, "Server doesn't understand TYPE\r");
			   break;
		case 504 : log(LOG_ALERT, "Server does not support IMAGE type\r");
			   break;
		case 530 : log(LOG_ALERT, "Not logged in\n");
			   ftp_disconnect(connection);
			   return -response;
			   break;
		default  : log(LOG_ALERT, "Unknown response to TYPE: %d\r", response);
			   break;
	}
	/* How often should the progress bar be updated (if at all)? */
	if (config(connection->variables, "progress_update"))
		updateinterval = atoi(config(connection->variables, "progress_update"));
	if (updateinterval < 1) updateinterval = 0;

	/* Passive mode or normal? */
	buffer = config(connection->variables, "passive");
	if (buffer && strcasecmp(buffer, "true") == 0)
		ftp_open_data_pasv(connection, NULL, 0);
	else
		ftp_open_data(connection);

	if (item->size >= 0) {
		if (!stat(item->localfilename, &statbuf)) {
			response = 0;
			if (statbuf.st_size < item->size) {
				/* Restart from statbuf.st_size bytes */
				log(LOG_INFO, ", restarting from %lu bytes", statbuf.st_size);
				sprintf(line, "REST %lu\r\n", statbuf.st_size);
				ftp_sendline(connection, line);
				response = ftp_getrc(connection, NULL, 0, 0);
				if (response >= 500) {
					response = -1;
				} else if (response == 421) {
					log(LOG_ALERT, "Can't restart.. fatal error!\n");
					return -421;
				}
				resume = statbuf.st_size;
			}
			if (statbuf.st_size >= item->size || response == -1) {
				/* Make a backup copy and start again */
				log(LOG_WARNING, "Backing up old file to %s.quftp", item->localfilename);
				sprintf(line, "%s.quftp", item->localfilename);
				rename(item->localfilename, line);
			}
		}
	} else {
		log(LOG_WARNING, "Can't get filesize.. not displaying progress bar\n");
	}

	/* Sent RETR command and attempt to handle response */
	sprintf(line, "RETR %s\r\n", item->remotefilename);
	ftp_sendline(connection, line);
	response = ftp_getrc(connection, line, 0, 0);
	switch (response) {
		case 250 :
		case 350 :
		case 125 : break;
		case 150 : if (!ftp_wait_data(connection)) {	// Transfer ok
					   ftp_close_data(connection);
					   return -response;
				   }
				   break;
		case 110 : log(LOG_ALERT, "Server wants to restart.. don't know how yet!\n");
				   return -response;
				   break;
		case 226 : log(LOG_ALERT, "Got a closing data connection message... now?\n");
				   return -response;
				   break;
		case 421 : log(LOG_ALERT, "Service unavailable\n");
				   ftp_disconnect(connection);
				   ftp_close_data(connection);
				   return -response;
				   break;
		case 425 : log(LOG_ALERT, "Server can't open data connection\n");
				   return -response;
				   break;
		case 426 :
		case 451 : log(LOG_ALERT, "Server aborted transfer\n");
				   return -response;
				   break;
		case 500 : log(LOG_ALERT, "Server doesn't understand RETR\n");
				   ftp_close_data(connection);
				   return -response;
				   break;
		case 501 : log(LOG_ALERT, "Server doesn't understand RETR parameters\n");
				   ftp_close_data(connection);
				   return -response;
				   break;
		case 502 : log(LOG_ALERT, "Server doesn't understand RETR\n");
				   ftp_close_data(connection);
				   return -response;
				   break;
		case 530 : log(LOG_ALERT, "Not logged in\n");
				   ftp_close_data(connection);
				   return -response;
				   break;
		case 550 : { char *temp = &line[0];
				   while (temp && *temp != ':') temp++;
				   temp += 2;
				   log(LOG_WARNING, "Couldn't retrieve %s: %s\n", item->remotefilename, temp);
				   ftp_close_data(connection);
				   return -response; }
		default  : log(LOG_ALERT, "Unknown response to RETR: %d\n", response);
				   return -response;
				   break;
	}
	counter = 0;
	if (!(fd = open(item->localfilename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR))) {
		log(LOG_ALERT, "Can't create local file %s\n", item->localfilename);
		return -1;
	}
	if (resume) {
		lseek(fd, 0, SEEK_END);
		counter = resume;
	}
	setbuf(stdout, NULL);
	buffer = (char *)malloc(4096);

	connection->status = STATUS_TRANSFER;

	/* Start the progress bar display */
	if (item->size > 0 && updateinterval > 0) progress_bar(0, item->size, speed);

	/* Transfer the file */
	while (1) {
		if ((item->size >= 0) && updatetime) {
			gettimeofday(&tv, NULL);
			starttime = ((tv.tv_sec * 1.0) + (tv.tv_usec / 1000000.0));
			updatetime = 0;
			bytesthissecond = 0;
		}

		n = ftp_read_data(connection, buffer, 4096);
		if (n == 0) break;
		write(fd, buffer, n);
		counter += n;
		bytesthissecond += n;

		if (item->size >= 0) {
			gettimeofday(&tv, NULL);
			timelength = ((tv.tv_sec * 1.0) + (tv.tv_usec / 1000000.0)) - starttime;
			if (timelength >= updateinterval) {
				updatetime = 1;
				speed = (bytesthissecond / 1000.0) / (timelength * 1.0);
				if (item->size > 0 && updateinterval > 0) progress_bar(counter, item->size, speed);
			}
		}
	}

	/* Finish off the progress bar */
	if (item->size > 0 && updateinterval > 0) progress_bar(item->size, item->size, speed);

	connection->status = STATUS_IDLE;

	close(fd);
	ftp_close_data(connection);
	ftp_getrc(connection, NULL, 0, 0);
	free(buffer);
	return counter;
}

int ftp_put_file(struct ftpconnection *connection, struct qitem *item) {
	int fd, response, n, counter;
	int updatetime = 1, bytesthissecond = 0, updateinterval = 1;
	char line[MAX_LINE_SIZE], *buffer;
	struct stat statbuf;
	struct timeval tv;
	double starttime = 0.0, timelength, speed = 0.0;
	if (!ftp_check_ready(connection, 1)) return -1;
	if (!item) return -1;
	log(LOG_INFO, "Begin transferring %s\r", item->remotefilename);
	ftp_sendline(connection, "TYPE I\r\n");
	ftp_getrc(connection, NULL, 0, 0);
	if (!(fd = open(item->localfilename, O_RDONLY))) {
		log(LOG_ALERT, "%s doesn't exist!\n", item->localfilename);
		return -1;
	}
	fstat(fd, &statbuf);
	sprintf(line, "ALLO %lu\r\n", statbuf.st_size);
	ftp_sendline(connection, line);
	response = ftp_getrc(connection, NULL, 0, 0);
	switch (response) {
		case 200 :
		case 202 : break;
		case 504 : log(LOG_ALERT, "ALLO command does not implement parameter\r");
			   break;
		case 500 : log(LOG_ALERT, "Server doesn't understand ALLO command\r");
			   break;
		case 501 : log(LOG_ALERT, "Syntax error in ALLO command\n");
			   break;
		case 421 : log(LOG_ALERT, "Server shutting down\n");
			   ftp_close_data(connection);
			   ftp_disconnect(connection);
			   return -1;
		case 530 : log(LOG_ALERT, "Server says not logged in!\n");
			   ftp_close_data(connection);
			   ftp_disconnect(connection);
			   return -1;
		default  : log(LOG_ALERT, "ALLO: Unrecognised error %d\n", response);
			   ftp_close_data(connection);
			   ftp_disconnect(connection);
			   break;
	}
	if (config(connection->variables, "progress_update"))
		updateinterval = atoi(config(connection->variables, "progress_update"));
	if (updateinterval < 1) updateinterval = 1;

	buffer = config(connection->variables, "passive");
	if (buffer && strcasecmp(buffer, "true") == 0)
		ftp_open_data_pasv(connection, NULL, 0);
	else ftp_open_data(connection);

	sprintf(line, "STOR %s\r\n", item->remotefilename);
	ftp_sendline(connection, line);
	response = ftp_getrc(connection, NULL, 0, 0);
	switch (response) {
		case 125 : 
		case 150 : if (connection->status == STATUS_WAITING)
				   if (!ftp_wait_data(connection)) {
					   ftp_close_data(connection);
					   return -response;
				   }
				   break;
		case 532 : log(LOG_ALERT, "Server requires an account.. not supported yet\n");
			   ftp_close_data(connection);
			   return -response;
			   break;
		case 450 : log(LOG_ALERT, "Remote file is busy\n");
			   ftp_close_data(connection);
			   return -response;
			   break;
		case 452 : log(LOG_ALERT, "Insufficient storage space\n");
			   ftp_close_data(connection);
			   return -response;
			   break;
		case 553 : log(LOG_ALERT, "Permission denied to upload \"%s\"\n", item->remotefilename);
			   ftp_close_data(connection);
			   return -response;
			   break;
		case 500 : log(LOG_ALERT, "Server doesn't understand STOR command\n");
			   ftp_close_data(connection);
			   return -response;
			   break;
		case 501 : log(LOG_ALERT, "Syntax error in STOR command\n");
			   ftp_close_data(connection);
			   return -response;
			   break;
		case 421 : log(LOG_ALERT, "Server shutting down\n");
			   ftp_close_data(connection);
			   ftp_disconnect(connection);
			   return -response;
			   break;
		case 530 : log(LOG_ALERT, "Server says not logged in!\n");
			   ftp_close_data(connection);
			   ftp_disconnect(connection);
			   return -response;
			   break;
		default  : log(LOG_ALERT, "Unrecognised error %d\n", response);
			   ftp_close_data(connection);
			   ftp_disconnect(connection);
			   break;
	}
	counter = 0;
	buffer = (char *)malloc(0x100000);

	progress_bar(0, item->size, 0);
	while (1) {
		if (updatetime) {
			gettimeofday(&tv, NULL);
			starttime = ((tv.tv_sec * 1.0) + (tv.tv_usec / 1000000.0));
			updatetime = 0;
			bytesthissecond = 0;
		}

		n = read(fd, buffer, 0x100000);
		if (n == 0) break;
		if (!ftp_write_data(connection, buffer, n)) break;
		counter += n;
		bytesthissecond += n;

		gettimeofday(&tv, NULL);
		timelength = ((tv.tv_sec * 1.0) + (tv.tv_usec / 1000000.0)) - starttime;
		if (timelength >= updateinterval) {
			updatetime = 1;
			speed = (bytesthissecond / 1000.0) / (timelength * 1.0);
			progress_bar(counter, item->size, speed);
		}
	}
	progress_bar(item->size, item->size, speed);
	free(buffer);
	close(fd);
	ftp_close_data(connection);
	ftp_getrc(connection, NULL, 0, 0);
	return counter;
}

int ftp_fxp(struct ftpconnection *left, struct ftpconnection *right, char *source, char *dest) {
	char *line = NULL, *temp = NULL, *command = NULL;
	int response;
	int error = 0;

	if (!left || !right) return 1;
	if (!source || !*source) return 1;
	if (!dest || !*dest) return 1;
	if (!ftp_check_ready(left, 1)) return 1;
	if (!ftp_check_ready(right, 1)) return 1;

	/* Set Data mode to IMAGE */
	log(LOG_INFO, "Binary mode");
	ftp_sendline(left, "TYPE I\r\n");
	ftp_getrc(left, NULL, 0, 0);
	ftp_sendline(right, "TYPE I\r\n");
	ftp_getrc(right, NULL, 0, 0);

	/* Initiate data connection */
	log(LOG_INFO, "Create data connection\r");
	ftp_sendline(right, "PASV\r\n");
	line = (char *)malloc(MAX_LINE_SIZE);
	response = ftp_getrc(right, line, MAX_LINE_SIZE, 0);
	if (response != 227) {
		log(LOG_ALERT, "Receiving server does not support PASV transfers: %d\n", response);
		return response;
	}
	right->status = STATUS_WAITING;
	temp = strstr(line, "(");
	temp++;
	temp[(strstr(temp, ")") - temp)] = 0;
	command = (char *)malloc(MAX_LINE_SIZE);
	sprintf(command, "PORT %s\r\n", temp);
	free(line);
	ftp_sendline(left, command);
	response = ftp_getrc(left, NULL, 0, 0);
	if (response != 200) {
		log(LOG_ALERT, "Can't establish PORT connection on sending server: %d\n", response);
		ftp_close_data(right);
		free(command);
		return response;
	}
	left->status = STATUS_WAITING;

	/* Begin transfer */
	log(LOG_WARNING, "Begin transferring %s to %s\n", source, dest);
	sprintf(command, "STOR %s\r\n", dest);
	ftp_sendline(right, command);
	right->status = STATUS_TRANSFER;

	sprintf(command, "RETR %s\r\n", source);
	ftp_sendline(left, command);
	left->status = STATUS_TRANSFER;

	/* Finish transfer */
	ftp_close_data(left);
	ftp_close_data(right);
	left->status = right->status = STATUS_IDLE;
	free(command);

	response = ftp_getrc(left, NULL, 0, 0);
	switch (response) {
		case 550 : log(LOG_ALERT, "Error: %s does not exist\n", source); break;
		case 150 :
		case 125 : response = ftp_getrc(left, NULL, 0, 0);
			   switch (response) {
				   case 226 :
				   case 250 : break; /* Transfer OK */
				   case 425 : log(LOG_ALERT, "Error opening data connection\n"); error = 425; break;
				   case 426 : log(LOG_ALERT, "FxP Aborted\n"); error = 426; break;
				   case 451 : log(LOG_ALERT, "FxP Aborted\n"); error = 451; break;
				   case 551 : log(LOG_ALERT, "FxP Aborted\n"); error = 551; break;
				   case 552 : log(LOG_ALERT, "FxP Aborted.. not enough space\n"); error = 552; break;
			   }
			   break;
		default  : break;
	}

	response = ftp_getrc(right, NULL, 0, 0);
	switch (response) {
		case 553 : log(LOG_ALERT, "Error: Permission denied uploading %s\n", dest);
			   error = 553;
			   break;
		case 150 :
		case 125 : response = ftp_getrc(right, NULL, 0, 0);
			   switch (response) {
				   case 226 :
				   case 250 : break; /* Transfer OK */
				   case 425 : log(LOG_ALERT, "Error opening data connection\n");
					      error = 425;
					      break;
				   case 426 : log(LOG_ALERT, "FxP Aborted\n"); error = 426; break;
				   case 451 : log(LOG_ALERT, "FxP Aborted\n"); error = 451; break;
				   case 551 : log(LOG_ALERT, "FxP Aborted\n"); error = 551; break;
				   case 552 : log(LOG_ALERT, "FxP Aborted.. not enough space\n");
					      error = 552;
					      break;
			   }
			   break;
		default  : break;
	}
	return error;
}

int ftp_read_file(struct ftpconnection *connection, char *data, int max_size, long status) {
	int response, n;
	char line[MAX_LINE_SIZE];
	if (status == TRANS_TRANS) {
		// Transfer data
		if (data == NULL) return -1;
		if (connection->status != STATUS_TRANSFER) return -1;
		n = ftp_read_data(connection, data, max_size);
		return n;
	} else if (status == TRANS_CLOSE) {
		// Close data connection
		if (connection->status != STATUS_TRANSFER) return -1;
		ftp_close_data(connection);
		ftp_getrc(connection, NULL, 0, 0);
		connection->status = STATUS_IDLE;
		return 0;
	} else {
		char *buffer;
		// Open a new file connection starting from 'status' bytes
		if (data == NULL) return -1;
		if (!ftp_check_ready(connection, 1)) return -1;
		log(LOG_WARNING, "Binary mode\r");
		ftp_sendline(connection, "TYPE I\r\n");
		ftp_getrc(connection, NULL, 0, 0);

		buffer = config(connection->variables, "passive");
		if (buffer && strcasecmp(buffer, "true") == 0)
			ftp_open_data_pasv(connection, NULL, 0);
		else ftp_open_data(connection);

		if (status > 0) {
			sprintf(line, "REST %lu\r\n", status);
			ftp_sendline(connection, line);
			response = ftp_getrc(connection, NULL, 0, 0);
			if (response >= 500) {
				response = -1;
			} else if (response == 421) {
				log(LOG_ALERT, "Can't restart.. fatal error!\n");
				return -421;
			} else {
				log(LOG_ALERT, "Unknown error %d\n", response);
				return -response;
			}
		}
		log(LOG_ALERT, "Read file %s\r", data);
		sprintf(line, "RETR %s\r\n", data);
		ftp_sendline(connection, line);
		response = ftp_getrc(connection, line, 0, 0);
		if (response == 150) {
			ftp_wait_data(connection);
		} else if (response == 125) {
		} else if (response == 550) {
			char *temp = &line[0];
			while (temp && *temp != ':') temp++;
			temp += 2;
			log(LOG_ALERT, "Couldn't retrieve %s: %s\n", data, temp);
			ftp_close_data(connection);
			return -response;
		} else {
			log(LOG_ALERT, "Unknown server response %d\n", -response);
			return -response;
		}
		connection->status = STATUS_TRANSFER;
		return 0;
	}
	return 0;
}

