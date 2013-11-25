
#ifndef _CSL_RSZ_IOCTL_
#define _CSL_RSZ_IOCTL_

#include <csl_rsz.h>

#define CSL_RSZ_CMD_HW_SETUP                     (0x00) ///< prm: [I ] CSL_RszHwSetup *
#define CSL_RSZ_CMD_HW_RESET                     (0x01) ///< prm: [  ] NONE
#define CSL_RSZ_CMD_SET_IN_CONFIG                (0x02) ///< prm: [I ] CSL_RszInConfig *
#define CSL_RSZ_CMD_SET_IN_FRAME_CONFIG          (0x03) ///< prm: [I ] CSL_RszInFrameConfig *
#define CSL_RSZ_CMD_GET_IN_FRAME_CONFIG          (0x04) ///< prm: [ O] CSL_RszInFrameConfig *
#define CSL_RSZ_CMD_SET_CLK_CONFIG               (0x05) ///< prm: [I ] CSL_RszClkConfig *
#define CSL_RSZ_CMD_SET_MINMAX_CONFIG            (0x06) ///< prm: [I ] CSL_RszMinMaxConfig *
#define CSL_RSZ_CMD_SET_OUT_CONFIG               (0x07) ///< prm: [I ] CSL_RszSetOutConfigPrm *
#define CSL_RSZ_CMD_ENABLE                       (0x08) ///< prm: [I ] Bool32
#define CSL_RSZ_CMD_ONE_SHOT_ENABLE              (0x09) ///< prm: [I ] Bool32
#define CSL_RSZ_CMD_OUT_ENABLE                   (0x0A) ///< prm: [I ] CSL_RszOutEnablePrm *
#define CSL_RSZ_CMD_OUT_ONE_SHOT_ENABLE          (0x0B) ///< prm: [I ] CSL_RszOutEnablePrm *
#define CSL_RSZ_CMD_SET_OUT_ADDR                 (0x0C) ///< prm: [I ] CSL_RszSetOutAddrPrm *
#define CSL_RSZ_CMD_GET_STATUS                   (0x0D) ///< prm: [IO] CSL_RszGetStatusPrm *
#define CSL_RSZ_CMD_SET_OUT_FRAME_CONFIG         (0x0E) ///< prm: [I ] CSL_RszSetOutFrameConfigPrm *
#define CSL_RSZ_CMD_GET_OUT_FRAME_CONFIG         (0x0F) ///< prm: [I ] CSL_RszGetOutFrameConfigPrm *
#define CSL_RSZ_CMD_SET_DZOOM_CONFIG             (0x10) ///< prm: [I ] CSL_RszDzoomConfig *
#define CSL_RSZ_CMD_BUF_INIT                     (0x11) ///< prm: [I ] CSL_RszBufInitPrm *
#define CSL_RSZ_CMD_BUF_SWITCH_ENABLE            (0x12) ///< prm: [I ] CSL_RszBufSwitchEnablePrm *
#define CSL_RSZ_CMD_BUF_GET_FULL                 (0x13) ///< prm: [I ] CSL_RszBufGetFullPrm *
#define CSL_RSZ_CMD_BUF_PUT_EMPTY                (0x14) ///< prm: [I ] CSL_RszBufPutEmptyPrm *
#define CSL_RSZ_CMD_INT_ENABLE                   (0x15) ///< prm: [I ] Bool32
#define CSL_RSZ_CMD_INT_CLEAR                    (0x16) ///< prm: [  ] NONE
#define CSL_RSZ_CMD_INT_WAIT                     (0x17) ///< prm: [  ] NONE
#define CSL_RSZ_CMD_SET_FLIP                     (0x18) ///< prm: [I ] CSL_RszSetFlipPrm *
#define CSL_RSZ_CMD_SET_RT_PRMS                  (0x19) ///< prm: [I ] CSL_RszSetRtPrm *

typedef struct {

  Uint8   rszMod;
  CSL_RszOutConfig *data;

} CSL_RszSetOutConfigPrm;

typedef struct {

  Uint8   rszMod;
  Bool32    enable;

} CSL_RszOutEnablePrm;

typedef struct {

  Uint8   rszMod;
  Uint8  *addr;
  Uint8  *yuv420CoutAddr;

} CSL_RszSetOutAddrPrm;

typedef struct {

  Uint8   rszMod;
  CSL_RszStatus *status;

} CSL_RszGetStatusPrm;

typedef struct {

  Uint8   rszMod;
  CSL_RszInFrameConfig *inFrameConfig;
  CSL_RszOutFrameConfig *outConfig;

} CSL_RszSetOutFrameConfigPrm;

typedef struct {

  Uint8   rszMod;
  CSL_RszOutFrameConfig *outConfig;

} CSL_RszGetOutFrameConfigPrm;

typedef struct {

  Uint8   rszMod;
  CSL_RszRuntimeConfig *rtConfig;

} CSL_RszSetRtPrm;

typedef struct {

  Uint8   rszMod;
  CSL_BufInit *bufInit;
  CSL_RszBufConfig *bufConfig;
  
} CSL_RszBufInitPrm;

typedef struct {

  Uint8   rszMod;
  Bool32    flipH;
  Bool32    flipV;
  
} CSL_RszSetFlipPrm;

typedef struct {

  Uint8   rszMod;
  Bool32    enable;

} CSL_RszBufSwitchEnablePrm;

typedef struct {

  Uint8   rszMod;
  CSL_BufInfo *buf;
  Uint32  minBuf;
  Uint32  timeout;

} CSL_RszBufGetFullPrm;

typedef struct {

  Uint8   rszMod;
  CSL_BufInfo *buf;

} CSL_RszBufPutEmptyPrm;

CSL_Status CSL_rszInit(CSL_RszHandle hndl);
CSL_Status CSL_rszExit(CSL_RszHandle hndl);
CSL_Status CSL_rszHwControl(CSL_RszHandle hndl, Uint32 cmd, void *prm);

#endif /* _CSL_RSZ_IOCTL_ */
