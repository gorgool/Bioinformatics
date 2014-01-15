#include <string>
#include <iostream>
#include "trie.h"

using namespace std;

static void traverse(const suffix_node* const node)
{
  cout << node->text << endl;
  for (size_t idx = 0; idx < 5; idx++)
  {
    if (node->childs[idx] != nullptr)
      traverse(node->childs[idx]);
  }
}


void print_tree(const suffix_tree& t)
{
  for (size_t idx = 0; idx < 5; idx++)
  {
    if (t.childs[idx] != nullptr)
      traverse(t.childs[idx]);
  }
}

suffix_tree suffix_tree_construct(string& text)
{
  suffix_tree t;

  for (size_t i = 0; i < text.size(); i++)
    t.process_word(text.substr(i));

  return t;
}