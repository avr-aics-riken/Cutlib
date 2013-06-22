/// @file
/// @brief ポリゴンデータ最適化関数 宣言
///

#ifndef CUTLIB_REPAIR_POLYGON_DATA_H
#define CUTLIB_REPAIR_POLYGON_DATA_H

#include "Polylib.h"
using namespace PolylibNS;

namespace cutlib {

/// @defgroup CutlibUtil ユーティリティ関数
//@{

/// ポリゴンデータを最適化する.
///
///  @param[in,out] pl Polylibクラスオブジェクト
///  @param[in] doubt_vertex_order 頂点並び順を最適化するフラグ
///  @param[in] doubt_normal_quality 法線ベクトルを再計算するフラグ
///
void RepairPolygonData(const Polylib* pl,
                       bool doubt_vertex_order = true,
                       bool doubt_normal_quality = true);

//@}

} // namespace cutlib

#endif // CUTLIB_REPAIR_POLYGON_DATA_H
