dnl Initialise autoconf

AC_INIT()
AC_CONFIG_SRCDIR([libmd3/loader.c])
AM_CONFIG_HEADER(config.h)

dnl Detect the canonical host and target environment

AC_CANONICAL_SYSTEM

VERSION=0.0.1
AM_INIT_AUTOMAKE("libmodelfile", $VERSION)

dnl Initialise automake

LIBMD3_VERSION=$VERSION
LIBMD3_INTERFACE_VERSION='-version-info 0:0:0'

AC_SUBST(LIBMD3_VERSION)
AC_SUBST(LIBMD3_INTERFACE_VERSION)

dnl Test for GNU libtool

AM_DISABLE_STATIC
AM_PROG_LIBTOOL

dnl Test for C++ compiler

AC_ARG_ENABLE(debug,
    [  --enable-debug          enable debug information [default=no]],
    [
        if test "$enableval" = "yes"; then
            CFLAGS="$CFLAGS -DDEBUG"
        else
            CFLAGS="$CFLAGS -DNDEBUG"
        fi
    ],[
        CFLAGS="$CFLAGS -DNDEBUG"
    ]
)

CFLAGS="$CFLAGS -Wall -std=c89"

LIBMD3_CFLAGS=""
LIBMD3_LIB_SUFFIX="-0.2"
LIBMD3_LIBS="-lmd3-$LIBMD3_LIB_SUFFIX"

AC_SUBST(LIBMD3_CFLAGS)
AC_SUBST(LIBMD3_LIB_SUFFIX)
AC_SUBST(LIBMD3_LIBS)

dnl Generate files

AC_OUTPUT([
	Makefile
	libmd3/Makefile
	tests/Makefile
	libmd3.spec
	libmd3-0.2.pc
])
