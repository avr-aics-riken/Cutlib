#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>
using std::vector;
using std::string;

class Config {
  const char* file_;
  int line_;

protected:
  FILE* fp_;
  int xscanf(char* fmt, ...);
  void readError();
  bool isBlankLine(char* p);
  bool isCommentLine(char* p);
  char* readLine(char* buf, int size);
  void closeFile() { fclose(fp_); }

  static const int MAX_LEN = 1024;

public:
  Config(const char* file); 
};

