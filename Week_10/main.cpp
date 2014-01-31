#include <iostream>
#include <string>
#include <vector>
#include "split_string.hpp"

using namespace std;

vector<size_t> approx_pattern_matching(const string& text, const vector<string>& patterns, const size_t k);

int main()
{
  string text, buff;
  cin >> text; cin.ignore();
  getline(cin, buff);
  
  vector<string> patterns = split_string<string, ' '>(buff);
  
  size_t k;
  cin >> k;

  auto ret = approx_pattern_matching(text, patterns, k);

  for (const auto var : ret)
    cout << var << " ";

  return 0;
}