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

# - Try to find Cutlib
# Once done, this will define
#
#  CUT_FOUND - system has Cutlib
#  CUT_INCLUDE_DIRS - Cutlib include directories
#  CUT_LIBRARIES - link these to use Cutlib

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(CUT_PKGCONF CUT)

if(CMAKE_PREFIX_PATH)
  set(CUT_CANDIDATE_PATH ${CMAKE_PREFIX_PATH})
  file(GLOB tmp "${CMAKE_PREFIX_PATH}/[Jj][Hh][Pp][Cc][Nn][Dd][Ff]*/")
  list(APPEND CUT_CANDIDATE_PATH ${tmp})
endif()

# Include dir
find_path(CUT_INCLUDE_DIR
  NAMES Cutlib.h
  PATHS ${CUT_ROOT} ${CUT_PKGCONF_INCLUDE_DIRS} ${CUT_CANDIDATE_PATH}
  PATH_SUFFIXES include
)

# Finally the library itself
find_library(CUT_LIBRARY
  NAMES CUT
  PATHS ${CUT_ROOT} ${CUT_PKGCONF_LIBRARY_DIRS} ${CUT_CANDIDATE_PATH}
  PATH_SUFFIXES lib
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(CUT_PROCESS_INCLUDES CUT_INCLUDE_DIR)
set(CUT_PROCESS_LIBS CUT_LIBRARY)
libfind_process(CUT)
