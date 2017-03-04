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

add_executable(node1 main.cpp)

target_link_libraries(node1 -lUtil -lCUT -lPOLY -lTP)


set (test_parameter5 "${PROJECT_SOURCE_DIR}/examples/Node/test.conf")
add_test(NAME TEST_5 COMMAND "node1" ${test_parameter5})

set (test_parameter6 "${PROJECT_SOURCE_DIR}/examples/Node/test-large.conf")
add_test(NAME TEST_6 COMMAND "node1" ${test_parameter6})


configure_file(${PROJECT_SOURCE_DIR}/examples/Node/small.tpp
               ${PROJECT_BINARY_DIR}/examples/Node/small.tpp
               COPYONLY)

configure_file(${PROJECT_SOURCE_DIR}/examples/Node/large.tpp
               ${PROJECT_BINARY_DIR}/examples/Node/large.tpp
               COPYONLY)
