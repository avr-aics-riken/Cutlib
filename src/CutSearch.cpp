/// @file
/// @brief 交点情報計算クラス 実装
///

#include "CutSearch.h"
#include "TargetTriangle.h"

#ifdef CUTLIB_TIMING
#include "CutTiming.h"
#endif

namespace cutlib {

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
void CutSearch::search(const double center[], const double range[],
                       double pos6[], BidType bid6[],
                       Triangle* tri6[]) const
{
  Vec3r min(center[X]-range[X_M], center[Y]-range[Y_M], center[Z]-range[Z_M]);
  Vec3r max(center[X]+range[X_P], center[Y]+range[Y_P], center[Z]+range[Z_P]);

  clearCutInfo(range, pos6, bid6, tri6);

  std::vector<std::string>::const_iterator pg;
  for (pg = pgList->begin(); pg != pgList->end(); ++pg) {

#ifdef CUTLIB_TIMING
    Timer::Start(SEARCH_POLYGON);
#endif

    std::vector<Triangle*>* tList = pl->search_polygons(*pg, min, max, false);

#ifdef CUTLIB_TIMING
    Timer::Stop(SEARCH_POLYGON);
#endif

    std::vector<Triangle*>::const_iterator t;
    for (t = tList->begin(); t != tList->end(); ++t) {
      int exid = (*t)->get_exid();
      if (0 < exid && exid < 256) {
        BidType bid = exid;
        checkTriangle(*t, bid, center, range, pos6, bid6, tri6);
      }
    } 

    delete tList;
  }
}


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
void CutSearch::checkTriangle(Triangle* t, BidType bid,
                              const double center[], const double range[],
                              double pos6[], BidType bid6[],
                              Triangle* tri6[])
{
  TargetTriangle triangle(t);
  double p, pos;

  if (triangle.intersectX(center[Y], center[Z], p)) {
    if (p >= center[X]) {
      pos = p - center[X];
      if (pos < pos6[X_P]) {
        pos6[X_P] = pos;
        bid6[X_P] = bid;
        tri6[X_P] = t;
      }
    }
    if (p <= center[X]) {
      pos = center[X] - p;
      if (pos < pos6[X_M]) {
        pos6[X_M] = pos;
        bid6[X_M] = bid;
        tri6[X_M] = t;
      }
    }
  }

  if (triangle.intersectY(center[Z], center[X], p)) {
    if (p >= center[Y]) {
      pos = p - center[Y];
      if (pos < pos6[Y_P]) {
        pos6[Y_P] = pos;
        bid6[Y_P] = bid;
        tri6[Y_P] = t;
      }
    }
    if (p <= center[Y]) {
      pos = center[Y]- p;
      if (pos < pos6[Y_M]) {
        pos6[Y_M] = pos;
        bid6[Y_M] = bid;
        tri6[Y_M] = t;
      }
    }
  }

  if (triangle.intersectZ(center[X], center[Y], p)) {
    if (p >= center[Z]) {
      pos = p - center[Z];
      if (pos < pos6[Z_P]) {
        pos6[Z_P] = pos;
        bid6[Z_P] = bid;
        tri6[Z_P] = t;
      }
    }
    if (p <= center[Z]) {
      pos = center[Z] - p;
      if (pos < pos6[Z_M]) {
        pos6[Z_M] = pos;
        bid6[Z_M] = bid;
        tri6[Z_M] = t;
      }
    }
  }

}


} // namespace cutlib
