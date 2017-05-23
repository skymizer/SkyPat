dnl
dnl @synopsis CHECK_SKYPAT
dnl
dnl @summary check "skypat" library, and set up variables:
dnl   SKYPAT_CFLAGS="-I${skypat_path}/include"
dnl   SKYPAT_CPPFLAGS="-I${skypat_path}/include"
dnl   SKYPAT_LDFLAGS="-L${skypat_path}/lib -lskypat"
dnl
dnl This macro is used to check if skypat is already installed in the system. If
dnl you want to integrate skypat in your own project, please use SETUP_SKYPAT.
dnl
dnl Luba Tang <luba@skymizer.com>
dnl

AC_DEFUN([CHECK_SKYPAT],
[dnl
AC_ARG_WITH([skypat],
            [AS_HELP_STRING([--with-skypat[[=PATH]]],
              [set up the path of skypat library])],
            [skypat_path="${withval}"],
            [skypat_path="${prefix}"])

# Check for skypat.
AC_MSG_CHECKING(for "skypat" library)

HAVE_SKYPAT=0

AC_LANG_PUSH([C++])
org_CXXFLAGS="${CXXFLAGS}"
CXXFLAGS="-I${skypat_path}/include"

AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([[
      #include <skypat/skypat.h>
      #if !defined(SKYPAT_VERNUM) || (SKYPAT_VERNUM < 0x22)
      #error skypat version too old
      #endif
  ]], [])],
  [dnl action-if-true
      AC_MSG_RESULT([ok (2.2 or later)])
  ], dnl action-if-true
  [ dnl action-if-false
      AS_IF([test "$skypat_path" = "${prefix}" ],
        [AC_MSG_FAILURE([headers missing or too old (requires >= 2.2)])],
        [AC_MSG_FAILURE([--with-skypat was given, but skypat headers were not present or were too old (requires >= 2.2)])]
      )
])

CXXFLAGS="${org_CXXFLAGS}"
AC_LANG_POP([C++])

SKYPAT_CFLAGS="-I${skypat_path}/include"
SKYPAT_CPPFLAGS="-I${skypat_path}/include"
SKYPAT_LDFLAGS="-L${skypat_path}/lib -lskypat"

AC_SUBST(SKYPAT_CFLAGS)
AC_SUBST(SKYPAT_CPPFLAGS)
AC_SUBST(SKYPAT_LDFLAGS)

AM_CONDITIONAL([HAVE_SKYPAT], [test $HAVE_SKYPAT = 1])
])
