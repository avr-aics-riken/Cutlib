#include "Config.h"
#include <cstdio>
#include <cstdarg>
#include <cstdlib>


Config::Config(const char* file) : file_(file), line_(0)
{
  if ((fp_ = fopen(file, "r")) == NULL) {
    perror(file);
    exit(1);
  }
}


void Config::readError()
{
  fprintf(stderr, "*** read configfile error.");
  fprintf(stderr, "   file:%s,  line:%d\n", file_, line_);
  exit(1);
}


bool Config::isBlankLine(char* p)
{
  while (*p && *p != '\n') {
    if (*p != ' ' && *p != '\t') return false;
    p++;
  }
  return true;
}


bool Config::isCommentLine(char* p)
{
  while (*p == ' ' || *p == '\t') p++;
  if (*p == '#') return true;
  else return false;
}


char* Config::readLine(char* buf, int size)
{
  while (fgets(buf, size, fp_)) {
    line_++;
    if (isBlankLine(buf)) continue;
    if (isCommentLine(buf)) continue;
    return buf;
  }
  return NULL;
}


int Config::xscanf(char* fmt, ...)
{
  char buf[MAX_LEN];
  va_list ap;
  int n;

  if (readLine(buf, MAX_LEN) == NULL) return 0;

  va_start(ap, fmt);
  n = vsscanf(buf, fmt, ap);
  va_end(ap);

  return n;
}




