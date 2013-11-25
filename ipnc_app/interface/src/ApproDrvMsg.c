/* ===========================================================================
* @file ApproDrvMsg.c
*
* @path $(IPNCPATH)/util/
*
* @desc Message driver for Appro_interface
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */

#include <stdio.h>
#include <string.h>
#include <ApproDrvMsg.h>
#include <cmem.h>
#include "sem_util.h"

static int gProcId = MSG_TYPE_MSG1;
static int qid;
static SemHandl_t hndlApproDrvSem = NULL;

#define PROC_MSG_ID	gProcId

#define ENABLE_CMEM	(1)

/**
 * @brief set the message type id of the process
 *
 *
 * @param   proc_id    message type id of the proceess, defined at ipnc_app/include/Stream_Msg_Def.h
 *
 *
 *
 */
void ApproDrv_SetProcId(int proc_id)
{
	if(proc_id < MSG_TYPE_MSG1 || proc_id >= MSG_TYPE_END){
		gProcId = MSG_TYPE_MSG1;
		return;
	} else
		gProcId = proc_id;
}

/**
 * @brief set the message queue id of the process
 *
 *
 * @param   iqid    message queue id of the proceess
 *
 *
 *
 */
void ApproDrv_SaveQid(int iqid)
{
	qid = iqid;
}

int Testflag = 0;
/**
 * @brief Initiliztion of the message driver for Appro interface
 *
 *
 * @param   proc_id    message type id of the proceess, defined at ipnc_app/include/Stream_Msg_Def.h
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int ApproDrvInit(int proc_id)
{
	if(proc_id < MSG_TYPE_MSG1 || proc_id > 20){
		gProcId = MSG_TYPE_MSG1;
		return -1;
	}
	gProcId = proc_id;
	fprintf(stderr, "%s: %d\n", __func__, proc_id);
#if ENABLE_CMEM
	/* CMEM only one init is allowed in each process */
	if(Testflag==0)
	{
		if(CMEM_init() < 0){
			gProcId = MSG_TYPE_MSG1;
			return -1;
		}
	}
#endif
	Testflag = 1;;
	if(hndlApproDrvSem == NULL)
		hndlApproDrvSem = MakeSem();
	if(hndlApproDrvSem == NULL){
#if ENABLE_CMEM
		CMEM_exit();
#endif
		gProcId = MSG_TYPE_MSG1;
		return -1;
	}
	if((qid=Msg_Init(MSG_KEY)) < 0){
		DestroySem(hndlApproDrvSem);
		hndlApproDrvSem = NULL;
#if ENABLE_CMEM
		CMEM_exit();
#endif
		gProcId = MSG_TYPE_MSG1;
		return -1;
	}
	return 0;
}
/**
 * @brief Resource releasing of the message driver for Appro interface
 *
 *
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int ApproDrvExit()
{
	fprintf(stderr, "%s: %d\n", __func__, gProcId);
	gProcId = MSG_TYPE_MSG1;
	DestroySem(hndlApproDrvSem);
	hndlApproDrvSem = NULL;
#if ENABLE_CMEM

	if( Testflag != 0 )
	{
	 	CMEM_exit();
	}
	Testflag = 0;;
	return 0;
#else
	return 0;
#endif
}

/**
 * @brief  Message driver for getting current frame information
 *
 *
* @param   fmt    stucture of frame information, defined at Msg_def.h
 *
 *
 *
 */
FrameInfo_t GetCurrentFrame(FrameFormat_t fmt)
{
	MSG_BUF msgbuf;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_GET_CUR_FRAME;
	msgbuf.frame_info.format = fmt;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	if(msgbuf.ret != 0)
		msgbuf.frame_info.serial_no = -1;
	return msgbuf.frame_info;
}

/**
 * @brief  Message driver for waiting newframe (Do not use now)
 *
 *
* @param   fmt    stucture pointer of frame information, defined at Msg_def.h
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
FrameInfo_t WaitNewFrame(FrameFormat_t fmt)
{
	MSG_BUF msgbuf;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_GET_NEW_FRAME;
	msgbuf.frame_info.format = fmt;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	if(msgbuf.ret != 0)
		msgbuf.frame_info.serial_no = -1;
	return msgbuf.frame_info;
}
/**
 * @brief  Message driver for getting memory information
 *
 *
 * @return MSG_MEM
 *
 *
 */
