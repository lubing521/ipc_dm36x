/* ===========================================================================
* @path $(IPNCPATH)\sys_server\inc\
*
* @desc 
* .
* Copyright (c) Appro Photoelectron Inc.  2009
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
/**
* @file sd_config.h
* @brief Functions about SD Card control.
*/
#ifndef __SD_CONFIG_H__
#define __SD_CONFIG_H__

int CheckSDCard(int isInit, int bEnable);
int SdUnmount(unsigned char value);
int SdFormat();

#endif /*__SD_CONFIG_H__*/
