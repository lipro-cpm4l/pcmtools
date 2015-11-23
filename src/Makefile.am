# Copyright (c) 2002-2015, Stephan Linz <linz@li-pro.net>
# All rights reserved.
#

AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = 			\
	mkzg.6416			\
	mkzg.vid2k

noinst_LTLIBRARIES = libmkzg.la
bin_PROGRAMS = mkzg.6416 mkzg.vid2k

mkzg_6416_SOURCES = mkzg.6416.c
mkzg_6416_LDADD = libmkzg.la

mkzg_vid2k_SOURCES = mkzg.vid2k.c
mkzg_vid2k_LDADD = libmkzg.la

libmkzg_la_SOURCES = \
	mkzg.c

noinst_HEADERS = \
	pcmtools.h

MAINTAINERCLEANFILES = $(srcdir)/Makefile.in
