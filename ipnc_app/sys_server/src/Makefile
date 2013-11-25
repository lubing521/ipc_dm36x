# Makefile
#
ROOTDIR = ../../..
include $(ROOTDIR)/Rules.make

TARGET = system_server

C_FLAGS += -Wall -g -O3

CPP_FLAGS += -I$(PUBLIC_INCLUDE_DIR) \
		-I../inc \
		-D_GNU_SOURCE \
		-D_LARGEFILE64_SOURCE \
		-D_FILE_OFFSET_BITS=64 \
                $(XDC_FLAGS)

LD_FLAGS += -lfreetype -lpthread

COMPILE.c = $(MVTOOL_PREFIX)gcc $(C_FLAGS) $(CPP_FLAGS) -c
LINK.c = $(MVTOOL_PREFIX)gcc $(LD_FLAGS)

RELTARGET = release/$(TARGET)

RELCFLAGS = -fno-strict-aliasing

SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h) $(PUBLIC_INCLUDE_DIR)/*.h

RELOBJFILES = $(SOURCES:%.c=release/%.o)

RELLDFLAGS =



.PHONY: clean release install

all:    release

install: release
	install -d $(EXEC_DIR)
	install $(RELTARGET) $(EXEC_DIR)
	ln -sf /tmp/localtime $(TARGET_FS)/usr/share/zoneinfo/localtime
	ln -sf /tmp/localtime $(TARGET_FS)/usr/share/zoneinfo/posixrules
	ln -sf /tmp/localtime $(TARGET_FS)/etc/localtime

release:    $(RELTARGET)

$(RELTARGET):   $(RELOBJFILES) $(APP_LIB_DIR)/file_msg_drv.a $(APP_LIB_DIR)/Appro_interface.a $(APP_LIB_DIR)/alarm_msg_drv.a $(APP_LIB_DIR)/dm355_gio_util.a $(CMEM_INSTALL_DIR)/packages/ti/sdo/linuxutils/cmem/lib/cmem.a470MV
	$(LINK.c) -o $@ $^ $(RELLDFLAGS)


$(RELOBJFILES): release/%.o: %.c $(HEADERS)
	@mkdir -p release
	$(COMPILE.c) $(RELCFLAGS) -o $@ $<

clean:
	-$(RM) -rf release  *.d
