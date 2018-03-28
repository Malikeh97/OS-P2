#include "server.h"
#include <cstdlib>

using namespace std;


int main(int argc, char* argv[]) {
  Util util;
  if (argc != 3) {
    util.printl("valid operation: ./server [IP address] [port number]");
    exit(-1);
  }
  Server server(argv[1], atoi(argv[2]));
  server.run();
  return 0;
}
