#include "client_socket.h"
#include "util.h"
using namespace std;


void* ClientSocket::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

ClientSocket::ClientSocket(string server_address, int server_port)
{
    Util util;
    sentbytes = 0;
    receivedbytes = 0;

    int numbytes;
    char buf[MAX_BUFFER_SIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo((const char*)server_address.c_str(), util.itos(server_port).c_str(), &hints, &servinfo)) != 0) {
        util.printl("getaddrinfo: ");
        util.printl(gai_strerror(rv));
        return;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            util.printl("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            util.printl("client: connect");
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL) {
        util.printl("selectserver: failed to bind");
        return;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    util.prints("client: connecting to ");
    util.printl(s);

    freeaddrinfo(servinfo);
}



void ClientSocket::sendData(int sockfd, char* message)
{
  Util util;
  if( send(sockfd , message , strlen(message) , 0) < 0) {
  		util.printl("Send failed");
      return;
  }

}


ClientSocket::~ClientSocket()
{
  close(sockfd);
}
