#pragma once
#include <string>
#include <vector>

struct node;

struct edge
{
  std::string name;
  std::vector<node>::iterator to;
  std::vector<node>::iterator from;
  bool visited;
};

struct node
{
  std::string data;
  std::vector<edge> input_edges;
  std::vector<edge> output_edges;
};

struct graph
{
  std::vector<node> nodes;
};