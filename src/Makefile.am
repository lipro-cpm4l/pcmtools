# Copyright (c) 2002-2015, Stephan Linz <linz@li-pro.net>
# All rights reserved.
#

AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = 			\
	mkcg.6416			\
	mkcg.vid2k

noinst_LTLIBRARIES = libmkcg.la
bin_PROGRAMS = mkcg.6416 mkcg.vid2k

mkcg_6416_SOURCES = mkcg.6416.c
mkcg_6416_LDADD = libmkcg.la

mkcg_vid2k_SOURCES = mkcg.vid2k.c
mkcg_vid2k_LDADD = libmkcg.la

libmkcg_la_SOURCES = \
	mkcg.c

noinst_HEADERS = \
	pcmtools.h

MAINTAINERCLEANFILES = $(srcdir)/Makefile.in
