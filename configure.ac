#
# Autoconf definitions for top source directory
#
# Copyright (C) 2002-2020  Stephan Linz <linz@li-pro.net>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA  02110-1301, USA.
#

# use autogen.sh

dnl *************************************************************************
dnl *** Configure autoconf
dnl *************************************************************************
AC_PREREQ([2.69])
AC_INIT([PC/M Tools],
	[m4_esyscmd_s([./acx/get-version])],
	[pcmtools-bug@li-pro.net],
	[pcmtools],
	[https://github.com/lipro-cpm4l/pcmtools/])

AC_CONFIG_AUX_DIR([acx])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/mkcg_cli.c])
AC_CONFIG_HEADERS([config.h:config.in])dnl Keep filename to 8.3 for MS-DOS.

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

AC_USE_SYSTEM_EXTENSIONS

dnl *************************************************************************
dnl *** Configure automake
dnl *************************************************************************
AM_INIT_AUTOMAKE([
	1.14
	-Wall
	-Wno-portability
	dist-bzip2
	dist-zip
	dist-xz
	subdir-objects
])
AM_MAINTAINER_MODE
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES(yes)])
m4_pattern_allow([AM_PROG_AR])
AM_PROG_AR

dnl *************************************************************************
dnl *** Configure libtool
dnl *************************************************************************
LT_PREREQ([2.2])
LT_INIT

dnl *************************************************************************
dnl *** Checks for compiler.
dnl *************************************************************************
AC_LANG([C])
AC_PROG_CC
AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_REQUIRE_CPP

dnl *************************************************************************
dnl *** Checks for programs.
dnl *************************************************************************
PKG_PROG_PKG_CONFIG
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AC_PROG_LN_S
AC_PROG_EGREP

dnl GNU help2man creates man pages from --help output; in many cases, this
dnl is sufficient, and obviates the need to maintain man pages separately.
AC_PROG_HELP2MAN

dnl However, this means invoking executables, which we generally cannot do
dnl when cross-compiling, so we test to avoid that (the variable
dnl "cross_compiling" is set by AC_PROG_CC).
AS_IF([test $cross_compiling = yes],[HELP2MAN=:])

dnl *************************************************************************
dnl *** Check some preprocessor flags.
dnl *************************************************************************

# Enable _GNU_SOURCE and the like
AX_APPEND_COMPILE_FLAGS([-D_GNU_SOURCE],[SYSTEM_INTERFACE_CPPFLAGS]) dnl GNU systems (asprintf, ...)
case $host_os in
  solaris*)	dnl (CMSG_*)
    AX_APPEND_COMPILE_FLAGS([-D__EXTENSIONS__],[SYSTEM_INTERFACE_CPPFLAGS])
    AX_APPEND_COMPILE_FLAGS([-D_XPG4_2],[SYSTEM_INTERFACE_CPPFLAGS])
    ;;
  hpux*)	dnl HP-UX
    AX_APPEND_COMPILE_FLAGS([-D_XOPEN_SOURCE=500],[SYSTEM_INTERFACE_CPPFLAGS])
    AX_APPEND_COMPILE_FLAGS([-D_XOPEN_SOURCE_EXTENDED],[SYSTEM_INTERFACE_CPPFLAGS])
    ;;
esac

# FORTIFY_SOURCE Semantics
# http://tk-blog.blogspot.de/2011/01/checksecsh-now-with-fortifysource.html
# http://blog.isis.poly.edu/code%20auditing/compilers/exploitation%20mitigation%20techniques/2011/04/11/fortify_source-semantics.html

# https://wiki.debian.org/Hardening#DEB_BUILD_HARDENING_FORTIFY_.28gcc.2Fg.2B-.2B-_-D_FORTIFY_SOURCE.3D2.29
# https://wiki.debian.org/Hardening#gcc_-D_FORTIFY_SOURCE.3D2_-O1
AX_APPEND_COMPILE_FLAGS([-D_FORTIFY_SOURCE=2],[HARDENING_FORTIFY_CPPFLAGS])

dnl *************************************************************************
dnl *** Check some compiler flags.
dnl *************************************************************************

# Use pipes rather than temporary files for communication between the various
# stages of compilation. This fails to work on some systems where the assembler
# is unable to read from a pipe; but the GNU assembler has no trouble.
AX_APPEND_COMPILE_FLAGS([-pipe],[GCC2AS_PIPE_CFLAGS])

# https://wiki.debian.org/Hardening#DEB_BUILD_HARDENING_STACKPROTECTOR_.28gcc.2Fg.2B-.2B-_-fstack-protector-strong.29
# https://wiki.debian.org/Hardening#Stack_Protector
AX_APPEND_COMPILE_FLAGS([-fstack-protector],[HARDENING_STACK_CFLAGS])

