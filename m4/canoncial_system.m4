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
;;
Win32)
  AC_DEFINE([SKYPAT_ON_WIN32],[1],[Define if this is Win32ish platform])
  AC_SUBST(SKYPAT_ON_PLATFORM,[SKYPAT_ON_WIN32])
;;
esac

AC_SUBST(HOST_OS,$skypat_cv_host_os_type)

AC_CANONICAL_TARGET
AC_CACHE_CHECK([type of operating system we're going to target],
  [skypat_cv_target_os_type],
  [case $target in
  *-*-aix*)
    skypat_cv_target_os_type="AIX" ;;
  *-*-irix*)
    skypat_cv_target_os_type="IRIX" ;;
  *-*-cygwin*)
    skypat_cv_target_os_type="Cygwin" ;;
  *-*-darwin*)
  skypat_cv_target_os_type="Darwin" ;;
  *-*-minix*)
    skypat_cv_target_os_type="Minix" ;;
  *-*-freebsd* | *-*-kfreebsd-gnu)
    skypat_cv_target_os_type="FreeBSD" ;;
  *-*-openbsd*)
    skypat_cv_target_os_type="OpenBSD" ;;
  *-*-netbsd*)
    skypat_cv_target_os_type="NetBSD" ;;
  *-*-dragonfly*)
    skypat_cv_target_os_type="DragonFly" ;;
  *-*-hpux*)
    skypat_cv_target_os_type="HP-UX" ;;
  *-*-interix*)
    skypat_cv_target_os_type="Interix" ;;
  *-*-linux*)
    skypat_cv_target_os_type="Linux" ;;
  *-*-gnu*)
    skypat_cv_target_os_type="GNU" ;;
  *-*-solaris*)
    skypat_cv_target_os_type="SunOS" ;;
  *-*-auroraux*)
    skypat_cv_target_os_type="AuroraUX" ;;
  *-*-win32*)
    skypat_cv_target_os_type="Win32" ;;
  *-*-mingw*)
    skypat_cv_target_os_type="MingW" ;;
  *-*-haiku*)
    skypat_cv_target_os_type="Haiku" ;;
  *-*-rtems*)
    skypat_cv_target_os_type="RTEMS" ;;
  *-*-nacl*)
    skypat_cv_target_os_type="NativeClient" ;;
  *-unknown-eabi*)
    skypat_cv_target_os_type="Freestanding" ;;
  *)
    skypat_cv_target_os_type="Unknown" ;;
  esac])

if test "$skypat_cv_target_os_type" = "Unknown" ; then
  AC_MSG_WARN([Configuring for an unknown target operating system])
fi

AC_SUBST(TARGET_OS,$skypat_cv_target_os_type)

AC_CACHE_CHECK([target architecture],
  [skypat_cv_target_arch_type],
  [case $target in
  i?86-*)                 skypat_cv_target_arch_type="x86" ;;
  amd64-* | x86_64-*)     skypat_cv_target_arch_type="x86_64" ;;
  sparc*-*)               skypat_cv_target_arch_type="Sparc" ;;
  powerpc*-*)             skypat_cv_target_arch_type="PowerPC" ;;
  arm*-*)                 skypat_cv_target_arch_type="ARM" ;;
  aarch64*-*)             skypat_cv_target_arch_type="AArch64" ;;
  mips-* | mips64-*)      skypat_cv_target_arch_type="Mips" ;;
  mipsel-* | mips64el-*)  skypat_cv_target_arch_type="Mips" ;;
  xcore-*)                skypat_cv_target_arch_type="XCore" ;;
  msp430-*)               skypat_cv_target_arch_type="MSP430" ;;
  hexagon-*)              skypat_cv_target_arch_type="Hexagon" ;;
  mblaze-*)               skypat_cv_target_arch_type="MBlaze" ;;
  nvptx-*)                skypat_cv_target_arch_type="NVPTX" ;;
  *)                      skypat_cv_target_arch_type="Unknown" ;;
  esac])

if test "$skypat_cv_target_arch_type" = "Unknown" ; then
  AC_MSG_WARN([Configuring for an unknown target archicture])
fi

AC_SUBST(TARGET_ARCH,$skypat_cv_target_arch_type)

dnl Set the "SKYPAT_DEFAULT_TARGET_TRIPLE" variable based on $target.
dnl This is used to determine the default target triple and emulation
dnl to use.
AC_DEFINE([SKYPAT_DEFAULT_TARGET_TRIPLE],
          [$target],
          [default target triple])

])
