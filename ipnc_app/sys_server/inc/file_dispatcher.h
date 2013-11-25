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
* @file file_dispatcher.h
* @brief File dispatcher.
* @defgroup FILE_DISPATCHER File dispatcher

* This module will make all AVI and JPEG files transfer to correct place.
* @{
*/
#ifndef __FILE_DISPATCHER_H__
#define __FILE_DISPATCHER_H__

#define FILE_DISP_THR_LIMIT	2

#define FILE_DISPATCH_TO_SD			(1 << 1) ///< Transfer a file to SD card.
#define FILE_DISPATCH_TO_FTP			(1 << 2) ///< Transfer a file to FTP server.
#define FILE_DISPATCH_TO_SMTP			(1 << 3) ///< Transfer a file to SMTP server.
#define JPG_DISPATCH_TO_SD			(1 << 5) ///< Transfer a JPEG file to SD card.
#define JPG_DISPATCH_TO_FTP		(1 << 6) ///< Transfer a JPEG file to FTP server.
#define JPG_DISPATCH_TO_SMTP		(1 << 7) ///< Transfer a JPEG file to SMTP server.
/**
* @brief File type for file dispatcher to transfer.
*/
typedef enum{
	ALL_TYPE, ///< ALL file format
	JPG_TYPE, ///<JPEG file format only
	TYPE_MAX ///< File type number
} FileType;

int FileDispatcherInit();
int FileDispatcherExit();
void SetFileDispState(int nSetFlg);
int DoFileDispatch(char *strFilePath, int nDestination);
#if 0
int DoJpegDispatch(char *strFilePath, int nDestination);
#endif
#endif
/** @}
*/
