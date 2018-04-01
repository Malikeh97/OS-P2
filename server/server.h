#ifndef _SERVER_H
#define _SERVER_H

#include <sys/select.h>
#include <iostream>
#include "server_socket.h"
#include <iostream>
#include <fstream>

#define IDLE  0
#define BUSY  1

class User {
  public:
    User(int file_desc, std::string uname, std::vector<std::string> frst_priority,
    std::vector<std::string> sec_priority, std::vector<std::string> thrd_priority){
      fd = file_desc;
      username = uname;
      first_priority = frst_priority;
      second_priority = sec_priority;
      third_priority = thrd_priority;
    }
    ~User(){}
    int get_fd(){return fd;}
    std::string get_username(){return username;}
    std::vector<std::string> get_first(){return first_priority;}
    std::vector<std::string> get_second(){return second_priority;}
    std::vector<std::string> get_third(){return third_priority;}
  private:
    int fd;
    std::string username;
    std::vector<std::string> first_priority;
    std::vector<std::string> second_priority;
    std::vector<std::string> third_priority;
};

class Worker {
  public:
    Worker(int index) {
      Util util;
      this->pid = fork();
      std::string head_str = "./worker_fifo_";
      this->worker_fifo = (char*) (head_str + util.itos(index)).c_str();
      mkfifo(this->worker_fifo, 0666);
      cur_req = NULL;
      status = IDLE;
    }
    ~Worker();
    void set_req(User* cur_req) { this->cur_req = cur_req;}
    void change_status() {status = (status == IDLE) ? BUSY : IDLE;}
  private:
    pid_t pid;
    char* worker_fifo;
    User* cur_req;
    int status;
};

class Server {
public:
  void *get_in_addr(struct sockaddr *sa);
  Server(std::string address, int port, std::string addr, int num);
  void run();
  void register_user(std::vector<std::string> tokens, int fd);
  void write_in_pipe(char* myfifo, std::vector<User*> user_list);
  std::string read_from_pipe(char * myfifo);
  void log_process(char* myfifo);
  ~Server();
protected:
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
