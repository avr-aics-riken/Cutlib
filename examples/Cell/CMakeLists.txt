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

add_executable(cell1 main.cpp)

target_link_libraries(cell1 -lUtil -lCUT -l${PL_LIB_NAME} -l${TP_LIB_NAME})


set (test_parameter1 "${PROJECT_SOURCE_DIR}/examples/Cell/test.conf")
add_test(NAME TEST_1 COMMAND "cell1" ${test_parameter1})

set (test_parameter2 "${PROJECT_SOURCE_DIR}/examples/Cell/test-large.conf")
add_test(NAME TEST_2 COMMAND "cell1" ${test_parameter2})


configure_file(${PROJECT_SOURCE_DIR}/examples/Cell/small.tpp
               ${PROJECT_BINARY_DIR}/examples/Cell/small.tpp
               COPYONLY)

configure_file(${PROJECT_SOURCE_DIR}/examples/Cell/large.tpp
               ${PROJECT_BINARY_DIR}/examples/Cell/large.tpp
               COPYONLY)
