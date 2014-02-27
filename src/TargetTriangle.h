/// @file
/// @brief 交点計算可能な三角形クラス 宣言
///

#ifndef TARGET_TRIANGLE_H
#define TARGET_TRIANGLE_H

#include "Polylib.h"
using namespace PolylibNS;

namespace cutlib {

/// 交点計算可能な三角形クラス.
class TargetTriangle {

  /// 倍精度2次元ベクトルクラス.
  typedef Vec2<double> Vec2d;

  enum { X, Y, Z };

  double normal[3];     ///< 法線ベクトル
  double vertex[3][3];  ///< 頂点座標

  double dot_normal_vertex0;  ///< 法線ベクトルと頂点vertex[0]の内積値

public:

  /// コンストラクタ.
  ///
  ///  @param[in] t  対象三角形ポリゴン
  ///
  TargetTriangle(const Triangle* t) {
    Vertex  n = t->get_normal();
    Vertex** v = t->get_vertex();

    dot_normal_vertex0 = 0.0;
    for (int i = 0; i < 3; i++) {
      normal[i] = n[i];
      vertex[0][i] = (*v[0])[i];
      vertex[1][i] = (*v[1])[i];
      vertex[2][i] = (*v[2])[i];
      dot_normal_vertex0 += normal[i] * vertex[0][i];
    }
  }


  /// デストラクタ.
  ~TargetTriangle() {}


  /// x軸に平行な直線との交点を計算.
  ///
  ///  @param[in] y,z 直線位置
  ///  @param[out] x  交点座標
  ///  @return true:交点あり/false:交点なし
  ///
  bool intersectX(double y, double z, double& x) const;


  /// y軸に平行な直線との交点を計算.
  ///
  ///  @param[in] z,x 直線位置
  ///  @param[out] y  交点座標
  ///  @return true:交点あり/false:交点なし
  ///
  bool intersectY(double z, double x, double& y) const;


  /// z軸に平行な直線との交点を計算.
  ///
  ///  @param[in] x,y 直線位置
  ///  @param[out] z  交点座標
  ///  @return true:交点あり/false:交点なし
  ///
  bool intersectZ(double x, double y, double& z) const;


private:

  /// 2次元外積計算.
  ///
  ///  @param[in] v1,v2  対象2次元ベクトル
  ///  @return  外積値
  ///
  double cross2d(const Vec2d& v1, const Vec2d& v2) const {
    return v1.x * v2.y - v1.y * v2.x;
  }


  /// 平面内で点pが三角形の内部にあるかの判定.
  ///
  ///  @param[in] n 正:v0,v1,v2が反時計周り/負:時計周り
  ///  @param[in] p 判定対象点
  ///  @param[in] v0,v1,v2 三角形の頂点座標
  ///  @return true:内部/false:外部
  ///
  bool existIntersection(double n, const Vec2d& p,
                         const Vec2d& v0, const Vec2d& v1, const Vec2d& v2) const {
    if (n > 0.0) {
      if (cross2d(v1-v0, p-v0) < 0.0 ||
          cross2d(v2-v1, p-v1) < 0.0 ||
          cross2d(v0-v2, p-v2) < 0.0) return false;
    } else {
      if (cross2d(v1-v0, p-v0) > 0.0 ||
          cross2d(v2-v1, p-v1) > 0.0 ||
          cross2d(v0-v2, p-v2) > 0.0) return false;
    }
    return true;
  }

};


} // namespace cutlib

#endif // TARGET_TRIANGLE_H

