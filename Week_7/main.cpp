#include <iostream>
#include <string>

using namespace std;

pair<pair<size_t, size_t>, pair<size_t, size_t>> find_middle_edge(const string& s1, const string& s2);

int main()
{
  string s1, s2;
  cin >> s1 >> s2;

  if (s1.length() > s2.length())
    swap(s1, s2);

  auto ret = find_middle_edge(s1, s2);

  cout << "(" <<ret.first.first << "," << ret.first.second << ")" << " ";
  cout << "(" << ret.second.first << "," << ret.second.second << ")" << endl;
  
  return 0;
}
