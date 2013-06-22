//
// Cutlib Version 3.0.0-beta,  Feb.25.2013
//

///
/// @file
/// @brief 境界情報計算関数 宣言
///

#ifndef CUTLIB_H
#define CUTLIB_H

#include "Polylib.h"
using namespace PolylibNS;

#include "RepairPolygonData/RepairPolygonData.h"

#include "CutInfo/CutInfoArray.h"
#include "GridAccessor/GridAccessor.h"

#ifdef CUTLIB_OCTREE
#include "SklCompatibility.h"
#include "CutInfo/CutInfoOctree.h"
#endif

namespace cutlib {

/// @defgroup CutInfoFunc 交点情報計算関数
//@{

/// 交点情報計算関数リターンコード.
enum CutlibReturn {
  CL_SUCCESS = 0,         ///< 成功
  CL_BAD_GROUP_LIST = 1,  ///< (境界ID,ポリゴングループ名)対応リストが不正
  CL_BAD_POLYLIB = 2,     ///< Polylibオブジェクトが不正(未初期化等)
  CL_BAD_SKLTREE = 3,     ///< SklTreeオブジェクトが不正(未初期化等)
  CL_SIZE_EXCEED = 4,     ///< ista[]+nlen[]が配列サイズを越えている
  CL_OTHER_ERROR = 10,    ///< その他のエラー
};


/// 交点情報計算: 計算領域指定.
///
///  @param[in] ista 計算基準点開始位置3次元インデクス
///  @param[in] nlen 計算基準点3次元サイズ
///  @param[in] grid GridAccessorクラスオブジェクト
///  @param[in] pl Polylibクラスオブジェクト
///  @param[in,out] cutPos 交点座標配列ラッパ
///  @param[in,out] cutBid 境界ID配列ラッパ
///
CutlibReturn CalcCutInfo(const int ista[], const size_t nlen[],
                         const GridAccessor* grid, const Polylib* pl,
                         CutPosArray* cutPos, CutBidArray* cutBid);


/// 交点情報計算: 全領域.
///
///  @param[in] grid GridAccessorクラスオブジェクト
///  @param[in] pl Polylibクラスオブジェクト
///  @param[in,out] cutPos 交点座標配列ラッパ
///  @param[in,out] cutBid 境界ID配列ラッパ
///
inline 
CutlibReturn CalcCutInfo(const GridAccessor* grid, const Polylib* pl,
                         CutPosArray* cutPos, CutBidArray* cutBid) {
  int ista[3] = { 
    cutPos->getStartX(),
    cutPos->getStartY(),
    cutPos->getStartZ(),
  };
  size_t nlen[3] = { 
    cutPos->getSizeX(),
    cutPos->getSizeY(),
    cutPos->getSizeZ()
  };
  return CalcCutInfo(ista, nlen, grid, pl, cutPos, cutBid);
}


#ifdef CUTLIB_OCTREE

/// 交点情報計算: Octree, リーフセルのみ.
///
///  @param[in,out] tree SklTreeクラスオブジェクト
///  @param[in] pl Polylibクラスオブジェクト
///  @param cutPos 交点座標データアクセッサ
///  @param cutBid 境界IDデータアクセッサ
///
CutlibReturn CalcCutInfoOctreeLeafCell(SklTree* tree, const Polylib* pl,
                        CutPosOctree* cutPos, CutBidOctree* cutBid);


/// 交点情報計算: Octree, 全セル計算.
///
///  @param[in,out] tree SklTreeクラスオブジェクト
///  @param[in] pl Polylibクラスオブジェクト
///  @param cutPos 交点座標データアクセッサ
///  @param cutBid 境界IDデータアクセッサ
///
CutlibReturn CalcCutInfoOctreeAllCell(SklTree* tree, const Polylib* pl,
                        CutPosOctree* cutPos, CutBidOctree* cutBid);


/// 交点情報計算: Octree, 全セル計算, デバッグ用.
///
/// 全セルでPolylibの検索メソッドを使用
///
///  @param[in,out] tree SklTreeクラスオブジェクト
///  @param[in] pl Polylibクラスオブジェクト
///  @param cutPos 交点座標データアクセッサ
///  @param cutBid 境界IDデータアクセッサ
///
CutlibReturn CalcCutInfoOctreeAllCell0(SklTree* tree, const Polylib* pl,
                              CutPosOctree* cutPos, CutBidOctree* cutBid);


/// 交点情報計算: Octree, 計算対象セルタイプ指定.
///
///  @param[in,out] tree SklTreeクラスオブジェクト
///  @param[in] pl Polylibクラスオブジェクト
///  @param cutPos 交点座標データアクセッサ
///  @param cutBid 境界IDデータアクセッサ
///  @param[in] leafCellOnly 計算対象セルタイプフラグ
///
inline
CutlibReturn CalcCutInfoOctree(SklTree* tree,
                        const Polylib* pl,
                        CutPosOctree* cutPos, CutBidOctree* cutBid,
                        bool leafCellOnly = true) {
  if (leafCellOnly) {
    return CalcCutInfoOctreeLeafCell(tree, pl, cutPos, cutBid);
  } else {
    return CalcCutInfoOctreeAllCell(tree, pl, cutPos, cutBid);
  }
}

#endif //CUTLIB_OCTREE

//@} 

} // namespace cutlib

#endif // CUTLIB_H
