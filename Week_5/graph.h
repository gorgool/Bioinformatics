#pragma once
#include <string>
#include <vector>
#include <map>

struct node_m;

struct edge_m
{
  std::string name;
  std::map<std::string, node_m>::iterator to;
  bool visited;
};

struct node_m
{
  std::vector<edge_m> output_edges;
};

struct graph_m
{
  std::map<std::string, node_m> nodes;
};