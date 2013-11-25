
#include <imageTunePriv.h>


#ifdef IMAGE_TUNE_MULTI_PARAM
extern IMAGE_TUNE_ParamSetList *gImageTunePrmList;
static Uint32 gPID;
//static IMAGE_TUNE_ParamSet dstPrm;
static IMAGE_TUNE_Pkt_ParamSet dstPrm;
#endif

int IMAGE_TUNE_CmdExecuteEchoString(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  OSA_printf(" IMAGE TUNE SERVER: RECV 0x%04x, %s \n", cmdInfo->commandId, (char*)cmdInfo->prm);

  return OSA_SOK;
}

int IMAGE_TUNE_CmdExecuteGetVersion(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  statusInfo->prm[0] = IMAGE_TUNE_VERSION;
  statusInfo->prmSize = sizeof(Uint32);

  return OSA_SOK;
}

int IMAGE_TUNE_CmdExecuteSaveParamSet(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal;
  Uint32 paramSetId;

  paramSetId = cmdInfo->prm[0];

  retVal = IMAGE_TUNE_SaveParams(paramSetId);

  return retVal;
}

int IMAGE_TUNE_CmdExecuteLoadParamSet(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal;
  Uint32 paramSetId;

  paramSetId = cmdInfo->prm[0];

  retVal = IMAGE_TUNE_LoadParams(paramSetId);

  return retVal;
}


#ifdef IMAGE_TUNE_MULTI_PARAM

int IMAGE_TUNE_CmdExecuteGetParamSet(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retval = OSA_SOK;
  int pID = 0;
  IMAGE_TUNE_ParamSetList *pPrmList;
  //IMAGE_TUNE_Pkt_ParamSet *dstPrm;
  pPrmList = gImageTunePrmList;
  pID = ( (cmdInfo->commandFlags >>IMAGE_TUNE_CMD_PRMSET_ID_ST) & IMAGE_TUNE_CMD_PRMSET_ID_MUL);


#ifdef IMAGE_TUNE_TEST_GET_PRM
  //if(gPID >= IMAGE_TUNE_MAX_PARAMSET ||gPID < IMAGE_TUNE_DEFAULT_PARAMSET )
  //  	gPID = 0;

  pID = gPID;
  gPID++;
#endif

  if(pID >= IMAGE_TUNE_MAX_PARAMSET ||pID < IMAGE_TUNE_DEFAULT_PARAMSET || gImageTunePrmList == NULL ){  //send Current Param set

    OSA_printf(" IMAGE TUNE: Wrong PID:%d; Unable to get params. \n ", pID);
    retval = OSA_EFAIL;
  }
  else{   /* Copy the required parameterset */

	  OSA_printf(" IMAGE TUNE: Get PID:%d; Prm Size %d... \n", pID, sizeof(dstPrm) );

	  if(pID != IMAGE_TUNE_DEFAULT_PARAMSET)
	  {
		  memcpy(&dstPrm.ccdcPrm, &pPrmList->paramSet[pID].ccdcPrm,  sizeof(IMAGE_TUNE_CcdcParams_m));
		  memcpy(dstPrm.ccdcPrm.lscGainTable, pPrmList->ccdcTable.lscGainTable,  sizeof(dstPrm.ccdcPrm.lscGainTable));
		  memcpy(dstPrm.ccdcPrm.lscOffsetTable, pPrmList->ccdcTable.lscOffsetTable, sizeof(dstPrm.ccdcPrm.lscOffsetTable));
		  memcpy(&dstPrm.ipipeifPrm, &pPrmList->paramSet[pID].ipipeifPrm, sizeof(dstPrm.ipipeifPrm));
		  memcpy(&dstPrm.ipipePrm, &pPrmList->paramSet[pID].ipipePrm, sizeof(dstPrm.ipipePrm));
		  memcpy(&dstPrm.ldcPrm, &pPrmList->paramSet[pID].ldcPrm, sizeof(dstPrm.ldcPrm));
		  memcpy(&dstPrm.vnfPrm, &pPrmList->paramSet[pID].vnfPrm, sizeof(dstPrm.vnfPrm));
		  //memcpy(&dstPrm.awbPrm, &pPrmList->paramSet[pID].awbPrm, sizeof(dstPrm.awbPrm));
		  /* Copy of IMAGE_TUNE_ParamSet - Awb st removed to for sending it to IIT(PC)	*/

		  //dstPrm.checkSum = IMAGE_TUNE_CalcParamSetChecksum((IMAGE_TUNE_ParamSet*)&dstPrm);
		  dstPrm.checkSum = IMAGE_TUNE_CalcParamSetChecksum2(&dstPrm);

		  statusInfo->prmSize = sizeof(IMAGE_TUNE_Pkt_ParamSet);

		  memcpy(statusInfo->prm, &dstPrm, statusInfo->prmSize);
	  }
	  else
	  {
		  gIMAGE_TUNE_ctrl.curPrm.checkSum = IMAGE_TUNE_CalcParamSetChecksum2(&gIMAGE_TUNE_ctrl.curPrm);
		  statusInfo->prmSize = sizeof(IMAGE_TUNE_Pkt_ParamSet);
		  memcpy(statusInfo->prm, &gIMAGE_TUNE_ctrl.curPrm, statusInfo->prmSize);
	  }

	  //OSA_cmemFree(dstPrm);
  }

  return retval;
}


/* for Booting paramset */
int IMAGE_TUNE_CmdExecuteSetBootParamSet(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retval = OSA_SOK;
  int pID = 0;
  IMAGE_TUNE_ParamSetList *pPrmList;

  pPrmList = gImageTunePrmList;
  pID = ( (cmdInfo->commandFlags >>IMAGE_TUNE_CMD_PRMSET_ID_ST) & IMAGE_TUNE_CMD_PRMSET_ID_MUL);

  if(pID > IMAGE_TUNE_DEFAULT_PARAMSET || pID < IMAGE_TUNE_MAX_PARAMSET) {

	pPrmList->curParamset = pID;

  }

  return retval;
}

#else

int IMAGE_TUNE_CmdExecuteGetParamSet(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  gIMAGE_TUNE_ctrl.curPrm.checkSum = IMAGE_TUNE_CalcParamSetChecksum(&gIMAGE_TUNE_ctrl.curPrm);

  statusInfo->prmSize = sizeof(IMAGE_TUNE_ParamSet);

  memcpy(statusInfo->prm, &gIMAGE_TUNE_ctrl.curPrm, statusInfo->prmSize);
  OSA_fileWriteFile("/mnt/nand/param_org.bin", (Uint8*)&gIMAGE_TUNE_ctrl.curPrm, sizeof(gIMAGE_TUNE_ctrl.curPrm));

  return OSA_SOK;
}

#endif

