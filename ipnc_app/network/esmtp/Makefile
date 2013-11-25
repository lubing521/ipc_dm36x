############################################################
# File: Makefile
#
############################################################
ROOTDIR = ../../..
include $(ROOTDIR)/Rules.make

INSTALLDIR = $(EXEC_DIR)

CC = $(MVTOOL_PREFIX)gcc
CFLAGS += -Wall -O3 -I$(PUBLIC_INCLUDE_DIR)

BIN  = appro-mail
OBJS = appro-mail.o
LIBS = $(APP_LIB_DIR)/Appro_interface.a $(APP_LIB_DIR)/msg_util.a $(APP_LIB_DIR)/sys_msg_drv.a $(APP_LIB_DIR)/sysctrl.a \
	$(CMEM_INSTALL_DIR)/packages/ti/sdo/linuxutils/cmem/lib/cmem.a470MV

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS) $(LDLIBS)

install: all
	cp $(BIN) $(INSTALLDIR)
	cp esmtp $(INSTALLDIR)

uninstall:
	rm -rf $(INSTALLDIR)/$(BIN)

clean: 
	rm -f $(BIN) $(BIN).{elf,gdb} $(OBJS)
