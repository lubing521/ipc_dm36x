#include <imageTunePriv.h>

static Uint8 * gDataBuff;
static Uint32 imgDataSize;
static Uint32 imgExtraSize;
static int WriteDataintofile(Uint8* tmpBuf, Uint32 dataSize, Uint32 type);


int IMAGE_TUNE_ServerRun(int port)
{
  int retVal = OSA_SOK;
  Uint32 dataSize, prmSize;

  IMAGE_TUNE_CmdInfo cmdInfo, statusInfo;

  while(1) {

    retVal = IMAGE_TUNE_ServerWaitConnect();
    if(retVal != OSA_SOK)
      return retVal;

    #ifdef IMAGE_TUNE_SERVER_DEBUG
    OSA_printf(" IMAGE TUNE SERVER: Client connected !!!\n");
    #endif

    while(1) {

      dataSize = IMAGE_TUNE_PACKET_HEADER_SIZE;
      retVal = IMAGE_TUNE_ServerRecvData((Uint8*)gIMAGE_TUNE_ctrl.serverDataBuf, &dataSize);
      if(retVal!=OSA_SOK) {
        #ifdef IMAGE_TUNE_SERVER_DEBUG
      //OSA_printf( " IMAGE TUNE SERVER: Header receive ERROR !!!\n");
        #endif
        break;
      }

      if(gIMAGE_TUNE_ctrl.serverDataBuf[IMAGE_TUNE_HEADER_OFFSET]!=IMAGE_TUNE_HEADER_MAGIC_NUM) {
        #ifdef IMAGE_TUNE_SERVER_DEBUG
        OSA_printf( " IMAGE TUNE SERVER: Header mismatch ERROR !!!\n");
        #endif
        continue;
      }

      prmSize = gIMAGE_TUNE_ctrl.serverDataBuf[IMAGE_TUNE_PRM_SIZE_OFFSET];

      dataSize = prmSize+IMAGE_TUNE_PACKET_FOOTER_SIZE;
      retVal = IMAGE_TUNE_ServerRecvData(( (Uint8*)gIMAGE_TUNE_ctrl.serverDataBuf+IMAGE_TUNE_PACKET_HEADER_SIZE ) , &dataSize);
      if(retVal!=OSA_SOK) {
        #ifdef IMAGE_TUNE_SERVER_DEBUG
        OSA_printf( " IMAGE TUNE SERVER: Parameter/Footer receive ERROR !!!\n");
        #endif
        break;
      }

      if( (prmSize+IMAGE_TUNE_PACKET_FOOTER_SIZE) != dataSize) {
        #ifdef IMAGE_TUNE_SERVER_DEBUG
        OSA_printf( " IMAGE TUNE SERVER: Footer mismatch ERROR !!!\n");
        #endif
        continue;
      }

      retVal = IMAGE_TUNE_ParsePacket(gIMAGE_TUNE_ctrl.serverDataBuf, dataSize + IMAGE_TUNE_PACKET_HEADER_SIZE, &cmdInfo );
      if(retVal!=OSA_SOK) {
        #ifdef IMAGE_TUNE_SERVER_DEBUG
        OSA_printf( " IMAGE TUNE SERVER: Packet parse ERROR !!!\n");
        #endif
        continue;
      }

      statusInfo.commandId   = cmdInfo.commandId;
      statusInfo.commandFlags= cmdInfo.commandFlags;
      statusInfo.prmSize     = 0;
      statusInfo.prm         = gIMAGE_TUNE_ctrl.statusDataBuf;

      retVal = IMAGE_TUNE_CmdExecute(&cmdInfo, &statusInfo);
      if(retVal!=OSA_SOK) {
        statusInfo.commandFlags |= IMAGE_TUNE_CMD_FLAG_STATUS_ERROR;
      }

      if( statusInfo.commandId == IMAGE_TUNE_SYS_SEND_RAW_DATA || statusInfo.commandId == IMAGE_TUNE_SYS_SEND_YUV_DATA){
         /* Used separate send for image data due to the restriction in the sending data size (Param set)  */
		 retVal = IMAGE_TUNE_CmdDataSend(&statusInfo);
		 if(retVal!=OSA_SOK) {
		 #ifdef IMAGE_TUNE_SERVER_DEBUG
		     OSA_printf( " IMAGE TUNE SERVER: Reply send ERROR !!!\n");
		 #endif
		     break;
		 }
		 continue;
      }

      if(statusInfo.commandFlags & IMAGE_TUNE_CMD_FLAG_SEND_REPLY) {
        retVal = IMAGE_TUNE_MakePacket(&statusInfo, gIMAGE_TUNE_ctrl.serverDataBuf, &dataSize);
        if(retVal!=OSA_SOK) {
          #ifdef IMAGE_TUNE_SERVER_DEBUG
          OSA_printf( " IMAGE TUNE SERVER: Reply make ERROR !!!\n");
          #endif
          continue;
        }

        retVal = IMAGE_TUNE_ServerSendData((Uint8*)gIMAGE_TUNE_ctrl.serverDataBuf, dataSize);
        if(retVal!=OSA_SOK) {
          #ifdef IMAGE_TUNE_SERVER_DEBUG
          OSA_printf( " IMAGE TUNE SERVER: Reply send ERROR !!!\n");
          #endif
          break;
        }
      }
   }
   #ifdef IMAGE_TUNE_SERVER_DEBUG
   OSA_printf(" IMAGE TUNE SERVER: Client disconnected !!!\n");
   #endif
 }

 return retVal;
}




