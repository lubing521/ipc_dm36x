/**
 * \file smtp.c
 * Send a message via libESMTP.
 *
 * \author Adapted from the libESMTP's mail-file example by Jos� Fonseca.
 */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

#include <auth-client.h>
#include <libesmtp.h>

#include "smtp.h"
#include "main.h"
#include "xmalloc.h"

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 256
#endif


/**
 * \name Identity management
 */
/*@{*/

identity_t *default_identity = NULL;

static LIST_HEAD(identities);

identity_t *identity_new(void)
{
	identity_t *identity;

	identity = (identity_t *)xmalloc(sizeof(identity_t));

	memset(identity, 0, sizeof(identity_t));

	identity->starttls = Starttls_DISABLED;
		
	return identity;
}

void identity_free(identity_t *identity)
{
	if(identity->address)
		free(identity->address);

	if(identity->host)
		free(identity->host);

	if(identity->user)
		free(identity->user);

	if(identity->pass)
		free(identity->pass);

	if(identity->certificate_passphrase)
		free(identity->certificate_passphrase);

	if(identity->preconnect)
		free(identity->preconnect);

	if(identity->postconnect)
		free(identity->postconnect);

	if(identity->qualifydomain)
		free(identity->qualifydomain);

	if(identity->helo)
		free(identity->helo);

	if(identity->force_reverse_path)
		free(identity->force_reverse_path);

	if(identity->force_sender)
		free(identity->force_sender);

	free(identity);
}

void identity_add(identity_t *identity)
{
	list_add(&identity->list, &identities);
}

identity_t *identity_lookup(const char *address)
{
	if(address)
	{
		struct list_head *ptr;

		list_for_each(ptr, &identities)
		{
			identity_t *identity;
			
			identity = list_entry(ptr, identity_t, list);
			if(identity->address && !strcmp(identity->address, address))
				return identity;
		}
	}

	return default_identity;
}
	
void identities_init(void)
{
}

void identities_cleanup(void)
{
	default_identity = NULL;

	if(!list_empty(&identities))
	{
		struct list_head *ptr, *tmp;
	
		list_for_each_safe(ptr, tmp, &identities)
		{
			identity_t *identity;
			
			identity = list_entry(ptr, identity_t, list);
			list_del(ptr);
			
			identity_free(identity);
		}

	}
}
	
/*@}*/


/**
 * \name libESMTP interface
 */
/*@{*/

/**
 * Callback function to read the message from a file.  
 *
 * Since libESMTP does not provide callbacks which translate line endings, one
 * must be provided by the application.
 */
static const char * message_cb (void **buf, int *len, void *arg)
{
	message_t *message = (message_t *)arg;

	if (len == NULL)
	{
		/* only allow rewinding in the beginning of a message otherwise
		 * it will break the pipes */
		assert(*buf == NULL);
		return NULL;
	}

	if (*buf == NULL)
		*buf = malloc (BUFSIZ);

	*len = message_read(message, *buf, BUFSIZ);
	
	return *buf;
}

#define SIZETICKER 1024		/**< print 1 dot per this many bytes */

