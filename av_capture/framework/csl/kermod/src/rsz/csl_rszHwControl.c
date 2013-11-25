#include <csl_rszIoctl.h>

CSL_Status CSL_rszHwControl(CSL_RszHandle hndl, Uint32 cmd, void *prm)
{
  CSL_Status status = CSL_SOK;

  static CSL_RszBufInitPrm bufInitPrm;
  static CSL_BufInit bufInit;
  static CSL_RszBufSwitchEnablePrm bufSwitchEnablePrm;
  static CSL_RszBufGetFullPrm bufGetFullPrm;
  static CSL_RszBufPutEmptyPrm bufPutEmptyPrm;
  static CSL_BufInfo buf;
  static CSL_RszBufConfig bufConfig;
  static CSL_RszSetFlipPrm flipPrm;
  static CSL_RszSetRtPrm  runtimePrm;
  static CSL_RszRuntimeConfig runtimeCfg;

  switch (cmd) {

  case CSL_RSZ_CMD_BUF_INIT:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&bufInitPrm, prm, sizeof(bufInitPrm));

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&bufInit, bufInitPrm.bufInit, sizeof(bufInit));

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&bufConfig, bufInitPrm.bufConfig, sizeof(bufConfig));


    if (status == CSL_SOK) {
      bufInitPrm.bufInit = &bufInit;
      bufInitPrm.bufConfig = &bufConfig;
    }

    if (status == CSL_SOK)
      status = CSL_rszBufInit(hndl, bufInitPrm.rszMod, bufInitPrm.bufInit, bufInitPrm.bufConfig);

    break;

  case CSL_RSZ_CMD_BUF_SWITCH_ENABLE:

    status = CSL_copyFromUser(&bufSwitchEnablePrm, prm, sizeof(bufSwitchEnablePrm));

    if (status == CSL_SOK)
      status = CSL_rszBufSwitchEnable(hndl, bufSwitchEnablePrm.rszMod, bufSwitchEnablePrm.enable);

    break;

  case CSL_RSZ_CMD_BUF_GET_FULL:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&bufGetFullPrm, prm, sizeof(bufGetFullPrm));

    if (status == CSL_SOK)
      status = CSL_rszBufGetFull(hndl, bufGetFullPrm.rszMod, &buf, bufGetFullPrm.minBuf, bufGetFullPrm.timeout);

    status = CSL_copyToUser(bufGetFullPrm.buf, &buf, sizeof(buf));

    break;

  case CSL_RSZ_CMD_BUF_PUT_EMPTY:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&bufPutEmptyPrm, prm, sizeof(bufPutEmptyPrm));

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&buf, bufPutEmptyPrm.buf, sizeof(buf));

    if (status == CSL_SOK) {
      bufPutEmptyPrm.buf = &buf;
    }

    if (status == CSL_SOK)
      status = CSL_rszBufPutEmpty(hndl, bufPutEmptyPrm.rszMod, bufPutEmptyPrm.buf);

    break;

  case CSL_RSZ_CMD_INT_ENABLE:

    status = CSL_rszIntEnable(hndl, (Bool32) prm);
    break;

  case CSL_RSZ_CMD_INT_CLEAR:

    status = CSL_rszIntClear(hndl);
    break;

  case CSL_RSZ_CMD_INT_WAIT:

    status = CSL_rszIntWait(hndl);
    break;

  case CSL_RSZ_CMD_SET_FLIP:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&flipPrm, prm, sizeof(flipPrm));

    if (status == CSL_SOK)
      status = CSL_rszSetFlip(hndl, flipPrm.rszMod, flipPrm.flipH, flipPrm.flipV);
  
    break;

  case CSL_RSZ_CMD_SET_RT_PRMS:
	
	  if (status == CSL_SOK)
		status = CSL_copyFromUser(&runtimePrm, prm, sizeof(runtimePrm));
	
	  if (status == CSL_SOK)
		status = CSL_copyFromUser(&runtimeCfg, runtimePrm.rtConfig, sizeof(runtimeCfg));
	
	  if (status == CSL_SOK) {
		runtimePrm.rtConfig = &runtimeCfg;
	  }
	
	  if (status == CSL_SOK)
		status = CSL_rszSetRuntimeConfigInHndl(hndl, runtimePrm.rszMod, runtimePrm.rtConfig);
	
	  break;

  default:
    status = CSL_EFAIL;
    break;
  }

  return status;
}
