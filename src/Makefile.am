#
# Automake definitions for src/
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

AM_CPPFLAGS = -I$(top_srcdir)/src

EXTRA_DIST =
CLEANFILES =

SUBDIRS = . smoke

check_PROGRAMS = mkcg.6416 mkcg.vid2k

## TODO:9000 provide libmkcg as public library after refactoring
noinst_LTLIBRARIES = libmkcg.la
bin_PROGRAMS = mkcg.6416 mkcg.vid2k

mkcg_6416_SOURCES = mkcg_cmd_6416.c
mkcg_6416_LDADD = libmkcg.la

mkcg_vid2k_SOURCES = mkcg_cmd_vid2k.c
mkcg_vid2k_LDADD = libmkcg.la

libmkcg_la_SOURCES =			\
	mkcg_isvalidch.c		\
	mkcg_out_banner.c		\
	mkcg_out_xxd.c			\
	mkcg_out_xpm.c			\
	mkcg_cli.c
libmkcg_la_CFLAGS = $(AM_CFLAGS) @XPM_CFLAGS@
libmkcg_la_LDFLAGS = $(AM_LDFLAGS) @XPM_LIBS@

noinst_HEADERS = \
	pcmtools.h

MAINTAINERCLEANFILES = $(srcdir)/Makefile.in
