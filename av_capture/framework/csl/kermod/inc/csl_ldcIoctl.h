
#ifndef _CSL_LDC_IOCTL_
#define _CSL_LDC_IOCTL_

#include <csl_ldc.h>

#define CSL_LDC_CMD_HW_SETUP      (0x00)  ///< prm: [I ] CSL_LdcHwSetup *
#define CSL_LDC_CMD_HW_RESET      (0x01)  ///< prm: [  ] NONE
#define CSL_LDC_CMD_FRAME_CONFIG  (0x02)  ///< prm: [I ] CSL_LdcFrameConfig *
#define CSL_LDC_CMD_ENABLE        (0x03)  ///< prm: [I ] Bool32
#define CSL_LDC_CMD_IS_BUSY       (0x04)  ///< prm: [ O] Bool32 *
#define CSL_LDC_CMD_WRITE_TABLE   (0x05)  ///< prm: [I ] Uint16 *
#define CSL_LDC_CMD_READ_TABLE    (0x06)  ///< prm: [ O] Uint16 *
#define CSL_LDC_CMD_INT_ENABLE    (0x07)  ///< prm: [I ] Bool32
#define CSL_LDC_CMD_INT_CLEAR     (0x08)  ///< prm: [  ] NONE
#define CSL_LDC_CMD_INT_WAIT      (0x09)  ///< prm: [  ] NONE


CSL_Status CSL_ldcInit(CSL_LdcHandle hndl);
CSL_Status CSL_ldcExit(CSL_LdcHandle hndl);
CSL_Status CSL_ldcHwControl(CSL_LdcHandle hndl, Uint32 cmd, void *prm);

#endif /* _CSL_LDC_IOCTL_ */
