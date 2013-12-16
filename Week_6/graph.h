#pragma once
#include <string>
#include <vector>
#include <map>

struct node;

enum edge_type { deletion, insertion, matches };

struct edge
{
  int weight = 0;
  std::map<std::string, node>::iterator to;
  char label;
  bool visited = false;
  edge_type type;
  edge(int _weight, std::map<std::string, node>::iterator _to, char _label = 'X') : weight(_weight), to(_to), label(_label) {}
  edge(int _weight, std::map<std::string, node>::iterator _to, edge_type _type, char _label = '-') : weight(_weight), to(_to), type(_type), label(_label) {}
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