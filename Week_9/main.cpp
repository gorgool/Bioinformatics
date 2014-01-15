#include <iostream>
#include <string>
#include <vector>
#include "trie.h"

using namespace std;

suffix_tree suffix_tree_construct(string& text);
void print_tree(const suffix_tree& t);

int main()
{
  string text;
  cin >> text;

  auto ret = suffix_tree_construct(text);
  print_tree(ret);

  return 0;
}