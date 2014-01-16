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
  std::string text;
  suffix_node* childs[5];
  
  suffix_node(std::string _text) : text(_text) { for (size_t i = 0; i < 5; i++) childs[i] = nullptr; }
  suffix_node(std::string _text, suffix_node** _childs) : text(_text) 
  { 
    for (size_t i = 0; i < 5; i++) childs[i] = _childs[i];   
  }
};

struct suffix_tree
{
  suffix_node* childs[5];

  suffix_tree() { for (size_t i = 0; i < 5; i++) childs[i] = nullptr; }
  ~suffix_tree() { /* TODO: MEAMORY LEAK. ADD NODE DELETION */ }

  bool empty(suffix_node** _childs)
  {
    for (size_t i = 0; i < 5; i++) 
      if (_childs[i] != nullptr) return false;
      return true;
  }

  void process_word(std::string& s)
  {
    // If exist
    if (childs[tbl[s[0]]] != nullptr)
    {
      size_t pattern_idx = 0;
      size_t text_idx = 0;
      auto next_node = childs[tbl[s[0]]];
      while (next_node->text[text_idx] == s[pattern_idx])
      {
        pattern_idx++;
        text_idx++;
        if (text_idx == next_node->text.length())
        {
          if (next_node->childs[tbl[s[pattern_idx]]] != nullptr)
          {
            next_node = next_node->childs[tbl[s[pattern_idx]]];
            text_idx = 0;
          }   
          else
          {
            next_node->childs[tbl[s[pattern_idx]]] = new suffix_node(s.substr(pattern_idx));
            return;
          } 
        }
      }

      auto brach_text = next_node->text;
      auto new_text = brach_text.substr(0, text_idx);
      auto new_branch_text = brach_text.substr(text_idx);
      auto new_pattern_text = s.substr(pattern_idx);

      next_node->text = new_text;
      
      // If braching node has no childs
      if (empty(next_node->childs))
      {
        next_node->text = new_text;
        next_node->childs[tbl[new_branch_text[0]]] = new suffix_node(new_branch_text);
        next_node->childs[tbl[new_pattern_text[0]]] = new suffix_node(new_pattern_text);
      }
      else
      {
        // Create new node
        suffix_node* new_node = new suffix_node(new_branch_text, next_node->childs);

        next_node->text = new_text;
        for (size_t i = 0; i < 5; i++) next_node->childs[i] = nullptr;
        next_node->childs[tbl[new_branch_text[0]]] = new_node;
        next_node->childs[tbl[new_pattern_text[0]]] = new suffix_node(new_pattern_text);
      }
    }
    // If not
    else
    {
      // Add new node
      childs[tbl[s[0]]] = new suffix_node(s);
    }
  }
};

