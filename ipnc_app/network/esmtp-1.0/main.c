/**
 * \file main.c
 * Main program.
 */


#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include "main.h"
#include "message.h"
#include "smtp.h"
#include "local.h"
#include "rcfile.h"


static void drop_sgids( void )
{
	gid_t real_gid, effective_gid;

	real_gid = getgid();
	effective_gid = getegid();

	if( setregid(real_gid,real_gid) != 0 )
	{
		fprintf (stderr, "Could not drop setgid: %s\n", strerror(errno));
		exit(EX_DROPPERM);
	}
}


/** Modes of operation. */
typedef enum {
	ENQUEUE,		/**< delivery mode */
	NEWALIAS,		/**< initialize alias database */
	MAILQ,			/**< list mail queue */
	FLUSHQ			/**< flush the mail queue */
} opmode_t;


int verbose = 0;

FILE *log_fp = NULL;

static void message_send(message_t *message)
{
	int local, remote;
	identity_t *identity;

	/* Lookup the identity already here */
	identity = identity_lookup(message->reverse_path); 
	assert(identity);
	
	if( identity->qualifydomain )
	{
		local = 0;
		remote = 1;
	}
	else
	{
		local = !list_empty(&message->local_recipients);
		remote = !list_empty(&message->remote_recipients);
	}
	
	if(remote && !local)
		smtp_send(message,identity);
	else if(!remote && local)
	{
		local_init(message);
		local_flush(message);
	}
	else
	{
		local_init(message);
		smtp_send(message,identity);
		local_flush(message);
	}
	
	local_cleanup();
}

