dnl ac-prog-html2man.m4 0.2.0                               -*- autoconf -*-
dnl m4 macro to find help2man program

dnl Copyright (C) 2015-2020  Stephan Linz <linz@li-pro.net>

dnl Copying and distribution of this file, with or without modification,
dnl are permitted in any medium without royalty provided the copyright
dnl notice and this notice are preserved.

# AC_PROG_HELP2MAN
# ----------------
AN_MAKEVAR([HELP2MAN],       [AC_PROG_HELP2MAN])
AN_MAKEVAR([HELP2MANFLAGS],  [AC_PROG_HELP2MAN])
AN_PROGRAM([help2man],       [AC_PROG_HELP2MAN])
AC_DEFUN([AC_PROG_HELP2MAN],
[AC_CHECK_PROGS(HELP2MAN, help2man, :)dnl
AC_ARG_VAR(HELP2MAN,
[The `Simple Manual Page Generator' implementation to use.  Defaults to
the first program found out of: `help2man'.])dnl
AC_ARG_VAR(HELP2MANFLAGS,
[The list of arguments that will be passed by default to $HELP2MAN.  This script
will default HELP2MANFLAGS to the empty string to avoid a default value of `-N'
given by some make applications.])])
