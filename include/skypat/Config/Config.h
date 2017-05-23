/* include/skypat/Config/Config.h.  Generated from Config.h.in by configure.  */
/* include/skypat/Config/Config.h.in.  Generated from configure.ac by autoheader.  */

/* Enable clock_gettime */
/* #undef ENABLE_CLOCK_GETTIME */

/* Enable gettimeofday */
#define ENABLE_GETTIMEOFDAY 1

/* the name of <hash_map> */
#define HASH_MAP_CLASS unordered_map

/* the location of <unordered_map> or <hash_map> */
#define HASH_MAP_H <tr1/unordered_map>

/* the namespace of hash_map/hash_set */
#define HASH_NAMESPACE std::tr1

/* the name of <hash_set> */
#define HASH_SET_CLASS unordered_set

/* the location of <unordered_set> or <hash_set> */
#define HASH_SET_H <tr1/unordered_set>

/* Define to 1 if you have the <asm/unistd.h> header file. */
#define HAVE_ASM_UNISTD_H 1

/* Have clock_gettime */
#define HAVE_CLOCK_GETTIME 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* define if the compiler has hash_map */
#define HAVE_HASH_MAP 1

/* define if the compiler has hash_set */
#define HAVE_HASH_SET 1

/* Define to 1 if the system has the type `int16_t'. */
#define HAVE_INT16_T 1

/* Define to 1 if the system has the type `int32_t'. */
#define HAVE_INT32_T 1

/* Define to 1 if the system has the type `int64_t'. */
#define HAVE_INT64_T 1

/* Define to 1 if the system has the type `int8_t'. */
#define HAVE_INT8_T 1

/* Define to 1 if the system has the type `intptr_t'. */
#define HAVE_INTPTR_T 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <linux/perf_event.h> header file. */
#define HAVE_LINUX_PERF_EVENT_H 1

/* Define to 1 if the system has the type `long double'. */
#define HAVE_LONG_DOUBLE 1

/* Define to 1 if the type `long double' works and has more range or precision
   than `double'. */
#define HAVE_LONG_DOUBLE_WIDER 1

/* Define to 1 if the system has the type `long long'. */
#define HAVE_LONG_LONG 1

/* Define to 1 if the system has the type `long long int'. */
#define HAVE_LONG_LONG_INT 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if the system has the type `off_t'. */
#define HAVE_OFF_T 1

/* Define if you have POSIX threads libraries and header files. */
#define HAVE_PTHREAD 1

/* Define to 1 if the system has the type `size_t'. */
#define HAVE_SIZE_T 1

/* Define to 1 if the system has the type `ssize_t'. */
#define HAVE_SSIZE_T 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if the system has the type `uint16_t'. */
#define HAVE_UINT16_T 1

/* Define to 1 if the system has the type `uint32_t'. */
#define HAVE_UINT32_T 1

/* Define to 1 if the system has the type `uint64_t'. */
#define HAVE_UINT64_T 1

/* Define to 1 if the system has the type `uint8_t'. */
#define HAVE_UINT8_T 1

/* Define to 1 if the system has the type `uintptr_t'. */
#define HAVE_UINTPTR_T 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if the system has the type `unsigned long long int'. */
#define HAVE_UNSIGNED_LONG_LONG_INT 1

/* Enable classes using zlib compression. */
#define HAVE_ZLIB 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "skypat"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "skypat-dev@@googlegroups.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "skypat"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "skypat aquavit"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "skypat"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "aquavit"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* default target triple */
#define SKYPAT_DEFAULT_TARGET_TRIPLE $target

/* Define if this is Unixish platform */
#define SKYPAT_ON_UNIX 1

/* Define if this is Win32ish platform */
/* #undef SKYPAT_ON_WIN32 */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "aquavit"
