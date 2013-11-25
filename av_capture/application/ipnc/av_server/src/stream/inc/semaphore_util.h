/** ===========================================================================
* @file semephore_util.h
*
* @path $(IPNCPATH)\multimedia\encode_stream\stream
*
* @desc 
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */

#ifndef _SEM_UTIL_H_
#define _SEM_UTIL_H_

int Sem_Creat( int index );
int Sem_unlock( int index );
int Sem_lock( int index );
int Sem_kill( int index );


#endif
