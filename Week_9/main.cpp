#include <iostream>
#include <string>
#include <vector>
#include "trie.h"

using namespace std;

list<vector<size_t>> trie_matching(const string& text, const vector<string> &patterns);

int main()
{
  string text;
  cin >> text;
  vector<string> patterns;
  string buff;
  while(getline(cin, buff))
    patterns.push_back(buff);

  auto ret = trie_matching(text, patterns);
  for (const auto& item : ret)
  {
    for (const auto idx : item)
      cout << idx << " ";
    cout << endl;
  } 
  
  return 0;
}