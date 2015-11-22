# Copyright (c) 2002-2015, Stephan Linz <linz@li-pro.net>
# All rights reserved.
#

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src data

EXTRA_DIST =
EXTRA_DIST +=				\
	ChangeLog-20050419		\
	README.md

README: README.md
	fgrep -v "[Build Status]" $<  > $@

ChangeLog: $(srcdir)/.git
	git --git-dir $(srcdir)/.git log --pretty --numstat --summary | \
	  sed -e '/Signed-off-by:/d' | git2cl > $@

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
	$(srcdir)/missing \
	$(srcdir)/aclocal.m4 \
	$(srcdir)/install-sh
