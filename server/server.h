#ifndef _SERVER_H
#define _SERVER_H
//close pipe yadet bashe

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
      //this->pid = f_pid;
      // if(pid < 0) {
      //   std::cout << "#Error in creating child process\n";
      //   exit(-1);
      // }
      unnamed_pipe = pipe(pipefds);
      if (unnamed_pipe != 0) {
        std::cout << "worker pipe creation error\n";
        exit(-1);
      }
      cur_req = NULL;
      search_req = "\0";
      status = IDLE;
    }
    ~Worker();
    void set_req(User* cur_req, int cur_req_index) {
      this->cur_req = cur_req;
      this->cur_req_index = cur_req_index;
      status = BUSY;
     }
    void change_status() {status = (status == IDLE) ? BUSY : IDLE; }
    pid_t get_id() { return pid; }
    pid_t get_state() { return status; }
    int* get_pipefds() { return pipefds; }
    void set_pid(pid_t new_pid) { pid = new_pid; }
  private:
    int unnamed_pipe;
    pid_t pid;
    int pipefds[2];
    User* cur_req;
    std::string search_req;
    int cur_req_index;
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
