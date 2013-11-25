
include $(BASE_DIR)/COMMON_HEADER.MK

include $(DVSDK_BASE_DIR)/Rules.make

INCLUDE+=$(CSL_INC)
INCLUDE+=$(DRV_INC)
INCLUDE+=$(IMAGE_TUNE_INC)
INCLUDE+=$(ALG_INC)
INCLUDE+=-I$(XDAIS_INSTALL_DIR)/packages
INCLUDE+=-I$(XDC_INSTALL_DIR)/packages
INCLUDE+=-I./include
INCLUDE+=-I../../priv/aewb

# Where to output configuration files
XDC_CFG = ../alg_config

# Output compiler options
XDC_CFLAGS = $(XDC_CFG)/compiler.opt

CC_OPTS+=-DDM355 -Dti_sdo_ce_osal_Memory_USEDEPRECATEDAPIS=1 $(shell cat $(XDC_CFLAGS))
	
include $(BASE_DIR)/COMMON_FOOTER.MK


