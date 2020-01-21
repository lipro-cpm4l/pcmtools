#
# Automake definitions for src/smoke/
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

EXTRA_DIST =
CLEANFILES =

AUTOMAKE_OPTIONS = dejagnu

EXTRA_DIST +=							\
	mkcg/6416.exp						\
	data/6416_30.xpm					\
	mkcg/vid2k.exp						\
	data/vid2k_30.xpm					\
	config/default.exp

DEJATOOL = mkcg
RUNTESTFLAGS =							\
	--srcdir "$(abs_srcdir)"				\
	--objdir "$(abs_top_builddir)/src"			\
	--status --all --verbose

MAINTAINERCLEANFILES = $(srcdir)/Makefile.in
