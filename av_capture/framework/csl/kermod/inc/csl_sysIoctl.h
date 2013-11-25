
#ifndef _CSL_SYS_IOCTL_
#define _CSL_SYS_IOCTL_

#include <csl_soc.h>
#include <linux/ioctl.h>

#define CSL_IOCTL_CMD_MAKE(cmd)     ( _IO( CSL_DRV_MAGICNUM, cmd) )
#define CSL_IOCTL_CMD_GET(cmd)      ( _IOC_NR(cmd) )
#define CSL_IOCTL_CMD_IS_VALID(cmd) ( (_IOC_TYPE(cmd) == CSL_DRV_MAGICNUM ) ? 1 : 0)

#endif /* _CSL_LDC_IOCTL_ */
