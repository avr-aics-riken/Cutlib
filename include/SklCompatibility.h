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
