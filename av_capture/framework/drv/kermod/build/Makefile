
ifneq ($(KERNELRELEASE),)
# call from kernel build system

CFLAGS += -I$(M)/../inc -I$(M)/../priv -I$(M)/../../inc -DKERNEL_LEVEL -DDEBUG -D$(BOARD_ID) -O3
                                                          
ifeq ($(IMGS_ID), IMGS_TVP514X)
CFLAGS += -DYUV_MODE_INTERLACED 
endif

drv-objs := ../src/dev_drv.o ../src/dev_i2c.o ../src/dev_dma.o ../src/dev_dmaCopyFill.o ../src/dev_dmaDemux.o 
           
obj-m	:= drv.o 

else

PWD     := $(shell pwd)
SUBDIRS := build src 
KER_MODULE := drv_kernel

modules:
	@echo $(KER_MODULE): Building module
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=$(MVTOOL_PREFIX) modules
  
install:
	@echo $(KER_MODULE): Copying files to install directory
	-mkdir -p $(TARGET_FS_DIR)
	cp *.ko $(TARGET_FS_DIR)
	cp *.sh $(TARGET_FS_DIR)
	
clean:
	@echo $(KER_MODULE): Deleting temporary files
	for i in $(SUBDIRS) ; do cd ../$$i; rm -rf *.ko *.o *~ core .depend .*.cmd *.mod.c .tmp_versions; cd -; done  
	-rm -f $(TARGET_FS_DIR)/*.ko
	
depend:
	
endif



