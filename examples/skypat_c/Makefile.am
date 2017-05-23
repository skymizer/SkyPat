SKYINCLUDES = -I${abs_top_srcdir}/include \
              -I${abs_top_builddir}/include

SKYLDFLAGS = -L$(abs_top_builddir)/lib

SKY_SOURCES = main.cpp \
              my_case.h \
              my_case.cpp

AM_CPPFLAGS = ${SKYINCLUDES}

skypat_cdir = $(pkgdatadir)/examples/skypat_c

noinst_PROGRAMS = skypat_c

skypat_c_LDFLAGS = ${SKYLDFLAGS}

skypat_c_LDADD = -lskypat

dist_skypat_c_SOURCES = ${SKY_SOURCES}

include ../Example.mk

$(eval $(call gen_sample_make, skypat_c, ${SKY_SOURCES}))

dist_skypat_c_DATA = ${SKY_SOURCES} SampleMakefile

DISTCLEANFILES = SampleMakefile
