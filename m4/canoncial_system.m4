dnl
dnl @synopsis CHECK_CANONICAL_SYSTEM
dnl
dnl @summary set up default value of `host`, `build` and `target`
dnl
dnl Luba Tang <lubatang@gmail.com>

AC_DEFUN([CHECK_CANONICAL_SYSTEM],
[dnl

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
  AC_DEFINE([pat_ON_UNIX],[1],[Define if this is Unixish platform])
  AC_SUBST(pat_ON_PLATFORM,[PAT_ON_UNIX])
;;
Win32)
  AC_DEFINE([PAT_ON_WIN32],[1],[Define if this is Win32ish platform])
  AC_SUBST(PAT_ON_PLATFORM,[PAT_ON_WIN32])
;;
esac

AC_SUBST(HOST_OS,$pat_cv_host_os_type)

AC_CANONICAL_TARGET
AC_CACHE_CHECK([type of operating system we're going to target],
  [pat_cv_target_os_type],
  [case $target in
  *-*-aix*)
    pat_cv_target_os_type="AIX" ;;
  *-*-irix*)
    pat_cv_target_os_type="IRIX" ;;
  *-*-cygwin*)
    pat_cv_target_os_type="Cygwin" ;;
  *-*-darwin*)
  pat_cv_target_os_type="Darwin" ;;
  *-*-minix*)
    pat_cv_target_os_type="Minix" ;;
  *-*-freebsd* | *-*-kfreebsd-gnu)
    pat_cv_target_os_type="FreeBSD" ;;
  *-*-openbsd*)
    pat_cv_target_os_type="OpenBSD" ;;
  *-*-netbsd*)
    pat_cv_target_os_type="NetBSD" ;;
  *-*-dragonfly*)
    pat_cv_target_os_type="DragonFly" ;;
  *-*-hpux*)
    pat_cv_target_os_type="HP-UX" ;;
  *-*-interix*)
    pat_cv_target_os_type="Interix" ;;
  *-*-linux*)
    pat_cv_target_os_type="Linux" ;;
  *-*-gnu*)
    pat_cv_target_os_type="GNU" ;;
  *-*-solaris*)
    pat_cv_target_os_type="SunOS" ;;
  *-*-auroraux*)
    pat_cv_target_os_type="AuroraUX" ;;
  *-*-win32*)
    pat_cv_target_os_type="Win32" ;;
  *-*-mingw*)
    pat_cv_target_os_type="MingW" ;;
  *-*-haiku*)
    pat_cv_target_os_type="Haiku" ;;
  *-*-rtems*)
    pat_cv_target_os_type="RTEMS" ;;
  *-*-nacl*)
    pat_cv_target_os_type="NativeClient" ;;
  *-unknown-eabi*)
    pat_cv_target_os_type="Freestanding" ;;
  *)
    pat_cv_target_os_type="Unknown" ;;
  esac])

if test "$pat_cv_target_os_type" = "Unknown" ; then
  AC_MSG_WARN([Configuring for an unknown target operating system])
fi

AC_SUBST(TARGET_OS,$pat_cv_target_os_type)

AC_CACHE_CHECK([target architecture],
  [pat_cv_target_arch_type],
  [case $target in
  i?86-*)                 pat_cv_target_arch_type="x86" ;;
  amd64-* | x86_64-*)     pat_cv_target_arch_type="x86_64" ;;
  sparc*-*)               pat_cv_target_arch_type="Sparc" ;;
  powerpc*-*)             pat_cv_target_arch_type="PowerPC" ;;
  arm*-*)                 pat_cv_target_arch_type="ARM" ;;
  aarch64*-*)             pat_cv_target_arch_type="AArch64" ;;
  mips-* | mips64-*)      pat_cv_target_arch_type="Mips" ;;
  mipsel-* | mips64el-*)  pat_cv_target_arch_type="Mips" ;;
  xcore-*)                pat_cv_target_arch_type="XCore" ;;
  msp430-*)               pat_cv_target_arch_type="MSP430" ;;
  hexagon-*)              pat_cv_target_arch_type="Hexagon" ;;
  mblaze-*)               pat_cv_target_arch_type="MBlaze" ;;
  nvptx-*)                pat_cv_target_arch_type="NVPTX" ;;
  *)                      pat_cv_target_arch_type="Unknown" ;;
  esac])

if test "$pat_cv_target_arch_type" = "Unknown" ; then
  AC_MSG_WARN([Configuring for an unknown target archicture])
fi

AC_SUBST(TARGET_ARCH,$pat_cv_target_arch_type)

dnl Set the "PAT_DEFAULT_TARGET_TRIPLE" variable based on $target.
dnl This is used to determine the default target triple and emulation
dnl to use.
AC_DEFINE([PAT_DEFAULT_TARGET_TRIPLE],
          [$target],
          [default target triple])

])
