/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996 Charles F. Randall <crandall@goldsys.com>
 *  Some changes Copyright (C) 1996 Larry Doolittle <ldoolitt@boa.org>
 *  Some changes Copyright (C) 1996-2002 Jon Nelson <jnelson@boa.org>
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

/* $Id: boa.c,v 1.99.2.2 2002/07/23 15:50:29 jnelson Exp $*/

#include "boa.h"
#include <sys/resource.h>
#ifdef SERVER_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif /*SERVER_SSL*/
#ifdef DAVINCI_IPCAM
#include "ApproDrvMsg.h"
#include "Appro_interface.h"

#include "Msg_Def.h"
#include "appro_api.h"
#include <web_translate.h>
#include <file_msg_drv.h>
#include <share_mem.h>
#include <sys_msg_drv.h>
#include <sysctrl.h>
#endif

#ifdef SERVER_SSL
#define SSL_KEYF "/etc/config/ssl_key.pem"
#define SSL_CERTF "/etc/config/ssl_cert.pem"
	int server_ssl;				/*ssl socket */
	int do_ssl = 1;					/*We want to actually perform all of the ssl stuff.*/
	int do_sock = 1;				/*We may not want to actually connect to normal sockets*/
	SSL_CTX *ctx;				/*SSL context information*/
	SSL_METHOD *meth;			/*SSL method information*/
	int ssl_server_port = 443;		/*The port that the server should listen on*/
	/*Note that per socket ssl information is stored in */
#ifdef INET6
	struct sockaddr_in6 server_sockaddr;		/* boa ssl socket address */
#else
	struct sockaddr_in server_sockaddr;		/* boa ssl socket address */
#endif/*INET6*/
extern int InitSSLStuff(void);
extern void get_ssl_request(void);
#endif /*SERVER_SSL*/


/* globals */
int backlog = SO_MAXCONN;
time_t start_time;

int sighup_flag = 0;            /* 1 => signal has happened, needs attention */
int sigchld_flag = 0;           /* 1 => signal has happened, needs attention */
int sigalrm_flag = 0;           /* 1 => signal has happened, needs attention */
int sigterm_flag = 0;           /* lame duck mode */
time_t current_time;
int max_fd = 0;
int pending_requests = 0;

/* static to boa.c */
static void fixup_server_root(void);
static int create_server_socket(void);
static void drop_privs(void);

static int sock_opt = 1;
static int do_fork = 0;
int devnullfd = -1;

int main(int argc, char **argv)
{
    int c;                      /* command line arg */
    int server_s;                   /* boa socket */

    /* set umask to u+rw, u-x, go-rwx */
    c = umask(~0600);
    if (c == -1) {
        perror("umask");
        exit(1);
    }

    devnullfd = open("/dev/null", 0);

    /* make STDIN and STDOUT point to /dev/null */
    if (devnullfd == -1) {
        DIE("can't open /dev/null");
    }

    if (dup2(devnullfd, STDIN_FILENO) == -1) {
        DIE("can't dup2 /dev/null to STDIN_FILENO");
    }

    if (dup2(devnullfd, STDOUT_FILENO) == -1) {
        DIE("can't dup2 /dev/null to STDOUT_FILENO");
    }

    /* but first, update timestamp, because log_error_time uses it */
    (void) time(&current_time);

    while ((c = getopt(argc, argv, "c:r:d")) != -1) {
        switch (c) {
        case 'c':
            if (server_root)
                free(server_root);
            server_root = strdup(optarg);
            if (!server_root) {
                perror("strdup (for server_root)");
                exit(1);
            }
            break;
        case 'r':
            if (chdir(optarg) == -1) {
                log_error_time();
                perror("chdir (to chroot)");
                exit(1);
            }
            if (chroot(optarg) == -1) {
                log_error_time();
                perror("chroot");
                exit(1);
            }
            if (chdir("/") == -1) {
                log_error_time();
                perror("chdir (after chroot)");
                exit(1);
            }
            break;
        case 'd':
            do_fork = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-c serverroot] [-r chroot] [-d] [-b] [-q bitrate]\n", argv[0]);
            exit(1);
        }
    }

#ifdef DAVINCI_IPCAM
	if(ApproDrvInit(BOA_MSG_TYPE) < 0){
		exit(1);
	}
    if (func_get_mem(NULL)) {
        ApproDrvExit();
        exit(1);
    }

	if(SysDrvInit(SYS_BOA_MSG) < 0){
        ApproDrvExit();
        exit(1);
	}

    if (InitFileMsgDrv(FILE_MSG_KEY, FILE_BOA_MSG) < 0) {
	ApproDrvExit();
	SysDrvExit();
	exit(1);
    }

    if (hash_table_init() < 0) {
        exit(1);
    }

    if(arg_hash_table_init() < 0) {
        exit(1);
    }

    if(uri_hash_table_init() < 0) {
        exit(1);
    }
	
