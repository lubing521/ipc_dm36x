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
* @file File_Msg_Def.h
* @brief Definition of the message ID to communicate with file manager.

* This file lists all the message ID that will be used to initialize file
*  message driver.
* @warning Be sure that one message ID can only be used by one process.
* @note When you add a process to use file message driver, please add a new
*  definition here. And MSG_TYPE_MSG10 is the maximum.

* Add message ID may like following.
* @code
#define FILE_NEW_MSG		MSG_TYPE_MSG6
* @endcode
*/
#ifndef _FILE_MSG_DEF_H_
#define _FILE_MSG_DEF_H_
/* MSG_TYPE_MSG1 is reserved for server */
/**
 * @addtogroup FILE_MSG_DRV
* @{*/
#define FILE_SYS_MSG		MSG_TYPE_MSG2 ///< message ID used in system server to communicate with file manager.
#define FILE_BOA_MSG		MSG_TYPE_MSG3 ///< message ID used in BOA to communicate with file manager.
#define FILE_STREAM_MSG		MSG_TYPE_MSG4 ///< message ID used in stream server to communicate with file manager.
#define FILE_MOTION_MSG		MSG_TYPE_MSG5 ///< message ID used by motion detection to communicate with file manager.
/** @} */
#endif

