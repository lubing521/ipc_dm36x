#include <csl_usrDrv.h>
#include <csl_ccdcIoctl.h>

CSL_Status CSL_ccdcOpen(CSL_CcdcHandle hndl)
{
  hndl->fd = CSL_usrDrvOpen(CSL_MODULE_ISIF);

  if (hndl->fd < 0)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_ccdcClose(CSL_CcdcHandle hndl)
{
  return CSL_usrDrvClose(hndl->fd);
}

CSL_Status CSL_ccdcHwSetup(CSL_CcdcHandle hndl, CSL_CcdcHwSetup * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_HW_SETUP, data);
}

CSL_Status CSL_ccdcHwReset(CSL_CcdcHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_HW_RESET, NULL);
}

CSL_Status CSL_ccdcEnable(CSL_CcdcHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_ENABLE, (void *) enable);
}

CSL_Status CSL_ccdcSdramOutEnable(CSL_CcdcHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SDRAM_OUT_ENABLE, (void *) enable);
}

CSL_Status CSL_ccdcSetInDataConfig(CSL_CcdcHandle hndl, CSL_CcdcInDataConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_DATA_IN_CONFIG, data);
}

CSL_Status CSL_ccdcSetSyncConfig(CSL_CcdcHandle hndl, CSL_CcdcSyncConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_SYNC_CONFIG, data);
}

CSL_Status CSL_ccdcSetMiscConfig(CSL_CcdcHandle hndl, CSL_CcdcMiscConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_MISC_CONFIG, data);
}

CSL_Status CSL_ccdcSetColPatConfig(CSL_CcdcHandle hndl, CSL_CcdcColPatConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_COL_PAT_CONFIG, data);
}

CSL_Status CSL_ccdcSetLinerizationConfig(CSL_CcdcHandle hndl, CSL_CcdcLinerizationConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_LINEARIZATION_CONFIG, data);
}

CSL_Status CSL_ccdcSetFmtCscInFrameConfig(CSL_CcdcHandle hndl, CSL_CcdcFmtCscInFrameConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_FMT_CSC_IN_FRAME_CONFIG, data);
}

CSL_Status CSL_ccdcSetFmtConfig(CSL_CcdcHandle hndl, CSL_CcdcFmtConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_FMT_CONFIG, data);
}

CSL_Status CSL_ccdcSetCscConfig(CSL_CcdcHandle hndl, CSL_CcdcCscConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_CSC_CONFIG, data);
}

CSL_Status CSL_ccdcSetClampConfig(CSL_CcdcHandle hndl, CSL_CcdcClampConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_CLAMP_CONFIG, data);
}

CSL_Status CSL_ccdcSetDfcLscOffsetConfig(CSL_CcdcHandle hndl, CSL_CcdcDfcLscOffsetConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_DFC_LSC_OFFSET_CONFIG, data);
}

CSL_Status CSL_ccdcSetDfcConfig(CSL_CcdcHandle hndl, CSL_CcdcDfcConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_DFC_CONFIG, data);
}

CSL_Status CSL_ccdcSetLscConfig(CSL_CcdcHandle hndl, CSL_CcdcLscConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_LSC_CONFIG, data);
}

CSL_Status CSL_ccdcGetLscStatus(CSL_CcdcHandle hndl, Bool32 * intSof, Bool32 * prefetchDone, Bool32 * prefetchError, Bool32 * lscDone)
{
  CSL_CcdcGetLscStatusPrm lscStatusPrm;
  Uint8   status;

  status = CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_GET_LSC_STATUS, &lscStatusPrm);

  *intSof = lscStatusPrm.intSof;
  *prefetchDone = lscStatusPrm.prefetchDone;
  *prefetchError = lscStatusPrm.prefetchError;
  *lscDone = lscStatusPrm.lscDone;

  return status;
}

CSL_Status CSL_ccdcSetGainOffsetConfig(CSL_CcdcHandle hndl, CSL_CcdcGainOffsetConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_GAIN_OFFSET_CONFIG, data);
}

CSL_Status CSL_ccdcSetSdramOutSizeConfig(CSL_CcdcHandle hndl, CSL_CcdcSdramOutSizeConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_SDRAM_OUT_SIZE_CONFIG, data);
}

CSL_Status CSL_ccdcSetSdramOutConfig(CSL_CcdcHandle hndl, CSL_CcdcSdramOutConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_SDRAM_OUT_CONFIG, data);
}

CSL_Status CSL_ccdcGetSdramOutSizeConfig(CSL_CcdcHandle hndl, CSL_CcdcSdramOutSizeConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_GET_SDRAM_OUT_SIZE_CONFIG, data);
}

CSL_Status CSL_ccdcSetDpcmConfig(CSL_CcdcHandle hndl, CSL_CcdcDpcmConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_DPCM_CONFIG, data);
}

CSL_Status CSL_ccdcSetRec656Config(CSL_CcdcHandle hndl, CSL_CcdcRec656Config * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_REC656_CONFIG, data);
}

