#include <iostream>
#include <string>
#include <vector>
#include "exec_time.hpp"

using namespace std;

vector<pair<size_t, size_t>> find_kmers(const size_t k, const string& s1, const string& s2);

int main()
{
  size_t k;
  cin >> k;
  cin.ignore();

  string s1, s2;
  cin >> s1 >> s2;

  
  vector<pair<size_t, size_t>> ret;
  cout << exec_time(ret, find_kmers, k, s1, s2) << endl;


  for (const auto& item : ret)
    cout << "(" << item.first << ", " << item.second << ")" << endl;

  return 0;
}