#pragma once
#include <unordered_map>
#include <string>

struct leaf
{
  size_t idx;
  char symbol;
  std::unordered_map<char, leaf> childs;
};

struct trie
{
  size_t counter;
  leaf root;

  trie() : counter{ 2 }, root(leaf{ 1, '*' }) {}

  void process_word(const std::string& s, const size_t pattern_idx)
  {
    size_t idx = 1;
    std::unordered_map<char, leaf>::iterator next_leaf;
    auto it = root.childs.find(s[0]);
    // If exist
    if (it != root.childs.end())
      next_leaf = it;
    // If not
    else
    {
      root.childs.insert(std::make_pair(s[0], leaf{ counter++, s[0] }));
      next_leaf = root.childs.find(s[0]);
    }

    while (true)
    {
      auto it = next_leaf->second.childs.find(s[idx]);
      if (it != next_leaf->second.childs.end())
      {
        next_leaf = it;
        idx++;
        continue;
      }
      else
      {
        next_leaf->second.childs.insert(std::make_pair(s[idx], leaf{ counter++, s[idx] }));
        next_leaf = next_leaf->second.childs.find(s[idx]);
        idx++;
        if (idx == s.length())
        {
          next_leaf->second.childs.insert(std::make_pair('$', leaf{ pattern_idx, '$' }));
          break;
        }
        continue;
      }
    }
  }

  bool check_pattern(std::string::iterator from)
  {
    auto next = root.childs.find(*from);

    if (next == root.childs.end()) return false;

    while (true)
    {
      if (next->second.symbol == '$')
        return true;

      from++;
      auto it = next->second.childs.find(*from);
      if (it == next->second.childs.end())
        return false;
      
      next = it;
    }
  }
};