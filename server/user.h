#ifndef _USER_H
#define _USER_H

#include "util.h"
#include <sys/select.h>
#include <iostream>
#include <string>
#include <cstring>

class User {
  public:
    User(int file_desc, std::string uname, std::vector<std::string> frst_priority,
    std::vector<std::string> sec_priority, std::vector<std::string> thrd_priority);
    ~User(){}
    int get_fd(){return fd;}
    std::string get_username(){return username;}
    std::vector<std::string> get_first(){return first_priority;}
    std::vector<std::string> get_second(){return second_priority;}
    std::vector<std::string> get_third(){return third_priority;}
    void set_deleted() { is_deleted = true; }
  private:
    int fd;
    std::string username;
    std::vector<std::string> first_priority;
    std::vector<std::string> second_priority;
    std::vector<std::string> third_priority;
    bool is_deleted;
};

#endif
