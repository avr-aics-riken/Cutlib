/*
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
*/

/// @file
/// @brief pFTTクラスライブラリとの互換性のためのヘッダ
///
/// Compatibility.hの内容を「Sph〜」を「Skl〜」に修正して作成
///

#ifndef SKL_COMPATIBILITY_H
#define SKL_COMPATIBILITY_H

#include "Tree.h"
#include "Node.h"
#include "Cell.h"

#include "DimPolicy.h"
#include "DataPolicy.h"
#include "Timing.h"

typedef TD_NAMESPACE::Tree<
    TD_NAMESPACE::ThreeDimPolicy,
    TD_NAMESPACE::DefaultDataPolicy< float >
> SklTree;
typedef SklTree::cell_type SklCell;

typedef SklCell::NeighborSet SklNghbrSet;
typedef SklTree::SplitList SklSplitList;

inline
double
SklGetTime()
{
    return TD_NAMESPACE::GetTime();
}

#endif // SKL_COMPATIBILITY_H
