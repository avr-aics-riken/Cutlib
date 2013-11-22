/// @file
/// @brief 交点ポリゴン法線格納クラス
///

#ifndef CUT_NORMAL_ARRAY_H
#define CUT_NORMAL_ARRAY_H

#include <cassert>
#include "Cutlib.h"
//#include "CutPolygon.h"

namespace cutlib {

/// @defgroup CutInfoArray 交点情報一次元配列ラッパクラス
//@{

/// 法線ベクトルデータ格納位置型.
typedef int NormalIndex[6];

/// 法線ベクトル型.
#ifdef CUTINFO_NORMAL_DOUBLE
  typedef double Normal[3];
#else
  typedef float Normal[3];
#endif


/// 交点ポリゴンクラス.
class CutPolygon {

public:

  size_t ijk;  ///< 計算基準点インデクス
  unsigned char d;  ///< 計算基準線分番号(0〜5)
  Triangle* t;      ///< ポリゴンデータへのポインタ
  int id;       ///< ext_id

  /// コンストラクタ.
  ///
  ///  @param[in] ijk  計算基準点インデクス
  ///  @param[in] d  計算基準線分番号(0〜5)  
  ///  @param[in] t  ポリゴンデータへのポインタ
  ///
  CutPolygon(size_t ijk, int d, Triangle* t) : ijk(ijk), d(d), t(t) {
    id = t->get_exid();
  }

  /// デストラクタ.
  ~CutPolygon() {}

};

/// 交点ポリゴンのリスト.
typedef std::vector<const CutPolygon*> CutPolygonList;


/// 交点ポリゴン法線格納クラス.
class CutNormalArray : public CutInfoArray {

  size_t n;       ///< 一次元データサイズ
  int nNormal;    ///< ユニークな法線ベクトルデータの数

  NormalIndex* normalIndexData;  ///< 法線ベクトルデータ格納位置配列

  Normal* normalData;  ///< 法線ベクトルデータ配列


public:
  /// コンストラクタ.
  ///
  ///  @param[in] nx,ny,nz  配列サイズ(3次元で指定)
  ///
  CutNormalArray(size_t nx, size_t ny, size_t nz) : CutInfoArray(nx, ny, nz)
  {
    n = nx * ny * nz;
    initNormalIndex();
  }

  /// コンストラクタ.
  ///
  ///  @param[in] sx,sy,sz 領域開始位置3次元インデクス
  ///  @param[in] ex,ey,ez 領域終了位置3次元インデクス
  ///
  ///
  CutNormalArray(int sx, int sy, int sz, int ex, int ey, int ez)
    : CutInfoArray(sx, sy, sz, ex, ey, ez)
  {
    n = (ex-sx+1) * (ey-sy+1) * (ez-sz+1);
    initNormalIndex();
  }

  /// コンストラクタ.
  ///
  ///  @param[in] ndim  配列サイズ(3次元で指定)
  ///
  CutNormalArray(const size_t ndim[]) : CutInfoArray(ndim[0], ndim[1], ndim[2])
  {
    n = ndim[0] * ndim[1] * ndim[2];
    initNormalIndex();
  }

  /// コンストラクタ.
  ///
  ///  @param[in] start 領域開始位置3次元インデクス
  ///  @param[in] end   領域終了位置3次元インデクス
  ///
  CutNormalArray(int start[], int end[])
    : CutInfoArray(start[0], start[1], start[2], end[0], end[1], end[2])
  {
    n = (end[0]-start[0]+1) * (end[1]-start[1]+1) * (end[2]-start[2]+1);
    initNormalIndex();
  }


  /// ディストラクタ.
  ~CutNormalArray() {
    delete[] normalIndexData;
    delete[] normalData;
  }


  /// 一要素のバイトサイズを得る.
  size_t getElementSize() const { return sizeof(NormalIndex); }


