/* ===========================================================================
* @file jpg_mng.h
*
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
* @file jpg_mng.h
* @brief JPEG manager.
* @defgroup JPG_MNG JPEG manager
* @{
*/
#ifndef __JPG_MNG_H__
#define __JPG_MNG_H__

//#define JPG_PATH "/tmp"

#define JPG_TO_SD	(1<<2) ///< Save JPEG files to SD card.
#define JPG_TO_SMTP	(1<<3) ///< Save JPEG files to SMTP server.
#define JPG_TO_FTP	(1<<4) ///< Save JPEG files to FTP server.

int JpgMngInit();
int JpgMngExit();
int JpgRun(int nDstFlg, int isJpgContinue);
int JpgStop(int isJpgContinue);
void SetJpegStartSerial(int nSerial);
#endif
/**@}
*/
