SKYINCLUDES = -I${abs_top_srcdir}/include \
              -I${abs_top_builddir}/include

SKYLDFLAGS = -L$(abs_top_builddir)/lib

SKY_SOURCES = main.cpp \
              my_case.h \
              my_case.cpp

AM_CPPFLAGS = ${SKYINCLUDES}

multi_assertdir = $(pkgdatadir)/examples/multi_assert

noinst_PROGRAMS = multi_assert

multi_assert_LDFLAGS = ${SKYLDFLAGS}

multi_assert_LDADD = -lskypat

dist_multi_assert_SOURCES = ${SKY_SOURCES}

include ../Example.mk

$(eval $(call gen_sample_make, multi_assert, ${SKY_SOURCES}))

dist_multi_assert_DATA = ${SKY_SOURCES} SampleMakefile

DISTCLEANFILES = SampleMakefile
