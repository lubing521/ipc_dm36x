
#include <osa_ipc.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>

#define OSA_IPC_MBX_MSG_TYPE_ID   (0x0000ACBD)
#define OSA_IPC_SEM_MEMBERS_MAX   (32)

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short  *array;
} sem_arg;

Uint32 OSA_ipcMakeKey(char *path, char id)
{
  key_t key;

  key = ftok(path, id);

  return (Uint32)key;
}

// shared memory
char *OSA_ipcShmOpen(OSA_IpcShmHndl *hndl, Uint32 key, Uint32 size, Uint32 flags)
{
  hndl->ptr=NULL;

  if(flags & OSA_IPC_FLAG_CREATE) {
    hndl->id = shmget(key, size, IPC_CREAT|IPC_EXCL|0666);
  } else {
    hndl->id = shmget(key, size, 0);
  }

  if(hndl->id < 0)
    return hndl->ptr;

  hndl->ptr = shmat(hndl->id, 0, 0);

  if(hndl->ptr == (char*)-1)
    hndl->ptr=NULL;

  return hndl->ptr;
}

int   OSA_ipcShmClose(OSA_IpcShmHndl *hndl)
{
  return shmctl(hndl->id, IPC_RMID, 0);
}

// message box
int OSA_ipcMbxOpen(OSA_IpcMbxHndl *hndl, Uint32 key, Uint32 flags)
{
  if(flags & OSA_IPC_FLAG_CREATE) {
    hndl->id = msgget(key, IPC_EXCL|IPC_CREAT|0660);
  } else {
    hndl->id = msgget(key, 0);
  }

  if(hndl->id<0)
    return OSA_EFAIL;

  return OSA_SOK;
}

int OSA_ipcMbxSend(OSA_IpcMbxHndl *hndl, OSA_IpcMsgHndl *msgData, Uint32 msgSize)
{
  int status;

  msgData->type = OSA_IPC_MBX_MSG_TYPE_ID;

  status = msgsnd(hndl->id, msgData, msgSize, 0);

  return status;
}

int OSA_ipcMbxRecv(OSA_IpcMbxHndl *hndl, OSA_IpcMsgHndl *msgData, Uint32 msgSize)
{
  int status;

  msgData->type = OSA_IPC_MBX_MSG_TYPE_ID;

  status = msgrcv(hndl->id, msgData, msgSize, msgData->type, 0);

  return status;
}

int OSA_ipcMbxClose(OSA_IpcMbxHndl *hndl)
{
  return msgctl(hndl->id, IPC_RMID, 0);
}

// semaphores
int OSA_ipcSemOpen(OSA_IpcSemHndl *hndl, Uint32 key, Uint32 members, Uint32 initVal, Uint32 maxVal, Uint32 flags)
{
	int cntr;
	union semun semopts;

  hndl->members = members;
  hndl->maxVal = maxVal;

  if(initVal > maxVal)
    initVal = maxVal;

  if(flags & OSA_IPC_FLAG_CREATE) {

  	if(members > OSA_IPC_SEM_MEMBERS_MAX)
	    return OSA_EFAIL;

    hndl->id = semget(key, members, IPC_CREAT|IPC_EXCL|0666);
	  if(hndl->id < 0 )
      return OSA_EFAIL;

	  semopts.val = initVal;

	  for(cntr=0; cntr<members; cntr++)
		  semctl(hndl->id, cntr, SETVAL, semopts);

  } else {
    hndl->id = semget(key, 0, 0666);
  }

  if(hndl->id < 0 )
    return OSA_EFAIL;

  return OSA_SOK;
}

int OSA_ipcSemLock(OSA_IpcSemHndl *hndl, Uint32 member, Uint32 timeout)
{
	struct sembuf sem_op={ 0, -1, 0};

	if( member < 0 || member > (hndl->members-1) )
	{
		return OSA_EFAIL;
	}

	sem_op.sem_num = member;

  if(timeout==OSA_TIMEOUT_NONE)
    sem_op.sem_flg |= IPC_NOWAIT;

	if( (semop(hndl->id, &sem_op, 1)) == -1)
	{
    return OSA_EFAIL;
	}

  return OSA_SOK;
}

int OSA_ipcSemUnlock(OSA_IpcSemHndl *hndl, Uint32 member, Uint32 timeout)
{
	struct sembuf sem_op={ 0, 1, 0};

	if( member < 0 || member > (hndl->members-1) )
	{
		return OSA_EFAIL;
	}

  if(OSA_ipcSemGetVal(hndl, member) >= hndl->maxVal)
    return OSA_SOK;

	sem_op.sem_num = member;

  if(timeout==OSA_TIMEOUT_NONE)
    sem_op.sem_flg |= IPC_NOWAIT;

	if( (semop(hndl->id, &sem_op, 1)) == -1)
	{
    return OSA_EFAIL;
	}

  return OSA_SOK;
}

int OSA_ipcSemGetVal(OSA_IpcSemHndl *hndl, Uint32 member)
{
  return semctl(hndl->id, member, GETVAL, 0);
}

int OSA_ipcSemClose(OSA_IpcSemHndl *hndl)
{
  return semctl(hndl->id, 0, IPC_RMID, 0);
}

