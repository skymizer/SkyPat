SKYINCLUDES = -I${abs_top_srcdir}/include \
              -I${abs_top_builddir}/include

SKYLDFLAGS = -L$(abs_top_builddir)/lib

SKY_SOURCES = main.cpp \
              my_case.h \
              my_case.cpp

AM_CPPFLAGS = ${SKYINCLUDES}

assertiondir = $(pkgdatadir)/examples/assertion

noinst_PROGRAMS = assertion

assertion_LDFLAGS = ${SKYLDFLAGS}

assertion_LDADD = -lpat

assertion_SOURCES = ${SKY_SOURCES}

include ../Example.mk

$(eval $(call gen_sample_make, assertion, ${SKY_SOURCES}))

dist_assertion_DATA = ${SKY_SOURCES} SampleMakefile

