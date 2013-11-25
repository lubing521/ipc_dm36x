%{
/**
 * \file parser.y
 * Parser for the rcfile.
 *
 * \author Adapted from fetchmail's rcfile_y.y by José Fonseca
 */


#include "config.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#include <libesmtp.h>

#include "main.h"
#include "smtp.h"
#include "local.h"
#include "xmalloc.h"

extern int yylex (void);


/** Path name of dot file */
static const char *rcfile = NULL;		

int yydebug;			/**< in case we didn't generate with -- debug */

static identity_t *identity = NULL;

/**
 * Utility macro to set the default identity, if one isn't set yet. 
 * 
 * It's necessary for compatability with older configurations files where the
 * default identity was defined in a global section, instead of defined by the
 * 'default' or the first defined identity, if no 'default' keyword is given.
 */
#define SET_DEFAULT_IDENTITY						\
do {									\
	if(!default_identity)						\
		default_identity = identity;				\
} while(0)

/* using Bison, this arranges that yydebug messages will show actual tokens */
extern char * yytext;
#define YYPRINT(fp, type, val)	fprintf(fp, " = \"%s\"", yytext)

void yyerror (const char *s);


%}

%union {
    int number;
    char *sval;
}

%token IDENTITY DEFAULT HOSTNAME USERNAME PASSWORD STARTTLS CERTIFICATE_PASSPHRASE PRECONNECT POSTCONNECT MDA QUALIFYDOMAIN HELO FORCE SENDER MSGID REVERSE_PATH

%token MAP

%token DISABLED ENABLED REQUIRED
%token <sval>  STRING
%token <number> NUMBER

%%

rcfile		: /* empty */
		| statement_list
		| statement_list identity_list
		| identity_list
		;

identity_list	: identity
		| identity statement_list
		| identity_list identity statement_list
		;


map		: /* empty */
		| MAP
		;

identity	: IDENTITY map STRING
			{
				identity = identity_new();
				identity_add(identity);
				identity->address = xstrdup($3);
			}
		;

statement_list	: statement
		| statement_list statement
		;

/* future global options should also have the form SET <name> optmap <value> */
statement	: HOSTNAME map STRING	{ identity->host = xstrdup($3); SET_DEFAULT_IDENTITY; }
		| USERNAME map STRING	{ identity->user = xstrdup($3); SET_DEFAULT_IDENTITY; }
		| PASSWORD map STRING	{ identity->pass = xstrdup($3); SET_DEFAULT_IDENTITY; }
		| STARTTLS map DISABLED	{ identity->starttls = Starttls_DISABLED; SET_DEFAULT_IDENTITY; }
		| STARTTLS map ENABLED	{ identity->starttls = Starttls_ENABLED; SET_DEFAULT_IDENTITY; }
		| STARTTLS map REQUIRED	{ identity->starttls = Starttls_REQUIRED; SET_DEFAULT_IDENTITY; }
		| CERTIFICATE_PASSPHRASE map STRING { identity->certificate_passphrase = xstrdup($3); SET_DEFAULT_IDENTITY; }
		| PRECONNECT map STRING	{ identity->preconnect = xstrdup($3); SET_DEFAULT_IDENTITY; }
		| POSTCONNECT map STRING { identity->postconnect = xstrdup($3); SET_DEFAULT_IDENTITY; }
		| QUALIFYDOMAIN map STRING	{ identity->qualifydomain = xstrdup($3); SET_DEFAULT_IDENTITY; }
		| HELO map STRING	{ identity->helo = xstrdup($3); SET_DEFAULT_IDENTITY; }
		| FORCE REVERSE_PATH map STRING	{ identity->force_reverse_path = xstrdup($4); SET_DEFAULT_IDENTITY; }
		| FORCE SENDER map STRING	{ identity->force_sender = xstrdup($4); SET_DEFAULT_IDENTITY; }
		| MSGID map DISABLED	{ identity->prohibit_msgid = 1; SET_DEFAULT_IDENTITY; }
		| MSGID map ENABLED	{ identity->prohibit_msgid = 0; SET_DEFAULT_IDENTITY; }
		| MDA map STRING	{ mda = xstrdup($3); }
		| DEFAULT		{ default_identity = identity; }
		;

%%

/* lexer interface */
extern int lineno;
extern char *yytext;
extern FILE *yyin;

