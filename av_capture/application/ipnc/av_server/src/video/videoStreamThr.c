#include <avserver.h>

typedef struct {

  Uint16 startX;
  Uint16 startY;
  Uint16 width;
  Uint16 height;
  Uint16 confidence;
  Uint16 angle;

} VIDEO_SaveFaceInfo;

typedef struct {

  Uint32 magicNum;
  Uint32 numFaces;
  VIDEO_SaveFaceInfo  faceInfo[16];

} VIDEO_SaveFaceList;


int VIDEO_streamSaveFaceInfo(FILE *fileHndl, Uint16 streamWidth, Uint16 streamHeight)
{
  static DRV_FaceDetectRunStatus faceStatus;
  static VIDEO_SaveFaceList saveFaceList;
  
  int i, centerX, centerY, size, writtenBytes;  
  
  faceStatus.numFaces=0;
  VIDEO_fdGetFaceStatus(&faceStatus);

  memset(&saveFaceList, 0, sizeof(saveFaceList));
  
  saveFaceList.magicNum = 0xFACEFACE;
  saveFaceList.numFaces = faceStatus.numFaces;
  
  if(saveFaceList.numFaces>16)
    saveFaceList.numFaces = 16;
      
  for(i=0; i<saveFaceList.numFaces; i++) {

    centerX = (faceStatus.info[i].centerX * streamWidth ) / gVIDEO_ctrl.faceDetectStream.fdWidth;
    centerY = (faceStatus.info[i].centerY * streamHeight ) / gVIDEO_ctrl.faceDetectStream.fdHeight;    
    size    = (CSL_FACE_DETECT_GET_SIZE(faceStatus.info[i].sizeConfidence)*streamWidth ) / streamHeight;  

    saveFaceList.faceInfo[i].startX  = centerX - size/2;
    saveFaceList.faceInfo[i].startY  = centerY - size/2;
    
    saveFaceList.faceInfo[i].width   = size;
    saveFaceList.faceInfo[i].height  = size;
    saveFaceList.faceInfo[i].confidence = CSL_FACE_DETECT_GET_CONFIDENCE(faceStatus.info[i].sizeConfidence);
    saveFaceList.faceInfo[i].angle   = faceStatus.info[i].angle;
  }

  writtenBytes = fwrite( &saveFaceList, 1, sizeof(saveFaceList), fileHndl);
  
  if(writtenBytes!=sizeof(saveFaceList)) {
    return OSA_EFAIL;
  }
  
  return OSA_SOK;
}