int main (int argc, char **argv)
{
	int c;
	message_t *message;
	int parse_headers = 0;
	opmode_t mode;
	char *rcfile = NULL;
	
	message = message_new();

	/* Set the default mode of operation. */
	if (!strcmp(argv[0], "mailq")) {
		mode = MAILQ;
	} else if (!strcmp(argv[0], "newaliases")) {
		mode = NEWALIAS;
	} else {
		mode = ENQUEUE;
	}

	while ((c = getopt (argc, argv, "A:B:b:C:cd:e:F:f:Gh:IiL:M:mN:nO:o:p:q:R:r:sTtV:vX:")) != EOF)
		switch (c)
		{
			case 'A':
				/* Use alternate sendmail/submit.cf */
				break;

			case 'B':
				/* Body type */
				if (!strcmp (optarg, "7BIT"))
					message->body = E8bitmime_7BIT;
				else if (!strcmp (optarg, "8BITMIME"))
					message->body = E8bitmime_8BITMIME;
				else
				{
					fprintf (stderr, "Unsupported body type %s\n", optarg);
					exit (EX_USAGE);
				}
				break;

			case 'C':
				/* Select configuration file */
				rcfile = optarg;
				break;

			case 'F':
				/* Set full name */
				break;

			case 'G':
				/* Relay (gateway) submission */
				break;

			case 'I':
				/* Initialize alias database */
				mode = NEWALIAS;
				break;

			case 'L':
				/* Program label */
				break;

			case 'M':
				/* Define macro */
				break;

			case 'N':
				/* Delivery status notifications */
				if (!strcmp (optarg, "never"))
					message->notify = Notify_NEVER;
				else
				{
					if (strstr (optarg, "failure"))
						message->notify |= Notify_FAILURE;
					if (strstr (optarg, "delay"))
						message->notify |= Notify_DELAY;
					if (strstr (optarg, "success"))
						message->notify |= Notify_SUCCESS;
				}
				break;

			case 'R':
				/* What to return */
				if (!strcmp (optarg, "full"))
					message->ret |= Ret_FULL;
				if (!strcmp (optarg, "hdrs"))
					message->ret |= Ret_HDRS;
				break;

			case 'T':
				/* Set timeout interval */
				break;

			case 'X':
				/* Traffic log file */
				if (log_fp)
					fclose(log_fp);
				log_fp = fopen(optarg, "a");
				break;

			case 'V':
				/* Set original envelope id */
				message_set_envid(message, optarg);

			case 'b':
				/* Operations mode */
				c = (optarg == NULL) ? ' ' : *optarg;
				switch (c)
				{
					case 'm':
						/* Deliver mail in the usual way */
						mode = ENQUEUE;
						break;

					case 'i':
						/* Initialize the alias database */
						mode = NEWALIAS;
						break;

					case 'p':
						/* Print a listing of the queue(s) */
						mode = MAILQ;
						break;
						
					case 'a':
						/* Go into ARPANET mode */
					case 'd':
						/* Run as a daemon */
					case 'D':
						/* Run as a daemon in foreground */
					case 'h':
						/* Print the persistent host status database */
					case 'H':
						/* Purge expired entries from the persistent host
						 * status database */
					case 'P':
						/* Print number of entries in the queue(s) */
					case 's':
						/* Use the SMTP protocol as described in RFC821
						 * on standard input and output */
					case 't':
						/* Run in address test mode */
					case 'v':
						/* Verify names only */
						fprintf (stderr, "Unsupported operation mode %c\n", c);
						exit (EX_USAGE);
						break;

					default:
						fprintf (stderr, "Invalid operation mode %c\n", c);
						exit (EX_USAGE);
						break;
				 }
				 break;

			case 'c':
				/* Connect to non-local mailers */
				break;

			case 'd':
				/* Debugging */
				break;

			case 'e':
				/* Error message disposition */
				break;

			case 'f':
				/* From address */
			case 'r':
				/* Obsolete -f flag */
				message_set_reverse_path(message, optarg);
				break;

			case 'h':
				/* Hop count */
				break;

			case 'i':
				/* Don't let dot stop me */
				break;

			case 'm':
				/* Send to me too */
				break;

			case 'n':
				/* don't alias */
				break;

			case 'o':
				/* Set option */
				break;

			case 'p':
				/* Set protocol */
				break;

			case 'q':
				/* Run queue files at intervals */
				mode = FLUSHQ;
				if (optarg[0] == '!')
				{
					/* Negate the meaning of pattern match */
					optarg++;
				}

				switch (optarg[0])
				{
					case 'G':
						/* Limit by queue group name */
						break;

					case 'I':
						/* Limit by ID */
						break;

					case 'R':
						/* Limit by recipient */
						break;

					case 'S':
						/* Limit by sender */
						break;

					case 'f':
						/* Foreground queue run */
						break;

					case 'p':
						/* Persistent queue */
						if (optarg[1] == '\0')
							break;
						++optarg;

					default:
						break;
				}
				break;

			case 's':
				/* Save From lines in headers */
				break;

			case 't':
				/* Read recipients from message */
				if(!message_parse_headers(message))
				{
					fprintf(stderr, "No recipients found\n");
					exit(EX_DATAERR);
				}
				parse_headers = 1;
				break;

			case 'v':
				/* Verbose */
				verbose = 1;
				break;

			default:
				fprintf (stderr, "Invalid option %c\n", c);
				exit (EX_USAGE);
			}

	switch (mode)
	{
		case ENQUEUE:
			break;
		
		case MAILQ:
			printf ("Mail queue is empty\n");
		case NEWALIAS:
		case FLUSHQ:
			goto done;
	}

	/* At least one more argument is needed. */
	if (optind > argc - 1 && !parse_headers)
	{
		fprintf(stderr, "Recipient names must be specified\n");
		exit (EX_USAGE);
	}

	/* Add remaining program arguments as message recipients. */
	while (optind < argc)
		message_add_recipient(message, argv[optind++]);

	identities_init();

	/* Parse the rc file. */
	rcfile_parse(rcfile);

	drop_sgids();

	message_send(message);
	
	identities_cleanup();

done:
	if(log_fp)
		fclose(log_fp);
	
	message_free(message);

	exit(EX_OK);
}
