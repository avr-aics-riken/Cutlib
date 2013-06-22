#####################################################################################
#
#  Cutlibテストプログラム コンパイル・リンク方法
#  ---------------------------------------
#
#  「環境に依存するマクロ変数」を確認・修正後
#      make
#  で，テストプログラム test1, test3 がコンパイル・リンクされます．
#
#
#  環境に依存するマクロ変数
#  ------------------------
#
#    - CXX: C++ コンパイラ
#    - POLYLIB_DIR: Polylibインストールディレクトリ
#    - CFLAGS: 最適化オプションの変更
#
#
#  Octree版テストプログラム test2 のコンパイル・リンク方法
#  -------------------------------------------------------
#
#     1. 「with Octree lib」下の行のコメントを外す．
#     2. 「without Octree lib」下をコメントアウト.
#     3. OCTREE_DIRにOctreeライブラリのインストールディレクトリを指定.
# 
#    (*)Octreeライブラリは, MRpFTT_SGI_20080428 による動作確認をしています．
#
#####################################################################################
 
TEST1_OBJS = test1.o Config.o outputVtk.o
TEST2_OBJS = test2.o Config.o outputVtk.o
TEST3_OBJS = test3.o Config.o

OBJS = $(TEST_OBJS) $(TEST1_OBJS) $(TEST2_OBJS) $(TEST3_OBJS)

#CXX = g++
CXX = icpc

CUTLIB_DIR = ..
CUTLIB_FLAGS = -I$(CUTLIB_DIR)/include
CUTLIB_LIBS = -L$(CUTLIB_DIR)/lib -lcutlib

POLYLIB_DIR = ../../Polylib_2_0_1
POLYLIB_FLAGS = -I$(POLYLIB_DIR)/include -I/usr/include/libxml2
POLYLIB_LIBS = -L$(POLYLIB_DIR)/lib -lPolylib -lxml2

# with Octree lib ############################################################
#all: test1 test2 test3
#OCTREE_DIR = ../../sgi-octree
#OCTREE_FLAGS = -DCUTLIB_OCTREE -DTD_USE_NAMESPACE -I$(OCTREE_DIR)/include
#OCTREE_LIBS = -L$(OCTREE_DIR)/lib -loctree
##############################################################################

# without Octree lib #########################################################
all: test1 test3
OCTREE_FLAGS = 
OCTREE_LIBS = 
##############################################################################

DEFINES =

#CFLAGS = -g -Wall $(CUTLIB_FLAGS) $(POLYLIB_FLAGS) $(OCTREE_FLAGS) $(DEFINES)
CFLAGS = -O3 -Wall $(CUTLIB_FLAGS) $(POLYLIB_FLAGS) $(OCTREE_FLAGS) $(DEFINES)


LDFLAGS = 
LIBS = $(CUTLIB_LIBS) $(POLYLIB_LIBS) $(OCTREE_LIBS) -lm

test1: $(TEST1_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(TEST1_OBJS) $(LIBS)

test2: $(TEST2_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(TEST2_OBJS) $(LIBS)

test3: $(TEST3_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(TEST3_OBJS) $(LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

clean:
	rm -rf *.o test1 test2 test3

depend: $(OBJS:.o=.cpp)
	@ rm -rf depend.inc
	@ for i in $^; do\
		$(CXX) $(CFLAGS) -MM $$i >> depend.inc;\
	done

-include depend.inc