int VIDEO_streamFileWrite(int streamId, OSA_BufInfo *pBufInfo)
{
  Bool saveToFile;
  int  fileSaveState;
  int  status=OSA_SOK, writtenBytes;
  VIDEO_BufHeader *pInBufHeader;
  VIDEO_EncodeConfig *pEncodeConfig;
  VIDEO_EncodeStream *pEncodeStream;
  
  static char filename[256];  
  static char faceInfoFile[256];
  
  pEncodeConfig = &gAVSERVER_config.encodeConfig[streamId];
  pEncodeStream = &gVIDEO_ctrl.encodeStream[streamId];
  
  pInBufHeader = (VIDEO_BufHeader *)pBufInfo->virtAddr;

  fileSaveState = pEncodeStream->fileSaveState;
  
  saveToFile = pEncodeConfig->fileSaveEnable;
  
  #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
    #ifdef AVSERVER_DEBUG_RUNNING
    OSA_printf(" STREAM: Stream %d, saveToFile = %d, fileSaveState = %d, frame size = %d, key frame = %d\n", 
          streamId, saveToFile, fileSaveState, pInBufHeader->encFrameSize, pInBufHeader->encFrameType);
    #endif
  #endif    
  
  if(saveToFile) {
  
    if(fileSaveState==VIDEO_STREAM_FILE_SAVE_STATE_IDLE)
      fileSaveState=VIDEO_STREAM_FILE_SAVE_STATE_OPEN;
  
  } else {

    if(fileSaveState==VIDEO_STREAM_FILE_SAVE_STATE_OPEN)
      fileSaveState=VIDEO_STREAM_FILE_SAVE_STATE_IDLE;
    else
    if(fileSaveState==VIDEO_STREAM_FILE_SAVE_STATE_WRITE)
      fileSaveState=VIDEO_STREAM_FILE_SAVE_STATE_CLOSE;
  }
  
  if(fileSaveState==VIDEO_STREAM_FILE_SAVE_STATE_OPEN) {
    if(pInBufHeader->encFrameType==VIDEO_ENC_FRAME_TYPE_KEY_FRAME) {

      sprintf(filename, "CH%02d_%04d_%dx%d.bits", 
                streamId, 
                pEncodeStream->fileSaveIndex,                
                pEncodeConfig->cropWidth,
                pEncodeConfig->cropHeight                
            );

      sprintf(faceInfoFile, "CH%02d_%04d_%dx%d.faceInf", 
                streamId, 
                pEncodeStream->fileSaveIndex,                
                pEncodeConfig->cropWidth,
                pEncodeConfig->cropHeight                
            );


      #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
      OSA_printf(" STREAM: Opening file stream %d, %s\n", streamId, filename);
      #endif  
      
      pEncodeStream->fileSaveHndl = fopen( filename, "wb");
      pEncodeStream->fileFaceInfoHndl = fopen( faceInfoFile, "wb");  
              
      if(   pEncodeStream->fileSaveHndl == NULL 
         || pEncodeStream->fileFaceInfoHndl == NULL 
        ) { 
        #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR            
        OSA_ERROR("Stream %d open\n", streamId);    
        #endif         
        fileSaveState = VIDEO_STREAM_FILE_SAVE_STATE_IDLE;
        
        fclose(pEncodeStream->fileSaveHndl);
        fclose(pEncodeStream->fileFaceInfoHndl);
        
      } else {
        fileSaveState = VIDEO_STREAM_FILE_SAVE_STATE_WRITE;
        pEncodeStream->fileSaveIndex++;
      }
    }
  }
  
  if(fileSaveState==VIDEO_STREAM_FILE_SAVE_STATE_WRITE) {  
    if(pInBufHeader->encFrameSize>0) {
    
      #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
        #ifdef AVSERVER_DEBUG_RUNNING
        OSA_printf(" STREAM: Stream %d writing %d bytes\n", streamId, pInBufHeader->encFrameSize);
        #endif
      #endif    
      
      writtenBytes = fwrite(pBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE, 
                      1, 
                      pInBufHeader->encFrameSize, 
                      pEncodeStream->fileSaveHndl
                      );
      if(writtenBytes!=pInBufHeader->encFrameSize) {
        #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR      
        OSA_ERROR("Stream %d write\n", streamId);
        #endif
        pEncodeConfig->fileSaveEnable = FALSE;
        fileSaveState=VIDEO_STREAM_FILE_SAVE_STATE_CLOSE;
      }
      
      if(fileSaveState==VIDEO_STREAM_FILE_SAVE_STATE_WRITE) {
        status = VIDEO_streamSaveFaceInfo(pEncodeStream->fileFaceInfoHndl, pEncodeConfig->cropWidth, pEncodeConfig->cropHeight);
        
        if(status!=OSA_SOK) {
          #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR      
          OSA_ERROR("Stream %d write\n", streamId);
          #endif
          pEncodeConfig->fileSaveEnable = FALSE;
          fileSaveState=VIDEO_STREAM_FILE_SAVE_STATE_CLOSE;
        }
      }      
    }
  }
  if(fileSaveState==VIDEO_STREAM_FILE_SAVE_STATE_CLOSE) {   
  
    #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
    OSA_printf(" STREAM: Closing file stream %d\n", streamId);
    #endif   
    
    fclose(pEncodeStream->fileSaveHndl);
    fclose(pEncodeStream->fileFaceInfoHndl);    
    
    fileSaveState=VIDEO_STREAM_FILE_SAVE_STATE_IDLE;
    
    #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
    OSA_printf(" STREAM: Closing file stream ... DONE\n");
    #endif  
  }
    
  pEncodeStream->fileSaveState = fileSaveState; 

  return status;
}

