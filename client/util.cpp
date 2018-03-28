#include "util.h"

using namespace std;

vector<string> Util::split(string statement, char delimeter) {
	vector<string> result;
	string token;
	for(int i=0; i<statement.length(); i++)
  {
    if(statement[i] != delimeter)
      token += statement[i];
    else if(token.length()) {
      result.push_back(token);
      token = "";
    }
  }
	if(token.length())
		result.push_back(token);

	return result;
}


int Util::max(int x, int y)	{
		if(x >= y)
				return x;
		else if( y > 0)
				return y;
		else
				return 0;
}

string Util::itos(int number) {
  ostringstream oss;
  oss << number;
  return oss.str();
}

int Util::mystoi(string number) {
  int n;
  stringstream ss;
  ss << number;
  ss >> n;
  return n;
}
void Util::printl(const char* myStr) {
   write(STDOUT_FILENO, myStr, strlen(myStr));
	 write(STDOUT_FILENO, "\n", strlen("\n"));
}

void Util::prints(const char* myStr) {
   write(STDOUT_FILENO, myStr, strlen(myStr));
}
