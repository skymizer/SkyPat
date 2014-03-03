SKYINCLUDES = -I${abs_top_srcdir}/include \
              -I${abs_top_builddir}/include

SKYLDFLAGS = -L$(abs_top_builddir)/lib

SKY_SOURCES = main.cpp \
              my_case.h \
              my_case.cpp

AM_CPPFLAGS = ${SKYINCLUDES}

pat_cdir = $(pkgdatadir)/examples/pat_c

noinst_PROGRAMS = pat_c

pat_c_LDFLAGS = ${SKYLDFLAGS}

pat_c_LDADD = -lpat

dist_pat_c_SOURCES = ${SKY_SOURCES}

include ../Example.mk

$(eval $(call gen_sample_make, pat_c, ${SKY_SOURCES}))

dist_pat_c_DATA = ${SKY_SOURCES} SampleMakefile

