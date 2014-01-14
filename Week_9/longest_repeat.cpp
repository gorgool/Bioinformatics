#include <string>
#include <algorithm>
#include "trie.h"

using namespace std;

static void traverse(map<char, leaf_light>::iterator next, const string& s, vector<string>& seq)
{
  string repeat_str = s + next->second.symbol;

  if (next->second.childs.size() > 1)
    seq.push_back(repeat_str);

  for (auto it = next->second.childs.begin(); it != next->second.childs.end(); ++it)
  {
    traverse(it, repeat_str, seq);
  }
}

string longest_repeat(const string& text)
{
  trie_light t;

  for (size_t i = 0; i < text.size(); i++)
    t.process_word(text.substr(i));

  vector<string> seq;

  for (auto it = t.root.childs.begin(); it != t.root.childs.end(); ++it)
  {
    traverse(it, "", seq);
  }

  sort(seq.begin(), seq.end(), [](const string& lhs, const string& rhs) { return lhs.size() < rhs.size(); });

  return *seq.rbegin();
}