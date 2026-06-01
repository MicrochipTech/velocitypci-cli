AC_INIT(VERSION-GEN)

AC_PROG_CC
AC_CHECK_TOOL([WINDRES], [windres])

AC_ARG_WITH([curses],
             [AS_HELP_STRING([--with-curses],
	                   [support fancy gui @<:@default=check@:>@])],
             [],
	     [with_curses=check])

AC_ARG_WITH([openssl],
             [AS_HELP_STRING([--with-openssl],
	                   [support OpenSSL library @<:@default=check@:>@])],
             [],
	     [with_openssl=check])

AS_IF([test "x$with_curses" != xno],
       [AC_CHECK_LIB([ncurses], [initscr], [], [], [])])

AS_IF([test "x$with_curses" != xno &&
	   test "x$ac_cv_lib_ncurses_initscr" != xyes ],
       [AC_CHECK_LIB([tinfo], [tigetstr], [], [], [])
        AC_CHECK_LIB([curses], [initscr], [], [], [-ltinfo])])

AC_CHECK_HEADERS([curses.h ncurses/curses.h])

AS_IF([test "x$with_openssl" != xno],
       [AC_CHECK_HEADERS([openssl/pem.h])
        AC_CHECK_LIB([crypto], [RSA_free], [], [], [])
        AC_CHECK_DECLS([RSA_get0_key], [], [], [#include <openssl/pem.h>])])

AC_CONFIG_FILES([Makefile])

AC_CONFIG_HEADER([config.h])

AC_OUTPUT()

AS_IF([test "x$ac_cv_lib_ncurses_initscr" != xyes &&
       test "x$ac_cv_lib_curses_initscr"  != xyes],
      [echo "NOTE  libcurses not enabled - 'gui' and 'diag eye' commands are not supported"])
AS_IF([test "x$ac_cv_lib_crypto_RSA_free" != xyes ||
       test "x$ac_cv_header_openssl_pem_h" != xyes],
      [echo "NOTE  openssl not enabled - 'mfg kmsk-entry-add', 'mfg debug-unlock', 'mfg debug-lock-update', and 'mfg sjtag-unlock' commands are not supported"])
