SUBDIRS = util gui

ROOTDIR = ../..
include $(ROOTDIR)/Rules.make


.PHONY: $(SUBDIRS) $(DEBUGSUBDIRS) $(RELEASESUBDIRS) $(INSTALLSUBDIRS) \
	$(CLEANSUBDIRS)

all: 
	@echo "do nothing!!!"

install:
	rm -rf $(ROOT_FILE_SYS)/var/www
	cp -aR -f gui/. $(ROOT_FILE_SYS)/var/www/
	cp -aR -f util/. $(ROOT_FILE_SYS)/var/www/
	cp -aR -f etc/ $(ROOT_FILE_SYS)/
	cp -aR -f alarm_wav/. $(EXEC_DIR)/
	rm -f $(ROOT_FILE_SYS)/var/www/tvdevicedesc.xml
	ln -s /tmp/tvdevicedesc.xml $(ROOT_FILE_SYS)/var/www/tvdevicedesc.xml
	
clean:
	@echo "do nothing!!!"

