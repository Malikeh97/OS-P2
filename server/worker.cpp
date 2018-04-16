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

 void Worker::open_directory(string path, vector<string> &files) {//caught from https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
   DIR *dir;
   struct dirent *ent;

   if((dir = opendir (path.c_str())) == NULL) {
     cerr << "Err:: problem in opening the directory(" << path << ")" << endl;
     return;
   }

   while ((ent = readdir (dir)) != NULL){
     if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 && strcmp(ent->d_name, ".DS_Store") != 0) {
       files.push_back(ent->d_name);
       cout << ent->d_name << endl;//test
     }
   }
   closedir (dir);
 }
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
double Worker::calculate_point(string dir) {
   ifstream my_file(dir);
   int total_point = 0;
   if(my_file.is_open()) {
    string str;
    while(my_file) {
      getline(my_file, str);  // delim defaults to '\n'
      if(my_file) {
        for(int i = 0; i < cur_req->get_first().size(); i++) {
          regex  const expr1(cur_req->get_first()[i]);
          ptrdiff_t const match_count1(distance(
            sregex_iterator(str.begin(), str.end(), expr1),
            sregex_iterator()));
          total_point += match_count1*6;
        }

        for(int i = 0; i < cur_req->get_second().size(); i++) {
          regex  const expr2(cur_req->get_second()[i]);
          ptrdiff_t const match_count2(distance(
            sregex_iterator(str.begin(), str.end(), expr2),
            sregex_iterator()));
          total_point += match_count2*3;
        }

        for(int i = 0; i < cur_req->get_third().size(); i++) {
          regex  const expr3(cur_req->get_third()[i]);
          ptrdiff_t const match_count3(distance(
            sregex_iterator(str.begin(), str.end(), expr3),
            sregex_iterator()));
          total_point += match_count3*2;
        }

      }
    }
      my_file.close();
    }
   return  (double) total_point/2;
 }

 string Worker::traverse_folders(string root_folder) {//traverse through folders if no file exists, if so calculate the total point of the file
    Util util;
    vector<string> file_list;
    open_directory(root_folder + "/", file_list);

    int pipefd_list[file_list.size()][2];//unnamed pipes as many as the number of folders
    double total_point = 0;

    for(int i = 0; i < file_list.size(); i++) {
      if (pipe(pipefd_list[i]) == -1)
        cout << "Error in creating pipe"<< endl;
      if(fork() == 0) {
        string tmp;
        string path = root_folder + "/" + file_list[i];
        if(file_list[i].find(".txt") != -1) {//if file detected
          total_point = calculate_point(path);
          tmp = path + "/" + util.itos(total_point) ;
          cout << tmp << endl;//test
        }
        else {//if folder detected recursively traverse through files
          tmp = traverse_folders(path);
        }
        write(pipefd_list[i][1], tmp.c_str(), tmp.length());//write output for the parent
         exit(EXIT_SUCCESS);
      }
    }
   //
   // vector<std::string> calculeted_points;
   // for (int i = 0; i < file_list.size(); i++) {
   //   char buffer[MAXDATASIZE] = {0};
   //   read(pipefd_list[i][0], buffer, MAXDATASIZE);
   //   calculeted_points.push_back(std::string(buffer));
   // }
   // return concat_list(points);
   return "hello";
 }

 void Worker::set_user(string search_req) {//tokenize client info and save them in the assigned worker
   Util util;
   char delimeter = '-';
   vector<string> tokens = util.split(search_req, delimeter);
   delimeter = ',';
   vector<string> frst_priority = util.split(string(tokens[2]), delimeter);
   vector<string> sec_priority = util.split(string(tokens[3]), delimeter);
   vector<string> thrd_priority = util.split(string(tokens[4]), delimeter);
   cur_req = new User(0, tokens[1], frst_priority, sec_priority, thrd_priority);
 }
