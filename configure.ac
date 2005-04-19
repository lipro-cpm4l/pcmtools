PAK="pcmtools"
VER="20050419"
AC_PREREQ(2.59)
AC_INIT(pcmtools, 20050419, linz@li-pro.net)
AM_INIT_AUTOMAKE(pcmtools, 20050419)
AM_CONFIG_HEADER([config.h])

TMP=/usr
AC_MSG_CHECKING([for X11 installation prefix])
AC_ARG_WITH(X11,
  [  --with-X11=<path>       prefix of X11 installation. [/usr]],
  [X11_PREFIX=$with_X11] AC_MSG_RESULT([$with_X11]),
  AC_MSG_RESULT([$TMP (default)])
  AC_MSG_WARN([You should call configure with the --with-X11 option.
  This tells configure where to find the X11 libraries and headers.
  e.g. --with-X11=/usr or --with-X11=/usr/local])
)

if test -z "$with_X11" -o "$with_X11" = yes
then
  X11_PREFIX=$TMP
fi

AC_SUBST(X11_PREFIX)
X11_LIBS="-L${X11_PREFIX}/X11R6/lib"
X11_CFLAGS="-I${X11_PREFIX}/include/X11"
AC_SUBST(X11_LIBS)
AC_SUBST(X11_CFLAGS)

DATA_INST_DIR='$(DESTDIR)$(datadir)/pcmtools'
AC_SUBST(DATA_INST_DIR)

AC_GNU_SOURCE

AC_PROG_CC
AM_PROG_CC_STDC
AC_PROG_CPP
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AC_PROG_LN_S
AC_PROG_RANLIB

if test "$GCC" = yes
then
  CFLAGS="${CFLAGS} ${PIPE}-Wmissing-prototypes -Wstrict-prototypes -Wcast-qual -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-declarations -Wnested-externs -fno-common"
  LDFLAGS="-g ${X11_LIBS}"
fi

dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_CHECK_FUNCS(strcmp strtoul)
AC_CHECK_HEADERS(unistd.h getopt.h X11/xpm.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_VOLATILE
AC_C_INLINE

dnl Checks for library functions.

dnl Checks for libraries.
AC_CHECK_LIB(X11, XCreateWindow,,AC_MSG_ERROR([Cannot find correct X11 library]),)
AC_CHECK_LIB(Xpm, XpmReadFileToXpmImage,,AC_MSG_ERROR([Cannot find correct Xpm library]),)

AC_OUTPUT(Makefile data/Makefile)
