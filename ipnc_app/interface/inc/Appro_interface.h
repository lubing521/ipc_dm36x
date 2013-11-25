/* ===========================================================================
* @file Appro_interface.h
*
* @path $(IPNCPATH)/include/
*
* @desc Appro_interface common include file
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
/**
  \file Appro_interface.h

  \brief Appro_interface common include file
*/

#ifndef _APPRO_INTERFACE_
#define _APPRO_INTERFACE_

#if defined (__cplusplus)
extern "C" {
#endif
#include <Msg_Def.h>

#ifdef DEBUG
#define dbg(x...)		fprintf(stderr,x)
#else
#define dbg(x...)
#endif

/**
  \defgroup AV_INTERFACE Appro interface datatypes, data structures, functions

  The section defines some common datatypes, data structures, callbacks which are used by all parts of the Appro interface framework.
  System init APIs are also defined in this section.

  \section AV_COMMON_DIR_STRUCT Directory Structure

  \subsection AV_COMMON_INTERFACE Interface

  \code
  Appro_interface.h
  \endcode

  \subsection AV_COMMON_SOURCE Source

  \code
  Appro_interface.c
  \endcode


  @{
*/


/* return value */
#define RET_SUCCESS 		0		/**< error code : no error */
#define RET_NO_VALID_DATA   -1		/**< error code : request data not ready */
#define RET_NEW_MJPEG       -2		/**< error code : new jpeg(not use now) */
#define RET_INVALID_PRM     -100	/**< error code : invalid parameter */
#define RET_OVERWRTE        -102	/**< error code : request data loss */
#define RET_NO_MEM          -103	/**< error code : not enough memory for locking frame */
#define RET_ERROR_OP		-104	/**< error code : error operation */
#define RET_INVALID_COMMAND -200	/**< error code : invalid command */
#define RET_UNKNOWN_ERROR	-400	/**< error code : unknow error */

/* definition for field */
#define AV_OP_LOCK_MJPEG 				1 /**< Lock one MJPEG*/
#define AV_OP_BOOK_MJPEG 				2 /**< BOOK one MJPEG (not support now)*/
#define AV_OP_UNLOCK_MJPEG 				3 /**< Unlock one MJPEG */
#define AV_OP_LOCK_ULAW 				4 /**< Lock a period of sound */
#define AV_OP_UNLOCK_ULAW 				5 /**< Unlock a period of sound */
#define AV_OP_LOCK_MP4_VOL 				6 /**< Lock VOL of MPEG4 */
#define AV_OP_UNLOCK_MP4_VOL 			7 /**< Unlock VOL of MPEG4 */
#define AV_OP_LOCK_MP4 					8 /**< Lock one MPEG4 frame */
#define AV_OP_LOCK_MP4_IFRAME 			9 /**< Lock one MPEG4 I frame*/
#define AV_OP_UNLOCK_MP4 				10 /**< Unlock one MPEG4 frame */
#define AV_OP_GET_MJPEG_SERIAL 			11 /**< Get latest MJPEG serial number */
#define AV_OP_GET_MPEG4_SERIAL 			12 /**< Get latest MPEG4 serial number */
#define AV_OP_GET_ULAW_SERIAL 			13 /**< Get latest AUDIO serial number */
#define AV_OP_WAIT_NEW_MJPEG_SERIAL 	14 /**< Get new MJPEG serial number with wait (do not use now)*/
#define AV_OP_WAIT_NEW_MPEG4_SERIAL 	15 /**< Get new MPEG4 serial number with wait (do not use now)*/

#define AV_OP_LOCK_MP4_CIF_VOL 				16 /**< Lock VOL of MPEG4_2 */
#define AV_OP_UNLOCK_MP4_CIF_VOL 			17 /**< Unlock VOL of MPEG4_2 */
#define AV_OP_LOCK_MP4_CIF 					18 /**< Lock one MPEG4_2 frame */
#define AV_OP_LOCK_MP4_CIF_IFRAME 			19 /**< Lock one MPEG4_2 I frame*/
#define AV_OP_UNLOCK_MP4_CIF 				20 /**< Unlock one MPEG4_2 frame */
#define AV_OP_GET_MPEG4_CIF_SERIAL 			21 /**< Get latest MPEG4_2 serial number */
#define AV_OP_WAIT_NEW_MPEG4_CIF_SERIAL 	22 /**< Get new MPEG4_2 serial number with wait (do not use now)*/
#define AV_OP_WAIT_NEW_ULAW_SERIAL			23 /**< Get new AUDIO serial number with wait (do not use now)*/
#define AV_OP_GET_MEDIA_INFO				24 /**< Get media info*/

/* definition for AV_DATA.flags */
#define AV_FLAGS_MP4_I_FRAME 			0x00001 /**< First frame of GOP */
#define AV_FLAGS_MP4_LAST_FRAME 		0x00002 /**< Last frame of GOP */
/* definition for AV_DATA.frameType */
#define AV_FRAME_TYPE_I_FRAME	0x1	/**< frame type is I frame */
#define AV_FRAME_TYPE_P_FRAME	0x2	/**< frame type is P frame */

/**
 * @brief frame information data structure for GetAVData()
 */
typedef struct _av_data
{
	unsigned int serial;	/**< frame serial number */
	unsigned int size;		/**< frame size */
	unsigned int width;		/**< frame width */
	unsigned int height;	/**< frame height */
	unsigned int quality;	/**< frame quality */
	unsigned int flags;		/**< frame serial number */
	unsigned int frameType;	/**< either AV_FRAME_TYPE_I_FRAME  or AV_FRAME_TYPE_P_FRAME */
	unsigned int timestamp;	/**< get frame time stamp */
	unsigned int temporalId; /**< get frame temporalId */
	int			 ref_serial[FMT_MAX_NUM];	/**<  all frame serial record for reference */
	unsigned char * ptr;	/**<  pointer for data ouput */
} AV_DATA;

/* interface functions */
int func_get_mem( void *pdata );
int GetAVData( unsigned int field, int serial, AV_DATA * ptr );
int ApproInterfaceExit();

/* @} */

#if defined (__cplusplus)
}
#endif



