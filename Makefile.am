##############################################################################
#
# Cutlib - Cut Information Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
# All rights reserved.
#
# Copyright (c) 2012-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
##############################################################################
#

SUBDIRS = @MAKE_SUB_DIRS@

bin_SCRIPTS = cut-config
CLEANFILES  = $(bin_SCRIPTS)
EXTRA_DIST  = cut-config.in Makefile_hand LICENSE doxygen/Doxyfile make_setting \
              examples BUILD_DIR/.gitkeep
dist_data_DATA = AUTHORS COPYING ChangeLog README LICENSE NEWS

clean-local:
	find ./ -name *.obj | xargs rm -f
	find ./ -name *.exe | xargs rm -f
	find ./ -name *.lib | xargs rm -f

distclean-local: clean-local
	rm -f ./bin/*
	rm -f ./lib/*

