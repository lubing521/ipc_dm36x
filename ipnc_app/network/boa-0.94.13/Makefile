SUBDIRS = src

ROOTDIR = ../../..
include $(ROOTDIR)/Rules.make

DEBUGSUBDIRS = $(addsuffix .debug, $(SUBDIRS))
RELEASESUBDIRS = $(addsuffix .release, $(SUBDIRS))
CLEANSUBDIRS = $(addsuffix .clean, $(SUBDIRS))
INSTALLSUBDIRS = $(addsuffix .install, $(SUBDIRS))

.PHONY: $(SUBDIRS) $(DEBUGSUBDIRS) $(RELEASESUBDIRS) $(INSTALLSUBDIRS) \
	$(CLEANSUBDIRS)

all:	$(SUBDIRS)

$(SUBDIRS):
	@echo
	@echo Making all in subdirectory $@...
	@$(MAKE) -C $@

install: $(INSTALLSUBDIRS)

$(DEBUGSUBDIRS):
	@echo
	@echo Executing make debug in subdirectory $(basename $@)...
	@cd $(basename $@) ; $(MAKE) debug

debug:	$(DEBUGSUBDIRS)

$(RELEASESUBDIRS):
	@echo
	@echo Executing make release in subdirectory $(basename $@)...
	@cd $(basename $@) ; $(MAKE) release

release:	$(RELEASESUBDIRS)

$(INSTALLSUBDIRS):
	@echo
	@echo Executing make install in subdirectory $(basename $@)...
	@cd $(basename $@) ; $(MAKE) install

clean:	$(CLEANSUBDIRS)
	@rm -rf lib
	@rm -rf include

$(CLEANSUBDIRS):
	@cd $(basename $@) ; $(MAKE) clean
