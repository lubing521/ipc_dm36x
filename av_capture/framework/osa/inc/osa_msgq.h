

#ifndef _OSA_MSGQ_H_
#define _OSA_MSGQ_H_

#include <osa.h>

#define OSA_MSGQ_LEN_MAX    32

struct OSA_MsgHndl;

typedef struct {

  Uint32 curRd;
  Uint32 curWr;
  Uint32 len;
  Uint32 count;

  struct OSA_MsgHndl *queue[OSA_MSGQ_LEN_MAX];

  pthread_mutex_t lock;
  pthread_cond_t  condRd;
  pthread_cond_t  condWr;
  
} OSA_MsgqHndl;

typedef struct OSA_MsgHndl {

  OSA_MsgqHndl *pTo;
  OSA_MsgqHndl *pFrom;  
  void         *pPrm;
  int           status;
  Uint16        cmd;
  Uint16        flags;

} OSA_MsgHndl;

#define OSA_msgGetCmd(msg)         ( (msg)->cmd )
#define OSA_msgGetPrm(msg)         ( (msg)->pPrm )
#define OSA_msgGetAckStatus(msg)   ( (msg)->status )

int OSA_msgqCreate(OSA_MsgqHndl *hndl);
int OSA_msgqDelete(OSA_MsgqHndl *hndl);
int OSA_msgqSendMsg(OSA_MsgqHndl *to, OSA_MsgqHndl *from, Uint16 cmd, void *prm, Uint16 msgFlags, OSA_MsgHndl **msg);
int OSA_msgqRecvMsg(OSA_MsgqHndl *hndl, OSA_MsgHndl **msg, Uint32 timeout);
int OSA_msgqSendAck(OSA_MsgHndl *msg, int ackRetVal);
int OSA_msgqFreeMsgHndl(OSA_MsgHndl *msg);


#endif /* _OSA_FLG_H_ */



