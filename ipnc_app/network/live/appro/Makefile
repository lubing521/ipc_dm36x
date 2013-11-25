ROOTDIR = ../../../..
include $(ROOTDIR)/Rules.make

LIVE_DIR = ../
EXEC = wis-streamer

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

OBJS = wis-streamer.o Err.o APPROInput.o \
	WISServerMediaSubsession.o \
	WISJPEGStreamSource.o \
	WISJPEGVideoServerMediaSubsession.o \
	WISMPEG4VideoServerMediaSubsession.o \
	WISH264VideoServerMediaSubsession.o \
	WISPCMAudioServerMediaSubsession.o \
	$(APP_LIB_DIR)/Appro_interface.a

$(EXEC): $(OBJS)
	$(CPLUSPLUS) $(CFLAGS) -o $(EXEC) $(OBJS) $(LIBS)


wis-streamer.cpp:			Err.hh

Err.cpp:				Err.hh

APPROInput.cpp:				APPROInput.hh Err.hh

WISServerMediaSubsession.cpp:		WISServerMediaSubsession.hh

WISServerMediaSubsession.hh:		APPROInput.hh
WISMPEG4VideoServerMediaSubsession.hh:	WISServerMediaSubsession.hh

WISMPEG4VideoServerMediaSubsession.cpp:	WISMPEG4VideoServerMediaSubsession.hh

WISH264VideoServerMediaSubsession.hh:	WISServerMediaSubsession.hh

WISH264VideoServerMediaSubsession.cpp:	WISH264VideoServerMediaSubsession.hh

WISJPEGStreamSource.cpp:		WISJPEGStreamSource.hh
WISPCMAudioServerMediaSubsession.cpp:	WISPCMAudioServerMediaSubsession.hh


.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

.cpp.o:
	$(CPLUSPLUS) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *~
	rm -f $(EXEC)
