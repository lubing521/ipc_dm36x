#include <imageTunePriv.h>

int IMAGE_TUNE_CmdExecuteCcdcHLpf(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal=OSA_SOK;
  Uint32 reqdPrmSize;

  reqdPrmSize = sizeof(Uint32);

  if(cmdInfo->prmSize != reqdPrmSize ) {
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
    OSA_printf(" IMAGE TUNE SERVER: Received parameter size (%d != %d) mismatch ERROR for CMD 0x%04x\n", cmdInfo->prmSize, reqdPrmSize, cmdInfo->commandId);
    #endif
    return OSA_EFAIL;
  }

  gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.hLpfEnable = cmdInfo->prm[0];

  CSL_FINS(gCSL_vpssHndl.isifRegs->MODESET, ISIF_MODESET_HLPF, gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.hLpfEnable);

  return retVal;
}

int IMAGE_TUNE_CmdExecuteCcdcInDataMsbPosition(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal=OSA_SOK;
  Uint32 reqdPrmSize;

  reqdPrmSize = sizeof(Uint32);

  if(cmdInfo->prmSize != reqdPrmSize ) {
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
    OSA_printf(" IMAGE TUNE SERVER: Received parameter size (%d != %d) mismatch ERROR for CMD 0x%04x\n", cmdInfo->prmSize, reqdPrmSize, cmdInfo->commandId);
    #endif
    return OSA_EFAIL;
  }

  gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.inDataMsbPosition = cmdInfo->prm[0];

  CSL_FINS(gCSL_vpssHndl.isifRegs->MODESET, ISIF_MODESET_CCDW, gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.inDataMsbPosition);

  return retVal;
}

int IMAGE_TUNE_CmdExecuteCcdcLinerization(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal;
  Uint32 reqdPrmSize;
  Uint32 prmSize;
  Uint32 tableSize;
  CSL_CcdcLinerizationConfig *pPrm = &gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.lin;
  Uint32 *pTable = gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.linTable;


  prmSize = sizeof(*pPrm);
  tableSize = sizeof(gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.linTable);

  reqdPrmSize =  prmSize + tableSize;

  if(cmdInfo->prmSize != reqdPrmSize ) {
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
    OSA_printf(" IMAGE TUNE SERVER: Received parameter size (%d != %d) mismatch ERROR for CMD 0x%04x\n", cmdInfo->prmSize, reqdPrmSize, cmdInfo->commandId);
    #endif
    return OSA_EFAIL;
  }

  memcpy( pPrm, cmdInfo->prm, prmSize);
  memcpy( pTable, (Uint8*)cmdInfo->prm + prmSize , tableSize);

  if(pPrm->enable) {
    // if LIN enable, then first disable, wait for one VD time interval and then enable, else while table setting system will hang
    
    pPrm->enable = FALSE;
    pPrm->lutTable = NULL;
  
    retVal = CSL_ccdcSetLinerizationConfig(&gCSL_ccdcHndl, pPrm);  
    
    OSA_waitMsecs(100);
    
    pPrm->enable = TRUE;    
  }

  pPrm->lutTable = pTable;

  retVal = CSL_ccdcSetLinerizationConfig(&gCSL_ccdcHndl, pPrm);

  return retVal;
}

int IMAGE_TUNE_CmdExecuteCcdcColPat(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal=OSA_SOK;
  Uint32 reqdPrmSize;
  CSL_CcdcColPatConfig *pPrm = &gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.colPat;

  reqdPrmSize = sizeof(*pPrm);

  if(cmdInfo->prmSize != reqdPrmSize ) {
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
    OSA_printf(" IMAGE TUNE SERVER: Received parameter size (%d != %d) mismatch ERROR for CMD 0x%04x\n", cmdInfo->prmSize, reqdPrmSize, cmdInfo->commandId);
    #endif
    return OSA_EFAIL;
  }

  memcpy(pPrm, cmdInfo->prm, reqdPrmSize);

  retVal = CSL_ccdcSetColPatConfig(&gCSL_ccdcHndl, pPrm);
  
  return retVal;
}


