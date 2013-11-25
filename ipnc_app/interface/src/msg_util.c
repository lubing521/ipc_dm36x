/* ===========================================================================
* @file msg_util.c
*
* @path $(IPNCPATH)/util/
*
* @desc An easier message queue interface.
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
#include <stdio.h>
#include <linux/errno.h>
#include <string.h>
#include "Msg_Def.h"


#define __DEBUG

#ifdef __DEBUG
#define __D(fmt, args...) fprintf(stderr,  fmt, ## args)
#else
#define __D(fmt, args...)
#endif

#ifdef __DEBUG
#define __E(fmt, args...) fprintf(stderr, "Error: " fmt, ## args)
#else
#define __E(fmt, args...)
#endif
/**
* @brief Initialize message queue.

* Initialize message queue.
* @note This API must be used before use any other message driver API.
* @param msgKey [I ] Key number for message queue and share memory.
* @return message ID.
*/
int Msg_Init( int msgKey )
{
	int qid;
	key_t key = msgKey;

	qid = msgget(key,0);
	if(  qid < 0 )
	{
		qid = msgget(key,IPC_CREAT|0666);
		__D("Creat queue id:%d\n",qid);
	}

	__D("queue id:%d\n",qid);

	return qid;
}
/**
* @brief Send message .

* Excute send message command.
* @param qid [I ] Message ID.
* @param *pdata [I ] pointer to the data.
* @param size [I ] Data size.
* @return send data to message queue.
*/
int Msg_Send( int qid, void *pdata , int size )
{

	return msgsnd( qid, pdata, size-sizeof(long), 0 );///< send msg1

}
/**
* @brief Receive message .

* Excute receive message command.
* @param qid [I ] Message ID.
* @param msg_type [I ] Message type.
* @param *pdata [I ] pointer to the data.
* @param size [I ] Data size.
* @return receive data from message queue.
*/
int Msg_Rsv( int qid, int msg_type, void *pdata , int size )
{
	return msgrcv( qid, pdata, size-sizeof(long), msg_type, 0);
}
/**
* @brief Send and receive message .

* Excute send and receive message command.
* @param qid [I ] Message ID.
* @param msg_type [I ] Message type.
* @param *pdata [I ] pointer to the data.
* @param size [I ] Data size.
* @return receive data from message queue if send success.
*/
int Msg_Send_Rsv( int qid, int msg_type, void *pdata , int size )
{
	int ret = 0;
	ret = msgsnd( qid, pdata, size-sizeof(long), 0 );/* send msg1 */
	if( ret < 0 )
	{
		return ret;
	}
	return msgrcv( qid, pdata, size-sizeof(long), msg_type, 0);

}
/**
* @brief Kill message queue.

* Kill message queue.
* @param qid [I ] Message ID.
* @retval 0 Queue killed.
*/
int Msg_Kill( int qid )
{
	msgctl( qid, IPC_RMID, NULL);

	__D("Kill queue id:%d\n",qid);

	return 0;
}
