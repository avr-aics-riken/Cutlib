/// @file
/// @brief 交点情報計算クラス 宣言
///

#ifndef CUTLIB_SEARCH_H
#define CUTLIB_SEARCH_H

#include <string>
#include <vector>

#include "GridAccessor/GridAccessor.h"
#include "CutInfo/CutInfoArray.h"

#include "Polylib.h"
using namespace PolylibNS;

namespace cutlib {

/// 交点情報計算クラス.
class CutSearch {

  const Polylib* pl;    ///< Polylibクラスオブジェクト
  const std::vector<std::string>* pgList; ///< ポリゴングループ(パス名)リスト

  enum { X, Y, Z};

public:

  /// コンストラクタ.
  ///
  ///  @param[in] pl Polylibクラスオブジェクト
  ///  @param[in] bList (境界ID,ポリゴングループ名)対応リスト
  ///
  CutSearch(const Polylib* pl, const std::vector<std::string>* pgList)
    : pl(pl), pgList(pgList) {}


  /// デストラクタ.
  ~CutSearch() {}


  /// 最近接交点の探索.
  ///
  ///  @param[in] center 計算基準点座標
  ///  @param[in] range  6方向毎の計算基準線分の長さ
  ///  @param[out] pos6  交点座標値配列
  ///  @param[out] bid6  境界ID配列
  ///  @param[out] tri6  交点ポリゴンポインタ配列
  ///
  ///  @note pos6には計算基準線分長で規格化する前の値を格納
  ///
  void search(const double center[], const double range[],
              double pos6[], BidType bid6[], Triangle* tri6[]) const;


  /// 三角形ポリゴンの交点調査.
  ///
  ///  @param[in] t      対象三角形ポリゴン
  ///  @param[in] bid    対象三角形ポリゴンの境界ID
  ///  @param[in] center 計算基準点座標
  ///  @param[in] range  6方向毎の計算基準線分の長さ
  ///  @param[in,out] pos6  交点座標値配列
  ///  @param[in,out] bid6  境界ID配列
  ///  @param[in,out] tri6  交点ポリゴンポインタ配列
  ///
  ///  @note pos6には計算基準線分長で規格化する前の値を格納
  ///
  static void checkTriangle(Triangle* t, BidType bid,
                            const double center[], const double range[],
                            double pos6[], BidType bid6[],
                            Triangle* tri6[]);


  /// 交点情報配列の初期化.
  ///
  ///  @param[in] range  6方向毎の計算基準線分の長さ
  ///  @param[out] pos6  交点座標値配列
  ///  @param[out] bid6  境界ID配列
  ///  @param[out] tri6  交点ポリゴンポインタ配列
  ///
  ///  @note pos6はrangeで初期化される
  ///
  static void clearCutInfo(const double range[],
                   double pos6[], BidType bid6[], Triangle* tri6[]) {
    for (int i = 0; i < 6; i++) {
      pos6[i] = range[i];
      bid6[i] = 0;
      tri6[i] = 0;
    }
  }

};

} // namespace cutlib

#endif // CUTLIB_SEARCH_H

