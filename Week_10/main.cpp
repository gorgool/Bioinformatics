#include <iostream>
#include <string>
#include "split_string.hpp"

using namespace std;

vector<size_t> BWT_matching(const string& bwt, const vector<string>& patterns);

int main()
{
  string text, buff;
  cin >> text; cin.ignore();
  getline(cin, buff);

  vector<string> patterns = split_string<string, ' '>(buff);

  auto ret = BWT_matching(text, patterns);

  for (const auto idx : ret)
    cout << idx << " ";

  return 0;
}