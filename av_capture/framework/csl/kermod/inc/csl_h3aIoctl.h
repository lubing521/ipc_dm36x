
#ifndef _CSL_H3A_IOCTL_
#define _CSL_H3A_IOCTL_

#include <csl_h3a.h>

#define CSL_H3A_CMD_HW_SETUP                     (0x00) ///< prm: [I ] CSL_H3aHwSetup *
#define CSL_H3A_CMD_HW_RESET                     (0x01) ///< prm: [  ] NONE
#define CSL_H3A_CMD_INFRAME_CONFIG               (0x02) ///< prm: [I ] CSL_H3aInFrameConfig *
#define CSL_H3A_CMD_AF_ENABLE                    (0x03) ///< prm: [I ] Bool32
#define CSL_H3A_CMD_AF_VF_ENABLE                 (0x04) ///< prm: [I ] Bool32
#define CSL_H3A_CMD_AF_ALAW_ENABLE               (0x05) ///< prm: [I ] Bool32
#define CSL_H3A_CMD_AF_SET_OUTADDR               (0x06) ///< prm: [I ] Uint8 *
#define CSL_H3A_CMD_AF_SET_CONFIG                (0x07) ///< prm: [I ] CSL_H3aAfConfig *
#define CSL_H3A_CMD_AEWB_ENABLE                  (0x08) ///< prm: [I ] Bool32
#define CSL_H3A_CMD_AEWB_ALAW_ENABLE             (0x09) ///< prm: [I ] Bool32
#define CSL_H3A_CMD_AEWB_SET_OUTADDR             (0x10) ///< prm: [I ] Uint8 *
#define CSL_H3A_CMD_AEWB_SET_CONFIG              (0x11) ///< prm: [I ] CSL_H3aAewbConfig *
#define CSL_H3A_CMD_BUF_INIT                     (0x12) ///< prm: [I ] CSL_H3aBufInitPrm
#define CSL_H3A_CMD_BUF_SWITCH_ENABLE            (0x13) ///< prm: [I ] CSL_H3aBufSwitchEnablePrm *
#define CSL_H3A_CMD_BUF_GET_FULL                 (0x14) ///< prm: [I ] CSL_H3aBufGetFullPrm
#define CSL_H3A_CMD_BUF_PUT_EMPTY                (0x15) ///< prm: [I ] CSL_h3aBufPutEmptyPrm


typedef struct {

  Uint8   h3aModId;
  CSL_BufInit *bufInit;

} CSL_H3aBufInitPrm;

typedef struct {

  Uint8   h3aModId;
  CSL_BufInfo *buf;
  Uint32  minBuf;
  Uint32  timeout;

} CSL_H3aBufGetFullPrm;

typedef struct {

  Uint8   h3aModId;
  CSL_BufInfo *buf;

} CSL_H3aBufPutEmptyPrm;

typedef struct {

  Uint8   h3aModId;
  Bool32    enable;

} CSL_H3aBufSwitchEnablePrm;

CSL_Status CSL_h3aInit(CSL_H3aHandle hndl);
CSL_Status CSL_h3aExit(CSL_H3aHandle hndl);
CSL_Status CSL_h3aHwControl(CSL_H3aHandle hndl, Uint32 cmd, void *prm);

#endif /* _CSL_H3A_IOCTL_ */
