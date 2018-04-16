#include "client.h"

using namespace std;
Client::Client(ClientSocket* cs, string uname, string fst_priority, string scnd_priority, string thrd_priority)
{
  FD_ZERO(&master);    // clear the master and temp sets
  FD_ZERO(&read_fds);
  first_priority = fst_priority;
  second_priority = scnd_priority;
  third_priority = thrd_priority;
  username = uname;
  socket = cs;
  FD_SET(cs->sockfd, &master);
	FD_SET(STDIN, &master);
  fdmax = max(STDIN, socket->sockfd);
}


void Client::waitForResponse()
{
    int nbytes;
    std::string buf = std::string("register_user-" ) + username + std::string("-") + first_priority + std::string("-") + second_priority + std::string("-") + third_priority;
  	socket->sendData(socket->sockfd, (char*) buf.c_str());
    while(true)
    {
      char buffer[MAXDATASIZE];
      memset (buffer,'\0',MAXDATASIZE);
      if ((nbytes = recv(socket->sockfd, buffer, sizeof buffer, 0)) <= 0) {
        // got error or connection closed by client
        if (nbytes == 0) {
        // connection closed
					cout << "server hung up";
        }
        else {
          cout << "recv" << endl;
        }
        close(socket->sockfd); // bye!
        FD_CLR(socket->sockfd, &master); // remove from master set
        exit(-1);
      }
      else {
        if(strcmp(buffer, "server_quit") == 0) {
          cout << "server quit" << endl;
          exit(-1);
        }

      }

    }
}

Client::~Client()
{
    Util util;
    util.printl("**Client destructor called.");
    delete socket;
    socket = NULL;
}