# https://wiki.debian.org/Hardening#DEB_BUILD_HARDENING_FORMAT_.28gcc.2Fg.2B-.2B-_-Wformat_-Wformat-security_-Werror.3Dformat-security.29
# https://wiki.debian.org/Hardening#gcc_-Wformat_-Wformat-security
AX_APPEND_COMPILE_FLAGS([-Wformat],[HARDENING_FORMAT_CFLAGS])
AX_APPEND_COMPILE_FLAGS([-Wformat-security],[HARDENING_FORMAT_CFLAGS])

# displays the option used to switch off a warning
AX_APPEND_COMPILE_FLAGS([-fdiagnostics-show-option],[WARN_SHOWOPT_CFLAGS])

# list of warnings
AX_APPEND_COMPILE_FLAGS([-W],[WARN_ALL_CFLAGS])
AX_APPEND_COMPILE_FLAGS([-Wall],[WARN_ALL_CFLAGS])
AX_APPEND_COMPILE_FLAGS([-Wextra],[WARN_EXTRA_CFLAGS])
AX_APPEND_COMPILE_FLAGS([-Wfatal-errors],[WARN_FATALERR_CFLAGS])
AX_APPEND_COMPILE_FLAGS([-Winline],[WARN_INLINE_CFLAGS])
AX_APPEND_COMPILE_FLAGS([-Wcast-qual],[WARN_CAST_CFLAGS])
AX_APPEND_COMPILE_FLAGS([-Wcast-align],[WARN_CAST_CFLAGS])
AX_APPEND_COMPILE_FLAGS([-Wno-unused-parameter],[WARN_NO_CFLAGS])
AX_APPEND_COMPILE_FLAGS([-Wno-missing-field-initializers],[WARN_NO_CFLAGS])

dnl *************************************************************************
dnl *** Check some linker flags.
dnl *************************************************************************

# RELRO - Memory Corruption Mitigation Technique
# http://tk-blog.blogspot.de/2009/02/relro-not-so-well-known-memory.html

# https://wiki.debian.org/Hardening#DEB_BUILD_HARDENING_RELRO_.28ld_-z_relro.29
# https://wiki.debian.org/Hardening#ld_-z_relro
AX_APPEND_LINK_FLAGS([-Wl,-z,relro],[HARDENING_RELRO_LDFLAGS])

# https://wiki.debian.org/Hardening#DEB_BUILD_HARDENING_BINDNOW_.28ld_-z_now.29
# https://wiki.debian.org/Hardening#ld_-z_now
AX_APPEND_LINK_FLAGS([-Wl,-z,now],[HARDENING_BINDNOW_LDFLAGS])

dnl *************************************************************************
dnl *** Checks for typedefs, structures, and compiler characteristics.
dnl *************************************************************************
AC_C_CONST
AC_C_VOLATILE
AC_C_INLINE

AC_CACHE_SAVE

dnl *************************************************************************
dnl *** Checks for header files.
dnl *************************************************************************
AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_CHECK_HEADERS([getopt.h])

dnl *************************************************************************
dnl *** Checks for library functions.
dnl *************************************************************************
AC_CHECK_FUNCS([strcmp strtoul])

dnl *************************************************************************
dnl *** Checks for libraries.
dnl *************************************************************************
PKG_CHECK_MODULES([XPM], [xpm >= 3.5],[
  AX_SAVE_FLAGS([XPM])
  CFLAGS="$CFLAGS $XPM_CFLAGS"
  LIBS="$LIBS $XPM_LIBS"
  AX_APPEND_COMPILE_FLAGS([${XPM_CFLAGS}],[XPM_CFLAGS])
  AC_CHECK_HEADERS([X11/xpm.h],,[
    AC_MSG_ERROR([Could not find Xpm header])
  ],)
  AX_APPEND_LINK_FLAGS([${XPM_LIBS}],[XPM_LIBS])
  AC_CHECK_LIB([Xpm],[XpmReadFileToXpmImage],,[
    AC_MSG_ERROR([Could not find Xpm library])
  ],)
  AX_RESTORE_FLAGS([XPM])
  HAVE_XPM="yes"
],[
  AC_MSG_ERROR([Could not find Xpm package])
])

AC_CACHE_SAVE

dnl *************************************************************************
dnl *** Collect configuration.
dnl *************************************************************************
AS_AC_EXPAND([SYSCONFDIR],[${sysconfdir}])
AS_AC_EXPAND([BINDIR],[${bindir}])
AS_AC_EXPAND([DATADIR],[${datarootdir}])

pkgdatadir="${DATADIR}/${PACKAGE}"
AC_SUBST([pkgdatadir])

AS_AC_EXPAND([DOCDIR],[${docdir}])
AC_SUBST([DOCDIR])

sys_cppflags="${CPPFLAGS}"

AX_APPEND_FLAG([${HARDENING_FORTIFY_CPPFLAGS}],[CPPFLAGS])
AC_SUBST([HARDENING_FORTIFY_CPPFLAGS])

AX_APPEND_FLAG([${SYSTEM_INTERFACE_CPPFLAGS}],[CPPFLAGS])
AC_SUBST([SYSTEM_INTERFACE_CPPFLAGS])

