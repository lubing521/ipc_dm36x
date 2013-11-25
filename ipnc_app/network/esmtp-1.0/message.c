/**
 * \file message.c
 * Simple message handling.
 * 
 * \author José Fonseca
 */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "local.h"
#include "rfc822.h"
#include "xmalloc.h"


message_t *message_new(void)
{
	message_t *message;

	message = (message_t *)xmalloc(sizeof(message_t));

	memset(message, 0, sizeof(message_t));

	INIT_LIST_HEAD(&message->remote_recipients);
	INIT_LIST_HEAD(&message->local_recipients);
	
	message->ret = Ret_NOTSET;
	message->notify = Notify_NOTSET;
	message->body = E8bitmime_NOTSET;
		
	return message;
}

void message_free(message_t *message)
{
	struct list_head *ptr, *tmp;
	recipient_t *recipient;
	
	if(message->reverse_path)
		free(message->reverse_path);

	if(!list_empty(&message->remote_recipients))
		list_for_each_safe(ptr, tmp, &message->remote_recipients)
		{
			recipient = list_entry(ptr, recipient_t, list);
			list_del(ptr);
			
			assert(recipient->address);
			
			free(recipient->address);
					
			free(ptr);
		}

	if(!list_empty(&message->local_recipients))
		list_for_each_safe(ptr, tmp, &message->local_recipients)
		{
			recipient = list_entry(ptr, recipient_t, list);
			list_del(ptr);
			
			assert(recipient->address);
			
			free(recipient->address);
					
			free(ptr);
		}

	if(message->envid)
		free(message->envid);

	if(message->fp)
		fclose(message->fp);
	
	free(message);
}
	
void message_set_reverse_path(message_t *message, const char *address)
{
	if(message->reverse_path)
		free(message->reverse_path);

	message->reverse_path = xstrdup(address);
}

void message_set_envid(message_t *message, const char *address)
{
	if(message->envid)
		free(message->envid);

	message->envid = xstrdup(address);
}

void message_add_recipient(message_t *message, const char *address)
{
	recipient_t *recipient;

	if(address)
	{
		recipient = (recipient_t *)xmalloc(sizeof(recipient_t));

		recipient->address = xstrdup(address);

		if(local_address(address))
			list_add(&recipient->list, &message->local_recipients);
		else
			list_add(&recipient->list, &message->remote_recipients);
	}
}

static void message_buffer_alloc(message_t *message)
{
	char *buffer;
	size_t buffer_size;

	if(!message->buffer_size)
		buffer_size = BUFSIZ;
	else
		buffer_size = message->buffer_size << 1;
	
	buffer = (char *)xrealloc(message->buffer, buffer_size);

	message->buffer = buffer;
	message->buffer_size = buffer_size;
}

static char *message_buffer_readline(message_t *message)
{
	FILE *fp = message->fp ? message->fp : stdin;
	size_t ret = message->buffer_stop, n;
	
	while(1)
	{
		if(message->buffer_stop >= message->buffer_size - 1)
			message_buffer_alloc(message);

		if(!fgets(message->buffer + message->buffer_stop, message->buffer_size - message->buffer_stop, fp))
			return NULL;
		
		n = strlen(message->buffer + message->buffer_stop);
		message->buffer_stop += n;
		
		if(*(message->buffer + message->buffer_stop - 1) == '\n')
			return message->buffer + ret;
	}
}

static void message_buffer_fill(message_t *message)
{
	FILE *fp = message->fp ? message->fp : stdin;

	message->buffer_stop += fread(message->buffer + message->buffer_stop, 1, message->buffer_size - message->buffer_stop, fp);
	
}

static size_t message_buffer_flush(message_t *message, char *ptr, size_t size)
{
	size_t count, n, s;
	char *p, *q;
	
	s = message->buffer_start;
	p = message->buffer + s;
	count = 0;
	while(count < size && message->buffer_start < message->buffer_stop)
	{
		q = memchr(p, '\n', message->buffer_stop - message->buffer_start);
		
		if(q)
			/* read up to the newline */
			n = q - p;
		else
			/* read up to the end of the buffer */
			n = message->buffer_stop - message->buffer_start;

		if(n)
		{
			if(n > (size - count))
				n = size - count;
			
			memcpy(ptr, p, n);

			p += n;
			message->buffer_start += n;
			ptr += n;
			count += n;

			message->buffer_r = *(p - 1) == '\r';
		}

		if(count == size)
			break;
		
		if(q)
		{
			if(!message->buffer_r)
			{
				*ptr++ = '\r';
				count++;
				
				if(count == size)
				{
					message->buffer_r = 1;
					break;
				}
			}
			else
				message->buffer_r = 0;
			
			*ptr++ = *p++;	/* '\n' */
			message->buffer_start++;
			count++;
		}
	}

	/* hook for the MDA pipe */
	if(mda_fp && message->buffer_start != s)
		fwrite(message->buffer +s, 1, message->buffer_start - s, mda_fp);

	if(message->buffer_start == message->buffer_stop)
		message->buffer_start = message->buffer_stop = 0;
	
	return count;
}
	
size_t message_read(message_t *message, char *ptr, size_t size)
{
	size_t count = 0, n;
	char *p = ptr;

	if(!message->buffer)
		message_buffer_alloc(message);

	n = message_buffer_flush(message, p, size);
	count += n;
	p += n;
	
	while(count != size)
	{
		message_buffer_fill(message);
		
		if(!(n = message_buffer_flush(message, p, size - count)))
			break;
		count += n;
		p += n;
	};
		
	return count;
}

int message_eof(message_t *message)
{
	FILE *fp = message->fp ? message->fp : stdin;
	
	if(message->buffer_start != message->buffer_stop)
		return 0;

	return feof(fp);
}

static unsigned message_parse_header(message_t *message, size_t start, size_t stop)
{
	unsigned count = 0;
	const char *address;
	char *header, *next, c;
	
	header = message->buffer + start;
	next = message->buffer + stop;
	
	c = *next;
	*next = '\0';
	
	if(!strncasecmp("From: ", header, 6))
	{
		if((address = next_address(header)))
		{
			message_set_reverse_path(message, address);
			count++;
		}
	}
	else if(!strncasecmp("To: ", header, 4) || 
			!strncasecmp("Cc: ", header, 4) || 
			!strncasecmp("Bcc: ", header, 5))
	{
		address = next_address(header);
		while(address)
		{
			message_add_recipient(message, address);
			count++;
			address = next_address(NULL);
		}

	}
	
	*next = c;
	
	if(!strncasecmp("Bcc: ", header, 5))
	{
		size_t n = message->buffer_stop - stop;

		memcpy(header, next, n);

		message->buffer_stop = start + n;
	}

	return count;
}

unsigned message_parse_headers(message_t *message)
{
	char *line;
	size_t start, stop;
	unsigned count = 0;

	assert(!message->buffer);

	message_buffer_alloc(message);

	start = 0;
	while((line = message_buffer_readline(message)))
	{
		if(line[0] == ' ' || line[0] == '\t')
		{
			/* append line */
		}
		else
		{
			stop = line - message->buffer;
			if(stop)
				count += message_parse_header(message, start, stop);

			start = stop;

			if(line[0] == '\n' || line[0] == '\r')
				return count;
		}
	}
	
	fprintf(stderr, "Failed to parse headers\n");
	exit(EX_DATAERR);
}
