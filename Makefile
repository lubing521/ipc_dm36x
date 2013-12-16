# (c) Texas Instruments 

include Rules.make
include $(BASE_DIR)/COMMON_HEADER.MK

exe:
	make depend
	make libs
	make -fMAKEFILE.MK -C$(BASE_DIR)/../application/test exe
#ifeq ($(SYSTEM), IPNC)		
	make -fMAKEFILE.MK -C$(BASE_DIR)/../application/ipnc/av_server exe								
#endif
	make install
	@echo "------------------------------------------------------------------------------------------------------------------"
	@echo "Please run < make lsp > if you have changed anything in csl/drv kernel modules and use the new built image!!!!"
	@echo
	@echo "Build Completed for $(HARDWARE) $(SYSTEM)"
	@echo "------------------------------------------------------------------------------------------------------------------"
	
libs:
	make -fMAKEFILE.MK -C$(BASE_DIR)/../framework $(MAKE_TARGET)
	make -fMAKEFILE.MK -C$(BASE_DIR)/../application/test $(MAKE_TARGET)
#ifeq ($(SYSTEM), IPNC)		
	make -fMAKEFILE.MK -C$(BASE_DIR)/../application/ipnc $(MAKE_TARGET)
#endif

all:
	make clean
	make exe

clean:
	make libs MAKE_TARGET=clean
	-rm -f $(EXE_BASE_DIR)/$(CONFIG)/*.out
	-rm -f $(TARGET_FS_DIR)/*.out
	
depend:
	-mkdir -p $(EXE_BASE_DIR)
	-mkdir -p $(TARGET_FS_DIR)	
	make libs MAKE_TARGET=depend  

install:
	make libs MAKE_TARGET=install  
	cp $(EXE_BASE_DIR)/*.sh $(TARGET_FS_DIR)	
	cp $(LINUXUTILS_INSTALL_DIR)/packages/ti/sdo/linuxutils/cmem/src/module/cmemk.ko $(TARGET_FS_DIR)
	cp $(LINUXUTILS_INSTALL_DIR)/packages/ti/sdo/linuxutils/edma/src/module/edmak.ko $(TARGET_FS_DIR)
	cp $(LINUXUTILS_INSTALL_DIR)/packages/ti/sdo/linuxutils/irq/src/module/irqk.ko $(TARGET_FS_DIR)
	cp $(DVSDK_BASE_DIR)/dm365mm/module/dm365mmap.ko $(TARGET_FS_DIR)
	cp $(EXE_BASE_DIR)/$(CONFIG)/av_server.out $(TARGET_FS_DIR)
	cp $(EXE_BASE_DIR)/$(CONFIG)/i2crw.out $(TARGET_FS_DIR)
	cp $(EXE_BASE_DIR)/$(CONFIG)/regrw.out $(TARGET_FS_DIR)
	cp $(KERNELDIR)/block/sbull/sbull.ko $(TARGET_FS_DIR)	
	cp $(KERNELDIR)/drivers/usb/gadget/g_file_storage.ko $(TARGET_FS_DIR)		
#	cp $(EXE_BASE_DIR)/$(CONFIG)/moduletest.out $(TARGET_FS_DIR)
	cp $(EXE_BASE_DIR)/$(CONFIG)/av_server.out  $(TARGET_FS_DIR)/../../sys_app/ipcam/	

dvsdkbuild:
	make -C$(DVSDK_BASE_DIR) $(DVSDK_TARGET)
	
dvsdk:
	make dvsdkbuild DVSDK_TARGET=
#	make dvsdkbuild DVSDK_TARGET=install

dvsdkclean:
	make dvsdkbuild DVSDK_TARGET=clean
	
dvsdkall:	
	make dvsdkclean
	make dvsdk
		
lspbuild:
	make -C$(KERNELDIR) ARCH=arm CROSS_COMPILE=$(MVTOOL_PREFIX) $(MAKE_TARGET)
	
lspcfg:
ifeq ($(SYSTEM), IPNC)
ifeq ($(HARDWARE), DM365)
	make lspbuild MAKE_TARGET=davinci_dm365_ipnc_defconfig
endif
ifeq ($(HARDWARE), DM368)
	make lspbuild MAKE_TARGET=davinci_dm368_ipnc_defconfig
endif
endif	
ifeq ($(SYSTEM), EVM)
	make lspbuild MAKE_TARGET=davinci_dm365_defconfig
endif	
ifeq ($(SYSTEM), DVR)
	make lspbuild MAKE_TARGET=davinci_dm365_dvr_defconfig
endif	
	make lspbuild MAKE_TARGET=checksetconfig
	
lsp:
	make lspcfg
	make lspbuild MAKE_TARGET=uImage
ifeq ($(SYSTEM), IPNC)	
	make lspbuild MAKE_TARGET=modules
	cp $(KERNELDIR)/arch/arm/boot/uImage $(TFTP_HOME)/uImage_ipnc_$(HARDWARE)
	-cp $(KERNELDIR)/arch/arm/boot/uImage /home/jiangjx/UbuntuShare/tftpboot/
	find $(KERNELDIR) -name "*.ko" -exec cp '{}' $(TARGET_FS_DIR)/modules \;
	rm $(TARGET_FS_DIR)/modules/ltt*.ko
endif
ifeq ($(SYSTEM), EVM)	
	make lspbuild MAKE_TARGET=modules
	cp $(KERNELDIR)/arch/arm/boot/uImage $(TFTP_HOME)/uImage_ipnc_$(HARDWARE)_EVM
	find $(KERNELDIR) -name "*.ko" -exec cp '{}' $(TARGET_FS_DIR)/modules \;
	rm $(TARGET_FS_DIR)/modules/ltt*.ko		
endif
ifeq ($(SYSTEM), DVR)	
	cp $(KERNELDIR)/arch/arm/boot/uImage $(TFTP_HOME)/uImage_$(HARDWARE)_DVR		
endif

lspall:
	make lspclean
	make lsp
	
lspclean:	
	make lspbuild MAKE_TARGET=distclean
	rm -f $(TARGET_FS_DIR)/modules/*.ko
	
nfsreset:
	sudo /usr/sbin/exportfs -av
	sudo /sbin/service nfs restart	

sysall:
	make dvsdkall
	make all
	make lspall
	make
	
sysclean:
	make lspclean	
	make dvsdkclean
	make clean

sys:
	make lsp
	make dvsdk
	make 	

cramfs:
	sudo mkfs.cramfs $(TARGET_FS)/ cramfsImage_ipnc_$(HARDWARE)
#	mv cramfsImage_ipnc_$(HARDWARE) $(TFTP_HOME)
	mv cramfsImage_ipnc_$(HARDWARE) /home/jiangjx/UbuntuShare/tftpboot/rootfs.cramfs
	
.PHONY : exe libs all clean depend install dvsdkbuild dvsdk dvsdkclean dvsdkall lspbuild lspcfg lsp lspall lspclean nfsreset sysall sysclean sys ipnc ipncbuild ipncclean ipncall ramdisk
