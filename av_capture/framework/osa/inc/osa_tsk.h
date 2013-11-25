

#ifndef _OSA_TSK_H_
#define _OSA_TSK_H_

#include <osa_thr.h>
#include <osa_mbx.h>

struct OSA_TskHndl;


typedef int (*OSA_TskFncMain)(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState );


/**
  \brief Task Handle
*/
typedef struct OSA_TskHndl {

  OSA_MbxHndl mbxHndl;      ///< Mailbox handle
  OSA_ThrHndl thrHndl;      ///< OS thread handle

  Uint32 curState;          ///< Task state as defined by user
  OSA_TskFncMain fncMain;   ///< Task Main, this function is entered when a message is received by the process

} OSA_TskHndl;

int OSA_tskCreate(OSA_TskHndl *pTsk, OSA_TskFncMain fncMain, Uint32 tskPri, Uint32 tskStackSize, Uint32 initState);
int OSA_tskDelete(OSA_TskHndl *pTsk);
int OSA_tskSendMsg(OSA_TskHndl *pTskTo, OSA_TskHndl *pTskFrom, Uint16 cmd, void *pPrm, Uint16 flags);
int OSA_tskBroadcastMsg(OSA_TskHndl *pTskToList[], OSA_TskHndl *pTskFrom, Uint16 cmd, void *pPrm, Uint16 flags);
int OSA_tskAckOrFreeMsg(OSA_MsgHndl *pMsg, int ackRetVal);
int OSA_tskWaitMsg(OSA_TskHndl *pTsk, OSA_MsgHndl **pMsg);
int OSA_tskCheckMsg(OSA_TskHndl *pTsk, OSA_MsgHndl **pMsg);
int OSA_tskWaitCmd(OSA_TskHndl *pTsk, OSA_MsgHndl **pMsg, Uint16 waitCmd);
int OSA_tskFlushMsg(OSA_TskHndl *pTsk);

int    OSA_tskSetState(OSA_TskHndl *pPrc, Uint32 curState);
Uint32 OSA_tskGetState(OSA_TskHndl *pPrc);

#endif /* _OSA_TSK_H_ */




