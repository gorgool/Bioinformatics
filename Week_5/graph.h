#pragma once
#include <string>
#include <vector>
#include <list>
#include <algorithm>

struct node;

struct edge
{
  std::string name;
  std::list<node>::iterator to;
  bool visited;
};

struct node
{
  std::string data;
  std::vector<edge> output_edges;
};

struct graph
{
  std::list<node> nodes;
  std::list<node>::iterator find_node(const std::string name)
  {
    return find_if(nodes.begin(), nodes.end(), [&](const node& item) { return item.data == name; });
  }
};