AC_PREREQ(2.59)
AC_INIT(misc-tools, 1.0.0, fargier.sylvain@free.fr)

AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([build])
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES])

AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
PKG_CHECK_MODULES(CPPUNIT, cppunit >= 1.12.1)

AC_SUBST(CPPUNIT_LIBS)
AC_SUBST(CPPUNIT_CFLAGS)

LDFLAGS+=" -Wl,--no-undefined "
CFLAGS+=" -Wall -W -Werror "

AC_CONFIG_FILES([
        Makefile
        src/Makefile
        src/cxx/Makefile
        src/c/Makefile])

AC_OUTPUT
