#pragma once
#include <string>
#include <vector>
#include <map>

struct node;

enum edge_type { deletion, insertion, matches, none, jump };

struct edge
{
  int weight = 0;
  std::map<std::string, node>::iterator to;
  bool visited = false;
  edge_type type = none;
  edge(int _weight, std::map<std::string, node>::iterator _to) : weight(_weight), to(_to) {}
  edge(int _weight, std::map<std::string, node>::iterator _to, edge_type _type) : weight(_weight), to(_to), type(_type) {}
};

struct node
{
  int value = 0;
  std::vector<edge> input_edges;
  std::vector<edge> output_edges;
};

struct graph
{
  std::map<std::string, node> nodes;
};