#include "server.h"

using namespace std;

void* Server::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Server::Server(string address, int port)
{
  Util util;
  FD_ZERO(&master);    // clear the master and temp sets
  FD_ZERO(&read_fds);
  socket = new ServerSocket(address, port);
  active = true;
  listener = socket->sockfd;
  FD_SET(listener, &master);
	FD_SET(STDIN, &master);
  fdmax = listener;
}

void Server::run()
{
  Util util;
  int newfd;
  struct sockaddr_storage remoteaddr;
  socklen_t addrlen;

  char buf[MAXDATASIZE];
  int nbytes;

  char remoteIP[INET6_ADDRSTRLEN];

  int yes = 1;
  int i, j, rv;

  struct addrinfo hints, *ai, *p;

  memset(&hints, 0, sizeof hints);
  while(active){
          read_fds = master;
          if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
              util.printl("select");
              exit(4);
          }
          for(i = 0; i <= fdmax; i++) {
              if (FD_ISSET(i, &read_fds)) {
                  if (i == listener) {
                      addrlen = sizeof remoteaddr;
                      newfd = accept(listener,
                          (struct sockaddr *)&remoteaddr,
                          &addrlen);

                      if (newfd == -1) {
                        util.printl("accept");
                      }
                      else {
                          FD_SET(newfd, &master);
                          if (newfd > fdmax) {
                              fdmax = newfd;
                          }
                          util.prints("selectserver: connection request ");
											    util.prints(inet_ntop(remoteaddr.ss_family,
													get_in_addr((struct sockaddr*)&remoteaddr),
													remoteIP, INET6_ADDRSTRLEN));
										      util.prints(" on socket ");
											    util.printl(util.itos(newfd).c_str());
                      }
                  }
                  else	{
                  char buf[MAXDATASIZE];
  								memset (buf,'\0',MAXDATASIZE);
                  if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                  		// got error or connection closed by client
                      if (nbytes == 0) {
                          util.prints("selectserver: socket ");
												  util.prints(util.itos(i).c_str());
												  util.printl(" hung up");
                      }
  										else {
                          util.printl("recv");
                      }
                      close(i); // bye!
                      FD_CLR(i, &master); // remove from master set
                      int index;
	                    for(int z = 0; z <= user_list.size(); z++)	{//function
		                       if(user_list[z]->get_fd() == i)	{
			                          index = z;
		                       }
	                    }
                      user_list.erase(user_list.begin() + index);

                   }
  								 else {
                      const char delimeter= '-';
                      vector<string> tokens = util.split(string(buf), delimeter);
  					          util.printl(buf);
                      if(strcmp(tokens[0].c_str(), "register_user") == 0)
                        register_user(tokens, i);
                  }
               } // END handle data from client
              } // END got new incoming connection
          } // END looping through file descriptors
      } // END for(;;)--and you thought it would never end!
}


void Server::register_user(vector<string> tokens, int fd)
{
  Util util;
  const char delimeter= ',';
  vector<string> frst_priority = util.split(string(tokens[2]), delimeter);
  vector<string> sec_priority = util.split(string(tokens[3]), delimeter);
  vector<string> thrd_priority = util.split(string(tokens[4]), delimeter);

  User* new_user = new User(fd, tokens[1], frst_priority, sec_priority, thrd_priority);
  user_list.push_back(new_user);
  cout<<fd<<" "<<tokens[1]<< frst_priority[0] << sec_priority[0] << thrd_priority[0]<<endl;//test
}

Server::~Server()
{
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  delete(socket);
  active = false;
}
