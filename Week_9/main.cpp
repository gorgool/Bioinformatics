#include <iostream>
#include <string>
#include <vector>
#include "trie.h"

using namespace std;

suffix_tree to_suffix_tree(const string& text, const string& suffix_array, const string& lcp);
void print_tree(const suffix_tree& t);

int main()
{
  string text, suffix_array, lcp;
  cin >> text >> suffix_array >> lcp;
  
  auto ret = to_suffix_tree(text, suffix_array, lcp);
  print_tree(ret);

  return 0;
}