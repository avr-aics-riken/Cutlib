/// @file
/// @brief 境界情報計算関数 実装
///

#include <string>
#include <vector>

#include "Cutlib.h"
#include "CutSearch.h"

#ifdef CUTLIB_OCTREE
#include "CutOctree.h"
#endif

#ifdef CUTLIB_TIMING
#include "CutTiming.h"
#endif

#ifdef _OPENMP
#include "omp.h"
#endif

namespace cutlib {

namespace {

/// 配列サイズ, 計算対象領域のチェック.
CutlibReturn checkSize(const char* funcName, const char* className,
                       const int ista[], const size_t nlen[],
                       const CutInfoArray* cutInfoArray)
{
  if (ista[0] < cutInfoArray->getStartX() ||
      ista[1] < cutInfoArray->getStartY() ||
      ista[2] < cutInfoArray->getStartZ()) {
    std::cerr << "*** " << funcName << ": " << className << ": "
              << "out of the range: ista[]" << std::endl;
    return CL_SIZE_EXCEED;
  }

  if (ista[0] + nlen[0] > cutInfoArray->getStartX() + cutInfoArray->getSizeX() ||
      ista[1] + nlen[1] > cutInfoArray->getStartY() + cutInfoArray->getSizeY() ||
      ista[2] + nlen[2] > cutInfoArray->getStartZ() + cutInfoArray->getSizeZ()) {
    std::cerr << "*** " << funcName << ": " << className << ": "
              << "out of the range: ista[]+nlen[]" << std::endl;
    return CL_SIZE_EXCEED;
  }
  return CL_SUCCESS;
}


/// Polylibのチェック.
CutlibReturn checkPolylib(const char* func_name, const Polylib* pl)
{
  if (pl == 0) {
    std::cerr << "*** " << func_name << ": Polylib not initialized." << std::endl;
    return CL_BAD_POLYLIB;
  }
  return CL_SUCCESS;
}


#ifdef CUTLIB_OCTREE

/// SklTreeのチェック.
CutlibReturn checkTree(const char* func_name, SklTree* tree)
{
  if (tree == 0) {
    std::cerr << "*** " << func_name << ": SklTree not initialized." << std::endl;
    return CL_BAD_SKLTREE;
  }
  return CL_SUCCESS;
}

#endif //CUTLIB_OCTREE


/// 計算対象ポリゴングループのパス名リストを作成.
std::vector<std::string>* createPolygonGroupPathList(const Polylib* pl)
{
  std::vector<std::string>* pgList = new std::vector<std::string>;
  std::vector<PolygonGroup *>* leafGroups = pl->get_leaf_groups();
  std::vector<PolygonGroup*>::iterator it = leafGroups->begin();
  for (it = leafGroups->begin(); it != leafGroups->end(); ++it) {
    pgList->push_back((*it)->acq_fullpath());
  }
  delete leafGroups;
  return pgList;
}

} // namespace ANONYMOUS


/// 交点情報計算: 計算領域指定.
///
///  @param[in] ista 計算基準点開始位置3次元インデクス
///  @param[in] nlen 計算基準点3次元サイズ
///  @param[in] grid GridAccessorクラスオブジェクト
///  @param[in] pl Polylibクラスオブジェクト
///  @param[in,out] cutPos 交点座標配列ラッパ
///  @param[in,out] cutBid 境界ID配列ラッパ
///  @param[in,out] cutNormal 法線ベクトル格納クラス
///
CutlibReturn CalcCutInfo(const int ista[], const size_t nlen[],
                         const GridAccessor* grid, const Polylib* pl,
                         CutPosArray* cutPos, CutBidArray* cutBid,
                         CutNormalArray* cutNormal)
{
  { 
    // check input parameters
    CutlibReturn ret;
    ret = checkSize("CalcCutInfo", "cutPos", ista, nlen, cutPos);
    if (ret != CL_SUCCESS) return ret;
    ret = checkSize("CalcCutInfo", "cutBid", ista, nlen, cutBid);
    if (ret != CL_SUCCESS) return ret;
    if (cutNormal) {
      ret = checkSize("CalcCutInfo", "cutNormal", ista, nlen, cutNormal);
      if (ret != CL_SUCCESS) return ret;
    }
    ret = checkPolylib("CalcCutInfo", pl);
    if (ret != CL_SUCCESS) return ret;
  }

#ifdef CUTLIB_TIMING
  Timer::Start(TOTAL);
#endif

  std::vector<std::string>* pgList = createPolygonGroupPathList(pl);

  CutSearch* cutSearch = new CutSearch(pl, pgList);

  cutPos->clear();
  cutBid->clear();

  CutPolygonList* cutPolygonList = 0;
  int nThread;
#ifdef _OPENMP
  nThread = omp_get_max_threads();
#else
  nThread = 1;
#endif
  if (cutNormal) cutPolygonList = new CutPolygonList[nThread];

#ifdef CUTLIB_TIMING
  Timer::Start(MAIN_LOOP);
#endif
#pragma omp parallel
  {
  int iThread;
#ifdef _OPENMP
  iThread = omp_get_thread_num();
#else
  iThread = 0;
#endif
#pragma omp for schedule(dynamic), collapse(3)
  for (int k = ista[2]; k < ista[2]+nlen[2]; k++) {
    for (int j = ista[1]; j < ista[1]+nlen[1]; j++) {
      for (int i = ista[0]; i < ista[0]+nlen[0]; i++) {
        double pos6[6];
        float pos6_f[6];
        BidType bid6[6];
        Triangle* tri6[6];
        double center[3];
        double range[6];

#ifdef CUTLIB_TIMING
        Timer::Start(THREAD_TOTAL);
#endif
        grid->getSearchRange(i, j, k, center, range);
        cutSearch->search(center, range, pos6, bid6, tri6);

        for (int d = 0; d < 6; d++) pos6_f[d] = (float)(pos6[d]/range[d]);

        cutPos->setPos(i, j, k, pos6_f);
        cutBid->setBid(i, j, k, bid6);

        if (cutNormal) {
          for (int d = 0; d < 6; d++) {
            if (bid6[d] > 0) {
              CutPolygon* p = new CutPolygon(cutNormal->getIndex(i, j, k), d, tri6[d]);
              cutPolygonList[iThread].push_back(p);
            }
          }
        }

#ifdef CUTLIB_TIMING
        Timer::Stop(THREAD_TOTAL);
#endif
      }
    }
  }
  } // parallel reagion
#ifdef CUTLIB_TIMING
  Timer::Stop(MAIN_LOOP);
#endif

  if (cutNormal) {
#ifdef CUTLIB_TIMING
    Timer::Start(PACK_NORMAL);
#endif
    cutNormal->setNormalInfo(cutPolygonList, nThread);
#ifdef CUTLIB_TIMING
    Timer::Stop(PACK_NORMAL);
#endif
  }

  delete cutSearch;
  delete pgList;
  delete[] cutPolygonList;

#ifdef CUTLIB_TIMING
  Timer::Stop(TOTAL);
  Timer::Print(TOTAL, "Total");
  Timer::Print(MAIN_LOOP, "Main Loop");
  if (cutNormal) {
    Timer::Print(PACK_NORMAL, "Pack Normal");
  }
  Timer::PrintFull(THREAD_TOTAL, "Theread Total");
  Timer::PrintFull(SEARCH_POLYGON, "Polylib::search_polygons");
#endif

  return CL_SUCCESS;
}


/// 交点情報計算: 計算領域指定.
///
///  @param[in] ista 計算基準点開始位置3次元インデクス
///  @param[in] nlen 計算基準点3次元サイズ
///  @param[in] grid GridAccessorクラスオブジェクト
///  @param[in] pl Polylibクラスオブジェクト
///  @param[in] pgList 計算対象ポリゴングループのパス名リスト
///  @param[in,out] cutPos 交点座標配列ラッパ
///  @param[in,out] cutBid 境界ID配列ラッパ
///  @param[in,out] cutNormal 法線ベクトル格納クラス
///
CutlibReturn CalcCutInfo(const int ista[], const size_t nlen[],
                         const GridAccessor* grid,
												 const Polylib* pl, std::vector<std::string>* pgList,
                         CutPosArray* cutPos, CutBidArray* cutBid,
                         CutNormalArray* cutNormal)
{
  { 
    // check input parameters
    CutlibReturn ret;
    ret = checkSize("CalcCutInfo", "cutPos", ista, nlen, cutPos);
    if (ret != CL_SUCCESS) return ret;
    ret = checkSize("CalcCutInfo", "cutBid", ista, nlen, cutBid);
    if (ret != CL_SUCCESS) return ret;
    if (cutNormal) {
      ret = checkSize("CalcCutInfo", "cutNormal", ista, nlen, cutNormal);
      if (ret != CL_SUCCESS) return ret;
    }
    ret = checkPolylib("CalcCutInfo", pl);
    if (ret != CL_SUCCESS) return ret;
  }

#ifdef CUTLIB_TIMING
  Timer::Start(TOTAL);
#endif

//  std::vector<std::string>* pgList = createPolygonGroupPathList(pl);

  CutSearch* cutSearch = new CutSearch(pl, pgList);

  cutPos->clear();
  cutBid->clear();

  CutPolygonList* cutPolygonList = 0;
  int nThread;
#ifdef _OPENMP
  nThread = omp_get_max_threads();
#else
  nThread = 1;
#endif
  if (cutNormal) cutPolygonList = new CutPolygonList[nThread];

#ifdef CUTLIB_TIMING
  Timer::Start(MAIN_LOOP);
#endif
#pragma omp parallel
  {
  int iThread;
#ifdef _OPENMP
  iThread = omp_get_thread_num();
#else
  iThread = 0;
#endif
#pragma omp for schedule(dynamic), collapse(3)
  for (int k = ista[2]; k < ista[2]+nlen[2]; k++) {
    for (int j = ista[1]; j < ista[1]+nlen[1]; j++) {
      for (int i = ista[0]; i < ista[0]+nlen[0]; i++) {
        double pos6[6];
        float pos6_f[6];
        BidType bid6[6];
        Triangle* tri6[6];
        double center[3];
        double range[6];

#ifdef CUTLIB_TIMING
        Timer::Start(THREAD_TOTAL);
#endif
        grid->getSearchRange(i, j, k, center, range);
        cutSearch->search(center, range, pos6, bid6, tri6);

        for (int d = 0; d < 6; d++) pos6_f[d] = (float)(pos6[d]/range[d]);

        cutPos->setPos(i, j, k, pos6_f);
        cutBid->setBid(i, j, k, bid6);

        if (cutNormal) {
          for (int d = 0; d < 6; d++) {
            if (bid6[d] > 0) {
              CutPolygon* p = new CutPolygon(cutNormal->getIndex(i, j, k), d, tri6[d]);
              cutPolygonList[iThread].push_back(p);
            }
          }
        }

#ifdef CUTLIB_TIMING
        Timer::Stop(THREAD_TOTAL);
#endif
      }
    }
  }
  } // parallel reagion
#ifdef CUTLIB_TIMING
  Timer::Stop(MAIN_LOOP);
#endif

  if (cutNormal) {
#ifdef CUTLIB_TIMING
    Timer::Start(PACK_NORMAL);
#endif
    cutNormal->setNormalInfo(cutPolygonList, nThread);
#ifdef CUTLIB_TIMING
    Timer::Stop(PACK_NORMAL);
#endif
  }

  delete cutSearch;
//  delete pgList;
  delete[] cutPolygonList;

#ifdef CUTLIB_TIMING
  Timer::Stop(TOTAL);
  Timer::Print(TOTAL, "Total");
  Timer::Print(MAIN_LOOP, "Main Loop");
  if (cutNormal) {
    Timer::Print(PACK_NORMAL, "Pack Normal");
  }
  Timer::PrintFull(THREAD_TOTAL, "Theread Total");
  Timer::PrintFull(SEARCH_POLYGON, "Polylib::search_polygons");
#endif

  return CL_SUCCESS;
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
                              CutPosOctree* cutPos, CutBidOctree* cutBid)
{
  { 
    // check input parameters
    CutlibReturn ret;
    ret = checkPolylib("CalcCutInfoOctreeLeafCell", pl);
    if (ret != CL_SUCCESS) return ret;
    ret = checkTree("CalcCutInfoOctreeLeafCell", tree);
    if (ret != CL_SUCCESS) return ret;
  }

#ifdef CUTLIB_TIMING
  Timer::Start(TOTAL);
#endif

  std::vector<std::string>* pgList = createPolygonGroupPathList(pl);

  CutSearch* cutSearch = new CutSearch(pl, pgList);

#ifdef CUTLIB_TIMING
  Timer::Start(MAIN_LOOP);
#endif
  for (SklCell* cell = tree->GetLeafCellFirst(); cell != 0;
       cell = tree->GetLeafCellNext(cell)) {
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
  }
#ifdef CUTLIB_TIMING
  Timer::Stop(MAIN_LOOP);
#endif

  delete cutSearch;
  delete pgList;

#ifdef CUTLIB_TIMING
  Timer::Stop(TOTAL);
  Timer::Print(TOTAL, "Total");
  Timer::Print(MAIN_LOOP, "Main Loop");
  Timer::Print(SEARCH_POLYGON, "Polylib::search_polygons");
#endif

  return CL_SUCCESS;
}


/// 交点情報計算: Octree, 全セル計算.
///
///  @param[in,out] tree SklTreeクラスオブジェクト
///  @param[in] pl Polylibクラスオブジェクト
///  @param cutPos 交点座標データアクセッサ
///  @param cutBid 境界IDデータアクセッサ
///
CutlibReturn CalcCutInfoOctreeAllCell(SklTree* tree, const Polylib* pl,
                              CutPosOctree* cutPos, CutBidOctree* cutBid)
{
  { 
    // check input parameters
    CutlibReturn ret;
    ret = checkPolylib("CalcCutInfoOctreeAllCell", pl);
    if (ret != CL_SUCCESS) return ret;
    ret = checkTree("CalcCutInfoOctreeAllCell", tree);
    if (ret != CL_SUCCESS) return ret;
  }

#ifdef CUTLIB_TIMING
  Timer::Start(TOTAL);
#endif

  std::vector<std::string>* pgList = createPolygonGroupPathList(pl);

  size_t nx, ny, nz;
  tree->GetSize(nx, ny, nz);

#ifdef CUTLIB_TIMING
  Timer::Start(MAIN_LOOP);
#endif
  for (size_t k = 0; k < nz; k++) {
    for (size_t j = 0; j < ny; j++) {
      for (size_t i = 0; i < nx; i++) {
        SklCell* rootCell = tree->GetRootCell(i, j, k);
        float org[3], d[3];
        rootCell->GetOrigin(org[0], org[1], org[2]);
        rootCell->GetPitch(d[0], d[1], d[2]);
        Vec3r min = Vec3r(org[0]-0.5*d[0], org[1]-0.5*d[1], org[2]-0.5*d[2]);
        Vec3r max = Vec3r(org[0]+1.5*d[0], org[1]+1.5*d[1], org[2]+1.5*d[2]);

        cutOctree::CutTriangles ctList;
        cutOctree::CutTriangle::AppendCutTriangles(ctList, pl, pgList, min, max);

        cutOctree::calcCutInfo(rootCell, org, d, cutPos, cutBid, ctList);

        cutOctree::CutTriangle::DeleteCutTriangles(ctList);
      }
    }
  }
#ifdef CUTLIB_TIMING
  Timer::Stop(MAIN_LOOP);
#endif

  delete pgList;

#ifdef CUTLIB_TIMING
  Timer::Stop(TOTAL);
  Timer::Print(TOTAL, "Total");
  Timer::Print(MAIN_LOOP, "Main Loop");
  Timer::Print(SEARCH_POLYGON, "Polylib::search_polygons");
#endif

  return CL_SUCCESS;
}


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
                              CutPosOctree* cutPos, CutBidOctree* cutBid)
{
  {
    // check input parameters
    CutlibReturn ret;
    ret = checkPolylib("CalcCutInfoOctreeAllCell0", pl);
    if (ret != CL_SUCCESS) return ret;
    ret = checkTree("CalcCutInfoOctreeAllCell0", tree);
    if (ret != CL_SUCCESS) return ret;
  }

#ifdef CUTLIB_TIMING
  Timer::Start(TOTAL);
#endif

  std::vector<std::string>* pgList = createPolygonGroupPathList(pl);

  CutSearch* cutSearch = new CutSearch(pl, pgList);

  size_t nx, ny, nz;
  tree->GetSize(nx, ny, nz);

#ifdef CUTLIB_TIMING
  Timer::Start(MAIN_LOOP);
#endif
  for (size_t k = 0; k < nz; k++) {
    for (size_t j = 0; j < ny; j++) {
      for (size_t i = 0; i < nx; i++) {
        SklCell* cell = tree->GetRootCell(i, j, k);
        cutOctree::calcCutInfo0(cell, cutSearch, cutPos, cutBid);
      }
    }
  }
#ifdef CUTLIB_TIMING
  Timer::Stop(MAIN_LOOP);
#endif

  delete cutSearch;
  delete pgList;

#ifdef CUTLIB_TIMING
  Timer::Stop(TOTAL);
  Timer::Print(TOTAL, "Total");
  Timer::Print(MAIN_LOOP, "Main Loop");
  Timer::Print(SEARCH_POLYGON, "Polylib::search_polygons");
#endif

  return CL_SUCCESS;
}

#endif // CUTLIB_OCTREE

} // namespace cutlib
