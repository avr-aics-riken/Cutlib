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
/// @brief 交点情報配列ラッパクラス
///

#ifndef CUTINFO_ARRAY_H
#define CUTINFO_ARRAY_H

#include "CutInfo.h"

namespace cutlib {

/// @defgroup CutInfoArray 交点情報一次元配列ラッパクラス
//@{

/// 一次元配列ラッパクラスの基底クラス
class CutInfoArray {

  int sx;   ///< x方向対象領域開始インデクス
  int sy;   ///< y方向対象領域開始インデクス
  int sz;   ///< z方向対象領域開始インデクス

  size_t nx;  ///< x方向対象領域サイズ
  size_t ny;  ///< y方向対象領域サイズ
  size_t nz;  ///< z方向対象領域サイズ

public:
  /// コンストラクタ.
  ///
  ///   @param[in] nx,ny,nz 配列サイズ(3次元で指定)
  ///
  CutInfoArray(size_t nx, size_t ny, size_t nz)
    : sx(0), sy(0), sz(0), nx(nx), ny(ny), nz(nz) {}

  /// コンストラクタ.
  ///
  ///   @param[in] sx,sy,sz 領域開始位置3次元インデクス
  ///   @param[in] ex,ey,ez 領域終了位置3次元インデクス
  ///
  CutInfoArray(int sx, int sy, int sz, int ex, int ey, int ez)
    : sx(sx), sy(sy), sz(sz), nx(ex-sx+1), ny(ey-sy+1), nz(ez-sz+1) {}

  /// デストラクタ.
  virtual ~CutInfoArray() {}

  /// 一要素のバイトサイズを得る.
  virtual size_t getElementSize() const = 0;

  /// x方向のサイズを得る.
  size_t getSizeX() const { return nx; }

  /// y方向のサイズを得る.
  size_t getSizeY() const { return ny; }

  /// z方向のサイズを得る.
  size_t getSizeZ() const { return nz; }

  /// x方向の開始位置を得る.
  int getStartX() const { return sx; }

  /// y方向の開始位置を得る.
  int getStartY() const { return sy; }

  /// z方向の開始位置を得る.
  int getStartZ() const { return sz; }

  /// 3次元インデックス(i,j,k)より1次元インデックスを計算
  ///
  ///  @param[in] i,j,k  3次元インデックス
  ///
  size_t getIndex(int i, int j, int k) const {
    return (i-sx) + (j-sy) * nx + (k-sz) * nx * ny;
  }
};


/// 交点座標一次元配列ラッパ仮想クラス.
class CutPosArray : public CutInfoArray {
public:
  /// コンストラクタ.
  ///
  ///  @param[in] nx,ny,nz 配列サイズ(3次元で指定)
  ///
  CutPosArray(size_t nx, size_t ny, size_t nz) : CutInfoArray(nx, ny, nz) {}

  /// コンストラクタ.
  ///
  ///   @param[in] sx,sy,sz 領域開始位置3次元インデクス
  ///   @param[in] ex,ey,ez 領域終了位置3次元インデクス
  ///
  CutPosArray(int sx, int sy, int sz, int ex, int ey, int ez)
    : CutInfoArray(sx, sy, sz, ex, ey, ez) {}

  /// デストラクタ.
  virtual ~CutPosArray() {}

  /// 交点座標値を設定(d方向).
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[in] d 交点探査方向(0〜5)
  ///  @param[in] pos 交点座標値
  ///
  virtual void setPos(int i, int j, int k, int d, float pos) = 0;

  /// 交点座標値を設定(6方向まとめて).
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[in] pos 交点座標配列
  ///
  virtual void setPos(int i, int j, int k, const float pos[]) = 0;

  /// 交点座標値(d方向)を得る.
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[in] d 交点探査方向(0〜5)
  ///  @return 交点座標値
  ///
  virtual float getPos(int i, int j, int k, int d) const = 0;

  /// 交点座標値(d方向)を得る(1次元インデックスで指定).
  ///
  ///  @param[in] ijk 1次元インデックス
  ///  @param[in] d 交点探査方向(0〜5)
  ///  @return 交点座標値
  ///
  virtual float getPos(size_t ijk, int d) const = 0;

