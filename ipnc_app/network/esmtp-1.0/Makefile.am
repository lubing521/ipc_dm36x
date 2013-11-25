AM_CFLAGS= -DSYSCONFDIR=\"@sysconfdir@\"
bin_PROGRAMS = esmtp

dist_man_MANS = esmtp.1 esmtprc.5

esmtp_SOURCES = \
	lexer.l \
	list.h \
	local.c \
	local.h \
	main.c \
	main.h \
	message.c \
	message.h \
	parser.y \
	rcfile.h \
	rfc822.c \
	rfc822.h \
	smtp.c \
	smtp.h \
	xmalloc.h

EXTRA_DIST = \
	sample.esmtprc \
	esmtp-wrapper

AM_YFLAGS = -d

install-exec-hook:
	$(mkinstalldirs) $(DESTDIR)$(sbindir) $(DESTDIR)$(libdir)
	rm -f $(DESTDIR)$(sbindir)/sendmail
	rm -f $(DESTDIR)$(libdir)/sendmail
	rm -f $(DESTDIR)$(bindir)/mailq
	rm -f $(DESTDIR)$(bindir)/newaliases
	$(LN_S) $(bindir)/esmtp $(DESTDIR)$(sbindir)/sendmail
	$(LN_S) $(bindir)/esmtp $(DESTDIR)$(libdir)/sendmail
	$(LN_S) $(bindir)/esmtp $(DESTDIR)$(bindir)/mailq
	$(LN_S) $(bindir)/esmtp $(DESTDIR)$(bindir)/newaliases

install-data-hook:
	rm -f $(DESTDIR)$(man1dir)/sendmail.1
	rm -f $(DESTDIR)$(man1dir)/mailq.1
	rm -f $(DESTDIR)$(man1dir)/newaliases.1
	$(LN_S) esmtp.1 $(DESTDIR)$(man1dir)/sendmail.1
	$(LN_S) esmtp.1 $(DESTDIR)$(man1dir)/mailq.1
	$(LN_S) esmtp.1 $(DESTDIR)$(man1dir)/newaliases.1
