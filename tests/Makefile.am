SUBDIRS =

INCLUDES = -I$(top_srcdir)

LDADD  = -L$(top_builddir)/libmd3/

LIBS = -lmd3-0.2

bin_PROGRAMS = dumpmd3 rendermd3

check_PROGRAMS = testmd3

TESTS = $(check_PROGRAMS)

testmd3_SOURCES = testmd3.c

rendermd3_LDADD = $(top_builddir)/libmd3/libmd3-0.2.la -L/usr/X11R6/lib -lSDL -lpthread -lGL -lGLU