MSG_MEM GetPhyMem()
{
	MSG_BUF msgbuf;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_GET_MEM;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	if(msgbuf.ret != 0)
		msgbuf.mem_info.addr = 0;
	return msgbuf.mem_info;
}

/**
 * @brief  Message driver for sending leave message (Do not use now)
 *
 *
 *
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
void SendQuitCmd()
{
	MSG_BUF msgbuf;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = 0;
	msgbuf.cmd = MSG_CMD_QUIT;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	SemRelease(hndlApproDrvSem);

}
/**
 * @brief  Message driver for unLock frame
 *
 *
 * @param   pFrame    stucture pointer of frame information, defined at Msg_def.h
 *
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
void UnlockFrame(FrameInfo_t *pFrame)
{
	MSG_BUF msgbuf;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = 0;
	msgbuf.cmd = MSG_CMD_UNLOCK_FRAME;
	msgbuf.frame_info.format = pFrame->format;
	msgbuf.frame_info.serial_no = pFrame->serial_no;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	SemRelease(hndlApproDrvSem);
	return ;
}
/**
 * @brief  Message driver for Lock frame
 *
 *
 * @param   pFrame    stucture pointer of frame information, defined at Msg_def.h
 *
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int LockFrame(FrameInfo_t *pFrame)
{
	int cnt = 0;
	MSG_BUF msgbuf;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_LOCK_FRAME;
	msgbuf.frame_info.format = pFrame->format;
	msgbuf.frame_info.serial_no = pFrame->serial_no;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	pFrame->size = msgbuf.frame_info.size;
	pFrame->width = msgbuf.frame_info.width;
	pFrame->height = msgbuf.frame_info.height;
	pFrame->offset = msgbuf.frame_info.offset;
	pFrame->quality = msgbuf.frame_info.quality;
	pFrame->flags = msgbuf.frame_info.flags;
	pFrame->frameType = msgbuf.frame_info.frameType;
	pFrame->timestamp = msgbuf.frame_info.timestamp;
	pFrame->temporalId = msgbuf.frame_info.temporalId;
	for (cnt = 0; cnt < FMT_MAX_NUM; cnt++ )
	{
		pFrame->ref_serial[cnt] = msgbuf.frame_info.ref_serial[cnt];
	}

	return msgbuf.ret;
}
/**
 * @brief  Message driver for getting MPEG4 VOL data
 *
 *
 * @param   pVolBuf   ouput buffer for getting MPEG4 VOL data
 *
 * @param   fmt_type    frame type ID : FMT_MJPEG, FMT_MPEG4 , FMT_MPEG4_EXT, FMT_AUDIO, defined at Msg_def.h
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int GetVolInfo(FrameInfo_t *pFrame, int fmt_type)
{
	MSG_BUF msgbuf;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_GET_VOL;
	msgbuf.frame_info.format = fmt_type;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);

	pFrame->offset 	= msgbuf.frame_info.offset;
	pFrame->size 	= msgbuf.frame_info.size;

	return msgbuf.frame_info.size;
}

/**
 * @brief  Message driver for getting Media info
 *
 *
 * @param   pVolBuf   ouput buffer for getting Media info
 *
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int GetMediaInfo(FrameInfo_t *pFrame)
{
	MSG_BUF msgbuf;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_GET_MEDIA_INFO;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);

	pFrame->format 	= msgbuf.frame_info.format;

	return msgbuf.frame_info.format;
}


/**
 * @brief  Message driver for Lock MPEG4 I-frame
 *
 *
 * @param   pFrame    stucture pointer of frame information, defined at Msg_def.h
 *
 * @param   fmt_type    frame type ID : FMT_MJPEG, FMT_MPEG4 , FMT_MPEG4_EXT, FMT_AUDIO, defined at Msg_def.h
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int LockMpeg4IFrame(FrameInfo_t *pFrame,int fmt_type)
{
	int cnt = 0;
	MSG_BUF msgbuf;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_GET_AND_LOCK_IFRAME;
	msgbuf.frame_info.format = fmt_type;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	if(msgbuf.ret == -1)
		msgbuf.frame_info.serial_no = -1;
	else if(msgbuf.ret != 0)
		msgbuf.frame_info.serial_no = -2;
	pFrame->serial_no = msgbuf.frame_info.serial_no;
	pFrame->size = msgbuf.frame_info.size;
	pFrame->width = msgbuf.frame_info.width;
	pFrame->height = msgbuf.frame_info.height;
	pFrame->offset = msgbuf.frame_info.offset;
	pFrame->quality = msgbuf.frame_info.quality;
	pFrame->flags = msgbuf.frame_info.flags;
	pFrame->frameType = msgbuf.frame_info.frameType;
	pFrame->timestamp = msgbuf.frame_info.timestamp;
	pFrame->temporalId = msgbuf.frame_info.temporalId;
	for (cnt = 0; cnt < FMT_MAX_NUM; cnt++ )
	{
		pFrame->ref_serial[cnt] = msgbuf.frame_info.ref_serial[cnt];
	}
	return msgbuf.frame_info.serial_no;
}

/**
 * @brief  day / night mode setting for UI
 *
 *
 * @param   nDayNight    0 : night mode 	1: day mode
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetDayNight(unsigned char nDayNight)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_DAY_NIGHT;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nDayNight;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

/**
 * @brief  Motion Enable mode setting for UI
 *
 *
 * @param   enableVal  0: Enable		1:Disable
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int ApproSetMotion(ApproMotionPrm* pMotionPrm)
{
	MSG_BUF msgbuf;
	void* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_MOTION;
	ptr = (void*)&msgbuf.mem_info;
	memcpy(ptr, pMotionPrm, sizeof(ApproMotionPrm));
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SendFaceDetectMsg(FaceDetectParam* faceParam)
{
	MSG_BUF msgbuf;
	void* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_FDETECT;
	ptr = (void*)&msgbuf.mem_info;
	memcpy(ptr, faceParam, sizeof(FaceDetectParam));
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;

}
int SetDateTimeFormat(DateTimePrm* datetimeParam)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_DATETIMEPRM;
	ptr = (unsigned char*)&msgbuf.mem_info;
	memcpy(ptr, datetimeParam, sizeof(DateTimePrm));
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetOSDPrmMsg(int id, OSDPrm* osdprm)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;

	if(id==0)
		msgbuf.cmd = MSG_CMD_SET_OSDPRM1;
	else if(id==1)
		msgbuf.cmd = MSG_CMD_SET_OSDPRM2;
	else if(id==2)
		msgbuf.cmd = MSG_CMD_SET_OSDPRM3;

	ptr = (unsigned char*)&msgbuf.mem_info;
	memcpy(ptr, osdprm, sizeof(OSDPrm));
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetCodecROIMsg(int id, CodecROIPrm* codecROIPrm)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;

	if(id==0)
		msgbuf.cmd = MSG_CMD_SET_ROIPRM1;
	else if(id==1)
		msgbuf.cmd = MSG_CMD_SET_ROIPRM2;
	else if(id==2)
		msgbuf.cmd = MSG_CMD_SET_ROIPRM3;

	ptr = (unsigned char*)&msgbuf.mem_info;
	memcpy(ptr, codecROIPrm, sizeof(CodecROIPrm));
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetCodecAdvPrmMsg(int id, CodecAdvPrm* codecAdvPrm)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;

	if(id==0)
		msgbuf.cmd = MSG_CMD_SET_ADVCODECPRM1;
	else if(id==1)
		msgbuf.cmd = MSG_CMD_SET_ADVCODECPRM2;
	else if(id==2)
		msgbuf.cmd = MSG_CMD_SET_ADVCODECPRM3;

	ptr = (unsigned char*)&msgbuf.mem_info;
	memcpy(ptr, codecAdvPrm, sizeof(CodecAdvPrm));
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

/**
 * @brief  indoor / outdoor mode setting for UI
 *
 *
 * @param   nValue    0 : indoor	1: outdoor	2: Auto
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetWhiteBalance(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_WHITE_BALANCE;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}
int SetDisplayValue(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_LOCALDISPLAY;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetOSDEnable(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_OSDENABLE;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

/**
 * @brief  back light setting for UI
 *
 *
 * @param   nValue    	=128: median \n
*				>128: hight \n
*				<128: low \n
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetBacklight(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_BACKLIGHT;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}
/**
 * @brief  brightness setting for UI
 *
 *
 * @param   nValue    	=128: median \n
*				>128: hight \n
*				<128: low \n
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetBrightness(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_BRIGHTNESS;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}
/**
 * @brief  contrast setting for UI
 *
 *
 * @param   nValue    	=128: median \n
*				>128: hight \n
*				<128: low \n
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetContrast(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_CONTRAST;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

/**
 * @brief  Saturation setting for UI
 *
 *
 * @param   nValue    	=128: median \n
*				>128: hight \n
*				<128: low \n
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetSaturation(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_SATURATION;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}
/**
 * @brief  Sharpness setting for UI
 *
 *
 * @param   nValue    	=128: median \n
*				>128: hight \n
*				<128: low \n
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetSharpness(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_SHARPNESS;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

/**
 * @brief  Bitrate setting for UI
 *
 *
 * @param   stream    	0: stream1 720P \n
*				1: stream2 SIF \n
*
 * @param   nValue    	64~8000
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetEncBitrate(unsigned char stream, int nValue)
{
	MSG_BUF msgbuf;
	int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	if (stream == 0)
		msgbuf.cmd = MSG_CMD_SET_BITRATE1;
	else
		msgbuf.cmd = MSG_CMD_SET_BITRATE2;
	ptr = (int*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

/**
 * @brief  frame rate setting for UI
 *
 *
 * @param   stream    	0: stream1 MPEG4 720P \n
*				1: stream2 MPEG4 SIF \n
*				2: stream3 JPG VGA
*
 * @param   nValue    	7500~30000 ( 7.5fps ~30fps )
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetStreamFramerate(unsigned char stream, unsigned int nValue)
{
	MSG_BUF msgbuf;
	unsigned int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	if (stream == 0)
		msgbuf.cmd = MSG_CMD_SET_FRAMERATE1;
	else if (stream == 1)
		msgbuf.cmd = MSG_CMD_SET_FRAMERATE2;
	else
		msgbuf.cmd = MSG_CMD_SET_FRAMERATE3;

	ptr = (unsigned int*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

/**
 * @brief  frame rate setting for UI
 *
 *
 * @param   nValue    	7500~30000 ( 7.5fps ~30fps )
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetJpgQuality(int nValue)
{
	MSG_BUF msgbuf;
	int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_JPEG_QUALITY;
	ptr = (int*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetMirr(int nValue)
{
	MSG_BUF msgbuf;
	int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_MIRROR;
	ptr = (int*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetOSDWindow(int nValue)
{
	MSG_BUF msgbuf;
	int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_OSDWINDOW;
	ptr = (int*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

/**
 * @brief  2A enable/disable setting for UI
 *
 *
 * @param   nValue    	0: disable \n
 *				1: enable
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetImage2AType(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_TYPE_2A;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int Set2APriority(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_2A_PRIORITY;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

/**
 * @brief  TV setting for UI
 *
 *
 * @param   nValue    	0: NTSC \n
 *				1: PAL
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetTvSystem(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_TV_SYSTEM;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

/**
 * @brief  PTZ  setting  for UI
 *
 *
 * @param   nValue    		0: PTZ_ZOOM_IN \n
*					1: PTZ_ZOOM_OUT \n
*					2: PTZ_PAN_UP \n
*					3: PTZ_PAN_DOWN \n
*					4: PTZ_PAN_LEFT \n
*					5: PTZ_PAN_RIGHT \n
*					6: PTZ_INIT_4X\n
*					7: PTZ_ZOOM_EMPTY \n
*					8: PTZ_ZOOM_RESET
 *
 *@note This function only work at sensor output is VGA
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetPtz(int nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_PTZ;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

/**
 * @brief  BinningSkip  setting  for UI
 *
 *
 * @param   nValue    		0: Binning \n
*					1: Skipping \n
*
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetBinningSkip(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_BINNING_SKIP;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

/**
 * @brief  time stamp  setting  for UI
 *
 *
 * @param   nValue    		0: timestamp open \n
*					1: timestamp close \n
*
 *
 * @return 0 is ok and -1 is error
 *
 *
 */
