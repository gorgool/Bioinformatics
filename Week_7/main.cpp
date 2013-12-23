#include <iostream>
#include <string>
#include <tuple>

using namespace std;

tuple<int, string, string> overlap_alignment(const string& s1, const string& s2);

int main()
{
  string s1, s2;
  cin >> s1 >> s2;

  if (s1.length() < s2.length())
    swap(s1, s2);

  auto ret = overlap_alignment(s1, s2);

  cout << get<0>(ret) << endl;
  cout << get<1>(ret) << endl;
  cout << get<2>(ret) << endl;

  return 0;
}
