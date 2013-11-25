
#ifndef _CSL_CCDC_IOCTL_
#define _CSL_CCDC_IOCTL_

#include <csl_ccdc.h>

#define CSL_CCDC_CMD_HW_SETUP                     (0x00)  ///< prm: [I ] CSL_CcdcHwSetup *
#define CSL_CCDC_CMD_HW_RESET                     (0x01)  ///< prm: [  ] NONE
#define CSL_CCDC_CMD_ENABLE                       (0x02)  ///< prm: [I ] Bool32
#define CSL_CCDC_CMD_SDRAM_OUT_ENABLE             (0x03)  ///< prm: [I ] Bool32
#define CSL_CCDC_CMD_SET_DATA_IN_CONFIG           (0x04)  ///< prm: [I ] CSL_CcdcInDataConfig *
#define CSL_CCDC_CMD_SET_SYNC_CONFIG              (0x05)  ///< prm: [I ] CSL_CcdcSyncConfig *
#define CSL_CCDC_CMD_SET_MISC_CONFIG              (0x06)  ///< prm: [I ] CSL_CcdcMiscConfig *
#define CSL_CCDC_CMD_SET_COL_PAT_CONFIG           (0x07)  ///< prm: [I ] CSL_CcdcColPatConfig *
#define CSL_CCDC_CMD_SET_LINEARIZATION_CONFIG     (0x08)  ///< prm: [I ] CSL_CcdcLinerizationConfig *
#define CSL_CCDC_CMD_SET_FMT_CSC_IN_FRAME_CONFIG  (0x09)  ///< prm: [I ] CSL_CcdcFmtCscInFrameConfig *
#define CSL_CCDC_CMD_SET_FMT_CONFIG               (0x0A)  ///< prm: [I ] CSL_CcdcFmtConfig *
#define CSL_CCDC_CMD_SET_CSC_CONFIG               (0x0B)  ///< prm: [I ] CSL_CcdcCscConfig *
#define CSL_CCDC_CMD_SET_CLAMP_CONFIG             (0x0C)  ///< prm: [I ] CSL_CcdcClampConfig *
#define CSL_CCDC_CMD_SET_DFC_LSC_OFFSET_CONFIG    (0x0D)  ///< prm: [I ] CSL_CcdcGainOffsetConfig *
#define CSL_CCDC_CMD_SET_DFC_CONFIG               (0x0E)  ///< prm: [I ] CSL_CcdcDfcConfig *
#define CSL_CCDC_CMD_SET_LSC_CONFIG               (0x0F)  ///< prm: [I ] CSL_CcdcLscConfig *
#define CSL_CCDC_CMD_GET_LSC_STATUS               (0x10)  ///< prm: [O ] CSL_CcdcGetLscStatusPrm *
#define CSL_CCDC_CMD_SET_GAIN_OFFSET_CONFIG       (0x11)  ///< prm: [I ] CSL_CcdcGainOffsetConfig *
#define CSL_CCDC_CMD_SET_SDRAM_OUT_SIZE_CONFIG    (0x12)  ///< prm: [I ] CSL_CcdcSdramOutSizeConfig *
#define CSL_CCDC_CMD_SET_SDRAM_OUT_CONFIG         (0x13)  ///< prm: [I ] CSL_CcdcSdramOutConfig *
#define CSL_CCDC_CMD_GET_SDRAM_OUT_SIZE_CONFIG    (0x14)  ///< prm: [O ] CSL_CcdcSdramOutSizeConfig *
#define CSL_CCDC_CMD_SET_DPCM_CONFIG              (0x15)  ///< prm: [I ] CSL_CcdcDpcmConfig
#define CSL_CCDC_CMD_SET_REC656_CONFIG            (0x16)  ///< prm: [I ] CSL_CcdcRec656Config
#define CSL_CCDC_CMD_SET_FLASH_CONFIG             (0x17)  ///< prm: [I ] CSL_CcdcFlashConfig
#define CSL_CCDC_CMD_SET_VD_INT_CONFIG            (0x18)  ///< prm: [I ] CSL_CcdcVdIntConfig
#define CSL_CCDC_CMD_SET_SDRAM_OUT_ADDR           (0x19)  ///< prm: [I ] Uint8 *
#define CSL_CCDC_CMD_WRITE_DFC_TABLE              (0x1A)  ///< prm: [I ] CSL_CcdcVdfcTable *
#define CSL_CCDC_CMD_READ_DFC_TABLE               (0x1B)  ///< prm: [O ] CSL_CcdcVdfcTable *
#define CSL_CCDC_CMD_BUF_INIT                     (0x1C)  ///< prm: [I ] CSL_BufInit *
#define CSL_CCDC_CMD_BUF_SWITCH_ENABLE            (0x1D)  ///< prm: [I ] Bool32
#define CSL_CCDC_CMD_BUF_GET_FULL                 (0x1E)  ///< prm: [I ] CSL_CcdcBufGetFullPrm
#define CSL_CCDC_CMD_BUF_PUT_EMPTY                (0x1F)  ///< prm: [I ] CSL_BufInfo
#define CSL_CCDC_CMD_INT_ENABLE                   (0x20)  ///< prm: [I ] CSL_CcdcIntEnablePrm *
#define CSL_CCDC_CMD_INT_CLEAR                    (0x21)  ///< prm: [I ] Uint8
#define CSL_CCDC_CMD_INT_WAIT                     (0x22)  ///< prm: [I ] CSL_CcdcIntWaitPrm *
#define CSL_CCDC_CMD_LSC_BUF_INIT                 (0x23)  ///< prm: [I ] CSL_BufInit *
#define CSL_CCDC_CMD_LSC_BUF_SWITCH_ENABLE        (0x24)  ///< prm: [I ] Bool32
#define CSL_CCDC_CMD_LSC_BUF_GET_EMPTY            (0x25)  ///< prm: [I ] CSL_CcdcBufGetFullPrm
#define CSL_CCDC_CMD_LSC_BUF_PUT_FULL             (0x26)  ///< prm: [I ] CSL_BufInfo
#define CSL_CCDC_CMD_LSC_SET_GAIN_TABLE_ADDR      (0x27)  ///< prm: [I ] Uint8 *
#define CSL_CCDC_CMD_LSC_SET_OFFSET_TABLE_ADDR    (0x28)  ///< prm: [I ] Uint8 *
#define CSL_CCDC_CMD_LSC_ENABLE                   (0x29)  ///< prm: [I ] Bool32                 
#define CSL_CCDC_CMD_LSC_BUF_SET_GAIN_TABLE_SIZE  (0x30)  ///< prm: [I ] Uint32                 

typedef struct {

  Uint8   intType;
  Uint32  numIntWait;

} CSL_CcdcIntWaitPrm;

typedef struct {

  Uint8   intType;
  Bool32    enable;

} CSL_CcdcIntEnablePrm;

typedef struct {

  CSL_BufInfo *buf;
  Uint32  minBuf;
  Uint32  timeout;

} CSL_CcdcBufGetFullPrm;

typedef struct {

  Bool32    intSof;
  Bool32    prefetchDone;
  Bool32    prefetchError;
  Bool32    lscDone;

} CSL_CcdcGetLscStatusPrm;

CSL_Status CSL_ccdcInit(CSL_CcdcHandle hndl);
CSL_Status CSL_ccdcExit(CSL_CcdcHandle hndl);
CSL_Status CSL_ccdcHwControl(CSL_CcdcHandle hndl, Uint32 cmd, void *prm);
CSL_Status CSL_ccdcLscPrefetchErrReset(CSL_CcdcHandle hndl);

#endif /* _CSL_CCDC_IOCTL_ */
