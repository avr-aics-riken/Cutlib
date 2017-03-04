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


include_directories(
      ${PROJECT_BINARY_DIR}/include
      ${PROJECT_SOURCE_DIR}/include
      ${TP_INC}
      ${PL_INC}
      ${PROJECT_SOURCE_DIR}/examples/util/include
)

link_directories(
      ${PROJECT_BINARY_DIR}/src
      ${PROJECT_BINARY_DIR}/examples/util
      ${TP_LIB}
      ${PL_LIB}
)


file(COPY ${PROJECT_SOURCE_DIR}/examples/STL_data/
     DESTINATION ${PROJECT_BINARY_DIR}/examples/STL_data
     FILES_MATCHING PATTERN "*.stl")


add_subdirectory(util)
add_subdirectory(Cell)
add_subdirectory(Cell_Normal)
add_subdirectory(Node)
add_subdirectory(Node_Normal)
