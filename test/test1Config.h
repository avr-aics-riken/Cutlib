#include "Config.h"

class Test1Config : public Config {
public:
  int calcType;
  int cutPosType, cutBidType;
  size_t ndim[3], ista[3], nlen[3];
  float org[3], d[3];
  char xmlFile[MAX_LEN];

  Test1Config(const char* file) : Config(file) 
  {
    if (xscanf("%d", &calcType) != 1) readError();
    if (xscanf("%d", &cutPosType) != 1) readError();
    if (xscanf("%d", &cutBidType) != 1) readError();
    if (xscanf("%zd %zd %zd", &ndim[0], &ndim[1], &ndim[2]) != 3) readError();
    if (xscanf("%zd %zd %zd", &ista[0], &ista[1], &ista[2]) != 3) readError();
    if (xscanf("%zd %zd %zd", &nlen[0], &nlen[1], &nlen[2]) != 3) readError();
    if (xscanf("%g %g %g", &org[0], &org[1], &org[2]) != 3) readError();
    if (xscanf("%g %g %g", &d[0], &d[1], &d[2]) != 3) readError();
    if (xscanf("%s", xmlFile) != 1) readError();
    closeFile();
  }
};

