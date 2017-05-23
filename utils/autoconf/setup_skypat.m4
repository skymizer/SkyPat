dnl
dnl @synopsis SETUP_SKYPAT([SKYPAT-INSTALL-PATH])
dnl
dnl @summary install "skypat" library, and set up variables:
dnl   SKYPAT_CFLAGS="-I${abs_top_srcdir}/skypat/include"
dnl   SKYPAT_CPPFLAGS="-I${abs_top_srcdir}/skypat/include"
dnl   SKYPAT_LDFLAGS="-L${abs_top_builddir}/lib -lskypat"
dnl
dnl This macro set up Config.h and configuration of the skypat which is installed
dnl at [SKYPAT-INSTALL-PATH].
dnl
dnl Luba Tang <luba@skymizer.com>

AC_DEFUN([SETUP_SKYPAT],
[dnl

# Check for skypat.
AC_MSG_CHECKING(configure "skypat" library)

SKYPAT_PATH=$1

AC_CANONICAL_HOST

AC_CACHE_CHECK([type of operating system we're going to host on],
  [skypat_cv_host_os_type],
  [case $host in
  *-*-aix*)
    skypat_cv_host_os_type="AIX"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-irix*)
    skypat_cv_host_os_type="IRIX"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-cygwin*)
    skypat_cv_host_os_type="Cygwin"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-darwin*)
    skypat_cv_host_os_type="Darwin"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-minix*)
    skypat_cv_host_os_type="Minix"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-freebsd*)
    skypat_cv_host_os_type="FreeBSD"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-openbsd*)
    skypat_cv_host_os_type="OpenBSD"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-netbsd*)
    skypat_cv_host_os_type="NetBSD"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-dragonfly*)
    skypat_cv_host_os_type="DragonFly"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-hpux*)
    skypat_cv_host_os_type="HP-UX"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-interix*)
    skypat_cv_host_os_type="Interix"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-linux*)
    skypat_cv_host_os_type="Linux"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-solaris*)
    skypat_cv_host_os_type="SunOS"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-auroraux*)
    skypat_cv_host_os_type="AuroraUX"
    skypat_cv_host_platform_type="Unix" ;;
  *-*-win32*)
    skypat_cv_host_os_type="Win32"
    skypat_cv_host_platform_type="Win32" ;;
  *-*-mingw*)
    skypat_cv_host_os_type="MingW"
    skypat_cv_host_platform_type="Win32" ;;
  *-*-haiku*)
    skypat_cv_host_os_type="Haiku"
    skypat_cv_host_platform_type="Unix" ;;
  *-unknown-eabi*)
    skypat_cv_host_os_type="Freestanding"
    skypat_cv_host_platform_type="Unix" ;;
  *-unknown-elf*)
    skypat_cv_host_os_type="Freestanding"
    skypat_cv_host_platform_type="Unix" ;;
  *)
    skypat_cv_host_os_type="Unknown"
    skypat_cv_host_platform_type="Unknown" ;;
  esac])

dnl Set the "OS" Makefile variable based on the platform type so the
dnl makefile can configure itself to specific build hosts
if test "$skypat_cv_host_os_type" = "Unknown" ; then
  AC_MSG_ERROR([Operating system is unknown, configure can't continue])
fi

dnl Set the "SKYPAT_ON_*" variables based on llvm_cv_llvm_cv_platform_type
dnl This is used by lib/Support to determine the basic kind of implementation
dnl to use.
case $skypat_cv_host_platform_type in
Unix)
  AC_DEFINE([SKYPAT_ON_UNIX],[1],[Define if this is Unixish platform])
  AC_SUBST(SKYPAT_ON_PLATFORM,[SKYPAT_ON_UNIX])
  AC_CHECK_HEADERS([sys/time.h])
  AC_CHECK_HEADERS([sys/times.h])
  AC_SEARCH_LIBS([clock_gettime], [rt], [
    AC_DEFINE(HAVE_CLOCK_GETTIME, 1,[Have clock_gettime])
  ])
  AC_CHECK_FUNCS([gettimeofday])

  AC_CHECK_TYPES([int8_t, int16_t, int32_t, int64_t, intptr_t])
  AC_CHECK_TYPES([uint8_t, uint16_t, uint32_t, uint64_t, uintptr_t])
  AC_CHECK_TYPES([long double, long long, long long int, unsigned long long int])
  AC_CHECK_TYPES([off_t, size_t, ssize_t])
  AC_TYPE_LONG_DOUBLE_WIDER
;;
Win32)
  AC_DEFINE([SKYPAT_ON_WIN32],[1],[Define if this is Win32ish platform])
  AC_SUBST(SKYPAT_ON_PLATFORM,[SKYPAT_ON_WIN32])
;;
esac

AC_CONFIG_HEADERS([utils/skypat/include/skypat/Config/Config.h])

skypat_src_path='${abs_top_srcdir}/'
skypat_src_path+=${SKYPAT_PATH}

skypat_build_path='${abs_top_builddir}/'
skypat_build_path+=${SKYPAT_PATH}

SKYPAT_CFLAGS="-I${skypat_src_path}/include -I${skypat_build_path}/include"
SKYPAT_CPPFLAGS="-I${skypat_src_path}/include -I${skypat_build_path}/include"
SKYPAT_LDFLAGS="-L${skypat_build_path}/lib -lskypat"

AC_SUBST(SKYPAT_CFLAGS)
AC_SUBST(SKYPAT_CPPFLAGS)
AC_SUBST(SKYPAT_LDFLAGS)

])
