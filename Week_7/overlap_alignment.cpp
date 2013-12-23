#include <string>
#include <tuple>
#include <algorithm>
#include <thread>
#include <iostream>
#include "graph.h"

using namespace std;

static void gen_graph(graph& g, const string& s1, const string& s2)
{
  const int sigma = -2;

  for (size_t i = 0; i < s1.length() + 1; i++)
  {
    for (size_t j = 0; j < s2.length() + 1; j++)
    {
      if (j == s2.length() && i != s1.length())
      {
        g.nodes[to_key(s1.length(), s2.length())].input_edges.push_back(edge(0, g.nodes.find(to_key(i, j)), none));
        g.nodes[to_key(i, s2.length())].output_edges.push_back(edge(0, g.nodes.find(to_key(s1.length(), s2.length())), none));
      }

      if ( i == 0 && j != 0)
      {
        g.nodes[to_key(0, 0)].output_edges.push_back(edge(0, g.nodes.find(to_key(i, j)), none));
        g.nodes[to_key(i, j)].input_edges.push_back(edge(0, g.nodes.find(to_key(0, 0)), none));
      }

      if (i == s1.length() && j != s2.length())
      {
        g.nodes[to_key(i, j)].output_edges.push_back(edge(0, g.nodes.find(to_key(s1.length(), s2.length())), none));
        g.nodes[to_key(s1.length(), s2.length())].input_edges.push_back(edge(0, g.nodes.find(to_key(i, j)), none));
      }

      if (i != 0 && j == 0)
      {
        g.nodes[to_key(0, 0)].output_edges.push_back(edge(0, g.nodes.find(to_key(i, j)), none));
        g.nodes[to_key(i, j)].input_edges.push_back(edge(0, g.nodes.find(to_key(0, 0)), none));
      }

      if (i < s1.length() && j < s2.length())
      {
        g.nodes[to_key(i + 1, j + 1)];
        g.nodes[to_key(i, j + 1)];
        g.nodes[to_key(i + 1, j)];
        g.nodes[to_key(i, j)];

        string key{ s1[i], s2[j] };

        // Deletion
        g.nodes[to_key(i, j)].output_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j + 1)), deletion));
        // Insertion
        g.nodes[to_key(i, j)].output_edges.push_back(edge(sigma, g.nodes.find(to_key(i + 1, j)), insertion));
        // Matches
        g.nodes[to_key(i, j)].output_edges.push_back(edge(s1[i] == s2[j] ? 1 : sigma, g.nodes.find(to_key(i + 1, j + 1)), matches));

        // Deletion
        g.nodes[to_key(i + 1, j)].input_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j)), deletion));
        // Insertion
        g.nodes[to_key(i, j + 1)].input_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j)), insertion));
        // Matches
        g.nodes[to_key(i + 1, j + 1)].input_edges.push_back(edge(s1[i] == s2[j] ? 1 : sigma, g.nodes.find(to_key(i, j)), matches));
      }

      if (i == s1.length() && j != s2.length())
      {
        g.nodes[to_key(i, j)].output_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j + 1)), deletion));
        g.nodes[to_key(i, j + 1)].input_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j)), insertion));
      }

      if (j == s2.length() && i != s1.length())
      {
        g.nodes[to_key(i, j)].output_edges.push_back(edge(sigma, g.nodes.find(to_key(i + 1, j)), insertion));
        g.nodes[to_key(i + 1, j)].input_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j)), deletion));
      }
    }
  }
}

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
        case deletion:
        {
                       ret1 += s1[idx.first];
                       ret2 += '-';
                       break;
        }
        case matches:
        {
                      ret1 += s1[idx.first];
                      ret2 += s2[idx.second];
                      break;
        }
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

tuple<int, string, string> overlap_alignment(const string& s1, const string& s2)
{
  graph g;
  gen_graph(g, s1, s2);

  auto score = traverse_path(g.nodes.find(to_key(s1.length(), s2.length())));
  auto ret = backtrace(g.nodes.find(to_key(s1.length(), s2.length())), s1, s2);

  return make_tuple(score, ret.first, ret.second);
}