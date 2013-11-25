ROOTDIR = ../../..
include $(ROOTDIR)/Rules.make

CROSS=$(MVTOOL_DIR)/bin/arm_v5t_le-
CC := $(CROSS)gcc
STRIP := $(CROSS)strip

TARGET_EXE = Appro_avi_save
TARGET_DIR = .
LIB_PATH := ./lib
LDFLAGS := -L$(LIB_PATH) -L$(APP_LIB_DIR) $(LDFLAGS) 
EXTRALIBS := -lavdevice$(BUILDSUF) -lavformat$(BUILDSUF) -lavcodec$(BUILDSUF) -lavutil$(BUILDSUF) $(APP_LIB_DIR)/Appro_interface.a  $(APP_LIB_DIR)/msg_util.a $(CMEM_INSTALL_DIR)/packages/ti/sdo/linuxutils/cmem/lib/cmem.a470MV -lm -lx264 -lpthread  $(EXTRALIBS)
OPTFLAGS = -fomit-frame-pointer -Wdeclaration-after-statement -Wall -Wno-switch -Wdisabled-optimization -Wpointer-arith -Wredundant-decls -Wcast-qual -Wwrite-strings -O3 -fno-math-errno 
INCLUDE_PATH := ./include
CFLAGS = $(OPTFLAGS) -I$(INCLUDE_PATH) -I$(PUBLIC_INCLUDE_DIR) -I$(INCLUDE_PATH)/libavutil \
       -I$(INCLUDE_PATH)/libavcodec -I$(INCLUDE_PATH)/libavformat -I$(INCLUDE_PATH)/libswscale \
       -I$(INCLUDE_PATH)/libavdevice -I$(INCLUDE_PATH)/libavfilter -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_ISOC9X_SOURCE 

all: $(TARGET_EXE)
	
	
clean:
	rm -f $(TARGET_DIR)/$(TARGET_EXE)
	rm -f $(TARGET_DIR)/$(TARGET_EXE).o
	
install: 
	install -d $(EXEC_DIR)
	install $(TARGET_DIR)/$(TARGET_EXE) $(EXEC_DIR)

Appro_avi_save: Appro_avi_save.o 
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $^ $(TARGET_EXE).c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(EXTRALIBS)
	$(STRIP) $@
