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
     }
   }
   closedir (dir);
 }

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
          tmp = path + ":" + util.itos(total_point);
        }
        else {//if folder detected recursively traverse through files
          tmp = traverse_folders(path);
        }
        write(pipefd_list[i][1], tmp.c_str(), tmp.length());//write output for the parent
        exit(EXIT_SUCCESS);
      }
    }

  vector<FileInfo> calculated_points;
  for (int i = 0; i < file_list.size(); i++) {
     char buffer[MAXDATASIZE] = {0};
     read(pipefd_list[i][0], buffer, MAXDATASIZE);
     Util util;
     const char delimeter= '+';
     struct FileInfo file_point;
     vector<string> tempo = util.split(string(buffer), delimeter);
     for(int j = 0; j < tempo.size(); j++) {
       const char delim= ':';
       vector<string> tmp = util.split(string(tempo[j]), delim);
       file_point.path = tmp[0];
       file_point.total_point = util.mystoi(tmp[1]);
       calculated_points.push_back(file_point);
     }
   }
   util.mergeSort(calculated_points, 0, calculated_points.size()-1);
   string final_concat;
   for(int i = 0; i < calculated_points.size(); i++) {
     // if(i == calculated_points.size()-1) {
     //   final_concat = calculated_points[i].path + ":" + util.itos(calculated_points[i].total_point);
     //   break;
     // }
     final_concat +=  calculated_points[i].path + ":" + util.itos(calculated_points[i].total_point) + "+";
   }
   return final_concat;
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