  /// 交点座標値(6方向まとめて)を得る.
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[out] pos 交点座標配列
  ///
  virtual void getPos(int i, int j, int k, float pos[]) const = 0;

  /// 交点座標値(6方向まとめて)を得る(1次元インデックスで指定).
  ///
  ///  @param[in] ijk 1次元インデックス
  ///  @param[out] pos 交点座標配列
  ///
  virtual void getPos(size_t ijk, float pos[]) const = 0;

  /// 全配列データを1.0でクリア
  virtual void clear() = 0;
};


/// 境界ID一次元配列ラッパ仮想クラス
class CutBidArray : public CutInfoArray {
public:
  /// コンストラクタ.
  ///
  ///  @param[in] nx,ny,nz  配列サイズ(3次元で指定)
  ///
  CutBidArray(size_t nx, size_t ny, size_t nz) : CutInfoArray(nx, ny, nz) {}

  /// コンストラクタ.
  ///
  ///   @param[in] sx,sy,sz 領域開始位置3次元インデクス
  ///   @param[in] ex,ey,ez 領域終了位置3次元インデクス
  ///
  CutBidArray(int sx, int sy, int sz, int ex, int ey, int ez)
    : CutInfoArray(sx, sy, sz, ex, ey, ez) {}

  /// デストラクタ.
  virtual ~CutBidArray() {}

  /// 境界IDを設定(d方向).
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[in] d 交点探査方向(0〜5)
  ///  @param[in] bid 境界ID
  ///
  virtual void setBid(int i, int j, int k, int d, BidType bid) = 0;

  /// 境界IDを設定(6方向まとめて).
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[in] bid 境界ID配列
  ///
  virtual void setBid(int i, int j, int k, const BidType bid[]) = 0;

  /// 境界ID(d方向)を得る.
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[in] d 交点探査方向(0〜5)
  ///  @return 境界ID
  ///
  virtual BidType getBid(int i, int j, int k, int d) const = 0;

  /// 境界ID(d方向)を得る(1次元インデックスで指定).
  ///
  ///  @param[in] ijk 1次元インデックス
  ///  @param[in] d 交点探査方向(0〜5)
  ///  @return 境界ID
  ///
  virtual BidType getBid(size_t ijk, int d) const = 0;

  /// 境界ID(6方向まとめて)を得る.
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[out] bid  境界ID配列
  ///
  virtual void getBid(int i, int j, int k, BidType bid[]) const = 0;

  /// 境界ID(6方向まとめて)を得る(1次元インデックスで指定).
  ///
  ///  @param[in] ijk 1次元インデックス
  ///  @param[out] bid  境界ID配列
  ///
  virtual void getBid(size_t ijk, BidType bid[]) const = 0;

  /// 全配列データを0クリア.
  virtual void clear() = 0;
};

//-----------------------------------------------------------------------------

/// 交点座標一次元配列ラッパクラステンプレート.
template<typename CUT_POS>
class CutPosArrayTemplate : public CutPosArray {
  size_t n;        ///< 一次元データサイズ
  CUT_POS* data;   ///< 一次元データポインタ
  bool allocated;  ///< 一次元データ管理フラグ

public:
  /// コンストラクタ(自前で一次元データ領域を確保).
  ///
  ///  @param[in] nx,ny,nz  配列サイズ(3次元で指定)
  ///
  ///  @note デストラクタで一次元データ領域を開放(allocated_=true)
  ///
  CutPosArrayTemplate(size_t nx, size_t ny, size_t nz) : CutPosArray(nx, ny, nz)
  {
    n = nx * ny * nz;
    data = new CUT_POS[n];
    for (size_t ijk = 0; ijk < n; ijk++) ClearCutPos(data[ijk]);
    allocated = true;
  }

  /// コンストラクタ(自前で一次元データ領域を確保).
  ///
  ///  @param[in] sx,sy,sz 領域開始位置3次元インデクス
  ///  @param[in] ex,ey,ez 領域終了位置3次元インデクス
  ///
  ///  @note デストラクタで一次元データ領域を開放(allocated_=true)
  ///
  CutPosArrayTemplate(int sx, int sy, int sz, int ex, int ey, int ez)
    : CutPosArray(sx, sy, sz, ex, ey, ez)
  {
    n = (ex-sx+1) * (ey-sy+1) * (ez-sz+1);
    data = new CUT_POS[n];
    for (size_t ijk = 0; ijk < n; ijk++) ClearCutPos(data[ijk]);
    allocated = true;
  }