static void event_cb (smtp_session_t session, int event_no, void *arg, ...)
{
	va_list ap;

	va_start (ap, arg);
		
	switch (event_no) {
		case SMTP_EV_EXTNA_DSN:
			fprintf(stderr, "Delivery Status Notification extension not supported by MTA\n");
			break;

		case SMTP_EV_EXTNA_8BITMIME:
			fprintf(stderr, "8bit-MIME extension not supported by MTA\n");
			break;
			
		case SMTP_EV_EXTNA_STARTTLS:
			fprintf(stderr, "StartTLS extension not supported by MTA\n");
			break;

		case SMTP_EV_WEAK_CIPHER:
		{
			int bits = va_arg (ap, int);
			int *ok = va_arg (ap, int *);

			fprintf(stderr, "Weak cipher (%d bits)\n", bits);

			*ok = 0;
			break;
		}

		case SMTP_EV_INVALID_PEER_CERTIFICATE:
		{
			long result = va_arg (ap, long);
			int *ok = va_arg (ap, int *);

			fprintf(stderr, "Invalid peer certificate (error %ld)\n", result);

			*ok = 0;
			break;
		}

		case SMTP_EV_NO_PEER_CERTIFICATE:
		{
			int *ok = va_arg (ap, int *);
			
			fprintf(stderr, "No peer certificate\n");

			*ok = 0;
			break;
		}

		case SMTP_EV_WRONG_PEER_CERTIFICATE:
		{
			int *ok = va_arg (ap, int *);

			fprintf(stderr, "Wrong peer certificate\n");

			*ok = 0;
			break;
		}
	}
	
	if (verbose)
	{
		static int sizeticking = 0, sizeticker;

		if (event_no != SMTP_EV_MESSAGEDATA && sizeticking)
		{
			fputs("\n", stdout);
			sizeticking = 0;
		}

		switch (event_no) {
			case SMTP_EV_CONNECT:
				fputs("Connected to MTA\n", stdout);
				break;
				
			case SMTP_EV_MAILSTATUS:
			{
				const char *mailbox = va_arg (ap, const char *);
				smtp_message_t message = message = va_arg (ap, smtp_message_t);
				const smtp_status_t *status = smtp_reverse_path_status (message);
				
				fprintf (stdout, "From %s: %d %s", mailbox, status->code, status->text);
				break;
			}

			case SMTP_EV_RCPTSTATUS:
			{
				const char *mailbox = va_arg (ap, const char *);
				smtp_recipient_t recipient = va_arg (ap, smtp_recipient_t);
				const smtp_status_t *status = smtp_recipient_status (recipient);
				
				fprintf (stdout, "To %s: %d %s", mailbox, status->code, status->text);
				break;
			}
				
			case SMTP_EV_MESSAGEDATA:
			{
				smtp_message_t message = message = va_arg (ap, smtp_message_t);
				if (!sizeticking)
				{
					fputs("Message data: ", stdout);
					sizeticking = 1;
					sizeticker = SIZETICKER - 1;
				}
				sizeticker += va_arg (ap, int);
				while (sizeticker >= SIZETICKER)
				{
					fputc('.', stdout);
					sizeticker -= SIZETICKER;
				}
				break;
			}

			case SMTP_EV_MESSAGESENT:
			{
				smtp_message_t message = va_arg (ap, smtp_message_t);
				const smtp_status_t *status = smtp_message_transfer_status (message);
				
				fprintf (stdout, "Message sent: %d %s", status->code, status->text);
				break;
			}
				
			case SMTP_EV_DISCONNECT:
				fputs("Disconnected to MTA\n", stdout);
				break;

			case SMTP_EV_STARTTLS_OK:
			{
				void *ssl = va_arg(ap, void *);
				void *cipher = va_arg(ap, void *);
				int bits = va_arg(ap, int);
				
				(void) ssl;
				(void) cipher;

				fprintf(stdout, "StartTLS OK (%d bits)\n", bits);
				break;
			}

		}
	}
		
	va_end (ap);
}

static void monitor_cb (const char *buf, int buflen, int writing, void *arg)
{
	assert(log_fp);

	if (writing == SMTP_CB_HEADERS)
	{
		fputs ("H: ", log_fp);
		fwrite (buf, 1, buflen, log_fp);
		return;
	}
	
	fputs (writing ? "C: " : "S: ", log_fp);
	fwrite (buf, 1, buflen, log_fp);
	if (buf[buflen - 1] != '\n')
		putc ('\n', log_fp);
}

/**
 * Callback to request user/password info.  
 *
 * Not thread safe. 
 */
static int authinteract (auth_client_request_t request, char **result, int fields,
				  void *arg)
{
	identity_t *identity = (identity_t *)arg;
	int i;

	if(!identity)
		return 0;
	
	for (i = 0; i < fields; i++)
	{
		if (request[i].flags & AUTH_USER && identity->user)
			result[i] = identity->user;
		else if (request[i].flags & AUTH_PASS && identity->pass)
			result[i] = identity->pass;
		else if (request[i].flags & AUTH_REALM)
			/* Just supply an empty string for the domain */
			result[i] = identity->user + strlen(identity->user);
		else
			return 0;
	}
	return 1;
}

