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

EXTRA_DIST= Makefile_hand

lib_LIBRARIES=libCUT.a

libCUT_a_CXXFLAGS = @PL_CFLAGS@ @TP_CFLAGS@ \
                   -I$(top_srcdir)/include \
                   -I$(top_srcdir)/include/CutInfo \
                   -I$(top_srcdir)/include/GridAccessor \
                   -I$(top_srcdir)/include/RepairPolygonData 


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
   $(top_srcdir)/include/CutInfo/CutInfo.h \
   $(top_srcdir)/include/CutInfo/CutInfoArray.h \
   $(top_srcdir)/include/CutInfo/CutInfoOctree.h \
   $(top_srcdir)/include/CutInfo/CutNormalArray.h \
   $(top_srcdir)/include/Cutlib.h \
   $(top_srcdir)/include/GridAccessor/Cell.h \
   $(top_srcdir)/include/GridAccessor/GridAccessor.h \
   $(top_srcdir)/include/GridAccessor/Node.h \
   $(top_srcdir)/include/RepairPolygonData/RepairPolygonData.h \
   $(top_srcdir)/include/SklCompatibility.h \
   $(top_srcdir)/include/Version.h

