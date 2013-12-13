#include <string>
#include "graph.h"
#include <algorithm>
#include <iostream>

using namespace std;

string to_key(const size_t i, const size_t j);

static void gen_graph(graph& g, const string& s, const string& t)
{
  for (size_t i = 0; i < s.length() + 1; i++)
  {
    for (size_t j = 0; j < t.length() + 1; j++)
    {
      if (i < s.length() && j < t.length())
      {
        g.nodes[to_key(i + 1, j + 1)];
        g.nodes[to_key(i, j + 1)];
        g.nodes[to_key(i + 1, j)];
        g.nodes[to_key(i, j)];

        g.nodes[to_key(i, j)].output_edges.push_back(edge(s[i] == t[j] ? 1 : 0, g.nodes.find(to_key(i + 1, j + 1)), s[i] == t[j] ? s[i] : 'X'));
        g.nodes[to_key(i, j)].output_edges.push_back(edge(0, g.nodes.find(to_key(i + 1, j))));
        g.nodes[to_key(i, j)].output_edges.push_back(edge(0, g.nodes.find(to_key(i, j + 1))));

        g.nodes[to_key(i + 1, j + 1)].input_edges.push_back(edge(s[i] == t[j] ? 1 : 0, g.nodes.find(to_key(i, j)), s[i] == t[j] ? s[i] : 'X'));
        g.nodes[to_key(i, j + 1)].input_edges.push_back(edge(0, g.nodes.find(to_key(i, j))));
        g.nodes[to_key(i + 1, j)].input_edges.push_back(edge(0, g.nodes.find(to_key(i, j))));
      }

      if (i == s.length() && j != t.length())
      {
        g.nodes[to_key(i, j)].output_edges.push_back(edge(0, g.nodes.find(to_key(i, j + 1))));
        g.nodes[to_key(i, j + 1)].input_edges.push_back(edge(0, g.nodes.find(to_key(i, j))));
      }

      if (j == t.length() && i != s.length())
      {
        g.nodes[to_key(i, j)].output_edges.push_back(edge(0, g.nodes.find(to_key(i + 1, j))));
        g.nodes[to_key(i + 1, j)].input_edges.push_back(edge(0, g.nodes.find(to_key(i, j))));
      }
    }
  }
}

static map<string, int> memo;

static int traverse_path(graph& g, map<string, node>::iterator n)
{
  int ret = 0;
  vector<pair<vector<edge>::iterator, int>> weights;
  for (auto input_edge = n->second.input_edges.begin(); input_edge != n->second.input_edges.end(); ++input_edge)
  {
    auto key = input_edge->to->first;
    if (memo.find(key) == memo.end())
      memo[key] = traverse_path(g, input_edge->to);

    weights.push_back({ input_edge, input_edge->weight + memo[key] });
  }

  if (!weights.empty())
  {
    auto it = max_element(weights.begin(), weights.end(), 
      [](const pair<vector<edge>::iterator, int>& lhs, const pair<vector<edge>::iterator, int>& rhs)
      {
        return lhs.second < rhs.second;
      });

    ret = it->second;
    it->first->visited = true;
  }

  n->second.value = ret;
  return ret;
}

static string backtrace(const graph& g, map<string, node>::iterator n)
{
  auto it = n;
  string ret;
  while (true)
  {
    for (const auto& e : it->second.input_edges)
    {
      if (e.visited == true)
      {
        if (e.label != 'X')
          ret += e.label;
        it = e.to;
        break;
      }
    }

    if (it->second.input_edges.empty())
      break;
  }

  return string(ret.rbegin(), ret.rend());
}

string LCSubsequence(const string& s, const string& t)
{
  graph g;
  gen_graph(g, s, t);
  auto score = traverse_path(g, g.nodes.find(to_key(s.length(), t.length())));
  return backtrace(g, g.nodes.find(to_key(s.length(), t.length())));
}