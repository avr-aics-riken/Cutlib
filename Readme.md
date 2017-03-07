# CPMlib - Computational space Partitioning Management library

* Copyright (c) 2012-2014 Institute of Industrial Science (IIS), The University of Tokyo. All rights reserved.
* Copyright (c) 2014-2016 Advanced Institute for Computational Science (AICS), RIKEN. All rights reserved.
* Copyright (c) 2016-2017 Research Institute for Information Technology (RIIT), Kyushu University. All rights reserved.

## OUTLINE

This Cut Information library provides functions to calculate intersection between polygon face and background grid, and to manage generated information. Both Cartesian and Octree grid data structure can be utilized for the background grid.

## SOFTWARE REQUIREMENT
- Cmake
- TextParser
- Polylib


## INGREDIENTS
~~~
ChangeLog.md           History of modification
doc/                   
  cutlib_ug.pdf        User's manual (in Japanese)
  Cutlib3intro.pdf     Outline and introduction (in Japanese)
  doxygen/
    Doxyfile           Configuration file to generate a doxygen file
examples/
  Makefile_hand        For hand compile
  Cell/                Cartesian cell center
  Cell_Normal/         Cartesian cell center with normal vector
  Node/                Cartesian node
  Node_Normal/         Cartesian node with normal vector
  Octree/              Octree source
  STL_data/            sample STL data
  util/                utility source
include/               Header files
License.txt            License to apply
Readme.md              This document
src/                   Source codes
~~~

## HOW TO BUILD

### Build

~~~
$ export CT_HOME=/hogehoge
$ mkdir BUILD
$ cd BUILD
$ cmake [options] ..
$ make
$ sudo make install
~~~


### Options

`-D INSTALL_DIR=install_directory`

>  Specify the directory that this library will be installed. Built library is installed at `install_directory/lib` and the header files are placed at `install_directory/include`. The default install directory is `/usr/local/PMlib`.

`-D enable_OPENMP=` {yes | no}

> Enable OpenMP directives. This option is valid if only PAPI interface is enabled.

`-D enable_timing=` {no | yes}

>  This option allows you to measure timing.

`-D enable_debug=` {no | yes}

>  This option gives debug information.

`-D with_example=` {no | yes}

>  This option turns on compiling sample codes. The default is no.

`-D with_normal=` {float | double}

>  Specify floating type to compute normal vector. Default is float precision, otherwise double precision.

`-D with_TP=` *Installed_directory*

> Specify the directory path that TextParser is installed.

`-D with_PL=` *Installed_directory*

> Specify the directory path that Polylib is installed.


The default compiler options are described in `cmake/CompilerOptionSelector.cmake` file. See BUILD OPTION section in CMakeLists.txt in detail.


## Configure Examples

`$ export CT_HOME=hogehoge`

In following exsmples, assuming that TextParser and Polylib are installed under the CT_HOME directory. If not, please specify applicable directory paths.

### INTEL/GNU compiler

~~~
$ make -DINSTALL_DIR=${CT_HOME}/Cutlib -Denable_OPENMP=yes -Denable_timing=no -Denable_debug=no -Dwith_normal=float -Dwith_example=no -Dwith_TP=${HOME}/TextParser -Dwith_PL=${HOME}/Polylib ..
~~~

#### Note
In case of some Intel compiler environment, please specify environemnt variables
`export CC=icc CXX=icpc F90=ifort FC=ifort` before compiling.


### FUJITSU compiler / FX10, FX100, K on login nodes (Cross compilation)

~~~
$ cmake -DINSTALL_DIR=${CT_HOME}/Cutlib -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain_fx10.cmake -Denable_OPENMP=yes -Denable_timing=no -Denable_debug=no -Dwith_normal=float -Dwith_example=no -Dwith_TP=${HOME}/TextParser -Dwith_PL=${HOME}/Polylib ..

$ cmake -DINSTALL_DIR=${CT_HOME}/Cutlib -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain_fx100.cmake -Denable_OPENMP=yes -Denable_timing=no -Denable_debug=no -Dwith_normal=float -Dwith_example=no -Dwith_TP=${HOME}/TextParser -Dwith_PL=${HOME}/Polylib ..

$ cmake -DINSTALL_DIR=${CT_HOME}/Cutlib -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain_K.cmake -Denable_OPENMP=yes -Denable_timing=no -Denable_debug=no -Dwith_normal=float -Dwith_example=no -Dwith_TP=${HOME}/TextParser -Dwith_PL=${HOME}/Polylib ..
~~~


##### Note
- On Fujitsu machines(fx10, K, fx100), confirm appropriate directrory path for compiler environment.
- Before building, execute following command for clean. `$ make distclean`



## EXAMPLES

* If you specify the test option by `-Dwith_example=yes`, you can
execute the intrinsic tests by;

	`$ make test` or `$ ctest`

* The detailed results are written in `build/Testing/Temporary/LastTest.log` file.
Meanwhile, the summary is displayed for stdout.




## CONTRIBUTORS

* Kenji     Ono      *keno@{cc.kyushu-u.ac, riken}.jp*
* Soichiro  Suzuki
