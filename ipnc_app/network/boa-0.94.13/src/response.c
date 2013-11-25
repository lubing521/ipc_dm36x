/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996 Larry Doolittle <ldoolitt@boa.org>
 *  Some changes Copyright (C) 1996-99 Jon Nelson <jnelson@boa.org>
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

/* $Id: response.c,v 1.41.2.1 2002/06/06 05:08:54 jnelson Exp $*/

#include "boa.h"
#include <system_default.h>

//#define PRINT_XML

#define HTML "text/html; charset=ISO-8859-1"
#define CRLF "\r\n"

void print_content_type(request * req)
{
    req_write(req, "Content-Type: ");
	req_write(req, get_mime_type(req->request_uri));
    req_write(req, "\r\n");
}

void print_content_length(request * req)
{
    req_write(req, "Content-Length: ");
    req_write(req, simple_itoa(req->filesize));
    req_write(req, "\r\n");
}

void print_last_modified(request * req)
{
    static char lm[] = "Last-Modified: "
        "                             " "\r\n";
    rfc822_time_buf(lm + 15, req->last_modified);
    req_write(req, lm);
}

void print_ka_phrase(request * req)
{
    if (req->kacount > 0 &&
        req->keepalive == KA_ACTIVE && req->response_status < 500) {
        req_write(req, "Connection: Keep-Alive\r\nKeep-Alive: timeout=");
        req_write(req, simple_itoa(ka_timeout));
        req_write(req, ", max=");
        req_write(req, simple_itoa(req->kacount));
        req_write(req, "\r\n");
    } else
        req_write(req, "Connection: close\r\n");
}

void print_http_headers(request * req)
{
    static char stuff[] = "Date: "
        "                             "
        "\r\nServer: " SERVER_VERSION "\r\n";

    rfc822_time_buf(stuff + 6, 0);
    req_write(req, stuff);
    print_ka_phrase(req);
}

/* The routines above are only called by the routines below.
 * The rest of Boa only enters through the routines below.
 */

/* R_REQUEST_OK: 200 */
void send_r_request_ok(request * req)
{
    req->response_status = R_REQUEST_OK;
    if (req->simple)
        return;
//    req_write(req, "HTTP/1.1 200 OK\r\nConnection: close\r\n");
    req_write(req, "HTTP/1.1 200 OK\r\n");
    print_http_headers(req);

    if (!req->is_cgi) {
        print_content_type(req);
        print_content_length(req);
        print_last_modified(req);
        req_write(req, "\r\n");
    }
}

#ifdef DAVINCI_IPCAM
void send_request_ok_no_cache(request * req)
{
    req->response_status = R_REQUEST_OK;

    req_write(req, "HTTP/1.1 200 OK\r\nPragma: no-cache\r\nCache-Control: no-store\r\nConnection: close\r\n");

    if (!req->is_cgi) {
        print_content_type(req);
        print_content_length(req);
        req_write(req, "\r\n");
    }
}

void send_request_ok_sdget(request * req)
{
    req->response_status = R_REQUEST_OK;

    req_write(req, "HTTP/1.1 200 OK\r\nContent-type: text/html\r\nConnection: close\r\nPragma: no-cache\r\nCache-Control: no-store\r\n");
    print_content_length(req);
    req_write(req, "\r\n");
}

void send_request_ok_api(request * req)
{
    req->response_status = R_REQUEST_OK;
    req_write(req, "HTTP/1.1 200 OK\r\nContent-type: text/html\r\nPragma: no-cache\r\nCache-Control: no-store\r\n\r\n");
}

void send_request_ok_jpeg(request * req)
{
    req->response_status = R_REQUEST_OK;
    req_write(req, "HTTP/1.1 200 OK\r\nPragma: no-cache\r\nCache-Control: no-store\r\n");
    print_content_length(req);
    req_write(req, "\r\n");
}

