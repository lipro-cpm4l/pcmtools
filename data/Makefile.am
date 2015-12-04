#
# Automake definitions for data/
#
# Copyright (C) 2002-2015  Stephan Linz <linz@li-pro.net>
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
EXTRA_DIST +=				\
	CG_6416				\
	CG_VID2K

pkgdatadir = @pkgdatadir@

install-data-local:
	$(AM_V_GEN)for dir in `cd $(top_srcdir)/data; \
	                       find $(EXTRA_DIST) -type d` ; do \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/$$dir; \
	done
	$(AM_V_GEN)for file in `cd $(top_srcdir)/data; \
	                        find $(EXTRA_DIST) -type f` ; do \
	  $(INSTALL_DATA) $(top_srcdir)/data/$$file \
                          $(DESTDIR)$(pkgdatadir)/$$file; \
	done

uninstall-local:
	rm -rf $(DESTDIR)$(pkgdatadir)

MAINTAINERCLEANFILES = $(srcdir)/Makefile.in