#endif /*_APPRO_INTERFACE_*/

/**
   \page AV_INTERFACE_GUIDE    Usage Guide for GetAVData

   \section AV_INTERFACE_INTRO Introduction

   This API GetAVData() is designed for passing encoded audio and video between processes.And the communication between
   processes is baed on the POSIX message queue. In this usage guide, we will descript how to get the encoded audio or video
   data by this interface for your application to use.


   \section AV_INTERFACE_MODULES Module List

   The related data structure and function information are listed below.

   - \ref AV_INTERFACE
     \n API of the GetAVData()

   - \ref AV_MSG_DRV
     \n APIs of the message driver used by GetAVData()

   \section AV_INTERFACE_GUIDE Usage Guide


   Before we start to use the GetAVData, we have to initial the message driver and setup the
   memory for tranferring the encoded data.

   The example for the initialization is as below, and the Msg_id is defined at /include/Stream_Msg_Def.h.
   If your process is not define at Stream_Msg_Def.h, please add one in sequence by yourself.
   \code
	void Init_Interface(int Msg_id)
	{
		int qid;
		if(ApproDrvInit(Msg_id))
			exit(1);

		if(func_get_mem(&qid))
		{
			ApproDrvExit();
			exit(1);
		}
	}
   \endcode
   And before quit an process, please release the related resource aquired at initialization using following interface.
   \code
	void Clean_Interface(void)
	{
		ApproInterfaceExit();
	}
   \endcode

   \note Please do not initilize the interface more than once in one process.\n\n

   \subsection AV_INTERFACE_GUIDE_AUDIO Get Encoded Audio by GetAVData()

   Following is the example code for saving audio frames as file
   \code
	#include <ApproDrvMsg.h>
	#include <Appro_interface.h>
	#include <cmem.h>
	#include <Msg_Def.h>
	int SaveAudioFile( char *pFileName , int	save_cnt)
	{
		FILE *pfd = NULL;
		int error_code = 0;
		int	SerialBook = -1;
		int	SerialLock = -1;
		int	ret = 0;
		AV_DATA av_data;

		pfd = fopen(pFileName,"wb");
		if( pfd == NULL )
		{
			ERR("open file error!!");
			error_code = -1;
			goto RECORD_AU_QUIT;
		}


		if( fseek( pfd, 0, SEEK_SET ) < 0)
		{
			ERR("seek file error!!");
			error_code = -1;
			goto RECORD_AU_QUIT;
		}

		do
		{
			GetAVData(AV_OP_GET_ULAW_SERIAL, -1, &av_data );
		} while (av_data.serial < 0);

		SerialBook = av_data.serial;

		while( save_cnt > 0)
		{
			ret = GetAVData(AV_OP_LOCK_ULAW, SerialBook, &av_data );
			if (ret == RET_SUCCESS)
			{
				if( fwrite( av_data.ptr,av_data.size,1, pfd )  <= 0)
				{
					ERR("save file error!!");
					error_code = -1;
					goto RECORD_AU_QUIT;
				}
				if (SerialLock >= 0)
				{
					GetAVData(AV_OP_UNLOCK_ULAW, SerialLock, &av_data);
				}
				SerialLock = SerialBook;
				save_cnt--;
				SerialBook++;
			}
			else
			{
				GetAVData(AV_OP_GET_ULAW_SERIAL, -1, &av_data );
				if( SerialBook > av_data.serial )
				{
					// delay for waiting next frame
					usleep(5);
				}else{
					SerialBook = av_data.serial;
				}
			}
		}
		GetAVData(AV_OP_UNLOCK_ULAW, SerialLock, &av_data);
	RECORD_AU_QUIT:
		if( pfd )
		{
			fclose(pfd);
		}
		return error_code;
	}
   \endcode


   \subsection AV_INTERFACE_GUIDE_720P Get Encoded MPEG4 720P by GetAVData()
   Following is the example code for saving video frames(720P) as file
   \code
   	#include <ApproDrvMsg.h>
	#include <Appro_interface.h>
	#include <cmem.h>
	#include <Msg_Def.h>
	int SaveVideoFile( char *pFileName , int	save_cnt)
	{
		FILE *pfd = NULL;
		AV_DATA vol_data;
		int vType = 0;
		int error_code = 0;
		int	SerialBook = -1;
		int	SerialLock = -1;
		int	ret = 0;
		AV_DATA av_data;
		char mpeg4_header[] = {0x00, 0x00, 0x01, 0xb0, 0x01, 0x00, 0x00, 0x01, 0xb5, 0x09};
		static int mpeg4_field[7][2] =
		{
		{AV_OP_LOCK_MP4_VOL, 			AV_OP_LOCK_MP4_CIF_VOL},
		{AV_OP_UNLOCK_MP4_VOL, 			AV_OP_UNLOCK_MP4_CIF_VOL},
		{AV_OP_LOCK_MP4, 				AV_OP_LOCK_MP4_CIF},
		{AV_OP_LOCK_MP4_IFRAME, 		AV_OP_LOCK_MP4_CIF_IFRAME},
		{AV_OP_UNLOCK_MP4, 				AV_OP_UNLOCK_MP4_CIF},
		{AV_OP_GET_MPEG4_SERIAL, 		AV_OP_GET_MPEG4_CIF_SERIAL},
		{AV_OP_WAIT_NEW_MPEG4_SERIAL, 	AV_OP_WAIT_NEW_MPEG4_CIF_SERIAL}};

		#define LOCK_MP4_VOL			0
		#define UNLOCK_MP4_VOL 			1
		#define LOCK_MP4				2
		#define LOCK_MP4_IFRAM			3
		#define UNLOCK_MP4				4
		#define GET_MPEG4_SERIAL 		5
		#define WAIT_NEW_MPEG4_SERIAL 	6

		pfd = fopen(pFileName,"wb");
		if( pfd == NULL )
		{
			ERR("open file error!!");
			error_code = -1;
			goto RECORD_QUIT;
		}


		if( fseek( pfd, 0, SEEK_SET ) < 0)
		{
			ERR("seek file error!!");
			error_code = -1;
			goto RECORD_QUIT;
		}
		if( fwrite( mpeg4_header , sizeof(char), sizeof(mpeg4_header), pfd ) <= 0 )
		{
			ERR("save file error!!");
			error_code = -1;
			goto RECORD_QUIT;
		}

		if(GetAVData(mpeg4_field[LOCK_MP4_VOL][vType], -1, &vol_data) != RET_SUCCESS)
		{
			ERR("Error on Get Vol data\n");
			goto RECORD_QUIT;
		}

		if( fwrite( vol_data.ptr , sizeof(char), vol_data.size, pfd )  <= 0)
		{
			ERR("save file error!!");
			error_code = -1;
			goto RECORD_QUIT;
		}


		GetAVData(mpeg4_field[UNLOCK_MP4_VOL][vType], -1, &vol_data);
		do
		{
			GetAVData(mpeg4_field[GET_MPEG4_SERIAL][vType], -1, &av_data );
		} while (av_data.flags != AV_FLAGS_MP4_I_FRAME);
		SerialBook = av_data.serial;


		while( save_cnt > 0)
		{

			ret = GetAVData(mpeg4_field[LOCK_MP4][vType], SerialBook, &av_data );


			if (ret == RET_SUCCESS)
			{
				if( fwrite( av_data.ptr,av_data.size,1, pfd )  <= 0)
				{
					ERR("save file error!!");
					error_code = -1;
					goto RECORD_QUIT;
				}


				if (SerialLock >= 0)
				{
					GetAVData(mpeg4_field[UNLOCK_MP4][vType], SerialLock, &av_data);
				}

				SerialLock = SerialBook;

				save_cnt--;
				SerialBook++;
			}
			else if (ret == RET_NO_VALID_DATA) {

				// wait new frame
				usleep(5);

			}
			else {

				GetAVData(mpeg4_field[GET_MPEG4_SERIAL][vType], -1, &av_data );
				SerialBook = av_data.serial;

			}

		}

		GetAVData(mpeg4_field[UNLOCK_MP4][vType], SerialLock, &av_data);


	RECORD_QUIT:


		if( pfd )
		{
			fclose(pfd);
		}
		return error_code;
	}


   \endcode

   \subsection AV_INTERFACE_GUIDE_SIF Get Encoded MPEG4 CIF by GetAVData()
   Following is the example code for saving video frames(CIF) as file
   \code
   	#include <ApproDrvMsg.h>
	#include <Appro_interface.h>
	#include <cmem.h>
	#include <Msg_Def.h>
	int SaveVideoFile( char *pFileName , int	save_cnt)
	{
		FILE *pfd = NULL;
		AV_DATA vol_data;
		int vType = 1;
		int error_code = 0;
		int	SerialBook = -1;
		int	SerialLock = -1;
		int	ret = 0;
		AV_DATA av_data;
		char mpeg4_header[] = {0x00, 0x00, 0x01, 0xb0, 0x01, 0x00, 0x00, 0x01, 0xb5, 0x09};
		static int mpeg4_field[7][2] =
		{
		{AV_OP_LOCK_MP4_VOL, 			AV_OP_LOCK_MP4_CIF_VOL},
		{AV_OP_UNLOCK_MP4_VOL, 			AV_OP_UNLOCK_MP4_CIF_VOL},
		{AV_OP_LOCK_MP4, 				AV_OP_LOCK_MP4_CIF},
		{AV_OP_LOCK_MP4_IFRAME, 		AV_OP_LOCK_MP4_CIF_IFRAME},
		{AV_OP_UNLOCK_MP4, 				AV_OP_UNLOCK_MP4_CIF},
		{AV_OP_GET_MPEG4_SERIAL, 		AV_OP_GET_MPEG4_CIF_SERIAL},
		{AV_OP_WAIT_NEW_MPEG4_SERIAL, 	AV_OP_WAIT_NEW_MPEG4_CIF_SERIAL}};

		#define LOCK_MP4_VOL			0
		#define UNLOCK_MP4_VOL 			1
		#define LOCK_MP4				2
		#define LOCK_MP4_IFRAM			3
		#define UNLOCK_MP4				4
		#define GET_MPEG4_SERIAL 		5
		#define WAIT_NEW_MPEG4_SERIAL 	6

		pfd = fopen(pFileName,"wb");
		if( pfd == NULL )
		{
			ERR("open file error!!");
			error_code = -1;
			goto RECORD_QUIT;
		}


		if( fseek( pfd, 0, SEEK_SET ) < 0)
		{
			ERR("seek file error!!");
			error_code = -1;
			goto RECORD_QUIT;
		}
		if( fwrite( mpeg4_header , sizeof(char), sizeof(mpeg4_header), pfd ) <= 0 )
		{
			ERR("save file error!!");
			error_code = -1;
			goto RECORD_QUIT;
		}

		if(GetAVData(mpeg4_field[LOCK_MP4_VOL][vType], -1, &vol_data) != RET_SUCCESS)
		{
			ERR("Error on Get Vol data\n");
			goto RECORD_QUIT;
		}

		if( fwrite( vol_data.ptr , sizeof(char), vol_data.size, pfd )  <= 0)
		{
			ERR("save file error!!");
			error_code = -1;
			goto RECORD_QUIT;
		}


		GetAVData(mpeg4_field[UNLOCK_MP4_VOL][vType], -1, &vol_data);
		do
		{
			GetAVData(mpeg4_field[GET_MPEG4_SERIAL][vType], -1, &av_data );
		} while (av_data.flags != AV_FLAGS_MP4_I_FRAME);
		SerialBook = av_data.serial;


		while( save_cnt > 0)
		{

			ret = GetAVData(mpeg4_field[LOCK_MP4][vType], SerialBook, &av_data );


			if (ret == RET_SUCCESS)
			{
				if( fwrite( av_data.ptr,av_data.size,1, pfd )  <= 0)
				{
					ERR("save file error!!");
					error_code = -1;
					goto RECORD_QUIT;
				}


				if (SerialLock >= 0)
				{
					GetAVData(mpeg4_field[UNLOCK_MP4][vType], SerialLock, &av_data);
				}

				SerialLock = SerialBook;

				save_cnt--;
				SerialBook++;
			}
			else if (ret == RET_NO_VALID_DATA) {

				// wait new frame
				usleep(5);

			}
			else {

				GetAVData(mpeg4_field[GET_MPEG4_SERIAL][vType], -1, &av_data );
				SerialBook = av_data.serial;

			}

		}

		GetAVData(mpeg4_field[UNLOCK_MP4][vType], SerialLock, &av_data);


	RECORD_QUIT:


		if( pfd )
		{
			fclose(pfd);
		}
		return error_code;
	}


   \endcode

   \subsection AV_INTERFACE_GUIDE_JPEG Get Encoded JPEG by GetAVData()

   Following is the example code for saving JPEG as file
   \code
	#include <ApproDrvMsg.h>
	#include <Appro_interface.h>
	#include <cmem.h>
	#include <Msg_Def.h>
	int SaveAudioFile( char *pFileName )
	{
		FILE *pfd = NULL;
		int error_code = 0;
		int	SerialBook = -1;
		int	SerialLock = -1;
		int	ret = 0;
		int	save_cnt = 1;
		AV_DATA av_data;

		pfd = fopen(pFileName,"wb");
		if( pfd == NULL )
		{
			ERR("open file error!!");
			error_code = -1;
			goto RECORD_QUIT;
		}


		if( fseek( pfd, 0, SEEK_SET ) < 0)
		{
			ERR("seek file error!!");
			error_code = -1;
			goto RECORD_QUIT;
		}

		do
		{
			GetAVData(AV_OP_GET_MJPEG_SERIAL, -1, &av_data );
		} while (av_data.serial < 0);

		SerialBook = av_data.serial;

		while( save_cnt > 0)
		{
			ret = GetAVData(AV_OP_LOCK_MJPEG, SerialBook, &av_data );
			if (ret == RET_SUCCESS)
			{
				if( fwrite( av_data.ptr,av_data.size,1, pfd )  <= 0)
				{
					ERR("save file error!!");
					error_code = -1;
					goto RECORD_QUIT;
				}
				if (SerialLock >= 0)
				{
					GetAVData(AV_OP_UNLOCK_MJPEG, SerialLock, &av_data);
				}
				SerialLock = SerialBook;
				save_cnt--;
				SerialBook++;
			}
			else
			{
				GetAVData(AV_OP_GET_MJPEG_SERIAL, -1, &av_data );
				if( SerialBook > av_data.serial )
				{
					// delay for waiting next frame
					usleep(5);
				}else{
					SerialBook = av_data.serial;
				}
			}
		}
		GetAVData(AV_OP_UNLOCK_MJPEG, SerialLock, &av_data);
	RECORD_QUIT:
		if( pfd )
		{
			fclose(pfd);
		}
		return error_code;
	}
   \endcode
*/

