# Generated automatically from Makefile.in by configure.
# $Id: Makefile.in,v 1.59 2002/03/24 22:20:19 jnelson Exp $

.SUFFIXES:
.SUFFIXES: .o .c
.PHONY: clean mrclean distclean depend all dist

ROOTDIR = ../../../..
include $(ROOTDIR)/Rules.make

INSTALLDIR = $(EXEC_DIR)

GCC_FLAGS = -Wstrict-prototypes -Wpointer-arith -Wcast-align -Wcast-qual\
  -Wtraditional\
  -Wshadow\
  -Wconversion\
  -Waggregate-return\
  -Wmissing-prototypes\
  -Wnested-externs\
  -Wall \
  -Wundef -Wwrite-strings -Wredundant-decls -Winline


srcdir = .
VPATH = .:./../extras
LDFLAGS = -g -lpthread
LIBS = $(APP_LIB_DIR)/msg_util.a $(APP_LIB_DIR)/Appro_interface.a \
	$(APP_LIB_DIR)/file_msg_drv.a $(APP_LIB_DIR)/sysctrl.a $(APP_LIB_DIR)/sys_msg_drv.a \
	$(CMEM_INSTALL_DIR)/packages/ti/sdo/linuxutils/cmem/lib/cmem.a470MV
CFLAGS = -O3 -pipe -Wall -I. -I$(PUBLIC_INCLUDE_DIR)

# Change these if necessary

YACC = bison -y 
LEX = flex 
CC = $(MVTOOL_PREFIX)gcc 
CPP = $(MVTOOL_PREFIX)gcc -E

SOURCES = alias.c boa.c buffer.c cgi.c cgi_header.c config.c escape.c \
	get.c hash.c ip.c log.c mmap_cache.c pipe.c queue.c read.c \
	request.c response.c select.c signals.c util.c sublog.c appro_api.c \
	net_config.c web_translate.c file_list.c auth.c para_list.c syslog_list.c accesslog_list.c actvcon_list.c tree.c psia_root.c \
	index.c description.c xadd.c xparse.c iface.c 

OBJS = y.tab.o lex.yy.o $(SOURCES:.c=.o) timestamp.o


all:	boa boa_indexer

install: boa
	install boa $(INSTALLDIR)

boa:	$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)
	
boa_indexer:	index_dir.o escape.o   
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJS) boa core lex.yy.c y.tab.c y.tab.h *~ boa_indexer index_dir.o

distclean:	mrclean

mrclean:	clean
	rm -f config.status config.cache config.h Makefile config.log

# parser dependencies

y.tab.c y.tab.h:	boa_grammar.y
	$(YACC) -d $^

lex.yy.c:	boa_lexer.l
	$(LEX) $^

# timestamp

timestamp.o:	$(SOURCES) boa_grammar.y boa_lexer.l

# depend stuff
.depend:
	$(CPP) -MM $(SOURCES) > .depend
        
depend:
	-rm -f .depend
	$(MAKE) .depend
        
include .depend

# tags
tags:	$(SOURCES)
	ctags -o tags $^ *.h

# dist
dist:
	$(MAKE) clean
	./makedist.sh
        
# object dump
boa.objdump:    boa
	objdump --disassemble-all --source boa > $@

