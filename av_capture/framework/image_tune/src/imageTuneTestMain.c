
#include <imageTune.h>
#include <csl_soc.h>

void IMAGE_TUNE_TEST_showUsage(char *name)
{
  OSA_printf("\n");  
  OSA_printf("Server run,\n");
  OSA_printf("%s & \n", name);
  OSA_printf("\n");  
  OSA_printf("Client run,\n");
  OSA_printf("%s -s <server ip addr> -p <port num> -m <message string>\n", name);  
  OSA_printf("\n");    
}

int IMAGE_TUNE_sysTestMain(int argc, char **argv)
{
  int retVal, port, i;
  char serverIpAddr[20];
  char message[256];
  IMAGE_TUNE_CmdInfo cmdInfo, statusInfo;
  Uint32 cmd;
    
  if(argc == 3 ) {
    if(strcmp(argv[2], "-h")==0) {
      IMAGE_TUNE_TEST_showUsage(argv[0]);  
      return OSA_SOK;
    }
  }

  if(argc == 2) {
    OSA_printf(" IMAGE TUNE: Server Starting ...\n");
    
    CSL_sysInit();

    IMAGE_TUNE_Init();

    retVal = IMAGE_TUNE_ServerOpen(IMAGE_TUNE_SERVER_DEFAULT_PORT);
    if(retVal == OSA_SOK) {
      retVal = IMAGE_TUNE_ServerRun();
      retVal = IMAGE_TUNE_ServerClose();
    }
    
    IMAGE_TUNE_Exit();
    
    CSL_sysExit();

    if(retVal!=OSA_SOK) 
      OSA_printf(" IMAGE TUNE: Server Stopped !!!\n")    

    return retVal;
  }

  strcpy(serverIpAddr, IMAGE_TUNE_SERVER_DEFAULT_IP_ADDR);
  strcpy(message, "hi there");
  port = IMAGE_TUNE_SERVER_DEFAULT_PORT;
  cmd = IMAGE_TUNE_SYS_ECHO_STRING;

  for(i=2; i<argc; i++) {
    if(strcmp(argv[i], "-s")==0) { // server ip address

      i++;
      strcpy(serverIpAddr, argv[i]);
    }
    if(strcmp(argv[i], "-p")==0) { // server port number

      i++;
      port = atoi(argv[i]);
    }
    if(strcmp(argv[i], "-m")==0) { // message string

      i++;
      strcpy(message, argv[i]);
      cmd = IMAGE_TUNE_SYS_ECHO_STRING;
    }
    if(strcmp(argv[i], "-v")==0) { // get server version

      cmd = IMAGE_TUNE_SYS_GET_VERSION;
    }
  }

  switch(cmd) {

    case IMAGE_TUNE_SYS_GET_VERSION:

      cmdInfo.commandId = cmd;
      cmdInfo.commandFlags = IMAGE_TUNE_CMD_FLAG_SEND_REPLY;
      cmdInfo.prmSize = 0;
      cmdInfo.prm = NULL;

      retVal = IMAGE_TUNE_ClientSendCmd(serverIpAddr, port, &cmdInfo, &statusInfo);
      if(retVal==OSA_SOK) {
        OSA_printf(" IMAGE TUNE: Server version 0x%08x\n", statusInfo.prm[0]);
      }
      break;

    case IMAGE_TUNE_SYS_ECHO_STRING:

      cmdInfo.commandId = cmd;
      cmdInfo.commandFlags = IMAGE_TUNE_CMD_FLAG_SEND_REPLY;
      cmdInfo.prm = (Uint32*)message;
      cmdInfo.prmSize = strlen(message)+1;

      retVal = IMAGE_TUNE_ClientSendCmd(serverIpAddr, port, &cmdInfo, &statusInfo);
      break;
  }

  return retVal;
}

