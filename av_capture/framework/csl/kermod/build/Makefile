
ifneq ($(KERNELRELEASE),)
# call from kernel build system

CFLAGS += -I$(M)/../inc -I$(M)/../../inc -DCSL_KERNEL_API -D_OS_LINUX_ 

ifeq ($(IMGS_ID), IMGS_TVP514X)
CFLAGS += -DYUV_MODE_INTERLACED 
endif

CCDC_OBJS  := ../src/ccdc/csl_ccdcCommon.o \
              ../src/ccdc/csl_ccdcOpenClose.o \
              ../src/ccdc/csl_ccdcHwReset.o \
              ../src/ccdc/csl_ccdcHwSetup.o \
              ../src/ccdc/csl_ccdcHwControl.o \
              ../src/ccdc/csl_ccdcInt.o \
              ../src/ccdc/csl_ccdcBuf.o


IPIPE_OBJS := ../src/ipipe/csl_ipipeCommon.o \
              ../src/ipipe/csl_ipipeOpenClose.o \
              ../src/ipipe/csl_ipipeHwReset.o \
              ../src/ipipe/csl_ipipeHwSetup.o \
              ../src/ipipe/csl_ipipeHwControl.o \
              ../src/ipipe/csl_ipipeInt.o \
              ../src/ipipe/csl_ipipeBuf.o

IPIPEIF_OBJS := ../src/ipipeif/csl_ipipeifOpenClose.o \
                ../src/ipipeif/csl_ipipeifHwControl.o

RSZ_OBJS := ../src/rsz/csl_rszCommon.o \
            ../src/rsz/csl_rszOpenClose.o \
            ../src/rsz/csl_rszHwControl.o \
            ../src/rsz/csl_rszInt.o \
            ../src/rsz/csl_rszBuf.o

LDC_OBJS := ../src/ldc/csl_ldcCommon.o \
            ../src/ldc/csl_ldcOpenClose.o \
            ../src/ldc/csl_ldcHwReset.o \
            ../src/ldc/csl_ldcHwSetup.o \
            ../src/ldc/csl_ldcHwControl.o \
            ../src/ldc/csl_ldcInt.o

H3A_OBJS := ../src/h3a/csl_h3aCommon.o \
            ../src/h3a/csl_h3aOpenClose.o \
            ../src/h3a/csl_h3aHwReset.o \
            ../src/h3a/csl_h3aHwSetup.o \
            ../src/h3a/csl_h3aHwControl.o \
            ../src/h3a/csl_h3aBuf.o

FACE_DETECT_OBJS := ../src/faceDetect/csl_faceDetectOpenClose.o \
                    ../src/faceDetect/csl_faceDetectHwSetup.o \
                    ../src/faceDetect/csl_faceDetectHwControl.o \
                    ../src/faceDetect/csl_faceDetectInt.o

VPSS_OBJS := ../src/vpss/csl_vpssCommon.o \
             ../src/vpss/csl_vpssOpenClose.o \
             ../src/vpss/csl_vpssHwControl.o \
             ../src/vpss/csl_vpssHwReset.o \
             ../src/vpss/csl_vpssHwSetup.o

GPIO_OBJS := ../src/gpio/csl_gpioCommon.o \
             ../src/gpio/csl_gpioOpenClose.o \
             ../src/gpio/csl_gpioHwControl.o 


SYSTEM_OBJS := ../src/system/csl_sysDrv.o \
               ../src/system/csl_buf.o \
               ../src/system/csl_dma.o \
               ../src/system/csl_sysLinuxMutex.o \
               ../src/system/csl_sysLinuxFlag.o \
               ../src/system/csl_sysLinuxInt.o \
               ../src/system/csl_sysLinuxDrv.o

csl-objs := $(CCDC_OBJS) \
            $(IPIPEIF_OBJS) \
            $(IPIPE_OBJS) \
            $(RSZ_OBJS) \
            $(H3A_OBJS) \
            $(LDC_OBJS) \
            $(FACE_DETECT_OBJS) \
            $(VPSS_OBJS) \
            $(GPIO_OBJS) \
            $(SYSTEM_OBJS)

obj-m	:= csl.o

else

PWD     := $(shell pwd)
SUBDIRS := build src/ccdc src/ipipe src/ipipeif src/rsz src/ldc src/h3a src/faceDetect src/vpss src/gpio src/system

KER_MODULE := csl_kernel

modules:
	@echo $(KER_MODULE): Building module
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=$(MVTOOL_PREFIX) modules

install:
	@echo $(KER_MODULE): Copying files to install directory
	-mkdir -p $(TARGET_FS_DIR)
	cp csl.ko $(TARGET_FS_DIR)
	cp csl_*.sh $(TARGET_FS_DIR)

clean:
	@echo $(KER_MODULE): Deleting temporary files
	for i in $(SUBDIRS) ; do cd ../$$i; rm -rf *.ko *.o *~ core .depend .*.cmd *.mod.c .tmp_versions; cd -; done
	-rm -f $(TARGET_FS_DIR)/csl.ko
	-rm -f $(TARGET_FS_DIR)/csl_*.sh

depend:

endif



