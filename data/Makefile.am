# Copyright (c) 2002-2015, Stephan Linz <linz@li-pro.net>
# All rights reserved.
#

EXTRA_DIST =
EXTRA_DIST +=				\
	ZG_6416				\
	ZG_VID2K

DATA_INST_DIR = @DATA_INST_DIR@

install-data-local:
	for dir in `cd $(top_srcdir)/data; find $(EXTRA_DIST) -type d` ; do \
	  echo " $(mkinstalldirs) $(DATA_INST_DIR)/$$dir"; \
	  $(mkinstalldirs) $(DATA_INST_DIR)/$$dir; \
	done
	for file in `cd $(top_srcdir)/data; find $(EXTRA_DIST) -type f` ; do \
	  echo " $(INSTALL_DATA) $(top_srcdir)/data/$$file $(DATA_INST_DIR)/$$file"; \
	  $(INSTALL_DATA) $(top_srcdir)/data/$$file $(DATA_INST_DIR)/$$file; \
	done

uninstall-local:
	rm -rf $(DATA_INST_DIR)

MAINTAINERCLEANFILES = $(srcdir)/Makefile.in
