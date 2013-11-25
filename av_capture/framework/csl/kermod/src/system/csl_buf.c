/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_buf.h>


CSL_Status CSL_bufCreate(CSL_BufHndl * hndl)
{
  CSL_Status status;

  if (hndl == NULL)
    return CSL_EFAIL;

  memset(hndl, 0, sizeof(CSL_BufHndl));

  status = CSL_sysFlagCreate(&hndl->fullFlag);

  return status;
}

CSL_Status CSL_bufDelete(CSL_BufHndl * hndl)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL)
    return CSL_EFAIL;

  status |= CSL_sysFlagDelete(&hndl->fullFlag);

  return status;
}

CSL_Status CSL_bufInit(CSL_BufHndl * hndl, CSL_BufInit * bufInit)
{
  CSL_Status status = CSL_SOK;
  int     i;

  if (bufInit->numBuf > CSL_BUF_NUM_MAX || bufInit->curBuf >= CSL_BUF_NUM_MAX) {
    return CSL_EFAIL;
  }

  hndl->numBuf = bufInit->numBuf;
  hndl->emptyIdx = bufInit->curBuf;
  hndl->fullIdx = bufInit->curBuf;

  for (i = 0; i < hndl->numBuf; i++) {
    hndl->bufInfo[i].id = i;
    hndl->bufInfo[i].addr = bufInit->bufAddr[i];
    hndl->bufInfo[i].state = CSL_BUF_STATE_EMPTY;
    hndl->bufInfo[i].timestamp = 0;
    hndl->bufInfo[i].count = 0;
  }
  


  status |= CSL_sysFlagClear(&hndl->fullFlag);

  return status;
}

/**
  Internal API,  should not be used by user
  Checks if 'minCount' number of buffers have state 'state' starting from buffer ID 'bufId'
*/
inline Bool32 CSL_bufFind(CSL_BufHndl * hndl, Uint32 bufId, Uint32 minCount, Uint32 state)
{
  Bool32    found = TRUE;

  while (minCount) {
    if (hndl->bufInfo[bufId].state != state) {
      found = FALSE;
      break;
    }
    bufId = (bufId + 1) % hndl->numBuf;

    minCount--;
  }

  return found;
}

Bool32 CSL_bufIsEmpty(CSL_BufHndl * hndl, Uint32 minBuf)
{
  Bool32 found;
  unsigned long intState;

  CSL_intGlobalDisable(intState);

  found = CSL_bufFind(hndl, hndl->emptyIdx, minBuf, CSL_BUF_STATE_EMPTY);

  CSL_intGlobalRestore(intState);

  return found;
}

CSL_Status CSL_bufGetEmpty(CSL_BufHndl * hndl, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout)
{
  CSL_Status status = CSL_EFAIL;
  Bool32    found;
  unsigned long intState;

  if (minBuf > hndl->numBuf)
    minBuf = hndl->numBuf;

  buf->id = CSL_BUF_ID_INVALID;

  CSL_intGlobalDisable(intState);

  found = CSL_bufFind(hndl, hndl->emptyIdx, minBuf, CSL_BUF_STATE_EMPTY);

  if (found) {

    hndl->bufInfo[hndl->emptyIdx].state = CSL_BUF_STATE_LOCKED;

    buf->id = hndl->emptyIdx;
    buf->addr = hndl->bufInfo[buf->id].addr;
    buf->timestamp = hndl->bufInfo[buf->id].timestamp;
    buf->state = hndl->bufInfo[buf->id].state;
    buf->count = hndl->bufInfo[buf->id].count;

    hndl->emptyIdx = (hndl->emptyIdx + 1) % hndl->numBuf;

    status = CSL_SOK;
  }

  CSL_intGlobalRestore(intState);

  return status;
}

CSL_Status CSL_bufPutFull(CSL_BufHndl * hndl, CSL_BufInfo * buf)
{
  unsigned long intState;
  
  CSL_intGlobalDisable(intState);
  
  hndl->bufInfo[buf->id].count = buf->count;
  hndl->bufInfo[buf->id].timestamp = buf->timestamp;
  hndl->bufInfo[buf->id].width     = buf->width;
  hndl->bufInfo[buf->id].height    = buf->height;
  hndl->bufInfo[buf->id].state = CSL_BUF_STATE_FULL;

  CSL_intGlobalRestore(intState);

  CSL_sysFlagSet(&hndl->fullFlag);

  return CSL_SOK;
}

Bool32 CSL_bufIsFull(CSL_BufHndl * hndl, Uint32 minBuf)
{
  Bool32 found;
  unsigned long intState;

  CSL_intGlobalDisable(intState);

  found = CSL_bufFind(hndl, hndl->fullIdx, minBuf, CSL_BUF_STATE_FULL);

  CSL_intGlobalRestore(intState);

  return found;
}

CSL_Status CSL_bufGetFull(CSL_BufHndl * hndl, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout)
{
  CSL_Status status = CSL_SOK;
  Bool32    found;
  unsigned long intState;


  if (minBuf > hndl->numBuf)
    minBuf = hndl->numBuf;

  if (timeout != CSL_SYS_TIMEOUT_NONE) {
    CSL_sysFlagClear(&hndl->fullFlag);
  }
  
  buf->id = CSL_BUF_ID_INVALID;

try_again:

  CSL_intGlobalDisable(intState);
  found = CSL_bufFind(hndl, hndl->fullIdx, minBuf, CSL_BUF_STATE_FULL);

  if (found) {

    hndl->bufInfo[hndl->fullIdx].state = CSL_BUF_STATE_LOCKED;

    buf->id = hndl->fullIdx;
    buf->addr = hndl->bufInfo[buf->id].addr;
    buf->timestamp = hndl->bufInfo[buf->id].timestamp;
    buf->state = hndl->bufInfo[buf->id].state;
    buf->count = hndl->bufInfo[buf->id].count;
    buf->height= hndl->bufInfo[buf->id].height;
    buf->width = hndl->bufInfo[buf->id].width;

    hndl->fullIdx = (hndl->fullIdx + 1) % hndl->numBuf;

    CSL_intGlobalRestore(intState);

  } else {

    CSL_intGlobalRestore(intState);

    if (timeout != CSL_SYS_TIMEOUT_NONE) {
      status = CSL_sysFlagWait(&hndl->fullFlag, CSL_SYS_TIMEOUT_FOREVER);
      if (status != CSL_SOK)
        return status;

      goto try_again;
    }
  }

  if (!found)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_bufPutEmpty(CSL_BufHndl * hndl, CSL_BufInfo * buf)
{
  unsigned long intState;
  
  CSL_intGlobalDisable(intState);

  hndl->bufInfo[buf->id].count = buf->count;
  hndl->bufInfo[buf->id].timestamp = buf->timestamp;
  hndl->bufInfo[buf->id].state = CSL_BUF_STATE_EMPTY;
  
  CSL_intGlobalRestore(intState);  

  return CSL_SOK;
}

CSL_Status CSL_bufSwitchFull(CSL_BufHndl * hndl, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timestamp, Uint32 count)
{
  CSL_Status status;
  CSL_BufInfo newBuf;

  status = CSL_bufGetEmpty(hndl, &newBuf, minBuf, CSL_SYS_TIMEOUT_NONE);

  if (status == CSL_SOK) {

    if (buf->id != CSL_BUF_ID_INVALID) {

      buf->timestamp = timestamp;
      buf->count = count;      
      CSL_bufPutFull(hndl, buf);
    }

    memcpy(buf, &newBuf, sizeof(CSL_BufInfo));
  }

  return status;
}

