/* ===========================================================================
* @path $(IPNCPATH)\include
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
/**
* @file dm355_gio_util.h
* @brief Gio API
*/
#ifndef _DM355_GIO_UTIL_H_
#define _DM355_GIO_UTIL_H_
/**
* @ingroup UTILITY API
* @defgroup DM355_GIO_DRV GIO API

* This is the communication interface of GIO manager. 
* @{
*/
/**
* @brief GIO direction.
*/
typedef enum{
	GIO_INPUT, ///< GIO input.
	GIO_OUTPUT ///< GIO output.
}GIO_DIR;
/**
* @brief GIO return value.
*/
typedef enum{
	GIO_ERROR = -1, ///< GIO error.
	GIO_OK ///< GIO ok.
}GIO_RET;

int dm355_gio_getname( char *pName, int gio_id ); ///< Get GIO name.
int dm355_gio_read( int gio_id ); ///< Read GIO status.
int dm355_gio_write( int gio_id , int IsOn); ///< Set GIO status.
/** @} */
#endif
