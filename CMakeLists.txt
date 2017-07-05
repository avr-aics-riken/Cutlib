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

#-*- CMAKE -*-#
# Process this file with cmake to produce a configure script.

# Following parameters must be given at cmake command line

# Build options, see Readme.md for details.
#
# -D INSTALL_DIR=install_directory
#
# -D enable_OPENMP={yes|no}
#
# -D enable_timing={no|yes}
#
# -D enable_debug={no|yes}
#
# -D with_example={no|yes}
#
# -D real_type={float|double}
#
# -D with_TP=Installed_directory
#
# -D with_PL=Installed_directory
#

cmake_minimum_required(VERSION 2.6)


#######
# Build options
#######

option (enable_OPENMP "Enable OpenMP" "ON")
option (enable_timing "Enable Timing" "OFF")
option (enable_debug "Enable Debug" "OFF")
option (with_example "Compiling examples" "OFF")
option (real_type "Precision of float" "OFF")
option (with_octree "Use Octree" "OFF")



#######
# Project setting
#######

project(Cutlib C CXX)

if(INSTALL_DIR STREQUAL "")
  set(INSTALL_DIR "/usr/local/Cutlib")
endif()

# project()でCMAKE_INSTALL_PREFIX=/usr/localとなるので、再定義
set (CMAKE_INSTALL_PREFIX "${INSTALL_DIR}")

#set(CMAKE_BUILD_TYPE Release)



#######
set(PROJECT_VERSION "3.4.6")
set(LIB_REVISION "20170706_0830")
#######



#######
# Cmake module
#######

set(CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake)
include(${PROJECT_SOURCE_DIR}/cmake/CompileOptionSelector.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/cutlib_package.cmake)



#######
# Options
#######

AddOptimizeOption()

checkOpenMP()


precision()


if(enable_timing)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCUTLIB_TIMING")
endif()


if(enable_debug)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCUTLIB_DEBUG")
endif()


#######
# Display options
#######

message(" ")
message( STATUS "Destination PATH   : "      ${CMAKE_INSTALL_PREFIX})
message( STATUS "CMAKE_C_COMPILER   : "      ${CMAKE_C_COMPILER})
message( STATUS "CMAKE_C_FLAGS      : "      ${CMAKE_C_FLAGS})
message( STATUS "CMAKE_CXX_COMPILER : "      ${CMAKE_CXX_COMPILER})
message( STATUS "CMAKE_CXX_FLAGS    : "      ${CMAKE_CXX_FLAGS})

message(" ")
message( STATUS "OpenMP support   : "      ${enable_OPENMP})
message( STATUS "Timing support   : "      ${enable_timing})
message( STATUS "Debugging        : "      ${enable_debug})
message( STATUS "Example          : "      ${with_example})
message( STATUS "Floating point   : "      ${real_type})
#message( STATUS "Use Octree: "             ${with_octree})
message(" ")

if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
else()
  if(with_MPI)
    include(FindMPI)

    if(MPI_CXX_FOUND)
      message("MPI-CXX-CMP  : ${MPI_CXX_COMPILER}")
      message("MPI-CXX-FLAGS: ${MPI_CXX_COMPILE_FLAGS}")
      message("MPI-CXX-INC  : ${MPI_CXX_INCLUDE_PATH}")
      message("MPI-LNK-FLAGS: ${MPI_CXX_LINK_FLAGS}")
      message("MPI-LIBS     : ${MPI_CXX_LIBRARIES}")
    endif(MPI_CXX_FOUND)
  endif()
endif()


message(" ")
message("PROJECT : Cutlib " ${PROJECT_VERSION} " : " ${LIB_REVISION})
message(" ")



#######
# include PATH
#######

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
)


#######
# Find Libraries to depend on
#######

# TextParser
IF(with_TP)
  SET(TP_DIR "${with_TP}")
  SET(TP_INC "${TP_DIR}/include")
  SET(TP_LIB "${TP_DIR}/lib")
ELSE()
  MESSAGE("Error: can not find TPlib.")
ENDIF()

if(EXISTS ${TP_DIR}/lib/libTPmpi.a)
  set(TP_LIB_NAME "TPmpi")
else()
  set(TP_LIB_NAME "TP")
endif()


# Polylib
IF(with_PL)
  SET(PL_DIR "${with_PL}")
  SET(PL_INC "${PL_DIR}/include")
  SET(PL_LIB "${PL_DIR}/lib")
ELSE()
  MESSAGE("Error: can not find Polylib.")
ENDIF()

if(EXISTS ${PL_DIR}/lib/libPOLYmpi.a)
  set(PL_LIB_NAME "POLYmpi")
else()
  set(PL_LIB_NAME "POLY")
endif()


add_definitions(-DHAVE_CONFIG_H)

message("TextParser : ${TP_DIR}")
message("Polylib    : ${PL_DIR}")
message(" ")


#######
# Example
#######

if (with_example)
  enable_testing()
  message("Code test will be performed")
else()
  message("Code test is skipped")
endif()
message(" ")


#######
# Check header files
#######

include(CheckIncludeFiles)

CHECK_INCLUDE_FILES(inttypes.h HAVE_INTTYPES_H)
CHECK_INCLUDE_FILES(memory.h HAVE_MEMORY_H)
CHECK_INCLUDE_FILES(stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILES(stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILES(strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILES(string.h HAVE_STRING_H)
CHECK_INCLUDE_FILES(sys/stat.h HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILES(sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILES(unistd.h HAVE_UNISTD_H)



#######
# Sub Directories
#######

add_subdirectory(src)

add_subdirectory(doc)

if(with_example)
  add_subdirectory(examples)
endif()


#######
# configure files
#######

configure_file( config.h.in.cmake config.h )
configure_file( include/cutVersion.h.in ${PROJECT_BINARY_DIR}/include/cutVersion.h @ONLY)


#######
# Install files
#######

install(FILES  ChangeLog.md
               Readme.md
               License.txt
        DESTINATION share)


#######
# Installer
#######

cutlib_package()
