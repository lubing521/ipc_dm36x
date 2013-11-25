# (c) Texas Instruments 

	
libs:
	make -fMAKEFILE.MK -C./src/fonts MODULE=av_server $(MAKE_TARGET)								
	make -fMAKEFILE.MK -C./src/video MODULE=av_server $(MAKE_TARGET)								
	make -fMAKEFILE.MK -C./src/common MODULE=av_server $(MAKE_TARGET)									
	make -fMAKEFILE.MK -C./src/stream MODULE=av_server $(MAKE_TARGET)										
	make -fMAKEFILE.MK -C./src/audio MODULE=av_server $(MAKE_TARGET)										


exe:
	make -fMAKEFILE.MK -C./src/common MODULE=av_server exe			
				
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
	
