PROGRAM = a.out

CUTLIB_DIR = ../..
UTIL_DIR = ../util

vpath %.cpp $(UTIL_DIR)/src

OBJS = main.o
UTIL_OBJS = ConfigFile.o ConfigBase.o outputVtk.o

include $(CUTLIB_DIR)/make_setting

CUTLIB_FLAGS = -I$(CUTLIB_DIR)/include
CUTLIB_LIBS = -L$(CUTLIB_DIR)/lib -lcutlib

POLYLIB_FLAGS = -I$(POLYLIB_DIR)/include
POLYLIB_LIBS = -L$(POLYLIB_DIR)/lib -lPolylib

TEXTPARSER_FLAGS =  -I$(TEXTPARSER_DIR)/include
TEXTPARSER_LIBS = -L$(TEXTPARSER_DIR)/lib -lTextParser

ifneq (, $(findstring -DCUTLIB_OCTRE, $(DEFINES)))
	OCTREE_FLAGS = -DTD_USE_NAMESPACE -I$(OCTREE_DIR)/include
	OCTREE_LIBS = -L$(OCTREE_DIR)/lib -loctree
endif

CFLAGS = $(OPTIONS) -I$(UTIL_DIR)/include $(CUTLIB_FLAGS) $(POLYLIB_FLAGS) $(TEXTPARSER_FLAGS) $(OCTREE_FLAGS) $(DEFINES)

LIBS = $(CUTLIB_LIBS) $(POLYLIB_LIBS) $(TEXTPARSER_LIBS) -lm

PROGRAM: $(OBJS) $(UTIL_OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(PROGRAM) $(OBJS) $(UTIL_OBJS) $(LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

clean:
	rm -rf *.o

dist_clean: clean
	rm -rf $(PROGRAM) depend.inc *.vtk

depend: $(OBJS:.o=.cpp) $(UTIL_OBJS:.o=.cpp)
	@ rm -rf depend.inc
	@ for i in $^; do\
		$(CXX) $(CFLAGS) -MM $$i >> depend.inc;\
	done

$(PROGRAM): $(CUTLIB_DIR)/lib/libcutlib.a

-include depend.inc
