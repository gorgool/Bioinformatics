#include <vector>
#include <list>
#include "graph.h"
#include <unordered_set>
#include <iostream>

using namespace std;

static void connect(const int from, const int to, edge_type t, graph& g)
{
  int from_abs = abs(from), to_abs = abs(to);
  auto e = edge(g.nodes.find(to_abs), g.nodes.find(from_abs), neg, pos, t);
  if (to > 0)
  {
    if (from < 0)
      e.from_sign = neg;
  }
  else
  {
    e.to_sign = pos;
    if (from < 0)
      e.from_sign = neg;
  }
  g.nodes[from_abs].edges.push_back(e);
  swap(e.from, e.to); swap(e.from_sign, e.to_sign);
  g.nodes[to_abs].edges.push_back(e);
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

static void traverse_cycle(edge& from, graph& g)
{
  from.visited = true;
  //for (auto& e : from.to->second.edges) 
  //  if (e.to == from.from && e.type == from.type) 
  //    e.visited = true;

  for (auto& e : from.to->second.edges)
  {
    if (e.type != from.type && e.from_sign == from.to_sign && e.visited == false)
      traverse_cycle(e, g);
  }
}

static int compute_cycles(graph& g)
{
  int ret = 0;
  for (auto& node : g.nodes)
  {
    for (auto& e : node.second.edges)
    {
      if (!e.visited)
      {
        traverse_cycle(e, g);
        ret++;
      }
    }
  }
  return ret;
}

static int compute_blocks(const list<vector<int>>& Q)
{
  unordered_set<int> temp;
  for (const auto& seq : Q)
  {
    for (const auto item : seq)
      temp.insert(abs(item));
  }
  return temp.size();
}

size_t breakpoint_distance(const list<vector<int>>& Q, const list<vector<int>>& P)
{
  graph g;
  gen_graph_Q(g, Q);
  gen_graph_P(g, P);

  // # of cycles 2 times larger due to input and output edges which were computed 2 times 
  return compute_blocks(Q) - compute_cycles(g) / 2;
}