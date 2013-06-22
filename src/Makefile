include ../make_setting

CUTLIB = ../lib/libcutlib.a

all: $(CUTLIB)

OBJS = Cutlib.o CutBoundary.o CutSearch.o TargetTriangle.o \
       RepairPolygonData.o

ifneq (, $(findstring -DCUTLIB_OCTRE, $(DEFINES)))
	OBJS += CutOctree.o
endif

ifneq (, $(findstring -DCUTLIB_TIMING, $(DEFINES)))
	OBJS += CutTiming.o
endif
 
CXX ?= g++
AR ?= ar
AR_FLAGS ?= cr
RANLIB ?= ranlib

POLYLIB_FLAGS = -I$(POLYLIB_DIR)/include
POLYLIB_LIBS = -L$(POLYLIB_DIR)/lib -lPolylib

TEXTPARSER_FLAGS =  -I$(TEXTPARSER_DIR)/include
TEXTPARSER_LIBS = -L$(TEXTPARSER_DIR)/lib -lTextParser

ifneq (, $(findstring -DCUTLIB_OCTRE, $(DEFINES)))
	OCTREE_FLAGS = -DTD_USE_NAMESPACE -I$(OCTREE_DIR)/include
	OCTREE_LIBS = -L$(OCTREE_DIR)/lib -loctree
endif

CFLAGS = $(OPTIONS) -I../include $(POLYLIB_FLAGS) $(TEXTPARSER_FLAGS) $(OCTREE_FLAGS) $(DEFINES)

$(CUTLIB): $(OBJS)
	$(AR) $(AR_FLAGS) $@ $(OBJS)
	$(RANLIB) $@

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

clean:
	rm -rf *.o

dist_clean: clean
	rm -rf $(CUTLIB) depend.inc


depend: $(OBJS:.o=.cpp)
	@ rm -rf depend.inc
	@ for i in $^; do\
		$(CXX) $(CFLAGS) -MM $$i >> depend.inc;\
	done

-include depend.inc
