/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#ifndef _CSL_BUF_H_
#define _CSL_BUF_H_

#include <csl_soc.h>

/**
  \file csl_buf.h
  \brief Buffer APIs
*/

/**
  \defgroup  CSL_BUF Buffer

  Buffer APIs

  @{
*/

#define CSL_BUF_NUM_MAX 16          ///< Max number of buffer's

#define CSL_BUF_STATE_LOCKED 0      ///< Buffer state : Locked
#define CSL_BUF_STATE_FULL   1      ///< Buffer state : Full
#define CSL_BUF_STATE_EMPTY  2      ///< Buffer state : Empty

#define CSL_BUF_ID_INVALID   ((Uint32)-1) ///< Buffer ID : Invalid

/**
  \brief Buffer information
*/
typedef struct {

  Uint32  id;         ///< Buffer ID, USER SHOULD NOT CHANGE THIS VALUE
  Uint32  addr;       ///< Buffer Address
  Uint32  state;      ///< Buffer state
  Uint32  timestamp;  ///< Buffer timestamp, in msecs
  Uint32  count;      ///< BUffer frame count
  Uint32  width;      ///< BUffer width in pixels
  Uint32  height;      ///< BUffer height in lines
  
} CSL_BufInfo;

/**
  \brief Buffer initializatio parameters
*/
typedef struct {
  Uint32  bufAddr[CSL_BUF_NUM_MAX];   ///< List of buffer addresses
  Uint32  numBuf;                     ///< Number of buffer
  Uint32  curBuf;                     ///< Current/initial empty buffer
} CSL_BufInit;

#ifdef CSL_KERNEL_API
#include <csl_bufPriv.h>
#endif

/*@}*/

#endif
