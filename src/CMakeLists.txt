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

include_directories(
       ${PROJECT_BINARY_DIR}/include                     # cutVersion.h
       ${PROJECT_SOURCE_DIR}/include
       ${TP_INC}
       ${PL_INC}
)


#link_directories( ${TP_LIB} ${PL_LIB} )


set(cut_files
    Cutlib.cpp
    CutSearch.cpp
    RepairPolygonData.cpp
    TargetTriangle.cpp
)

add_library(CUT STATIC ${cut_files})


if(enable_timing)
  set(cut_files ${cut_files}  CutTiming.cpp)
endif()


if(with_octree)
  set(cut_files ${cut_files}  CutOctree.cpp)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTD_USE_NAMESPACE")
  target_link_libraries(CUT -loctree)
endif()



install(TARGETS CUT DESTINATION lib)


install(FILES
        ${PROJECT_SOURCE_DIR}/include/Cutlib.h
        ${PROJECT_SOURCE_DIR}/include/SklCompatibility.h
        ${PROJECT_BINARY_DIR}/include/cutVersion.h
        DESTINATION include
)

install(FILES
        ${PROJECT_SOURCE_DIR}/include/CutInfo/CutInfo.h
        ${PROJECT_SOURCE_DIR}/include/CutInfo/CutInfoArray.h
        ${PROJECT_SOURCE_DIR}/include/CutInfo/CutInfoOctree.h
        ${PROJECT_SOURCE_DIR}/include/CutInfo/CutNormalArray.h
        DESTINATION include/CutInfo
)

install(FILES
        ${PROJECT_SOURCE_DIR}/include/GridAccessor/Cell.h
        ${PROJECT_SOURCE_DIR}/include/GridAccessor/GridAccessor.h
        ${PROJECT_SOURCE_DIR}/include/GridAccessor/Node.h
        DESTINATION include/GridAccessor
)

install(FILES
        ${PROJECT_SOURCE_DIR}/include/RepairPolygonData/RepairPolygonData.h
        DESTINATION include/RepairPolygonData
)
