#include "server.h"
#include <cstdlib>

using namespace std;


int main(int argc, char* argv[]) {
  Util util;
  if (argc != 5) {
    util.printl("valid operation: ./server [IP address] [port number] [root address] [worker number]");
    exit(-1);
  }
  Server server(argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
  server.run();
  return 0;
}
