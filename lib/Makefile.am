SOURCE = pat.cpp \
	ADT/Color.cpp \
	Support/ManagedStatic.cpp \
	Support/Timer.cpp \
	Support/Unix/Timer.inc \
	Support/Path.cpp \
	Support/Perf.cpp \
	Support/Unix/Path.inc \
	Support/Unix/Perf.inc \
	Listeners/PrettyResultPrinter.cpp \
	Listeners/CSVResultPrinter.cpp \
	Core/Test.cpp \
	Core/Repeater.cpp \
	Core/UnitTest.cpp \
	Thread/Thread.cpp \
	Thread/ThreadImpl.cpp \
	Thread/Mutex.cpp \
	Thread/Pthread/Mutex.inc

ANDROID_CPPFLAGS=-fno-rtti -fno-exceptions -Waddress -Wchar-subscripts -Wcomment -Wformat -Wparentheses -Wreorder -Wreturn-type -Wsequence-point -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var

PAT_INCLUDES = -I${abs_top_srcdir}/include \
                    -I${abs_top_builddir}/include

if ENABLE_OPTIMIZE
PAT_CPPFLAGS = -O2 -std=c++0x
else
PAT_CPPFLAGS = -g -std=c++0x
endif

AM_CPPFLAGS = ${PAT_INCLUDES} ${PAT_CPPFLAGS} ${ANDROID_CPPFLAGS}

#############################
# Build shared library
lib_LTLIBRARIES = libpat.la

libpat_la_SOURCES = ${SOURCE}

libpat_la_LDFLAGS = -version-info @PAT_VERSION@

if HAVE_PTHREADS
libpat_la_LDFLAGS += -lpthread
endif

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = pat.pc

# Remove temporary *.la files
install-exec-hook:
	rm -f $(DESTDIR)$(libdir)/libpat.la
