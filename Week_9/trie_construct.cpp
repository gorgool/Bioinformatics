#include <string>
#include <vector>
#include <iostream>
#include "trie.h"

using namespace std;

trie trie_construct(const vector<string>& vs)
{
  trie ret;

  for (size_t i = 0; i < vs.size(); i++)
  {
    ret.process_word(vs[i], i);
  }

  return ret;
}

static void print_leaf(const char sym, const size_t prev, const size_t curr)
{
  if (sym == '$')
    return;
  cout << prev << " " << curr << " " << sym << endl;
}

static void traverse(const unordered_map<char, leaf>::const_iterator l, const size_t prev)
{
  print_leaf(l->second.symbol, prev, l->second.idx);
  for (auto it = l->second.childs.begin(); it != l->second.childs.end(); ++it)
  {
    traverse(it, l->second.idx);
  }
}

void trie_print(const trie& t)
{
  for (auto it = t.root.childs.begin(); it != t.root.childs.end(); ++it)
    traverse(it, 1);
}