#include <avserver.h>

Uint8 gVIDEO_defaultCryptoKey[ALG_CRYPTO_MAX_KEY_LEN] = {

  0x01, 0x02, 0x03, 0x04,
  0x05, 0x06, 0x07, 0x08,
  0x09, 0x0A, 0x0B, 0x0C,
  0x0D, 0x0E, 0x0F, 0x10,
};

int VIDEO_encryptTskCreate()
{
  int i;
  ALG_CryptoCreate createPrm;
  
  for(i=0; i<gAVSERVER_config.numEncodeStream; i++) 
  {
    memcpy( &createPrm.key[0], &gVIDEO_defaultCryptoKey[0], sizeof(createPrm.key));
    
    createPrm.mode = ALG_CRYPTO_MODE_FAST;
    
    gVIDEO_ctrl.encodeStream[i].algEncryptHndl = ALG_encryptCreate(&createPrm);
    
    #if 0
    if(gVIDEO_ctrl.encodeStream[i].algEncryptHndl==NULL) {
      OSA_ERROR("ALG_encryptCreate(%d)", i);
      return OSA_EFAIL;
    }
    #endif
  }

  return OSA_SOK;
}

int VIDEO_encryptTskDelete()
{
  int i;
  
  for(i=0; i<gAVSERVER_config.numEncodeStream; i++)
  {
    ALG_encryptDelete(gVIDEO_ctrl.encodeStream[i].algEncryptHndl);
  }
  
  return OSA_SOK;
}

int VIDEO_encryptTskRun(int streamId)
{
  int status=OSA_EFAIL, inBufId, outBufId;
  OSA_BufInfo *pInBufInfo, *pOutBufInfo;
  VIDEO_BufHeader *pInBufHeader, *pOutBufHeader;
  
  pInBufInfo = AVSERVER_bufGetFull( VIDEO_TSK_ENCRYPT, streamId, &inBufId, OSA_TIMEOUT_FOREVER);
  
  OSA_assert(pInBufInfo!=NULL);
  
  if(pInBufInfo!=NULL) {
  
    pOutBufInfo = AVSERVER_bufGetEmpty( VIDEO_TSK_ENCRYPT, streamId, &outBufId, OSA_TIMEOUT_FOREVER);  
    
    OSA_assert(pOutBufInfo!=NULL);    

    #ifdef AVSERVER_DEBUG_VIDEO_ENCRYPT_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" ENCRYPT: Stream %d InBuf %d OutBuf %d\n", streamId, inBufId, outBufId);
      #endif
    #endif
  
    if(pOutBufInfo!=NULL) {
  
      pInBufHeader  = (VIDEO_BufHeader*)pInBufInfo->virtAddr;
      pOutBufHeader = (VIDEO_BufHeader*)pOutBufInfo->virtAddr;
      
      OSA_assert(pInBufHeader!=NULL);
      OSA_assert(pOutBufHeader!=NULL);                          

      if(pInBufHeader->encFrameSize > 0) {
      
        status = ALG_encryptRun(gVIDEO_ctrl.encodeStream[streamId].algEncryptHndl, 
                                pInBufInfo->virtAddr, 
                                pOutBufInfo->virtAddr,
                                pInBufHeader->encFrameSize
                              );
        if(status!=OSA_SOK) {
          OSA_ERROR("ALG_encryptRun(%d)\n", streamId);
          pOutBufHeader->encFrameSize = 0;          
        }
      }      

      memcpy(pOutBufHeader, pInBufHeader, sizeof(*pOutBufHeader));
      
      AVSERVER_bufPutFull( VIDEO_TSK_ENCRYPT, streamId, outBufId);          
    }

    AVSERVER_bufPutEmpty( VIDEO_TSK_ENCRYPT, streamId, inBufId);  
  }
  
  return status;
}

int VIDEO_encryptTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status, streamId;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);
  
  #ifdef AVSERVER_DEBUG_VIDEO_ENCRYPT_THR
  OSA_printf(" ENCRYPT: Recevied CMD = 0x%04x\n", cmd);
  #endif  
  
  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_ENCRYPT_THR
  OSA_printf(" ENCRYPT: Create...\n");
  #endif  
  
  status = VIDEO_encryptTskCreate();
  
  OSA_tskAckOrFreeMsg(pMsg, status);  
  
  if(status!=OSA_SOK) {
    OSA_ERROR("VIDEO_encryptTskCreate()");
    return status;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_ENCRYPT_THR
  OSA_printf(" ENCRYPT: Create...DONE\n");
  #endif  
    
  while(!done) {
    
    status = OSA_tskWaitMsg(pTsk, &pMsg);

    if(status!=OSA_SOK) 
      break;

    cmd = OSA_msgGetCmd(pMsg);

    switch(cmd) {
      case AVSERVER_CMD_DELETE:
        done = TRUE;
        ackMsg = TRUE;
        break;

      case AVSERVER_CMD_NEW_DATA:
        streamId = (int)OSA_msgGetPrm(pMsg);
        
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        
        VIDEO_encryptTskRun(streamId);
        
        break;
      default:

        #ifdef AVSERVER_DEBUG_VIDEO_ENCRYPT_THR
        OSA_printf(" ENCRYPT: Unknown CMD = 0x%04x\n", cmd);
        #endif  
      
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);      
        break;
    }
  }
  
  #ifdef AVSERVER_DEBUG_VIDEO_ENCRYPT_THR
  OSA_printf(" ENCRYPT: Delete...\n");
  #endif  
  
  VIDEO_encryptTskDelete();
  
  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  #ifdef AVSERVER_DEBUG_VIDEO_ENCRYPT_THR
  OSA_printf(" ENCRYPT: Delete...DONE\n");
  #endif  

  return OSA_SOK;
}

int VIDEO_encryptCreate()
{
  int status;
  
  status = OSA_tskCreate( &gVIDEO_ctrl.encryptTsk, VIDEO_encryptTskMain, VIDEO_ENCRYPT_THR_PRI, VIDEO_ENCRYPT_STACK_SIZE, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }
  
  return status;
}

int VIDEO_encryptDelete()
{
  int status;
  
  status = OSA_tskDelete( &gVIDEO_ctrl.encryptTsk );
  
  return status;
}
