/// @file
/// @brief ポリゴンデータ最適化関数 実装
///

#include "RepairPolygonData/RepairPolygonData.h"

namespace cutlib {

namespace {

/// ポリゴングループに所属する三角形ポリゴンデータを最適化.
///
///  @param[in,out] pg 対象ポリゴングループ
///  @param[in] doubt_vertex_order 頂点並び順を最適化するフラグ
///  @param[in] doubt_normal_quality 法線ベクトルを再計算するフラグ
///
void repairPolygons(PolygonGroup *pg,
                   bool doubt_vertex_order, bool doubt_normal_quality)
{
  std::vector<PrivateTriangle*>* polygonList = pg->get_triangles();
  std::vector<PrivateTriangle*>::iterator p;
  for (p = polygonList->begin(); p != polygonList->end(); p++) {
    bool reset = false;
    Vertex** vertex0 = (*p)->get_vertex();
    Vertex *vertex[3] = { vertex0[0], vertex0[1], vertex0[2] }; 

    if (doubt_normal_quality) reset = true;

    if (doubt_vertex_order) {
      Vec3r normal0 = (*p)->get_normal();
      Vec3r normal = cross(*vertex0[1]-*vertex0[0], *vertex0[2]-*vertex0[0]); 
      if (dot(normal, normal0) < 0.0) {
        // 節点1と節点2を入れ替え
        vertex[1] = vertex0[2];  
        vertex[2] = vertex0[1];  
        reset = true;
      }
    }

    if (reset) (*p)->set_vertexes(vertex, doubt_normal_quality, true);
  }
}

} // namespace ANONYMOUS


/// ポリゴンデータを最適化する.
///
///  @param[in,out] pl Polylibクラスオブジェクト
///  @param[in] doubt_vertex_order 頂点並び順を最適化するフラグ
///  @param[in] doubt_normal_quality 法線ベクトルを再計算するフラグ
///
void RepairPolygonData(const Polylib* pl,
                       bool doubt_vertex_order, bool doubt_normal_quality)
{
  if (!doubt_vertex_order && !doubt_normal_quality) return;

  std::vector<PolygonGroup *>* leafGroups = pl->get_leaf_groups();
  std::vector<PolygonGroup*>::iterator pg;
  for (pg = leafGroups->begin(); pg != leafGroups->end(); ++pg) {
    repairPolygons(*pg, doubt_vertex_order, doubt_normal_quality);
  }
  delete leafGroups;
}


} // namespace cutlib
