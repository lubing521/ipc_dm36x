#include <imageTunePriv.h> /* - IT */

static int gflgAWBCalcDataUpdate;

int IMAGE_TUNE_CmdExecuteAwbPrm(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
    Uint32 tableSize = 0;
    awb_calc_data_t *pPrm = &gIMAGE_TUNE_ctrl.curPrm.awbPrm;
    Uint32 prmSize = sizeof(*pPrm);
    Uint32 reqdPrmSize =  prmSize;

    if (cmdInfo->prmSize != reqdPrmSize )
    {
#ifdef IMAGE_TUNE_CMD_HANDLER_DEBUG
        OSA_printf(" IMAGE TUNE SERVER: Received parameter size (%d != %d) mismatch ERROR for CMD 0x%04x\n", 
            cmdInfo->prmSize, 
            reqdPrmSize, 
            cmdInfo->commandId);
#endif
        return OSA_EFAIL;
    }

    memcpy( pPrm, cmdInfo->prm, prmSize);

    /* to Awb Prm input */
    IMAGE_TUNE_CmdSetAwbPrmStatus(1);
    return OSA_SOK;

}

int IMAGE_TUNE_CmdSetAwbPrmStatus(int status)
{
    gflgAWBCalcDataUpdate = status;
    return status;
}

int IMAGE_TUNE_CmdGetAwbPrmStatus(int* status)
{
    *status = gflgAWBCalcDataUpdate;
    return *status;
}

