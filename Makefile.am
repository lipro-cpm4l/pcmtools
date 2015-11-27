#
# Automake definitions for top source directory
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

ACLOCAL_AMFLAGS = -I m4

AUTOMAKE_OPTIONS = dejagnu

SUBDIRS = src data

EXTRA_DIST =
EXTRA_DIST +=				\
	pcmtools.test/mkcg.6416.exp	\
	pcmtools.test/mkcg.vid2k.exp	\
	ChangeLog-20050419		\
	README.md

README: README.md
	fgrep -v "[Build Status]" $<  > $@

ChangeLog: $(srcdir)/.git
	git --git-dir $(srcdir)/.git log --pretty --numstat --summary | \
	  sed -e '/Signed-off-by:/d' | $(srcdir)/tools/git2cl/git2cl > $@

# release with paranoia backup to avoid debauched git repository
rel__tar = ${AMTAR} cof - ".git"
.PHONY: release
release: $(srcdir)/.git
	$(rel__tar) | bzip2 -9 -c >.backup-gitmeta-$$(date +%s).tar.bz2 && \
	touch configure.ac && \
	$(MAKE)  $(AM_MAKEFLAGS) \
	  top_distdir="$(top_distdir)" \
	  distdir="$(distdir)" \
	  distcheck && \
	git --git-dir $(srcdir)/.git \
	  commit -asm 'Prepare release $(PACKAGE_VERSION)' && \
	git --git-dir $(srcdir)/.git \
	  branch --no-track release/v$(PACKAGE_VERSION) && \
	git --git-dir $(srcdir)/.git \
	  tag -sm 'v$(PACKAGE_VERSION)' v$(PACKAGE_VERSION) && \
	touch configure.ac && \
	$(MAKE)  $(AM_MAKEFLAGS) \
	  top_distdir="$(top_distdir)" distdir="$(distdir)" \
	  distcheck && \
	git --git-dir $(srcdir)/.git \
	  commit --amend -aC HEAD

MAINTAINERCLEANFILES = \
	$(srcdir)/README \
	$(srcdir)/INSTALL \
	$(srcdir)/ChangeLog \
	$(srcdir)/configure \
	$(srcdir)/configure.ac~ \
	$(srcdir)/Makefile.in \
	$(srcdir)/depcomp \
	$(srcdir)/config.guess \
	$(srcdir)/config.sub \
	$(srcdir)/config.h.in \
	$(srcdir)/config.h.in~ \
	$(srcdir)/compile \
	$(srcdir)/ltmain.sh \
	$(srcdir)/m4/libtool.m4 \
	$(srcdir)/m4/lt~obsolete.m4 \
	$(srcdir)/m4/ltoptions.m4 \
	$(srcdir)/m4/ltsugar.m4 \
	$(srcdir)/m4/ltversion.m4 \
	$(srcdir)/missing \
	$(srcdir)/aclocal.m4 \
	$(srcdir)/install-sh
