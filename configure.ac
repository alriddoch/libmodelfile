dnl Initialise autoconf

AC_INIT([libmodelfile], [0.1.94], [alriddoch@googlemail.com])
AC_CONFIG_SRCDIR([libmd3/loader.c])
AC_CONFIG_HEADERS([config.h])

dnl Detect the canonical host and target environment

AC_CANONICAL_HOST

dnl Initialise automake

AM_INIT_AUTOMAKE([nostdinc dist-bzip2 dist-zip check-news])

dnl Initialise automake

LIBMF_VERSION=$VERSION
LIBMF_INTERFACE_VERSION='-version-info 1:0:0'

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

AC_CHECK_FUNC(cos,,[AC_CHECK_LIB(m,cos)])

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES(LIBXML2, libxml-2.0,
[
    CPPFLAGS="$CPPFLAGS $LIBXML2_CFLAGS"
    LIBS="$LIBS $LIBXML2_LIBS"
],
[
    AC_MSG_RESULT([no])
    AC_MSG_ERROR([
Cannot find valid versions of required libxml2 library:
$LIBXML2_PKG_ERRORS
Perhaps you should add the directory containing the missing libraries to the
PKG_CONFIG_PATH environment variable.])
])

LIBMF_CFLAGS=""
LIBMF_LIB_SUFFIX="-0.2"
LIBMD3_LIBS="-lmd3$LIBMF_LIB_SUFFIX"
LIBMF_LIBS="$LIBMD3_LIBS"

AC_SUBST(LIBMF_CFLAGS)
AC_SUBST(LIBMF_LIB_SUFFIX)
AC_SUBST(LIBMD3_LIBS)
AC_SUBST(LIBMF_LIBS)

dnl Generate files

AC_CONFIG_FILES([
	Makefile
	libmd3/Makefile
	libcolladadae/Makefile
	libcolladadae/tests/Makefile
	tests/Makefile
	libmodelfile.spec
	libmd3-0.2.pc
])
AC_OUTPUT
