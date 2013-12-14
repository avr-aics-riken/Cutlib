/// @file
/// @brief グリッド情報アクセッサ サンプル(ノード)
///

#ifndef GRID_ACCESSOR_NODE_H
#define GRID_ACCESSOR_NODE_H

#include "GridAccessor/GridAccessor.h"

namespace cutlib {

/// @addtogroup GridAccessor
// @{

/// ノードグリッドアクセッサ(サンプル実装).
class Node : public GridAccessor {

  double o[3];   ///< 点(=ノード(0,0,0))の座標値
  double d[3];   ///< ノード間隔

public:

  /// コンストラクタ.
  ///
  ///  @param[in] org   原点(=ノード(0,0,0))の座標値
  ///  @param[in] pitch ノード間隔
  ///
  Node(const double org[3], const double pitch[3]) {
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
    center[0] = o[0] + i * d[0];
    center[1] = o[1] + j * d[1];
    center[2] = o[2] + k * d[2];
    range[X_M] = range[X_P] = d[0];
    range[Y_M] = range[Y_P] = d[1];
    range[Z_M] = range[Z_P] = d[2];
  }

};

//@}


} // namespace cutlib

#endif // GRID_ACCESSOR_NODE_H
