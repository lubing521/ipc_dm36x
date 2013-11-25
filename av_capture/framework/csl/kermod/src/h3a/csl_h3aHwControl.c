#include <csl_h3aIoctl.h>

CSL_Status CSL_h3aHwControl(CSL_H3aHandle hndl, Uint32 cmd, void *prm)
{
  CSL_Status status = CSL_SOK;
  static CSL_H3aInFrameConfig inFrameConfig;
  static CSL_H3aAfConfig afConfig;
  static CSL_H3aAewbConfig aewbConfig;
  static CSL_H3aHwSetup data;

  static CSL_H3aBufInitPrm bufInitPrm;
  static CSL_H3aBufSwitchEnablePrm bufSwitchEnablePrm;  
  static CSL_H3aBufGetFullPrm bufGetFullPrm;
  static CSL_H3aBufPutEmptyPrm bufPutEmptyPrm;

  static CSL_BufInit bufInit;
  static CSL_BufInfo buf;

  switch (cmd) {

  case CSL_H3A_CMD_HW_SETUP:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&data, prm, sizeof(data));

    if (status == CSL_SOK) {
      if(data.inFrameConfig) {
        status = CSL_copyFromUser(&inFrameConfig, data.inFrameConfig, sizeof(inFrameConfig));
        
        data.inFrameConfig = &inFrameConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.afConfig) {
        status = CSL_copyFromUser(&afConfig, data.afConfig, sizeof(afConfig));
        
        data.afConfig = &afConfig;
      }
    }

    if (status == CSL_SOK) {
      if(data.aewbConfig) {
        status = CSL_copyFromUser(&aewbConfig, data.aewbConfig, sizeof(aewbConfig));
        
        data.aewbConfig = &aewbConfig;
      }
    }

    if (status == CSL_SOK)
      status = CSL_h3aHwSetup(hndl, &data);

    break;

  case CSL_H3A_CMD_HW_RESET:

    status = CSL_h3aHwReset(hndl);
    break;

  case CSL_H3A_CMD_INFRAME_CONFIG:

    status = CSL_copyFromUser(&inFrameConfig, prm, sizeof(inFrameConfig));

    if (status == CSL_SOK)
      status = CSL_h3aInFrameConfig(hndl, &inFrameConfig);

    break;

  case CSL_H3A_CMD_AF_ENABLE:

    status = CSL_h3aAfEnable(hndl, (Bool32) prm);
    break;

  case CSL_H3A_CMD_AF_VF_ENABLE:

    status = CSL_h3aAfVfEnable(hndl, (Bool32) prm);
    break;

  case CSL_H3A_CMD_AF_ALAW_ENABLE:

    status = CSL_h3aAfAlawEnable(hndl, (Bool32) prm);
    break;

  case CSL_H3A_CMD_AF_SET_OUTADDR:

    status = CSL_h3aAfSetOutAddr(hndl, (Uint8 *) prm);
    break;

  case CSL_H3A_CMD_AF_SET_CONFIG:

    status = CSL_copyFromUser(&afConfig, prm, sizeof(afConfig));

    if (status == CSL_SOK)
      status = CSL_h3aAfSetConfig(hndl, &afConfig);

    break;

  case CSL_H3A_CMD_AEWB_ENABLE:

    status = CSL_h3aAewbEnable(hndl, (Bool32) prm);
    break;

  case CSL_H3A_CMD_AEWB_ALAW_ENABLE:

    status = CSL_h3aAewbAlawEnable(hndl, (Bool32) prm);
    break;

  case CSL_H3A_CMD_AEWB_SET_OUTADDR:

    status = CSL_h3aAewbSetOutAddr(hndl, (Uint8 *) prm);
    break;

  case CSL_H3A_CMD_AEWB_SET_CONFIG:

    status = CSL_copyFromUser(&aewbConfig, prm, sizeof(aewbConfig));

    if (status == CSL_SOK)
      status = CSL_h3aAewbSetConfig(hndl, &aewbConfig);

    break;

  case CSL_H3A_CMD_BUF_INIT:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&bufInitPrm, prm, sizeof(bufInitPrm));

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&bufInit, bufInitPrm.bufInit, sizeof(bufInit));

    if (status == CSL_SOK) {
      bufInitPrm.bufInit = &bufInit;
    }

    if (status == CSL_SOK)
      status = CSL_h3aBufInit(hndl, bufInitPrm.h3aModId, bufInitPrm.bufInit);

    break;

  case CSL_H3A_CMD_BUF_SWITCH_ENABLE:

    status = CSL_copyFromUser(&bufSwitchEnablePrm, prm, sizeof(bufSwitchEnablePrm));

    if (status == CSL_SOK)
      status = CSL_h3aBufSwitchEnable(hndl, bufSwitchEnablePrm.h3aModId, bufSwitchEnablePrm.enable);

    break;

  case CSL_H3A_CMD_BUF_GET_FULL:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&bufGetFullPrm, prm, sizeof(bufGetFullPrm));

    if (status == CSL_SOK)
      status = CSL_h3aBufGetFull(hndl, bufGetFullPrm.h3aModId, &buf, bufGetFullPrm.minBuf, bufGetFullPrm.timeout);

    status = CSL_copyToUser(bufGetFullPrm.buf, &buf, sizeof(buf));

    break;

  case CSL_H3A_CMD_BUF_PUT_EMPTY:

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&bufPutEmptyPrm, prm, sizeof(bufPutEmptyPrm));

    if (status == CSL_SOK)
      status = CSL_copyFromUser(&buf, bufPutEmptyPrm.buf, sizeof(buf));

    if (status == CSL_SOK) {
      bufPutEmptyPrm.buf = &buf;
    }

    if (status == CSL_SOK)
      status = CSL_h3aBufPutEmpty(hndl, bufPutEmptyPrm.h3aModId, bufPutEmptyPrm.buf);

    break;

  default:
    status = CSL_EFAIL;
    break;
  }

  return status;
}
