#ifndef _WORKER_H
#define _WORKER_H
//close pipe yadet bashe

#include <sys/select.h>
#include <iostream>
#include "server_socket.h"
#include <fstream>
#include <dirent.h>
#include <regex>
#include "user.h"

#define IDLE  0
#define BUSY  1

class Worker {
  public:
    Worker(int index);
    ~Worker();
    void set_req(User* cur_req, int cur_req_index);
    void open_directory(std::string path, std::vector<std::string> &files);
    std::string find_min(std::vector<std::vector<std::string> > &str);
    std::string pack_msg(std::vector<std::string> points);
    double calculate_point(std::string dir);
    std::string traverse_folders(std::string root_folder);
    void change_status() {status = (status == IDLE) ? BUSY : IDLE; }
    pid_t get_id() { return pid; }
    pid_t get_state() { return status; }
    int* get_pipefds() { return pipefds; }
    void set_pid(pid_t new_pid) { pid = new_pid; }
    void set_user(std::string search_req);
    User* get_user() { return cur_req; };
  private:
    int unnamed_pipe;
    pid_t pid;
    int pipefds[2];
    User* cur_req;
    int cur_req_index;
    int status;
};

#endif
