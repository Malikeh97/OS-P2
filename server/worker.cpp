#include "worker.h"
using namespace std;

Worker::Worker(int index) {
  Util util;
  unnamed_pipe = pipe(pipefds);
  if (unnamed_pipe != 0) {
    cout << "worker pipe creation error\n";
    exit(-1);
  }
  cur_req = NULL;
  status = IDLE;
}

void Worker::set_req(User* cur_req, int cur_req_index) {
  this->cur_req = cur_req;
  this->cur_req_index = cur_req_index;
  status = BUSY;
 }

//  void Worker::open_directory(string path, std::vector<std::string> &files) {
//    DIR *pDir = opendir(path.c_str());
//    if(pDir == NULL)
//      return;
//    struct dirent* pDirent = readdir(pDir);
//    while (pDirent != NULL)
//      if(strcmp(pDirent->d_name, ".") != 0 && strcmp(pDirent->d_name, "..") != 0 && strcmp(pDirent->d_name, ".DS_Store") != 0)
//        files.push_back(pDirent->d_name);
//    closedir (pDir);
//   }
//   string Worker::find_min(vector<vector<string> > &str) {
//     double min = 1000;
//     int index = -1;
//     for (int i = 0; i < str.size(); i++) {
//       if (str[i].size() == 0)
//         continue;
//       string s = str[i][0];
//       double score = atof(s.substr(s.find_last_of(" ") + 1).c_str());
//       if (score < min) {
//         min = score;
//         index = i;
//      }
//      }
//   if (index != -1) {
//   string s = str[index][0];
//   str[index].erase(str[index].begin());
//   return s;
//   }
//   return "--- MIN NOT FOUND!!!";
//   }
//
//   std::string Worker::pack_msg(std::vector<string> points) {
//     Util util;
//     std::vector< std::vector<std::string> > tokenized;
//     std::string res = "";
//     int counter = 0;
//     for (int i = 0; i < points.size(); i++) {
//       std::vector<std::string> splitted = util.split(l[i], '\\');
//       tokenized.push_back(splitted);
//       counter += splitted.size();
//     }
//     for (int i = 0; i < counter; i++) {
//       std::string min = find_min(tokenized);
//       if (i == 0)
//         res = min;
//       else
//         res += "\\" + min;
//     }
//     return res;
// }
// double calculate_point(std::string dir) {
//
//    std::regex  const expr2();
//    std::regex  const expr3();
//    std::ifstream my_file(dir);
//    int total_point = 0;
//    if(my_file.is_open()) {
//     std::string str;
//     while(my_file) {
//       getline(my_file, str);  // delim defaults to '\n'
//       if(my_file) {
//         for(int i = 0; i < cur_req->get_first().size(); i++) {
//           std::regex  const expr1(cur_req->get_first()[i]);
//           std::ptrdiff_t const match_count1(std::distance(
//             std::sregex_iterator(str.begin(), str.end(), expr1),
//             std::sregex_iterator()));
//           total_point += match_count1*3;
//         }
//
//         for(int i = 0; i < cur_req->get_second().size(); i++) {
//           std::regex  const expr2(cur_req->get_second()[i]);
//           std::ptrdiff_t const match_count2(std::distance(
//             std::sregex_iterator(str.begin(), str.end(), expr2),
//             std::sregex_iterator()));
//           total_point += match_count2*2;
//         }
//
//         for(int i = 0; i < cur_req->get_third().size(); i++) {
//           std::regex  const expr3(cur_req->get_third()[i]);
//           std::ptrdiff_t const match_count3(std::distance(
//             std::sregex_iterator(str.begin(), str.end(), expr3),
//             std::sregex_iterator()));
//           total_point += match_count3;
//         }
//
//       }
//     }
//       my_file.close();
//     }
//    return  (double) total_point/2;
//  }
//
//  std::string traverse_folders(std::string root_folder) {//traverse throught folders if not file
//     Util util;
//     std::vector<std::string> file_list;
//     open_directory(root_folder + "/", file_list);
//
//     int pipefd_list[file_list.size()][2];
//     double total_point = 0;
//
//     for(int i = 0; i < file_list.size(); i++) {
//       if (pipe(pipefd_list[i]) == -1)
//        std::cout << "Error in creating pipe"<< std::endl;
//       if(fork() == 0) {
//         std::string tmp;
//         std::string path = root_folder + "/" + file_list[i];
//         if(file_list[i].find(".txt") != -1) {
//           total_point = calculate_point(path);
//           tmp = "path: " + path + " point: " + util.itos(total_point);
//         }
//         else {
//           tmp = traverse_folders(path);
//         }
//         write(pipefd_list[i][1], tmp.c_str(), tmp.length());
//         exit(EXIT_SUCCESS);
//       }
//     }
//
//    std::vector<std::string> calculeted_points;
//    for (int i = 0; i < file_list.size(); i++) {
//      char buffer[MAXDATASIZE] = {0};
//      read(pipefd_list[i][0], buffer, MAXDATASIZE);
//      calculeted_points.push_back(std::string(buffer));
//    }
//    return concat_list(points);
//  }

 void Worker::set_user(string search_req) {
   Util util;
   char delimeter = '-';
   vector<string> tokens = util.split(search_req, delimeter);
   delimeter = ',';
   vector<string> frst_priority = util.split(string(tokens[2]), delimeter);
   vector<string> sec_priority = util.split(string(tokens[3]), delimeter);
   vector<string> thrd_priority = util.split(string(tokens[4]), delimeter);
   cur_req = new User(0, tokens[1], frst_priority, sec_priority, thrd_priority);
   cout<<cur_req->get_username()<<":"<<endl;
   for(int i = 0; i < cur_req->get_first().size(); i++){
      cout<<cur_req->get_first()[i]<<",";
   }
   cout<<":";
   for(int i = 0; i < cur_req->get_second().size(); i++){
      cout<<cur_req->get_second()[i]<<",";
   }
   cout<<":";
   for(int i = 0; i < cur_req->get_third().size(); i++){
      cout<<cur_req->get_third()[i]<<",";
   }
   cout<<endl;
 }
