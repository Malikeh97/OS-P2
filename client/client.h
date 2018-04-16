#ifndef __CLIENT__
#define __CLIENT__

#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include "client_socket.h"
#include "util.h"

#define MAXDATASIZE 1000000

class BAD_COMMAND_EXCEPTION{};
class Client{
    public:
    	Client(ClientSocket* cs, std::string username, std::string first_priority, std::string second_priority, std::string third_priority);
      void waitForResponse();
      ~Client();
    private:
      fd_set master;
      fd_set read_fds;
      int fdmax;
      std::string first_priority;
      std::string second_priority;
      std::string third_priority;
      std::string username;
    	std::string final_command;
      ClientSocket* socket;
};

#endif
