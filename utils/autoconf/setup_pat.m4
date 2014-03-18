dnl
dnl @synopsis SETUP_PAT([PAT-INSTALL-PATH])
dnl
dnl @summary install "pat" library, and set up variables:
dnl   PAT_CFLAGS="-I${abs_top_srcdir}/pat/include"
dnl   PAT_CPPFLAGS="-I${abs_top_srcdir}/pat/include"
dnl   PAT_LDFLAGS="-L${abs_top_builddir}/lib -lpat"
dnl
dnl This macro set up Config.h and configuration of the pat which is installed
dnl at [PAT-INSTALL-PATH].
dnl
dnl Luba Tang <luba@skymizer.com>

AC_DEFUN([SETUP_PAT],
[dnl

# Check for pat.
AC_MSG_CHECKING(configure "pat" library)

PAT_PATH=$1

AC_CANONICAL_HOST

AC_CACHE_CHECK([type of operating system we're going to host on],
  [pat_cv_host_os_type],
  [case $host in
  *-*-aix*)
    pat_cv_host_os_type="AIX"
    pat_cv_host_platform_type="Unix" ;;
  *-*-irix*)
    pat_cv_host_os_type="IRIX"
    pat_cv_host_platform_type="Unix" ;;
  *-*-cygwin*)
    pat_cv_host_os_type="Cygwin"
    pat_cv_host_platform_type="Unix" ;;
  *-*-darwin*)
    pat_cv_host_os_type="Darwin"
    pat_cv_host_platform_type="Unix" ;;
  *-*-minix*)
    pat_cv_host_os_type="Minix"
    pat_cv_host_platform_type="Unix" ;;
  *-*-freebsd*)
    pat_cv_host_os_type="FreeBSD"
    pat_cv_host_platform_type="Unix" ;;
  *-*-openbsd*)
    pat_cv_host_os_type="OpenBSD"
    pat_cv_host_platform_type="Unix" ;;
  *-*-netbsd*)
    pat_cv_host_os_type="NetBSD"
    pat_cv_host_platform_type="Unix" ;;
  *-*-dragonfly*)
    pat_cv_host_os_type="DragonFly"
    pat_cv_host_platform_type="Unix" ;;
  *-*-hpux*)
    pat_cv_host_os_type="HP-UX"
    pat_cv_host_platform_type="Unix" ;;
  *-*-interix*)
    pat_cv_host_os_type="Interix"
    pat_cv_host_platform_type="Unix" ;;
  *-*-linux*)
    pat_cv_host_os_type="Linux"
    pat_cv_host_platform_type="Unix" ;;
  *-*-solaris*)
    pat_cv_host_os_type="SunOS"
    pat_cv_host_platform_type="Unix" ;;
  *-*-auroraux*)
    pat_cv_host_os_type="AuroraUX"
    pat_cv_host_platform_type="Unix" ;;
  *-*-win32*)
    pat_cv_host_os_type="Win32"
    pat_cv_host_platform_type="Win32" ;;
  *-*-mingw*)
    pat_cv_host_os_type="MingW"
    pat_cv_host_platform_type="Win32" ;;
  *-*-haiku*)
    pat_cv_host_os_type="Haiku"
    pat_cv_host_platform_type="Unix" ;;
  *-unknown-eabi*)
    pat_cv_host_os_type="Freestanding"
    pat_cv_host_platform_type="Unix" ;;
  *-unknown-elf*)
    pat_cv_host_os_type="Freestanding"
    pat_cv_host_platform_type="Unix" ;;
  *)
    pat_cv_host_os_type="Unknown"
    pat_cv_host_platform_type="Unknown" ;;
  esac])

dnl Set the "OS" Makefile variable based on the platform type so the
dnl makefile can configure itself to specific build hosts
if test "$pat_cv_host_os_type" = "Unknown" ; then
  AC_MSG_ERROR([Operating system is unknown, configure can't continue])
fi

dnl Set the "PAT_ON_*" variables based on llvm_cv_llvm_cv_platform_type
dnl This is used by lib/Support to determine the basic kind of implementation
dnl to use.
case $pat_cv_host_platform_type in
Unix)
  AC_DEFINE([PAT_ON_UNIX],[1],[Define if this is Unixish platform])
  AC_SUBST(PAT_ON_PLATFORM,[PAT_ON_UNIX])
;;
Win32)
  AC_DEFINE([PAT_ON_WIN32],[1],[Define if this is Win32ish platform])
  AC_SUBST(PAT_ON_PLATFORM,[PAT_ON_WIN32])
;;
esac

AC_CONFIG_HEADERS([utils/pat/include/pat/Config/Config.h])

pat_src_path='${abs_top_srcdir}/'
pat_src_path+=${PAT_PATH}

pat_build_path='${abs_top_builddir}/'
pat_build_path+=${PAT_PATH}

PAT_CFLAGS="-I${pat_src_path}/include -I${pat_build_path}/include"
PAT_CPPFLAGS="-I${pat_src_path}/include -I${pat_build_path}/include"
PAT_LDFLAGS="-L${pat_build_path}/lib -lpat"

AC_SUBST(PAT_CFLAGS)
AC_SUBST(PAT_CPPFLAGS)
AC_SUBST(PAT_LDFLAGS)

])
