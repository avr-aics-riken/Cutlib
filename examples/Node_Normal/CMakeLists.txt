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

add_executable(node2 main.cpp)

target_link_libraries(node2 -lUtil -lCUT -lPOLY -lTP)


set (test_parameter7 "${PROJECT_SOURCE_DIR}/examples/Node_Normal/test.conf")
add_test(NAME TEST_7 COMMAND "node2" ${test_parameter7})

set (test_parameter8 "${PROJECT_SOURCE_DIR}/examples/Node_Normal/test-large.conf")
add_test(NAME TEST_8 COMMAND "node2" ${test_parameter8})


configure_file(${PROJECT_SOURCE_DIR}/examples/Node_Normal/small.tpp
               ${PROJECT_BINARY_DIR}/examples/Node_Normal/small.tpp
               COPYONLY)

configure_file(${PROJECT_SOURCE_DIR}/examples/Node_Normal/large.tpp
               ${PROJECT_BINARY_DIR}/examples/Node_Normal/large.tpp
               COPYONLY)
