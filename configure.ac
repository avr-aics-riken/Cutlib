##############################################################################
#
# Cutlib - Cut Information Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
# All rights reserved.
#
# Copyright (c) 2012-2013 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
##############################################################################
#
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Cutlib], [3.1.7], [keno@riken.jp], [Cutlib])

#AM_INIT_AUTOMAKE()
# to remedy error : Makefile.am:20: but option 'subdir-objects' is disabled
AM_INIT_AUTOMAKE([subdir-objects])


AC_CONFIG_SRCDIR([src/Cutlib.cpp])
AC_CONFIG_HEADERS([config.h])


#
# revision No.
#
CT_REVISION="20131124_1700"
AC_SUBST(CT_REVISION)



#
# default prefix setting
#
AC_PREFIX_DEFAULT([/usr/local/Cutlib])
if test "$prefix" = "NONE" ; then
  CT_INST_DIR=/usr/local/Cutlib
else
  CT_INST_DIR="$prefix"
fi



#
# TextParser
#
AC_ARG_WITH(parser, [AC_HELP_STRING([--with-parser=dir],[Specify TextParser install directory])], , with_parser=none)
AC_SUBST(TP_DIR)
AC_SUBST(TP_CFLAGS)
AC_SUBST(TP_LDFLAGS)

if test "$with_parser" != "none" ; then
  TP_DIR=$with_parser;
  TP_CFLAGS=`$TP_DIR/bin/tp-config --cflags`
  TP_LDFLAGS=`$TP_DIR/bin/tp-config --libs`
else
  echo "  Error: TextParser library is not specified."
  echo "         Specify --with-parser option."
  exit 1
fi


#
# Polylib
#
AC_ARG_WITH(polylib, [AC_HELP_STRING([--with-polylib=dir],[Specify Polylib install directory])], , with_polylib=none)
AC_SUBST(PL_DIR)
AC_SUBST(PL_CFLAGS)
AC_SUBST(PL_LDFLAGS)

if test "$with_polylib" != "none" ; then
  PL_DIR=$with_polylib;
  PL_CFLAGS=`$PL_DIR/bin/polylib-config --cflags`
  PL_LDFLAGS=`$PL_DIR/bin/polylib-config --libs`
else
  echo "  Error: Polylib is not specified."
  echo "         Specify --with-polylib option."
  exit 1
fi


#
# example
#
AC_ARG_WITH(example, [AC_HELP_STRING([--with-example=(no|yes)],[make example [no]])], , with_example=no)


#
# Compiler option
#
CT_CFLAGS="-I$CT_INST_DIR/include"
AC_SUBST(CT_CFLAGS)


#
# normal
#
AC_ARG_WITH(normal, [AC_HELP_STRING([--with-normal=(double|float)],[Precision of normal vector [double]])], , with_normal=double)

if test "$with_normal" = "double" ; then
 CT_CFLAGS="$CT_CFLAGS"" -DCUTINFO_NORMAL_DOUBLE"
fi



#
# timing
#
AC_ARG_ENABLE(timing,
[  --enable-timing    Turn on timing log [[default=no]]],
[case "${enableval}" in
  yes) enable_timing=yes ;;
  no)  enable_timing=no ;;
  *) AC_MSG_ERROR(bad value for --enable-timing) ;;
esac],
enable_timing=no)

# CT_TIMING is true, if measure timing
AC_SUBST(CT_TIMING)
AM_CONDITIONAL([CT_TIMING], [test x"${enable_timing}" = x"yes"])

if test x"${enable_timing}" = x"yes"; then
  CT_CFLAGS="$CT_CFLAGS"" -DCUTLIB_TIMING"
fi


#
# debug
#
AC_ARG_ENABLE(debug,
[  --enable-debug    Turn on debugging [[default=no]]],
[case "${enableval}" in
  yes) enable_debug=yes ;;
  no)  enable_debug=no ;;
  *) AC_MSG_ERROR(bad value for --enable-debug) ;;
esac],
enable_debug=no)

# generate  MACRO "#define CUTLIB_DEBUG 1" in config.h
#if test x"${enable_debug}" = x"yes"; then
#  AC_DEFINE(CUTLIB_DEBUG, 1, [Define to 1 if you want to debug])
#fi

if test x"${enable_debug}" = x"yes"; then
  CT_CFLAGS="$CT_CFLAGS"" -DCUTLIB_DEBUG"
fi



#
# octree
#
AC_ARG_WITH(octree, [AC_HELP_STRING([--with-octree=(no|yes)],[Use Octree [no]])], , with_octree=no)

# CT_OCTREE = true, if octree
AC_SUBST(CT_OCTREE)
AM_CONDITIONAL([CT_OCTREE], [test "$with_octree" = "yes"])



#
# Special flags
#

CT_CXX="$CXX"
AC_SUBST(CT_CXX)

CT_LIBS="-lCUT"
AC_SUBST(CT_LIBS)

CT_LDFLAGS="-L$CT_INST_DIR/lib"
AC_SUBST(CT_LDFLAGS)



#
# MAKE_SUB_DIRS
#
AC_SUBST(MAKE_SUB_DIRS)
MAKE_SUB_DIRS="src doc"

if test "$with_example" = "yes" ; then
  MAKE_SUB_DIRS="$MAKE_SUB_DIRS"" examples/Cell examples/Cell_Normal"
  MAKE_SUB_DIRS="$MAKE_SUB_DIRS"" examples/Node examples/Node_Normal"
  if test "$with_octree" = "yes" ; then
    MAKE_SUB_DIRS="$MAKE_SUB_DIRS"" examples/Octree"
  fi
fi



#
# Checks for programs.
#
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB



# Checks for libraries.


#
# Checks for header files.
#
AC_CHECK_HEADERS([stdint.h sys/time.h])

#
# Checks for typedefs, structures, and compiler characteristics.
#
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_SIZE_T


#
# Checks for library functions.
#
AC_CHECK_FUNCS([gettimeofday])



AC_CONFIG_FILES([Makefile \
                 doc/Makefile \
                 src/Makefile \
                 examples/Cell/Makefile \
                 examples/Cell_Normal/Makefile \
                 examples/Node/Makefile \
                 examples/Node_Normal/Makefile \
                 examples/Octree/Makefile \
                 include/Version.h \
                 cut-config ])

AC_OUTPUT

chmod +x ./cut-config