/** 
 * Report a syntax error.
 */
void yyerror (const char *s)
{
	fprintf(stderr, "%s:%d: %s at %s\n", rcfile, lineno, s, 
		(yytext && yytext[0]) ? yytext : "end of input");
	exit(EX_CONFIG);
}

/**
 * Check that a configuration file is secure.
 * 
 * \param securecheck if set to 1 strict file permission tests will be run.
 * 
 * \return 0 if everything is OK, -1 in case of error 
 * 
 */
int rcfile_check(const char *pathname, const int securecheck)
{
#ifndef __EMX__
	struct stat statbuf;

	errno = 0;

	/* special case useful for debugging purposes */
	if (strcmp("/dev/null", pathname) == 0)
		return 0;

	/* pass through the special name for stdin */
	if (strcmp("-", pathname) == 0)
		return 0;

	/* the run control file must have the same uid as the REAL uid of this 
	 * process, it must have permissions no greater than 600, and it must
	 * not be a symbolic link.  We check these conditions here. 
	 */
	if (lstat(pathname, &statbuf) < 0) {
		if (errno == ENOENT) 
			return 0;
		else {
			fprintf(stderr, "lstat: %s: %s\n", pathname, strerror(errno));
			return -1;
		}
	}

	if (!securecheck)
		return 0;

	if (!S_ISREG(statbuf.st_mode))
	{
		fprintf(stderr, "File %s must be a regular file.\n", pathname);
		return -1;
	}

#ifndef __BEOS__
	if (statbuf.st_mode & (S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IXOTH))
	{
		fprintf(stderr, "File %s must have no more than -rwx--x--- (0710) permissions.\n", pathname);
		return -1;
	}
#endif /* !__BEOS__ */

#ifdef HAVE_GETEUID
	if (statbuf.st_uid != geteuid())
#else
	if (statbuf.st_uid != getuid())
#endif /* HAVE_GETEUID */
	{
		fprintf(stderr, "File %s must be owned by you.\n", pathname);
		return -1;
	}
#endif
	return 0;
}


#define RCFILE "esmtprc"
#define DOT_RCFILE "." RCFILE
#define ETC_RCFILE SYSCONFDIR "/" RCFILE

void rcfile_parse(const char *_rcfile)
{
	char *dot_rcfile = NULL;

	if(_rcfile)
	{
		/* Configuration file specified on the command line */
		rcfile = _rcfile;
		if(!(yyin = fopen(rcfile, "r")))
			goto failure;
			
		goto success;
	}
	
	/* Search for the user configuration file */
	do 
	{
		char *home;

		if (!(home = getenv("HOME")))
			break;
			
		dot_rcfile = xmalloc(strlen(home) + strlen(DOT_RCFILE) + 2);

		strcpy(dot_rcfile, home);
		if (dot_rcfile[strlen(dot_rcfile) - 1] != '/')
			strcat(dot_rcfile, "/");
		strcat(dot_rcfile, DOT_RCFILE);
		rcfile = dot_rcfile;

 		if (rcfile_check(rcfile, 1) < 0)
 			goto failure;
 
		if(!(yyin = fopen(rcfile, "r")))
		{
			if(errno == ENOENT)
				break;
			else
				goto failure;
		}
		
		goto success;
	} 
	while(0);
	
	/* Search for the global configuration file */
	do
	{
		rcfile = ETC_RCFILE;
		if(!(yyin = fopen(rcfile, "r")))
		{
			if(errno == ENOENT)
				break;
			else
				goto failure;
		}

		goto success;
	}
	while(0);
		
	/* No configuration file found */
	fprintf(stderr, "No configuration file found at %s or " ETC_RCFILE "\n", dot_rcfile);
	exit(EX_CONFIG);

success:
	/* Configuration file opened */
	
	identity = identity_new();
	identity_add(identity);

	yyparse();	/* parse entire file */

	SET_DEFAULT_IDENTITY;

	fclose(yyin);	/* not checking this should be safe, file mode was r */

	rcfile = NULL;
	if(dot_rcfile)
		free(dot_rcfile);

	return;

failure:
	/* Failure to open the configuration file */
	fprintf(stderr, "open: %s: %s\n", rcfile, strerror(errno));
	exit(EX_CONFIG);
}

/* easier to do this than cope with variations in where the library lives */
int yywrap(void) { return 1; }
