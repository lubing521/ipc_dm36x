ROOTDIR = ../../..
include $(ROOTDIR)/Rules.make
CFLAGS +=  -I$(LINUXKERNEL_INSTALL_DIR)/include -O3 -Wall -static
TARGETS = arm_loading
LIBS = -lio
CC = $(MVTOOL_PREFIX)gcc
all:$(TARGETS)
		

%.o:%.c
	$(CC) $(CFLAGS) -g -D__DEBUG -c $^

install:
	cp -f $(TARGETS) $(EXEC_DIR)
	
arm_loading: main.o
	$(CC) $(LDFLAGS) -o $@ $^

	
clean:
	rm -f *.o *~
	rm -f $(TARGETS)
