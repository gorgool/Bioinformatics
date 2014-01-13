#include <iostream>
#include <string>
#include <vector>
#include "trie.h"

using namespace std;

vector<size_t> trie_matching(const string& text, const vector<string> &patterns);

int main()
{
  string text;
  cin >> text;
  cin.ignore();
  vector<string> patterns;
  string buff;
  while(getline(cin, buff))
    patterns.push_back(buff);

  auto ret = trie_matching(text, patterns);
  for (const auto& item : ret)
  {
    cout << item << " ";
  }

  // 108 115 312 319 375 382 500 565 1178 1185 1265 1858 2113 3107 3229 3236 3525 3537 3668 3675 3722 3729 4144 4151 4291 4315 4477 5345 5352 5664 6449 6456 6652 6659 6721 7105 7424 7431 7800 7868 7875 8078 8304 8311 8523 8536
  
  return 0;
}