int SetTStamp(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_TSTAMP;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetVNFStatus(unsigned char stream, unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;

	if (stream == 0)
		msgbuf.cmd = MSG_CMD_SET_VNFSTATUS1;
	else if (stream == 1)
		msgbuf.cmd = MSG_CMD_SET_VNFSTATUS2;
	else if (stream == 2)
		msgbuf.cmd = MSG_CMD_SET_VNFSTATUS3;

	ptr = (unsigned int*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetROIConfig(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_ROICFG;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetAvOsdTextEnable(int enable)
{
	MSG_BUF msgbuf;
	int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;

	msgbuf.cmd = MSG_CMD_SET_OSDTEXT_EN;

	ptr = (int*)&msgbuf.mem_info;
	*ptr = enable;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetHistEnable(int nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_HIST_EN;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetAACBitrate(int nValue)
{
	MSG_BUF msgbuf;
	int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_AAC_BITRATE;
	ptr = (int*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetAvOsdLogoEnable(int enable)
{
	MSG_BUF msgbuf;
	int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;

	msgbuf.cmd = MSG_CMD_SET_OSDLOGO_EN;

	ptr = (int*)&msgbuf.mem_info;
	*ptr = enable;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetAvOsdText(char* strText, int nLength)
{
	MSG_BUF msgbuf;
	int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;

	msgbuf.cmd = MSG_CMD_SET_OSDTEXT;

	ptr = (int*)&msgbuf.mem_info;
	*ptr = nLength;
	memcpy(++ptr, strText, nLength);
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetClipSnapName(char* strText, int nLength)
{
	MSG_BUF msgbuf;
	int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;

	msgbuf.cmd = MSG_CMD_SET_CLICKNAME;

	ptr = (int*)&msgbuf.mem_info;
	*ptr = nLength;
	memcpy(++ptr, strText, nLength);
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetClipSnapLocation(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_CLICKSTORAGE;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SendForceIFrameMsg(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_IFRAME;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetAudioAlarmValue(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_AUDIO_ALARM_LEVEL;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetAudioAlarmFlag(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_AUDIO_ALARM_FLAG;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetDynRangePrmMsg(DynRangePrm* dynRangePrm)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_DYNRANGE;
	ptr = (unsigned char*)&msgbuf.mem_info;
	memcpy(ptr, dynRangePrm, sizeof(DynRangePrm));
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int ApproPollingMsg()
{
	MSG_BUF msgbuf;
	int ret;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_POLLING;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	ret = msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, IPC_NOWAIT);
	if(ret >= 0){
		while(msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, IPC_NOWAIT) >= 0);
		ret = msgbuf.ret;
	}
	SemRelease(hndlApproDrvSem);
	return ret;
}

int SetLinWdr(unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_LINEAR_WAR;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int SetFlickDec(int nValue)
{
	MSG_BUF msgbuf;
	int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_FLICKER_DETECT;
	ptr = (int *)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return *ptr;
}



/////zeng 2013-09-16

int Appro_SetTurnColor(int nEnable)
{
	MSG_BUF msgbuf;
	int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_TURNCOLOR;
	ptr = (int *)&msgbuf.mem_info;
	*ptr = nEnable;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return *ptr;	
}

int Appro_GetTurnColor()
{
	MSG_BUF msgbuf;
	int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_GET_TURNCOLOR;
	ptr = (int *)&msgbuf.mem_info;

	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	
	return *ptr;	
}


//////zeng 2012-06-28

int Appro_SetEncRateControl(int nChannel, int nValue)
{
	MSG_BUF msgbuf;
	int* ptr;	
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	
	if(0 == nChannel) msgbuf.cmd = MSG_CMD_SET_CVBR_CHN1;
	if(1 == nChannel) msgbuf.cmd = MSG_CMD_SET_CVBR_CHN2;
	if(2 == nChannel) msgbuf.cmd = MSG_CMD_SET_CVBR_CHN3;
		
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;	
}


int Appro_SetCapturePrm(int nChannel ,MSGVCAPCFG* pCaptureCfg)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;	
	if(pCaptureCfg==NULL || pCaptureCfg->nCapChn != nChannel)
	{
		return 0;
	}
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_VCAP_PARAM;
	ptr = (unsigned char*)&msgbuf.mem_info;
	memcpy(ptr, pCaptureCfg, sizeof(MSGVCAPCFG));
	
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;	
}

int Appro_GetCapturePrm(int nChannel ,MSGVCAPCFG* pVcapCfg)
{
	MSGVCAPCFG* pRecvCfg = NULL;
	MSGVCAPCFG VcapCfg;
	MSG_BUF msgbuf;
	unsigned char* ptr;	
	if(pVcapCfg==NULL)
	{
		return 0;
	}

	memset(&VcapCfg , 0 , sizeof(MSGVCAPCFG));
	VcapCfg.nCapChn = nChannel;

	
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_GET_VCAP_PARAM;
	ptr = (unsigned char*)&msgbuf.mem_info;
	memcpy(ptr, &VcapCfg, sizeof(MSGVCAPCFG));
	
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);

	if(msgbuf.ret == 0)
	{
		return msgbuf.ret;
	}
	pRecvCfg =(MSGVCAPCFG*)&msgbuf.mem_info;
	if(pRecvCfg == NULL)
	{
		return msgbuf.ret;
	}
	memcpy( pVcapCfg , pRecvCfg , sizeof(MSGVCAPCFG));
	return msgbuf.ret;
}



int Appro_SetEncodePrm(int nChannel ,MSGVENCCFG* pVencCfg)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;	
	if(pVencCfg==NULL || pVencCfg->captureStreamId != nChannel)
	{
		return 0;
	}
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_VENC_PARAM;
	ptr = (unsigned char*)&msgbuf.mem_info;
	memcpy(ptr, pVencCfg, sizeof(MSGVENCCFG));
	
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int Appro_GetEncodePrm(int nChannel ,MSGVENCCFG* pVencCfg)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;	
	if(pVencCfg==NULL)
	{
		return 0;
	}
	MSGVENCCFG* pRecvCfg = NULL;
	MSGVENCCFG VencCfg;
	memset(&VencCfg , 0 , sizeof(MSGVENCCFG));
	VencCfg.captureStreamId = nChannel;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_GET_VENC_PARAM;
	ptr = (unsigned char*)&msgbuf.mem_info;
	memcpy(ptr, &VencCfg, sizeof(MSGVENCCFG));
	
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	if(msgbuf.ret == 0)
	{
		return msgbuf.ret;
	}
	pRecvCfg =(MSGVENCCFG*)&msgbuf.mem_info;
	if(pRecvCfg == NULL)
	{
		return msgbuf.ret;
	}
	memcpy( pVencCfg , pRecvCfg , sizeof(MSGVENCCFG));
	return msgbuf.ret;
}


int Appro_SetOsdEnable(int nType ,unsigned char nValue)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;

	if(nType<0 || nType>2) return -1;
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	if(0 == nType)	msgbuf.cmd = MSG_CMD_SET_OSD_ENGLISHEN;
	if(1 == nType)	msgbuf.cmd = MSG_CMD_SET_OSD_TIMEEN;
	if(2 == nType)	msgbuf.cmd = MSG_CMD_SET_OSD_TEXTEN;
	ptr = (unsigned char*)&msgbuf.mem_info;
	*ptr = nValue;
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);	
	return msgbuf.ret;

}
int Appro_SetOsdTextStr(char* strText, int nLength)
{
	MSG_BUF msgbuf;
	int* ptr;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;

	msgbuf.cmd = MSG_CMD_SET_OSD_TEXTSTR;

	ptr = (int*)&msgbuf.mem_info;
	*ptr = nLength;
	memcpy(++ptr, strText, nLength);
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}

int Appro_SetVideoMask(MSGVMASK* pMsgVideoMask)
{
	MSG_BUF msgbuf;
	unsigned char* ptr;
	
	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = PROC_MSG_ID;
	msgbuf.cmd = MSG_CMD_SET_MASK_EXT;
	ptr = (unsigned char*)&msgbuf.mem_info;
	memcpy(ptr, pMsgVideoMask, sizeof(MSGVMASK));
	
	SemWait(hndlApproDrvSem);
	msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);/*send msg1*/
	msgrcv( qid, &msgbuf,sizeof(msgbuf)-sizeof(long), PROC_MSG_ID, 0);
	SemRelease(hndlApproDrvSem);
	return msgbuf.ret;
}




