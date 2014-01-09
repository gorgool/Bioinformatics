#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>

struct node;

enum edge_type { red, blue };
enum edge_sign { pos, neg };

struct edge
{
  std::map<int, node>::iterator to;
  std::map<int, node>::iterator from;

  edge_type type;

  edge_sign to_sign;
  edge_sign from_sign;

  bool visited = false;
  
  edge(
    std::map<int, node>::iterator _to,
    std::map<int, node>::iterator _from,
    edge_sign _to_sign, edge_sign _from_sign,
    edge_type _type) : 
    to(_to),
    from(_from),
    to_sign(_to_sign),
    from_sign(_from_sign),
    type(_type) {}
};

struct node
{
  int value = 0;
  std::vector<edge> edges;
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