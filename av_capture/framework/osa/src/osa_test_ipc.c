
#include <osa_ipc.h>

#define OSA_TEST_IPC_SEM_MAX_VALUE  5

int OSA_TEST_ipcServerMain(int argc, char **argv);
int OSA_TEST_ipcClientMain(int argc, char **argv);

void OSA_TEST_showUsage(char *name)
{
  OSA_printf("%s [--client] \n", name);
}

int OSA_ipcTestMain(int argc, char **argv)
{
 
  if(argc>1) {
  
    if(strcmp(argv[1], "--client")==0) {
    
      OSA_TEST_ipcClientMain(argc, argv);
      
    } else {
      OSA_TEST_showUsage(argv[0]);
      exit(0);
    }
      
  } else {
  
    OSA_TEST_ipcServerMain(argc, argv);
  }
    
  return 0;
}

// client
int OSA_TEST_ipcClientMain(int argc, char **argv)
{
  OSA_IpcMbxHndl mbx;
  OSA_IpcShmHndl shm;  
  OSA_IpcSemHndl sem;  
  OSA_IpcMsgHndl msgData;
  Uint32 mbxKey, shmKey, semKey;
  int status=OSA_EFAIL;
  char *shmMemPtr;
  
  mbxKey = OSA_ipcMakeKey(".", 'M');
  if(mbxKey==OSA_IPC_KEY_INVALID) {
    OSA_ERROR("OSA_ipcMakeKey() \n");
    return status;
  }
  
  shmKey = OSA_ipcMakeKey(".", 'S');
  if(shmKey==OSA_IPC_KEY_INVALID) {
    OSA_ERROR("OSA_ipcMakeKey() \n");
    return status;
  }
  
  semKey = OSA_ipcMakeKey(".", 's');
  if(semKey==OSA_IPC_KEY_INVALID) {
    OSA_ERROR("OSA_ipcMakeKey() \n");
    return status;
  }
    
  status = OSA_ipcMbxOpen(&mbx, mbxKey, OSA_IPC_FLAG_OPEN);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_ipcMbxOpen() \n");
    return status;
  }
  
  shmMemPtr = OSA_ipcShmOpen(&shm, shmKey, KB, OSA_IPC_FLAG_OPEN);
  if(shmMemPtr==NULL) {    
    OSA_ERROR("OSA_ipcShmOpen() \n");
    return OSA_EFAIL;
  }
  
  status = OSA_ipcSemOpen(&sem, semKey, 1, 0, OSA_TEST_IPC_SEM_MAX_VALUE, OSA_IPC_FLAG_OPEN);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_ipcMbxOpen() \n");
    return status;
  }
  
  if(argc>2)
    strcpy((char*)msgData.data, argv[2]);
  else  
    strcpy((char*)msgData.data, "hello");
    
  if(strcmp((char*)msgData.data, "shm")==0) {
    if(argc>3)
      strcpy(shmMemPtr, argv[3]);
    else
      strcpy(shmMemPtr, "we are in shared memory now");
  }

  if(strcmp((char*)msgData.data, "semunlock")==0) {
    status = OSA_ipcSemUnlock(&sem, 0, OSA_TIMEOUT_FOREVER);
    if(status!=OSA_SOK) {
      OSA_ERROR("OSA_ipcSemUnlock() \n");
      return status;
    }    
  }

  status = OSA_ipcMbxSend(&mbx, &msgData, sizeof(msgData.data));
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_ipcMbxSend() \n");
  }
  
  return status;
}


// server
int OSA_TEST_ipcServerMain(int argc, char **argv)
{
  OSA_IpcMbxHndl mbx;
  OSA_IpcMsgHndl msgData;
  OSA_IpcShmHndl shm;
  OSA_IpcSemHndl sem;  
  Uint32 mbxKey, shmKey, semKey;
  char *shmMemPtr;
  int status=OSA_EFAIL;
 
  mbxKey = OSA_ipcMakeKey(".", 'M');
  if(mbxKey==OSA_IPC_KEY_INVALID) {
    OSA_ERROR("OSA_ipcMakeKey() \n");
    return status;
  }

  shmKey = OSA_ipcMakeKey(".", 'S');
  if(shmKey==OSA_IPC_KEY_INVALID) {
    OSA_ERROR("OSA_ipcMakeKey() \n");
    return status;
  }

  semKey = OSA_ipcMakeKey(".", 's');
  if(shmKey==OSA_IPC_KEY_INVALID) {
    OSA_ERROR("OSA_ipcMakeKey() \n");
    return status;
  }
    
  status = OSA_ipcMbxOpen(&mbx, mbxKey, OSA_IPC_FLAG_CREATE);
  if(status!=OSA_SOK) {
    // Check if Mbx is already open'ed
    status = OSA_ipcMbxOpen(&mbx, mbxKey, OSA_IPC_FLAG_OPEN);
    if(status!=OSA_SOK) {
      OSA_ERROR("OSA_ipcMbxOpen() \n");
      return status;
    }  
  }
  
  shmMemPtr = OSA_ipcShmOpen(&shm, shmKey, KB, OSA_IPC_FLAG_CREATE);
  if(shmMemPtr==NULL) {
    shmMemPtr = OSA_ipcShmOpen(&shm, shmKey, KB, OSA_IPC_FLAG_OPEN);
    if(shmMemPtr==NULL) {    
      OSA_ERROR("OSA_ipcShmOpen() \n");
      goto exit_shm_error;
    }
  }

  status = OSA_ipcSemOpen(&sem, semKey, 1, 0, OSA_TEST_IPC_SEM_MAX_VALUE, OSA_IPC_FLAG_CREATE);
  if(status!=OSA_SOK) {
    status = OSA_ipcSemOpen(&sem, semKey, 1, 0, OSA_TEST_IPC_SEM_MAX_VALUE, OSA_IPC_FLAG_OPEN);
    if(status!=OSA_SOK) {
      OSA_ERROR("OSA_ipcSemOpen() \n");
      goto exit_sem_error;      
    }
  }

  OSA_printf("\n IPC TEST Server Running !!!\n");
        
  while(1) {
  
    msgData.data[0]=0;
    
    status = OSA_ipcMbxRecv(&mbx, &msgData, sizeof(msgData.data));
    if(status < 0) {
      OSA_ERROR("OSA_ipcMbxRecv() \n");    
      break;
    }
    
    OSA_printf("\n IPC TEST Server -> [%s]\n", msgData.data);
    if(strcmp((char*)msgData.data, "quit")==0)
      break;
    if(strcmp((char*)msgData.data, "shm")==0) {
      OSA_printf(" IPC TEST Server SHM -> [%s]\n", shmMemPtr);    
    }
    if(strcmp((char*)msgData.data, "semlock")==0) {
      OSA_ipcSemLock(&sem, 0, OSA_TIMEOUT_FOREVER);
      OSA_printf(" IPC TEST Server SEM -> %d\n", OSA_ipcSemGetVal(&sem, 0) );    
    }
    if(strcmp((char*)msgData.data, "semunlock")==0) {
      OSA_printf(" IPC TEST Server SEM -> %d\n", OSA_ipcSemGetVal(&sem, 0) );    
    }
  }

  OSA_ipcSemClose(&sem);  
exit_sem_error:  
  OSA_ipcShmClose(&shm);
exit_shm_error:  
  OSA_ipcMbxClose(&mbx);

  OSA_printf("\n IPC TEST Server Stopped !!!\n");
  
  return status;
}