int IMAGE_TUNE_CmdExecuteCcdcGainOffset(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal;
  Uint32 reqdPrmSize;
  CSL_CcdcGainOffsetConfig *pPrm = &gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.gainOffset;

  reqdPrmSize = sizeof(*pPrm);

  if(cmdInfo->prmSize != reqdPrmSize ) {
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
    OSA_printf(" IMAGE TUNE SERVER: Received parameter size (%d != %d) mismatch ERROR for CMD 0x%04x\n", cmdInfo->prmSize, reqdPrmSize, cmdInfo->commandId);
    #endif
    return OSA_EFAIL;
  }

  memcpy(pPrm, cmdInfo->prm, reqdPrmSize);

  retVal = CSL_ccdcSetGainOffsetConfig(&gCSL_ccdcHndl, pPrm);

  return retVal;
}

int IMAGE_TUNE_CmdExecuteCcdcDfc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal;

  Uint32 prmSize;
  Uint32 tableSize;
  Uint32 reqdPrmSize;

  CSL_CcdcDfcConfig *pPrm = &gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.dfc;
  CSL_CcdcVdfcTable *pTable = &gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.vdfcTable;

  prmSize = sizeof(*pPrm);
  tableSize = sizeof(gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.vdfcTable);

  reqdPrmSize =  prmSize + tableSize;

  if(cmdInfo->prmSize != reqdPrmSize ) {
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
    OSA_printf(" IMAGE TUNE SERVER: Received parameter size (%d != %d) mismatch ERROR for CMD 0x%04x\n", cmdInfo->prmSize, reqdPrmSize, cmdInfo->commandId);
    #endif
    return OSA_EFAIL;
  }

  memcpy( pPrm, cmdInfo->prm, prmSize);
  memcpy( pTable, (Uint8*)cmdInfo->prm + prmSize , tableSize);

  pPrm->vdfcTable = pTable;

  retVal = CSL_ccdcSetDfcConfig(&gCSL_ccdcHndl, pPrm);

  return retVal;
}

int IMAGE_TUNE_CmdExecuteCcdcClamp(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal;
  Uint32 reqdPrmSize;
  CSL_CcdcClampConfig *pPrm = &gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.clamp;

  reqdPrmSize = sizeof(*pPrm);

  if(cmdInfo->prmSize != reqdPrmSize ) {
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
    OSA_printf(" IMAGE TUNE SERVER: Received parameter size (%d != %d) mismatch ERROR for CMD 0x%04x\n", cmdInfo->prmSize, reqdPrmSize, cmdInfo->commandId);
    #endif
    return OSA_EFAIL;
  }

  memcpy(pPrm, cmdInfo->prm, reqdPrmSize);
  
  DRV_isifLock();  //ANR - ISIF
  retVal = CSL_ccdcSetClampConfig(&gCSL_ccdcHndl, pPrm);
  DRV_isifUnlock();  //ANR - ISIF

  return retVal;
}

int IMAGE_TUNE_CmdExecuteCcdcLsc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  Uint32 tableSize1;
  Uint32 tableSize2;
  Uint32 prmSize;
  Uint32 reqdPrmSize;
  CSL_CcdcLscConfig *pPrm = &gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.lsc;
  Uint32 *pTable1 = gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.lscGainTable;
  Uint32 *pTable2 = gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.lscOffsetTable;


  tableSize1  = sizeof(gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.lscGainTable);
  tableSize2  = sizeof(gIMAGE_TUNE_ctrl.curPrm.ccdcPrm.lscOffsetTable);
  prmSize     = sizeof(*pPrm);

  reqdPrmSize = prmSize + tableSize1 + tableSize2;

  if(cmdInfo->prmSize != reqdPrmSize ) {
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
    OSA_printf(" IMAGE TUNE SERVER: Received parameter size (%d != %d) mismatch ERROR for CMD 0x%04x\n", cmdInfo->prmSize, reqdPrmSize, cmdInfo->commandId);
    #endif
    return OSA_EFAIL;
  }

  memcpy(pPrm   , cmdInfo->prm                                , prmSize);
  memcpy(pTable1, (Uint8*)cmdInfo->prm + prmSize              , tableSize1);
  memcpy(pTable2, (Uint8*)cmdInfo->prm + prmSize + tableSize1 , tableSize2);

  pPrm->gainTableAddr = (Uint8*)pTable1;
  pPrm->offsetTableAddr = (Uint8*)pTable2;

  return OSA_SOK;
}


