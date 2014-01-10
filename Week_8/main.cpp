#include <iostream>
#include <string>
#include <vector>
#include "exec_time.hpp"

using namespace std;

vector<pair<size_t, size_t>> find_kmers_hash(const size_t k, const string& s1, const string& s2);

int main()
{
  size_t k;
  cin >> k;
  cin.ignore();

  string s1, s2;
  cin >> s1 >> s2;
  
  vector<pair<size_t, size_t>> ret = find_kmers_hash( k, s1, s2);

  for (const auto& item : ret)
    cout << "(" << item.first << ", " << item.second << ")" << endl;

  return 0;
}