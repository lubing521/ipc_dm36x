/**
 * \file xmalloc.c
 * Allocate space or die.
 *
 * \author Adapted from fetchmail's xmalloc.c by José Fonseca.
 */

#ifndef _XMALLOC_H
#define _XMALLOC_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"


static inline 
void *xmalloc(size_t size)
{
	void *ptr;

	ptr = malloc(size);
	if (ptr == NULL)
	{
		perror(NULL);
		exit(EX_OSERR);
	}
	return(ptr);
}

static inline
void *xrealloc(void *ptr, size_t size)
{
	if (ptr == 0)
		return xmalloc(size);
	ptr = realloc(ptr, size);
	if (ptr == NULL)
	{
		perror(NULL);
		exit(EX_OSERR);
	}
	return ptr;
}

static inline
char *xstrdup(const char *s)
{
	char *p;
	p = (char *)xmalloc(strlen(s) + 1);
	strcpy(p, s);
	return p;
}

#endif
