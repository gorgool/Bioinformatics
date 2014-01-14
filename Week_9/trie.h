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

static std::map<char, size_t> tbl =
{
  std::make_pair('A', 0),
  std::make_pair('C', 1),
  std::make_pair('G', 2),
  std::make_pair('T', 3),
  std::make_pair('$', 4)
};

struct leaf_light
{
  char symbol;
  leaf_light* childs[5];

  leaf_light(char c) : symbol(c) { for (size_t i = 0; i < 5; i++) childs[i] = nullptr;}
   
};

struct trie_light
{
  leaf_light root;

  trie_light() : root(leaf_light{ '*' }) {}

  void process_word(const std::string& s)
  {
    size_t idx = 1;
    leaf_light* next_leaf = nullptr;

    // If exist
    if (root.childs[tbl[s[0]]] != nullptr)
      next_leaf = root.childs[tbl[s[0]]];
    // If not
    else
    {
      root.childs[tbl[s[0]]] = new leaf_light(s[0]);
      next_leaf = root.childs[tbl[s[0]]];
    }

    // If pattern lenght 1
    if (s.size() == 1)
    {
      next_leaf->childs[4] = new leaf_light('$');
      return;
    }

    while (true)
    {
      if (next_leaf->childs[tbl[s[idx]]] != nullptr)
      {
        next_leaf = next_leaf->childs[tbl[s[idx]]];
        idx++;
        if (idx == s.length())
        {
          next_leaf->childs[4] = new leaf_light('$');
          break;
        }
        continue;
      }
      else
      {
        next_leaf->childs[tbl[s[idx]]] = new leaf_light(s[idx]);
        next_leaf = next_leaf->childs[tbl[s[idx]]];
        idx++;
        if (idx == s.length())
        {
          next_leaf->childs[4] = new leaf_light('$');
          break;
        }
        continue;
      }
    }
  }
};


struct suffix_node
{
  std::string::iterator from;
  size_t len;

  suffix_node* childs[5];
  
  suffix_node(std::string::iterator _from, size_t _len) : from(_from), len(_len) { for (size_t i = 0; i < 5; i++) childs[i] = nullptr; }
};

struct suffix_tree
{
  suffix_node* childs[5];

  suffix_tree() { for (size_t i = 0; i < 5; i++) childs[i] = nullptr; }

  size_t find_branch(std::string::iterator from, const std::string& pattern)
  {
    size_t idx = 0;
    while (pattern[idx] != *from) { idx++, from++; }
    return idx;
  }


  void process_word(std::string& s)
  {
    // If exist
    if (childs[tbl[s[0]]] != nullptr)
    {
      //finding branch point
      auto branch_point = find_branch(childs[tbl[s[0]]]->from, s);
    }
    // If not
    else
    {
      // Add new leaf
      childs[tbl[s[0]]] = new suffix_node(s.begin(), s.length());
    }
  }
};

