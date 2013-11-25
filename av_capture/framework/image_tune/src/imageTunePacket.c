#include <imageTunePriv.h>

Uint32 IMAGE_TUNE_CalcPacketChecksum(Uint32 *packet) 
{
  Uint32 *tmpPtr;
  Uint32 size, checkSum, i;
  
  tmpPtr   = &packet[IMAGE_TUNE_COMMAND_ID_OFFSET];
  size     = packet[IMAGE_TUNE_PRM_SIZE_OFFSET] + IMAGE_TUNE_PACKET_HEADER_SIZE - 2*sizeof(Uint32);
  checkSum = 0;
  for(i=0; i < size/4 ; i++)
    checkSum += *tmpPtr++;

  return checkSum;
}

int IMAGE_TUNE_MakePacket(IMAGE_TUNE_CmdInfo *cmdInfo, Uint32 *packet, Uint32 *packetSize)
{
  int retVal = OSA_SOK;
  
  IMAGE_TUNE_PacketHeader packetHeader;
  IMAGE_TUNE_PacketFooter packetFooter;
  
  Uint32 extraBytes;
  
  if(cmdInfo->prmSize > (IMAGE_TUNE_PACKET_SIZE_MAX-IMAGE_TUNE_PACKET_HEADER_SIZE-IMAGE_TUNE_PACKET_FOOTER_SIZE) ) {
    #ifdef IMAGE_TUNE_PACKET_DEBUG  
    OSA_ERROR(" IMAGE TUNE\n");
    #endif
    return OSA_EFAIL;
  }
  
  extraBytes = sizeof(Uint32) - (cmdInfo->prmSize%sizeof(Uint32));

  packetHeader.headerMagicNum = IMAGE_TUNE_HEADER_MAGIC_NUM;
  packetHeader.checkSum       = 0;
  packetHeader.commandId      = cmdInfo->commandId;
  packetHeader.commandFlags   = cmdInfo->commandFlags;
  packetHeader.prmSize        = cmdInfo->prmSize+extraBytes;

  packetFooter.footerMagicNum = IMAGE_TUNE_FOOTER_MAGIC_NUM;

  *packetSize = (IMAGE_TUNE_PACKET_HEADER_SIZE + packetHeader.prmSize + IMAGE_TUNE_PACKET_FOOTER_SIZE);
  
  memcpy( (Uint8*)packet, &packetHeader, IMAGE_TUNE_PACKET_HEADER_SIZE);

  memcpy(((Uint8*)packet + IMAGE_TUNE_PACKET_HEADER_SIZE), cmdInfo->prm, cmdInfo->prmSize);
  memset(((Uint8*)packet + IMAGE_TUNE_PACKET_HEADER_SIZE + cmdInfo->prmSize), 0, extraBytes);

  memcpy(((Uint8*)packet + IMAGE_TUNE_PACKET_HEADER_SIZE + packetHeader.prmSize), &packetFooter, IMAGE_TUNE_PACKET_FOOTER_SIZE);   
  
  #ifdef IMAGE_TUNE_PACKET_DEBUG
  OSA_printf(" IMAGE TUNE: IMAGE_TUNE_MakePacket(), packet size is %d bytes\n", packetHeader.prmSize);
  #endif
  
  // calculate check-sum
  packet[IMAGE_TUNE_CHECK_SUM_OFFSET] = IMAGE_TUNE_CalcPacketChecksum(packet);

  return retVal;
}

int IMAGE_TUNE_ParsePacket(Uint32 *packet, Uint32 packetSize, IMAGE_TUNE_CmdInfo *cmdInfo )
{
  int retVal = OSA_SOK;
  IMAGE_TUNE_PacketHeader *pRecvPacketHeader;
  Uint32 *tmpPtr;
  Uint32 checkSum;

  memset(cmdInfo, 0, sizeof(*cmdInfo));

  if(packetSize > (IMAGE_TUNE_PACKET_SIZE_MAX) ) {
    #ifdef IMAGE_TUNE_PACKET_DEBUG  
    OSA_ERROR("IMAGE TUNE\n");  
    #endif    
    return OSA_EFAIL;
  }

  pRecvPacketHeader=(IMAGE_TUNE_PacketHeader *)packet;

  // check header
  if(packet[IMAGE_TUNE_HEADER_OFFSET] != IMAGE_TUNE_HEADER_MAGIC_NUM) {
    #ifdef IMAGE_TUNE_PACKET_DEBUG    
    OSA_ERROR("IMAGE TUNE\n");  
    #endif
    return OSA_EFAIL;
  }

  if(pRecvPacketHeader->prmSize > (IMAGE_TUNE_PACKET_SIZE_MAX-IMAGE_TUNE_PACKET_HEADER_SIZE-IMAGE_TUNE_PACKET_FOOTER_SIZE) ) {
    #ifdef IMAGE_TUNE_PACKET_DEBUG    
    OSA_ERROR("IMAGE TUNE\n");  
    #endif
    return OSA_EFAIL;
  }

  // check footer
  tmpPtr = packet + (pRecvPacketHeader->prmSize + IMAGE_TUNE_PACKET_HEADER_SIZE)/4;
  if(*tmpPtr!=IMAGE_TUNE_FOOTER_MAGIC_NUM) {
    #ifdef IMAGE_TUNE_PACKET_DEBUG      
    OSA_ERROR("IMAGE TUNE: Checksum ERROR %08x != %08x\n", *tmpPtr, IMAGE_TUNE_FOOTER_MAGIC_NUM);  
    #endif
    return OSA_EFAIL;
  }

  // check checksum
  checkSum = IMAGE_TUNE_CalcPacketChecksum(packet);
  
  if(packet[IMAGE_TUNE_CHECK_SUM_OFFSET] != checkSum) {
    #ifdef IMAGE_TUNE_PACKET_DEBUG    
    OSA_ERROR("IMAGE TUNE\n");  
    #endif
    return OSA_EFAIL;
  }
  
  cmdInfo->commandId    = pRecvPacketHeader->commandId;
  cmdInfo->commandFlags = pRecvPacketHeader->commandFlags;  
  cmdInfo->prmSize      = pRecvPacketHeader->prmSize;
  cmdInfo->prm          = (Uint32*)((Uint8*)packet + IMAGE_TUNE_PACKET_HEADER_SIZE);
  
  return retVal;    
}