  /// コンストラクタ(自前で一次元データ領域を確保).
  ///
  ///  @param[in] ndim  配列サイズ(3次元で指定)
  ///
  ///  @note デストラクタで一次元データ領域を開放(allocated_=true)
  ///
  CutPosArrayTemplate(const size_t ndim[]) : CutPosArray(ndim[0], ndim[1], ndim[2])
  {
    n = ndim[0] * ndim[1] * ndim[2];
    data = new CUT_POS[n];
    for (size_t ijk = 0; ijk < n; ijk++) ClearCutPos(data[ijk]);
    allocated = true;
  }

  /// コンストラクタ(自前で一次元データ領域を確保).
  ///
  ///  @param[in] start 領域開始位置3次元インデクス
  ///  @param[in] end   領域終了位置3次元インデクス
  ///
  ///  @note デストラクタで一次元データ領域を開放(allocated_=true)
  ///
  CutPosArrayTemplate(int start[], int end[])
    : CutPosArray(start[0], start[1], start[2], end[0], end[1], end[2])
  {
    n = (end[0]-start[0]+1) * (end[1]-start[1]+1) * (end[2]-start[2]+1);
    data = new CUT_POS[n];
    for (size_t ijk = 0; ijk < n; ijk++) ClearCutPos(data[ijk]);
    allocated = true;
  }

  /// コンストラクタ(一次元データ領域をインポート).
  ///
  ///  @param[in] data 交点座標基本型配列
  ///  @param[in] nx,ny,nz  配列サイズ(3次元で指定)
  ///
  ///  @note デストラクタで一次元データ領域を開放しない(allocated_=false)
  ///
  CutPosArrayTemplate(CUT_POS* data, size_t nx, size_t ny, size_t nz)
    : CutPosArray(nx, ny, nz), data(data)
  {
    n = nx * ny * nz;
    allocated = false;
  }

  /// コンストラクタ(一次元データ領域をインポート).
  ///
  ///  @param[in] data 交点座標基本型配列
  ///  @param[in] sx,sy,sz 領域開始位置3次元インデクス
  ///  @param[in] ex,ey,ez 領域終了位置3次元インデクス
  ///
  ///  @note デストラクタで一次元データ領域を開放しない(allocated_=false)
  ///
  CutPosArrayTemplate(CUT_POS* data, int sx, int sy, int sz, int ex, int ey, int ez)
    : CutPosArray(sx, sy, sz, ex, ey, ez), data(data)
  {
    n = (ex-sx+1) * (ey-sy+1) * (ez-sz+1);
    allocated = false;
  }

  /// ディストラクタ.
  ///
  ///  @note allocated_=trueの場合のみ一次元データ領域を開放
  ///
  ~CutPosArrayTemplate() { if (allocated) delete[] data; }

  /// 一要素のバイトサイズを得る.
  size_t getElementSize() const { return sizeof(CUT_POS); }

  /// 一要素のバイトサイズを得る(クラスメソッド).
  static size_t GetElementSize() { return sizeof(CUT_POS); }

  /// 交点座標値を設定(d方向).
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[in] d 交点探査方向(0〜5)
  ///  @param[in] pos 交点座標値
  ///
  void setPos(int i, int j, int k, int d, float pos)
  {
    SetCutPos(data[getIndex(i,j,k)], d, pos);
  }

  /// 交点座標値を設定(6方向まとめて).
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[in] pos 交点座標配列
  ///
  void setPos(int i, int j, int k, const float pos[])
  {
    SetCutPos(data[getIndex(i,j,k)], pos);
  }

  /// 交点座標値(d方向)を得る.
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[in] d 交点探査方向(0〜5)
  ///  @return 交点座標値
  ///
  float getPos(int i, int j, int k, int d) const
  {
    return GetCutPos(data[getIndex(i,j,k)], d);
  }

  /// 交点座標値(d方向)を得る(1次元インデックスで指定).
  ///
  ///  @param[in] ijk 1次元インデックス
  ///  @param[in] d 交点探査方向(0〜5)
  ///  @return 交点座標値
  ///
  float getPos(size_t ijk, int d) const
  {
    return GetCutPos(data[ijk], d);
  }

