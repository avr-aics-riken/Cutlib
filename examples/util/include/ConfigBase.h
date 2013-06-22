///
/// @file ConfigBase.h
/// @brief 設定パラメータクラス(基底クラス)
/// 

#ifndef CONFIG_BASE_H
#define CONFIG_BASE_H

#include "ConfigFile.h"

#ifdef USE_MPI
#include "mpi.h"
#endif

/// 設定パラメータクラス(基底クラス).
///
/// このクラスを次ぎのように継承してカスタマイズすること
///  @li 設定パラメータをpublicメンバに追加
///  @li parseメソッド内で、readメソッドを用いて各パラメータ値を入力
///  @li 必要なら、validateメソッド内でパラメータ値の検証
///
class ConfigBase {

#ifdef USE_MPI
  MPI::Comm& comm;   ///< MPIコミュニケータ
#endif

  ConfigFile* configFile;  ///< ConfigFileオブジェクト

public:

#ifdef USE_MPI
  /// コンストラクタ.
  ///
  ///  @param[in] comm MPIコミュニケータ
  ///
  ConfigBase(MPI::Comm& comm = MPI::COMM_WORLD);
#else
  /// コンストラクタ.
  ConfigBase() {}
#endif

  /// デストラクタ.
  ~ConfigBase();

  /// 設定ファイル読み込み.
  ///
  ///  @param[in] file 設定ファイルパス
  ///
  ///  @note rank0のみがファイルから読み込み，他rankに転送する.
  ///
  void load(const char* file);


private:

  /// パラメータのパース(派生クラスでカスタマイズ).
  virtual void parse() = 0;

  /// パラメータ値のチェック(派生クラスでカスタマイズ).
  virtual bool validate() { return true; }

#ifdef USE_MPI
  /// ConfigFileオブジェクトの内容をrank0からブロードキャスト.
  void broadcastConfigFile(const ConfigFile* configFile);

  /// ConfigFileオブジェクトの内容をrank0から受信.
  void receiveConfigFile(ConfigFile* configFile);
#endif

protected:

  /// keyに対応したパラメータの読み込み.
  template<class T> T read(const std::string& key) const {
    return configFile->read<T>(key);
  }

  /// keyに対応したパラメータの読み込み(ディフォルト値あり).
  template<class T> T read(const std::string& key, const T& value) const {
    return configFile->read<T>(key, value);
  }

  /// エラー終了.
  void errorExit(const char* message, int code = 1); 

};


#endif // CONFIG_BASE_H
