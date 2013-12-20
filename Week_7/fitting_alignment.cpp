#include <string>
#include <tuple>
#include "graph.h"
#include <algorithm>
#include <thread>
#include <iostream>

using namespace std;

static map<string, int> upper_memo, bottom_memo, middle_memo;

static int traverse_path(map<string, node>::iterator n)
{
  int ret = 0;
  vector<pair<vector<edge>::reverse_iterator, int>> weights;
  for (auto input_edge = n->second.input_edges.rbegin(); input_edge != n->second.input_edges.rend(); ++input_edge)
  {
    auto key = input_edge->to->first;

    switch (input_edge->type)
    {
    case jump_to_upper:
    case deletion:
    {
      if (upper_memo.find(key) == upper_memo.end())
        upper_memo[key] = traverse_path(input_edge->to);
      weights.push_back({ input_edge, input_edge->weight + upper_memo[key] });
      break;            
    }
    case jump_to_bottom:
    case insertion:
    {
      if (bottom_memo.find(key) == bottom_memo.end())
        bottom_memo[key] = traverse_path(input_edge->to);
      weights.push_back({ input_edge, input_edge->weight + bottom_memo[key] });
      break;                            
    }
    case jump_to_middle:
    case matches:
    case none:
    {
      if (middle_memo.find(key) == middle_memo.end())            
        middle_memo[key] = traverse_path(input_edge->to);
      weights.push_back({ input_edge, input_edge->weight + middle_memo[key] });
      break;            
    }
    }  
  }

  if (!weights.empty())
  {
    auto it = max_element(weights.begin(), weights.end(),
      [](const pair<vector<edge>::reverse_iterator, int>& lhs, const pair<vector<edge>::reverse_iterator, int>& rhs)
    {
      return lhs.second < rhs.second;
    });

    ret = it->second;
    it->first->visited = true;
  }

  n->second.value = ret;
  return ret;
}

static pair<size_t, size_t> key_to_idx(const string& key)
{
  size_t pos = key.find(",");
  return make_pair(stoi(key.substr(0, pos)), stoi(key.substr(pos + 1)));
}

static pair<string, string> backtrace(map<string, node>::iterator n, const string& s1, const string& s2)
{
  auto it = n;
  string ret1, ret2;
  edge_type prev_type = none;
  while (true)
  {
    for (const auto& e : it->second.input_edges)
    {
      if (e.visited == true)
      {
        auto idx = key_to_idx(e.to->first);
        switch (e.type)
        {
        //case jump_to_upper:
        case deletion:
        {
          ret1 += s1[idx.first];
          ret2 += '-';
          break;
        }
        case jump_to_middle:
        {
          if (prev_type == deletion || prev_type == jump_to_upper)
          {
            ret1 += s1[idx.first];
            ret2 += '-';
            break;
          }
          
          if (prev_type == insertion || prev_type == jump_to_bottom)
          {
            ret2 += s2[idx.second];
            ret1 += '-';
            break;
          }
        }
        case matches:
        {
          ret1 += s1[idx.first];
          ret2 += s2[idx.second];
          break;
        }
        //case jump_to_bottom:
        case insertion:
        {
          ret2 += s2[idx.second];
          ret1 += '-';
          break;
        }
        }
        prev_type = e.type;
        it = e.to;
        break;
      }
    }

    if (it->second.input_edges.empty())
      break;
  }

  return make_pair(string(ret1.rbegin(), ret1.rend()), string(ret2.rbegin(), ret2.rend()));
}

tuple<int, string, string> fitting_alignment(const string& s1, const string& s2)
{
  L3_graph g;
  g.node_initializing(s1.length(), s2.length());
  g.gen_graph(s1, s2);

  auto score = traverse_path(g.g_middle.nodes.find(to_key(9, 6)));
  auto ret = backtrace(g.g_middle.nodes.find(to_key(9, 6)), s1, s2);

  return make_tuple(score, ret.first, ret.second);
}