/* ===========================================================================
* @path $(IPNCPATH)\sys_server\inc\
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
* @file avi_mng.h
* @brief AVI manager.
* @defgroup AVI_MNG AVI manager

* This module takes care of recording AVI file and ask file dispatcher to send
*  the AVI file when recording is done.
* @{
*/
#ifndef __AVI_MNG_H__
#define __AVI_MNG_H__
#include <Msg_Def.h>

#define AVI_PATH "/tmp"

#define AVI_TO_SD	(1<<3) ///< Save AVI files to SD card.
#define AVI_TO_SMTP	(1<<4) ///< Save AVI files to SMTP server.
#define AVI_TO_FTP	(1<<5) ///< Save AVI files to FTP server.

int AviMngInit();
int AviMngExit();
int AviRun(int isNoStop, int nDstFlg);
int AviStop();
#endif
/**@}
 */
