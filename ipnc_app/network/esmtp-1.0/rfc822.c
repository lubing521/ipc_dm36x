/**
 * \file rfc822.c
 * Code for slicing and dicing RFC822 mail headers.
 * 
 *  How to parse RFC822 headers in C. This is not a fully conformant
 *  implementation of RFC822 or RFC2822, but it has been in production use in a
 *  widely-deployed MTA (fetcmail) since 1996 without complaints.  Really
 *  perverse combinations of quoting and commenting could break it.
 * 
 * \author Eric S. Raymond <esr@thyrsus.com>, 1997.  This source code example
 * is part of fetchmail and the Unix Cookbook, and are released under the MIT
 * license. 
 */


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


#define HEADER_END(p)	((p)[0] == '\n' && ((p)[1] != ' ' && (p)[1] != '\t'))


#define START_HDR	0	/**< before header colon */
#define SKIP_JUNK	1	/**< skip whitespace, \n, and junk */
#define BARE_ADDRESS	2	/**< collecting address without delimiters */
#define INSIDE_DQUOTE	3	/**< inside double quotes */
#define INSIDE_PARENS	4	/**< inside parentheses */
#define INSIDE_BRACKETS	5	/**< inside bracketed address */
#define ENDIT_ALL	6	/**< after last address */

/**
 * Parse addresses in succession out of a specified RFC822 header.
 *
 * \param hdr header to be parsed, NUL to continue previous \p hdr. 
 */
char *next_address(const char *hdr)
{
    static char address[BUFSIZ];
    static int tp;
    static const char *hp;
    static int	state, oldstate;
    int parendepth = 0;

#define NEXTTP()	((tp < sizeof(address)-1) ? tp++ : tp)

    if (hdr)
    {
	hp = hdr;
	state = START_HDR;
	tp = 0;
    }

    for (; *hp; hp++)
    {

	if (state == ENDIT_ALL)		/* after last address */
	    return(NULL);
	else if (HEADER_END(hp))
	{
	    state = ENDIT_ALL;
	    if (tp)
	    {
		while (isspace(address[--tp]))
		    continue;
		address[++tp] = '\0';
		tp = 0;
		return (address);
	    }
	    return(NULL);
	}
	else if (*hp == '\\')		/* handle RFC822 escaping */
	{
	    if (state != INSIDE_PARENS)
	    {
		address[NEXTTP()] = *hp++;	/* take the escape */
		address[NEXTTP()] = *hp;	/* take following char */
	    }
	}
	else switch (state)
	{
	case START_HDR:   /* before header colon */
	    if (*hp == ':')
		state = SKIP_JUNK;
	    break;

	case SKIP_JUNK:		/* looking for address start */
	    if (*hp == '"')	/* quoted string */
	    {
		oldstate = SKIP_JUNK;
	        state = INSIDE_DQUOTE;
		address[NEXTTP()] = *hp;
	    }
	    else if (*hp == '(')	/* address comment -- ignore */
	    {
		parendepth = 1;
		oldstate = SKIP_JUNK;
		state = INSIDE_PARENS;    
	    }
	    else if (*hp == '<')	/* begin <address> */
	    {
		state = INSIDE_BRACKETS;
		tp = 0;
	    }
	    else if (*hp != ',' && !isspace(*hp))
	    {
		--hp;
	        state = BARE_ADDRESS;
	    }
	    break;

	case BARE_ADDRESS:   	/* collecting address without delimiters */
	    if (*hp == ',')  	/* end of address */
	    {
		if (tp)
		{
		    address[NEXTTP()] = '\0';
		    state = SKIP_JUNK;
		    tp = 0;
		    return(address);
		}
	    }
	    else if (*hp == '(')  	/* beginning of comment */
	    {
		parendepth = 1;
		oldstate = BARE_ADDRESS;
		state = INSIDE_PARENS;    
	    }
	    else if (*hp == '<')  	/* beginning of real address */
	    {
		state = INSIDE_BRACKETS;
		tp = 0;
	    }
	    else if (*hp == '"')        /* quoted word, copy verbatim */
	    {
	        oldstate = state;
		state = INSIDE_DQUOTE;
                address[NEXTTP()] = *hp;
            }
	    else if (!isspace(*hp)) 	/* just take it, ignoring whitespace */
		address[NEXTTP()] = *hp;
	    break;

	case INSIDE_DQUOTE:	/* we're in a quoted string, copy verbatim */
	    if (*hp != '"')
	        address[NEXTTP()] = *hp;
	    else
	    {
	        address[NEXTTP()] = *hp;
		state = oldstate;
	    }
	    break;

	case INSIDE_PARENS:	/* we're in a parenthesized comment, ignore */
	    if (*hp == '(')
		++parendepth;
	    else if (*hp == ')')
		--parendepth;
	    if (parendepth == 0)
		state = oldstate;
	    break;

	case INSIDE_BRACKETS:	/* possible <>-enclosed address */
	    if (*hp == '>')	/* end of address */
	    {
		address[NEXTTP()] = '\0';
		state = SKIP_JUNK;
		++hp;
		tp = 0;
		return(address);
	    }
	    else if (*hp == '<')	/* nested <> */
	        tp = 0;
	    else if (*hp == '"')	/* quoted address */
	    {
	        address[NEXTTP()] = *hp;
		oldstate = INSIDE_BRACKETS;
		state = INSIDE_DQUOTE;
	    }
	    else			/* just copy address */
		address[NEXTTP()] = *hp;
	    break;
	}
    }

    return(NULL);
}
