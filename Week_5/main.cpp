#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "graph.h"
#include <string>

using namespace std;

string assemble_read_pairs(vector<string>& v);
vector<string> split_string(const string& s);

int main()
{
  size_t k;
  cin >> k;
  cin.ignore();

  string input_string;
  while (getline(cin, input_string))
  {
    vector<string> v = split_string(input_string);
  }
 
  cout << assemble_read_pairs(k);

  return 0;
}