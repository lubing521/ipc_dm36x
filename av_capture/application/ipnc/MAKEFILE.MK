# (c) Texas Instruments 

	
libs:
	make -fMAKEFILE.MK -C./av_server $(MAKE_TARGET)								
	make -C$(IPNC_DIR) $(MAKE_TARGET)
	
exe:
					
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
	
