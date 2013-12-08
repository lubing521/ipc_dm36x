# (c) Texas Instruments 

USERMOD = $(shell pwd)/usermod/src
KERMOD = $(shell pwd)/kermod/build

libs:
	make -C$(KERMOD)  $(MAKE_TARGET)
	make -fMAKEFILE.MK -C$(USERMOD)                    MODULE=drv $(MAKE_TARGET)
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_none          MODULE=drv $(MAKE_TARGET)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_OV9712_1MP    MODULE=drv $(MAKE_TARGET)
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_AR0130_1.2MP  MODULE=drv $(MAKE_TARGET)
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_MT9M034_1.2MP MODULE=drv $(MAKE_TARGET)	
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_MT9P031_5MP   MODULE=drv $(MAKE_TARGET)
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_AR0330_3.1MP  MODULE=drv $(MAKE_TARGET)
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_AR0331_3.1MP  MODULE=drv $(MAKE_TARGET)
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_AP0101        MODULE=drv $(MAKE_TARGET)
	make -fMAKEFILE.MK -C$(USERMOD)/imgs_IMX122_2_3MP  MODULE=drv $(MAKE_TARGET)
	
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

