#include <iostream>
#include <string>

using namespace std;

string LCSubsequence(const string& s, const string& t);

int main()
{
  string s, t;
  cin >> s >> t;

  auto ret = LCSubsequence(s, t);
  cout << ret;

  return 0;
}