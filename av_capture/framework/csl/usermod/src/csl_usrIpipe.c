#include <csl_usrDrv.h>
#include <csl_ipipeIoctl.h>

CSL_Status CSL_ipipeOpen(CSL_IpipeHandle hndl)
{
  hndl->fd = CSL_usrDrvOpen(CSL_MODULE_IPIPE);

  if (hndl->fd < 0)
    return CSL_EFAIL;

  return CSL_SOK;
}

CSL_Status CSL_ipipeClose(CSL_IpipeHandle hndl)
{
  return CSL_usrDrvClose(hndl->fd);
}

CSL_Status CSL_ipipeHwSetup(CSL_IpipeHandle hndl, CSL_IpipeHwSetup * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_HW_SETUP, data);
}

CSL_Status CSL_ipipeHwReset(CSL_IpipeHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_HW_RESET, NULL);
}

CSL_Status CSL_ipipeSetColPatConfig(CSL_IpipeHandle hndl, CSL_IpipeColPatConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_COL_PAT_CONFIG, data);
}

CSL_Status CSL_ipipeSetClkConfig(CSL_IpipeHandle hndl, CSL_IpipeClkConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_CLK_COFNIG, data);
}

CSL_Status CSL_ipipeSetInFrameConfig(CSL_IpipeHandle hndl, CSL_IpipeInFrameConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_IN_FRAME_CONFIG, data);
}

CSL_Status CSL_ipipeSetLscConfig(CSL_IpipeHandle hndl, CSL_IpipeLscConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_LSC_CONFIG, data);
}

CSL_Status CSL_ipipeSetDpcConfig(CSL_IpipeHandle hndl, CSL_IpipeDpcConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_DPC_CONFIG, data);
}

CSL_Status CSL_ipipeSetNf1Config(CSL_IpipeHandle hndl, CSL_IpipeNfConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_NF1_CONFIG, data);
}

CSL_Status CSL_ipipeSetNf2Config(CSL_IpipeHandle hndl, CSL_IpipeNfConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_NF2_CONFIG, data);
}

CSL_Status CSL_ipipeSetGicConfig(CSL_IpipeHandle hndl, CSL_IpipeGicConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_GIC_CONFIG, data);
}

CSL_Status CSL_ipipeSetWbConfig(CSL_IpipeHandle hndl, CSL_IpipeWbConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_WB_CONFIG, data);
}

CSL_Status CSL_ipipeSetCfaConfig(CSL_IpipeHandle hndl, CSL_IpipeCfaConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_CFA_CONFIG, data);
}

CSL_Status CSL_ipipeSetRgb2Rgb1Config(CSL_IpipeHandle hndl, CSL_IpipeRgb2RgbConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_RGB2RGB1_CONFIG, data);
}

CSL_Status CSL_ipipeSetGammaConfig(CSL_IpipeHandle hndl, CSL_IpipeGammaConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_GAMMA_CONFIG, data);
}

CSL_Status CSL_ipipeSetRgb2Rgb2Config(CSL_IpipeHandle hndl, CSL_IpipeRgb2RgbConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_RGB2RGB2_CONFIG, data);
}

CSL_Status CSL_ipipeSet3dLutConfig(CSL_IpipeHandle hndl, CSL_Ipipe3dLutConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_3DLUT_CONFIG, data);
}

CSL_Status CSL_ipipeSetRgb2YuvConfig(CSL_IpipeHandle hndl, CSL_IpipeRgb2YuvConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_RGB2YUV_CONFIG, data);
}

CSL_Status CSL_ipipeSetCntBrtConfig(CSL_IpipeHandle hndl, CSL_IpipeCntBrtConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_CNT_BRT_CONFIG, data);
}

CSL_Status CSL_ipipeSetGbceConfig(CSL_IpipeHandle hndl, CSL_IpipeGbceConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_GBCE_CONFIG, data);
}

CSL_Status CSL_ipipeSetEdgeEnhanceConfig(CSL_IpipeHandle hndl, CSL_IpipeEdgeEnhanceConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_EDGE_ENHANCE_CONFIG, data);
}

CSL_Status CSL_ipipeSetChromaArtifactReduceConfig(CSL_IpipeHandle hndl, CSL_IpipeChromaArtifactReduceConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_CAR_CONFIG, data);
}

CSL_Status CSL_ipipeSetChromaGainSupressConfig(CSL_IpipeHandle hndl, CSL_IpipeChromaGainSupressConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_CGS_CONFIG, data);
}

CSL_Status CSL_ipipeSetBoxcarConfig(CSL_IpipeHandle hndl, CSL_IpipeBoxcarConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BOXCAR_CONFIG, data);
}

CSL_Status CSL_ipipeSetHistogramConfig(CSL_IpipeHandle hndl, CSL_IpipeHistogramConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_HISTOGRAM_CONFIG, data);
}

CSL_Status CSL_ipipeSetBscConfig(CSL_IpipeHandle hndl, CSL_IpipeBscConfig * data)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BSC_CONFIG, data);
}

CSL_Status CSL_ipipeEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_ENABLE, (void *) enable);
}

CSL_Status CSL_ipipeOneShotEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_ONE_SHOT_ENABLE, (void *) enable);
}

CSL_Status CSL_ipipeSetDataPath(CSL_IpipeHandle hndl, Uint8 dataPath)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_SET_DATA_PATH, (void *) (Uint32)dataPath);
}

CSL_Status CSL_ipipeBoxcarEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BOXCAR_ENABLE, (void *) enable);
}

