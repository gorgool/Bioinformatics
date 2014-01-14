#include <string>
#include <iostream>
#include "trie.h"

using namespace std;

void print_tree(const suffix_tree& t)
{

}


suffix_tree suffix_tree_construct(const string& text)
{
  suffix_tree t;

  for (size_t i = 0; i < text.size(); i++)
    t.process_word(text.substr(i));

  return t;
}