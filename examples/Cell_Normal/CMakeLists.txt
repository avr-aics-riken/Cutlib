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

add_executable(cell2 main.cpp)

target_link_libraries(cell2 -lUtil -lCUT -l${PL_LIB_NAME} -l${TP_LIB_NAME})


set (test_parameter3 "${PROJECT_SOURCE_DIR}/examples/Cell_Normal/test.conf")
add_test(NAME TEST_3 COMMAND "cell2" ${test_parameter3})

set (test_parameter4 "${PROJECT_SOURCE_DIR}/examples/Cell_Normal/test-large.conf")
add_test(NAME TEST_4 COMMAND "cell2" ${test_parameter4})


configure_file(${PROJECT_SOURCE_DIR}/examples/Cell_Normal/small.tpp
               ${PROJECT_BINARY_DIR}/examples/Cell_Normal/small.tpp
               COPYONLY)

configure_file(${PROJECT_SOURCE_DIR}/examples/Cell_Normal/large.tpp
               ${PROJECT_BINARY_DIR}/examples/Cell_Normal/large.tpp
               COPYONLY)
