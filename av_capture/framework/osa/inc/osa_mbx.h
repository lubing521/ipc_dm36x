

#ifndef _OSA_MBX_H_
#define _OSA_MBX_H_

#include <osa_msgq.h>

#define OSA_MBX_WAIT_ACK      0x0002   ///< Message Flag : Wait for ACK
#define OSA_MBX_FREE_PRM      0x0004   ///< Message Flag : Message parameters are malloc'ed and need to be free'ed

#define OSA_MBX_BROADCAST_MAX     10   ///< Max message queues/PRC's that can be broadcast to

/**
  \brief Mailbox handle
*/
typedef struct {

  OSA_MsgqHndl rcvMbx;    ///< Receive mailbox
  OSA_MsgqHndl ackMbx;    ///< ACK mailbox

} OSA_MbxHndl;


int OSA_mbxCreate(OSA_MbxHndl *pHndl);
int OSA_mbxDelete(OSA_MbxHndl *pHndl);
int OSA_mbxSendMsg(OSA_MbxHndl *pTo, OSA_MbxHndl *pFrom, Uint16 cmd, void *pPrm, Uint16 flags);
int OSA_mbxBroadcastMsg(OSA_MbxHndl *pToList[], OSA_MbxHndl *pFrom, Uint16 cmd, void *pPrm, Uint16 flags);
int OSA_mbxAckOrFreeMsg(OSA_MsgHndl *pMsg, int ackRetVal);
int OSA_mbxWaitMsg(OSA_MbxHndl *pHndl, OSA_MsgHndl **pMsg);
int OSA_mbxCheckMsg(OSA_MbxHndl *pHndl, OSA_MsgHndl **pMsg);
int OSA_mbxWaitCmd(OSA_MbxHndl *pHndl, OSA_MsgHndl **pMsg, Uint16 waitCmd);
int OSA_mbxFlush(OSA_MbxHndl *pHndl);


#endif /* _OSA_MBX_H_ */



