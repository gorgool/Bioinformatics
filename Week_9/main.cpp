#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "trie.h"
#include "split_string.hpp"

using namespace std;

suffix_tree to_suffix_tree(const string& text, const vector<size_t>& suffix_array, const vector<size_t>& lcp);
void print_tree(const suffix_tree& t);
suffix_tree suffix_tree_construct(string& text);

int main()
{
  string text, suffix_array, lcp_text;
  getline(cin, text);
  getline(cin, suffix_array);
  getline(cin, lcp_text);

  vector<size_t> sa = split_string<size_t, ' ', ','>(suffix_array);
  vector<size_t> lcp = split_string<size_t, ' ', ','>(lcp_text);

  
  //auto ret = to_suffix_tree(text, sa, lcp);
  auto ret = suffix_tree_construct(text);
  print_tree(ret);

  return 0;
}