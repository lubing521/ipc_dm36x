#include <imageTunePriv.h>

int IMAGE_TUNE_CmdExecuteVnfPrm(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  Uint32 reqdPrmSize;
  Uint32 prmSize;
  Uint32 tableSize;
  ALG_vnfParams *pPrm = &gIMAGE_TUNE_ctrl.curPrm.vnfPrm;

  prmSize = sizeof(*pPrm);
  reqdPrmSize =  prmSize;

  if(cmdInfo->prmSize != reqdPrmSize ) {
    #ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
    OSA_printf(" IMAGE TUNE SERVER: Received parameter size (%d != %d) mismatch ERROR for CMD 0x%04x\n", cmdInfo->prmSize, reqdPrmSize, cmdInfo->commandId);
    #endif
    return OSA_EFAIL;
  }

  memcpy( pPrm, cmdInfo->prm, prmSize);

  return OSA_SOK;
}
