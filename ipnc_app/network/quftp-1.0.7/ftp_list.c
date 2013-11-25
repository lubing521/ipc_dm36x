/* FTP Directory List Functions
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
#include "ftp.h"
#include "rc.h"
#include "text.h"
#include "quftp.h"

int ftp_list(struct ftpconnection *connection, char *params, char showthem) {
	char *buffer = NULL;
	int response = 0, index = 0;

	char *outputformat = NULL;

	if (!ftp_check_ready(connection, 1)) return -1;
	ftp_sendline(connection, "TYPE A\r\n");	// Go to ASCII mode
	ftp_getrc(connection, NULL, 0, 0);
	buffer = config(connection->variables, "passive");
	if (!buffer || strcasecmp(buffer, "true") == 0)
		response = ftp_open_data_pasv(connection, NULL, 0);
	else response = ftp_open_data(connection);
	if (response) {
		log(LOG_ALERT, "ftp_list: bad response from ftp_open_data (%d)\n",
				response);
		return -response;	// Can't open a data connection
	}
	if (params && *params) {
		buffer = (char *)malloc(MAX_LINE_SIZE);
		sprintf(buffer, "LIST %s\r\n", params);
		ftp_sendline(connection, buffer);
		free(buffer);
	} else ftp_sendline(connection, "LIST\r\n");
	response = ftp_getrc(connection, NULL, 0, 0);
	switch (response) {
		case 125 : /* Already a data connection */
			   break;
		case 150 : ftp_wait_data(connection);	/* Wait for the data connection */
			   break;
		case 250 :
		case 226 : log(LOG_INFO, "No files available\n");
			   return response;
			   break;
		case 425 :
		case 426 :
		case 451 :
		case 450 : log(LOG_ALERT, "Could not list directory\n");
			   return response;
			   break;
		case 421 : log(LOG_ALERT, "Service unavailable\n");
			   ftp_disconnect(connection);
			   return response;
			   break;
		case 500 : log(LOG_ALERT, "Server doesn't understand LIST\n");
			   return response;
			   break;
		case 501 : log(LOG_ALERT, "Server doesn't understand LIST parameters\n");
			   return response;
			   break;
		case 502 : log(LOG_ALERT, "Server doesn't understand LIST\n");
			   return response;
			   break;
		case 530 : log(LOG_ALERT, "Not logged in\n");
			   return 0;
			   break;
		default  : log(LOG_ALERT, "Unknown response to LIST: %d\n", response);
			   return response;
			   break;
	}
	log(LOG_MESSAGE, "ftp_list: processing list\n");
	buffer = (char *)malloc(4096);
	outputformat = config(connection->variables, "lsformat");
	while (1) {
		char *temp;
		memset(buffer, 0, 4096);
		if ((response = ftp_read_data_line(connection, buffer, 4096))
				== 0) break; 		/* Read Line */
		if (strlen(buffer) == 0) continue;	/* Got crap? */
		if (ftp_parse_permissions(buffer) < 0) continue;
					/* Parse -rw-r--r-- etc */
		temp = strchr(buffer, '\n');
		if (temp) *temp = 0;
		if (showthem) {
			if (outputformat && *outputformat)
				printf("%s\n", listoutput(buffer, outputformat));
			else
				printf("%s\n", buffer);
		}
		index++;		/* Keep a count of files */
	}
	free(buffer);
	ftp_close_data(connection);		/* Close the data connection */
	response = ftp_getrc(connection, NULL, 0, 0);	/* Server says ok? */
	return index;
}

