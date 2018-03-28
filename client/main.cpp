#include "client.h"
#include "client_socket.h"
#include "util.h"
#include <string.h>
#include <cstdlib>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
  const char delimeter= '-';
    Util util;
  if (argc != 2) {
    util.printl("valid operation: ./client [server ip]--[server port]--[username]--[first word with priority 1, second word with priority 1,…]--[first word with priority 2, second word with priority 2, …]--[first word with priority 3, second word with priority 3, …]");
    exit(-1);
  }
  vector<string> arguments = util.split(string(argv[1]), delimeter);
  ClientSocket* socket = new ClientSocket(arguments[0], atoi(arguments[1].c_str()));
  Client* newclient=new Client(socket, arguments[2], arguments[3], arguments[4], arguments[5]);
  newclient->waitForResponse();
  return 0;
}
