ROOTDIR = ../../../..
include $(ROOTDIR)/Rules.make

LIVE_DIR = ../
EXEC = audio-receiver

all:	$(EXEC)

install:	$(EXEC)
	install $(EXEC) $(EXEC_DIR)

CC = $(MVTOOL_PREFIX)gcc
CPLUSPLUS = $(MVTOOL_PREFIX)g++

INCLUDES = -I . \
	-I$(LIVE_DIR)/BasicUsageEnvironment/include \
	-I$(LIVE_DIR)/UsageEnvironment/include \
	-I$(LIVE_DIR)/groupsock/include \
	-I$(LIVE_DIR)/liveMedia/include \
	-I$(PUBLIC_INCLUDE_DIR)

CFLAGS = $(INCLUDES) -D_LINUX -g -Wall -O3

LIBS =	$(CMEM_INSTALL_DIR)/packages/ti/sdo/linuxutils/cmem/lib/cmem.a470MV \
	-L$(LIVE_DIR)/BasicUsageEnvironment -lBasicUsageEnvironment \
	-L$(LIVE_DIR)/UsageEnvironment -lUsageEnvironment \
	-L$(LIVE_DIR)/groupsock -lgroupsock \
	-L$(LIVE_DIR)/liveMedia -lliveMedia

OBJS = audio-receiver.o AudioSink.o \
	$(APP_LIB_DIR)/Appro_interface.a

$(EXEC): $(OBJS)
	$(CPLUSPLUS) $(CFLAGS) -o $(EXEC) $(OBJS) $(LIBS)

Err.cpp:		Err.hh

AudioSink.cpp:		AudioSink.hh 

audio-receiver.cpp:	AudioSink.hh playCommon.hh
 
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

.cpp.o:
	$(CPLUSPLUS) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *~
	rm -f $(EXEC)
