#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([mll], [0.1], [javier.angulo1@gmail.com])
AC_PREFIX_DEFAULT([/usr])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AM_INIT_AUTOMAKE

AC_LANG(C++)

# Internationalization
IT_PROG_INTLTOOL([0.41.0])
GETTEXT_PACKAGE=mll
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Translation domain used])
AM_GLIB_GNU_GETTEXT

PROGRAMNAME_LOCALEDIR=[${datadir}/locale]
AC_SUBST(PROGRAMNAME_LOCALEDIR)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CPP

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_PID_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([atexit getcwd mkfifo])

PKG_CHECK_MODULES([MLL], [gtk+-3.0])

AC_CONFIG_FILES(po/Makefile.in)
AC_OUTPUT