CSL_Status CSL_ccdcSetFlashConfig(CSL_CcdcHandle hndl, CSL_CcdcFlashConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_FLASH_CONFIG, data);
}

CSL_Status CSL_ccdcSetVdIntConfig(CSL_CcdcHandle hndl, CSL_CcdcVdIntConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_VD_INT_CONFIG, data);
}

CSL_Status CSL_ccdcSetSdramOutAddr(CSL_CcdcHandle hndl, Uint8 * addr)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_SET_SDRAM_OUT_ADDR, (Uint8 *) addr);
}

CSL_Status CSL_ccdcWriteDfcTable(CSL_CcdcHandle hndl, CSL_CcdcVdfcTable * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_WRITE_DFC_TABLE, data);
}

CSL_Status CSL_ccdcReadDfcTable(CSL_CcdcHandle hndl, CSL_CcdcVdfcTable * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_READ_DFC_TABLE, data);
}

CSL_Status CSL_ccdcBufInit(CSL_CcdcHandle hndl, CSL_BufInit * bufInit)
{
  if(bufInit==NULL)
    return CSL_EFAIL;

  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_BUF_INIT, bufInit);
}

CSL_Status CSL_ccdcBufSwitchEnable(CSL_CcdcHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_BUF_SWITCH_ENABLE, (void *) enable);
}

CSL_Status CSL_ccdcBufGetFull(CSL_CcdcHandle hndl, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout)
{
  CSL_CcdcBufGetFullPrm bufGetFullPrm;

  bufGetFullPrm.buf = buf;
  bufGetFullPrm.minBuf = minBuf;
  bufGetFullPrm.timeout = timeout;

  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_BUF_GET_FULL, &bufGetFullPrm);
}

CSL_Status CSL_ccdcBufPutEmpty(CSL_CcdcHandle hndl, CSL_BufInfo * buf)
{
  if(buf==NULL)
    return CSL_EFAIL;
    
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_BUF_PUT_EMPTY, buf);
}

CSL_Status CSL_ccdcIntEnable(CSL_CcdcHandle hndl, Uint8 intType, Bool32 enable)
{
  CSL_CcdcIntEnablePrm intEnablePrm;

  intEnablePrm.intType = intType;
  intEnablePrm.enable = enable;

  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_INT_ENABLE, &intEnablePrm);
}

CSL_Status CSL_ccdcIntClear(CSL_CcdcHandle hndl, Uint8 intType)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_INT_CLEAR, (void *) (Uint32)intType);
}

CSL_Status CSL_ccdcIntWait(CSL_CcdcHandle hndl, Uint8 intType, Uint32 numIntWait)
{
  CSL_CcdcIntWaitPrm intWaitPrm;

  intWaitPrm.intType = intType;
  intWaitPrm.numIntWait = numIntWait;

  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_INT_WAIT, &intWaitPrm);
}

CSL_Status CSL_ccdcLock(CSL_CcdcHandle hndl, Uint32 timeout)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_LOCK, (void *) timeout);
}

CSL_Status CSL_ccdcUnlock(CSL_CcdcHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_UNLOCK, NULL);
}

CSL_Status CSL_ccdcLscBufInit(CSL_CcdcHandle hndl, CSL_BufInit * bufInit)
{
  if(bufInit==NULL)
    return CSL_EFAIL;

  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_LSC_BUF_INIT, bufInit);
}

CSL_Status CSL_ccdcLscBufSwitchEnable(CSL_CcdcHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_LSC_BUF_SWITCH_ENABLE, (void *) enable);
}

CSL_Status CSL_ccdcLscBufGetEmpty(CSL_CcdcHandle hndl, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout)
{
  CSL_CcdcBufGetFullPrm bufGetFullPrm;

  bufGetFullPrm.buf = buf;
  bufGetFullPrm.minBuf = minBuf;
  bufGetFullPrm.timeout = timeout;

  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_LSC_BUF_GET_EMPTY, &bufGetFullPrm);
}

CSL_Status CSL_ccdcLscBufPutFull(CSL_CcdcHandle hndl, CSL_BufInfo * buf)
{
  if(buf==NULL)
    return CSL_EFAIL;
    
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_LSC_BUF_PUT_FULL, buf);
}

CSL_Status CSL_ccdcLscSetGainTableAddr(CSL_CcdcHandle hndl, Uint8 *inAddr)
{
  if(inAddr==NULL)
    return CSL_EFAIL;
    
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_LSC_SET_GAIN_TABLE_ADDR, inAddr);
}

CSL_Status CSL_ccdcLscSetOffsetTableAddr(CSL_CcdcHandle hndl, Uint8 *inAddr)
{
  if(inAddr==NULL)
    return CSL_EFAIL;
    
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_LSC_SET_OFFSET_TABLE_ADDR, inAddr);
}

CSL_Status CSL_ccdcLscEnable(CSL_CcdcHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_LSC_ENABLE, (void*)enable);
}

CSL_Status CSL_ccdcLscBufSetGainTableSize(CSL_CcdcHandle hndl, Uint32 gainTableSize)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_CCDC_CMD_LSC_BUF_SET_GAIN_TABLE_SIZE, (void*)gainTableSize);
}