void print_mjpeg_headers(request * req)
{
    char res[32];

    req_write(req, HTTP_BOUNDARY "\r\nContent-Type: image/jpeg\r\n");
    print_content_length(req);

    req_write(req, "X-Resolution: ");
    sprintf(res, "%d*%d" CRLF, req->av_data.width, req->av_data.height);
    req_write(req, res);

    req_write(req, CRLF);
}

void write_time_string(char *buf)
{
    struct tm *t;
    char *p;
    unsigned int a;

    t = gmtime(&current_time);

    p = buf + 18;
    /* p points to the last char in the buf */

    a = t->tm_sec;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = t->tm_min;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = t->tm_hour;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ' ';

    a = t->tm_mday;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = '-';
    a = t->tm_mon+1;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = '-';
    a = 1900 + t->tm_year;
    while (a) {
        *p-- = '0' + a % 10;
        a /= 10;
    }
}

void print_mpeg4_headers(request * req)
{
    static char xtime[] = "X-Time:                    " CRLF;
    char res[32];

    req_write(req, HTTP_BOUNDARY "\r\nContent-Type: image/mpeg4\r\n");
    print_content_length(req);

    req_write(req, "X-Status: 0" CRLF);

    req_write(req, "X-Tag: ");
    req_write(req, simple_itoa(req->av_data.serial));
    req_write(req, CRLF);

    req_write(req, "X-Flags: 0" CRLF);

    req_write(req, "X-Alarm: 0" CRLF);

//    req_write(req, "X-Frametype: ");
//    req_write(req, (req->av_data.frameType==AV_FRAME_TYPE_I_FRAME)?"I"CRLF:"P"CRLF);

    req_write(req, "X-Framerate: 30" CRLF);

    req_write(req, "X-Resolution: ");
    sprintf(res, "%d*%d" CRLF, req->av_data.width, req->av_data.height);
    req_write(req, res);

    req_write(req, "X-Audio: 1" CRLF);

    write_time_string(xtime+8);
    req_write(req, xtime);

    req_write(req, CRLF);
}

void print_avc_headers(request * req)
{
    static char xtime[] = "X-Time:                    " CRLF;
    char res[32];

    req_write(req, HTTP_BOUNDARY "\r\nContent-Type: image/avc\r\n");
    print_content_length(req);

    req_write(req, "X-Status: 0" CRLF);

    req_write(req, "X-Tag: ");
    req_write(req, simple_itoa(req->av_data.serial));
    req_write(req, CRLF);

    req_write(req, "X-Flags: 0" CRLF);

    req_write(req, "X-Alarm: 0" CRLF);

//    req_write(req, "X-Frametype: ");
//    req_write(req, (req->av_data.frameType==AV_FRAME_TYPE_I_FRAME)?"I"CRLF:"P"CRLF);

    req_write(req, "X-Framerate: 30" CRLF);

    req_write(req, "X-Resolution: ");
    sprintf(res, "%d*%d" CRLF, req->av_data.width, req->av_data.height);
    req_write(req, res);

    req_write(req, "X-Audio: 1" CRLF);

    write_time_string(xtime+8);
    req_write(req, xtime);

    req_write(req, CRLF);
}

void print_audio_headers(request * req)
{
    req_write(req, HTTP_BOUNDARY "\r\nContent-Type: audio/wav\r\n");
    print_content_length(req);

    req_write(req, "X-Codec: U-LAW" CRLF);

    req_write(req, "X-Bitrate: 8000" CRLF);

    req_write(req, "X-Tag: ");
    req_write(req, simple_itoa(req->audio_sync));
    req_write(req, CRLF);

    req_write(req, CRLF);
}

void send_request_ok_mjpeg(request * req)
{
    req->response_status = R_REQUEST_OK;
    req_write(req, "HTTP/1.1 200 OK\r\nConnection: Close\r\nPragma: no-cache\r\nCache-Control: no-store\r\nContent-Type: multipart/x-mixed-replace;boundary=" HTTP_BOUNDARY "\r\n\r\n");
    print_mjpeg_headers(req);
}

