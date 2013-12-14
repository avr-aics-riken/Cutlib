/// @file
/// @brief 時間測定用クラス 宣言
///

#ifndef CUTLIB_TIMING_H
#define CUTLIB_TIMING_H

#include <iostream>
#include <string>
#include <sys/time.h>
#ifdef _OPENMP
#include "omp.h"
#endif

namespace cutlib {

/// 測定区間名.
enum Section {
  TOTAL,
  SEARCH_POLYGON,
  MAIN_LOOP,
  THREAD_TOTAL,
  PACK_NORMAL,
  TEST1,
  TEST2,
  NumSections,
};


/// 時間測定ストップウオッチクラス.
class Timer {

  double* tStart;   ///< スタート時刻
  double* time;     ///< 経過時間
  int* count;       ///< 測定回数

  /// 時間測定ストップウオッチリスト.
  static Timer Timers[];

  /// コンストラクタ(private).
  Timer() {
#ifdef _OPENMP
    int nThread = omp_get_max_threads();
#else
    int nThread = 1;
#endif
    tStart = new double[nThread];
    time = new double[nThread];
    count = new int[nThread];

    for (int i = 0; i < nThread; i++) {
      tStart[i] = time[i] = 0.0;
      count[i] = 0;
    }
  }

  /// デストラクタ.
  ~Timer() {
    delete[] tStart;
    delete[] time;
    delete[] count;
  }

  /// 経過時間取得(gettimeofdayシステムコール).
  double getWTime() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (double)tv.tv_sec + (double)tv.tv_usec * 1.e-6;
  }

  /// 時間測定スタート.
  void start() { 
#ifdef _OPENMP
    int iThread = omp_get_thread_num();
#else
    int iThread = 0;
#endif
    tStart[iThread] = getWTime();
  }

  /// 時間測定ストップ.
  void stop() {
#ifdef _OPENMP
    int iThread = omp_get_thread_num();
#else
    int iThread = 0;
#endif
    time[iThread] += getWTime() - tStart[iThread];
    ++count[iThread];
  }

  /// 測定結果出力(マスタスレッドのみ).
  void print(const std::string& name) const {
    std::cout << name << ": "
              << "time = " << time[0] << ", "
              << "count = " << count[0] << std::endl;
  }

  /// 測定結果出力(スレッド単位).
  void print(const std::string& name, int iThread) const {
    std::cout << name << "[" << iThread << "]: "
              << "time = " << time[iThread] << ", "
              << "count = " << count[iThread] << std::endl;
  }

  /// 測定結果出力(全スレッド).
  void printFull(const std::string& name) const {
#ifdef _OPENMP
    int nThread = omp_get_max_threads();
#else
    int nThread = 1;
#endif
    for (int i = 0; i < nThread; i++) print(name, i);
  }

public:

  /// ストップウオッチsecをスタート.
  ///
  ///  @param[in] sec ストップウオッチキーワード
  ///
  static void Start(Section sec) { Timers[sec].start(); }
 
  /// ストップウオッチsecをストップ.
  ///
  ///  @param[in] sec ストップウオッチキーワード
  ///
  static void Stop(Section sec) { Timers[sec].stop(); }

  /// ストップウオッチsecの計測結果を表示(マスタスレッドのみ).
  ///
  ///  @param[in] sec ストップウオッチキーワード
  ///  @param[in] name ストップウオッチ名
  ///
  static void Print(Section sec, const std::string& name) { Timers[sec].print(name); }

  /// ストップウオッチsecの計測結果を表示(全スレッド).
  ///
  ///  @param[in] sec ストップウオッチキーワード
  ///  @param[in] name ストップウオッチ名
  ///
  static void PrintFull(Section sec, const std::string& name) { Timers[sec].printFull(name); }

};

} // namespace cutlib

#endif // CUTLIB_TIMING_H
