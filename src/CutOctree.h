/// @file
/// @brief Octree用クラス, 関数 宣言
///

#ifndef CUTLIB_OCTREE_H
#define CUTLIB_OCTREE_H

#include "Cutlib.h"
#include "CutBoundary.h"
#include "CutSearch.h"

namespace cutlib {
namespace cutOctree {

enum { X, Y, Z };

class CutTriangle;

/// 三角形リスト.
typedef std::vector<CutTriangle*> CutTriangles;


/// BBox(binding box)情報と境界IDを持つカスタムポリゴンクラス.
class CutTriangle {

public:

  Triangle* t;    ///< Polylib三角形ポリゴンクラス
  BidType bid;    ///< 境界ID
  Vec3f bboxMin;  ///< BBox最小値
  Vec3f bboxMax;  ///< BBox最大値

  /// コンストラクタ.
  ///
  ///  @param[in] t Polylib三角形ポリゴンクラス
  ///  @param[in] bid 境界ID
  ///
  CutTriangle(Triangle* t, BidType bid);

  /// 三角形が直方体領域と交わるかの判定.
  ///
  ///  @param[in] min,max 直方体頂点座標
  ///  @return true:交わる/false:交わらない
  ///
  bool intersectBox(const Vec3f& min, const Vec3f& max);

  /// Polylib検索メソッドの結果をカスタムリストに追加.
  ///
  ///  @param[in,out] ctList 三角形リスト
  ///  @param[in] pl Polylibクラスオブジェクト
  ///  @param[in] bList (境界ID,ポリゴングループ名)対応リスト
  ///  @param[in] min,max 検索領域
  ///
  static void AppendCutTriangles(CutTriangles& ctList, 
                                 const Polylib* pl, const CutBoundaries* bList,
                                 const Vec3f& min, const Vec3f& max);

  /// 直方体領域と交わる三角形のリストをコピー.
  ///
  ///  @param[in] ctListFrom 三角形リスト コピー元
  ///  @param[out] ctListTo 三角形リスト コピー先
  ///  @param[in] min,max 直方体領域
  ///
  static void CopyCutTriangles(const CutTriangles& ctListFrom, 
                               CutTriangles& ctListTo,
                               const Vec3f& min, const Vec3f& max);

  /// リスト内の三角形オブジェクトを消去.
  ///
  ///  @param[in,out] ctList 三角形リスト
  ///
  static void DeleteCutTriangles(CutTriangles& ctList);
};


/// 検索領域を取得.
///
///  @param[in]  cell  SklCellセル
///  @param[out] center 検索基準点位置
///  @param[out] range  各6方向の検索基準線分長
///
inline
void getSearchRange(SklCell* cell, double center[], double range[]) {
  float o[3], d[3];
  cell->GetOrigin(o[0], o[1], o[2]);
  cell->GetPitch(d[0], d[1], d[2]);
  for (int i = 0; i < 3; i++) center[i] = o[i] + 0.5 * d[i];
  range[X_M] = range[X_P] = d[0];
  range[Y_M] = range[Y_P] = d[1];
  range[Z_M] = range[Z_P] = d[2];
}


/// Octree上のセルでの交点情報を計算.
///
///  再帰的に呼び出される
///
///  @param[in,out]  cell  SklCellセル
///  @param[in] center セル中心座標
///  @param[in] d セルピッチ
///  @param cutPos 交点座標データアクセッサ
///  @param cutBid 境界IDデータアクセッサ
///  @param[in] ctList ポリゴンリスト
///
void calcCutInfo(SklCell* cell, const Vec3f& org, const Vec3f& d,
                 CutPosOctree* cutPos, CutBidOctree* cutBid,
                 const CutTriangles& ctList);


/// Octree上のセルでの交点情報を計算(デバッグ用).
///
///  Polylibの検索メソッドを使用，再帰的に呼び出される
///
///  @param[in,out]  cell  SklCellセル
///  @param[in] cutSearch  交点検索クラスオブジェクト
///  @param cutPos 交点座標データアクセッサ
///  @param cutBid 境界IDデータアクセッサ
///
void calcCutInfo0(SklCell* cell, const CutSearch* cutSearch,
                  CutPosOctree* cutPos, CutBidOctree* cutBid);


} // namespace cutOctree
} // namespace cutlib

#endif // CUTLIB_OCTREE_H