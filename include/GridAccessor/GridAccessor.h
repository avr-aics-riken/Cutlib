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
/// @brief グリッド情報アクセッサ基底クラス
///

#ifndef GRID_ACCESSOR_H
#define GRID_ACCESSOR_H

#include "CutInfo/CutInfo.h"  // for CutInfoOrder

namespace cutlib {

/// @defgroup GridAccessor グリッド情報アクセッサクラス
//@{

/// グリッド情報アクセッサ基底クラス.
class GridAccessor {

public:

  /// コンストラクタ.
  GridAccessor() {}

  /// デストラクタ.
  virtual ~GridAccessor() {}

  /// ポリゴン検索領域を指定するメソッド.
  ///
  ///  @param[in] i,j,k  3次元インデックス
  ///  @param[out] center 計算基準点座標
  ///  @param[out] range  6方向毎の計算基準線分の長さ
  ///
  virtual void getSearchRange(int i, int j, int k,
                              double center[3], double range[6]) const = 0;

};

//@}

} // namespace cutlib

#endif // GRID_ACCESSOR_H
