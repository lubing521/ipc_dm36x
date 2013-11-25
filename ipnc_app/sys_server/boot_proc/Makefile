# Makefile
#
ROOTDIR = ../../..
include $(ROOTDIR)/Rules.make

TARGET = boot_proc

C_FLAGS += -Wall -g -O3

CPP_FLAGS += -I$(PUBLIC_INCLUDE_DIR) \
				-I. \
		-D_GNU_SOURCE \
		-D_LARGEFILE64_SOURCE \
		-D_FILE_OFFSET_BITS=64 \
                $(XDC_FLAGS)

COMPILE.c = $(MVTOOL_PREFIX)gcc $(C_FLAGS) $(CPP_FLAGS) -c
LINK.c = $(MVTOOL_PREFIX)gcc $(LD_FLAGS)

RELTARGET = release/$(TARGET)

RELCFLAGS = -O3 -fno-strict-aliasing

SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h) $(PUBLIC_INCLUDE_DIR)/*.h

RELOBJFILES = $(SOURCES:%.c=release/%.o)

RELLDFLAGS =



.PHONY: clean release install

all:    release

install: release
	install -d $(EXEC_DIR)
	install $(RELTARGET) $(EXEC_DIR)

release:    $(RELTARGET)

$(RELTARGET):   $(RELOBJFILES) $(APP_LIB_DIR)/sys_msg_drv.a $(APP_LIB_DIR)/msg_util.a \
				$(APP_LIB_DIR)/sem_util.a
	$(LINK.c) -o $@ $^ $(RELLDFLAGS)


$(RELOBJFILES): release/%.o: %.c $(HEADERS)
	@mkdir -p release
	$(COMPILE.c) $(RELCFLAGS) -o $@ $<

clean:
	-$(RM) -rf release  *.d
