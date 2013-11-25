
include $(BASE_DIR)/COMMON_HEADER.MK

include $(DVSDK_BASE_DIR)/Rules.make

INCLUDE+=$(CSL_INC)
INCLUDE+=$(DRV_INC)
INCLUDE+=$(IMAGE_TUNE_INC)
INCLUDE+=$(ALG_INC)
INCLUDE+=-I$(CODEC_INSTALL_DIR)/examples
INCLUDE+=-I$(CODEC_INSTALL_DIR)/packages
INCLUDE+=-I$(CE_INSTALL_DIR)/packages
INCLUDE+=-I$(XDAIS_INSTALL_DIR)/packages
INCLUDE+=-I$(FC_INSTALL_DIR)/packages
INCLUDE+=-I$(LINUXUTILS_INSTALL_DIR)/packages
INCLUDE+=-I$(EDMA3_LLD_INSTALL_DIR)/packages
INCLUDE+=-I$(XDC_INSTALL_DIR)/packages
INCLUDE+=-I../priv/aewb
INCLUDE+=-I../priv/vstab
INCLUDE+=-I../priv/swosd
INCLUDE+=-I../priv/gbce
INCLUDE+=-I../priv/facetrack
INCLUDE+=-I./aewb_ap/include

ifeq ($(IMGS_ID), IMGS_AR0331_3.1MP_WDR)
INCLUDE+=-I./aewb_ar0331wdr/include
else
INCLUDE+=-I./aewb_ti/include
endif

INCLUDE+=-I./aewb_ti/include

# Where to output configuration files
XDC_CFG = alg_config

# Output compiler options
XDC_CFLAGS = $(XDC_CFG)/compiler.opt

# Output linker file
XDC_LFILE = $(XDC_CFG)/linker.cmd

# Input configuration file
XDC_CFGFILE = alg_server.cfg

# Target tools
XDC_TARGET = gnu.targets.MVArm9

# Platform (board) to build for
XDC_PLATFORM = ti.platforms.evmDM365

# Package path for the XDC tools
XDC_PATH = $(CE_INSTALL_DIR)/examples;$(CODEC_INSTALL_DIR)/packages;$(CE_INSTALL_DIR)/packages;$(XDAIS_INSTALL_DIR)/packages;$(FC_INSTALL_DIR)/packages;$(EDMA3_LLD_INSTALL_DIR)/packages;$(LINUXUTILS_INSTALL_DIR)/packages
 
# The XDC configuration tool command line
CONFIGURO = XDCPATH="$(XDC_PATH)" $(XDC_INSTALL_DIR)/xs xdc.tools.configuro

CC_OPTS+=-DDM355 -Dti_sdo_ce_osal_Memory_USEDEPRECATEDAPIS=1 $(shell cat $(XDC_CFLAGS))

ifeq ($(IMGS_ID), IMGS_AR0331_3.1MP_WDR)
CC_OPTS+=-DAR0331_WDR
endif

xdc:
#	rm -rf $(XDC_CFG)
	make -fMAKEFILE.MK $(XDC_CFG)
	make -fMAKEFILE.MK lib	

$(XDC_CFG):	$(XDC_CFGFILE)
	@echo
	@echo ======== Building $(XDC_CFG) ========
	@echo Configuring application using $<
	@echo
	$(CONFIGURO) -c $(MVTOOL_DIR) -o $(XDC_CFG) -t $(XDC_TARGET) -p $(XDC_PLATFORM) $(XDC_CFGFILE)

	
include $(BASE_DIR)/COMMON_FOOTER.MK


