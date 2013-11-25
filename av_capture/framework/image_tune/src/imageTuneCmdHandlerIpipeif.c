#include <imageTunePriv.h>

int IMAGE_TUNE_CmdExecuteIpipeifVpiIsifInDpc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal=OSA_SOK;
  Uint32 reqdPrmSize;

  reqdPrmSize = sizeof(Uint32)*2;

  if(cmdInfo->prmSize != reqdPrmSize ) {
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
    OSA_printf(" IMAGE TUNE SERVER: Received parameter size (%d != %d) mismatch ERROR for CMD 0x%04x\n", cmdInfo->prmSize, reqdPrmSize, cmdInfo->commandId);
    #endif
    return OSA_EFAIL;
  }

  gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.vpiIsifInDpcEnable = cmdInfo->prm[0];
  gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.vpiIsifInDpcThreshold = cmdInfo->prm[1];

  CSL_FINS(gCSL_vpssHndl.ipipeifRegs->DPC1, IPIPEIF_DPC1_DPC1ENA, gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.vpiIsifInDpcEnable);
  CSL_FINS(gCSL_vpssHndl.ipipeifRegs->DPC1, IPIPEIF_DPC1_DPC1TH, gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.vpiIsifInDpcThreshold);

  return retVal;
}

int IMAGE_TUNE_CmdExecuteIpipeifDdrInDpc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  int retVal=OSA_SOK;
  Uint32 reqdPrmSize;

  reqdPrmSize = sizeof(Uint32)*2;

  if(cmdInfo->prmSize != reqdPrmSize ) {
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
    OSA_printf(" IMAGE TUNE SERVER: Received parameter size (%d != %d) mismatch ERROR for CMD 0x%04x\n", cmdInfo->prmSize, reqdPrmSize, cmdInfo->commandId);
    #endif
    return OSA_EFAIL;
  }

  gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.ddrInDpcEnable = cmdInfo->prm[0];
  gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.ddrInDpcThreshold = cmdInfo->prm[1];

  CSL_FINS(gCSL_vpssHndl.ipipeifRegs->DPC2, IPIPEIF_DPC2_DPC2ENA, gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.ddrInDpcEnable);
  CSL_FINS(gCSL_vpssHndl.ipipeifRegs->DPC2, IPIPEIF_DPC2_DPC2TH, gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.ddrInDpcThreshold);

  return retVal;
}

int IMAGE_TUNE_CmdExecuteIpipeifGain(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
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

  gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.gain = cmdInfo->prm[0];

  gCSL_vpssHndl.ipipeifRegs->GAIN = gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.gain;

  return retVal;
}

int IMAGE_TUNE_CmdExecuteIpipeifOutClip(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
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

  gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.outClip = cmdInfo->prm[0];

  gCSL_vpssHndl.ipipeifRegs->OCLIP = gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.outClip;

  return retVal;
}

int IMAGE_TUNE_CmdExecuteIpipeifAvgFilter(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
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

  gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.avgFilterEnable = cmdInfo->prm[0];

  CSL_FINS(gCSL_vpssHndl.ipipeifRegs->CFG1, IPIPEIF_CFG1_AVGFILT, gIMAGE_TUNE_ctrl.curPrm.ipipeifPrm.avgFilterEnable);

  return retVal;
}
