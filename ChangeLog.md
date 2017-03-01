# Cutlib - Cut Information Library

## REVISION HISTORY

---
- 2017-03-02  Version 3.4.0
  - cmake Version
  - Tested.

|Compiler|OpenMP|timing|debug|normal|octree|Example|
|:--|:--:|:--:|:--:|:--:|:--:|:--:|
|Intel 17.0.1 |ok|ok|ok|float/double|||
|GNU 6.2.0    ||||||||
|fx10         ||||||||
|K            ||||||||

---
- 2015-06-10  Version 3.3.0
  - r28 introduce BUILD_DIR to keep source directory clean
  - Change to run configure
  - Change configure.ac

---
- 2014-07-29  Version 3.2.5
  - r27 bug fix
  - include/CutInfo/CutNormalArray.h

---
- 2014-05-16  Version 3.2.4
  - r26 update src/Makefile.am
  - include CT_CFLAGS to reflect -DCUTINFO_NORMAL_DOUBLE


---
- 2014-03-17  Version 3.2.3
  - r25 introduce common namespace Vec3class


---
- 2014-03-04  Version 3.2.2
  - r24 version format
  - change output format for cut-config --version


---
- 2014-02-27  Version 3.2.1
  - r23 update related to Polylib
  - TargetTriangle.h, RepairPolygonData.cpp
  - change specification of floating point type in configure

---
- 2014-01-19
  - r22 update year 2014 for copyright
  - merge check_on_K to main trunk


---
- 2013-12-15  Version 3.1.8
  - r21 check packaging on branch check_on_K
    - add include/Version.h again for hand compile

  - r20 merge szks fix
    - fix file permissions and delete unnecessary files
    - remove an extra ";" in CutSearch.h
    - calculate TargetTriangle::dot_normal_vertex0 in double precision
    - delete include/Version.h


---
- 2013-11-24
  - on branch check_on_K

  - modify configure.ac
    - CutBoundary.cpp/h were expired
    - INSTALL example is modified

  - merge pull request from Use exid to branch check_on_K
  - fix examples/util/src/outputVtk.cpp for octree examples
  - Change the polygon selection policy and the definition of boundary ID as follows:
   1. All polygon groups are searched by Polylib::search_polygons method.
   2. The polygons whose exid between 1 and 255 are used for intersection calculations.
   3. Each polygon's exid is adopted as a boundary ID.

  - changed files are;
    - outputVtk.cpp, CutOctree.cpp, CutOctree.h, CutSearch.cpp, CutSearch.h, Cutlib.cpp, RepairPolygonData.cpp


---
- 2013-11-22  Version 3.1.7
  - r19 expire changes from r17 to r18
  - replace TargetTriangle.h and ChangeLog(this file)
  - modify AM_INIT_AUTOMAKE([subdir-objects]) to fix error


---
- 2013-10-10
  - r18 merge precision improvement by szks


---
- 2013-10-02  Version 3.1.6
  - r17 modify for intel mpi
  - include mpi.h before stdio.h to suppress error message #error "SEEK_SET is #defined but must not be for the C++ binding of MPI"


---
- 2013-09-15  Version 3.1.5
  - r16 update user guide
  - add cutlib_ug.pdf and Cutlib3intro.pdf


---
- 2013-07-20  Version 3.1.4
  - r15 Introduce getVersionInfo()
  - add Version.h.in


---
- 2013-07-15  Version 3.1.3
  - r14 update configure.ac and Makefile.am


---
- 2013-07-06  Version 3.1.2
  - r13 update configure.ac and ChangeLog, NEW
  - Update configure.ac, typo normal vector.
  - Change contents in ChangeLog and NEWS.
  - Add install examples.


---
- 2013-07-04
  - r12 update INSTALL


---
- 2013-07-03  Version 3.1.1
  - r11 add depend.inc


---
- 2013-07-02  Version 3.1.0
  - r10 autotools version
  - r9 update ver. 3.1.0


---
- 2013-06-09  Version 3.0.1
  - r8 update Cutlib-3.0.1-beta


---
- 2013-02-25  Version 3.0.0
  - r7 update Cutlib-3.0.0-beta


---
- 2012-10-16  Version 2.0.5
  - r6 update Cutlib-2.0.5


---
- 2012-09-06  Version 2.0.4


---
- 2012-04-21  Version 2.0.3
  - r5 update Cutlib-2.0.3


---
- 2011-05-12  Version 2.0.2
  - r4 update Cutlib-2.0.2


---
- 2011-05-09  Version 2.0.1
  - r3 update Cutlib-2.0.1


---
- 2010-11-10  Version 2.0.0
  - r2 add Cutlib-2.0.0


---
- 2010-04-28  Version 1.0.0