int VIDEO_streamFileClose()
{
  int  i;
  VIDEO_EncodeStream *pEncodeStream;

  for(i=0; i<gAVSERVER_config.numEncodeStream; i++) {  
  
    pEncodeStream = &gVIDEO_ctrl.encodeStream[i];

    if(pEncodeStream->fileSaveState==VIDEO_STREAM_FILE_SAVE_STATE_WRITE) {

      #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
      OSA_printf(" STREAM: Closing file stream %d\n", i);
      #endif  

      fclose(pEncodeStream->fileSaveHndl);
      
      #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
      OSA_printf(" STREAM: Closing file stream ... DONE\n");
      #endif  
      
    }
      
    pEncodeStream->fileSaveState = VIDEO_STREAM_FILE_SAVE_STATE_IDLE;    
  }

  return OSA_SOK;
}

int VIDEO_streamTskCreate()
{
  VIDEO_streamSysInit();
  
  return OSA_SOK;
}

int VIDEO_streamTskDelete()
{
  VIDEO_streamFileClose();
  
  VIDEO_streamSysExit();
    
  return OSA_SOK;
}

int VIDEO_streamTskRun(int streamId)
{
  int status=OSA_EFAIL, inBufId;
  OSA_BufInfo *pInBufInfo;
  VIDEO_BufHeader *pInBufHeader;
  
  pInBufInfo = AVSERVER_bufGetFull( VIDEO_TSK_STREAM, streamId, &inBufId, OSA_TIMEOUT_FOREVER);
  
  OSA_assert(pInBufInfo!=NULL);
  
  if(pInBufInfo!=NULL) {
   
    pInBufHeader  = (VIDEO_BufHeader*)pInBufInfo->virtAddr;
    
    OSA_assert(pInBufHeader!=NULL);    

    #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
      #ifdef AVSERVER_DEBUG_RUNNING
      OSA_printf(" STREAM: Stream %d InBuf %d, Frame size %d bytes, Key frame %d\n", streamId, inBufId, pInBufHeader->encFrameSize, pInBufHeader->encFrameType);
      #endif
    #endif  

    if(pInBufHeader->encFrameSize > 0) {
    
      OSA_prfBegin(&gAVSERVER_ctrl.streamPrf);
    
      VIDEO_streamShmCopy(streamId, pInBufInfo);
      VIDEO_streamFileWrite(streamId, pInBufInfo);
      
      OSA_prfEnd(&gAVSERVER_ctrl.streamPrf, 1);      
    }      

    AVSERVER_bufPutEmpty( VIDEO_TSK_STREAM, streamId, inBufId);  
  }
  
  return status;
}

int VIDEO_streamTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status, streamId;
  Bool done=FALSE, ackMsg = FALSE;
  Uint16 cmd = OSA_msgGetCmd(pMsg);
  
  #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
  OSA_printf(" STREAM: Recevied CMD = 0x%04x\n", cmd);
  #endif  
  
  if(cmd!=AVSERVER_CMD_CREATE) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    return OSA_SOK;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
  OSA_printf(" STREAM: Create...\n");
  #endif  
  
  status = VIDEO_streamTskCreate();
  
  OSA_tskAckOrFreeMsg(pMsg, status);  
  
  if(status!=OSA_SOK) {
    OSA_ERROR("VIDEO_streamTskCreate()\n");
    return status;
  }

  #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
  OSA_printf(" STREAM: Create...DONE\n");
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
        
        VIDEO_streamTskRun(streamId);
        
        break;
      default:
      
        #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
        OSA_printf(" STREAM: Unknown CMD = 0x%04x\n", cmd);
        #endif  
      
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);      
        break;
    }
  }
 
  #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
  OSA_printf(" STREAM: Delete...\n");
  #endif  
    
  VIDEO_streamTskDelete();
  
  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  #ifdef AVSERVER_DEBUG_VIDEO_STREAM_THR
  OSA_printf(" STREAM: Delete...DONE\n");
  #endif  

  return OSA_SOK;
}

int VIDEO_streamCreate()
{
  int status;
  
  status = OSA_tskCreate( &gVIDEO_ctrl.streamTsk, VIDEO_streamTskMain, VIDEO_STREAM_THR_PRI, VIDEO_STREAM_STACK_SIZE*2, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }
  
  return status;
}

int VIDEO_streamDelete()
{
  int status;
  
  status = OSA_tskDelete( &gVIDEO_ctrl.streamTsk );
  
  return status;
}


