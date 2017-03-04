###################################################################################
#
# Cutlib - Cut Information Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
# All rights reserved.
#
# Copyright (c) 2012-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
# Copyright (c) 2016-2017 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################

set(util_srcs
        src/ConfigBase.cpp
        src/ConfigFile.cpp
        src/CutTest.cpp
        src/outputVtk.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/examples/util/include)

add_library(Util STATIC ${util_srcs})

target_link_libraries(Util -lCUT)
