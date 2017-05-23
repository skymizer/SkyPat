SKYINCLUDES = -I${abs_top_srcdir}/include \
              -I${abs_top_builddir}/include

SKYLDFLAGS = -L$(abs_top_builddir)/lib

SKY_SOURCES = main.cpp \
              my_case.h \
              my_case.cpp

AM_CPPFLAGS = ${SKYINCLUDES}

performancedir = $(pkgdatadir)/examples/performance

noinst_PROGRAMS = performance

performance_LDFLAGS = ${SKYLDFLAGS}

performance_LDADD = -lskypat

dist_performance_SOURCES = ${SKY_SOURCES}

include ../Example.mk

$(eval $(call gen_sample_make, performance, ${SKY_SOURCES}))

dist_performance_DATA = ${SKY_SOURCES} SampleMakefile

DISTCLEANFILES = SampleMakefile