/* Send Image data to ITT (PC) -
    Copy image data in to a separate buffer; same data sent to ITT(PC) */
int IMAGE_TUNE_CopyImageData(int hSize, int dSize, Uint8 *headerData, Uint8 *ImgData)
{
  Uint8* buff;
  imgDataSize = IMAGE_TUNE_PACKET_HEADER_SIZE + hSize + dSize + IMAGE_TUNE_PACKET_FOOTER_SIZE;

  if(gDataBuff != NULL){
	OSA_cmemFree(gDataBuff);
#ifdef IMAGE_TUNE_SERVER_DEBUG
	OSA_printf("IMAGE TUNE SERVER: Send data mem released\n");
#endif
  }
  imgExtraSize = (32 - (imgDataSize % 32));
  imgDataSize = imgDataSize + imgExtraSize;

  gDataBuff = OSA_cmemAlloc(imgDataSize,32);
  if(gDataBuff == NULL)
  	return OSA_EFAIL;

  memset(gDataBuff, 0, imgDataSize);
  buff = gDataBuff + IMAGE_TUNE_PACKET_HEADER_SIZE;

  memcpy(buff, headerData, hSize);
  buff = buff + hSize;
  memcpy(buff, ImgData, dSize);

  return OSA_SOK;

}


/* Send Image data to ITT (PC)
    Used separate send data as teh other send (Image paramset) has limited size of data send and
    breaking image data was not handled in ITT(PC) */
int IMAGE_TUNE_CmdDataSend(IMAGE_TUNE_CmdInfo *statusInfo){
	Uint32 pktDataSize = 0, dataSize = 0;
    int retVal = OSA_EFAIL;
	IMAGE_TUNE_PacketHeader packetHeader;
	IMAGE_TUNE_PacketFooter packetFooter;
	Uint32* packet;

	if(gDataBuff == NULL)
		return OSA_EFAIL;

	packet = (Uint32*)gDataBuff;
	pktDataSize = imgDataSize; //(IMAGE_TUNE_PACKET_HEADER_SIZE + packetHeader.prmSize + IMAGE_TUNE_PACKET_FOOTER_SIZE);

	packetHeader.headerMagicNum = IMAGE_TUNE_HEADER_MAGIC_NUM;
	packetHeader.checkSum		= 0;
	packetHeader.commandId		= statusInfo->commandId;
	packetHeader.commandFlags	= statusInfo->commandFlags;
	packetHeader.prmSize		= imgDataSize-IMAGE_TUNE_PACKET_HEADER_SIZE-IMAGE_TUNE_PACKET_FOOTER_SIZE-imgExtraSize;
	packetFooter.footerMagicNum = IMAGE_TUNE_FOOTER_MAGIC_NUM;

	memcpy( (Uint8*)packet, &packetHeader, IMAGE_TUNE_PACKET_HEADER_SIZE);
	memcpy(((Uint8*)packet + imgDataSize - IMAGE_TUNE_PACKET_FOOTER_SIZE), &packetFooter, IMAGE_TUNE_PACKET_FOOTER_SIZE);

#ifdef IMAGE_TUNE_PACKET_DEBUG
	OSA_printf(" IMAGE TUNE SERVER: Image Data Packet size is %d bytes\n", imgDataSize);
#endif

	// calculate check-sum
	packet[IMAGE_TUNE_CHECK_SUM_OFFSET] = IMAGE_TUNE_CalcPacketChecksum(packet);

	retVal = IMAGE_TUNE_ServerSendData((Uint8*)packet, pktDataSize);
	if(retVal!=OSA_SOK) {
#ifdef IMAGE_TUNE_SERVER_DEBUG
		OSA_printf( " IMAGE TUNE SERVER: Reply send ERROR !!!\n");
#endif
	}

#if 0

	dataSize = WriteDataintofile((Uint8*)packet, pktDataSize, packetHeader.commandId);
	OSA_printf(" ITS: Send Image Data to ITT %d & copied %d, \n", pktDataSize, dataSize);
	OSA_printf(" ITS: imgDataSize %d, PACKET_HEADER_SIZE %d, PACKET_FOOTER_SIZE %d, imgExtraSize %d",imgDataSize,IMAGE_TUNE_PACKET_HEADER_SIZE,IMAGE_TUNE_PACKET_FOOTER_SIZE, imgExtraSize)

#endif

	if(gDataBuff != NULL){
		OSA_cmemFree(gDataBuff);
#ifdef IMAGE_TUNE_SERVER_DEBUG
		OSA_printf(" IMAGE TUNE SERVER: Send Iamge Data memory released\n");
#endif
        gDataBuff = NULL;
	}

	return OSA_SOK;

}


/* Test - Write the image data in to a  file */
int WriteDataintofile(Uint8* tmpBuf, Uint32 dataSize, Uint32 type){
  FILE *fp;
  int wsize = 0;
  if(type == IMAGE_TUNE_SYS_SEND_RAW_DATA)
    fp = fopen("/mnt/mmc/ipnc/SentData.rawTI", "wb");
  else if(type == IMAGE_TUNE_SYS_SEND_YUV_DATA)
    fp = fopen("/mnt/mmc/ipnc/SentData.yuvTI", "wb");

  if(fp==NULL) {
     return OSA_EFAIL;
  }

  wsize = fwrite(tmpBuf, 1, dataSize, fp);
  fclose(fp);

  return wsize;
}



