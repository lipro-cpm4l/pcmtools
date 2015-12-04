#
# Automake definitions for man/
#
# Copyright (C) 2015  Stephan Linz <linz@li-pro.net>
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

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST =
CLEANFILES =

EXTRA_DIST +=				\
	mkcg.6416.x			\
	mkcg.vid2k.x

if !CROSS_COMPILING_FROM_GIT
man_MANS = mkcg.6416.1 mkcg.vid2k.1
EXTRA_DIST += $(man_MANS)
endif

if BUILD_FROM_GIT
HELP2MANFLAGS += -N

$(top_builddir)/src/mkcg.%$(EXEEXT): 	$(top_srcdir)/src/mkcg_cmd_%.c \
					$(top_srcdir)/src/mkcg_cli.c \
					$(top_srcdir)/configure.ac
	$(AM_V_GEN)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

mkcg.%.1: $(top_builddir)/src/mkcg.%$(EXEEXT) mkcg.%.x
	$(AM_V_GEN)$(HELP2MAN) $(HELP2MANFLAGS) \
	  -i $(srcdir)/$(patsubst %$(EXEEXT),%,$(<F)).x \
	  $(top_builddir)/src/$(<F) -o $@

CLEANFILES += $(man_MANS)
endif

MAINTAINERCLEANFILES = $(srcdir)/Makefile.in
