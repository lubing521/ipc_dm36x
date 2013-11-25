# (c) Texas Instruments 

ifndef $(INCLUDES_MK)
INCLUDES_MK = 1

KERNEL_INC=-I$(KERNELDIR)/include
CMEM_INC=-I$(CMEM_INSTALL_DIR)/packages/ti/sdo/linuxutils/cmem/include
COMMON_INC=-I../inc -I../priv 

OSA_INC=-I$(BASE_DIR)/../framework/osa/inc 
CSL_INC=-I$(BASE_DIR)/../framework/csl/inc
DRV_INC=-I$(BASE_DIR)/../framework/drv/inc

ALG_INC=-I$(BASE_DIR)/../framework/alg/inc
ALG_INC+=-I$(BASE_DIR)/../framework/alg/priv/swosd
ALG_INC+=-I$(BASE_DIR)/../framework/alg/priv/swosd
ALG_INC+=-I$(BASE_DIR)/../framework/alg/priv/vstab
ALG_INC+=-I$(BASE_DIR)/../framework/alg/priv/aewb
ALG_INC+=-I$(BASE_DIR)/../framework/alg/priv/facetrack
ALG_INC+=-I$(BASE_DIR)/../framework/alg/priv/saldre
ALG_INC+=-I$(BASE_DIR)/../framework/alg/src/aewb_ap/include
#ALG_INC+=-I$(BASE_DIR)/../framework/alg/src/aewb_ti/include
ifeq ($(IMGS_ID), IMGS_AR0331_3.1MP_WDR)
ALG_INC+=-I$(BASE_DIR)/../framework/alg/src/aewb_ar0331wdr/include
else
ALG_INC+=-I$(BASE_DIR)/../framework/alg/src/aewb_ti/include
endif




IMAGE_TUNE_INC=-I$(BASE_DIR)/../framework/image_tune/inc
IMAGE_TUNE_INC+=-I$(BASE_DIR)/../framework/image_tune/priv

AV_SERVER_INC=-I$(BASE_DIR)/../application/ipnc/av_server/inc

STREAM_INC=-I$(BASE_DIR)/../application/ipnc/av_server/src/stream/inc

FRAMEWORK_INC=$(CSL_INC) $(DRV_INC) $(ALG_INC) $(IMAGE_TUNE_INC)

CMEM_LIB=$(CMEM_INSTALL_DIR)/packages/ti/sdo/linuxutils/cmem/lib/cmem.a470MV
DM365MM_LIB=$(DVSDK_BASE_DIR)/dm365mm/lib/libdm365mm.a

#VICP libraries
CODEC_LIBS=$(BASE_DIR)/../framework/alg/lib/alg_facetrack_ti.a
CODEC_LIBS+=$(BASE_DIR)/../framework/alg/lib/alg_saldre_ti.a

CODEC_LIBS+=$(BASE_DIR)/../framework/alg/src/alg_config/linker.cmd 

#Algorithm libraries
CODEC_LIBS+=$(BASE_DIR)/../framework/alg/lib/alg_vstab_ti.a
CODEC_LIBS+=$(BASE_DIR)/../framework/alg/lib/alg_osd_ti.a

# 2A libaries
CODEC_LIBS+=$(BASE_DIR)/../framework/alg/src/aewb_ti/lib/alg_awb_ti.a 
CODEC_LIBS+=$(BASE_DIR)/../framework/alg/src/aewb_ar0331wdr/lib/ae_ti_wdr.a 
CODEC_LIBS+=$(BASE_DIR)/../framework/alg/src/aewb_ti/lib/alg_flicker_detect_ti.a
CODEC_LIBS+=$(BASE_DIR)/../framework/alg/src/aewb_ap/lib/Appro_2a_lib.a
CODEC_LIBS+=$(BASE_DIR)/../framework/alg/src/aewb_ap/lib/Appro_aew_control.a

#Face Recognition Libraries
CODEC_LIBS+=$(BASE_DIR)/../framework/alg/lib/fr_ti/alg_fr_ti.a
CODEC_LIBS+=$(BASE_DIR)/../framework/alg/lib/fr_ti/libeOkaoFr.a
CODEC_LIBS+=$(BASE_DIR)/../framework/alg/lib/fr_ti/libeOkaoPt.a
CODEC_LIBS+=$(BASE_DIR)/../framework/alg/lib/fr_ti/libeOkaoCo.a


endif # ifndef $(INCLUDES_MK)
