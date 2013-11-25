#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "log.h"

/* $Id: log.c,v 1.2 2002/06/24 04:04:32 fred_nerk Exp $ */

#undef	FLUSH_LOG_STREAM

int global_log_level;
int log_show_function = false;
int log_show_pid = false;
int log_show_file = true;
int log_show_level = true;
int log_show_date = true;
FILE *log_stream = NULL;

pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

void log_set_level(int level)
{
	global_log_level = level;
}

void log(int level, const char *file, int line, const char *function, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);

	if (!log_stream)
		log_stream = stderr;

	if (global_log_level >= level)
	{
		pthread_mutex_lock(&log_mutex);
		if (log_show_date)
			fprintf(log_stream, "%s", date_iso(NULL));
		if (log_show_level)
			fprintf(log_stream, " %2d", level);
		if (log_show_pid)
			fprintf(log_stream, " %5d", getpid());
		if (log_show_file)
			fprintf(log_stream, " %10s:%-3d", file, line);
		if (log_show_function && function)
			fprintf(log_stream, " %13s()", function);
		if (log_show_function || log_show_date || log_show_date || log_show_pid || log_show_file)
			fprintf(log_stream, ": ");
		vfprintf(log_stream, format, ap);
		fprintf(log_stream, "\n");
#ifndef FLUSH_LOG_STREAM
		fflush(log_stream);
#endif
		pthread_mutex_unlock(&log_mutex);
	}
}

int log_open(char *filename)
{
	static char *saved_log_filename = NULL;

	if (log_stream && log_stream != stderr)
		log_close();

	if (filename == NULL && saved_log_filename)
	{
		filename = saved_log_filename;
	}

	if (filename)
	{
		if (saved_log_filename) free(saved_log_filename);
		saved_log_filename = strdup(filename);
		log_stream = fopen(filename, "w+");
		if (!log_stream)
		{
			log_stream = stderr;
			return false;
		}
		fseek(log_stream, 0, SEEK_END);
	}
	else
	{
		log_stream = stderr;
	}

	return true;
}

int log_close()
{
	if (log_stream && log_stream != stderr)
	{
		fflush(log_stream);
		fclose(log_stream);
	}

	log_stream = stderr;
	return true;
}

void log_hex(int level, const char *file, int line, const char *function, const char *title, const char *data, int maxsize)
{
	unsigned int i, j;
	unsigned const char *d = (unsigned const char *)data;

	if (global_log_level >= level)
	{
		log(level, file, line, function, "%s (%d bytes):", title, maxsize);
		pthread_mutex_lock(&log_mutex);
		for (i = 0; i < maxsize; )
		{
			fprintf(log_stream, "%4X: ", i);
			for (j = i; j < maxsize && j < (i + 16); j++)
			{
				fprintf(log_stream, "%02X ", d[j]);
				if (j == i + 7)
					fputs(": ", log_stream);
			}

			for (; j < i + 16; j++)
			{
				fputs("   ", log_stream);
				if (j == i + 7)
					fputs(": ", log_stream);
			}

			fputs("  ", log_stream);
			for (j = i; j < maxsize && j < (i + 16); j++)
			{
				if (d[j] >= 0x20 && d[j] < 0x7f && d[j] != 0x20)
					fputc(d[j], log_stream);
				else
					fputc('.', log_stream);

				if (j == i + 7)
					fputs("  ", log_stream);
			}

			i = j;
			fputs("\n", log_stream);
		}
		fflush(log_stream);
		pthread_mutex_unlock(&log_mutex);
	}
}

Date *date_now()
{
	time_t tt;
	Date *date;

	date = (Date *)malloc(sizeof(Date));
	tt = time(NULL);
	date->tm = localtime(&tt);

	return date;
}

void date_free(Date *date)
{
	free(date);
}

char *date_iso(Date *date)
{
	Date *tempdate;
	static char *buffer = NULL;

	tempdate = date;

	if (!tempdate)
		tempdate = date_now();

	if (!buffer) buffer = (char *)malloc(20);

	sprintf(buffer, "%4d-%02d-%02d %02d:%02d:%02d", tempdate->tm->tm_year + 1900, tempdate->tm->tm_mon + 1, tempdate->tm->tm_mday,
	        tempdate->tm->tm_hour, tempdate->tm->tm_min, tempdate->tm->tm_sec);

	if (!date)
		date_free(tempdate);

	return buffer;
}

int log_get_level()
{
	return global_log_level;
}

