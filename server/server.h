#ifndef _SERVER_H
#define _SERVER_H
//close pipe yadet bashe

#include <sys/select.h>
#include <iostream>
#include "server_socket.h"
#include <fstream>
#include <dirent.h>
#include <regex>
#include "user.h"
#include "worker.h"

#define IDLE  0
#define BUSY  1

class Server {
public:
  void *get_in_addr(struct sockaddr *sa);
  Server(std::string address, int port, std::string addr, int num);
  void run();
  void register_user(std::vector<std::string> tokens, int fd);
  void write_in_pipe(char* myfifo, std::vector<User*> user_list);
  std::string read_from_pipe(char * myfifo);
  void log_process(char* myfifo);
  //void set_req(User* new_req) { req_queue.push_back(new_req);}
  ~Server();
protected:
  std::vector<std::string> req_queue;
  std::vector<std::string> batched_req;
  std::string root_folder;
  int worker_num;
  std::vector<Worker*> worker_list;
  std::vector<User*> user_list;
  bool active;
  int listener;
  fd_set master;
  fd_set read_fds;
  int fdmax;
  ServerSocket* socket;
};

#endif
