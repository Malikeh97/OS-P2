#include "user.h"

User::User(int file_desc, std::string uname, std::vector<std::string> frst_priority,
std::vector<std::string> sec_priority, std::vector<std::string> thrd_priority){
  fd = file_desc;
  username = uname;
  first_priority = frst_priority;
  second_priority = sec_priority;
  third_priority = thrd_priority;
}
