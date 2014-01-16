#include <iostream>
#include <string>

using namespace std;

string shortest_nonshared_substr(const string& s1, const string& s2);

int main()
{
  string s1, s2;
  cin >> s1 >> s2;

  cout << shortest_nonshared_substr(s1, s2);

  return 0;
}