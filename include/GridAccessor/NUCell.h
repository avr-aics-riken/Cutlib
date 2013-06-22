/// @file
/// @brief グリッド情報アクセッサ サンプル(セル中心, 不等間隔格子)
///

#ifndef GRID_ACCESSOR_NON_UNIFORM_CELL_H
#define GRID_ACCESSOR_NON_UNIFORM_CELL_H

#include <cstddef> // for size_t
#include <cassert> // for debug

#include "GridAccessor/GridAccessor.h"

namespace cutlib {

/// @addtogroup GridAccessor
// @{

/// セル中心グリッドアクセッサ(サンプル実装).
class NUCell : public GridAccessor {

  size_t nx;   ///< x方向セル数
  size_t ny;   ///< y方向セル数
  size_t nz;   ///< z方向セル数
  const double* x;   ///< グリッドx座標配列(nx+1要素)
  const double* y;   ///< グリッドy座標配列(ny+1要素)
  const double* z;   ///< グリッドz座標配列(nz+1要素)

public:

  /// コンストラクタ.
  ///
  ///  @param[in] nx  x方向セル数
  ///  @param[in] ny  y方向セル数
  ///  @param[in] nz  z方向セル数
  ///  @param[in] x   グリッドx座標配列(nx+1要素)
  ///  @param[in] y   グリッドy座標配列(ny+1要素)
  ///  @param[in] z   グリッドz座標配列(nz+1要素)
  ///
  NUCell(size_t nx, size_t ny, size_t nz,
         const double* x, const double* y, const double* z)
   : nx(nx), ny(ny), nz(nz), x(x), y(y), z(z) {}

  /// コンストラクタ.
  ///
  ///  @param[in] ndim  セル数3次元配列
  ///  @param[in] x   グリッドx座標配列(nx+1要素)
  ///  @param[in] y   グリッドy座標配列(ny+1要素)
  ///  @param[in] z   グリッドz座標配列(nz+1要素)
  ///
  NUCell(const size_t ndim[3],
         const double* x, const double* y, const double* z)
   : nx(ndim[0]), ny(ndim[1]), nz(ndim[2]), x(x), y(y), z(z) {}

  /// ポリゴン検索領域を指定するメソッド.
  ///
  ///  @param[in] i,j,k  3次元インデックス
  ///  @param[out] center 計算基準点座標
  ///  @param[out] range  6方向毎の計算基準線分の長さ
  ///
  void getSearchRange(int i, int j, int k,
                      double center[3], double range[6]) const {
    assert(0 <= i && i < nx);
    assert(0 <= j && j < ny);
    assert(0 <= k && k < nz);

    center[0] = 0.5 * (x[i] + x[i+1]);
    center[1] = 0.5 * (y[j] + y[j+1]);
    center[2] = 0.5 * (z[k] + z[k+1]);

    if (i == 0)    range[X_M] = 0.5 * (x[i+1] - x[i]);
    else           range[X_M] = 0.5 * (x[i+1] - x[i-1]);

    if (i == nx-1) range[X_P] = 0.5 * (x[i+1] - x[i]);
    else           range[X_P] = 0.5 * (x[i+2] - x[i]);

    if (j == 0)    range[Y_M] = 0.5 * (y[j+1] - y[j]);
    else           range[Y_M] = 0.5 * (y[j+1] - y[j-1]);

    if (j == ny-1) range[Y_P] = 0.5 * (y[j+1] - y[j]);
    else           range[Y_P] = 0.5 * (y[j+2] - y[j]);

    if (k == 0)    range[Z_M] = 0.5 * (z[k+1] - z[k]);
    else           range[Z_M] = 0.5 * (z[k+1] - z[k-1]);

    if (k == nz-1) range[Z_P] = 0.5 * (z[k+1] - z[k]);
    else           range[Z_P] = 0.5 * (z[k+2] - z[k]);

  }

};

// @}

} // namespace cutlib

#endif // GRID_ACCESSOR_NON_UNIFORM_CELL_H
