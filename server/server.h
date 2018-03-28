#ifndef _SERVER_H
#define _SERVER_H

#include <sys/select.h>
#include <iostream>

#include "server_socket.h"

class User {
  public:
    User(int file_desc, std::string uname, std::vector<std::string> frst_priority,
    std::vector<std::string> sec_priority, std::vector<std::string> thrd_priority){
      fd = file_desc;
      username = uname;
      first_priority = frst_priority;
      second_priority = sec_priority;
      thrd_priority = third_priority;
    }
    ~User(){}
    int get_fd(){return fd;}

  private:
    int fd;
    std::string username;
    std::vector<std::string> first_priority;
    std::vector<std::string> second_priority;
    std::vector<std::string> third_priority;
};

class Server {
public:
  void *get_in_addr(struct sockaddr *sa);
  Server(std::string address, int port);
  void run();
  void register_user(std::vector<std::string> tokens, int fd);
  ~Server();
protected:
  std::vector<User*> user_list;
  bool active;
  int listener;
  fd_set master;
  fd_set read_fds;
  int fdmax;
  ServerSocket* socket;
};

#endif