void send_request_ok_mpeg4(request * req)
{
    req->response_status = R_REQUEST_OK;
    req_write(req, "HTTP/1.1 200 OK\r\nConnection: Close\r\nPragma: no-cache\r\nCache-Control: no-store\r\nContent-Type: multipart/x-mixed-replace;boundary=" HTTP_BOUNDARY "\r\n\r\n");
    print_mpeg4_headers(req);
}

void send_request_ok_avc(request * req)
{
    req->response_status = R_REQUEST_OK;
    req_write(req, "HTTP/1.1 200 OK\r\nConnection: Close\r\nPragma: no-cache\r\nCache-Control: no-store\r\nContent-Type: multipart/x-mixed-replace;boundary=" HTTP_BOUNDARY "\r\n\r\n");
    print_avc_headers(req);
}

#endif

/* R_MOVED_PERM: 301 */
void send_r_moved_perm(request * req, char *url)
{
    SQUASH_KA(req);
    req->response_status = R_MOVED_PERM;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 301 Moved Permanently\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n");

        req_write(req, "Location: ");
        req_write_escape_http(req, url);
        req_write(req, "\r\n\r\n");
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>301 Moved Permanently</TITLE></HEAD>\n"
                  "<BODY>\n<H1>301 Moved</H1>The document has moved\n"
                  "<A HREF=\"");
        req_write_escape_html(req, url);
        req_write(req, "\">here</A>.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_MOVED_TEMP: 302 */
void send_r_moved_temp(request * req, char *url, char *more_hdr)
{
    SQUASH_KA(req);
    req->response_status = R_MOVED_TEMP;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 302 Moved Temporarily\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n");

        req_write(req, "Location: ");
        req_write_escape_http(req, url);
        req_write(req, "\r\n");
        req_write(req, more_hdr);
        req_write(req, "\r\n\r\n");
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>302 Moved Temporarily</TITLE></HEAD>\n"
                  "<BODY>\n<H1>302 Moved</H1>The document has moved\n"
                  "<A HREF=\"");
        req_write_escape_html(req, url);
        req_write(req, "\">here</A>.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_NOT_MODIFIED: 304 */
void send_r_not_modified(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_NOT_MODIFIED;
    req_write(req, "HTTP/1.0 304 Not Modified\r\n");
    print_http_headers(req);
    print_content_type(req);
    req_write(req, "\r\n");
    req_flush(req);
}

/* R_BAD_REQUEST: 400 */
void send_r_bad_request(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_BAD_REQUEST;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 400 Bad Request\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD)
        req_write(req,
                  "<HTML><HEAD><TITLE>400 Bad Request</TITLE></HEAD>\n"
                  "<BODY><H1>400 Bad Request</H1>\nYour client has issued "
                  "a malformed or illegal request.\n</BODY></HTML>\n");
    req_flush(req);
}

/* R_UNAUTHORIZED: 401 */
void send_r_unauthorized(request * req, char *realm_name)
{
    SQUASH_KA(req);
    req->response_status = R_UNAUTHORIZED;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 401 Unauthorized\r\n");
        print_http_headers(req);
        req_write(req, "WWW-Authenticate: Basic realm=\"");
        req_write(req, realm_name);
        req_write(req, "\"\r\n");
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>401 Unauthorized</TITLE></HEAD>\n"
                  "<BODY><H1>401 Unauthorized</H1>\nYour client does not "
                  "have permission to get URL ");
        req_write_escape_html(req, req->request_uri);
        req_write(req, " from this server.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_FORBIDDEN: 403 */
void send_r_forbidden(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_FORBIDDEN;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 403 Forbidden\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req, "<HTML><HEAD><TITLE>403 Forbidden</TITLE></HEAD>\n"
                  "<BODY><H1>403 Forbidden</H1>\nYour client does not "
                  "have permission to get URL ");
        req_write_escape_html(req, req->request_uri);
        req_write(req, " from this server.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_NOT_FOUND: 404 */
void send_r_not_found(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_NOT_FOUND;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 404 Not Found\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req, "<HTML><HEAD><TITLE>404 Not Found</TITLE></HEAD>\n"
                  "<BODY><H1>404 Not Found</H1>\nThe requested URL ");
        req_write_escape_html(req, req->request_uri);
        req_write(req, " was not found on this server.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_ERROR: 500 */
void send_r_error(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_ERROR;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 500 Server Error\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>500 Server Error</TITLE></HEAD>\n"
                  "<BODY><H1>500 Server Error</H1>\nThe server encountered "
                  "an internal error and could not complete your request.\n"
                  "</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_NOT_IMP: 501 */
void send_r_not_implemented(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_NOT_IMP;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 501 Not Implemented\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>501 Not Implemented</TITLE></HEAD>\n"
                  "<BODY><H1>501 Not Implemented</H1>\nPOST to non-script "
                  "is not supported in Boa.\n</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_BAD_GATEWAY: 502 */
void send_r_bad_gateway(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_BAD_GATEWAY;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 502 Bad Gateway" CRLF);
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML CRLF CRLF); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>502 Bad Gateway</TITLE></HEAD>\n"
                  "<BODY><H1>502 Bad Gateway</H1>\nThe CGI was "
                  "not CGI/1.1 compliant.\n" "</BODY></HTML>\n");
    }
    req_flush(req);
}

/* R_SERVICE_UNAVAILABLE: 503 */
void send_r_service_unavailable(request * req) /* 503 */
{
    static char body[] =
        "<HTML><HEAD><TITLE>503 Service Unavailable</TITLE></HEAD>\n"
        "<BODY><H1>503 Service Unavailable</H1>\n"
        "There are too many connections in use right now.\r\n"
        "Please try again later.\r\n</BODY></HTML>\n";
    static int _body_len;
    static char *body_len;

    if (!_body_len)
        _body_len = strlen(body);
    if (!body_len)
        body_len = strdup(simple_itoa(_body_len));
    if (!body_len) {
        log_error_time();
        perror("strdup of _body_len from simple_itoa");
    }


    SQUASH_KA(req);
    req->response_status = R_SERVICE_UNAV;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 503 Service Unavailable\r\n");
        print_http_headers(req);
        if (body_len) {
            req_write(req, "Content-Length: ");
            req_write(req, body_len);
            req_write(req, "\r\n");
        }
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header
                                                               */
    }
    if (req->method != M_HEAD) {
        req_write(req, body);
    }
    req_flush(req);
}


/* R_NOT_IMP: 505 */
void send_r_bad_version(request * req)
{
    SQUASH_KA(req);
    req->response_status = R_BAD_VERSION;
    if (!req->simple) {
        req_write(req, "HTTP/1.0 505 HTTP Version Not Supported\r\n");
        print_http_headers(req);
        req_write(req, "Content-Type: " HTML "\r\n\r\n"); /* terminate header */
    }
    if (req->method != M_HEAD) {
        req_write(req,
                  "<HTML><HEAD><TITLE>505 HTTP Version Not Supported</TITLE></HEAD>\n"
                  "<BODY><H1>505 HTTP Version Not Supported</H1>\nHTTP versions "
                  "other than 0.9 and 1.0 "
                  "are not supported in Boa.\n<p><p>Version encountered: ");
        req_write(req, req->http_version);
        req_write(req, "<p><p></BODY></HTML>\n");
    }
    req_flush(req);
}

/* PSIA GET Response */
void send_r_psia_get(request * req)
{
#ifdef PRINT_XML
   	int i;
#endif
	char *xml_content;

	int body_length = req->xmllen;
	char *body_len = strdup(simple_itoa(body_length));

	xml_content = req->xmlstr;

#ifdef PRINT_XML
	fprintf(stderr,"\r\n\nResponse XML Content: \n");
	for (i=0;i <= req->xmllen ;i++ )
	    fprintf(stderr,"%c",*xml_content++);
#endif

	//fprintf(stderr,"\r\nContent Length: %d\n",body_length);

    req->response_status = R_REQUEST_OK;

	req_write(req, "HTTP/1.1 200 OK\r\n");

    req_write(req, "Content-Length: ");
    req_write(req, body_len);
    req_write(req, "\r\n");

    req_write(req, "Content-Type: application/xml; charset=\"UTF-8\" \r\n\r\n"); /* terminate header*/

    req_write(req, req->xmlstr);

    req_flush(req);
}

void send_r_psia_not_implemented(request * req)
{
   	//int i;
	//char *xml_content;

	int body_length = req->xmllen;
	char *body_len = strdup(simple_itoa(body_length));

	//xml_content = req->xmlstr;

	//fprintf(stderr,"\r\nResponse XML Content: \n");
	//for (i=0;i <= req->xmllen ;i++ )
		//fprintf(stderr,"%c",*xml_content++);

	//fprintf(stderr,"\r\nContent Length: %d\n",body_length);


    req->response_status = R_FORBIDDEN;

	req_write(req, "HTTP/1.0 403 Forbidden\r\n");

    req_write(req, "Content-Length: ");
    req_write(req, body_len);
    req_write(req, "\r\n");

    req_write(req, "Content-Type: application/xml; charset=\"UTF-8\" \r\n\r\n"); /* terminate header*/

    req_write(req, req->xmlstr);

    req_flush(req);
}

/* PSIA DateTime String Response */
void send_r_psia_datetime(request * req,char *local_time)
{
    SQUASH_KA(req);
    req->response_status = R_REQUEST_OK;

	req_write(req,
                  "<HTML><HEAD><TITLE>Local Time</TITLE></HEAD>\n"
                  "<BODY>\n<H1>DateTime</H1>Local Time:\n");
    req_write_escape_html(req, local_time);
    req_write(req, "\n</BODY></HTML>\n");

    req_flush(req);
}

/* PSIA TimeZone String Response */
void send_r_psia_timezone(request * req,char *timezone)
{
    SQUASH_KA(req);
    req->response_status = R_REQUEST_OK;

	req_write(req,
                  "<HTML><HEAD><TITLE>Time Zone</TITLE></HEAD>\n"
                  "<BODY>\n<H1>Time Zone String</H1>Time Zone:\n");
    req_write_escape_html(req, timezone);
    req_write(req, "\n</BODY></HTML>\n");

    req_flush(req);
}

/* PSIA Streaming Response */
void send_r_psia_ok_mjpeg(request * req)
{
	char res[32];

	req->response_status = R_REQUEST_OK;
    req_write(req, "HTTP/1.1 200 OK\r\nConnection: Close\r\nPragma: no-cache\r\nCache-Control: no-store\r\nContent-Type: multipart/x-mixed-replace;boundary=" HTTP_BOUNDARY "\r\n\r\n");

   	req_write(req, HTTP_BOUNDARY "\r\nContent-Type: image/jpeg\r\n");
	req_write(req, "Content-Length: ");
    req_write(req, simple_itoa(req->filesize));
    req_write(req, "\r\n");

    req_write(req, "X-Resolution: ");
    sprintf(res, "%d*%d" CRLF, req->av_data.width, req->av_data.height);
    req_write(req, res);
	req_write(req, CRLF);
}

void send_r_psia_ok_avc(request * req)
{
	static char xtime[] = "X-Time:                    " CRLF;
    char res[32];

	req->response_status = R_REQUEST_OK;
    req_write(req, "HTTP/1.1 200 OK\r\nConnection: Close\r\nPragma: no-cache\r\nCache-Control: no-store\r\nContent-Type: multipart/x-mixed-replace;boundary=" HTTP_BOUNDARY "\r\n\r\n");

    req_write(req, HTTP_BOUNDARY "\r\nContent-Type: image/avc\r\n");
    req_write(req, "Content-Length: ");
    req_write(req, simple_itoa(req->filesize));
    req_write(req, "\r\n");

    req_write(req, "X-Status: 0" CRLF);

    req_write(req, "X-Tag: ");
    req_write(req, simple_itoa(req->av_data.serial));
    req_write(req, CRLF);

    req_write(req, "X-Flags: 0" CRLF);

    req_write(req, "X-Alarm: 0" CRLF);

    req_write(req, "X-Framerate: 30" CRLF);

    req_write(req, "X-Resolution: ");
    sprintf(res, "%d*%d" CRLF, req->av_data.width, req->av_data.height);
    req_write(req, res);

    req_write(req, "X-Audio: 1" CRLF);

    write_time_string(xtime+8);
    req_write(req, xtime);

    req_write(req, CRLF);
}

void send_r_psia_ok_mpeg4(request * req)
{
	static char xtime[] = "X-Time:                    " CRLF;
    char res[32];

	req->response_status = R_REQUEST_OK;
    req_write(req, "HTTP/1.1 200 OK\r\nConnection: Close\r\nPragma: no-cache\r\nCache-Control: no-store\r\nContent-Type: multipart/x-mixed-replace;boundary=" HTTP_BOUNDARY "\r\n\r\n");


    req_write(req, HTTP_BOUNDARY "\r\nContent-Type: image/mpeg4\r\n");
    req_write(req, "Content-Length: ");
    req_write(req, simple_itoa(req->filesize));
    req_write(req, "\r\n");

    req_write(req, "X-Status: 0" CRLF);

    req_write(req, "X-Tag: ");
    req_write(req, simple_itoa(req->av_data.serial));
    req_write(req, CRLF);

    req_write(req, "X-Flags: 0" CRLF);

    req_write(req, "X-Alarm: 0" CRLF);

    req_write(req, "X-Framerate: 30" CRLF);

    req_write(req, "X-Resolution: ");
    sprintf(res, "%d*%d" CRLF, req->av_data.width, req->av_data.height);
    req_write(req, res);

    req_write(req, "X-Audio: 1" CRLF);

    write_time_string(xtime+8);
    req_write(req, xtime);

    req_write(req, CRLF);
}

void send_r_psia_supportReport(request * req)
{
   	int readSize;
	//char *xml_content;
	FILE *fp;
	char *buff;

	fp = fopen(SUPPORT_REPORT, "rb");

	if(fp == NULL)
	{
		fprintf(stderr,"\r\nCan't open support report\n");
		return;
	}
	fseek(fp,0,SEEK_END);
	int body_length = ftell(fp);
	rewind(fp);

	char *body_len = strdup(simple_itoa(body_length));

    req->response_status = R_REQUEST_OK;

	req_write(req, "HTTP/1.1 200 OK\r\n");

    req_write(req, "Content-Length: ");
    req_write(req, body_len);
    req_write(req, "\r\n");

	print_content_type(req);
    req_write(req, "\r\n");

	buff = (char*) malloc (sizeof(char)*body_length);

	if(buff == NULL)
	{
		fprintf(stderr,"\r\nMemory error\n");
		return;
	}

	readSize = fread(buff,1,body_length,fp);

	//fprintf(stderr,"\r\nbody_length:%d\n",body_length);

	if(readSize != body_length)
	{
		fprintf(stderr,"\r\nReading error\n");
		return;
	}

	memcpy(req->buffer + req->buffer_end, buff, body_length);
	req->buffer_end += body_length;

    req_flush(req);
	fclose(fp);
	free(buff);
}
