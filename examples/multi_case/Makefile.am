SKYINCLUDES = -I${abs_top_srcdir}/include \
              -I${abs_top_builddir}/include

SKYLDFLAGS = -L$(abs_top_builddir)/lib

SKY_SOURCES = main.cpp \
              my_case.h \
              my_case.cpp

AM_CPPFLAGS = ${SKYINCLUDES}

multi_casedir = $(pkgdatadir)/examples/multi_case

noinst_PROGRAMS = multi_case

multi_case_LDFLAGS = ${SKYLDFLAGS}

multi_case_LDADD = -lpat

dist_multi_case_SOURCES = ${SKY_SOURCES}

include ../Example.mk

$(eval $(call gen_sample_make, multi_case, ${SKY_SOURCES}))

dist_multi_case_DATA = ${SKY_SOURCES} SampleMakefile

DISTCLEANFILES = SampleMakefile
