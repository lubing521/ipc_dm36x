############################################################
# File: Makefile
#
############################################################
ROOTDIR = ../../..
include $(ROOTDIR)/Rules.make

INSTALLDIR = $(EXEC_DIR)

INCLUDES += -I ./ -I$(PUBLIC_INCLUDE_DIR) -I ./sysctrl -I include

CC = $(MVTOOL_PREFIX)gcc $(INCLUDES) $(C_FLAGS)
CFLAGS += -Wall -O3

BIN  = upnp-scanip
OBJS = upnp-scanip.o
LINK_TARGET = $(OBJS) $(APP_LIB_DIR)/sysctrl.a $(APP_LIB_DIR)/sys_msg_drv.a $(APP_LIB_DIR)/sem_util.a
UPNP_LIB_DIR = lib/
LIBS = -L$(UPNP_LIB_DIR) -lupnp -lthreadutil -lixml
#LIBS = libupnp.a libthreadutil.a libixml.a
SOURCES = $(wildcard *.c)
REL_OBJ = $(SOURCES:%.c=%.o)

all: $(REL_OBJ)
	$(CC) $(LIBS) -o $(BIN) $(LINK_TARGET)

REL_OBJ = $(SOURCES:%.c=%.o)

install: all
	cp $(BIN) $(INSTALLDIR)
	cp -a -f  lib/*.so* $(ROOT_FILE_SYS)/usr/lib/

uninstall:
	rm -rf $(INSTALLDIR)/$(BIN)

clean: 
	rm -f $(BIN) $(BIN).{elf,gdb} $(OBJS)
