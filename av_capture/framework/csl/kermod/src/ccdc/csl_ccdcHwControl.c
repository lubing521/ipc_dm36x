#include <csl_ccdcIoctl.h>

CSL_Status CSL_ccdcHwControl(CSL_CcdcHandle hndl, Uint32 cmd, void *prm)
{
  CSL_Status status = CSL_SOK;

  static CSL_CcdcHwSetup data;
  static CSL_CcdcInDataConfig inDataConfig;
  static CSL_CcdcSyncConfig syncConfig;
  static CSL_CcdcMiscConfig miscConfig;
  static CSL_CcdcColPatConfig colPatConfig;
  static CSL_CcdcLinerizationConfig linerizationConfig;
  static CSL_CcdcFmtCscInFrameConfig fmtInFrameConfig;
  static CSL_CcdcFmtConfig fmtConfig;
  static CSL_CcdcCscConfig cscConfig;
  static CSL_CcdcClampConfig clampConfig;
  static CSL_CcdcDfcLscOffsetConfig dfcLscOffsetConfig;
  static CSL_CcdcDfcConfig dfcConfig;
  static CSL_CcdcLscConfig lscConfig;
  static CSL_CcdcGainOffsetConfig gainOffsetConfig;
  static CSL_CcdcSdramOutSizeConfig sdramOutSizeConfig;
  static CSL_CcdcSdramOutConfig sdramOutConfig;
  static CSL_CcdcDpcmConfig dpcmConfig;
  static CSL_CcdcRec656Config rec656Config;
  static CSL_CcdcFlashConfig flashConfig;
  static CSL_CcdcVdIntConfig vdIntConfig;

  static CSL_CcdcVdfcTable vdfcTable;
  static CSL_BufInit bufInit;
  static CSL_BufInfo buf;

  static CSL_CcdcGetLscStatusPrm lscStatusPrm;
  static CSL_CcdcBufGetFullPrm bufGetFullPrm;
  static CSL_CcdcIntEnablePrm intEnablePrm;
  static CSL_CcdcIntWaitPrm intWaitPrm;

  Uint32 *linTable = NULL;
  Uint32  linTableSize;

  switch (cmd) {

  case CSL_CCDC_CMD_HW_SETUP:
  
    if (status == CSL_SOK)
      status = CSL_copyFromUser(&data, prm, sizeof(data));

    if (status == CSL_SOK) {
      if(data.inDataConfig) {
        status = CSL_copyFromUser(&inDataConfig, data.inDataConfig, sizeof(inDataConfig));
        
        data.inDataConfig = &inDataConfig;        
      }
    }

    if (status == CSL_SOK) {
      if(data.syncConfig) {
        status = CSL_copyFromUser(&syncConfig, data.syncConfig, sizeof(syncConfig));
        
        data.syncConfig = &syncConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.miscConfig) {
        status = CSL_copyFromUser(&miscConfig, data.miscConfig, sizeof(miscConfig));

        data.miscConfig = &miscConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.colPatConfig) {
        status = CSL_copyFromUser(&colPatConfig, data.colPatConfig, sizeof(colPatConfig));

        data.colPatConfig = &colPatConfig;        
      }
    }
    
    if (status == CSL_SOK) {
      if(data.linerizationConfig) {
      
        linTableSize = CSL_ISIF_LIN_TBL_MAX_ENTRIES*4;
    
        linTable = CSL_sysMemAlloc(linTableSize);
        if (linTable == NULL)
          status = CSL_EFAIL;
      
        if(status == CSL_SOK) {
          status = CSL_copyFromUser(&linerizationConfig, data.linerizationConfig, sizeof(linerizationConfig));

          data.linerizationConfig = &linerizationConfig;
  
          if(linerizationConfig.numLutEntries>CSL_ISIF_LIN_TBL_MAX_ENTRIES)
            linerizationConfig.numLutEntries = CSL_ISIF_LIN_TBL_MAX_ENTRIES;
          
          if(linerizationConfig.lutTable) {
            status = CSL_copyFromUser(linTable, linerizationConfig.lutTable, linerizationConfig.numLutEntries*4);
            
            linerizationConfig.lutTable = linTable;
          }
        }
      }      
    }
      
    if (status == CSL_SOK) {
      if(data.fmtInFrameConfig) {
        status = CSL_copyFromUser(&fmtInFrameConfig, data.fmtInFrameConfig, sizeof(fmtInFrameConfig));

        data.fmtInFrameConfig = &fmtInFrameConfig;        
      }
    }

    if (status == CSL_SOK) {
      if(data.fmtConfig) {
        status = CSL_copyFromUser(&fmtConfig, data.fmtConfig, sizeof(fmtConfig));

        data.fmtConfig = &fmtConfig;        
      }
    }

    if (status == CSL_SOK) {
      if(data.cscConfig) {
        status = CSL_copyFromUser(&cscConfig, data.cscConfig, sizeof(cscConfig));

        data.cscConfig = &cscConfig;        
      }
    }

    if (status == CSL_SOK) {
      if(data.clampConfig) {
        status = CSL_copyFromUser(&clampConfig, data.clampConfig, sizeof(clampConfig));

        data.clampConfig = &clampConfig;        
      }
    }

    if (status == CSL_SOK) {
      if(data.dfcLscOffsetConfig) {
        status = CSL_copyFromUser(&dfcLscOffsetConfig, data.dfcLscOffsetConfig, sizeof(dfcLscOffsetConfig));

        data.dfcLscOffsetConfig = &dfcLscOffsetConfig;        
      }
    }

    if (status == CSL_SOK) {
      if(data.dfcConfig) {
        status = CSL_copyFromUser(&dfcConfig, data.dfcConfig, sizeof(dfcConfig));

        data.dfcConfig = &dfcConfig;   
        
        if (status == CSL_SOK) {
          if(dfcConfig.vdfcTable) {
            status = CSL_copyFromUser(&vdfcTable, dfcConfig.vdfcTable, sizeof(vdfcTable));
        
            dfcConfig.vdfcTable = &vdfcTable;
          }
        }
      }
    }

    if (status == CSL_SOK) {
      if(data.lscConfig) {
        status = CSL_copyFromUser(&lscConfig, data.lscConfig, sizeof(lscConfig));


        data.lscConfig = &lscConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.gainOffsetConfig) {
        status = CSL_copyFromUser(&gainOffsetConfig, data.gainOffsetConfig, sizeof(gainOffsetConfig));
        
        data.gainOffsetConfig = &gainOffsetConfig;        
      }
    }

    if (status == CSL_SOK) {
      if(data.sdramOutConfig) {
        status = CSL_copyFromUser(&sdramOutConfig, data.sdramOutConfig, sizeof(sdramOutConfig));
  
        data.sdramOutConfig = &sdramOutConfig;        
      }
    }

    if (status == CSL_SOK) {
      if(data.dpcmConfig) {
        status = CSL_copyFromUser(&dpcmConfig, data.dpcmConfig, sizeof(dpcmConfig));
        
        data.dpcmConfig = &dpcmConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.rec656Config) {
        status = CSL_copyFromUser(&rec656Config, data.rec656Config, sizeof(rec656Config));
        
        data.rec656Config = &rec656Config;
      }
    }

    if (status == CSL_SOK) {
      if(data.flashConfig) {
        status = CSL_copyFromUser(&flashConfig, data.flashConfig, sizeof(flashConfig));

        data.flashConfig = &flashConfig;        
      }
    }

    if (status == CSL_SOK) {
      if(data.vdIntConfig) {
        status = CSL_copyFromUser(&vdIntConfig, data.vdIntConfig, sizeof(vdIntConfig));

        data.vdIntConfig = &vdIntConfig;        
      }
    }

    if (status == CSL_SOK)
      status = CSL_ccdcHwSetup(hndl, &data);

    if (linTable != NULL)
      CSL_sysMemFree(linTable);

    break;

  case CSL_CCDC_CMD_HW_RESET:

    status = CSL_ccdcHwReset(hndl);
    break;

  case CSL_CCDC_CMD_ENABLE:

    status = CSL_ccdcEnable(hndl, (Bool32) prm);
    break;

  case CSL_CCDC_CMD_SDRAM_OUT_ENABLE:

    status = CSL_ccdcSdramOutEnable(hndl, (Bool32) prm);
    break;

  case CSL_CCDC_CMD_SET_DATA_IN_CONFIG:

    status = CSL_copyFromUser(&inDataConfig, prm, sizeof(inDataConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetInDataConfig(hndl, &inDataConfig);

    break;

  case CSL_CCDC_CMD_SET_SYNC_CONFIG:

    status = CSL_copyFromUser(&syncConfig, prm, sizeof(syncConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetSyncConfig(hndl, &syncConfig);

    break;

  case CSL_CCDC_CMD_SET_MISC_CONFIG:

    status = CSL_copyFromUser(&miscConfig, prm, sizeof(miscConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetMiscConfig(hndl, &miscConfig);

    break;

  case CSL_CCDC_CMD_SET_COL_PAT_CONFIG:

    status = CSL_copyFromUser(&colPatConfig, prm, sizeof(colPatConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetColPatConfig(hndl, &colPatConfig);

    break;

  case CSL_CCDC_CMD_SET_LINEARIZATION_CONFIG:

    linTableSize = CSL_ISIF_LIN_TBL_MAX_ENTRIES*4;
    
    linTable = CSL_sysMemAlloc(linTableSize);
    if (linTable == NULL)
      status = CSL_EFAIL;

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&linerizationConfig, prm, sizeof(linerizationConfig));

    if(linerizationConfig.numLutEntries>CSL_ISIF_LIN_TBL_MAX_ENTRIES)
      linerizationConfig.numLutEntries = CSL_ISIF_LIN_TBL_MAX_ENTRIES;
          
    if(linerizationConfig.lutTable) {
      status = CSL_copyFromUser(linTable, linerizationConfig.lutTable, linerizationConfig.numLutEntries*4);
      
      linerizationConfig.lutTable = linTable;
    }

    if (status == CSL_SOK)
      status = CSL_ccdcSetLinerizationConfig(hndl, &linerizationConfig);

    if (linTable != NULL)
      CSL_sysMemFree(linTable);

    break;

  case CSL_CCDC_CMD_SET_FMT_CSC_IN_FRAME_CONFIG:

    status = CSL_copyFromUser(&fmtInFrameConfig, prm, sizeof(fmtInFrameConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetFmtCscInFrameConfig(hndl, &fmtInFrameConfig);

    break;

  case CSL_CCDC_CMD_SET_FMT_CONFIG:

    status = CSL_copyFromUser(&fmtConfig, prm, sizeof(fmtConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetFmtConfig(hndl, &fmtConfig);

    break;

  case CSL_CCDC_CMD_SET_CSC_CONFIG:

    status = CSL_copyFromUser(&cscConfig, prm, sizeof(cscConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetCscConfig(hndl, &cscConfig);

    break;

  case CSL_CCDC_CMD_SET_CLAMP_CONFIG:

    status = CSL_copyFromUser(&clampConfig, prm, sizeof(clampConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetClampConfig(hndl, &clampConfig);

    break;

  case CSL_CCDC_CMD_SET_DFC_LSC_OFFSET_CONFIG:

    status = CSL_copyFromUser(&dfcLscOffsetConfig, prm, sizeof(dfcLscOffsetConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetDfcLscOffsetConfig(hndl, &dfcLscOffsetConfig);

    break;

  case CSL_CCDC_CMD_SET_DFC_CONFIG:

    status = CSL_copyFromUser(&dfcConfig, prm, sizeof(dfcConfig));

    if (status == CSL_SOK) {
      if(dfcConfig.vdfcTable) {
        status = CSL_copyFromUser(&vdfcTable, dfcConfig.vdfcTable, sizeof(vdfcTable));
        
        dfcConfig.vdfcTable = &vdfcTable;
      }
    }

    if (status == CSL_SOK)
      status = CSL_ccdcSetDfcConfig(hndl, &dfcConfig);

    break;

  case CSL_CCDC_CMD_SET_LSC_CONFIG:

    status = CSL_copyFromUser(&lscConfig, prm, sizeof(lscConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetLscConfig(hndl, &lscConfig);

    break;

  case CSL_CCDC_CMD_GET_LSC_STATUS:

    status = CSL_ccdcGetLscStatus(hndl, &lscStatusPrm.intSof, &lscStatusPrm.prefetchDone, &lscStatusPrm.prefetchError, &lscStatusPrm.lscDone);

    if (status == CSL_SOK)
      status = CSL_copyToUser(prm, &lscStatusPrm, sizeof(lscStatusPrm));

    break;

  case CSL_CCDC_CMD_SET_GAIN_OFFSET_CONFIG:

    status = CSL_copyFromUser(&gainOffsetConfig, prm, sizeof(gainOffsetConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetGainOffsetConfig(hndl, &gainOffsetConfig);

    break;

  case CSL_CCDC_CMD_SET_SDRAM_OUT_SIZE_CONFIG:

    status = CSL_copyFromUser(&sdramOutSizeConfig, prm, sizeof(sdramOutSizeConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetSdramOutSizeConfig(hndl, &sdramOutSizeConfig);

    break;

  case CSL_CCDC_CMD_SET_SDRAM_OUT_CONFIG:

    status = CSL_copyFromUser(&sdramOutConfig, prm, sizeof(sdramOutConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetSdramOutConfig(hndl, &sdramOutConfig);

    break;

  case CSL_CCDC_CMD_GET_SDRAM_OUT_SIZE_CONFIG:

    status = CSL_ccdcGetSdramOutSizeConfig(hndl, &sdramOutSizeConfig);

    if (status == CSL_SOK)
      status = CSL_copyToUser(prm, &sdramOutSizeConfig, sizeof(sdramOutSizeConfig));

    break;

  case CSL_CCDC_CMD_SET_DPCM_CONFIG:

    status = CSL_copyFromUser(&dpcmConfig, prm, sizeof(dpcmConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetDpcmConfig(hndl, &dpcmConfig);

    break;

  case CSL_CCDC_CMD_SET_REC656_CONFIG:

    status = CSL_copyFromUser(&rec656Config, prm, sizeof(rec656Config));

    if (status == CSL_SOK)
      status = CSL_ccdcSetRec656Config(hndl, &rec656Config);

    break;

  case CSL_CCDC_CMD_SET_FLASH_CONFIG:

    status = CSL_copyFromUser(&flashConfig, prm, sizeof(flashConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetFlashConfig(hndl, &flashConfig);

    break;

  case CSL_CCDC_CMD_SET_VD_INT_CONFIG:

    status = CSL_copyFromUser(&vdIntConfig, prm, sizeof(vdIntConfig));

    if (status == CSL_SOK)
      status = CSL_ccdcSetVdIntConfig(hndl, &vdIntConfig);

    break;

  case CSL_CCDC_CMD_SET_SDRAM_OUT_ADDR:

    status = CSL_ccdcSetSdramOutAddr(hndl, (Uint8 *) prm);
    break;

  case CSL_CCDC_CMD_WRITE_DFC_TABLE:

    status = CSL_copyFromUser(&vdfcTable, prm, sizeof(vdfcTable));

    if (status == CSL_SOK)
      status = CSL_ccdcWriteDfcTable(hndl, &vdfcTable);

    break;

  case CSL_CCDC_CMD_READ_DFC_TABLE:

    status = CSL_ccdcReadDfcTable(hndl, &vdfcTable);

    if (status == CSL_SOK)
      status = CSL_copyToUser(prm, &vdfcTable, sizeof(vdfcTable));

    break;

  case CSL_CCDC_CMD_BUF_INIT:

    status = CSL_copyFromUser(&bufInit, prm, sizeof(bufInit));

    if (status == CSL_SOK)
      status = CSL_ccdcBufInit(hndl, &bufInit);

    break;

  case CSL_CCDC_CMD_BUF_SWITCH_ENABLE:

    status = CSL_ccdcBufSwitchEnable(hndl, (Bool32) prm);
    break;

  case CSL_CCDC_CMD_BUF_GET_FULL:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&bufGetFullPrm, prm, sizeof(bufGetFullPrm));

    if (status == CSL_SOK)
      status = CSL_ccdcBufGetFull(hndl, &buf, bufGetFullPrm.minBuf, bufGetFullPrm.timeout);

    status = CSL_copyToUser(bufGetFullPrm.buf, &buf, sizeof(buf));

    break;

  case CSL_CCDC_CMD_BUF_PUT_EMPTY:

    status = CSL_copyFromUser(&buf, prm, sizeof(buf));

    if (status == CSL_SOK)
      status = CSL_ccdcBufPutEmpty(hndl, &buf);

    break;

  case CSL_CCDC_CMD_INT_ENABLE:

    status = CSL_copyFromUser(&intEnablePrm, prm, sizeof(intEnablePrm));

    if (status == CSL_SOK)
      status = CSL_ccdcIntEnable(hndl, intEnablePrm.intType, intEnablePrm.enable);

    break;

  case CSL_CCDC_CMD_INT_CLEAR:

    status = CSL_ccdcIntClear(hndl, (Uint32) prm);
    break;

  case CSL_CCDC_CMD_INT_WAIT:

    status = CSL_copyFromUser(&intWaitPrm, prm, sizeof(intWaitPrm));

    if (status == CSL_SOK)
      status = CSL_ccdcIntWait(hndl, intWaitPrm.intType, intWaitPrm.numIntWait);

    break;

  case CSL_CCDC_CMD_LSC_BUF_INIT:

    status = CSL_copyFromUser(&bufInit, prm, sizeof(bufInit));

    if (status == CSL_SOK)
      status = CSL_ccdcLscBufInit(hndl, &bufInit);

    break;

  case CSL_CCDC_CMD_LSC_BUF_SWITCH_ENABLE:

    status = CSL_ccdcLscBufSwitchEnable(hndl, (Bool32) prm);
    break;

  case CSL_CCDC_CMD_LSC_BUF_GET_EMPTY:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&bufGetFullPrm, prm, sizeof(bufGetFullPrm));

    if (status == CSL_SOK)
      status = CSL_ccdcLscBufGetEmpty(hndl, &buf, bufGetFullPrm.minBuf, bufGetFullPrm.timeout);

    status = CSL_copyToUser(bufGetFullPrm.buf, &buf, sizeof(buf));

    break;

  case CSL_CCDC_CMD_LSC_BUF_PUT_FULL:

    status = CSL_copyFromUser(&buf, prm, sizeof(buf));

    if (status == CSL_SOK)
      status = CSL_ccdcLscBufPutFull(hndl, &buf);

    break;

  case CSL_CCDC_CMD_LSC_SET_GAIN_TABLE_ADDR:    
    status = CSL_ccdcLscSetGainTableAddr(hndl, (Uint8*)prm);
    break;

  case CSL_CCDC_CMD_LSC_SET_OFFSET_TABLE_ADDR:    
    status = CSL_ccdcLscSetOffsetTableAddr(hndl, (Uint8*)prm);
    break;

  case CSL_CCDC_CMD_LSC_ENABLE:    
    status = CSL_ccdcLscEnable(hndl, (Bool32)prm);
    break;

  case CSL_CCDC_CMD_LSC_BUF_SET_GAIN_TABLE_SIZE:
    status = CSL_ccdcLscBufSetGainTableSize(hndl, (Uint32)prm);
    break;

  default:
    status = CSL_EFAIL;
    break;
  }

  return status;
}
