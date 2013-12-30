#include <vector>
#include <list>
#include "graph.h"

using namespace std;

static void connect(const int from, const int to, edge_type t, graph& g)
{
  int from_abs = abs(from), to_abs = abs(to);
  if (to > 0)
  {
    if (from > 0)
    {
      g.nodes[from_abs].positive_edges.push_back(edge(g.nodes.find(to_abs), t));
      g.nodes[to_abs].negative_edges.push_back(edge(g.nodes.find(from_abs), t));
    }
    else
    {
      g.nodes[from_abs].negative_edges.push_back(edge(g.nodes.find(to_abs), t));
      g.nodes[to_abs].negative_edges.push_back(edge(g.nodes.find(from_abs), t));
    }
  }
  else
  {
    if (from > 0)
    {
      g.nodes[from_abs].positive_edges.push_back(edge(g.nodes.find(to_abs), t));
      g.nodes[to_abs].positive_edges.push_back(edge(g.nodes.find(from_abs), t));
    }
    else
    {
      g.nodes[from_abs].negative_edges.push_back(edge(g.nodes.find(to_abs), t));
      g.nodes[to_abs].positive_edges.push_back(edge(g.nodes.find(from_abs), t));
    }
  }
}

static void gen_graph_Q(graph& g, const list<vector<int>>& seq)
{
  g.init(seq);
  for (const auto& part : seq)
  {
    for (size_t idx = 1; idx < part.size(); ++idx)
    {
      connect(part[idx - 1], part[idx], red, g);
    }
    connect(part[part.size() - 1], part[0], red, g);
  }
}

static void gen_graph_P(graph& g, const list<vector<int>>& seq)
{
  for (const auto& part : seq)
  {
    for (size_t idx = 1; idx < part.size(); ++idx)
    {
      connect(part[idx - 1], part[idx], blue, g);
    }
    connect(part[part.size() - 1], part[0], blue, g);
  }
}

static int traverse_graph(graph& g)
{
  int ret = 0;
  for (auto& node : g.nodes)
  {
    if (!node.second.visited)
    {
      ret++;
      auto it = node.second.positive_edges[0].to;
      while (it->second.visited != true)
      {
        it->second.positive_edges[0]
      }
    }
  }
}


size_t breakpoint_distance(const list<vector<int>>& Q, const list<vector<int>>& P)
{
  graph g;
  gen_graph_Q(g, Q);
  gen_graph_P(g, P);

  int ret = traverse_graph(g);

  return 0;
}