#endif

#ifdef PSIA_MEDIA
    if(psia_uri_hash_table_init() < 0) {
        exit(1);
    }
#endif

    fixup_server_root();
    read_config_files();
#ifdef DAVINCI_IPCAM
    {
        unsigned short value;
        if (ControlSystemData(SFIELD_GET_HTTPPORT, (void *)&value, sizeof(value)) >= 0)
            server_port = value;
        dbg("server_port=%d\n", server_port);
    }
#endif
    open_logs();
    server_s = create_server_socket();
    init_signals();
    drop_privs();
    create_common_env();
    build_needs_escape();

    if (max_connections < 1) {
        struct rlimit rl;

        /* has not been set explicitly */
        c = getrlimit(RLIMIT_NOFILE, &rl);
        if (c < 0) {
            perror("getrlimit");
            exit(1);
        }
        max_connections = rl.rlim_cur;
    }

    /* background ourself */
    if (do_fork) {
        switch(fork()) {
        case -1:
            /* error */
            perror("fork");
            exit(1);
            break;
        case 0:
            /* child, success */
            break;
        default:
            /* parent, success */
            exit(0);
            break;
        }
    }

	/* Ask system server to start upnp */
	system("/opt/ipnc/boot_proc 2\n");

    /* main loop */
    timestamp();

    status.requests = 0;
    status.errors = 0;

    start_time = current_time;
    select_loop(server_s);
    return 0;
}

static int create_server_socket(void)
{
    int server_s;

#ifdef SERVER_SSL
	if (do_ssl) {
		if (InitSSLStuff() != 1) {
			/*TO DO - emit warning the SSL stuff will not work*/
//			syslog(LOG_ALERT, "Failure initialising SSL support - ");
			if (do_sock == 0) {
//				syslog(LOG_ALERT, "    normal sockets disabled, so exiting");fflush(NULL);
				return 0;
			} else {
//				syslog(LOG_ALERT, "    supporting normal (unencrypted) sockets only");fflush(NULL);
				do_sock = 2;
			}
	  }
	} else
		do_sock = 2;
#endif /*SERVER_SSL*/

#ifdef SERVER_SSL
	if(do_sock){
#endif /*SERVER_SSL*/
    server_s = socket(SERVER_AF, SOCK_STREAM, IPPROTO_TCP);
    if (server_s == -1) {
        DIE("unable to create socket");
    }

    /* server socket is nonblocking */
    if (set_nonblock_fd(server_s) == -1) {
        DIE("fcntl: unable to set server socket to nonblocking");
    }

    /* close server socket on exec so cgi's can't write to it */
    if (fcntl(server_s, F_SETFD, 1) == -1) {
        DIE("can't set close-on-exec on server socket!");
    }

    /* reuse socket addr */
    if ((setsockopt(server_s, SOL_SOCKET, SO_REUSEADDR, (void *) &sock_opt,
                    sizeof (sock_opt))) == -1) {
        DIE("setsockopt");
    }

    /* internet family-specific code encapsulated in bind_server()  */
    if (bind_server(server_s, server_ip) == -1) {
        DIE("unable to bind");
    }

    /* listen: large number just in case your kernel is nicely tweaked */
    if (listen(server_s, backlog) == -1) {
        DIE("unable to listen");
    }
    return server_s;
#ifdef SERVER_SSL
	}
	else
		return -1;
#endif /*SERVER_SSL*/
}

static void drop_privs(void)
{
    /* give away our privs if we can */
    if (getuid() == 0) {
        struct passwd *passwdbuf;
        passwdbuf = getpwuid(server_uid);
        if (passwdbuf == NULL) {
            DIE("getpwuid");
        }
        if (initgroups(passwdbuf->pw_name, passwdbuf->pw_gid) == -1) {
            DIE("initgroups");
        }
        if (setgid(server_gid) == -1) {
            DIE("setgid");
        }
        if (setuid(server_uid) == -1) {
            DIE("setuid");
        }
        /* test for failed-but-return-was-successful setuid
         * http://www.securityportal.com/list-archive/bugtraq/2000/Jun/0101.html
         */
/*        if (setuid(0) != -1) {
            DIE("icky Linux kernel bug!");
        }*/
    } else {
        if (server_gid || server_uid) {
            log_error_time();
            fprintf(stderr, "Warning: "
                    "Not running as root: no attempt to change"
                    " to uid %d gid %d\n", server_uid, server_gid);
        }
        server_gid = getgid();
        server_uid = getuid();
    }
}

