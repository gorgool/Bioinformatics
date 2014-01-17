#include <string>
#include <algorithm>
#include "trie.h"

using namespace std;

static void traverse(const suffix_node* const node, const string& prev_str, vector<string>& ret)
{
  ret.push_back(prev_str);
  for (size_t idx = 0; idx < 5; idx++)
  {
    if (node->childs[idx] != nullptr)
      traverse(node->childs[idx], prev_str + node->text, ret);
  }
}

static vector<string> get_repeats(const suffix_tree& t)
{
  vector<string> ret;
  for (size_t idx = 0; idx < 5; idx++)
  {
    if (t.childs[idx] != nullptr)
      traverse(t.childs[idx], "", ret);
  }
  return ret;
}

string longest_substr(const string& s1, const string& s2)
{
  suffix_tree t;
  auto temp1 = s1 + "$";
  auto temp2 = s2 + "$";

  for (size_t i = 0; i < temp1.length(); i++)
    t.process_word(temp1.substr(i));

  for (size_t i = 0; i < temp2.length(); i++)
    t.process_word(temp2.substr(i));

  // All repeats (including repeats in the same string)
  auto repeats = get_repeats(t);
  sort(repeats.begin(), repeats.end(), [](const string& lhs, const string& rhs){ return lhs.length() < rhs.length(); });

  // Slice longest
  size_t max_len = repeats.rbegin()->length();

  // Check if repetition appeared in both strings
  for (auto it = repeats.rbegin(); it != repeats.rend(); it++)
  {
    if (s1.find(*it) != string::npos && s2.find(*it) != string::npos)
      return *it;
  }

  return "";
}