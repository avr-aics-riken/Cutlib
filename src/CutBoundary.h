/// @file
/// @brief Polylibポリゴングループ--境界ID 対応付けクラス 宣言
///

#ifndef CUT_BOUNDARY_H
#define CUT_BOUNDARY_H

#include "Cutlib.h"
#include "Polylib.h"
using namespace PolylibNS;

#include <string>
#include <vector>

namespace cutlib {

class CutBoundary;

/// Polylibポリゴングループ--境界ID 対応リスト.
typedef std::vector<CutBoundary> CutBoundaries;


///  Polylibポリゴングループ--境界ID 対応付けクラス.
class CutBoundary {
public:
  std::string name;  ///< ポリゴングループ名
  BidType id;        ///< 境界ID

  /// コンストラクタ.
  /**
   * @param[in] s ポリゴングループ名
   * @param[in] i 境界ID
   */
  CutBoundary(const std::string s, BidType i) : name(s), id(i) {}

  /// デストラクタ.
  ~CutBoundary() {}

  /// Polylibポリゴングループ--境界ID 対応リスト生成.
  ///
  ///  @param[in] pl Polylibクラスオブジェクト
  ///  @return Polylibポリゴングループ--境界ID 対応リスト
  ///
  static CutBoundaries* createCutBoundaryList(const Polylib* pl);


private:

  /// ポリゴングループのチェック.
  ///
  /// ポリゴングループのツリーを再帰的にたどり,
  /// リーフノードかつIDが1以上のグループをリストに登録
  ///
  ///  @param[in] pg Polylibポリゴングループ
  ///  @param[in,out] bList Polylibポリゴングループ--境界ID 対応リスト
  ///
  static void checkPolygonGroup(PolygonGroup *pg, CutBoundaries* bList);
};

} // namespace cutlib

#endif // CUT_BOUNDARY_H
