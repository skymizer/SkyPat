SKYINCLUDES = -I${abs_top_srcdir}/include \
              -I${abs_top_builddir}/include

SKYLDFLAGS = -L$(abs_top_builddir)/lib

SKY_SOURCES = main.cpp \
              ThreadTest.h

AM_CPPFLAGS = ${SKYINCLUDES} -fno-rtti

threaddir = $(pkgdatadir)/examples/thread

noinst_PROGRAMS = thread

thread_LDFLAGS = ${SKYLDFLAGS}

thread_LDADD = -lpat

thread_SOURCES = ${SKY_SOURCES}

include ../Example.mk

$(eval $(call gen_sample_make, thread, ${SKY_SOURCES}))

dist_thread_DATA = ${SKY_SOURCES} SampleMakefile

DISTCLEANFILES = SampleMakefile
