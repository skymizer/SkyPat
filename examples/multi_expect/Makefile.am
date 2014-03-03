SKYINCLUDES = -I${abs_top_srcdir}/include \
              -I${abs_top_builddir}/include

SKYLDFLAGS = -L$(abs_top_builddir)/lib

SKY_SOURCES = main.cpp \
              my_case.h \
              my_case.cpp

AM_CPPFLAGS = ${SKYINCLUDES}

multi_expectdir = $(pkgdatadir)/examples/multi_expect

noinst_PROGRAMS = multi_expect

multi_expect_LDFLAGS = ${SKYLDFLAGS}

multi_expect_LDADD = -lpat

dist_multi_expect_SOURCES = ${SKY_SOURCES}

include ../Example.mk

$(eval $(call gen_sample_make, multi_expect, ${SKY_SOURCES}))

dist_multi_expect_DATA = ${SKY_SOURCES} SampleMakefile

