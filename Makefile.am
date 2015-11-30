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
	autogen.sh			\
	tools/get-version		\
	ChangeLog-20050419

dist_doc_DATA  = README README.md TODO
doc_DATA       = ChangeLog ChangeLog-20050419

# Build readme from markdown
README: README.md
	$(AM_V_GEN)fgrep -v "[Build Status]" $<  > $@

# Git pritty format string for ChangeLog generator
GITLOG_PF = %w(78,0,8)%ad  %an <%ae>%n%n* %w(78,0,10)%s [%h]%n

# Build changelog from git history
ChangeLog:
	$(AM_V_GEN)if test -e $(top_srcdir)/.git; then \
	  prev=$$(git --git-dir $(top_srcdir)/.git describe \
	              --tags --always --match '[0-9]*' 2> /dev/null) ; \
	  for tag in $$(git --git-dir $(top_srcdir)/.git tag | \
	                $(EGREP) '^[0-9]+(\.[0-9]+){1,}$$' | \
	                sort -t. -k 1,1nr -k 2,2nr -k 3,3nr); do \
	    if [ x"$$prev" = x ]; then prev=$$tag ; fi ; \
	    if [ x"$$prev" = x"$$tag" ]; then continue; fi ; \
	    echo "$$prev [$$(git --git-dir $(top_srcdir)/.git log \
	                     $$prev -1 --pretty=format:'%ai')]:" ; \
	    echo "" ; \
	    git --git-dir $(top_srcdir)/.git log --date=short \
	        --pretty=format:'$(GITLOG_PF)' $$tag..$$prev ; \
	    echo "" ; \
	    prev=$$tag ; \
	  done > $@ ; \
	else \
	  touch $@ ; \
	fi

# Build todo from git controlled objects
TODO:
	$(AM_V_GEN)if test -e $(top_srcdir)/.git; then \
	  git --git-dir $(top_srcdir)/.git ls-tree \
	  -r --name-only HEAD | xargs grep -nH "T[O]DO:\|FIX[M][E]:" | \
	  sed 's/\([^:]*:[^:]*\):\(.*\)T[O]DO:\(.*\)/\3 (\1)/' | \
	  sed 's/\([^:]*:[^:]*\):\(.*\)FIX[M][E]:\(.*\)/\3 (\1)/' | \
	  grep -v '\$$(top_srcdir)/\.git' | sort -ns | \
	  awk '(last != $$1) {print ""} {last=$$1 ; print}' > $@ ; \
	else \
	  touch $@ ; \
	fi

# release with paranoia backup to avoid debauched git repository
rel__tar = ${AMTAR} cof - "$(top_srcdir)/.git"
.PHONY: release
release: $(top_srcdir)/.git $(top_srcdir)/.dist-version
	$(AM_V_GEN)$(rel__tar) | bzip2 -9 -c \
	  >$(top_srcdir)/.backup-gitdir-$$(date +%s).tar.bz2 && \
	touch configure.ac && \
	$(MAKE)  $(AM_MAKEFLAGS) \
	  top_distdir="$(top_distdir)" \
	  distdir="$(distdir)" \
	  distcheck && \
	test -z "$$(git --git-dir $(top_srcdir)/.git status --porcelain)" && \
	git --git-dir $(top_srcdir)/.git \
	  diff --quiet --ignore-submodules HEAD && \
	git --git-dir $(top_srcdir)/.git \
	  branch --no-track release/$(PACKAGE_VERSION) && \
	git --git-dir $(top_srcdir)/.git \
	  tag -sm 'Version $(PACKAGE_VERSION)' $(PACKAGE_VERSION)

MAINTAINERCLEANFILES = \
	$(srcdir)/config.guess \
	$(srcdir)/config.sub \
	$(srcdir)/install-sh \
	$(srcdir)/ltmain.sh \
	$(srcdir)/m4/libtool.m4 \
	$(srcdir)/m4/lt*.m4 \
	$(srcdir)/m4/ax_*.m4 \
	$(srcdir)/m4/pkg.m4 \
	$(srcdir)/aclocal.m4 \
	$(srcdir)/ar-lib \
	$(srcdir)/compile \
	$(srcdir)/missing \
	$(srcdir)/depcomp \
	$(srcdir)/INSTALL \
	$(srcdir)/README \
	$(srcdir)/ChangeLog \
	$(srcdir)/TODO \
	$(srcdir)/configure \
	$(srcdir)/configure.ac~ \
	$(srcdir)/config.h.in \
	$(srcdir)/config.h.in~ \
	$(srcdir)/Makefile.in
