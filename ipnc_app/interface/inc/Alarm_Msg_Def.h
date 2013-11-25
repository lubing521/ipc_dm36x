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
* @file Alarm_Msg_Def.h
* @brief Definition of the message ID to communicate with alarm manager.

* This file lists all the message ID that will be used to initialize alarm
*  message driver.
* @warning Be sure that one message ID can only be used by one process.
* @note When you add a process to use alarm message driver, please add a new
*  definition here.

* Add message ID may like following.
* @code
#define ALARM_NEW_MSG		MSG_TYPE_MSG5
* @endcode
*/
#ifndef _ALARM_MSG_DEF_H_
#define _ALARM_MSG_DEF_H_

/* MSG_TYPE_MSG1 is reserved for server */
/**
 * @addtogroup ALARM_MSG_DRV
* @{*/
#define ALARM_SYS_MSG           MSG_TYPE_MSG3 ///< message ID used in system server to communicate with alarm manager.
#define ALARM_AVSERVER_MSG      MSG_TYPE_MSG4 ///< message ID used in AV server to communicate with alarm manager.
/** @} */
#endif

