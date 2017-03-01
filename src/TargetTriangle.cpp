/*
###################################################################################
#
# Cutlib - Cut Information Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
# All rights reserved.
#
# Copyright (c) 2012-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
# Copyright (c) 2016-2017 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
*/

/// @file
/// @brief 交点計算可能な三角形クラス 実装
///

#include "TargetTriangle.h"

namespace cutlib {

/// x軸に平行な直線との交点を計算.
///
///  @param[in] y,z 直線位置
///  @param[out] x  交点座標
///  @return true:交点あり/false:交点なし
///
bool TargetTriangle::intersectX(double y, double z, double& x) const
{
  if (normal[X] == 0.0) return false;

  Vec2d v0(vertex[0][Y], vertex[0][Z]);
  Vec2d v1(vertex[1][Y], vertex[1][Z]);
  Vec2d v2(vertex[2][Y], vertex[2][Z]);
  Vec2d p(y, z);

  if (!existIntersection(normal[X], p, v0, v1, v2)) return false;

  x = (dot_normal_vertex0 - normal[Y]*y - normal[Z]*z) / normal[X];

  return true;
}


/// y軸に平行な直線との交点を計算.
///
///  @param[in] z,x 直線位置
///  @param[out] y  交点座標
///  @return true:交点あり/false:交点なし
///
bool TargetTriangle::intersectY(double z, double x, double& y) const
{
  if (normal[Y] == 0.0) return false;

  Vec2d v0(vertex[0][Z], vertex[0][X]);
  Vec2d v1(vertex[1][Z], vertex[1][X]);
  Vec2d v2(vertex[2][Z], vertex[2][X]);
  Vec2d p(z, x);

  if (!existIntersection(normal[Y], p, v0, v1, v2)) return false;

  y = (dot_normal_vertex0 - normal[Z]*z - normal[X]*x) / normal[Y];

  return true;
}


/// z軸に平行な直線との交点を計算.
///
///  @param[in] x,y 直線位置
///  @param[out] z  交点座標
///  @return true:交点あり/false:交点なし
///
bool TargetTriangle::intersectZ(double x, double y, double& z) const
{
  if (normal[Z] == 0.0) return false;

  Vec2d v0(vertex[0][X], vertex[0][Y]);
  Vec2d v1(vertex[1][X], vertex[1][Y]);
  Vec2d v2(vertex[2][X], vertex[2][Y]);
  Vec2d p(x, y);

  if (!existIntersection(normal[Z], p, v0, v1, v2)) return false;

  z = (dot_normal_vertex0 - normal[X]*x - normal[Y]*y) / normal[Z];

  return true;
}


} // namespace cutlib
