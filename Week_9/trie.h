#pragma once
#include <unordered_map>
#include <string>
#include <map>

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

    if (s.size() == 1)
    {
      next_leaf->second.childs.insert(std::make_pair('$', leaf{ pattern_idx, '$' }));
      return;
    }

    while (true)
    {
      auto it = next_leaf->second.childs.find(s[idx]);
      if (it != next_leaf->second.childs.end())
      {
        next_leaf = it;
        idx++;
        if (idx == s.length())
        {
          next_leaf->second.childs.insert(std::make_pair('$', leaf{ pattern_idx, '$' }));
          break;
        }
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
};

struct leaf_light
{
  char symbol;
  std::map<char, leaf_light> childs;
};

struct trie_light
{
  leaf_light root;

  trie_light() : root(leaf_light{ '*' }) {}

  void process_word(const std::string& s)
  {
    size_t idx = 1;
    std::map<char, leaf_light>::iterator next_leaf;
    auto it = root.childs.find(s[0]);
    // If exist
    if (it != root.childs.end())
      next_leaf = it;
    // If not
    else
    {
      root.childs.insert(std::make_pair(s[0], leaf_light{ s[0] }));
      next_leaf = root.childs.find(s[0]);
    }

    if (s.size() == 1)
    {
      next_leaf->second.childs.insert(std::make_pair('$', leaf_light{ '$' }));
      return;
    }

    while (true)
    {
      auto it = next_leaf->second.childs.find(s[idx]);
      if (it != next_leaf->second.childs.end())
      {
        next_leaf = it;
        idx++;
        if (idx == s.length())
        {
          next_leaf->second.childs.insert(std::make_pair('$', leaf_light{ '$' }));
          break;
        }
        continue;
      }
      else
      {
        next_leaf->second.childs.insert(std::make_pair(s[idx], leaf_light{ s[idx] }));
        next_leaf = next_leaf->second.childs.find(s[idx]);
        idx++;
        if (idx == s.length())
        {
          next_leaf->second.childs.insert(std::make_pair('$', leaf_light{ '$' }));
          break;
        }
        continue;
      }
    }
  }
};

