##############################################################################
#
#                              pat Configuration
#
#  This file is distributed under the New BSD License.
#  See LICENSE for details. 
#
##############################################################################

####################
#  Autoconf/Automake Initialization

dnl AC_INIT(PKG_NAME, BUILD_VERSION, BUG_REPORTER)
AC_INIT([pat],[bt],[pat-dev@@googlegroups.com])
AC_PREREQ(2.59)

AC_CONFIG_SRCDIR([LICENSE])
AC_CONFIG_AUX_DIR([scripts])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([include/pat/Config/Config.h])

####################
#  Check environments - `host`, `target` and `build`
CHECK_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE([1.9 dist-bzip2 foreign nostdinc subdir-objects])

####################
#  Versioning, see VERSION
CURRENT_VERSION=2
REVISION_VERSION=7
AGE_VERSION=0
FULL_VERSION_NAME=Black-throated
BUILD_VERSION=bt

PAT_VERSION=$CURRENT_VERSION:$REVISION_VERSION:$AGE_VERSION
PACKAGE_VERSION=$CURRENT_VERSION.$REVISION_VERSION.$AGE_VERSION-$BUILD_VERSION
AC_SUBST(PACKAGE_VERSION)
AC_SUBST(PAT_VERSION)

####################
#  Languages
AC_PROG_CC
AC_PROG_CXX
AC_LANG([C++])
AC_CXX_STL_HASH
AM_CONDITIONAL(GCC, test "$GCC" = yes)   # let the Makefile know if we're gcc
AC_ENABLE_SHARED
AC_DISABLE_STATIC
LT_INIT

####################
# Check for compiler warnings
CHECK_WERROR_FLAGS
CXX_FLAG_CHECK([-Wno-variadic-macros])

####################
# Check for headers
AC_CHECK_HEADERS([sys/time.h])

####################
# Check for functions
AC_SEARCH_LIBS([clock_gettime], [rt], [
    AC_DEFINE(HAVE_CLOCK_GETTIME, 1,[Have clock_gettime])
])
AC_CHECK_FUNCS([gettimeofday])

####################
# Check for types
AC_CHECK_TYPES([int8_t, int16_t, int32_t, int64_t, intptr_t])
AC_CHECK_TYPES([uint8_t, uint16_t, uint32_t, uint64_t, uintptr_t])
AC_CHECK_TYPES([long double, long long, long long int, unsigned long long int])
AC_CHECK_TYPES([off_t, size_t, ssize_t])
AC_TYPE_LONG_DOUBLE_WIDER

####################
# Check for options
CHECK_PTHREAD
CHECK_ZLIB
CHECK_ENABLE_UNITTEST
CHECK_ENABLE_OPTIMIZE
CHECK_ENABLE_OPTION([clock_gettime], [no], [ENABLE_CLOCK_GETTIME])
CHECK_ENABLE_OPTION([gettimeofday], [yes], [ENABLE_GETTIMEOFDAY])

####################
# OUTPUT
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([lib/Makefile])
AC_CONFIG_FILES([lib/pat.pc])
AC_CONFIG_FILES([examples/Makefile])
AC_CONFIG_FILES([examples/assertion/Makefile])
AC_CONFIG_FILES([examples/multi_assert/Makefile])
AC_CONFIG_FILES([examples/multi_expect/Makefile])
AC_CONFIG_FILES([examples/multi_case/Makefile])
AC_CONFIG_FILES([examples/performance/Makefile])
AC_CONFIG_FILES([examples/smoke/Makefile])
AC_CONFIG_FILES([examples/fall/Makefile])
AC_CONFIG_FILES([examples/pat_c/Makefile])
AC_CONFIG_FILES([examples/thread/Makefile])

AC_OUTPUT
