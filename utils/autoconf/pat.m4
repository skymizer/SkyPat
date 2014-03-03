dnl
dnl @synopsis CHECK_PAT
dnl
dnl @summary check "pat" library, and set up variables:
dnl   PAT_CFLAGS="-I${pat_path}/include"
dnl   PAT_CPPFLAGS="-I${pat_path}/include"
dnl   PAT_LDFLAGS="-L${pat_path}/lib -lpat"
dnl
dnl Luba Tang <luba@skymizer.com>
dnl

AC_DEFUN([CHECK_PAT],
[dnl
AC_ARG_WITH([pat],
            [AS_HELP_STRING([--with-pat[[=PATH]]],
              [set up the path of pat library])],
            [pat_path="${withval}"],
            [pat_path="${prefix}"])

# Check for pat.
AC_MSG_CHECKING(for "pat" library)

HAVE_PAT=0

AC_LANG_PUSH([C++])
org_CXXFLAGS="${CXXFLAGS}"
CXXFLAGS="-I${pat_path}/include"

AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([[
      #include <pat/pat.h>
      #if !defined(PAT_VERNUM) || (PAT_VERNUM < 0x22)
      #error pat version too old
      #endif
  ]], [])],
  [dnl action-if-true
      AC_MSG_RESULT([ok (2.2 or later)])
  ], dnl action-if-true
  [ dnl action-if-false
      AS_IF([test "$pat_path" = "${prefix}" ],
        [AC_MSG_FAILURE([headers missing or too old (requires >= 2.2)])],
        [AC_MSG_FAILURE([--with-pat was given, but pat headers were not present or were too old (requires >= 2.2)])]
      )
])

CXXFLAGS="${org_CXXFLAGS}"
AC_LANG_POP([C++])

PAT_CFLAGS="-I${pat_path}/include"
PAT_CPPFLAGS="-I${pat_path}/include"
PAT_LDFLAGS="-L${pat_path}/lib -lpat"

AC_SUBST(PAT_CFLAGS)
AC_SUBST(PAT_CPPFLAGS)
AC_SUBST(PAT_LDFLAGS)

AM_CONDITIONAL([HAVE_PAT], [test $HAVE_PAT = 1])
])
