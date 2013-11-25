
#ifndef _CSL_IPIPEIF_IOCTL_
#define _CSL_IPIPEIF_IOCTL_

#include <csl_ipipeif.h>

#define CSL_IPIPEIF_CMD_HW_SETUP                  (0x00)  ///< prm: [I ] CSL_IpipeifHwSetup *
#define CSL_IPIPEIF_CMD_HW_RESET                  (0x01)  ///< prm: [  ] NONE
#define CSL_IPIPEIF_CMD_SET_SDRAM_IN_CONFIG       (0x02)  ///< prm: [I ] CSL_IpipeifSdramInConfig *
#define CSL_IPIPEIF_CMD_SET_SDRAM_IN_FRAME_CONFIG (0x03)  ///< prm: [I ] CSL_IpipeifSdramInFrameConfig *
#define CSL_IPIPEIF_CMD_SET_IN_SOURCE_2_CONFIG    (0x04)  ///< prm: [I ] CSL_IpipeifInSource2Config *
#define CSL_IPIPEIF_CMD_SET_VPIF_ISIF_IN_CONFIG   (0x05)  ///< prm: [I ] CSL_IpipeifVpifIsifInConfig *
#define CSL_IPIPEIF_CMD_SET_DARK_FRAME_CONFIG     (0x06)  ///< prm: [I ] CSL_IpipeifDarkFrameConfig *
#define CSL_IPIPEIF_CMD_SET_H3A_CONFIG            (0x07)  ///< prm: [I ] CSL_IpipeifH3aConfig *
#define CSL_IPIPEIF_CMD_SET_CLK_CONFIG            (0x08)  ///< prm: [I ] CSL_IpipeifClkConfig *
#define CSL_IPIPEIF_CMD_SET_DPCM_CONFIG           (0x09)  ///< prm: [I ] CSL_IpipeifDpcmConfig *
#define CSL_IPIPEIF_CMD_SET_INPUT_SOURCE_1        (0x0A)  ///< prm: [I ] Uint8
#define CSL_IPIPEIF_CMD_SET_SDRAM_IN_ADDR         (0x0B)  ///< prm: [I ] Uint8 *
#define CSL_IPIPEIF_CMD_SDRAM_IN_ENABLE           (0x0C)  ///< prm: [I ] Bool32
#define CSL_IPIPEIF_CMD_SDRAM_IN_ONE_SHOT_ENABLE  (0x0D)  ///< prm: [I ] Bool32
#define CSL_IPIPEIF_CMD_UNDERFLOW_DETECT_CLEAR    (0x0E)  ///< prm: [  ] NONE
#define CSL_IPIPEIF_CMD_IS_UNDERFLOW_DETECT       (0x0F)  ///< prm: [ O] Bool32 *


CSL_Status CSL_ipipeifInit(CSL_IpipeifHandle hndl);
CSL_Status CSL_ipipeifExit(CSL_IpipeifHandle hndl);
CSL_Status CSL_ipipeifHwControl(CSL_IpipeifHandle hndl, Uint32 cmd, void *prm);


#endif /* _CSL_IPIPEIF_IOCTL_ */
