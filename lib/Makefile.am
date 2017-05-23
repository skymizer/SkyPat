SOURCE = skypat.cpp \
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

SKYPAT_INCLUDES = -I${abs_top_srcdir}/include \
                    -I${abs_top_builddir}/include

if ENABLE_OPTIMIZE
SKYPAT_CPPFLAGS = -O2 -std=c++11
else
SKYPAT_CPPFLAGS = -g -std=c++11
endif

AM_CPPFLAGS = ${SKYPAT_INCLUDES} ${SKYPAT_CPPFLAGS} ${ANDROID_CPPFLAGS}

#############################
# Build shared library
lib_LTLIBRARIES = libskypat.la

libskypat_la_SOURCES = ${SOURCE}

libskypat_la_LDFLAGS = -version-info @SKYPAT_VERSION@

if HAVE_PTHREADS
libskypat_la_LDFLAGS += -lpthread
endif

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = skypat.pc

# Remove temporary *.la files
install-exec-hook:
	rm -f $(DESTDIR)$(libdir)/libskypat.la
