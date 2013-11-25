
#include <imageTunePriv.h>

int IMAGE_TUNE_CmdExecute(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal = OSA_SOK;
  IMAGE_TUNE_CmdHandler handler;

  if(cmdInfo->commandId >= IMAGE_TUNE_CMD_MAX) {

    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG    
    OSA_printf(" IMAGE TUNE SERVER: RECV 0x%04x, UNKNOWN COMMAND \n", cmdInfo->commandId);
    #endif
    
    return OSA_EFAIL;
  }
  
  handler = gIMAGE_TUNE_ctrl.cmdHandlerList[cmdInfo->commandId];
  
  if(handler==NULL) {
  
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG    
    OSA_printf(" IMAGE TUNE SERVER: RECV 0x%04x, NO HANDLER \n", cmdInfo->commandId);
    #endif
    
    return OSA_SOK;
  }

  if(handler) {
  
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG    
    OSA_printf(" IMAGE TUNE SERVER: RECV 0x%04x, Parameters %d bytes \n", cmdInfo->commandId, cmdInfo->prmSize);
    #endif
  
    retVal = handler(cmdInfo, statusInfo);

#ifdef IMAGE_TUNE_MULTI_PARAM
    if( retVal == OSA_SOK && ( (cmdInfo->commandId > 0xF) && (cmdInfo->commandId < 0x60))){
	retVal = IMAGE_TUNE_MultiParams_Set(cmdInfo, statusInfo);
    }
#endif
  }

  return retVal;
}

int IMAGE_TUNE_AttachCmdHandler(Uint32 cmd, IMAGE_TUNE_CmdHandler handler)
{
  if(cmd >= IMAGE_TUNE_CMD_MAX) {
  
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG    
    OSA_printf(" IMAGE TUNE SERVER: 0x%04x ILLEGAL COMMAND \n", cmd);
    #endif
    
    return OSA_EFAIL;  
  }
  
  gIMAGE_TUNE_ctrl.cmdHandlerList[cmd] = handler;
  
  return OSA_SOK;
}

