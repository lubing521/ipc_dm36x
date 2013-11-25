/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996,97 Larry Doolittle <ldoolitt@jlab.org>
 *  Some changes Copyright (C) 1997 Jon Nelson <jnelson@boa.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* $Id: globals.h,v 1.65.2.3 2002/07/24 03:03:59 jnelson Exp $*/

#ifndef _GLOBALS_H
#define _GLOBALS_H

#ifdef DAVINCI_IPCAM

#include "Appro_interface.h"
#include <sys_env_type.h>

#define MAX_SDLIST_LENGTH	(128*1024)

#define MAX_HTML_LENGTH	25600
#define MAX_CMD_LENGTH	1024
#define MAX_CMD_NUM		64
typedef struct __COMMAND_ARGUMENT {
    char *name;
    char *value;
    int flags;
} COMMAND_ARGUMENT;

typedef enum {
	AUTHORITY_ADMIN = 0,
	AUTHORITY_OPERATOR,
	AUTHORITY_VIEWER,
	AUTHORITY_NONE = 9
} AUTHORITY;


struct __HTTP_URI;

#define MFLAG_IS_MEMORY    0x0001
#define BUSY_FLAG_AUDIO	0x01
#define BUSY_FLAG_VIDEO	0x02
#define AUDIO_SEND_SIZE      900

#endif

#ifdef PSIA_MEDIA

struct __PSIA_URI;

/* each service or resource supported by the device is assigned a node_t structure */
typedef struct node_t
{
#define NODE_MARKER 0x4E4F4445
	unsigned long	marker;
	unsigned long	count;

	int		    len;
	char		*name;
	const char	*version;
	int		    methods;
	int		    type;
   #define NODE_STD	        0x01
   #define NODE_SERVICE	    0x02
   #define NODE_RESOURCE	0x03

	/* description strings */
	const char	*get_query_str;
	const char	*get_inbound_str;
	const char	*get_function_str;
	const char	*get_result_str;
	const char	*get_notes_str;

	const char	*put_query_str;
	const char	*put_inbound_str;
	const char	*put_function_str;
	const char	*put_result_str;
	const char	*put_notes_str;

	const char	*post_query_str;
	const char	*post_inbound_str;
	const char	*post_function_str;
	const char	*post_result_str;
	const char	*post_notes_str;

	const char	*del_query_str;
	const char	*del_inbound_str;
	const char	*del_function_str;
	const char	*del_result_str;
	const char	*del_notes_str;

	int		(*process)(void *, void *);
	struct	node_t	*first_child;
	struct	node_t	*next;
	struct	node_t	*parent;
}node_t;

/* each element in the inbound xml data is assigned a tag_t structure */
typedef struct tag_t
{
	#define TAG_MARKER 0x54414754
	unsigned long	marker;
	unsigned long	count;
	int		        empty;		// empty flag

	char	*name;
	char	*value;
	char	*attr;

	char	*start_name;
	char	*value_name;
	char	*end_name;
	char	*attr_name;

	int		start_len;
	int		value_len;
	int		end_len;
	int		attr_len;

	struct	tag_t	*first_child;
	struct	tag_t	*next;
	struct	tag_t	*parent;
}tag_t;

#endif


struct mmap_entry {
    dev_t dev;
    ino_t ino;
    char *mmap;
    int use_count;
    size_t len;
};

struct alias {
    char *fakename;             /* URI path to file */
    char *realname;             /* Actual path to file */
    int type;                   /* ALIAS, SCRIPTALIAS, REDIRECT */
    int fake_len;               /* strlen of fakename */
    int real_len;               /* strlen of realname */
    struct alias *next;
};

typedef struct alias alias;

struct request {                /* pending requests */
    int fd;                     /* client's socket fd */
    int status;                 /* see #defines.h */
    time_t time_last;           /* time of last succ. op. */
    char *pathname;             /* pathname of requested file */
    int simple;                 /* simple request? */
    int keepalive;              /* keepalive status */
    int kacount;                /* keepalive count */

    int data_fd;                /* fd of data */
    unsigned long filesize;     /* filesize */
    unsigned long filepos;      /* position in file */
    char *data_mem;             /* mmapped/malloced char array */
    int method;                 /* M_GET, M_POST, etc. */

    char *logline;              /* line to log file */

    char *header_line;          /* beginning of un or incompletely processed header line */
    char *header_end;           /* last known end of header, or end of processed data */
    int parse_pos;              /* how much have we parsed */
    int client_stream_pos;      /* how much have we read... */

    int buffer_start;           /* where the buffer starts */
    int buffer_end;             /* where the buffer ends */

    char *http_version;         /* HTTP/?.? of req */
    int response_status;        /* R_NOT_FOUND etc. */

    char *if_modified_since;    /* If-Modified-Since */
    time_t last_modified;       /* Last-modified: */

    char local_ip_addr[NI_MAXHOST]; /* for virtualhost */

    /* CGI vars */

    int remote_port;            /* could be used for ident */

