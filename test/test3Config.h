#include "Config.h"

class Test3Config : public Config {
public:
  size_t ndim[3];
  float org[3], d[3];
  char xmlFile[MAX_LEN];

  Test3Config(const char* file) : Config(file) 
  {
    if (xscanf("%zd %zd %zd", &ndim[0], &ndim[1], &ndim[2]) != 3) readError();
    if (xscanf("%g %g %g", &org[0], &org[1], &org[2]) != 3) readError();
    if (xscanf("%g %g %g", &d[0], &d[1], &d[2]) != 3) readError();
    if (xscanf("%s", xmlFile) != 1) readError();
    closeFile();
  }
};

