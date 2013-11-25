/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/



#include <csl_h3a.h>

CSL_Status CSL_h3aBufInit(CSL_H3aHandle hndl, Uint8 h3aModId, CSL_BufInit * bufInit)
{
  CSL_Status status;

  if (h3aModId >= CSL_H3A_MOD_MAX)
    return CSL_EFAIL;

  status = CSL_bufInit(&hndl->outBuf[h3aModId], bufInit);

  hndl->curBufInfo[h3aModId].id = CSL_BUF_ID_INVALID;

  if(h3aModId==CSL_H3A_MOD_AEWB)
    CSL_h3aAewbSetOutAddr(hndl, (Uint8 *) bufInit->bufAddr[0]);
  else if(h3aModId==CSL_H3A_MOD_AF) 
    CSL_h3aAfSetOutAddr(hndl, (Uint8 *) bufInit->bufAddr[0]);

  return status;
}

CSL_Status CSL_h3aBufSwitchEnable(CSL_H3aHandle hndl, Uint8 h3aModId, Bool32 enable)
{
  if (h3aModId >= CSL_H3A_MOD_MAX)
    return CSL_EFAIL;

  hndl->outBufSwitchEnable[h3aModId] = enable;

  return CSL_SOK;
}

CSL_Status CSL_h3aBufGetFull(CSL_H3aHandle hndl, Uint8 h3aModId, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout)
{
  CSL_Status status;

  if (h3aModId >= CSL_H3A_MOD_MAX)
    return CSL_EFAIL;

  status = CSL_bufGetFull(&hndl->outBuf[h3aModId], buf, minBuf, timeout);

  return status;
}

CSL_Status CSL_h3aBufPutEmpty(CSL_H3aHandle hndl, Uint8 h3aModId, CSL_BufInfo * buf)
{
  CSL_Status status;

  if (h3aModId >= CSL_H3A_MOD_MAX)
    return CSL_EFAIL;

  status = CSL_bufPutEmpty(&hndl->outBuf[h3aModId], buf);

  return status;
}

CSL_Status CSL_h3aBufSwitch(CSL_H3aHandle hndl, Uint8 h3aModId, Uint32 timestamp, Uint32 count)
{
  CSL_Status status = CSL_SOK;

  if (h3aModId >= CSL_H3A_MOD_MAX)
    return CSL_EFAIL;

  if (hndl->outBufSwitchEnable[h3aModId]) {
    status = CSL_bufSwitchFull(&hndl->outBuf[h3aModId], &hndl->curBufInfo[h3aModId], 1, timestamp, count);

    if (hndl->curBufInfo[h3aModId].id != CSL_BUF_ID_INVALID) {
      if(h3aModId==CSL_H3A_MOD_AEWB)
        CSL_h3aAewbSetOutAddr(hndl, (Uint8 *) hndl->curBufInfo[h3aModId].addr);
      else if(h3aModId==CSL_H3A_MOD_AF) 
        CSL_h3aAfSetOutAddr(hndl, (Uint8 *) hndl->curBufInfo[h3aModId].addr);
    }
  }

  return status;
}
