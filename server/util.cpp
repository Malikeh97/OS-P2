#include "util.h"

using namespace std;

void Util::merge(vector<FileInfo> &arr, int l, int m, int r) {//from https://www.geeksforgeeks.org/merge-sort/
		int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    vector<FileInfo> L(n1), R(n2);

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i].total_point <= R[j].total_point)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void Util::mergeSort(vector<FileInfo> &arr, int l, int r) {
	if (l < r)
	 {
			 // Same as (l+r)/2, but avoids overflow for
			 // large l and h
			 int m = l+(r-l)/2;

			 // Sort first and second halves
			 mergeSort(arr, l, m);
			 mergeSort(arr, m+1, r);

			 merge(arr, l, m, r);
	 }
}
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