CSL_Status CSL_ipipeBscEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BSC_ENABLE, (void *) enable);
}

CSL_Status CSL_ipipeBscReadResults(CSL_IpipeHandle hndl, CSL_IpipeBscResults *data)
{
  int  status;

  status = CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BSC_READ_RESULTS, data);

  return status;
}

CSL_Status CSL_ipipeHistogramEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_HISTOGRAM_ENABLE, (void *) enable);
}

CSL_Status CSL_ipipeHistogramRegionEnable(CSL_IpipeHandle hndl, Bool32 * enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_HISTOGRAM_REGION_ENABLE, (void *) enable);
}

CSL_Status CSL_ipipeHistogramColorEnable(CSL_IpipeHandle hndl, Bool32 * enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_HISTOGRAM_COLOR_ENABLE, (void *) enable);
}

CSL_Status CSL_ipipeHistogramTableSelect(CSL_IpipeHandle hndl, Uint32 tableId)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_HISTOGRAM_TABLE_SELECT, (void*) tableId );
}

CSL_Status CSL_ipipeHistogramReadResults(CSL_IpipeHandle hndl, Uint32 * table, Uint32 offset, Uint32 size)
{
  Uint8   status;
  CSL_IpipeHistResults hstResults; //ANR-Hist
  hstResults.table = table;
  hstResults.offset = offset;  
  hstResults.size= size;

  //printf("\n UsrHistogramReadResults: hstResults->table %p, offset%d tableSize%d", hstResults.table, hstResults.offset, hstResults.size);

  status = CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_HISTOGRAM_READ_RESULTS, (void*)&hstResults);
  return status;
  
}
#if 1 //Gang: Histogram read idle Y
CSL_Status CSL_ipipeHistogramReadIdleY(CSL_IpipeHandle hndl, Uint32 * table)
{
  Uint8   status;

  status = CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_HISTOGRAM_READ_IDLE_Y, table);

  return status;
}
#endif

CSL_Status CSL_ipipeBscBufInit(CSL_IpipeHandle hndl, CSL_BufInit * bufInit)
{
  if(bufInit==NULL)
    return CSL_EFAIL;

  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BSC_BUF_INIT, bufInit);
}

CSL_Status CSL_ipipeBscBufSwitchEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BSC_BUF_SWITCH_ENABLE, (void *) enable);
}

CSL_Status CSL_ipipeBscBufGetFull(CSL_IpipeHandle hndl, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout)
{
  CSL_IpipeBufGetFullPrm bufGetFullPrm;

  bufGetFullPrm.buf = buf;
  bufGetFullPrm.minBuf = minBuf;
  bufGetFullPrm.timeout = timeout;

  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BSC_BUF_GET_FULL, &bufGetFullPrm);
}

CSL_Status CSL_ipipeBscBufPutEmpty(CSL_IpipeHandle hndl, CSL_BufInfo * buf)
{
  if(buf==NULL)
    return CSL_EFAIL;

  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BSC_BUF_PUT_EMPTY, buf);
}

CSL_Status CSL_ipipeIntEnable(CSL_IpipeHandle hndl, Uint8 ipipeIntType, Bool32 enable)
{
  CSL_IpipeIntEnablePrm intEnablePrm;

  intEnablePrm.ipipeIntType = ipipeIntType;
  intEnablePrm.enable = enable;

  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_INT_ENABLE, &intEnablePrm);
}

CSL_Status CSL_ipipeIntClear(CSL_IpipeHandle hndl, Uint8 ipipeIntType)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_INT_CLEAR, (void *) (Uint32)ipipeIntType);
}

CSL_Status CSL_ipipeIntWait(CSL_IpipeHandle hndl, Uint8 ipipeIntType)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_INT_WAIT, (void *) (Uint32)ipipeIntType);
}

CSL_Status CSL_ipipeLock(CSL_IpipeHandle hndl, Uint32 timeout)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_LOCK, (void *) timeout);
}

CSL_Status CSL_ipipeUnlock(CSL_IpipeHandle hndl)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_SYS_CMD_UNLOCK, NULL);
}

CSL_Status CSL_ipipeBoxcarBufInit(CSL_IpipeHandle hndl, CSL_BufInit * bufInit)
{
  if(bufInit==NULL)
    return CSL_EFAIL;

  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BOXCAR_BUF_INIT, bufInit);
}

CSL_Status CSL_ipipeBoxcarBufSwitchEnable(CSL_IpipeHandle hndl, Bool32 enable)
{
  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BOXCAR_BUF_SWITCH_ENABLE, (void *) enable);
}

CSL_Status CSL_ipipeBoxcarBufGetFull(CSL_IpipeHandle hndl, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout)
{
  CSL_IpipeBufGetFullPrm bufGetFullPrm;

  bufGetFullPrm.buf = buf;
  bufGetFullPrm.minBuf = minBuf;
  bufGetFullPrm.timeout = timeout;

  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BOXCAR_BUF_GET_FULL, &bufGetFullPrm);
}

CSL_Status CSL_ipipeBoxcarBufPutEmpty(CSL_IpipeHandle hndl, CSL_BufInfo * buf)
{
  if(buf==NULL)
    return CSL_EFAIL;

  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BOXCAR_BUF_PUT_EMPTY, buf);
}

CSL_Status CSL_ipipeBoxcarSetOutAddr(CSL_IpipeHandle hndl, Uint8 *outAddr)
{
  if(outAddr==NULL)
    return CSL_EFAIL;

  return CSL_usrDrvIoctl(hndl->fd, CSL_IPIPE_CMD_BOXCAR_SET_OUT_ADDR, outAddr);
}

