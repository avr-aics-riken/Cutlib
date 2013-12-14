/// @file
/// @brief グリッド情報アクセッサ サンプル(セル中心)
///

#ifndef GRID_ACCESSOR_CELL_H
#define GRID_ACCESSOR_CELL_H

#include "GridAccessor/GridAccessor.h"

namespace cutlib {

/// @addtogroup GridAccessor
// @{

/// セル中心グリッドアクセッサ(サンプル実装).
class Cell : public GridAccessor {

  double o[3];  ///< 原点(=セル(0,0,0)のマイナス側頂点)の座標値
  double d[3];  ///< セルピッチ

public:

  /// コンストラクタ.
  ///
  ///  @param[in] org   原点(=セル(0,0,0)のマイナス側頂点)の座標値
  ///  @param[in] pitch セルピッチ
  ///
  Cell(const double org[3], const double pitch[3]) {
    for (int i = 0; i < 3; i++) {
      o[i] = org[i];
      d[i] = pitch[i];
    }
  }

  /// ポリゴン検索領域を指定するメソッド.
  ///
  ///  @param[in] i,j,k  3次元インデックス
  ///  @param[out] center 計算基準点座標
  ///  @param[out] range  6方向毎の計算基準線分の長さ
  ///
  void getSearchRange(int i, int j, int k,
                      double center[3], double range[6]) const {
    center[0] = o[0] + (0.5 + i) * d[0];
    center[1] = o[1] + (0.5 + j) * d[1];
    center[2] = o[2] + (0.5 + k) * d[2];
    range[X_M] = range[X_P] = d[0];
    range[Y_M] = range[Y_P] = d[1];
    range[Z_M] = range[Z_P] = d[2];
  }

};

// @}

} // namespace cutlib

#endif // GRID_ACCESSOR_CELL_H
