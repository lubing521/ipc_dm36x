ROOTDIR = ../../..
include $(ROOTDIR)/Rules.make

INSTALLDIR = $(EXEC_DIR)

prefix=/usr/local
exec_prefix=${prefix}
MANDIR=${prefix}/man
BINDIR=${exec_prefix}/bin
SRCDIR=.
CC=arm_v5t_le-gcc

INSTALL=install
INSTALL_PROGRAM=${INSTALL}
INSTALL_DATA=${INSTALL}
BIN  = quftp upload

OBJS=ftp.o ftp_data.o ftp_directory.o ftp_login.o ftp_transfer.o ftp_list.o \
	text.o rc.o quftp.o log.o
LIBS += -lreadline -lncurses $(APP_LIB_DIR)/Appro_interface.a $(APP_LIB_DIR)/msg_util.a $(APP_LIB_DIR)/sys_msg_drv.a $(APP_LIB_DIR)/sysctrl.a \
	$(CMEM_INSTALL_DIR)/packages/ti/sdo/linuxutils/cmem/lib/cmem.a470MV

HEADERS=*.h
DEFS = -DHAVE_LIBNCURSES=1 -DHAVE_LIBREADLINE=1 -DHAVE_DIRENT_H=1 -DSTDC_HEADERS=1 -DHAVE_SYS_WAIT_H=1 -DHAVE_FCNTL_H=1 -DHAVE_MALLOC_H=1 -DHAVE_SYS_TIME_H=1 -DHAVE_UNISTD_H=1 -DTIME_WITH_SYS_TIME=1 -DHAVE_FNMATCH=1 -DHAVE_GETCWD=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_MKDIR=1 -DHAVE_SOCKET=1 -DHAVE_STRDUP=1 -DHAVE_STRSTR=1 
CFLAGS += -g -O3 -Wall -I$(PUBLIC_INCLUDE_DIR)

.SUFFIXES:
.SUFFIXES: .c .o

.PHONY:	install clean

all:	quftp

install: all
	cp $(BIN) $(INSTALLDIR)

install-strip:
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' install

uninstall:
	rm -f ${DESTDIR}${BINDIR}/quftp
	rm -f ${DESTDIR}${MANDIR}/man1/quftp.1

clean:
	rm -f *.o quftp

distclean:
	rm -f config.status config.log config.cache Makefile

quftp:	quftp.c ${OBJS} ${HEADERS}
	${CC} -I. -o $@ ${OBJS} ${CFLAGS} ${LIBS}

temp:	temp.o

ftp.o:		${HEADERS}
ftp_data.o:	${HEADERS}
ftp_directory.o:${HEADERS}
ftp_login.o:	${HEADERS}
ftp_transfer.o:	${HEADERS}
ftp_list.o:	${HEADERS}
text.o:		${HEADERS}
rc.o:		${HEADERS}
quftp.o:	${HEADERS}

