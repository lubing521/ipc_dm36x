/**
 * \file local.h
 */

#ifndef _LOCAL_H
#define _LOCAL_H


#include <stdio.h>

#include "message.h"


extern char *mda;
extern FILE *mda_fp;


/** Check whether it's a local or a remote address */
int local_address(const char *address);

/** Send a message locally (via a MDA) */
void local_init(message_t *msg);

void local_flush(message_t *msg);

void local_cleanup(void);

#endif
