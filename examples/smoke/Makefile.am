SKYINCLUDES = -I${abs_top_srcdir}/include \
              -I${abs_top_builddir}/include

SKYLDFLAGS = -L$(abs_top_builddir)/lib

SKY_SOURCES = main.cpp \
              my_case.h \
              my_case.cpp

AM_CPPFLAGS = ${SKYINCLUDES}

smokedir = $(pkgdatadir)/examples/smoke

noinst_PROGRAMS = smoke

smoke_LDFLAGS = ${SKYLDFLAGS}

smoke_LDADD = -lpat

dist_smoke_SOURCES = ${SKY_SOURCES}

include ../Example.mk

$(eval $(call gen_sample_make, smoke, ${SKY_SOURCES}))

dist_smoke_DATA = ${SKY_SOURCES} SampleMakefile