  /// 交点ポリゴンリストから法線ベクトルデータを抽出.
  ///
  ///  @param[in,out] cutPolygonList[] 交点ポリゴンリスト
  ///  @param[in] nThread スレッド数(交点ポリゴンリスト数)
  ///
  ///  @note Triangleクラスのext_idを作業用変数として使用
  ///  @note 終了時に交点ポリゴンリストの各要素のメモリを解放する
  ///
  void setNormalInfo(CutPolygonList* cutPolygonList, int nThread) {
    std::vector<Triangle*> tList;
    CutPolygonList::iterator it;

    // 作業用に使用するext_idの初期化(元の値はセーブ済み)
    for (int i = 0; i < nThread; i++) {
      for (it = cutPolygonList[i].begin(); it != cutPolygonList[i].end(); ++it) {
        (*it)->t->set_exid(-1);
      }
    }
    for (int i = 0; i < nThread; i++) {
      for (it = cutPolygonList[i].begin(); it != cutPolygonList[i].end(); ++it) {
        Triangle* t = (*it)->t;
        int id = t->get_exid();
        if (id < 0) {
          tList.push_back(t);
          t->set_exid(nNormal);
          nNormal++;
        }
      }
    }
#ifdef CUTLIB_DEBUG
    int listSize = 0;
    for (int i = 0; i < nThread; i++) listSize += cutPolygonList[i].size();
    std::cout << "CutNormalArray: normal data compress: " << listSize
              << " -> " << nNormal << std::endl;
#endif
    normalData = new Normal[nNormal];
    for (int i = 0; i < nNormal; i++) {
      Vec3f n = tList[i]->get_normal();
      normalData[i][0] = n[0];
      normalData[i][1] = n[1];
      normalData[i][2] = n[2];
    }
    for (int i = 0; i < nThread; i++) {
      for (it = cutPolygonList[i].begin(); it != cutPolygonList[i].end(); ++it) {
        int ijk = (*it)->ijk;
        assert(0 <= ijk && ijk < n);
        int d = (*it)->d;
        assert(0 <= d && d < 6);
        int id = (*it)->t->get_exid();
        assert(0 <= id && id < nNormal);
    //  std::cout << ijk << ":" << d << " -> " << id << " (" 
    //            << normalData[id][0] << ", "
    //            << normalData[id][1] << ", "
    //            << normalData[id][2] << ")" << std::endl;
        normalIndexData[ijk][d] = id;
      }
    }

    // ext_idの値を戻す
    // CutPolygonオブジェクトのメモリ解放
    for (int i = 0; i < nThread; i++) {
      for (it = cutPolygonList[i].begin(); it != cutPolygonList[i].end(); ++it) {
        (*it)->t->set_exid((*it)->id);
        delete *it;
      }
    }
  }

  ///  ユニークな法線ベクトルデータの総数を取得.
  int getNumNormal() const { return nNormal; }

  /// 法線ベクトルデータ配列へのポインタを取得.
  Normal* getNormalDataPointer() const { return normalData; }

  /// 法線ベクトルデータ格納位置配列.
  NormalIndex* getNormalIndexDataPointer() const { return normalIndexData; }

  /// 法線ベクトルデータの取得.
  void getNormal(size_t ijk, int d, Normal& normal) const {
    int id = normalIndexData[ijk][d];
    if (id < 0) {
      normal[0] = normal[1] = normal[2];
    } else {
      normal[0] = normalData[id][0];
      normal[1] = normalData[id][1];
      normal[2] = normalData[id][2];
    }
  }

  /// 法線ベクトルデータの取得.
  void getNormal(int i, int j, int k, int d, Normal& normal) const {
    getNormal(getIndex(i, j, k), d, normal);
  }

  /// 法線ベクトルデータの取得.
  void getNormal(size_t ijk, Normal normal[]) const {
    for (int d = 0; d < 6; d++) getNormal(ijk, d, normal[d]);
  }

  /// 法線ベクトルデータの取得.
  void getNormal(int i, int j, int k, Normal normal[]) const {
    for (int d = 0; d < 6; d++) getNormal(i, j, k, d, normal[d]);
  }

private:

  /// 法線ベクトルデータ格納位置配列の初期化.
  void initNormalIndex() {
    normalIndexData = new NormalIndex[n];
    for (int ijk = 0; ijk < n; ijk++) {
      for (int d = 0; d < 6; d++) {
        normalIndexData[ijk][d] = 0;
      }
    }
    nNormal = 0;
  }

};

//@} end gropu CutInfoArray

}

#endif // CUT_NORMAL_ARRAY_H
