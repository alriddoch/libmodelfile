dnl Initialise autoconf

AC_INIT(libmodelfile, 0.1.92, alriddoch@zepler.org)
AC_CONFIG_SRCDIR([libmd3/loader.c])
AC_CONFIG_HEADERS([config.h])

dnl Detect the canonical host and target environment

AC_CANONICAL_HOST

dnl Initialise automake

AM_INIT_AUTOMAKE([dist-bzip2 dist-zip check-news])

dnl Initialise automake

LIBMF_VERSION=$VERSION
LIBMF_INTERFACE_VERSION='-version-info 0:0:0'

AC_SUBST(LIBMF_VERSION)
AC_SUBST(LIBMF_INTERFACE_VERSION)

dnl Test for GNU libtool

AC_DISABLE_STATIC
AC_PROG_LIBTOOL

dnl Test for C compiler

AC_PROG_CC

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

CFLAGS="$CFLAGS -Wall -std=c89 -pedantic"

LIBMF_CFLAGS=""
LIBMF_LIB_SUFFIX="-0.2"
LIBMD3_LIBS="-lmd3$LIBMF_LIB_SUFFIX"
LIBMF_LIBS="$LIBMD3_LIBS"

AC_SUBST(LIBMF_CFLAGS)
AC_SUBST(LIBMF_LIB_SUFFIX)
AC_SUBST(LIBMD3_LIBS)
AC_SUBST(LIBMF_LIBS)

dnl Generate files

AC_OUTPUT([
	Makefile
	libmd3/Makefile
	tests/Makefile
	libmodelfile.spec
	libmd3-0.2.pc
])
