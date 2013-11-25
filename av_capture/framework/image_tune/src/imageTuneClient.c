
#include <imageTunePriv.h>

static Uint32 gIMAGE_TUNE_clientDataBuf[IMAGE_TUNE_PACKET_SIZE_MAX/4];

int IMAGE_TUNE_ClientSendCmd(char *serverIpAddr, int port, IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal;
  Uint32 dataSize, prmSize;

  retVal = IMAGE_TUNE_ClientOpen(serverIpAddr, port);
  if(retVal!=OSA_SOK) {
    #ifdef IMAGE_TUNE_CLIENT_DEBUG
    OSA_printf(" IMAGE TUNE CLIENT: Server open ERROR !!!\n");
    #endif
    return retVal;
  }

  retVal = IMAGE_TUNE_MakePacket(cmdInfo, gIMAGE_TUNE_clientDataBuf, &dataSize);
  if(retVal!=OSA_SOK) {
    #ifdef IMAGE_TUNE_CLIENT_DEBUG
    OSA_printf(" IMAGE TUNE CLIENT: Packet make ERROR !!!\n");
    #endif
    goto error_exit;
  }

  retVal = IMAGE_TUNE_ClientSendData((Uint8*)gIMAGE_TUNE_clientDataBuf, dataSize);
  if(retVal!=OSA_SOK) {
    #ifdef IMAGE_TUNE_CLIENT_DEBUG
    OSA_printf(" IMAGE TUNE CLIENT: Command send ERROR !!!\n");
    #endif
    goto error_exit;
  }

  if(cmdInfo->commandFlags & IMAGE_TUNE_CMD_FLAG_SEND_REPLY) {

    dataSize = IMAGE_TUNE_PACKET_HEADER_SIZE;

    memset(gIMAGE_TUNE_clientDataBuf, 0, dataSize);

    retVal = IMAGE_TUNE_ClientRecvData((Uint8*)gIMAGE_TUNE_clientDataBuf, &dataSize);
    if(retVal!=OSA_SOK) {
      #ifdef IMAGE_TUNE_CLIENT_DEBUG
      OSA_printf(" IMAGE TUNE CLIENT: Reply header receive ERROR !!!\n");
      #endif
      goto error_exit;
    }

    if(gIMAGE_TUNE_clientDataBuf[IMAGE_TUNE_HEADER_OFFSET]!=IMAGE_TUNE_HEADER_MAGIC_NUM) {
      retVal = OSA_EFAIL;
      #ifdef IMAGE_TUNE_CLIENT_DEBUG
      OSA_printf(" IMAGE TUNE CLIENT: Reply header mismatch ERROR !!!\n");
      #endif
      goto error_exit;
    }

    prmSize = gIMAGE_TUNE_clientDataBuf[IMAGE_TUNE_PRM_SIZE_OFFSET];

    dataSize = prmSize+IMAGE_TUNE_PACKET_FOOTER_SIZE;
    retVal = IMAGE_TUNE_ClientRecvData(( (Uint8*)gIMAGE_TUNE_clientDataBuf+IMAGE_TUNE_PACKET_HEADER_SIZE ) , &dataSize);
    if(retVal!=OSA_SOK) {
      #ifdef IMAGE_TUNE_CLIENT_DEBUG
      OSA_printf(" IMAGE TUNE CLIENT: Reply parameter/footer receive ERROR !!!\n");
      #endif
      goto error_exit;
    }

    if( (prmSize+IMAGE_TUNE_PACKET_FOOTER_SIZE) != dataSize)  {
      retVal = OSA_EFAIL;
      #ifdef IMAGE_TUNE_CLIENT_DEBUG
      OSA_printf(" IMAGE TUNE CLIENT: Reply footer mismatch ERROR !!!\n");
      #endif
      goto error_exit;
    }

    retVal = IMAGE_TUNE_ParsePacket(gIMAGE_TUNE_clientDataBuf, dataSize, statusInfo);
    if(retVal!=OSA_SOK) {
      #ifdef IMAGE_TUNE_CLIENT_DEBUG
      OSA_printf(" IMAGE TUNE CLIENT: Reply parse ERROR !!!\n");
      #endif
      goto error_exit;
    }

    if(cmdInfo->commandId != statusInfo->commandId) {
      retVal = OSA_EFAIL;
      #ifdef IMAGE_TUNE_CLIENT_DEBUG
      OSA_printf(" IMAGE TUNE CLIENT: Reply commandId mismatch ERROR !!!\n");
      #endif
      goto error_exit;
    }
  }

error_exit:
  if(retVal!=OSA_SOK) {
    #ifdef IMAGE_TUNE_CLIENT_DEBUG
    OSA_printf(" IMAGE TUNE CLIENT: IMAGE_TUNE_ClientSendCmd() ERROR !!!\n");
    #endif
  }

  retVal = IMAGE_TUNE_ClientClose();

  return retVal;

}


