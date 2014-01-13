#include <string>
#include <vector>
#include "trie.h"

using namespace std;

vector<size_t> trie_matching(const string& text, const vector<string> &patterns)
{
  vector<size_t> ret;
  for (size_t i = 0; i < text.length(); i++)
  {
    for (const auto& pattern : patterns)
    {
      auto substr = text.substr(i, pattern.length());
      if (pattern == substr)
        ret.push_back(i);
    }
  }
  return ret;
}