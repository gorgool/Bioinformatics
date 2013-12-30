#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>

struct node;

enum edge_type { red, blue };

struct edge
{
  std::map<int, node>::iterator to;
  edge_type type;
  edge(std::map<int, node>::iterator _to, edge_type _type) : to(_to), type(_type) {}
};

struct node
{
  int value = 0;
  bool visited = false;
  std::vector<edge> positive_edges;
  std::vector<edge> negative_edges;
};

struct graph
{
  std::map<int, node> nodes;
  void init(const std::list<std::vector<int>>& seq)
  {
    for (const auto& part : seq)
    {
      for (const auto node : part) nodes[std::abs(node)];
    }
  }
};