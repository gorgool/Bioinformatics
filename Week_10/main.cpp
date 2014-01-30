#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<size_t> pattern_matching(const string& text, const vector<string>& patterns);

int main()
{
  string text, buff;
  cin >> text; cin.ignore();
  
  vector<string> patterns;
  while (getline(cin, buff))
  {
    patterns.push_back(buff);
  }

  auto ret = pattern_matching(text, patterns);

  for (const auto var : ret)
    cout << var << " ";

  return 0;
}