
#include <imageTunePriv.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int IMAGE_TUNE_ClientOpen(char *ip_addr, Uint32 port)
{
  int sin_size;
  struct hostent *host;
  struct sockaddr_in server;

  host = gethostbyname(ip_addr);
  
  gIMAGE_TUNE_ctrl.clientSocketId = socket(AF_INET, SOCK_STREAM, 0);
  if (gIMAGE_TUNE_ctrl.clientSocketId < 0) {
    #ifdef IMAGE_TUNE_COMM_DEBUG
    OSA_ERROR("IMAGE TUNE\n");
    #endif
    return OSA_EFAIL;
  }

  server.sin_family = AF_INET;  
  server.sin_port = htons(port);
  server.sin_addr = *((struct in_addr *)host->h_addr);
  bzero(&(server.sin_zero),8);

  sin_size = sizeof(server);

  if (connect(gIMAGE_TUNE_ctrl.clientSocketId, (struct sockaddr *)&server, sin_size) == -1)
  {
    #ifdef IMAGE_TUNE_COMM_DEBUG
    OSA_ERROR("IMAGE TUNE\n");
    #endif
    return OSA_EFAIL;
  }
  
  return OSA_SOK;
}

int IMAGE_TUNE_ClientClose()
{
  if(gIMAGE_TUNE_ctrl.clientSocketId < 0)
    return OSA_EFAIL;

  return close(gIMAGE_TUNE_ctrl.clientSocketId);
}

int IMAGE_TUNE_ClientSendData(Uint8 *dataBuf, Uint32 dataSize)
{
  int actDataSize, totalDataSize=0;
  
  while(totalDataSize < (int)dataSize) {  
    actDataSize = send(gIMAGE_TUNE_ctrl.clientSocketId, dataBuf, dataSize, 0);    
    if(actDataSize<=0)
      break;
    totalDataSize += actDataSize;
    dataBuf += actDataSize;
  }
  
  if(totalDataSize!=(int)dataSize) {
    #ifdef IMAGE_TUNE_COMM_DEBUG
    OSA_ERROR("IMAGE TUNE\n");
    #endif
    return OSA_EFAIL;
  }
    

  return OSA_SOK;
}

int IMAGE_TUNE_ClientRecvData(Uint8 *dataBuf, Uint32 *dataSize)
{
  int actDataSize, totalDataSize=0;
  
  while(totalDataSize < (int)*dataSize) {  
    actDataSize = recv(gIMAGE_TUNE_ctrl.clientSocketId, dataBuf, *dataSize, 0);    
    if(actDataSize<=0)
      break;
    totalDataSize += actDataSize;
    dataBuf += actDataSize;
  }
  
  if(totalDataSize!=(int)*dataSize) {
    #ifdef IMAGE_TUNE_COMM_DEBUG
    OSA_ERROR("IMAGE TUNE\n");
    #endif
    return OSA_EFAIL;
  }

  return OSA_SOK;
}


int IMAGE_TUNE_ServerOpen(Uint32 port)
{
  struct sockaddr_in server;
  
  gIMAGE_TUNE_ctrl.serverPort = port;
  
  gIMAGE_TUNE_ctrl.serverSocketId = socket(AF_INET, SOCK_STREAM, 0);  
  if (gIMAGE_TUNE_ctrl.serverSocketId < 0) {
    #ifdef IMAGE_TUNE_COMM_DEBUG
    OSA_ERROR("IMAGE TUNE 1 \n");
    #endif
    return OSA_EFAIL;
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port);
  bzero(&(server.sin_zero),8);
  
  if (bind(gIMAGE_TUNE_ctrl.serverSocketId, (struct sockaddr *)&server, sizeof(server)) == -1) {
    #ifdef IMAGE_TUNE_COMM_DEBUG
    OSA_ERROR("IMAGE TUNE 2 \n");
    #endif
    return OSA_EFAIL;
  }
  
  if(listen(gIMAGE_TUNE_ctrl.serverSocketId, 5) < 0) {
    #ifdef IMAGE_TUNE_COMM_DEBUG
    OSA_ERROR("IMAGE TUNE 3 \n");
    #endif
    return OSA_EFAIL;
  }
  
  return OSA_SOK;
}

int IMAGE_TUNE_ServerWaitConnect()
{
  unsigned int sin_size; 
  struct sockaddr_in client;  

  if (gIMAGE_TUNE_ctrl.serverSocketId < 0) {
    OSA_ERROR("Illegal server socket ID\n");
    return OSA_EFAIL;
  }

  sin_size = sizeof(struct sockaddr_in);

  gIMAGE_TUNE_ctrl.connectedSocketId = accept(gIMAGE_TUNE_ctrl.serverSocketId, (struct sockaddr *)&client, &sin_size);
  
  if(gIMAGE_TUNE_ctrl.connectedSocketId<0){
    OSA_ERROR("Illegal client socket ID\n");
    return OSA_EFAIL;
  }
    
  return OSA_SOK;  
}

int IMAGE_TUNE_ServerClose()
{
  if(gIMAGE_TUNE_ctrl.serverSocketId < 0)
    return OSA_EFAIL;

  return close(gIMAGE_TUNE_ctrl.serverSocketId);
}

int IMAGE_TUNE_ServerSendData(Uint8 *dataBuf, Uint32 dataSize)
{
  int actDataSize=0; 
  
  while(dataSize > 0 ) {  
    actDataSize = send(gIMAGE_TUNE_ctrl.connectedSocketId, dataBuf, dataSize, 0);    
    #ifdef IMAGE_TUNE_COMM_DEBUG
    OSA_printf(" IMAGE TUNE: Send Data %d (%d)\n", actDataSize, dataSize);
    #endif
    if(actDataSize<=0)
      break;
    dataBuf += actDataSize;
    dataSize -= actDataSize;
  }
  
  if( dataSize > 0 ) {
    #ifdef IMAGE_TUNE_COMM_DEBUG
    OSA_ERROR("IMAGE TUNE\n");
    #endif
    return OSA_EFAIL;
  }

  return OSA_SOK;
}

int IMAGE_TUNE_ServerRecvData(Uint8 *dataBuf, Uint32 *dataSize)
{
  int actDataSize, recvDataSize;
  
  recvDataSize = *dataSize;
  
  while( recvDataSize > 0 ) {  
    actDataSize = recv(gIMAGE_TUNE_ctrl.connectedSocketId, dataBuf, recvDataSize, 0);    
    #ifdef IMAGE_TUNE_COMM_DEBUG
    OSA_printf(" IMAGE TUNE: Recv Data %d (%d)\n", actDataSize, recvDataSize);
    #endif
    if(actDataSize<=0)
      break;
    dataBuf += actDataSize;
    recvDataSize -= actDataSize;
  }
  
  if( recvDataSize > 0 ) {
    #ifdef IMAGE_TUNE_COMM_DEBUG
    //OSA_ERROR("IMAGE TUNE\n");
    #endif
    return OSA_EFAIL;
  }

  return OSA_SOK;
}