sys_cflags="${CFLAGS}"

AX_APPEND_FLAG([${GCC2AS_PIPE_CFLAGS}],[CFLAGS])
AC_SUBST([GCC2AS_PIPE_CFLAGS])

AX_APPEND_FLAG([${HARDENING_STACK_CFLAGS}],[CFLAGS])
AC_SUBST([HARDENING_STACK_CFLAGS])

AX_APPEND_FLAG([${HARDENING_FORMAT_CFLAGS}],[CFLAGS])
AC_SUBST([HARDENING_FORMAT_CFLAGS])

AX_APPEND_FLAG([${WARN_SHOWOPT_CFLAGS}],[CFLAGS])
AC_SUBST([WARN_SHOWOPT_CFLAGS])

AX_APPEND_FLAG([${WARN_ALL_CFLAGS}],[CFLAGS])
AC_SUBST([WARN_ALL_CFLAGS])

AX_APPEND_FLAG([${WARN_EXTRA_CFLAGS}],[CFLAGS])
AC_SUBST([WARN_EXTRA_CFLAGS])

AX_APPEND_FLAG([${WARN_FATALERR_CFLAGS}],[CFLAGS])
AC_SUBST([WARN_FATALERR_CFLAGS])

AX_APPEND_FLAG([${WARN_INLINE_CFLAGS}],[CFLAGS])
AC_SUBST([WARN_INLINE_CFLAGS])

AX_APPEND_FLAG([${WARN_CAST_CFLAGS}],[CFLAGS])
AC_SUBST([WARN_CAST_CFLAGS])

AX_APPEND_FLAG([${WARN_NO_CFLAGS}],[CFLAGS])
AC_SUBST([WARN_NO_CFLAGS])

sys_ldflags="${LDFLAGS}"

AX_APPEND_FLAG([${HARDENING_RELRO_LDFLAGS}],[LDFLAGS])
AC_SUBST([HARDENING_RELRO_LDFLAGS])

AX_APPEND_FLAG([${HARDENING_BINDNOW_LDFLAGS}],[LDFLAGS])
AC_SUBST([HARDENING_BINDNOW_LDFLAGS])

sys_libs="${LIBS}"

AC_CACHE_SAVE

dnl Copyright will apply as long as these sources are in use, e.g., are
dnl being compiled, which is reasonable year to claim the copyright.
AC_DEFINE([COPYRIGHT_YEAR], [m4_esyscmd([date +%Y])], [year in copyright message])

dnl Are we building from git checked-out sources, or a tarball ?
dnl This is used in "Makefile.am" to avoid re-generating distfiles
AM_CONDITIONAL([BUILD_FROM_GIT], [test -e "$srcdir/.git"])

dnl This is used in "Makefile.am" to avoid execute cross compiled
dnl binaries where re-generating distfiles
AM_CONDITIONAL([CROSS_COMPILING_FROM_GIT],
               [test -e "$srcdir/.git" && test $cross_compiling = yes])

AC_CONFIG_FILES([
	Makefile
	data/Makefile
	man/Makefile
	src/Makefile
	src/smoke/Makefile
])
AC_OUTPUT

dnl *************************************************************************
dnl *** Show configuration summary.
dnl *************************************************************************
AC_MSG_NOTICE([

---------------------------------- Summary ----------------------------------
 ${PACKAGE_NAME} version ${PACKAGE_VERSION}

 Configuration summary:

  X11 Pixmap Format: libXpm: .......... ${HAVE_XPM}

 Installation directories:

  Prefix: .............. ${prefix}
  Programs: ............ ${BINDIR}
  Data: ................ ${pkgdatadir}
  Documents: ........... ${DOCDIR}

 Used tools:

  C Preprocessor: ...... ${CPP}
    Flags:       ${sys_cppflags}
      System IF: ${SYSTEM_INTERFACE_CPPFLAGS}
      Hardening: ${HARDENING_FORTIFY_CPPFLAGS}

  C Compiler: .......... ${CC}
    Flags:       ${sys_cflags} ${GCC2AS_PIPE_CFLAGS}
      Hardening: ${HARDENING_STACK_CFLAGS} ${HARDENING_FORMAT_CFLAGS}
      Warning:   ${WARN_SHOWOPT_CFLAGS} ${WARN_ALL_CFLAGS}
                 ${WARN_EXTRA_CFLAGS} ${WARN_FATALERR_CFLAGS}
                 ${WARN_INLINE_CFLAGS} ${WARN_CAST_CFLAGS}
                 ${WARN_NO_CFLAGS}
      libXpm:    ${XPM_CFLAGS}

  Linker: .............. ${LD}
    Flags:       ${sys_ldflags}
      Hardening: ${HARDENING_RELRO_LDFLAGS} ${HARDENING_BINDNOW_LDFLAGS}
    Libs:        ${sys_libs}
      libXpm:    ${XPM_LIBS}
-----------------------------------------------------------------------------

Check the above options and compile with:
 ${MAKE-make}

])
