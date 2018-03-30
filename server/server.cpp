#include "server.h"

using namespace std;

void* Server::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Server::Server(string address, int port, string addr, int num)
{

  Util util;
  FD_ZERO(&master);    // clear the master and temp sets
  FD_ZERO(&read_fds);
  socket = new ServerSocket(address, port);
  root_folder = addr;
  worker_num = num;
  active = true;
  listener = socket->sockfd;
  FD_SET(listener, &master);
	FD_SET(STDIN, &master);
  fdmax = listener;
}

void Server::run()
{
  bool is_written = false;
  char* myfifo = "./myfifo";
  mkfifo(myfifo, 0666);


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
  pid_t log_pid = fork();

  if (log_pid == 0) {
    log_process(myfifo);
  }

  else if (log_pid > 0) {
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
                          if(strcmp(tokens[0].c_str(), "register_user") == 0) {
                            register_user(tokens, i);
                            write_in_pipe(myfifo, user_list);
                          }
                      }
                   } // END handle data from client
                  } // END got new incoming connection
              } // END looping through file descriptors
          } // END for(;;)--and you thought it would never end!
    }

    else
    {
        cout << "fork() failed!" << endl;
        return;
    }



}

void Server::write_in_pipe(char* myfifo, vector<User*> user_list)
{
  Util util;
  int fd = open(myfifo, O_WRONLY);
  if(fd < 0)
  {
    util.printl("#Error in opening fifo file\n");
    return;
  }
  int req_num = user_list[user_list.size()-1]->get_first().size() +
                user_list[user_list.size()-1]->get_second().size() +
                user_list[user_list.size()-1]->get_third().size();
  string new_request = user_list[user_list.size()-1]->get_username() + string("-") + util.itos(req_num) + string("\n");
  write(fd, new_request.c_str(), strlen(new_request.c_str())+1);
  close(fd);

}

string Server::read_from_pipe(char * myfifo){
  Util util;
  int fd = open(myfifo, O_RDONLY);
  if(fd < 0)   {
    util.printl("#Error in reading the fifo file\n");
    return NULL;
  }
  char arr1[2048];
  read(fd, arr1, 2048);
  close(fd);
  string res = arr1;
  return res;
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
}

void Server::log_process(char* myfifo)
{
  ofstream log_ofs;
  while(true) {
    string new_req = read_from_pipe(myfifo);
    if(strcmp(new_req.c_str(), "") != 0) {
        batched_req.push_back(new_req);
    }
    string log_req;
    if(batched_req.size() == 5) {//change
      for(int i = 0 ; i < 5; i++) {
        log_req += batched_req[i];
      }
      log_ofs.open ( "logs.txt", fstream::out | fstream::app);
      if(!log_ofs.is_open()) {
        cout << "Unable to open file" << endl;
        return;
      }
      log_ofs << log_req;
      log_req = "";
      batched_req.clear();
      log_ofs.close();
    }

  }
}

Server::~Server()
{
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  delete(socket);
  active = false;
}
