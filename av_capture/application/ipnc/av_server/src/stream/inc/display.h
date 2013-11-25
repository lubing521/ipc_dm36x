/*
 * display.h
 *
 * ============================================================================
 * Copyright (c) Texas Instruments Inc 2007
 *
 * Use of this software is controlled by the terms and conditions found in the
 * license agreement under which this software has been supplied or provided.
 * ============================================================================
 */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <fifoutil.h>
#include <rendezvous.h>
#include <pause.h>

/* Display thread commands */
#define DISPLAY_FLUSH -1
#define DISPLAY_PRIME -2

typedef struct DisplayBufferElement {
    int         id;
    int			prime;
    int         width;
    int         height;
    int 	  	size;
    char         *virtBuf;
    unsigned long physBuf;
} DisplayBufferElement;

typedef struct DisplayEnv {
    Rendezvous_Handle hRendezvousInit;
    Rendezvous_Handle hRendezvousCleanup;
    Rendezvous_Handle hRendezvousPrime;
    Pause_Handle      hPause;
    FifoUtil_Obj      outFifo;
    FifoUtil_Obj      inFifo;
    int				  resolution;
    int				  dispWidth;
    int				  dispHeight;
} DisplayEnv;

extern void *displayThrFxn(void *arg);

int SetDispInterface(int type);



#endif /* _DISPLAY_H */
