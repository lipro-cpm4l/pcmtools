#
# Automake definitions for top source directory
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

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST =
CLEANFILES =

SUBDIRS = src man data

EXTRA_DIST +=				\
	autogen.sh			\
	acx/get-version			\
	ChangeLog-20050419

dist_doc_DATA  = README README.md TODO
doc_DATA       = ChangeLog ChangeLog-20050419

if BUILD_FROM_GIT
# Build readme from markdown
README: README.md
	$(AM_V_GEN)fgrep -v "[Build Status]" $<  > $@
endif

if BUILD_FROM_GIT
# Git pritty format string for ChangeLog generator
GITLOG_PF = %w(78,0,8)%ad  %an <%ae>%n%n* %w(78,0,10)%s [%h]%n

# Build changelog from git history
ChangeLog:
	$(AM_V_GEN)prev=$$(git --git-dir $(top_srcdir)/.git describe \
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
	  done > $@
endif

# Build todo from git controlled objects
if BUILD_FROM_GIT
TODO:
	$(AM_V_GEN)git --git-dir $(top_srcdir)/.git ls-tree \
	  -r --name-only HEAD | grep -v '^Makefile\.am' | \
	  sed 's/^/$(top_srcdir)\//g' | \
	  xargs grep -nH "T[O]DO:\|FIX[M][E]:" | \
	  sed 's/\([^:]*:[^:]*\):\(.*\)T[O]DO:\(.*\)/\3 (\1)/' | \
	  sed 's/\([^:]*:[^:]*\):\(.*\)FIX[M][E]:\(.*\)/\3 (\1)/' | \
	  grep -v '\$$(top_srcdir)/\.git' | sort -ns | \
	  awk '(last != $$1) {print ""} {last=$$1 ; print}' > $@
endif

if BUILD_FROM_GIT
# Tar backup command line string for release generator
rel__tar = ${AMTAR} cof - "$(top_srcdir)/.git"

# release with paranoia backup to avoid debauched git repository
.PHONY: release
release: $(top_srcdir)/.dist-version
	$(AM_V_GEN)$(rel__tar) | bzip2 -9 -c \
	  >$(top_srcdir)/.backup-gitdir-$$(date +%s).tar.bz2 && \
	test -z "$$(git --git-dir $(top_srcdir)/.git status --porcelain)" && \
	git --git-dir $(top_srcdir)/.git \
	  diff --quiet --ignore-submodules HEAD && \
	git --git-dir $(top_srcdir)/.git \
	  branch --no-track release/$(PACKAGE_VERSION) && \
	git --git-dir $(top_srcdir)/.git \
	  tag -sm 'Version $(PACKAGE_VERSION)' $(PACKAGE_VERSION) && \
	touch configure.ac && \
	$(MAKE) $(AM_MAKEFLAGS) \
	  top_distdir="$(top_distdir)" \
	  distdir="$(distdir)" \
	  distcheck
endif

MAINTAINERCLEANFILES = \
	$(srcdir)/aclocal.m4 \
	$(srcdir)/acx/ar-lib \
	$(srcdir)/acx/compile \
	$(srcdir)/acx/config.* \
	$(srcdir)/acx/depcomp \
	$(srcdir)/acx/install-sh \
	$(srcdir)/acx/ltmain.sh \
	$(srcdir)/acx/missing \
	$(srcdir)/configure \
	$(srcdir)/configure.ac~ \
	$(srcdir)/config.in \
	$(srcdir)/config.in~ \
	$(srcdir)/m4/ax_*.m4 \
	$(srcdir)/m4/libtool.m4 \
	$(srcdir)/m4/lt*.m4 \
	$(srcdir)/m4/pkg.m4 \
	$(srcdir)/Makefile.in \
	$(srcdir)/INSTALL \
	$(srcdir)/README \
	$(srcdir)/ChangeLog \
	$(srcdir)/TODO
