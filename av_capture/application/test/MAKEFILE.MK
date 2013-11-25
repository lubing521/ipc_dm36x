# (c) Texas Instruments 

	
libs:
	make -fMAKEFILE.MK -C./moduletest MODULE=moduletest $(MAKE_TARGET)
	make -fMAKEFILE.MK -C./regrw MODULE=regrw $(MAKE_TARGET)
	make -fMAKEFILE.MK -C./i2crw MODULE=i2crw $(MAKE_TARGET)
exe:
	make -fMAKEFILE.MK -C./moduletest MODULE=moduletest exe
	make -fMAKEFILE.MK -C./regrw MODULE=regrw exe
	make -fMAKEFILE.MK -C./i2crw MODULE=i2crw exe			
	
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
	
