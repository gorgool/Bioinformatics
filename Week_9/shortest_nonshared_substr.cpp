#include <string>
#include "trie.h"

using namespace std;

//static void traverse(const suffix_node* const node, vector<string>& ret)
//{
//  if (node->text.empty())
//    return;
//
//  if (node->text[node->text.length() - 1] == '$')
//    ret.push_back(node->text);
//
//  for (size_t idx = 0; idx < 5; idx++)
//  {
//    if (node->childs[idx] != nullptr)
//      traverse(node->childs[idx], ret);
//  }
//}
//
//static vector<string> get_nonrepeats(const suffix_tree& t)
//{
//  vector<string> ret;
//  for (size_t idx = 0; idx < 5; idx++)
//  {
//    if (t.childs[idx] != nullptr)
//      traverse(t.childs[idx], ret);
//  }
//  return ret;
//}

map<char, char> alphabeth{ { 'A', 'T' }, { 'T', 'C' }, { 'C', 'G' }, { 'G', 'A' } };

static string next_pattern(const string& prev_pattern)
{
  string ret(prev_pattern);
  
  int idx = ret.length() - 1;
  while (true)
  {
    if (ret[idx] != 'G')
    {
      ret[idx] = alphabeth[ret[idx]];
      return ret;
    }
    else
    {
      ret[idx] = 'A';
      idx--;
    }

    if (idx < 0)
      return string(prev_pattern.length() + 1, 'A');
  }
}

string shortest_nonshared_substr(const string& s1, const string& s2)
{
  /*suffix_tree t;
  auto temp1 = s1 + "$";
  auto temp2 = s2 + "$";

  for (size_t i = 0; i < temp1.length(); i++)
    t.process_word(temp1.substr(i));

  for (size_t i = 0; i < temp2.length(); i++)
    t.process_word(temp2.substr(i));*/

  string pattern = "A";
  while (true)
  {
    // If found in s1 and not found in s2
    bool pred1 = s1.find(pattern) != string::npos && s2.find(pattern) == string::npos;
    // If found in s2 and not found in s1
    bool pred2 = s2.find(pattern) != string::npos && s1.find(pattern) == string::npos;
    if (pred1 || pred2)
      return pattern;
    pattern = next_pattern(pattern);
  }

  return "";
}