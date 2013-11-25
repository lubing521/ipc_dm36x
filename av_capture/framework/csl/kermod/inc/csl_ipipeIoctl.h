
#ifndef _CSL_IPIPE_IOCTL_
#define _CSL_IPIPE_IOCTL_

#include <csl_ipipe.h>

#define CSL_IPIPE_CMD_HW_SETUP                      (0x00)  ///< prm: [I ] CSL_IpipeHwSetup *
#define CSL_IPIPE_CMD_HW_RESET                      (0x01)  ///< prm: [  ] NONE
#define CSL_IPIPE_CMD_COL_PAT_CONFIG                (0x02)  ///< prm: [I ] CSL_IpipeColPatConfig *
#define CSL_IPIPE_CMD_CLK_COFNIG                    (0x03)  ///< prm: [I ] CSL_IpipeClkConfig *
#define CSL_IPIPE_CMD_IN_FRAME_CONFIG               (0x04)  ///< prm: [I ] CSL_IpipeInFrameConfig *
#define CSL_IPIPE_CMD_LSC_CONFIG                    (0x05)  ///< prm: [I ] CSL_IpipeLscConfig *
#define CSL_IPIPE_CMD_DPC_CONFIG                    (0x06)  ///< prm: [I ] CSL_IpipeDpcConfig *
#define CSL_IPIPE_CMD_NF1_CONFIG                    (0x07)  ///< prm: [I ] CSL_IpipeNfConfig *
#define CSL_IPIPE_CMD_NF2_CONFIG                    (0x08)  ///< prm: [I ] CSL_IpipeNfConfig *
#define CSL_IPIPE_CMD_GIC_CONFIG                    (0x09)  ///< prm: [I ] CSL_IpipeGicConfig *
#define CSL_IPIPE_CMD_WB_CONFIG                     (0x0A)  ///< prm: [I ] CSL_IpipeWbConfig *
#define CSL_IPIPE_CMD_CFA_CONFIG                    (0x0B)  ///< prm: [I ] CSL_IpipeCfaConfig *
#define CSL_IPIPE_CMD_RGB2RGB1_CONFIG               (0x0C)  ///< prm: [I ] CSL_IpipeRgb2RgbConfig *
#define CSL_IPIPE_CMD_GAMMA_CONFIG                  (0x0D)  ///< prm: [I ] CSL_IpipeGammaConfig *
#define CSL_IPIPE_CMD_RGB2RGB2_CONFIG               (0x0E)  ///< prm: [I ] CSL_IpipeRgb2RgbConfig *
#define CSL_IPIPE_CMD_3DLUT_CONFIG                  (0x0F)  ///< prm: [I ] CSL_Ipipe3dLutConfig *
#define CSL_IPIPE_CMD_RGB2YUV_CONFIG                (0x10)  ///< prm: [I ] CSL_IpipeRgb2YuvConfig *
#define CSL_IPIPE_CMD_CNT_BRT_CONFIG                (0x11)  ///< prm: [I ] CSL_IpipeCntBrtConfig *
#define CSL_IPIPE_CMD_GBCE_CONFIG                   (0x12)  ///< prm: [I ] CSL_IpipeGbceConfig *
#define CSL_IPIPE_CMD_EDGE_ENHANCE_CONFIG           (0x13)  ///< prm: [I ] CSL_IpipeEdgeEnhanceConfig *
#define CSL_IPIPE_CMD_CAR_CONFIG                    (0x14)  ///< prm: [I ] CSL_IpipeChromaArtifactReduceConfig *
#define CSL_IPIPE_CMD_CGS_CONFIG                    (0x15)  ///< prm: [I ] CSL_IpipeChromaGainSupressConfig *
#define CSL_IPIPE_CMD_BOXCAR_CONFIG                 (0x16)  ///< prm: [I ] CSL_IpipeBoxcarConfig *
#define CSL_IPIPE_CMD_HISTOGRAM_CONFIG              (0x17)  ///< prm: [I ] CSL_IpipeHistogramConfig *
#define CSL_IPIPE_CMD_BSC_CONFIG                    (0x18)  ///< prm: [I ] CSL_IpipeBscConfig *
#define CSL_IPIPE_CMD_ENABLE                        (0x19)  ///< prm: [I ] Bool32
#define CSL_IPIPE_CMD_ONE_SHOT_ENABLE               (0x1A)  ///< prm: [I ] Bool32
#define CSL_IPIPE_CMD_SET_DATA_PATH                 (0x1B)  ///< prm: [I ] Uint8
#define CSL_IPIPE_CMD_BOXCAR_ENABLE                 (0x1C)  ///< prm: [I ] Bool32
#define CSL_IPIPE_CMD_BSC_ENABLE                    (0x1D)  ///< prm: [I ] Bool32
#define CSL_IPIPE_CMD_BSC_READ_RESULTS              (0x1E)  ///< prm: [ O] CSL_IpipeTablePrm *
#define CSL_IPIPE_CMD_HISTOGRAM_ENABLE              (0x1F)  ///< prm: [I ] Bool32
#define CSL_IPIPE_CMD_HISTOGRAM_REGION_ENABLE       (0x20)  ///< prm: [I ] Bool32 enable[4]
#define CSL_IPIPE_CMD_HISTOGRAM_COLOR_ENABLE        (0x21)  ///< prm: [I ] Bool32 enable[4]
#define CSL_IPIPE_CMD_HISTOGRAM_TABLE_SELECT        (0x22)  ///< prm: [I ] Uint32
#define CSL_IPIPE_CMD_HISTOGRAM_READ_RESULTS        (0x23)  ///< prm: [I ] Uint32 *
#if 1 //Gang: Histogram read Idle Y
#define CSL_IPIPE_CMD_HISTOGRAM_READ_IDLE_Y         (0x24)  ///< prm: [I ] Uint32 *
#endif
#define CSL_IPIPE_CMD_BSC_BUF_INIT                  (0x26)  ///< prm: [I ] CSL_BufInit *
#define CSL_IPIPE_CMD_BSC_BUF_SWITCH_ENABLE         (0x27)  ///< prm: [I ] Bool32 enable
#define CSL_IPIPE_CMD_BSC_BUF_GET_FULL              (0x28)  ///< prm: [I ] CSL_IpipeBscBufGetFullPrm *
#define CSL_IPIPE_CMD_BSC_BUF_PUT_EMPTY             (0x29)  ///< prm: [I ] CSL_BufInfo *
#define CSL_IPIPE_CMD_INT_ENABLE                    (0x2A)  ///< prm: [I ] CSL_IpipeIntEnablePrm *
#define CSL_IPIPE_CMD_INT_CLEAR                     (0x2B)  ///< prm: [I ] Uint8
#define CSL_IPIPE_CMD_INT_WAIT                      (0x2C)  ///< prm: [I ] Uint8
#define CSL_IPIPE_CMD_BOXCAR_SET_OUT_ADDR           (0x2D)  ///< prm: [I ] Uint8 *
#define CSL_IPIPE_CMD_BOXCAR_BUF_INIT               (0x2E)  ///< prm: [I ] CSL_BufInit *
#define CSL_IPIPE_CMD_BOXCAR_BUF_SWITCH_ENABLE      (0x2F)  ///< prm: [I ] Bool32 enable
#define CSL_IPIPE_CMD_BOXCAR_BUF_GET_FULL           (0x30)  ///< prm: [I ] CSL_IpipeBufGetFullPrm *
#define CSL_IPIPE_CMD_BOXCAR_BUF_PUT_EMPTY          (0x31)  ///< prm: [I ] CSL_BufInfo *

typedef struct {

  CSL_BufInfo *buf;
  Uint32  minBuf;
  Uint32  timeout;

} CSL_IpipeBufGetFullPrm;

typedef struct {

  Uint8   ipipeIntType;
  Bool32    enable;

} CSL_IpipeIntEnablePrm;

CSL_Status CSL_ipipeInit(CSL_IpipeHandle hndl);
CSL_Status CSL_ipipeExit(CSL_IpipeHandle hndl);
CSL_Status CSL_ipipeHwControl(CSL_IpipeHandle hndl, Uint32 cmd, void *prm);

#endif /* _CSL_IPIPE_IOCTL_ */
