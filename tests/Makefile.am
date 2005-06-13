SUBDIRS =

INCLUDES = -I$(top_srcdir)

LDADD  = -L$(top_builddir)/libmd3/ -lmd3-0.2

check_PROGRAMS = checksizes dumpmd3 rendermd3

TESTS = checksizes

rendermd3_LDADD = $(top_builddir)/libmd3/libmd3-0.2.la -L/usr/X11R6/lib -lSDL_image -lSDL -lpthread -lGL -lGLU -lm
