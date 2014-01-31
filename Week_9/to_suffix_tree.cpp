#include <string>
#include <stack>
#include "trie.h"

using namespace std;

suffix_tree to_suffix_tree(const string& text, const vector<size_t>& suffix_array, const vector<size_t>& lcp)
{
  suffix_tree t;
  suffix_node* prev_node = new suffix_node("");

  for (size_t i = 0; i < suffix_array.size(); i++)
  {
    if (lcp[i] == 0)
    {
      auto temp = text.substr(suffix_array[i]);
      t.childs[tbl[temp[0]]] = new suffix_node(temp);
      prev_node = t.childs[tbl[temp[0]]];
    }
    else
    {
      if (lcp[i] > prev_node->text.length())
      {
        auto prev_len = prev_node->text.length();
        auto next_node = prev_node;
        while (true)
        {
          next_node = next_node->childs[tbl[text[suffix_array[i] + prev_len]]];
          if (prev_len + next_node->text.length() > lcp[i])
            break;
          prev_len += next_node->text.length();
          if (prev_len == lcp[i])
            break;
        }

        auto n = text.substr(suffix_array[i] + prev_len)[0];
 
        if (prev_len == lcp[i])
          next_node->childs[tbl[text[suffix_array[i] + lcp[i]]]] = new suffix_node(text.substr(suffix_array[i] + lcp[i]));
        else
        {
          if (t.empty(next_node->childs))
          {
            next_node->childs[tbl[next_node->text[lcp[i] - prev_len]]] = new suffix_node(next_node->text.substr(lcp[i] - prev_len));
            next_node->childs[tbl[text[suffix_array[i] + lcp[i]]]] = new suffix_node(text.substr(suffix_array[i] + lcp[i]));
            next_node->text = next_node->text.substr(0, lcp[i] - prev_len);
          }
          else
          {
            auto temp = new suffix_node(next_node->text.substr(lcp[i] - prev_len), next_node->childs);
            for (size_t i = 0; i < 5; i++) next_node->childs[i] = nullptr;
            next_node->childs[tbl[next_node->text[lcp[i] - prev_len]]] = temp;
            next_node->childs[tbl[text[suffix_array[i] + lcp[i]]]] = new suffix_node(text.substr(suffix_array[i] + lcp[i]));
            next_node->text = next_node->text.substr(0, lcp[i] - prev_len);
          }
        }
      }
      else
      {
        if (lcp[i] == prev_node->text.length())
          prev_node->childs[tbl[text[suffix_array[i] + lcp[i]]]] = new suffix_node(text.substr(suffix_array[i] + lcp[i]));
        else
        {
          prev_node->childs[tbl[prev_node->text[lcp[i]]]] = new suffix_node(prev_node->text.substr(lcp[i]));
          prev_node->childs[tbl[text[suffix_array[i] + lcp[i]]]] = new suffix_node(text.substr(suffix_array[i] + lcp[i]));
          prev_node->text = prev_node->text.substr(0, lcp[i]);
        }
      } 
    }
  }
  return t;
}