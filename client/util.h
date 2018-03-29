#ifndef _UTIL_H
#define _UTIL_H

#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

class Util {
public:
  std::vector<std::string> split(std::string statement, char delimeter);
  int max(int x, int y);
  std::string itos(int number);
  int mystoi(std::string number);
  void printl(const char* myStr);
  void prints(const char* myStr);
};

#endif
