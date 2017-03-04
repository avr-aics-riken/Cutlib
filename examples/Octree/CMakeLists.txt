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

add_executable(cell
               main.cpp
               ../util/src/ConfigBase.cpp
               ../util/src/ConfigFile.cpp
               ../util/src/CutTest.cpp
               ../util/src/outputVtk.cpp
               )

include_directories(${PROJECT_SOURCE_DIR}/examples/Cell_Normal)

target_link_libraries(cell -lCUT -lPOLY -lTP)

set (test_parameter1 "${PROJECT_SOURCE_DIR}/examples/Cell/test.conf")
add_test(NAME TEST_1 COMMAND "cell" ${test_parameter1})

set (test_parameter2 "${PROJECT_SOURCE_DIR}/examples/Cell/test-large.conf")
add_test(NAME TEST_2 COMMAND "cell" ${test_parameter2})

# for test
file(COPY "${PROJECT_SOURCE_DIR}/examples/Cell_Normal"
     DESTINATION "${PROJECT_BINARY_DIR}/examples/Cell_Normal"
     FILES_MATCHING PATTERN "*.tpp")

file(COPY "${PROJECT_SOURCE_DIR}/examples/Cell_Normal"
     DESTINATION "${PROJECT_BINARY_DIR}/examples/Cell_Normal"
     FILES_MATCHING PATTERN "*.conf")
