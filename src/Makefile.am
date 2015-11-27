#
# Automake definitions for src/
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

AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = 			\
	mkcg.6416			\
	mkcg.vid2k

## TODO:9000 provide libmkcg as public library after refactoring
noinst_LTLIBRARIES = libmkcg.la
bin_PROGRAMS = mkcg.6416 mkcg.vid2k

mkcg_6416_SOURCES = mkcg.6416.c
mkcg_6416_LDADD = libmkcg.la
mkcg_6416_CFLAGS = $(AM_CFLAGS) @XPM_CFLAGS@
mkcg_6416_LDFLAGS = $(AM_LDFLAGS) @XPM_LIBS@

mkcg_vid2k_SOURCES = mkcg.vid2k.c
mkcg_vid2k_LDADD = libmkcg.la
mkcg_vid2k_CFLAGS = $(AM_CFLAGS) @XPM_CFLAGS@
mkcg_vid2k_LDFLAGS = $(AM_LDFLAGS) @XPM_LIBS@

libmkcg_la_SOURCES = \
	mkcg.c

noinst_HEADERS = \
	pcmtools.h

MAINTAINERCLEANFILES = $(srcdir)/Makefile.in
