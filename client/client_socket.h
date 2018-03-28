#ifndef _CLIENT_SOCKET_H
#define _CLIENT_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include "util.h"
#include "network_exception.h"
#include <netdb.h>

#define MAX_BUFFER_SIZE 2048
#define STDIN 0
class ClientSocket {
public:
	void *get_in_addr(struct sockaddr *sa);
	ClientSocket(std::string server_ip, int server_port);
	void sendData(int sockfd, char* buf);
  ~ClientSocket();
  int sockfd;
private:
  int sentbytes;
  int receivedbytes;
};

#endif
