#pragma once
#include <string>
#include <vector>
#include <map>

// Eulerian graph

struct node;

struct edge
{
  std::string name;
  std::map<std::string, node>::iterator to;
  bool visited;
};

struct node
{
  std::vector<edge> output_edges;
  std::vector<edge> input_edges;
};

struct graph
{
  std::map<std::string, node> nodes;
};

// Paired Eulerian graph

struct paired_node;

struct paired_edge
{
  std::string name[2];
  std::map<std::string, paired_node>::iterator to;
  bool visited = false;
};

struct paired_node
{
  std::string label[2];
  std::vector<paired_edge> output_edges;
};

struct paired_graph
{
  std::map<std::string, paired_node> nodes;
};