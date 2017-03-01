###################################################################################
#
# CPMlib - Computational space Partitioning Management library
#
# Copyright (c) 2012-2014 Institute of Industrial Science (IIS), The University of Tokyo.
# All rights reserved.
#
# Copyright (c) 2014-2016 Advanced Institute for Computational Science (AICS), RIKEN.
# All rights reserved.
#
# Copyright (c) 2016-2017 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################

# Use OpenMP for examples
set(enable_OPENMP "${enable_OPENMP} ON")
checkOpenMP()


message(" ")
message("Examples : ")
message(STATUS "with_MPI           = " ${with_MPI})
message(STATUS "CXX FLAGS          = " ${CMAKE_CXX_FLAGS})
#message(STATUS "IMPLICIT_LINK_LIBS = " ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})


include_directories(
      ${PROJECT_BINARY_DIR}/include
      ${PROJECT_SOURCE_DIR}/include
      ${TP_INC}
)

link_directories(
      ${PROJECT_BINARY_DIR}/src
      ${TP_LIB}
)



#### Test1 : cxx/

add_executable(example1 ./cxx/main.C)
include_directories(${PROJECT_SOURCE_DIR}/Examples/cxx/include)

if(with_MPI)
  target_link_libraries(example1 -lCPMmpi -lTPmpi)
  set (test_parameters -np 24
                      "example1"
                      "${PROJECT_SOURCE_DIR}/Examples/cxx/data/input1.txt"
                      "${PROJECT_SOURCE_DIR}/Examples/cxx/data/input2.txt"
  )
  add_test(NAME TEST_1 COMMAND "mpirun" ${test_parameters}
  )
else()
  target_link_libraries(example1 -lCPM -lTP)
endif()



### Test2 : cxx_FDM

add_executable(example2 ./cxx_FDM/main.C)
include_directories(${PROJECT_SOURCE_DIR}/Examples/cxx_FDM/include)

if(with_MPI)
  target_link_libraries(example2 -lCPMmpi -lTPmpi)
  set (test_parameters -np 24
                      "example2"
                      "${PROJECT_SOURCE_DIR}/Examples/cxx_FDM/data/input1.txt"
                      "${PROJECT_SOURCE_DIR}/Examples/cxx_FDM/data/input2.txt"
  )
  add_test(NAME TEST_2 COMMAND "mpirun" ${test_parameters}
  )
else()
  target_link_libraries(example2 -lCPM -lTP)
endif()



### Test 3 : f90

add_executable(example3 ./f90/mconvp_CPM.f90)
set_target_properties(example3 PROPERTIES LINKER_LANGUAGE CXX)

if(with_MPI)
  target_link_libraries(example3 -lCPMmpi -lTPmpi)
else()
  target_link_libraries(example3 -lCPM -lTP)
endif()

# In the case of Fujitsu--linkfortran is mandatory
if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
  target_link_libraries(example3 ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES} "--linkfortran")
else()
  target_link_libraries(example3 ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
endif()

if(with_MPI)
  set (test_parameters -np 8
                      "example3"
                      "${PROJECT_SOURCE_DIR}/Examples/f90/data/in.jacobi"
  )
  add_test(NAME TEST_3 COMMAND "mpirun" ${test_parameters}
  )
else()
  add_test(TEST_3 example3)
endif()



### Test 4 : cxx_LMR

if(enable_LMR)
  add_executable(example4 ./cxx_LMR/main.C)
  include_directories(
        ${PROJECT_SOURCE_DIR}/include/LMR
        ${PROJECT_SOURCE_DIR}/include/LMR/BCM
        ${PROJECT_SOURCE_DIR}/include/LMR/inline
        ${PROJECT_SOURCE_DIR}/Examples/cxx_LMR/include
  )
  link_directories(${PROJECT_BINARY_DIR}/src/LMR)

  if(with_MPI)
    target_link_libraries(example4 -lCPM_LMRmpi -lCPMmpi -lTPmpi)
    set (test_parameters -np 15
                      "example4"
                      "${PROJECT_SOURCE_DIR}/Examples/cxx_LMR/data/leaf15.tp"
    )
    add_test(NAME TEST_4 COMMAND "mpirun" ${test_parameters})
  else()
    target_link_libraries(example4 -lCPM_LMR -lTP)
    add_test(TEST_4 example4)
  endif()
endif()


### Test 5 : mconvp_LMR

if(enable_LMR)
  add_executable(example5
              ./mconvp_LMR/mconvp_LMR_main.C
              ./mconvp_LMR/mconvp_LMR.f90
  )
  include_directories(
        ${PROJECT_SOURCE_DIR}/include/LMR
        ${PROJECT_SOURCE_DIR}/include/LMR/BCM
        ${PROJECT_SOURCE_DIR}/include/LMR/inline
  )
  link_directories(${PROJECT_BINARY_DIR}/src/LMR)

  set_target_properties(example5 PROPERTIES LINKER_LANGUAGE CXX)

  if(with_MPI)
    target_link_libraries(example5 -lCPM_LMRmpi -lCPMmpi -lTPmpi)
  else()
    target_link_libraries(example5 -lCPM_LMR -lTP)
  endif()

  # In the case of Fujitsu--linkfortran is mandatory
  if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
    target_link_libraries(example5 ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES} "--linkfortran")
  else()
    target_link_libraries(example5 ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
  endif()

  if(with_MPI)
    set (test_parameters -np 15
                      "example5"
                      "${PROJECT_SOURCE_DIR}/Examples/mconvp_LMR/data/mconv_jacobi.tp"
    )
    add_test(NAME TEST_5 COMMAND "mpirun" ${test_parameters}
    )
  else()
    add_test(TEST_5 example5)
  endif()
endif()