int ftp_nlst(struct ftpconnection *connection, char *params, char *list[], int max_items) {
	char *buffer;
	int index, response;
	if (!ftp_check_ready(connection, 0)) return -1;
	if (max_items > 0) memset(list, 0, sizeof(*list) * max_items);
	ftp_sendline(connection, "TYPE A\r\n");
	ftp_getrc(connection, NULL, 0, 0);

	buffer = config(connection->variables, "passive");
	if (!buffer || strcasecmp(buffer, "true") == 0)
		response = ftp_open_data_pasv(connection, NULL, 0);
	else response = ftp_open_data(connection);

	index = 0;
	if (params && *params) {
		buffer = (char *)malloc(MAX_LINE_SIZE);
		sprintf(buffer, "NLST %s\r\n", params);
		ftp_sendline(connection, buffer);
		free(buffer);
	} else ftp_sendline(connection, "NLST\r\n");
	response = ftp_getrc(connection, NULL, 0, 0);
	switch (response) {
		case 125 : /* Already a data connection */
			   break;
		case 150 : ftp_wait_data(connection);	/* Wait for the data connection */
			   break;
		case 250 :
		case 226 : printf("No files available\n");
			   connection->status = STATUS_IDLE;
			   return response;
			   break;
		case 425 :
		case 426 :
		case 451 :
		case 450 : log(LOG_ALERT, "Could not list directory\n");
			   connection->status = STATUS_IDLE;
			   return response;
			   break;
		case 421 : log(LOG_ALERT, "Service unavailable\n");
			   ftp_disconnect(connection);
			   return response;
			   break;
		case 500 : log(LOG_ALERT, "Server doesn't understand NLST\n");
			   connection->status = STATUS_IDLE;
			   return response;
			   break;
		case 501 : log(LOG_ALERT, "Server doesn't understand NLST parameters\n");
			   connection->status = STATUS_IDLE;
			   return response;
			   break;
		case 502 : log(LOG_ALERT, "Server doesn't understand NLST\n");
			   connection->status = STATUS_IDLE;
			   return response;
			   break;
		case 530 : log(LOG_ALERT, "Not logged in\n");
			   ftp_disconnect(connection);
			   return 0;
			   break;
		case 550 : /* log(LOG_ALERT, "No files found matching %s\n", params); */
			   connection->status = STATUS_IDLE;
			   return 0;
			   break;
		default  : log(LOG_ALERT, "Unknown response to NLST: %d\n", response);
			   connection->status = STATUS_IDLE;
			   return response;
			   break;
	}
	buffer = (char *)malloc(4096);
	while ((response = ftp_read_data_line(connection, buffer, 4096) > 0)) {
		if (strlen(buffer) == 0) continue;
		if (strcmp(buffer, ".") == 0) continue;
		if (strcmp(buffer, "..") == 0) continue;
		if (max_items > 0) list[index] = strdup(buffer);
		index++;
		if (max_items > 0) if (index > max_items) break;
	}
	if (index >= max_items) index = max_items;
	/* else for (temp = index; temp < max_items; temp++) list[temp] = 0; */
	list[index] = NULL;
	ftp_getrc(connection, NULL, 0, 0);
	free(buffer);
	ftp_close_data(connection);
	return index;
}

char *listoutput(char *input, char *format) {
	struct filedetails details;
	int inputindex;
	char output[MAX_LINE_SIZE], *temp;
	strcpy(output, input);
	if (ftp_split_ls(output, &details))
		return NULL;
	memset(output, 0, MAX_LINE_SIZE);
	for (inputindex = 0; format[inputindex] != 0; inputindex++) {
		if (format[inputindex] == '%') {
			inputindex++;
			switch (format[inputindex]) {
				case '%' : sprintf(output, "%s%%", output);
					   break;
				case 'f' : sprintf(output, "%s%s", output, details.filename);
					   break;
				case 's' : sprintf(output, "%s%lu", output, details.size);
					   break;
				case 't' : sprintf(output, "%s%s", output, asctime(&details.timestamp));
					   break;
				case 'p' : temp = print_permissions(details.permissions);
					   sprintf(output, "%s%s", output, temp);
					   free(temp);
					   break;
				case 'o' : sprintf(output, "%s%s", output, details.owner);
					   break;
				case 'g' : sprintf(output, "%s%s", output, details.group);
					   break;
				case 'l' : sprintf(output, "%s%d", output, details.links);
					   break;
			}
		} else
			sprintf(output, "%s%c", output, format[inputindex]);
	}
	return strdup(output);
}

