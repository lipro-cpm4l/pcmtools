#
# Autoconf definitions for top source directory
#
# Copyright (C) 2002-21015  Stephan Linz <linz@li-pro.net>
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

AC_PREREQ([2.64])
AC_INIT([PC/M Tools],
	[2015-devel],
	[pcmtools-bug@li-pro.net],
	[pcmtools],
	[https://github.com/lipro/pcmtools/])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

AC_CONFIG_SRCDIR([src/mkcg.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wall -Wno-portability dist-bzip2 dist-zip subdir-objects])
AM_MAINTAINER_MODE

AC_USE_SYSTEM_EXTENSIONS

AC_LANG([C])
AC_PROG_CC
AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_REQUIRE_CPP

LT_PREREQ([2.2])
LT_INIT

AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP

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

DATA_INST_DIR='$(DESTDIR)$(datadir)/pcmtools'
AC_SUBST(DATA_INST_DIR)

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	data/Makefile
])
AC_OUTPUT
