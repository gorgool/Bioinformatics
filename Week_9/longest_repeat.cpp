#include <string>
#include "trie.h"

using namespace std;

static bool check_leaf(leaf_light* l)
{
  size_t counter = 0;
  for (size_t i = 0; i < 5; i++)
  {
    if (l->childs[i] != nullptr)
      counter++;
    if (counter > 1)
      return true;
  }
  return false;
}

static void traverse(leaf_light* next, const string& s, string& seq)
{
  string repeat_str = s + next->symbol;

  if (check_leaf(next))
  {
    if (seq.length() < repeat_str.length())
      seq = repeat_str;
  }

  for (size_t i = 0; i < 5; i++)
  {
    if (next->childs[i] != nullptr)
      traverse(next->childs[i], repeat_str, seq);
  }
}

string longest_repeat(const string& text)
{
  trie_light t;

  for (size_t i = 0; i < text.size(); i++)
    t.process_word(text.substr(i));

  string ret;
  for (size_t i = 0; i < 5; i++)
  {
    if (t.root.childs[i] != nullptr)
      traverse(t.root.childs[i], "", ret);
  }

  return ret;
}