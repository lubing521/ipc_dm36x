#include <csl_ipipeIoctl.h>

CSL_Status CSL_ipipeHwControl(CSL_IpipeHandle hndl, Uint32 cmd, void *prm)
{
  CSL_Status status = CSL_SOK;

  static CSL_IpipeHwSetup data;
  static CSL_IpipeColPatConfig colpatConfig;
  static CSL_IpipeClkConfig clkConfig;
  static CSL_IpipeInFrameConfig inFrameConfig;
  static CSL_IpipeLscConfig lscConfig;
  static CSL_IpipeDpcConfig dpcConfig;
  static CSL_IpipeNfConfig nf1Config;
  static CSL_IpipeNfConfig nf2Config;
  static CSL_IpipeGicConfig gicConfig;
  static CSL_IpipeWbConfig wbConfig;
  static CSL_IpipeCfaConfig cfaConfig;
  static CSL_IpipeRgb2RgbConfig rgb2Rgb1Config;
  static CSL_IpipeGammaConfig gammaConfig;
  static CSL_IpipeRgb2RgbConfig rgb2Rgb2Config;
  static CSL_Ipipe3dLutConfig lut3DConfig;
  static CSL_IpipeRgb2YuvConfig rgb2YuvConfig;
  static CSL_IpipeCntBrtConfig cntBrtConfig;
  static CSL_IpipeGbceConfig gbceConfig;
  static CSL_IpipeEdgeEnhanceConfig edgeEnhanceConfig;
  static CSL_IpipeChromaArtifactReduceConfig chromaArtifactReduceConfig;
  static CSL_IpipeChromaGainSupressConfig chromaGainSupressConfig;
  static CSL_IpipeBoxcarConfig boxcarConfig;
  static CSL_IpipeHistogramConfig histogramConfig;
  static CSL_IpipeBscConfig bscConfig;
  static CSL_IpipeBscResults bscResults;

  static CSL_BufInit bufInit;
  static CSL_BufInfo buf;

  static CSL_IpipeBufGetFullPrm bufGetFullPrm;
  static CSL_IpipeIntEnablePrm intEnablePrm;

  static   CSL_IpipeHistResults *hstResults; //ANR-Hist
  Uint32 offset = 0;                
  Uint32 size = 0;
  
  Uint32 *tableAddr = NULL;
  Uint32 *tableR = NULL;
  Uint32 *tableG = NULL;
  Uint32 *tableB = NULL;

  Uint32 *lut3dTable = NULL;
  Uint32 *dpcLutTable = NULL;

  Uint32 *gbceGainTable = NULL;

  Uint32 *edgeEnhanceTable = NULL;
  Uint32 tableSize;

  switch (cmd) {

  case CSL_IPIPE_CMD_HW_SETUP:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&data, prm, sizeof(data));

    if (status == CSL_SOK) {
    
      if(data.colpatConfig) {
        status = CSL_copyFromUser(&colpatConfig, data.colpatConfig, sizeof(colpatConfig));
        
        data.colpatConfig = &colpatConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.clkConfig) {
        status = CSL_copyFromUser(&clkConfig, data.clkConfig, sizeof(clkConfig));
        
        data.clkConfig = &clkConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.inFrameConfig) {
        status = CSL_copyFromUser(&inFrameConfig, data.inFrameConfig, sizeof(inFrameConfig));
       
        data.inFrameConfig = &inFrameConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.lscConfig) {
        status = CSL_copyFromUser(&lscConfig, data.lscConfig, sizeof(lscConfig));
        
        data.lscConfig = &lscConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.dpcConfig) {

        status = CSL_copyFromUser(&dpcConfig, data.dpcConfig, sizeof(dpcConfig));
        
        data.dpcConfig = &dpcConfig;
          
        if(dpcConfig.lutAddr) {
          tableSize = CSL_IPIPE_DPC_LUT_MAX_ENTRIES*4;
          dpcLutTable = CSL_sysMemAlloc(tableSize);
          if(dpcLutTable==NULL)
            status = CSL_EFAIL;
          
          if(status==CSL_SOK) {
            status = CSL_copyFromUser(dpcLutTable, dpcConfig.lutAddr, tableSize);
            
            dpcConfig.lutAddr = dpcLutTable;
          }          
        }
      }
    }

    if (status == CSL_SOK) {
      if(data.nf1Config) {
        status = CSL_copyFromUser(&nf1Config, data.nf1Config, sizeof(nf1Config));
        
        data.nf1Config = &nf1Config;
      }
    }

    if (status == CSL_SOK) {
      if(data.nf2Config) {
        status = CSL_copyFromUser(&nf2Config, data.nf2Config, sizeof(nf2Config));
        
        data.nf2Config = &nf2Config;
      }
    }

    if (status == CSL_SOK) {
      if(data.gicConfig) {
        status = CSL_copyFromUser(&gicConfig, data.gicConfig, sizeof(gicConfig));
       
        data.gicConfig = &gicConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.wbConfig) {
        status = CSL_copyFromUser(&wbConfig, data.wbConfig, sizeof(wbConfig));
        
        data.wbConfig = &wbConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.cfaConfig) {
        status = CSL_copyFromUser(&cfaConfig, data.cfaConfig, sizeof(cfaConfig));
       
        data.cfaConfig = &cfaConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.rgb2Rgb1Config) {
        status = CSL_copyFromUser(&rgb2Rgb1Config, data.rgb2Rgb1Config, sizeof(rgb2Rgb1Config));
       
        data.rgb2Rgb1Config = &rgb2Rgb1Config; 
      }
    }

    if (status == CSL_SOK) {
      if(data.gammaConfig) {
        status = CSL_copyFromUser(&gammaConfig, data.gammaConfig, sizeof(gammaConfig));
        
        data.gammaConfig = &gammaConfig;

        tableSize = CSL_IPIPE_GAMMA_TABLE_MAX_ENTRIES*4;
        
        if(gammaConfig.tableR) {

          tableR = CSL_sysMemAlloc(tableSize);
          if(tableR==NULL)
            status = CSL_EFAIL;
            
          if(status==CSL_SOK) {
            status = CSL_copyFromUser(tableR, gammaConfig.tableR, tableSize);
            
            gammaConfig.tableR = tableR;
          }  
        }
        
        if(gammaConfig.tableG) {

          tableG = CSL_sysMemAlloc(tableSize);
          if(tableG==NULL)
            status = CSL_EFAIL;
            
          if(status==CSL_SOK) {
            status = CSL_copyFromUser(tableG, gammaConfig.tableG, tableSize);
            
            gammaConfig.tableG = tableG;
          }  
        }

        if(gammaConfig.tableB) {

          tableB = CSL_sysMemAlloc(tableSize);
          if(tableB==NULL)
            status = CSL_EFAIL;
            
          if(status==CSL_SOK) {
            status = CSL_copyFromUser(tableB, gammaConfig.tableB, tableSize);
            
            gammaConfig.tableB = tableB;
          }  
        }
      }
    }

    if (status == CSL_SOK) {
      if(data.rgb2Rgb2Config) {
        status = CSL_copyFromUser(&rgb2Rgb2Config, data.rgb2Rgb2Config, sizeof(rgb2Rgb2Config));
        
        data.rgb2Rgb2Config = &rgb2Rgb2Config;
      }
    }

    if (status == CSL_SOK) {
      if(data.lut3DConfig) {
        status = CSL_copyFromUser(&lut3DConfig, data.lut3DConfig, sizeof(lut3DConfig));
       
        data.lut3DConfig = &lut3DConfig;
        
        if(lut3DConfig.table) {
          tableSize = CSL_IPIPE_3D_LUT_MAX_ENTRIES*4;
          
          lut3dTable = CSL_sysMemAlloc(tableSize);
          if(lut3dTable==NULL)
            status = CSL_EFAIL;
            
          if(status == CSL_SOK ) {
            status = CSL_copyFromUser(lut3dTable, lut3DConfig.table, tableSize);
            
            lut3DConfig.table = lut3dTable;
          }
        }
      }
    }

    if (status == CSL_SOK) {
      if(data.rgb2YuvConfig) {
        status = CSL_copyFromUser(&rgb2YuvConfig, data.rgb2YuvConfig, sizeof(rgb2YuvConfig));
        
        data.rgb2YuvConfig = &rgb2YuvConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.cntBrtConfig) {
        status = CSL_copyFromUser(&cntBrtConfig, data.cntBrtConfig, sizeof(cntBrtConfig));
        
        data.cntBrtConfig = &cntBrtConfig;
      }
    }

    if (status == CSL_SOK) {
    
      if(data.gbceConfig) {
        status = CSL_copyFromUser(&gbceConfig, data.gbceConfig, sizeof(gbceConfig));
        
        data.gbceConfig = &gbceConfig;
        
        if(gbceConfig.gainTable) {
          tableSize = CSL_IPIPE_GBCE_GAIN_TABLE_MAX_ENTRIES*4;
          
          gbceGainTable = CSL_sysMemAlloc(tableSize);
          if(gbceGainTable==NULL)
            status = CSL_EFAIL;
            
          if(status == CSL_SOK ) {
            status = CSL_copyFromUser(gbceGainTable, gbceConfig.gainTable, tableSize);
            
            gbceConfig.gainTable = gbceGainTable;
          }
        }
      }
    }

    if (status == CSL_SOK) {
      if(data.edgeEnhanceConfig) {
        status = CSL_copyFromUser(&edgeEnhanceConfig, data.edgeEnhanceConfig, sizeof(edgeEnhanceConfig));
        
        data.edgeEnhanceConfig = &edgeEnhanceConfig;
        
        if(edgeEnhanceConfig.table) {
          tableSize = CSL_IPIPE_EDGE_ENHANCE_TABLE_MAX_ENTRIES*4;
          
          edgeEnhanceTable = CSL_sysMemAlloc(tableSize);
          if(edgeEnhanceTable==NULL)
            status = CSL_EFAIL;
            
          if(status==CSL_SOK) {
            status = CSL_copyFromUser(edgeEnhanceTable, edgeEnhanceConfig.table, tableSize);
            
            edgeEnhanceConfig.table = edgeEnhanceTable;
          }
        }
      }
    }

    if (status == CSL_SOK) {
      if(data.chromaArtifactReduceConfig) {
        status = CSL_copyFromUser(&chromaArtifactReduceConfig, data.chromaArtifactReduceConfig, sizeof(chromaArtifactReduceConfig));
        
        data.chromaArtifactReduceConfig = &chromaArtifactReduceConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.chromaGainSupressConfig) {
        status = CSL_copyFromUser(&chromaGainSupressConfig, data.chromaGainSupressConfig, sizeof(chromaGainSupressConfig));
        
        data.chromaGainSupressConfig = &chromaGainSupressConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.boxcarConfig) {
        status = CSL_copyFromUser(&boxcarConfig, data.boxcarConfig, sizeof(boxcarConfig));
        
        data.boxcarConfig = &boxcarConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.histogramConfig) {
        status = CSL_copyFromUser(&histogramConfig, data.histogramConfig, sizeof(histogramConfig));
        
        data.histogramConfig = &histogramConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.bscConfig) {
        status = CSL_copyFromUser(&bscConfig, data.bscConfig, sizeof(bscConfig));
        
        data.bscConfig = &bscConfig;
      }
    }

    if (status == CSL_SOK)
      status = CSL_ipipeHwSetup(hndl, &data);

    if(tableR != NULL)
      CSL_sysMemFree(tableR);
    if(tableG != NULL)
      CSL_sysMemFree(tableG);
    if(tableB != NULL)
      CSL_sysMemFree(tableB);

    if(lut3dTable!=NULL)
      CSL_sysMemFree(lut3dTable);

    if(dpcLutTable!=NULL)
      CSL_sysMemFree(dpcLutTable);
      
    if(gbceGainTable!=NULL)
      CSL_sysMemFree(gbceGainTable);
      
    if(edgeEnhanceTable!=NULL)
      CSL_sysMemFree(edgeEnhanceTable);
    
    break;

  case CSL_IPIPE_CMD_HW_RESET:

    status = CSL_ipipeHwReset(hndl);
    break;

  case CSL_IPIPE_CMD_COL_PAT_CONFIG:

    status = CSL_copyFromUser(&colpatConfig, prm, sizeof(colpatConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetColPatConfig(hndl, &colpatConfig);

    break;

  case CSL_IPIPE_CMD_CLK_COFNIG:

    status = CSL_copyFromUser(&clkConfig, prm, sizeof(clkConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetClkConfig(hndl, &clkConfig);

    break;

  case CSL_IPIPE_CMD_IN_FRAME_CONFIG:

    status = CSL_copyFromUser(&inFrameConfig, prm, sizeof(inFrameConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetInFrameConfig(hndl, &inFrameConfig);

    break;

  case CSL_IPIPE_CMD_LSC_CONFIG:

    status = CSL_copyFromUser(&lscConfig, prm, sizeof(lscConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetLscConfig(hndl, &lscConfig);

    break;

  case CSL_IPIPE_CMD_DPC_CONFIG:
  
    status = CSL_copyFromUser(&dpcConfig, prm, sizeof(dpcConfig));
    
    if(status==CSL_SOK) {
      if(dpcConfig.lutAddr) {
        tableSize = CSL_IPIPE_DPC_LUT_MAX_ENTRIES*4;      
        tableAddr = CSL_sysMemAlloc(tableSize);
        if(tableAddr==NULL)
          status = CSL_EFAIL;
          
        if(status==CSL_SOK) {
          status = CSL_copyFromUser(tableAddr, dpcConfig.lutAddr, tableSize); 
          
          dpcConfig.lutAddr = tableAddr;
        }
      }
    }

    if (status == CSL_SOK)
      status = CSL_ipipeSetDpcConfig(hndl, &dpcConfig);
      
    if(tableAddr!=NULL)
      CSL_sysMemFree(tableAddr);

    break;

  case CSL_IPIPE_CMD_NF1_CONFIG:

    status = CSL_copyFromUser(&nf1Config, prm, sizeof(nf1Config));

    if (status == CSL_SOK)
      status = CSL_ipipeSetNf1Config(hndl, &nf1Config);

    break;

  case CSL_IPIPE_CMD_NF2_CONFIG:

    status = CSL_copyFromUser(&nf2Config, prm, sizeof(nf2Config));

    if (status == CSL_SOK)
      status = CSL_ipipeSetNf1Config(hndl, &nf2Config);

    break;

  case CSL_IPIPE_CMD_GIC_CONFIG:

    status = CSL_copyFromUser(&gicConfig, prm, sizeof(gicConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetGicConfig(hndl, &gicConfig);

    break;

  case CSL_IPIPE_CMD_WB_CONFIG:

    status = CSL_copyFromUser(&wbConfig, prm, sizeof(wbConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetWbConfig(hndl, &wbConfig);

    break;

  case CSL_IPIPE_CMD_CFA_CONFIG:

    status = CSL_copyFromUser(&cfaConfig, prm, sizeof(cfaConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetCfaConfig(hndl, &cfaConfig);

    break;

  case CSL_IPIPE_CMD_RGB2RGB1_CONFIG:

    status = CSL_copyFromUser(&rgb2Rgb1Config, prm, sizeof(rgb2Rgb1Config));

    if (status == CSL_SOK)
      status = CSL_ipipeSetRgb2Rgb1Config(hndl, &rgb2Rgb1Config);

    break;

  case CSL_IPIPE_CMD_GAMMA_CONFIG:


    status = CSL_copyFromUser(&gammaConfig, prm, sizeof(gammaConfig));

    if(status==CSL_SOK) {
      tableSize = CSL_IPIPE_GAMMA_TABLE_MAX_ENTRIES*4;
      
      if(gammaConfig.tableR) {

        tableR = CSL_sysMemAlloc(tableSize);
        if(tableR==NULL)
          status = CSL_EFAIL;
          
        if(status==CSL_SOK) {
          status = CSL_copyFromUser(tableR, gammaConfig.tableR, tableSize);
          
          gammaConfig.tableR = tableR;
        }  
      }
      
      if(gammaConfig.tableG) {

        tableG = CSL_sysMemAlloc(tableSize);
        if(tableG==NULL)
          status = CSL_EFAIL;
          
        if(status==CSL_SOK) {
          status = CSL_copyFromUser(tableG, gammaConfig.tableG, tableSize);
          
          gammaConfig.tableG = tableG;
        }  
      }

      if(gammaConfig.tableB) {

        tableB = CSL_sysMemAlloc(tableSize);
        if(tableB==NULL)
          status = CSL_EFAIL;
          
        if(status==CSL_SOK) {
          status = CSL_copyFromUser(tableB, gammaConfig.tableB, tableSize);
          
          gammaConfig.tableB = tableB;
        }  
      }
    }

    if (status == CSL_SOK)
      status = CSL_ipipeSetGammaConfig(hndl, &gammaConfig);

    if (tableR != NULL)
      CSL_sysMemFree(tableR);

    if (tableG != NULL)
      CSL_sysMemFree(tableG);

    if (tableB != NULL)
      CSL_sysMemFree(tableB);

    break;

  case CSL_IPIPE_CMD_RGB2RGB2_CONFIG:

    status = CSL_copyFromUser(&rgb2Rgb2Config, prm, sizeof(rgb2Rgb2Config));

    if (status == CSL_SOK)
      status = CSL_ipipeSetRgb2Rgb2Config(hndl, &rgb2Rgb2Config);

    break;

  case CSL_IPIPE_CMD_3DLUT_CONFIG:

    status = CSL_copyFromUser(&lut3DConfig, prm, sizeof(lut3DConfig));

    if(status==CSL_SOK) {
      if(lut3DConfig.table) {
        tableSize = CSL_IPIPE_3D_LUT_MAX_ENTRIES*4;
        
        lut3dTable = CSL_sysMemAlloc(tableSize);
        if(lut3dTable==NULL)
          status = CSL_EFAIL;
          
        if(status == CSL_SOK ) {
          status = CSL_copyFromUser(lut3dTable, lut3DConfig.table, tableSize);
          
          lut3DConfig.table = lut3dTable;
        }
      }
    }

    if (status == CSL_SOK)
      status = CSL_ipipeSet3dLutConfig(hndl, &lut3DConfig);

    if (lut3dTable != NULL)
      CSL_sysMemFree(lut3dTable);

    break;

  case CSL_IPIPE_CMD_RGB2YUV_CONFIG:

    status = CSL_copyFromUser(&rgb2YuvConfig, prm, sizeof(rgb2YuvConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetRgb2YuvConfig(hndl, &rgb2YuvConfig);

    break;

  case CSL_IPIPE_CMD_CNT_BRT_CONFIG:

    status = CSL_copyFromUser(&cntBrtConfig, prm, sizeof(cntBrtConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetCntBrtConfig(hndl, &cntBrtConfig);

    break;

  case CSL_IPIPE_CMD_GBCE_CONFIG:


    status = CSL_copyFromUser(&gbceConfig, prm, sizeof(gbceConfig));

    if(status==CSL_SOK) {
      if(gbceConfig.gainTable) {
        tableSize = CSL_IPIPE_GBCE_GAIN_TABLE_MAX_ENTRIES*4;      
        tableAddr = CSL_sysMemAlloc(tableSize);
        if(tableAddr==NULL)
          status = CSL_EFAIL;
          
        if(status==CSL_SOK) {
          status = CSL_copyFromUser(tableAddr, gbceConfig.gainTable, tableSize); 
          
          gbceConfig.gainTable = tableAddr;
        }
      }
    }

    if (status == CSL_SOK)
      status = CSL_ipipeSetGbceConfig(hndl, &gbceConfig);

    if (tableAddr != NULL)
      CSL_sysMemFree(tableAddr);

    break;

  case CSL_IPIPE_CMD_EDGE_ENHANCE_CONFIG:

    
    status = CSL_copyFromUser(&edgeEnhanceConfig, prm, sizeof(edgeEnhanceConfig));

    if(status==CSL_SOK) {
      if(edgeEnhanceConfig.table) {
        tableSize = CSL_IPIPE_EDGE_ENHANCE_TABLE_MAX_ENTRIES*4;      
        tableAddr = CSL_sysMemAlloc(tableSize);
        if(tableAddr==NULL)
          status = CSL_EFAIL;
          
        if(status==CSL_SOK) {
          status = CSL_copyFromUser(tableAddr, edgeEnhanceConfig.table, tableSize); 
          
          edgeEnhanceConfig.table = tableAddr;
        }
      }
    }

    if (status == CSL_SOK)
      status = CSL_ipipeSetEdgeEnhanceConfig(hndl, &edgeEnhanceConfig);

    if (tableAddr != NULL)
      CSL_sysMemFree(tableAddr);

    break;

  case CSL_IPIPE_CMD_CAR_CONFIG:

    status = CSL_copyFromUser(&chromaArtifactReduceConfig, prm, sizeof(chromaArtifactReduceConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetChromaArtifactReduceConfig(hndl, &chromaArtifactReduceConfig);

    break;

  case CSL_IPIPE_CMD_CGS_CONFIG:

    status = CSL_copyFromUser(&chromaGainSupressConfig, prm, sizeof(chromaGainSupressConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetChromaGainSupressConfig(hndl, &chromaGainSupressConfig);

    break;

  case CSL_IPIPE_CMD_BOXCAR_CONFIG:

    status = CSL_copyFromUser(&boxcarConfig, prm, sizeof(boxcarConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetBoxcarConfig(hndl, &boxcarConfig);

    break;

  case CSL_IPIPE_CMD_HISTOGRAM_CONFIG:

    status = CSL_copyFromUser(&histogramConfig, prm, sizeof(histogramConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetHistogramConfig(hndl, &histogramConfig);

    break;

  case CSL_IPIPE_CMD_BSC_CONFIG:

    status = CSL_copyFromUser(&bscConfig, prm, sizeof(bscConfig));

    if (status == CSL_SOK)
      status = CSL_ipipeSetBscConfig(hndl, &bscConfig);

    break;

  case CSL_IPIPE_CMD_ENABLE:

    status = CSL_ipipeEnable(hndl, (Bool32) prm);
    break;

  case CSL_IPIPE_CMD_ONE_SHOT_ENABLE:

    status = CSL_ipipeOneShotEnable(hndl, (Bool32) prm);
    break;

  case CSL_IPIPE_CMD_SET_DATA_PATH:

    status = CSL_ipipeSetDataPath(hndl, (Uint32) prm);
    break;

  case CSL_IPIPE_CMD_BOXCAR_ENABLE:

    status = CSL_ipipeBoxcarEnable(hndl, (Bool32) prm);
    break;

  case CSL_IPIPE_CMD_BSC_ENABLE:

    status = CSL_ipipeBscEnable(hndl, (Bool32) prm);
    break;

  case CSL_IPIPE_CMD_BSC_READ_RESULTS:
  {
    CSL_IpipeBscResults userBscResults;
    
    status = CSL_copyFromUser(&userBscResults, prm, sizeof(userBscResults));
    
    if(status==CSL_SOK) {
      tableSize = CSL_IPIPE_BSC_RESULTS_MAX_ENTRIES*4;
      tableAddr = CSL_sysMemAlloc(tableSize*2);
    
      bscResults.rowSumTable = tableAddr;
      bscResults.colSumTable = tableAddr + CSL_IPIPE_BSC_RESULTS_MAX_ENTRIES;    
    
      status = CSL_ipipeBscReadResults(hndl, &bscResults);

      if (status == CSL_SOK) 
        status = CSL_copyToUser(userBscResults.rowSumTable, bscResults.rowSumTable, tableSize);
        
      if(status==CSL_SOK)
        status = CSL_copyToUser(userBscResults.colSumTable, bscResults.colSumTable, tableSize);        

      bscResults.rowSumTable = userBscResults.rowSumTable;
      bscResults.colSumTable = userBscResults.colSumTable;    
      if(status==CSL_SOK)
        status = CSL_copyToUser(prm, &bscResults, sizeof(bscResults));
    }  
    
    if(tableAddr!=NULL)
      CSL_sysMemFree(tableAddr);
  }
    break;

  case CSL_IPIPE_CMD_HISTOGRAM_ENABLE:

    status = CSL_ipipeHistogramEnable(hndl, (Bool32) prm);
    break;

  case CSL_IPIPE_CMD_HISTOGRAM_REGION_ENABLE:

    status = CSL_ipipeHistogramRegionEnable(hndl, (Bool32 *) prm);
    break;

  case CSL_IPIPE_CMD_HISTOGRAM_COLOR_ENABLE:

    status = CSL_ipipeHistogramColorEnable(hndl, (Bool32 *) prm);
    break;
  
  case CSL_IPIPE_CMD_HISTOGRAM_TABLE_SELECT:  
    status = CSL_ipipeHistogramTableSelect(hndl, (Uint32)prm);
    break;

  case CSL_IPIPE_CMD_HISTOGRAM_READ_RESULTS:

    hstResults = (CSL_IpipeHistResults*) prm;   //ANR-Hist
    if (hstResults->size == 0 )
        tableSize = CSL_IPIPE_HISTOGRAM_RESULTS_MAX_ENTRIES*4;
    else
	 tableSize = hstResults->size;
		
    tableAddr = CSL_sysMemAlloc(tableSize);
	
    if(tableAddr==NULL)
      status=CSL_EFAIL;
    
    if(status==CSL_SOK){  
      status = CSL_ipipeHistogramReadResults(hndl, tableAddr, hstResults->offset, hstResults->size); //ANR
    }

    if (status == CSL_SOK)
      status = CSL_copyToUser((void*)hstResults->table, tableAddr, tableSize);
      
    if(tableAddr!=NULL)
      CSL_sysMemFree(tableAddr);

    break;
#if 1 //Gang: Histogram read Idle Y
  case CSL_IPIPE_CMD_HISTOGRAM_READ_IDLE_Y:

    tableSize = 256 * 4;
    tableAddr = CSL_sysMemAlloc(tableSize);
    
    if(tableAddr==NULL)
      status=CSL_EFAIL;
    
    if(status==CSL_SOK)
      status = CSL_ipipeHistogramReadIdleY(hndl, tableAddr);

    if (status == CSL_SOK)
      status = CSL_copyToUser(prm, tableAddr, tableSize);
      
    if(tableAddr!=NULL)
      CSL_sysMemFree(tableAddr);

    break;
#endif

  case CSL_IPIPE_CMD_BSC_BUF_INIT:

    status = CSL_copyFromUser(&bufInit, prm, sizeof(bufInit));

    if (status == CSL_SOK)
      status = CSL_ipipeBscBufInit(hndl, &bufInit);

    break;

  case CSL_IPIPE_CMD_BSC_BUF_SWITCH_ENABLE:

    status = CSL_ipipeBscBufSwitchEnable(hndl, (Bool32) prm);
    break;

  case CSL_IPIPE_CMD_BSC_BUF_GET_FULL:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&bufGetFullPrm, prm, sizeof(bufGetFullPrm));

    if (status == CSL_SOK)
      status = CSL_ipipeBscBufGetFull(hndl, &buf, bufGetFullPrm.minBuf, bufGetFullPrm.timeout);

    status = CSL_copyToUser(bufGetFullPrm.buf, &buf, sizeof(buf));

    break;

  case CSL_IPIPE_CMD_BSC_BUF_PUT_EMPTY:

    status = CSL_copyFromUser(&buf, prm, sizeof(buf));

    if (status == CSL_SOK)
      status = CSL_ipipeBscBufPutEmpty(hndl, &buf);

    break;

  case CSL_IPIPE_CMD_BOXCAR_BUF_INIT:

    status = CSL_copyFromUser(&bufInit, prm, sizeof(bufInit));

    if (status == CSL_SOK)
      status = CSL_ipipeBoxcarBufInit(hndl, &bufInit);

    break;

  case CSL_IPIPE_CMD_BOXCAR_BUF_SWITCH_ENABLE:

    status = CSL_ipipeBoxcarBufSwitchEnable(hndl, (Bool32) prm);
    break;

  case CSL_IPIPE_CMD_BOXCAR_BUF_GET_FULL:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&bufGetFullPrm, prm, sizeof(bufGetFullPrm));

    if (status == CSL_SOK)
      status = CSL_ipipeBoxcarBufGetFull(hndl, &buf, bufGetFullPrm.minBuf, bufGetFullPrm.timeout);

    status = CSL_copyToUser(bufGetFullPrm.buf, &buf, sizeof(buf));

    break;

  case CSL_IPIPE_CMD_BOXCAR_BUF_PUT_EMPTY:

    status = CSL_copyFromUser(&buf, prm, sizeof(buf));

    if (status == CSL_SOK)
      status = CSL_ipipeBoxcarBufPutEmpty(hndl, &buf);

    break;

  case CSL_IPIPE_CMD_INT_ENABLE:

    status = CSL_copyFromUser(&intEnablePrm, prm, sizeof(intEnablePrm));

    if (status == CSL_SOK)
      status = CSL_ipipeIntEnable(hndl, intEnablePrm.ipipeIntType, intEnablePrm.enable);

    break;

  case CSL_IPIPE_CMD_INT_CLEAR:

    status = CSL_ipipeIntClear(hndl, (Uint32) prm);
    break;

  case CSL_IPIPE_CMD_INT_WAIT:

    status = CSL_ipipeIntClear(hndl, (Uint32) prm);
    break;

  case CSL_IPIPE_CMD_BOXCAR_SET_OUT_ADDR:
  
    status = CSL_ipipeBoxcarSetOutAddr(hndl, (Uint8*)prm);
    break;

  default:
    status = CSL_EFAIL;
    break;
  }

  return status;
}
