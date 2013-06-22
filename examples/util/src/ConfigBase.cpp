#include "ConfigBase.h"
#include <iostream>
#include <sstream>
#ifndef USE_MPI
#include <cstdlib>
#endif

#ifdef USE_MPI
/// コンストラクタ.
ConfigBase::ConfigBase(MPI::Comm& comm) : comm(comm)
{
}
#endif


/// デストラクタ.
ConfigBase::~ConfigBase()
{
  delete configFile;
}


/// 設定ファイル読み込み.
void ConfigBase::load(const char* file)
{
#ifdef USE_MPI
  if (comm.Get_rank() == 0) {
#endif
    try {
      configFile = new ConfigFile(file);
      parse();
    }
    catch(ConfigFile::file_not_found& e) {
      std::cout << "error: cannot open configfile: " << e.filename << std::endl;
      errorExit("input config file.");
    }
    catch(ConfigFile::key_not_found& e) {
      std::cout << "error: cannot find key: " << e.key << std::endl;
      errorExit("input config file.");
    }
    if (!validate()) errorExit("input config file.");
#ifdef USE_MPI
    broadcastConfigFile(configFile);
  }
  else {
    configFile = new ConfigFile;
    receiveConfigFile(configFile);
    parse();
  }
#endif
}


#ifdef USE_MPI
/// ConfigFileオブジェクトの内容をrank0からブロードキャスト.
void ConfigBase::broadcastConfigFile(const ConfigFile* configFile)
{
  std::ostringstream outStr;
  outStr << *configFile;

  int size = outStr.str().size() + 1;
  comm.Bcast(&size, 1, MPI::INT, 0);

  comm.Bcast((void*)outStr.str().c_str(), size, MPI::CHAR, 0);
  // constを消すためにキャストが必要
}


/// ConfigFileオブジェクトの内容をrank0から受信.
void ConfigBase::receiveConfigFile(ConfigFile* configFile)
{
  int size;
  comm.Bcast(&size, 1, MPI::INT, 0);

  char* buffer = new char[size];
  comm.Bcast(buffer, size, MPI::CHAR, 0);
  
  std::istringstream inStr(buffer);
  inStr >> *configFile;

  delete[] buffer;
}
#endif


/// エラー終了.
void ConfigBase::errorExit(const char* message, int code) 
{
  std::cout << "error: " << message << std::endl;
#ifdef USE_MPI
  comm.Abort(code);
#else
  exit(code);
#endif
}
