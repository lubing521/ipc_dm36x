# (c) Texas Instruments 

USERMOD = $(shell pwd)/usermod/src
KERMOD = $(shell pwd)/kermod/build

libs:
	make -C$(KERMOD)  $(MAKE_TARGET)
	make -fMAKEFILE.MK -C$(USERMOD) MODULE=csl $(MAKE_TARGET)

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
	
