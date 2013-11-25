# (c) Texas Instruments 

USERMOD = $(shell pwd)/usermod/src
KERMOD = $(shell pwd)/kermod/build

libs:
	make -C$(KERMOD)  $(MAKE_TARGET)
	make -fMAKEFILE.MK -C$(USERMOD) MODULE=drv $(MAKE_TARGET)
ifeq ($(IMGS_ID), IMGS_MICRON_MT9P031_5MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_MT9P031_5MP MODULE=drv $(MAKE_TARGET)
endif
ifeq ($(IMGS_ID), IMGS_TVP514X)
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_TVP514X MODULE=drv $(MAKE_TARGET)
endif
ifeq ($(IMGS_ID), IMGS_MICRON_MT9D131_2MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_MT9D131_2MP MODULE=drv $(MAKE_TARGET)
endif
ifeq ($(IMGS_ID), IMGS_OV9712_1MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_OV9712_1MP MODULE=drv $(MAKE_TARGET)
endif
ifeq ($(IMGS_ID), IMGS_OV2715_2MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_OV2715_2MP MODULE=drv $(MAKE_TARGET)
endif
ifeq ($(IMGS_ID), IMGS_MICRON_MT9M034_1.2MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_MT9M034_1.2MP MODULE=drv $(MAKE_TARGET)
endif
ifeq ($(IMGS_ID), IMGS_OV10630_1MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_OV10630_1MP MODULE=drv $(MAKE_TARGET)
endif
ifeq ($(IMGS_ID), IMGS_SONY_IMX036_3MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_IMX036_3MP MODULE=drv $(MAKE_TARGET)
endif
ifeq ($(IMGS_ID), IMGS_SONY_IMX136_2_3MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_IMX136_2_3MP MODULE=drv $(MAKE_TARGET)
endif
ifeq ($(IMGS_ID), IMGS_SONY_IMX122_2_3MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_IMX122_2_3MP MODULE=drv $(MAKE_TARGET)
endif

ifeq ($(IMGS_ID), IMGS_SONY_IMX035_1_3MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_IMX035_1_3MP MODULE=drv $(MAKE_TARGET)
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_IMX035_1_3MP/lib MODULE=drv $(MAKE_TARGET)
endif

ifeq ($(IMGS_ID), IMGS_AR0130_1.2MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_AR0130_1.2MP MODULE=drv $(MAKE_TARGET)
endif

ifeq ($(IMGS_ID), IMGS_AR0331_3.1MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_AR0331_3.1MP MODULE=drv $(MAKE_TARGET)
endif

ifeq ($(IMGS_ID), IMGS_AR0331_3.1MP_WDR)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_AR0331_3.1MP MODULE=drv $(MAKE_TARGET)
endif
ifeq ($(IMGS_ID), IMGS_PANASONIC_MN34041_2.1MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_MN34041_2.1MP MODULE=drv $(MAKE_TARGET)
endif
ifeq ($(IMGS_ID), IMGS_AP0100_1.2MP)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_AP0100_1.2MP MODULE=drv $(MAKE_TARGET)
endif

ifeq ($(IMGS_ID), IMGS_NONE)
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_none MODULE=drv $(MAKE_TARGET)			
endif

all:
	make -fMAKEFILE.MK depend
	make -fMAKEFILE.MK clean
	make -fMAKEFILE.MK libs
	
clean:
	make -fMAKEFILE.MK libs MAKE_TARGET=clean
	
depend:
	make -fMAKEFILE.MK libs MAKE_TARGET=depend  

install:
	make -fMAKEFILE.MK libs MAKE_TARGET=install  
	
.PHONY : install clean depend all libs 
	
	
