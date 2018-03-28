#include "server_socket.h"
using namespace std;

ServerSocket::ServerSocket(string IP_address, int port)
{
    Util util;
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen;

    char buf[MAXDATASIZE];
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];
    int yes = 1;
    int i, j, rv;

    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, util.itos(port).c_str(), &hints, &ai)) != 0) {
        util.prints("selectserver: ");
        util.printl(gai_strerror(rv));
        exit(1);
    }

    for(p = ai; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd < 0) {
            continue;
        }

        // lose the pesky "address already in use" error message
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL) {
        util.printl("selectserver: failed to bind");
        exit(2);
    }

    freeaddrinfo(ai);

    if (listen(sockfd, 10) == -1) {
        util.printl("listen");
        exit(3);
    }

    util.printl("#Server binding completed");

}




ServerSocket::~ServerSocket()
{
  close(sockfd);
}
