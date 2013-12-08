include $(BASE_DIR)/COMMON_HEADER.MK
	
libs:
	make -fMAKEFILE.MK -C./csl $(MAKE_TARGET)
	make -fMAKEFILE.MK -C./drv $(MAKE_TARGET)	
	make -fMAKEFILE.MK -C./osa/src MODULE=osa $(MAKE_TARGET)
	make -fMAKEFILE.MK -C./alg/src MODULE=alg $(MAKE_TARGET)	
	make -fMAKEFILE.MK -C./image_tune/src MODULE=image_tune $(MAKE_TARGET)			
ifeq ($(AEWB_ID), AEWB_ENABLE)
	make -fMAKEFILE.MK -C./alg/src/aewb_ap MODULE=alg $(MAKE_TARGET)	
ifeq ($(IMGS_ID), IMGS_AR0331_3.1MP_WDR)
	make -fMAKEFILE.MK -C./alg/src/aewb_ar0331wdr MODULE=alg $(MAKE_TARGET)	
else
	make -fMAKEFILE.MK -C./alg/src/aewb_ti MODULE=alg $(MAKE_TARGET)	
endif
	make -fMAKEFILE.MK -C./alg/src/aewb_ti/imgs_OV9712_1MP    MODULE=alg $(MAKE_TARGET)
	make -fMAKEFILE.MK -C./alg/src/aewb_ti/imgs_AR0130_1.2MP  MODULE=alg $(MAKE_TARGET)
	make -fMAKEFILE.MK -C./alg/src/aewb_ti/imgs_MT9M034_1.2MP MODULE=alg $(MAKE_TARGET)
	make -fMAKEFILE.MK -C./alg/src/aewb_ti/imgs_MT9P031_5MP   MODULE=alg $(MAKE_TARGET)
	make -fMAKEFILE.MK -C./alg/src/aewb_ti/imgs_AR0330_3.1MP  MODULE=alg $(MAKE_TARGET)
	make -fMAKEFILE.MK -C./alg/src/aewb_ti/imgs_AR0331_3.1MP  MODULE=alg $(MAKE_TARGET)
	make -fMAKEFILE.MK -C./alg/src/aewb_ti/imgs_IMX122_2_3MP  MODULE=alg $(MAKE_TARGET)
else	
	make -fMAKEFILE.MK -C./alg/src/aewb_none MODULE=alg $(MAKE_TARGET)
endif		
		
all:
	make -fMAKEFILE.MK depend
	make -fMAKEFILE.MK clean
	make -fMAKEFILE.MK libs
	
clean:
	make -fMAKEFILE.MK libs MAKE_TARGET=clean
	rm -R -f alg/src/alg_config
	
depend:
	make -fMAKEFILE.MK libs MAKE_TARGET=depend  

install:
	make -fMAKEFILE.MK libs MAKE_TARGET=install  
	
.PHONY : install clean depend all libs 
	
