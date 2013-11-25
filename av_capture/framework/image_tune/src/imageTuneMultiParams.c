
#include <imageTunePriv.h>

#ifdef IMAGE_TUNE_MULTI_PARAM

extern IMAGE_TUNE_ParamSetList *gImageTunePrmList;

#ifdef IMAGE_TUNE_GET_NEW_PRM
extern int gCurPrmSet;       // to keep current paramset id...
#endif

/* Update the Params in to the respective paramset in the multi param list */
int IMAGE_TUNE_MultiParams_Set(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo){

  int retVal = OSA_SOK;
  Uint32 PID = 0;
  IMAGE_TUNE_ParamSetList *dstPrm;
  IMAGE_TUNE_ParamSet *srcpPrm;
  Uint32 reqdPrmSize = 0, tableSize = 0, prmSize = 0;

  if( gImageTunePrmList == NULL)   // || (gIMAGE_TUNE_ctrl.curPrm == NULL))
        return retVal;

  srcpPrm = &gIMAGE_TUNE_ctrl.curPrm;
  dstPrm = gImageTunePrmList;
  
  PID = ( (cmdInfo->commandFlags >>IMAGE_TUNE_CMD_PRMSET_ID_ST) & IMAGE_TUNE_CMD_PRMSET_ID_MUL);
#ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
  OSA_printf("\n IMAGE TUNE: Multi Param: Cmd 0x%x; Parameters set ID 0x%x ",cmdInfo->commandId, PID);
#endif

  if ((PID <= IMAGE_TUNE_DEFAULT_PARAMSET) || (PID >= IMAGE_TUNE_MAX_PARAMSET ))
  {
#ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
    OSA_printf("\n IMAGE TUNE: Multi Param: Unable to set Parameters set ID 0x%x \n",PID);
#endif
    return retVal;
  }
 

  switch(cmdInfo->commandId){

        /* IPIPE - Params */
        case IMAGE_TUNE_IPIPE_COL_PAT:
            reqdPrmSize = sizeof(srcpPrm->ipipePrm.colPat);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.colPat, &srcpPrm->ipipePrm.colPat, reqdPrmSize);
            break;
		
        case IMAGE_TUNE_IPIPE_LSC:
            reqdPrmSize = sizeof(srcpPrm->ipipePrm.lsc);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.lsc, &srcpPrm->ipipePrm.lsc, reqdPrmSize);
            break;
		
        case IMAGE_TUNE_IPIPE_DPC:
            prmSize = sizeof(srcpPrm->ipipePrm.dpc);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.dpc, &srcpPrm->ipipePrm.dpc, prmSize);
            tableSize = sizeof(srcpPrm->ipipePrm.dpcTable);
            memcpy(dstPrm->paramSet[PID].ipipePrm.dpcTable, srcpPrm->ipipePrm.dpcTable, tableSize);
            dstPrm->paramSet[PID].ipipePrm.dpc.lutAddr = dstPrm->paramSet[PID].ipipePrm.dpcTable;            
            break;
		
        case IMAGE_TUNE_IPIPE_NF1:
            reqdPrmSize = sizeof(srcpPrm->ipipePrm.nf1);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.nf1, &srcpPrm->ipipePrm.nf1, reqdPrmSize);
            break;

        case IMAGE_TUNE_IPIPE_NF2:
            reqdPrmSize = sizeof(srcpPrm->ipipePrm.nf2);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.nf2, &srcpPrm->ipipePrm.nf2, reqdPrmSize);
            break;
		
        case IMAGE_TUNE_IPIPE_GIC:
            reqdPrmSize = sizeof(srcpPrm->ipipePrm.gic);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.gic, &srcpPrm->ipipePrm.gic, reqdPrmSize);
            break;
		
        case IMAGE_TUNE_IPIPE_WB:
            reqdPrmSize = sizeof(srcpPrm->ipipePrm.wb);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.wb, &srcpPrm->ipipePrm.wb, reqdPrmSize);
            break;
		
        case IMAGE_TUNE_IPIPE_CFA:
            reqdPrmSize = sizeof(srcpPrm->ipipePrm.cfa);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.cfa, &srcpPrm->ipipePrm.cfa, reqdPrmSize);
            break;
		
        case IMAGE_TUNE_IPIPE_RGB2RGB1:
            reqdPrmSize = sizeof(srcpPrm->ipipePrm.rgb2rgb1);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.rgb2rgb1, &srcpPrm->ipipePrm.rgb2rgb1, reqdPrmSize);
            break;
		
        case IMAGE_TUNE_IPIPE_GAMMA:
            prmSize = sizeof(srcpPrm->ipipePrm.gamma);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.gamma, &srcpPrm->ipipePrm.gamma, prmSize);

            tableSize = sizeof(srcpPrm->ipipePrm.gammaTableR);
            memcpy(dstPrm->paramSet[PID].ipipePrm.gammaTableR, srcpPrm->ipipePrm.gammaTableR, tableSize);
            dstPrm->paramSet[PID].ipipePrm.gamma.tableR= dstPrm->paramSet[PID].ipipePrm.gammaTableR;            

            tableSize = sizeof(srcpPrm->ipipePrm.gammaTableG);
            memcpy(dstPrm->paramSet[PID].ipipePrm.gammaTableG, srcpPrm->ipipePrm.gammaTableG, tableSize);
            dstPrm->paramSet[PID].ipipePrm.gamma.tableG= dstPrm->paramSet[PID].ipipePrm.gammaTableG;            

            tableSize = sizeof(srcpPrm->ipipePrm.gammaTableB);
            memcpy(dstPrm->paramSet[PID].ipipePrm.gammaTableB, srcpPrm->ipipePrm.gammaTableB, tableSize);
            dstPrm->paramSet[PID].ipipePrm.gamma.tableB= dstPrm->paramSet[PID].ipipePrm.gammaTableB;            
            break;

	case IMAGE_TUNE_IPIPE_RGB2RGB2:
            reqdPrmSize = sizeof(srcpPrm->ipipePrm.rgb2rgb2);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.rgb2rgb2, &srcpPrm->ipipePrm.rgb2rgb2, reqdPrmSize);
            break;
		
        case IMAGE_TUNE_IPIPE_LUT3D:
            prmSize = sizeof(srcpPrm->ipipePrm.lut3dTable);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.lut3d, &srcpPrm->ipipePrm.lut3d, prmSize);
            tableSize = sizeof(srcpPrm->ipipePrm.lut3d);
            memcpy(dstPrm->paramSet[PID].ipipePrm.lut3dTable, srcpPrm->ipipePrm.lut3dTable, tableSize);
            dstPrm->paramSet[PID].ipipePrm.lut3d.table= dstPrm->paramSet[PID].ipipePrm.lut3dTable;            
            break;
		
        case IMAGE_TUNE_IPIPE_RGB2YUV:
            reqdPrmSize = sizeof(srcpPrm->ipipePrm.rgb2yuv);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.rgb2yuv, &srcpPrm->ipipePrm.rgb2yuv, reqdPrmSize);
            break;
			
        case IMAGE_TUNE_IPIPE_CNT_BRT:
            reqdPrmSize = sizeof(srcpPrm->ipipePrm.cntBrt);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.cntBrt, &srcpPrm->ipipePrm.cntBrt, reqdPrmSize);
            break;
			
        case IMAGE_TUNE_IPIPE_GBCE:
            prmSize = sizeof(srcpPrm->ipipePrm.gbce);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.gbce, &srcpPrm->ipipePrm.gbce, prmSize);
            tableSize = sizeof(srcpPrm->ipipePrm.gbceTable);
            memcpy(dstPrm->paramSet[PID].ipipePrm.gbceTable, srcpPrm->ipipePrm.gbceTable, tableSize);
            dstPrm->paramSet[PID].ipipePrm.gbce.gainTable= dstPrm->paramSet[PID].ipipePrm.gbceTable;            
            break;
			
        case IMAGE_TUNE_IPIPE_EDGE_ENHANCE:
			
            prmSize = sizeof(srcpPrm->ipipePrm.yee);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.yee, &srcpPrm->ipipePrm.yee, prmSize);
            tableSize = sizeof(srcpPrm->ipipePrm.yeeTable);
            memcpy(dstPrm->paramSet[PID].ipipePrm.yeeTable, srcpPrm->ipipePrm.yeeTable, tableSize);
            dstPrm->paramSet[PID].ipipePrm.yee.table = dstPrm->paramSet[PID].ipipePrm.yeeTable;            
            break;
			
        case IMAGE_TUNE_IPIPE_CAR:
            reqdPrmSize = sizeof(srcpPrm->ipipePrm.car);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.car, &srcpPrm->ipipePrm.car, reqdPrmSize);
            break;
			
        case IMAGE_TUNE_IPIPE_CGS:
            reqdPrmSize = sizeof(srcpPrm->ipipePrm.cgs);
            memcpy(&dstPrm->paramSet[PID].ipipePrm.cgs, &srcpPrm->ipipePrm.cgs, reqdPrmSize);
            break;
        /* CCDC params */
        case IMAGE_TUNE_CCDC_HLPF_ENABLE:
            //reqdPrmSize = sizeof(*srcpPrm->ccdcPrm.hLpfEnable);
            //memcpy(dstPrm->paramSet[PID].ipipePrm.cgs, srcpPrm->ipipePrm.cgs, reqdPrmSize);
            dstPrm->paramSet[PID].ccdcPrm.hLpfEnable = srcpPrm->ccdcPrm.hLpfEnable;
            break;
			
        case IMAGE_TUNE_CCDC_IN_DATA_MSB_POSITION:
             dstPrm->paramSet[PID].ccdcPrm.inDataMsbPosition= srcpPrm->ccdcPrm.inDataMsbPosition;
            break;
			
        case IMAGE_TUNE_CCDC_LINEARIZATION:
            prmSize = sizeof(srcpPrm->ccdcPrm.lin);
            memcpy(&dstPrm->paramSet[PID].ccdcPrm.lin, &srcpPrm->ccdcPrm.lin, prmSize);
            tableSize = sizeof(srcpPrm->ccdcPrm.linTable);
            memcpy(dstPrm->paramSet[PID].ccdcPrm.linTable, srcpPrm->ccdcPrm.linTable, tableSize);
            dstPrm->paramSet[PID].ccdcPrm.lin.lutTable = dstPrm->paramSet[PID].ccdcPrm.linTable;            
            break;
			
        case IMAGE_TUNE_CCDC_COL_PAT:
            reqdPrmSize = sizeof(srcpPrm->ccdcPrm.colPat);
            memcpy(&dstPrm->paramSet[PID].ccdcPrm.colPat, &srcpPrm->ccdcPrm.colPat, reqdPrmSize);
            break;
			
        case IMAGE_TUNE_CCDC_GAIN_OFFSET:
            reqdPrmSize = sizeof(srcpPrm->ccdcPrm.gainOffset);
            memcpy(&dstPrm->paramSet[PID].ccdcPrm.gainOffset, &srcpPrm->ccdcPrm.gainOffset, reqdPrmSize);
            break;
			
        case IMAGE_TUNE_CCDC_DFC:
            prmSize = sizeof(srcpPrm->ccdcPrm.dfc);
            memcpy(&dstPrm->paramSet[PID].ccdcPrm.dfc, &srcpPrm->ccdcPrm.dfc, prmSize);
            tableSize = sizeof(srcpPrm->ccdcPrm.vdfcTable);
            memcpy(&dstPrm->paramSet[PID].ccdcPrm.vdfcTable, &srcpPrm->ccdcPrm.vdfcTable, tableSize);
            dstPrm->paramSet[PID].ccdcPrm.dfc.vdfcTable = &dstPrm->paramSet[PID].ccdcPrm.vdfcTable;      
            break;
			
        case IMAGE_TUNE_CCDC_CLAMP:
            reqdPrmSize = sizeof(srcpPrm->ccdcPrm.clamp);
            memcpy(&dstPrm->paramSet[PID].ccdcPrm.clamp, &srcpPrm->ccdcPrm.clamp, reqdPrmSize);
            break;
			
        case IMAGE_TUNE_CCDC_LSC:
            prmSize = sizeof(srcpPrm->ccdcPrm.lsc);
            memcpy(&dstPrm->paramSet[PID].ccdcPrm.lsc, &srcpPrm->ccdcPrm.lsc, prmSize);

            tableSize = sizeof(srcpPrm->ccdcPrm.lscGainTable); 
            memcpy(dstPrm->ccdcTable.lscGainTable, srcpPrm->ccdcPrm.lscGainTable, tableSize);
            dstPrm->paramSet[PID].ccdcPrm.lsc.gainTableAddr = dstPrm->ccdcTable.lscGainTable;            

            tableSize = sizeof(srcpPrm->ccdcPrm.lscOffsetTable); 
            memcpy(dstPrm->ccdcTable.lscOffsetTable, srcpPrm->ccdcPrm.lscOffsetTable, tableSize);
            dstPrm->paramSet[PID].ccdcPrm.lsc.offsetTableAddr = dstPrm->ccdcTable.lscOffsetTable;            
            break;

        /* IPIPEIF - Params */
        case IMAGE_TUNE_IPIPEIF_VPI_ISIF_IN_DPC:
            dstPrm->paramSet[PID].ipipeifPrm.vpiIsifInDpcEnable = srcpPrm->ipipeifPrm.vpiIsifInDpcEnable;
            dstPrm->paramSet[PID].ipipeifPrm.vpiIsifInDpcThreshold = srcpPrm->ipipeifPrm.vpiIsifInDpcThreshold;
            break;

        case IMAGE_TUNE_IPIPEIF_DDR_IN_DPC:
            dstPrm->paramSet[PID].ipipeifPrm.ddrInDpcEnable= srcpPrm->ipipeifPrm.ddrInDpcEnable;
            dstPrm->paramSet[PID].ipipeifPrm.ddrInDpcThreshold= srcpPrm->ipipeifPrm.ddrInDpcThreshold;
            break;

        case IMAGE_TUNE_IPIPEIF_GAIN:
            dstPrm->paramSet[PID].ipipeifPrm.gain= srcpPrm->ipipeifPrm.gain;
            break;

        case IMAGE_TUNE_IPIPEIF_OUTCLIP:
            dstPrm->paramSet[PID].ipipeifPrm.outClip= srcpPrm->ipipeifPrm.outClip;
            break;
			
        case IMAGE_TUNE_IPIPEIF_AVG_FILTER_ENABLE:
            dstPrm->paramSet[PID].ipipeifPrm.avgFilterEnable= srcpPrm->ipipeifPrm.avgFilterEnable;
            break;

        /* LDC - Params */
        case IMAGE_TUNE_LDC_PRM:
            prmSize = sizeof(srcpPrm->ldcPrm.ldcSetup);
            memcpy(&dstPrm->paramSet[PID].ldcPrm.ldcSetup, &srcpPrm->ldcPrm.ldcSetup, prmSize);
            tableSize = sizeof(srcpPrm->ldcPrm.ldcTable);
            memcpy(dstPrm->paramSet[PID].ldcPrm.ldcTable, srcpPrm->ldcPrm.ldcTable, tableSize);
            dstPrm->paramSet[PID].ldcPrm.ldcSetup.table= dstPrm->paramSet[PID].ldcPrm.ldcTable;            
            dstPrm->paramSet[PID].ldcPrm.ldcSetup.frameConfig = NULL;            
            break;
			
        case IMAGE_TUNE_VNF_PRM:
            reqdPrmSize = sizeof(srcpPrm->vnfPrm);
            memcpy(&dstPrm->paramSet[PID].vnfPrm, &srcpPrm->vnfPrm, reqdPrmSize);
            break;

        case IMAGE_TUNE_AWB_PRM:
             reqdPrmSize = sizeof(srcpPrm->awbPrm);
             memcpy(&dstPrm->paramSet[PID].awbPrm, &srcpPrm->awbPrm, reqdPrmSize);
            break;

        /* IPIPEIF - Params */
        default:
            retVal = OSA_EFAIL;
#ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
            OSA_printf(" \n");
            //OSA_printf(" - unable to set param(s).\n");
#endif
             break;

  }

  dstPrm->paramFlag[PID] = PID;   //Setting the flag for boot

  if(retVal == OSA_SOK){

#ifdef IMAGE_TUNE_GET_NEW_PRM
   gCurPrmSet = PID;   //Revisit
#endif

#ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
  	OSA_printf(" - param(s) set done.\n");
#endif  
  }
  return retVal;

}

#endif


