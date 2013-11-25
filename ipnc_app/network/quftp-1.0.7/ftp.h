/* FTP Functions
   $Revision: 1.3 $
   $Date: 2003/03/10 22:31:27 $
*/

#ifndef __FTP_H__
#define __FTP_H__

#include <time.h>
#include "quftp.h"

#define MAX_LINE_SIZE		200
#define MAX_DIR_ENTRIES		200

#define TYPE_FILE		0
#define TYPE_DIRECTORY		1
#define TYPE_LINK		2
#define TYPE_SPECIAL		3
#define TYPE_OTHER		99

#define STATUS_NEW		0
#define STATUS_WAIT_CONNECT	1
#define STATUS_WAIT_LOGIN	2
#define STATUS_IDLE		3
#define STATUS_WAITING		4
#define STATUS_TRANSFER		5
#define STATUS_ERROR		99

#define CONFIG_PASSIVE		1
#define CONFIG_SHOW_MOTD	2
#define CONFIG_SHOW_HASH	4

#define TRANS_START		0
#define TRANS_TRANS		-1
#define TRANS_CLOSE		-2

#define IS_NUMBER(x)		(x >= '0' && x <= '9')

#define SOCKADDR struct sockaddr

#ifdef __GLIBC__
	#define HERROR hstrerror(h_errno)
	#define SOCKLEN socklen_t
#else
	#define HERROR h_errno
	#define SOCKLEN int
#endif

struct ftpconnection {
	char hostname[64];
	unsigned short port;  //modified by mikewang
	char system[128];
	unsigned long localip;
	char username[32];
	char password[32];
	char remotedir[256];
	char localdir[256];
	int status;
	int controlfd;
	int datafd;
	int dataconfd;
	struct variable_list *variables;
};

struct filedetails {
	char filename[255];
	int permissions;
	int links;
	char owner[64];
	char group[64];
	struct tm timestamp;
	unsigned long size;
};

extern int ftp_debug;

struct ftpconnection *ftp_new(void);
int ftp_connect(struct ftpconnection *connection);
int ftp_disconnect(struct ftpconnection *connection);
int ftp_reset(struct ftpconnection *connection);
char *ftp_readline(struct ftpconnection *connection, int cleanup);
int ftp_sendline(struct ftpconnection *connection, char *line);
int ftp_login(struct ftpconnection *connection);
int ftp_getrc(struct ftpconnection *connection, char *data, int max_size, int print_line);
int ftp_cwd(struct ftpconnection *connection, char *path);
char *ftp_pwd(struct ftpconnection *connection);
int ftp_mkd(struct ftpconnection *connection, char *path);
int ftp_rmd(struct ftpconnection *connection, char *path);
int ftp_list(struct ftpconnection *connection, char *params, char showthem);
int ftp_parse_pasv(char *line, char *address);
int ftp_open_data_pasv(struct ftpconnection *connection, char *address, int port);
int ftp_close_data(struct ftpconnection *connection);
int ftp_read_data(struct ftpconnection *connection, char *buffer, int max_size);
int ftp_write_data(struct ftpconnection *connection, char *buffer, int size);
int ftp_read_data_line(struct ftpconnection *connection, char *buffer, int max_size);
int ftp_parse_permissions(char *perm);
int ftp_get_file(struct ftpconnection *connection, struct qitem *item);
int ftp_put_file(struct ftpconnection *connection, struct qitem *item);
int ftp_nlst(struct ftpconnection *connection, char *params, char *list[], int max_items);
int ftp_open_data(struct ftpconnection *connection);
int ftp_wait_data(struct ftpconnection *connection);
int ftp_check_ready(struct ftpconnection *connection, int print);
int ftp_stat(struct ftpconnection *connection, char *filename, struct filedetails *details);
int ftp_split_ls(char *buffer, struct filedetails *details);
int ftp_fxp(struct ftpconnection *left, struct ftpconnection *right, char *source, char *dest);
int ftp_read_file(struct ftpconnection *connection, char *data, int max_size, long status);
int ftp_delete(struct ftpconnection *connection, char *filename);
int ftp_exists(struct ftpconnection *connection, char *filename);
char *ftp_error_message(int error);
char *ftp_glob(struct ftpconnection *connection, char *wildcard, int status);
char *listoutput(char *input, char *format);

#endif
