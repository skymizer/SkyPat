dnl
dnl @synopsis CHECK_ENABLE_OPTIMIZE
dnl
dnl @summary check --enable-optimize
dnl
dnl Luba Tang <lubatang@gmail.com>

AC_DEFUN([CHECK_ENABLE_OPTIMIZE],
[dnl
  AC_ARG_ENABLE(optimize,
    [AS_HELP_STRING([--enable-optimize],
        [turn on unit-tests (default is no)])],
        [case "${enableval}" in
          yes) optimize=true ;;
          no)  optimize=false ;;
          *) AC_MSG_ERROR(bad value ${enableval} for --enable-optimize) ;;
         esac],
        [optimize=false])

  AM_CONDITIONAL([ENABLE_OPTIMIZE],[test "x${optimize}" = "xtrue" ])
])