/*
 * Name: fixup_server_root
 *
 * Description: Makes sure the server root is valid.
 *
 */

static void fixup_server_root()
{
    char *dirbuf;

    if (!server_root) {
#ifdef SERVER_ROOT
        server_root = strdup(SERVER_ROOT);
        if (!server_root) {
            perror("strdup (SERVER_ROOT)");
            exit(1);
        }
#else
        fputs("boa: don't know where server root is.  Please #define "
              "SERVER_ROOT in boa.h\n"
              "and recompile, or use the -c command line option to "
              "specify it.\n", stderr);
        exit(1);
#endif
    }

    if (chdir(server_root) == -1) {
        fprintf(stderr, "Could not chdir to \"%s\": aborting\n",
                server_root);
        exit(1);
    }

    dirbuf = normalize_path(server_root);
    free(server_root);
    server_root = dirbuf;
}

#ifdef SERVER_SSL
int InitSSLStuff(void)
{
	dbg("Enabling SSL security system\n");
#ifdef INET6
	if ((server_ssl = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		DIE("NO_CREATE_SOCKET");
		return 0;
	}
#else
	if ((server_ssl = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
		dbg("Couldn't create socket for ssl\n");
		DIE("NO_CREATE_SOCKET");
		return 0;
	}
#endif /*INET6*/

	/* server socket is nonblocking */
	if (fcntl(server_ssl, F_SETFL, NOBLOCK) == -1){
		dbg("%s, %i:Couldn't fcntl\n", __FILE__, __LINE__);
		DIE("NO_FCNTL");
		return 0;
	}

	if ((setsockopt(server_ssl, SOL_SOCKET, SO_REUSEADDR, (void *) &sock_opt,
		sizeof(sock_opt))) == -1){
		dbg("%s, %i:Couldn't sockopt\n", __FILE__,__LINE__);
		DIE("NO_SETSOCKOPT");
		return 0;
	}

	/* internet socket */
#ifdef INET6
	server_sockaddr.sin6_family = AF_INET6;
	memcpy(&server_sockaddr.sin6_addr,&in6addr_any,sizeof(in6addr_any));
	server_sockaddr.sin6_port = htons(ssl_server_port);
#else
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_sockaddr.sin_port = htons(ssl_server_port);
#endif

	if (bind(server_ssl, (struct sockaddr *) &server_sockaddr,
		sizeof(server_sockaddr)) == -1){
		dbg("Couldn't bind ssl to port %d\n", ntohs(server_sockaddr.sin_port));
		DIE("NO_BIND");
		return 0;
	}

	/* listen: large number just in case your kernel is nicely tweaked */
	if (listen(server_ssl, backlog) == -1){
		DIE("NO_LISTEN");
		return 0;
	}

	if (server_ssl > max_fd)
		max_fd = server_ssl;

	/*Init all of the ssl stuff*/
//	i don't know why this line is commented out... i found it like that - damion may-02
/*	SSL_load_error_strings();*/
	SSLeay_add_ssl_algorithms();
	meth = SSLv23_server_method();
	if(meth == NULL){
		ERR_print_errors_fp(stderr);
		dbg("Couldn't create the SSL method\n");
		DIE("NO_SSL");
		return 0;
	}
	ctx = SSL_CTX_new(meth);
	if(!ctx){
		dbg("Couldn't create a connection context\n");
		ERR_print_errors_fp(stderr);
		DIE("NO_SSL");
		return 0;
	}

	if (SSL_CTX_use_certificate_file(ctx, SSL_CERTF, SSL_FILETYPE_PEM) <= 0) {
		dbg("Failure reading SSL certificate file: %s\n",SSL_CERTF);
		close(server_ssl);
		return 0;
	}
	dbg("Loaded SSL certificate file: %s\n",SSL_CERTF);

	if (SSL_CTX_use_PrivateKey_file(ctx, SSL_KEYF, SSL_FILETYPE_PEM) <= 0) {
		dbg("Failure reading private key file: %s\n",SSL_KEYF);
		close(server_ssl);
		return 0;
	}
	dbg("Opened private key file: %s\n",SSL_KEYF);

	if (!SSL_CTX_check_private_key(ctx)) {
		dbg("Private key does not match the certificate public key\n");
		close(server_ssl);
		return 0;
	}

	/*load and check that the key files are appropriate.*/
	dbg("SSL security system enabled\n");
	return 1;
}
#endif /*SERVER_SSL*/

