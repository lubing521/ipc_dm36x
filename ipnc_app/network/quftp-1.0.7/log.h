/*
 * log.h
 * $Id: log.h,v 1.2 2002/06/24 04:04:32 fred_nerk Exp $
 */

#ifndef __LOG_C__
#define __LOG_C__

#ifdef __GNUC__

#define LOG_NONE	0, __FILE__, __LINE__, __FUNCTION__
#define LOG_CRITICAL	1, __FILE__, __LINE__, __FUNCTION__
#define LOG_ALERT	2, __FILE__, __LINE__, __FUNCTION__
#define LOG_ERROR	3, __FILE__, __LINE__, __FUNCTION__
#define LOG_WARNING	4, __FILE__, __LINE__, __FUNCTION__
#define LOG_NOTICE	5, __FILE__, __LINE__, __FUNCTION__
#define LOG_INFO	6, __FILE__, __LINE__, __FUNCTION__
#define LOG_MESSAGE	8, __FILE__, __LINE__, __FUNCTION__
#define LOG_CALLS	9, __FILE__, __LINE__, __FUNCTION__
#define LOG_DATA	10, __FILE__, __LINE__, __FUNCTION__

#else

#define LOG_NONE	0, __FILE__, __LINE__, NULL
#define LOG_ALERT	1, __FILE__, __LINE__, NULL
#define LOG_CRITICAL	2, __FILE__, __LINE__, NULL
#define LOG_ERROR	3, __FILE__, __LINE__, NULL
#define LOG_WARNING	4, __FILE__, __LINE__, NULL
#define LOG_NOTICE	5, __FILE__, __LINE__, NULL
#define LOG_INFO	6, __FILE__, __LINE__, NULL
#define LOG_MESSAGE	8, __FILE__, __LINE__, NULL
#define LOG_CALLS	9, __FILE__, __LINE__, NULL
#define LOG_DATA	10, __FILE__, __LINE__, NULL

#endif

#define false		0
#define true		!false
#define VALIDSTR(x)	(x != NULL && *x != 0)

typedef struct
{
	struct tm *tm;
} Date;

extern int log_show_function;
extern int log_show_pid;
extern int log_show_file;
extern int log_show_level;
extern int log_show_date;
void log(int level, const char *file, int line, const char *function, const char *format, ...);
void log_hex(int level, const char *file, int line, const char *function, const char *title, const char *data, int maxsize);
void log_set_level(int level);
int log_get_level();
int log_open(char *filename);
int log_close();
Date *date_now();
void date_free(Date *date);
char *date_iso(Date *date);

#endif
