/*
 * Functions for formatting the xml response
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "psia.h"

#define DEFAULT_RESPONSE_SIZE 3000   // big enough for most queries

/* append formatted string to the response buffer */
static void xml_append(request *req, const char *fmt, ...)
{
	va_list ap;
	char text[2056];  //256
	int len; //i;
	char *buf = NULL;
	req->xmlsize;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	len = strlen(text);
	/* check new string will fit in existing buffer */
	if((len + req->xmllen) >= req->xmlsize) {
		req->xmlsize *= 10;
		debug_printf("Response buffer size exceeded, increasing buffer to %d", req->xmlsize);
		buf = malloc(req->xmlsize);
		if(!buf) {
			req->xmlerr = 1;
			return;
		}
		/* copy exiting data into new buffer */
		memcpy(buf, req->xmlstr, req->xmllen);
		free(req->xmlstr);
		req->xmlstr = buf;
	}

	/* append new string to xml buffer */
	buf = &req->xmlstr[req->xmllen];
	memcpy(buf, text, len);

	req->xmllen += len;
	buf = &req->xmlstr[req->xmllen];
	*buf = 0x00;
}

/* initialize the response buffer */
int xadd_init(request *req)
{
	req->xmllen = 0;
	req->xmlerr = 0;
	req->xmlsize = DEFAULT_RESPONSE_SIZE;
	req->xmlstr = malloc(req->xmlsize);
	if(!req->xmlstr) {
		req->xmlerr = 1;
		return R_SERVICE_UNAV;
	}

	*req->xmlstr = 0x00;

	/* add the prolog before any other xml */
	xml_append(req, "%s", PrologString);
	return R_REQUEST_OK;
}

/* add plain text to the response buffer */
void xadd_text(request *req, const char *fmt, ...)
{
	va_list ap;
	char text[256];

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	xml_append(req, "%s", text);
}

/* add <starttag> to the response buffer */
void xadd_stag(request *req, const char *tag)
{
	xml_append(req, "<%s>", tag);
}

/* add <starttag attributes> to the response buffer */
void xadd_stag_attr(request *req, const char *tag, const char *fmt, ...)
{
	va_list ap;
	char attr[2056];  //256

	va_start(ap, fmt);
	vsprintf(attr, fmt, ap);
	va_end(ap);
	xml_append(req, "<%s %s>", tag, attr);
}

/* add <endtag> to the response buffer */
void xadd_etag(request *req, const char *tag)
{
	xml_append(req, "</%s>", tag);
}

/* add <starttag>value<endtag> to the response buffer */
void xadd_elem(request *req, const char *tag, const char *val)
{
	xml_append(req, "<%s>%s</%s>", tag, val, tag);
}

/* empty the response buffer */
void xadd_flush(request *req)
{
	*req->xmlstr = 0x00;
	req->xmllen = 0;
}

/* free the response buffer */
void xadd_cleanup(request *req)
{
	if(req->xmlstr) free(req->xmlstr);
}
