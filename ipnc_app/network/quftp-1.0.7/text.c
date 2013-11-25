/* Text Functions Functions
   $Revision: 1.4 $
   $Date: 2002/06/24 04:04:32 $
*/

#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pwd.h>
#include "ftp.h"
#include "text.h"
#include "quftp.h"

char *getnextword(char *text, int state) {
	static char *location = NULL;
	static char *textstart = NULL;
	static char *returnstring = NULL;
	char *startlocation;
	int length = 0;
	int quotes = 0;

	if (returnstring) {
		free(returnstring);
		returnstring = (char *)NULL;
	}
	if (!text) return (char *)NULL;
	if (textstart != text && state != GNW_START) {
		log(LOG_CRITICAL, "bad string start... error in program logic!\n");
		exit(0);
	}
	if (state == GNW_START) {	// New string, start from the beginning
		location = textstart = text;
	}

	// Cut off leading spaces
	while (*location == ' ') location++;

	// Want the rest of the string?
	if (state == GNW_REST) return location;

	// Look at each character in turn
	startlocation = location;
	length = 0;
	if (*location == '"') {
		quotes = 1;
		startlocation++;
		location++;
	}
	while (*location) {
		if (*location == ' ' && quotes == 0) break;
		if (*location == '"') break;
		if (*location == '\t') break;
		if (*location == '\r') break;
		if (*location == '\n') break;
		length++;
		location++;
	}

	// If there was no text, return NULL otherwise return the string
	if (length == 0) return (char *)NULL;
	returnstring = (char *)malloc(length + 1);
	strncpy(returnstring, startlocation, length);
	returnstring[length] = '\0';
	if (*location != '\0') location++;
	return returnstring;
}

char *ftp_strip_illegal(char *text) {
	int moveback = 0;
	int counter;
	unsigned char test;
	for (counter = 0; counter < strlen(text); counter++) {
		test = text[counter + moveback];
		if (test < 27 || test > 127) {
			moveback++;
			text[counter+moveback] = 0;
			continue;
		}
		text[counter] = text[counter + moveback];
	}
	return text;
}

int parseurl(char *url, struct ftpconnection *connection) {
	char *tempurl, *temp, *temp2, *originalurl;

	if (!connection) return 0;
	/* Look for a protocol specification (ftp://) */
	if ((temp = strstr(url, "://"))) {
		if (strstr(url, "ftp://")) {
			tempurl = (char *)malloc(strlen(url) - 5);
			strcpy(tempurl, temp + 3);
		} else {
			log(LOG_CRITICAL, "ftp is the only supported protocol\n");
			return 0;
		}
	} else {
		tempurl = strdup(url);
	}

	originalurl = tempurl;
	ftp_strip_illegal(tempurl); /* Strip off the crap */

	/* Set the Username and Password */
	if ((temp = strstr(tempurl, "@"))) {
		temp2 = tempurl;
		tempurl = temp + 1;
		temp[0] = 0;
		/* Check for password as well */
		if ((temp = strstr(temp2, ":"))) {
			temp[0] = 0;
			strncpy(connection->username, temp2, 32);
			strncpy(connection->password, temp + 1, 32);
		} else {
			strncpy(connection->username, temp2, 32);
			strncpy(connection->password, getpassword(connection->username), 32);
		}
	} else {
		strncpy((char *)connection->username, "anonymous", 32);
	}

	/* Grab the path off the end */
	if ((temp = strstr(tempurl, "/"))) {
		int pathlength;
		pathlength = strlen(tempurl) - (tempurl - url);
		strncpy(connection->remotedir, temp, 256);
		*temp = '\0';
	} else
		connection->remotedir[0] = '\0';

	/* Grab a port if there is one */
	if ((temp = strstr(tempurl, ":"))) {
		connection->port = atoi(temp + 1);
		temp[0] = 0;
		if (connection->port == 0) connection->port = 21;
	} else {
		connection->port = ftp_port; //modified by mikewang
	}
printf("in text.c...ftp port = %d\n",connection->port);
	/* The rest is the host name */
	if (strlen(tempurl) == 0) {
		log(LOG_CRITICAL, "Hostname is too short!\n");
		free(originalurl);
		return 0;
	}
	strncpy(connection->hostname, tempurl, 64);
	connection->status = STATUS_WAIT_CONNECT;
	free(originalurl);
	return 1;
}

char *getpassword(char *username) {
	char *temp = NULL, *password = NULL;
	temp = (char *)malloc(500);
	sprintf(temp, "Enter password for %s: ", username);
	password = getpass(temp);
	free(temp);
	return password;
}

char *expand_home(char *s) {
	if (s[0] == '~') {
		char *home = NULL, *sl, *ret_path = NULL;

		if (s[1] == 0 || s[1] == '/')
			home = getenv("HOME");
		else {
			char name[9];
			struct passwd *pw;
			strncpy(name, s + 1, 8);
			name[8] = 0;
			sl = strchr(name, '/');
			if (sl) *sl = 0;
			pw = getpwnam(name);
			if (pw) home = pw->pw_dir;
		}
		if (home == 0) return strdup(s);
		sl = strchr(s, '/');
		if (sl) {
			ret_path = (char *)realloc(ret_path, strlen(sl) + strlen(home)+1);
			strcpy(ret_path, home);
			strcat(ret_path, sl);
			return ret_path;
		}
		return home;
	}
	return strdup(s);
}

char *strupper(char *string) {
	char *ptr;
	for (ptr = string; ptr && *ptr; ptr++) 
		if (*ptr >= 'a' && *ptr <= 'z') *ptr -= 32;
	return string;
}

char *strlower(char *string) {
	char *ptr;
	for (ptr = string; ptr && *ptr; ptr++) 
		if (*ptr >= 'A' && *ptr <= 'Z') *ptr += 32;
	return string;
}

char *getpath(const char *path) {
	char *returnstring, *p, *temp;
	if (!path || !*path) return NULL;
	temp = strdup(path);
	p = temp + strlen(temp);
	while (p >= temp) {
		if (*p == '/') {
			int length = p - temp;
			returnstring = (char *)malloc(length + 1);
			memset(returnstring, 0, length);
			strncpy(returnstring, temp, length);
			free(temp);
			return returnstring;
		}
		p--;
	}
	free(temp);
	return NULL;
}

char *getfilename(char *path) {
	char *p;
	if (!path || !*path) return path;
	p = path + strlen(path);
	while (p >= path) {
		if (*p == '/') {
			if (p != (path + strlen(path)))
				return strdup(++p);
			else 
				return NULL;
		}
		p--;
	}
	return strdup(path);
}

