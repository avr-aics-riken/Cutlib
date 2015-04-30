##############################################################################
#
# Cutlib - Cut Information Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
# All rights reserved.
#
# Copyright (c) 2012-2014 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
##############################################################################
#

EXTRA_DIST= Makefile_hand

lib_LIBRARIES=libCUT.a

libCUT_a_CXXFLAGS = @CT_CFLAGS@ @PL_CFLAGS@ @TP_CFLAGS@ \
                   -I$(top_builddir)/include \
                   -I$(top_builddir)/include/CutInfo \
                   -I$(top_builddir)/include/GridAccessor \
                   -I$(top_builddir)/include/RepairPolygonData 


libCUT_a_SOURCES = \
   CutSearch.cpp \
   CutSearch.h \
   TargetTriangle.cpp \
   TargetTriangle.h \
   RepairPolygonData.cpp \
   Cutlib.cpp


if CT_TIMING
  libCUT_a_SOURCES += CutTiming.cpp CutTiming.h
endif


if CT_OCTREE
  libCUT_a_SOURCES += CutOctree.cpp CutOctree.h
endif


nobase_include_HEADERS = \
   $(top_builddir)/include/CutInfo/CutInfo.h \
   $(top_builddir)/include/CutInfo/CutInfoArray.h \
   $(top_builddir)/include/CutInfo/CutInfoOctree.h \
   $(top_builddir)/include/CutInfo/CutNormalArray.h \
   $(top_builddir)/include/Cutlib.h \
   $(top_builddir)/include/GridAccessor/Cell.h \
   $(top_builddir)/include/GridAccessor/GridAccessor.h \
   $(top_builddir)/include/GridAccessor/Node.h \
   $(top_builddir)/include/RepairPolygonData/RepairPolygonData.h \
   $(top_builddir)/include/SklCompatibility.h \
   $(top_builddir)/include/Version.h