static int tlsinteract (char *buf, int buflen, int rwflag, void *arg)
{
	identity_t *identity = (identity_t *)arg;
	char *pw;
	int len;

	if(!identity)
		return 0;
	
	if (identity->certificate_passphrase)
	{
		pw = identity->certificate_passphrase;
		len = strlen (pw);
		if (len + 1 > buflen)
			return 0;
		strcpy (buf, pw);
		return len;
	}
	else
		return 0;
}

/**
 * Callback to print the recipient status.
 */
void print_recipient_status (smtp_recipient_t recipient, const char *mailbox, 
							 void *arg)
{
	const smtp_status_t *status;

	status = smtp_recipient_status (recipient);
	fprintf (stderr, "%s: %d %s\n", mailbox, status->code, status->text);
}

/**
 * Get username.
 * 
 * NOTE: Return value may point to a static area, which may be overriden to 
 * subsequent calls to getpwent(), getpwnam(), or getpwuid().
 */
static char *get_username(void)
{
	uid_t uid;
	struct passwd *user;

	uid = getuid();
	user = getpwuid(uid);
	
	if (!user)
	{
		fprintf (stderr, "Could not determine the username of uid %d!\n", (int)uid);
		exit (EX_NOUSERNAME);
	}

	return user->pw_name;
}

/**
 * Escape a forced fields: %u by username %% by %
 */
static char *escape_forced_address (char *mask)
{
	char *username;
	char *escaped, *e;
	const char *p;
	int len;

	username = get_username();
	
	len = 0;
	for (p=mask ; *p ; p++)
	{
		if ( *p == '%' )
		{
			p++;
			if( !*p )
				break;
			switch( *p ) {
				case '%':
					len++;
					break;
				case 'u':
					len += strlen(username);
					break;
			}
		}
		else
			len++;
	}

	e = escaped = xmalloc(len+1);
	for (p=mask ; *p ; p++)
	{
		if (*p == '%')
		{
			p++;
			if (!*p)
				break;
			if (*p == '%')
				*(e++) = *p;
			else if (*p == 'u')
			{
				strcpy(e,username);
				e += strlen(username);
			}
		}
		else
		{
			*(e++) = *p;
			continue;
		}
	}
	*e = '\0';
	return escaped;
	
}

