/**
 * \file local.c
 * Local delivery of mail via a MDA.
 */


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#include "local.h"
#include "main.h"
#include "xmalloc.h"


char *mda = NULL;

FILE *mda_fp = NULL;


int local_address(const char *address)
{
	return !strchr(address, '@');
}

/** replace ' by _ */
static void sanitize(char *s)
{
	char *cp;

	for (cp = s; (cp = strchr (cp, '\'')); cp++)
			*cp = '_';
}


/**
 * Pipe the message to the MDA for local delivery.
 *
 * Based on fetchmail's open_mda_sink().
 */
void local_init(message_t *message)
{
	int		length = 0, fromlen = 0, nameslen = 0;
	char		*names = NULL, *before, *after, *from = NULL;

	if (!mda)
	{
		fprintf(stderr, "Local delivery not possible without a MDA\n");
		exit(EX_OSFILE);
	}

	length = strlen(mda);
	before = xstrdup(mda);

	/* get user addresses for %T */
	if (strstr(before, "%T"))
	{
		struct list_head *ptr;
		char *p;

		/*
		 * We go through this in order to be able to handle very
		 * long lists of users and (re)implement %s.
		 */
		nameslen = 0;
		list_for_each(ptr, &message->local_recipients)
		{
			recipient_t *recipient = list_entry(ptr, recipient_t, list);
			
			assert(recipient->address);
			
			nameslen += (strlen(recipient->address) + 3);	/* string + quotes + ' ' */
		}

		names = (char *)xmalloc(nameslen + 1);		/* account for '\0' */
		p = names;
		list_for_each(ptr, &message->local_recipients)
		{
			recipient_t *recipient = list_entry(ptr, recipient_t, list);
			int written;
			
			sanitize(recipient->address);
			written = sprintf(p, "'%s' ", recipient->address);
			if (written < 0)
			{
				perror(NULL);
				exit(EX_OSERR);
			}
			p += written;
		}
		names[--nameslen] = '\0';		/* chop trailing space */
	}

	/* get From address for %F */
	if (strstr(before, "%F"))
	{
		from = xstrdup(message->reverse_path ? message->reverse_path : "");

		sanitize(from);

		fromlen = strlen(from);
	}

	/* do we have to build an mda string? */
	if (names || from) 
	{				
		char		*sp, *dp;

		/* find length of resulting mda string */
		sp = before;
		while ((sp = strstr(sp, "%s"))) {
			length += nameslen;		/* subtract %s and add '' */
			sp += 2;
		}
		sp = before;
		while ((sp = strstr(sp, "%T"))) {
			length += nameslen;		/* subtract %T and add '' */
			sp += 2;
		}
		sp = before;
		while ((sp = strstr(sp, "%F"))) {
			length += fromlen;		/* subtract %F and add '' */
			sp += 2;
		}

		after = xmalloc(length + 1);

		/* copy mda source string to after, while expanding %[sTF] */
		for (dp = after, sp = before; (*dp = *sp); dp++, sp++) {
			if (sp[0] != '%')		continue;

			/* need to expand? BTW, no here overflow, because in
			** the worst case (end of string) sp[1] == '\0' */
			if (sp[1] == 'T') {
				strcpy(dp, names);
				dp += nameslen;
				sp++;		/* position sp over [sT] */
				dp--;		/* adjust dp */
			} else if (sp[1] == 'F') {
				*dp++ = '\'';
				strcpy(dp, from);
				dp += fromlen;
				*dp++ = '\'';
				sp++;		/* position sp over F */
				dp--;		/* adjust dp */
			}
		}

		if (names) {
			free(names);
			names = NULL;
		}
		if (from) {
			free(from);
			from = NULL;
		}

		free(before);

		before = after;
	}


	if(!(mda_fp = popen(before, "w")))
	{
		fprintf(stderr, "Failed to connect to MDA\n");
		exit(EX_OSERR);
	}
		
	if(verbose)
		fprintf(stdout, "Connected to MDA: %s\n", before);

	free(before);
}

void local_flush(message_t *message)
{
	char buffer[BUFSIZ];
	size_t n;

	do {
		n = message_read(message, buffer, BUFSIZ);
		if(ferror(mda_fp))
		{
			perror(NULL);
			exit(EX_OSERR);
		}
	} while(n == BUFSIZ);
}

void local_cleanup(void)
{
	if(mda_fp)
	{
		int status;
		
		if((status = pclose(mda_fp)))
		{
			if(WIFSIGNALED(status)) 
				fprintf(stderr, "MDA died of signal %d\n", WTERMSIG(status));
			else if(WIFEXITED(status))
				fprintf(stderr, "MDA returned nonzero status %d\n", WEXITSTATUS(status));
			else
				fprintf(stderr, "MDA failed\n");
			exit(EX_OSERR);
		}
			
		mda_fp = NULL;

		if(verbose)
			fprintf(stdout, "Disconnected to MDA\n");
	}

	if(mda)
		free(mda);
}
