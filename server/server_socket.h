#ifndef _SERVER_SOCKET_H
#define _SERVER_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <stdio.h>
#include "util.h"
#include "network_exception.h"
#include <sys/types.h>
#include <netdb.h>

#define MAXDATASIZE 256
#define SERVERCAPACITY 200
#define STDIN 0
#define CONNECTION_BACKLOG 10

class ServerSocket {
public:
	ServerSocket(std::string IP_address, int port);
  ~ServerSocket();
  int sockfd;
};

#endif