void smtp_send(message_t *msg, identity_t *identity)
{
	smtp_session_t session;
	smtp_message_t message;
	smtp_recipient_t recipient;
	auth_context_t authctx;
	const smtp_status_t *status;
	struct sigaction sa;
	struct list_head *ptr;

	/* This program sends only one message at a time.  Create an SMTP
	 * session.
	 */
	auth_client_init ();
	if(!(session = smtp_create_session ()))
		goto failure;

	/* Add a protocol monitor. */
	if(log_fp)
		if(!smtp_set_monitorcb (session, monitor_cb, NULL, 1))
			goto failure;

	/* Set the event callback. */
	if(!smtp_set_eventcb (session, event_cb, NULL))
		goto failure;

	/* NB.  libESMTP sets timeouts as it progresses through the protocol.  In
	 * addition the remote server might close its socket on a timeout.
	 * Consequently libESMTP may sometimes try to write to a socket with no
	 * reader.  Ignore SIGPIPE, then the program doesn't get killed if/when
	 * this happens.
	 */
	sa.sa_handler = SIG_IGN; sigemptyset (&sa.sa_mask); sa.sa_flags = 0;
	sigaction (SIGPIPE, &sa, NULL);

	/* Set the hostname of this computer to be used for HELO names: */
	if(identity->helo)
	{
		if(!smtp_set_hostname (session, identity->helo))
			goto failure;
	}

	/* Set the host running the SMTP server.  LibESMTP has a default port
	 * number of 587, however this is not widely deployed so the port is
	 * specified as 25 along with the default MTA host.
	 */
	if(!smtp_set_server (session, identity->host ? identity->host : "localhost:25"))
		goto failure;

	/* Set the SMTP Starttls extension. */
	if(identity->starttls && !smtp_starttls_enable (session, identity->starttls))
		goto failure;

	/* Do what's needed at application level to use authentication. */
	if(identity->user || identity->pass)
	{
		authctx = auth_create_context ();
		auth_set_mechanism_flags (authctx, AUTH_PLUGIN_PLAIN, 0);
		auth_set_interact_cb (authctx, authinteract, identity);
	}
	else
		authctx = NULL;

	/* Use our callback for X.509 certificate passwords.  If STARTTLS is not in
	 * use or disabled in configure, the following is harmless.
	 */
	if(identity->starttls && !smtp_starttls_set_password_cb (tlsinteract, identity))
		goto failure;

	/* Now tell libESMTP it can use the SMTP AUTH extension. */
	if(!smtp_auth_set_context (session, authctx))
		goto failure;

	/* At present it can't handle one recipient only out of many failing.  Make
	 * libESMTP require all specified recipients to succeed before transferring
	 * a message.
	 */
	if(!smtp_option_require_all_recipients (session, 1))
		goto failure;

	/* Add a message to the SMTP session. */
	if(!(message = smtp_add_message (session)))
		goto failure;

	/* Set the reverse path for the mail envelope. */
	if(identity->force_reverse_path)
	{
		/* Use forced reverse path. */
		char *value;

		value = escape_forced_address(identity->force_reverse_path);
		if(!smtp_set_reverse_path (message, value))
		{
			free(value);
			goto failure;
		}
		free(value);
		/* Allow -f to set an default From: address though */
		if(msg->reverse_path)
		{
			if(!smtp_set_header (message, "From", NULL, msg->reverse_path))
				goto failure;
		}
	}
	else if(msg->reverse_path)
	{
		/* Use reverse path specified at command line. */
		if(!smtp_set_reverse_path (message, msg->reverse_path))
			goto failure;
	}
	else if(identity->address)
	{
		/* Use the identity address as reverse path. */
		if(!smtp_set_reverse_path (message, identity->address))
			goto failure;
	}
	else
	{
		/* Use the "username@hostname" default. */
		char *user;
		char host[MAXHOSTNAMELEN];
		int user_len;
		int host_len;
		char *reverse_path;
		char *p;

		user = get_username();
		user_len = strlen(user);
		if(gethostname(host, sizeof(host)))
			strcpy(host, "localhost");
		host_len = strlen(host);
		
		p = reverse_path = xmalloc(user_len + 1 + host_len + 1);
		memcpy(p, user, user_len);
		p += user_len;
		*p++ = '@';
		memcpy(p, host, host_len);
		p += host_len;
		*p = '\0';
		
		if(!smtp_set_reverse_path (message, reverse_path))
			goto failure;
		free(reverse_path);
	}

	/* Open the message file and set the callback to read it. */
	if(!smtp_set_messagecb (message, message_cb, msg))
		goto failure;

	/* Overwrite Sender:-Header if force sender is specified */
	if(identity->force_sender)
	{
		char *value;

		value = escape_forced_address(identity->force_sender);
		if(!smtp_set_header (message, "Sender", NULL, value))
		{
			free(value);
			goto failure;
		}
		free(value);
		if(!smtp_set_header_option (message, "Sender", Hdr_OVERRIDE, (int)1))
			goto failure;
	}

	/* Prohibit Message-ID:-Header if force_msgid is not specified */
	if(identity->prohibit_msgid)
		if(!smtp_set_header_option(message, "Message-ID", Hdr_PROHIBIT, (int)1))
			goto failure;

	/* DSN options */
	if(!smtp_dsn_set_ret(message, msg->ret))
		goto failure;
	if(msg->envid)
		if(!smtp_dsn_set_envid(message, msg->envid))
			goto failure;
	
	/* 8bit-MIME */
	if(!smtp_8bitmime_set_body(message, msg->body))
		goto failure;

	/* Add remote message recipients. */
	list_for_each(ptr, &msg->remote_recipients)
	{
		recipient_t *entry = list_entry(ptr, recipient_t, list);
		
		assert(entry->address);

		if(!(recipient = smtp_add_recipient (message, entry->address)))
			goto failure;
		
		/* Recipient options set here */
		if (msg->notify != Notify_NOTSET)
			if(!smtp_dsn_set_notify (recipient, msg->notify))
				goto failure;
	}

	/* Add local message recipients if qualifydomain is set */
	if (identity->qualifydomain) list_for_each(ptr, &msg->local_recipients)
	{
		recipient_t *entry = list_entry(ptr, recipient_t, list);
		char *qualifiedaddress;

		assert(entry->address);

		qualifiedaddress = xmalloc(strlen(identity->qualifydomain) + strlen(entry->address) + 2);
		strcpy(qualifiedaddress, entry->address);
		strcat(qualifiedaddress, "@");
		strcat(qualifiedaddress, identity->qualifydomain);

		if(!(recipient = smtp_add_recipient (message, qualifiedaddress)))
			goto failure;
		free(qualifiedaddress);

		/* Recipient options set here */
		if (msg->notify != Notify_NOTSET)
			if(!smtp_dsn_set_notify (recipient, msg->notify))
				goto failure;
	}

	/* Execute pre-connect command if one was specified. */
	if (identity->preconnect)
	{
		int ret, exit_status;

		if (verbose)
			fprintf (stdout, "Executing pre-connect command: %s\n", identity->preconnect);

		ret = system (identity->preconnect);
		exit_status = WEXITSTATUS(ret);

		/* Check whether the child process caught a signal meant for us */
		if (WIFSIGNALED(ret))
		{
			int sig = WTERMSIG(ret);

			if (sig == SIGINT || sig == SIGQUIT)
			{
				fprintf (stderr, "Pre-connect command received signal %d\n", sig);
				exit (EX_SOFTWARE);
			}
		}

		if (ret == -1)
		{
			fputs ("Error executing pre-connect command\n", stderr);
			exit (EX_OSERR);
		}

		if (exit_status != 0)
		{
			fprintf (stderr, "Pre-connect command \"%s\" exited with non-zero status %d\n",
				 identity->preconnect, exit_status);
			exit (EX_SOFTWARE);
		}
	}

	/* Initiate a connection to the SMTP server and transfer the message. */
	if (!smtp_start_session (session))
	{
		char buf[128];

		fprintf (stderr, "SMTP server problem %s\n",
				 smtp_strerror (smtp_errno (), buf, sizeof(buf)));

		exit(EX_UNAVAILABLE);
	}


	/* Report on the success or otherwise of the mail transfer. */
	if(!(status = smtp_message_transfer_status (message)))
		goto failure;
	if (status->code / 100 != 2)
	{
		/* Report on the failure of the mail transfer. */
		status = smtp_message_transfer_status (message);
		fprintf (stderr, "%d %s\n", status->code, status->text);
		smtp_enumerate_recipients (message, print_recipient_status, NULL);

		exit(EX_SOFTWARE);
	}

	if (log_fp)
		fputc('\n', log_fp);

	smtp_destroy_session (session);
	if(authctx)
		auth_destroy_context (authctx);
	auth_client_exit ();

	/* Execute post-connect command if one was specified. */
	if (identity->postconnect)
	{
		int ret, exit_status;

		if (verbose)
			fprintf (stdout, "Executing post-connect command: %s\n", identity->postconnect);

		ret = system (identity->postconnect);
		exit_status = WEXITSTATUS(ret);

		/* Check whether the child process caught a signal meant for us */
		if (WIFSIGNALED(ret))
		{
			int sig = WTERMSIG(ret);

			if (sig == SIGINT || sig == SIGQUIT)
			{
				fprintf (stderr, "Post-connect command received signal %d\n", sig);
				exit (EX_SOFTWARE);
			}
		}

		if (ret == -1)
		{
			fputs ("Error executing post-connect command\n", stderr);
			exit (EX_OSERR);
		}

		if (exit_status != 0)
		{
			fprintf (stderr, "Post-connect command \"%s\" exited with non-zero status %d\n",
				 identity->postconnect, exit_status);
			exit (EX_SOFTWARE);
		}
	}

	return;

failure:
	{
		char buf[128];

		fprintf (stderr, "%s\n",
				 smtp_strerror (smtp_errno (), buf, sizeof(buf)));

		exit(EX_SOFTWARE);
	}
}

/*@}*/