  /// 交点座標値(6方向まとめて)を得る.
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[out] pos 交点座標配列
  ///
  void getPos(int i, int j, int k, float pos[]) const
  {
    GetCutPos(data[getIndex(i,j,k)], pos);
  }

  /// 交点座標値(6方向まとめて)を得る(1次元インデックスで指定).
  ///
  ///  @param[in] ijk 1次元インデックス
  ///  @param[out] pos 交点座標配列
  ///
  void getPos(size_t ijk, float pos[]) const
  {
    GetCutPos(data[ijk], pos);
  }

  /// 一次元配列データへのポインタを得る.
  CUT_POS* getDataPointer() const { return data; }


  /// 一次元配列データのサイズを得る.
  size_t getDataSize() const { return n; }

  /// 全配列データを1.0でクリア
  void clear()
  {
    for (size_t i = 0; i < n; i++) ClearCutPos(data[i]);
  }

};

//-----------------------------------------------------------------------------

/// 境界ID一次元配列ラッパクラステンプレート.
template<typename CUT_BID>
class CutBidArrayTemplate : public CutBidArray {
  size_t n;        ///< 一次元データサイズ
  CUT_BID* data;   ///< 一次元データポインタ
  bool allocated;  ///< 一次元データ管理フラグ

public:
  /// コンストラクタ(自前で一次元データ領域を確保).
  ///
  ///  @param[in] nx,ny,nz  配列サイズ(3次元で指定)
  ///
  ///  @note デストラクタで一次元データ領域を開放(allocated_=true)
  ///
  CutBidArrayTemplate(size_t nx, size_t ny, size_t nz) : CutBidArray(nx, ny, nz)
  {
    n = nx * ny * nz;
    data = new CUT_BID[n];
    for (size_t ijk = 0; ijk < n; ijk++) ClearCutBid(data[ijk]);
    allocated = true;
  }

  /// コンストラクタ(自前で一次元データ領域を確保).
  ///
  ///  @param[in] sx,sy,sz 領域開始位置3次元インデクス
  ///  @param[in] ex,ey,ez 領域終了位置3次元インデクス
  ///
  ///  @note デストラクタで一次元データ領域を開放(allocated_=true)
  ///
  CutBidArrayTemplate(int sx, int sy, int sz, int ex, int ey, int ez)
    : CutBidArray(sx, sy, sz, ex, ey, ez)
  {
    n = (ex-sx+1) * (ey-sy+1) * (ez-sz+1);
    data = new CUT_BID[n];
    for (size_t ijk = 0; ijk < n; ijk++) ClearCutBid(data[ijk]);
    allocated = true;
  }

  /// コンストラクタ(自前で一次元データ領域を確保).
  ///
  ///@param[in] ndim  配列サイズ(3次元で指定)
  ///
  ///  @note デストラクタで一次元データ領域を開放(allocated_=true)
  ///
  CutBidArrayTemplate(const size_t ndim[]) : CutBidArray(ndim[0], ndim[1], ndim[2])
  {
    n = ndim[0] * ndim[1] * ndim[2];
    data = new CUT_BID[n];
    for (size_t ijk = 0; ijk < n; ijk++) ClearCutBid(data[ijk]);
    allocated = true;
  }

  /// コンストラクタ(自前で一次元データ領域を確保).
  ///
  ///  @param[in] start 領域開始位置3次元インデクス
  ///  @param[in] end   領域終了位置3次元インデクス
  ///
  ///  @note デストラクタで一次元データ領域を開放(allocated_=true)
  ///
  CutBidArrayTemplate(int start[], int end[])
    : CutBidArray(start[0], start[1], start[2], end[0], end[1], end[2])
  {
    n = (end[0]-start[0]+1) * (end[1]-start[1]+1) * (end[2]-start[2]+1);
    data = new CUT_BID[n];
    for (size_t ijk = 0; ijk < n; ijk++) ClearCutBid(data[ijk]);
    allocated = true;
  }

