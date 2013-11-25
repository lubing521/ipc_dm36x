dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.53)
AC_INIT(esmtp, 1.0)
AC_CONFIG_SRCDIR(Makefile.am)
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE(1.9 dist-bzip2)
AM_MAINTAINER_MODE

AC_PROG_CC

AC_HEADER_STDC

dnl getopt needs this
AC_C_CONST

AC_PROG_LEX
AC_PROG_LN_S
AC_PROG_YACC

dnl Check for libESMTP
AC_ARG_WITH(libesmtp,
	    AC_HELP_STRING([--with-libesmtp=DIR],
			   [use libesmtp library from (prefix) directory DIR]),
	    ,)
if test "$with_libesmtp" != "no" -a "$with_libesmtp" != "yes"
then
	CPPFLAGS="$CPPFLAGS -I$with_libesmtp/include"
	LDFLAGS="$LDFLAGS -L$with_libesmtp/lib"
	AC_CHECK_HEADER(libesmtp.h, ,[AC_MSG_ERROR([libesmtp.h not found in $with_libesmtp])])
fi    
AC_MSG_CHECKING(for libESMTP)
if libesmtp-config --version > /dev/null 2>&1
then
	AC_MSG_RESULT(yes)
	CFLAGS="$CFLAGS `libesmtp-config --cflags`"
	LIBS="$LIBS `libesmtp-config --libs`"
else
	AC_MSG_RESULT(no)
	AC_MSG_ERROR(libESMTP library not found)
fi

AC_SUBST( sysconfdir )

jrf_FUNC_GETOPT

AC_CHECK_FUNCS([getuid geteuid])
		
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
