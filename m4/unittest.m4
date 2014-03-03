dnl
dnl @synopsis CHECK_ENABLE_UNITTEST
dnl
dnl @summary enable unittest
dnl
dnl Luba Tang <lubatang@gmail.com>

AC_DEFUN([CHECK_ENABLE_UNITTEST],
[dnl
  AC_ARG_ENABLE(unittest,
    [AS_HELP_STRING([--enable-unittest],
        [turn on unit-tests (default is no)])],
        [case "${enableval}" in
          yes) unittest=true ;;
          no)  unittest=false ;;
          *) AC_MSG_ERROR(bad value ${enableval} for --enable-unittest) ;;
         esac],
        [unittest=false])

  AM_CONDITIONAL([ENABLE_UNITTEST],[test "x${unittest}" = "xtrue" ])
])
