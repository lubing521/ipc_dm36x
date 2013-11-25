ROOTDIR = ../../..
include $(ROOTDIR)/Rules.make

CC = $(MVTOOL_PREFIX)gcc
CFLAGS += -Wall -O3

EXEC = dhcpcd
OBJS = arp.o buildmsg.o client.o dhcpcd.o peekfd.o signals.o udpipgen.o dhcpconfig.o cache.o
INSTALLDIR = $(EXEC_DIR)/dhcpcd

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

install: $(EXEC)
	install -d $(INSTALLDIR)
	install $(EXEC) $(INSTALLDIR)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