/**
   \page AV_FEATURE_INTERFACE   Usage Guide for setting video feature

   \section AV_FEATURE_INTRO Introduction

   In this usage guide, we will show some examples for user to set up video fearture between processes.
   Here the feature means video codec setting(bit rate, rate control,...) , image quality tunning
   ( brightness, sharpness,...) and auto exposure/white balance control setting( enable/disable, day/night,...).
   For user to apply needed effects before video ouput.


   \section AV_FEATURE_MODULES Module List

   The related data structure and function information are listed below.

   - \ref AV_MSG_DRV
     \n APIs of the message driver used by GetAVData() and video feature setup.

   \section AV_FEATURE_GUIDE Usage Guide
   Before we start to setup the video feature in the application, we have to initial the message driver and setup the
   memory for tranferring the encoded data.

   The example for the initialization is as below, and the Msg_id is defined at /include/Stream_Msg_Def.h.
   If your process is not define at Stream_Msg_Def.h, please add one in sequence by yourself.
   \code
	void Init_Interface(int Msg_id)
	{
		int qid;
		if(ApproDrvInit(Msg_id))
			exit(1);

	}
   \endcode

   And before quit an process, please release the related resource aquired at initialization using following interface.
   \code
	void Clean_Interface(void)
	{
		ApproDrvExit();
	}
   \endcode
   \note Please do not initilize the interface more than once in one process.\n\n

   \section AV_FEATURE_LIST Feature and related function List

   	- Bit rate setting for video1 (mpeg4 720P/VGA)
	- Bit rate setting for video2(mpeg4 CIF) \n
		SetEncBitrate() \n\n
	- Quality setting for JPEG(VGA)\n
		SetJpgQuality()\n\n
	- Frame rate control for video1 (mpeg4 720P/VGA)
	- Frame rate control for video2(mpeg4 CIF)
	- Frame rate control for video3(JPEG VGA)\n
		SetEncFramerate()\n\n
	- Back light control\n
		SetBacklight()\n\n
	- Saturation setting\n
		SetSaturation()\n\n
	- Indor/Outdoor setting\n
		SetWhiteBalance()\n\n
	- Day/Night setting\n
		SetDayNight()\n\n
	- Brightness setting\n
		SetBrightness()\n\n
	- Contrast setting\n
		SetContrast()\n\n
	- Sharpness setting\n
		SetSharpness()\n
	- 50/60 Hz setting\n
		SetTvSystem()\n\n
	- Sensor binning/skipping function setting\n
		SetBinningSkip()\n\n
	- time stamp enable/disable\n
		SetTStamp()\n\n
	- Pan/Tile/Zoom control\n
		SetPtz()\n\n

   \section AV_FEATURE_EXAM  example

	Here is an example to enable timestamp and set sensor as binning mode in application for reference.

	\code
	#include <ApproDrvMsg.h>
	#include <Appro_interface.h>
	#include <cmem.h>
	#include <Msg_Def.h>
	void SetupVideoFeature(void)
	{
		Init_Interface(SYS_MSG_TYPE);

		SetBinningSkip(0);
		SetTStamp(1);

		Clean_Interface();
	}
	\endcode

*/