  /// コンストラクタ(一次元データ領域をインポート).
  ///
  ///  @param[in] data 境界ID基本型配列
  ///  @param[in] nx,ny,nz  配列サイズ(3次元で指定)
  ///
  ///  @デストラクタで一次元データ領域を開放しない(allocated_=false)
  ///
  CutBidArrayTemplate(CUT_BID* data, size_t nx, size_t ny, size_t nz)
    : CutBidArray(nx, ny, nz), data(data)
  {
    n = nx * ny * nz;
    allocated = false;
  }

  /// コンストラクタ(一次元データ領域をインポート).
  ///
  ///  @param[in] data 交点座標基本型配列
  ///  @param[in] sx,sy,sz 領域開始位置3次元インデクス
  ///  @param[in] ex,ey,ez 領域終了位置3次元インデクス
  ///
  ///  @note デストラクタで一次元データ領域を開放しない(allocated_=false)
  ///
  CutBidArrayTemplate(CUT_BID* data, int sx, int sy, int sz, int ex, int ey, int ez)
    : CutBidArray(sx, sy, sz, ex, ey, ez), data(data)
  {
    n = (ex-sx+1) * (ey-sy+1) * (ez-sz+1);
    allocated = false;
  }

  /// デストラクタ.
  ///
  ///  @note allocated_=trueの場合のみ一次元データ領域を開放
  ///
  ~CutBidArrayTemplate() { if (allocated) delete[] data; }

  /// 一要素のバイトサイズを得る.
  size_t getElementSize() const { return sizeof(CUT_BID); }

  /// 一要素のバイトサイズを得る(クラスメソッド).
  static size_t GetElementSize() { return sizeof(CUT_BID); }

  /// 境界IDを設定(d方向).
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[in] d 交点探査方向(0〜5)
  ///  @param[in] bid 境界ID
  ///
  void setBid(int i, int j, int k, int d, BidType bid)
  {
    SetCutBid(data[getIndex(i,j,k)], d, bid);
  }

  /// 境界IDを設定(6方向まとめて).
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[in] bid 境界ID配列
  ///
  void setBid(int i, int j, int k, const BidType bid[])
  {
    SetCutBid(data[getIndex(i,j,k)], bid);
  }

  /// 境界ID(d方向)を得る.
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[in] d 交点探査方向(0〜5)
  ///  @return 境界ID
  ///
  BidType getBid(int i, int j, int k, int d) const
  {
    return GetCutBid(data[getIndex(i,j,k)], d);
  }

  /// 境界ID(d方向)を得る(1次元インデックスで指定).
  ///
  ///  @param[in] ijk 1次元インデックス
  ///  @param[in] d 交点探査方向(0〜5)
  ///  @return 境界ID
  ///
  BidType getBid(size_t ijk, int d) const
  {
    return GetCutBid(data[ijk], d);
  }

  /// 境界ID(6方向まとめて)を得る.
  ///
  ///  @param[in] i,j,k 3次元インデックス
  ///  @param[out] bid  境界ID配列
  ///
  void getBid(int i, int j, int k, BidType bid[]) const
  {
    GetCutBid(data[getIndex(i,j,k)], bid);
  }

  /// 境界ID(6方向まとめて)を得る(1次元インデックスで指定).
  ///
  ///  @param[in] ijk 1次元インデックス
  ///  @param[out] bid  境界ID配列
  ///
  void getBid(size_t ijk, BidType bid[]) const
  {
    GetCutBid(data[ijk], bid);
  }

  /// 一次元配列データへのポインタを得る.
  CUT_BID* getDataPointer() const { return data; }

  /// 一次元配列データのサイズを得る.
  size_t getDataSize() const { return n; }

  /// 全配列データを0クリア.
  void clear()
  {
    for (size_t i = 0; i < n; i++) ClearCutBid(data[i]);
  }
};

//-----------------------------------------------------------------------------

/// CutPos32型交点座標配列ラッパクラス.
typedef CutPosArrayTemplate<CutPos32> CutPos32Array;

/// CutPos8型交点座標配列ラッパクラス.
typedef CutPosArrayTemplate<CutPos8> CutPos8Array;

/// CutBid8型境界ID配列ラッパクラス.
typedef CutBidArrayTemplate<CutBid8> CutBid8Array;

/// CutBid5型境界ID配列ラッパクラス.
typedef CutBidArrayTemplate<CutBid5> CutBid5Array;

//@} end gropu CutInfoArray

} // namespace cutlib

#endif // CUTINFO_ARRAY_H
