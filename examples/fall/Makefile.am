SKYINCLUDES = -I${abs_top_srcdir}/include \
              -I${abs_top_builddir}/include

SKYLDFLAGS = -L$(abs_top_builddir)/lib

SKY_SOURCES = main.cpp \
              my_case.h \
              my_case.cpp

AM_CPPFLAGS = ${SKYINCLUDES}

falldir = $(pkgdatadir)/examples/fall

noinst_PROGRAMS = fall

fall_LDFLAGS = ${SKYLDFLAGS}

fall_LDADD = -lpat

dist_fall_SOURCES = ${SKY_SOURCES}

include ../Example.mk

$(eval $(call gen_sample_make, fall, ${SKY_SOURCES}))

dist_fall_DATA = ${SKY_SOURCES} SampleMakefile

DISTCLEANFILES = SampleMakefile