    char remote_ip_addr[NI_MAXHOST]; /* after inet_ntoa */

#ifdef DAVINCI_IPCAM
    struct __HTTP_URI *http_uri;
    int http_stream;
	char user_id[USER_LEN];
    AUTHORITY authority;
    int mem_flag;
    int busy_flag;
    char *mmap_ptr;
    unsigned long mmap_size;
    AV_DATA av_data;
    int serial_book;
    int serial_lock;
    char *cmd_string;
    int cmd_count;
    COMMAND_ARGUMENT cmd_arg[MAX_CMD_NUM];
    int audio_lock;
    int audio_book;
    int audio_sync;
#endif
    int is_cgi;                 /* true if CGI/NPH */
    int cgi_status;
    int cgi_env_index;          /* index into array */

    /* Agent and referer for logfiles */
    char *header_user_agent;
    char *header_referer;

    int post_data_fd;           /* fd for post data tmpfile */

    char *path_info;            /* env variable */
    char *path_translated;      /* env variable */
    char *script_name;          /* env variable */
    char *query_string;         /* env variable */
    char *content_type;         /* env variable */
    char *content_length;       /* env variable */
#ifdef PSIA_MEDIA
	/* psia specific fields */
    struct __PSIA_URI *psia_uri;

	const char	*command;
	char	*inbuf;
	int		inlen;
	tag_t	*prolog_tag;
	tag_t	*root_tag;
	int		tag_count;

	/* set this flag to enable index to be done recursively */
	int		indexr_flag;
	int 	list_index;

	/* response */
	char	*xmlstr;	// response buffer
	int		xmllen;		// bytes in response buffer
	int		xmlsize;	// response buffer size
	int		xmlerr;		// xml response error code
	int		response_complete;
	int		status_code;

	char	*status_string;
	/*end psia specific fields */
#endif
    struct mmap_entry *mmap_entry_var;

    struct request *next;       /* next */
    struct request *prev;       /* previous */

#ifdef DAVINCI_IPCAM
    char *authorization;
#endif

#ifdef SERVER_SSL
    void* ssl;
#endif /*SERVER_SSL*/

    /* everything below this line is kept regardless */
    char buffer[BUFFER_SIZE + 1]; /* generic I/O buffer */
    char request_uri[MAX_HEADER_LENGTH + 1]; /* uri */
    char client_stream[CLIENT_STREAM_SIZE]; /* data from client - fit or be hosed */
    char *cgi_env[CGI_ENV_MAX + 4];             /* CGI environment */

#ifdef ACCEPT_ON
    char accept[MAX_ACCEPT_LENGTH]; /* Accept: fields */
#endif
};

typedef struct request request;

struct status {
    long requests;
    long errors;
};


#ifdef DAVINCI_IPCAM

#define MAX_URI_HASH_SIZE		64

#define URI_FLAG_NEED_PARSE	    0x0001
#define URI_FLAG_VIRTUAL_PAGE  	0x0002
typedef struct __HTTP_URI
{
	char *name;
	int (*handler) (struct request *);
	AUTHORITY authority;
	int uri_flag;

	struct __HTTP_URI *next;

} HTTP_URI;

typedef struct __HTTP_OPTION {
	char			*name;
	void			(*handler)(request *, COMMAND_ARGUMENT *);
	AUTHORITY	authority;
	int			now;
	int			visiable;
	struct __HTTP_OPTION *next;
} HTTP_OPTION;

#define MAX_CMD_HASH_SIZE		64
typedef struct __CMD_HASH_TABLE {
	HTTP_OPTION *entry;
} CMD_HASH_TABLE;

#endif

#ifdef PSIA_MEDIA

#define MAX_PSIA_URI_HASH_SIZE		    64

#define PSIA_URI_FLAG_NEED_PARSE	    0x0001
#define PSIA_URI_FLAG_VIRTUAL_PAGE  	0x0002

typedef struct __PSIA_URI
{
	char *name;
	int (*handler) (struct request *,struct node_t *);
	AUTHORITY authority;
	int psia_uri_flag;

	struct __PSIA_URI *next;

} PSIA_URI;

#endif

extern struct status status;

extern char *optarg;            /* For getopt */
extern FILE *yyin;              /* yacc input */

extern request *request_ready;  /* first in ready list */
extern request *request_block;  /* first in blocked list */
extern request *request_free;   /* first in free list */

extern fd_set block_read_fdset; /* fds blocked on read */
extern fd_set block_write_fdset; /* fds blocked on write */

/* global server variables */

extern char *access_log_name;
extern char *error_log_name;
extern char *cgi_log_name;
extern int cgi_log_fd;
extern int use_localtime;

extern int server_port;
extern uid_t server_uid;
extern gid_t server_gid;
extern char *server_admin;
extern char *server_root;
extern char *server_name;
extern char *server_ip;
extern int max_fd;
extern int devnullfd;

extern char *document_root;
extern char *user_dir;
extern char *directory_index;
extern char *default_type;
extern char *dirmaker;
extern char *mime_types;
extern char *cachedir;

extern char *tempdir;

extern char *cgi_path;
extern int single_post_limit;

extern int ka_timeout;
extern int ka_max;

extern int sighup_flag;
extern int sigchld_flag;
extern int sigalrm_flag;
extern int sigterm_flag;
extern time_t start_time;

extern int pending_requests;
extern long int max_connections;

extern int verbose_cgi_logs;

extern int backlog;
extern time_t current_time;

extern int virtualhost;

extern int total_connections;

extern sigjmp_buf env;
extern int handle_sigbus;

#endif
