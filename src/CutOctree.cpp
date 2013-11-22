/// @file
/// @brief Octree用クラス, 関数 実装
///

#include "CutOctree.h"
#include "CutSearch.h"

#ifdef CUTLIB_TIMING
#include "CutTiming.h"
#endif

#include <vector>
#include <algorithm>   // for min, max

namespace cutlib {
namespace cutOctree {

/// コンストラクタ.
///
///  @param[in] t Polylib三角形ポリゴンクラス
///  @param[in] bid 境界ID
///
CutTriangle::CutTriangle(Triangle* t, BidType bid) : t(t), bid(bid)
{
  Vec3f* v = t->get_vertex();
  bboxMin[X] = std::min(std::min(v[0][X], v[1][X]), v[2][X]);
  bboxMin[Y] = std::min(std::min(v[0][Y], v[1][Y]), v[2][Y]);
  bboxMin[Z] = std::min(std::min(v[0][Z], v[1][Z]), v[2][Z]);
  bboxMax[X] = std::max(std::max(v[0][X], v[1][X]), v[2][X]);
  bboxMax[Y] = std::max(std::max(v[0][Y], v[1][Y]), v[2][Y]);
  bboxMax[Z] = std::max(std::max(v[0][Z], v[1][Z]), v[2][Z]);
}


/// 三角形が直方体領域と交わるかの判定.
///
///  @param[in] min,max 直方体頂点座標
///  @return true:交わる/false:交わらない
///
bool CutTriangle::intersectBox(const Vec3f& min, const Vec3f& max)
{
  if (bboxMin[X] > max[X] || bboxMax[X] < min[X]) return false;
  if (bboxMin[Y] > max[Y] || bboxMax[Y] < min[Y]) return false;
  if (bboxMin[Z] > max[Z] || bboxMax[Z] < min[Z]) return false;
  return true;
}


/// Polylib検索メソッドの結果をカスタムリストに追加.
///
///  @param[in,out] ctList 三角形リスト
///  @param[in] pl Polylibクラスオブジェクト
///  @param[in] bList (境界ID,ポリゴングループ名)対応リスト
///  @param[in] min,max 検索領域
///
void CutTriangle::AppendCutTriangles(CutTriangles& ctList, 
                                     const Polylib* pl, const CutBoundaries* bList,
                                     const Vec3f& min, const Vec3f& max)
{
  CutBoundaries::const_iterator b;
  for (b = bList->begin(); b != bList->end(); ++b) {

#ifdef CUTLIB_TIMING
    Timer::Start(SEARCH_POLYGON);
#endif

    std::vector<Triangle*>* tList = pl->search_polygons(b->name, min, max, false);

#ifdef CUTLIB_TIMING
    Timer::Stop(SEARCH_POLYGON);
#endif

    std::vector<Triangle*>::const_iterator t;
    for (t = tList->begin(); t != tList->end(); ++t) {
      CutTriangle* ct = new CutTriangle(*t, b->id);
      ctList.push_back(ct);
    }
  delete tList;
  }
}


/// 直方体領域と交わる三角形のリストをコピー.
///
///  @param[in] ctListFrom 三角形リスト コピー元
///  @param[out] ctListTo 三角形リスト コピー先
///  @param[in] min,max 直方体領域
///
void CutTriangle::CopyCutTriangles(const CutTriangles& ctListFrom, 
                                   CutTriangles& ctListTo,
                                   const Vec3f& min, const Vec3f& max)
{
  ctListTo.clear();
  CutTriangles::const_iterator ct;
  for (ct = ctListFrom.begin(); ct != ctListFrom.end(); ++ct) {
    if ((*ct)->intersectBox(min, max)) ctListTo.push_back(*ct);
  }
}
                                          

/// リスト内の三角形オブジェクトを消去.
///
///  @param[in,out] ctList 三角形リスト
///
void CutTriangle::DeleteCutTriangles(CutTriangles& ctList)
{
  CutTriangles::iterator ct;
  for (ct = ctList.begin(); ct != ctList.end(); ++ct) {
    delete *ct;
  }
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
                 const CutTriangles& ctList)
{
#ifdef CUTLIB_DEBUG
  std::cout << ctList.size() << "@" << cell->GetMyLevel() << std::endl;
#endif

  double pos6[6];
  float pos6_f[6];
  BidType bid6[6];
  Triangle* tri6[6];
  double center[3];
  double range[6];

  for (int l = 0; l < 3; l++) center[l] = org[l] + 0.5 * d[l];
  range[X_M] = range[X_P] = d[X];
  range[Y_M] = range[Y_P] = d[Y];
  range[Z_M] = range[Z_P] = d[Z];

  CutSearch::clearCutInfo(range, pos6, bid6, tri6);

  cutPos->assignData(cell->GetData());
  cutBid->assignData(cell->GetData());

  CutTriangles::const_iterator ct;
  for (ct = ctList.begin(); ct != ctList.end(); ++ct) {
    CutSearch::checkTriangle((*ct)->t, (*ct)->bid, center, range,
                             pos6, bid6, tri6);
  }

  for (int d = 0; d < 6; d++) pos6_f[d] = (float)(pos6[d]/range[d]);

  cutPos->setPos(pos6_f);
  cutBid->setBid(bid6);

  if (cell->hasChild()) {
    for (TdPos p = 0; p < 8; p++) {
      SklCell* cellChild = cell->GetChildCell(p);
      Vec3f orgChild, dChild;
      cellChild->GetOrigin(orgChild[0], orgChild[1], orgChild[2]);
      cellChild->GetPitch(dChild[0], dChild[1], dChild[2]);
      Vec3f min = orgChild - 0.5 * dChild;
      Vec3f max = orgChild + 1.5 * dChild;

      CutTriangles ctListChild;
      CutTriangle::CopyCutTriangles(ctList, ctListChild, min, max);
      calcCutInfo(cellChild, orgChild, dChild, cutPos, cutBid, ctListChild);
    }
  }
}


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
                  CutPosOctree* cutPos, CutBidOctree* cutBid)
{
  double pos6[6];
  float pos6_f[6];
  BidType bid6[6];
  Triangle* tri6[6];
  double center[3];
  double range[6];

  cutPos->assignData(cell->GetData());
  cutBid->assignData(cell->GetData());

  cutOctree::getSearchRange(cell, center, range);
  cutSearch->search(center, range, pos6, bid6, tri6);

  for (int d = 0; d < 6; d++) pos6_f[d] = (float)(pos6[d]/range[d]);

  cutPos->setPos(pos6_f);
  cutBid->setBid(bid6);

//cout << "L(" << cell->GetMyLevel() << ")P(" << cell->GetMyPos() << ") ";

  if (cell->hasChild()) {
    for (TdPos pos = 0; pos < 8; pos++) {
      SklCell* childCell = cell->GetChildCell(pos);
      calcCutInfo0(childCell, cutSearch, cutPos, cutBid);
    }
  }
}

} // namespace cutOctree
} // namespace cutlib
