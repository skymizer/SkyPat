SOURCE = pat.cpp \
	Support/ManagedStatic.cpp \
	Support/Timer.cpp

ANDROID_CPPFLAGS=-fno-rtti -fno-exceptions -Waddress -Wchar-subscripts -Wcomment -Wformat -Wparentheses -Wreorder -Wreturn-type -Wsequence-point -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var

PAT_INCLUDES = -I${abs_top_srcdir}/include \
                    -I${abs_top_builddir}/include \
                    -I${abs_top_srcdir}/lib

if ENABLE_OPTIMIZE
PAT_CPPFLAGS = -O2
else
PAT_CPPFLAGS = -g
endif

PAT_CPPFLAGS += -fvisibility=hidden

AM_CPPFLAGS = ${PAT_INCLUDES} ${PAT_CPPFLAGS} ${ANDROID_CPPFLAGS}

#############################
# Build shared library
lib_LTLIBRARIES = libpat.la

libpat_la_SOURCES = ${SOURCE}

libpat_la_LDFLAGS = -version-info @PAT_VERSION@

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = pat.pc

# Remove temporary *.la files
install-exec-hook:
	rm -f $(libdir)/libpat.la
