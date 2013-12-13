#pragma once
#include <string>
#include <vector>
#include <map>

struct node;

struct edge
{
  int weight = 0;
  std::map<std::string, node>::iterator to;
  char label;
  bool visited = false;
  edge(int _weight, std::map<std::string, node>::iterator _to, char _label = 'X') : weight(_weight), to(_to), label(_label) {}
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