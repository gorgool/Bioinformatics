#include <string>
#include <vector>
#include "trie.h"

using namespace std;

trie trie_construct(const vector<string>& vs);

list<vector<size_t>> trie_matching(const string& text, const vector<string> &patterns)
{
  trie t = trie_construct(patterns);

  for (size_t i = 0; i < text.length(); i++)
  {
    
  }
}