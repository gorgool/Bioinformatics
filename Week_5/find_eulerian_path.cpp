#include <vector>
#include <string>
#include <list>
#include <cassert>
#include <algorithm>
#include "graph.h"

using namespace std;

list<map<string, node>::iterator> find_eulerian_cycle(graph& g);
list<map<string, paired_node>::iterator> find_eulerian_cycle(paired_graph& g);

list<map<string, node>::iterator> find_eulerian_path(graph& g)
{
  vector<pair<map<string, node>::iterator, size_t>> special_nodes;
  enum { from0to1, from1to0 } direction;
  // Find special nodes, such that num of input edges != num of output edges
  for (auto it = g.nodes.begin(); it != g.nodes.end(); ++it)
  {
    size_t n_edges = it->second.output_edges.size();
    size_t count = 0;
    for (const auto& n : g.nodes)
    {
      for (const auto& oe : n.second.output_edges)
        if (oe.to->first == it->first)
          count++;
    }
    if (count != n_edges)
      special_nodes.push_back({ it, count });
  }
  
  if (!special_nodes.empty())
  {
    // Connect special nodes, such that graph become Eulerian
    if (special_nodes[0].first->second.output_edges.size() < special_nodes[0].second)
      special_nodes[0].first->second.output_edges.emplace_back(edge{ "", special_nodes[1].first, false }), direction = from0to1;
    else
      special_nodes[1].first->second.output_edges.emplace_back(edge{ "", special_nodes[0].first, false }), direction = from1to0;
  }
  
  // Find Eulerian cycle
  auto ret = find_eulerian_cycle(g);

  if (!special_nodes.empty())
  {
    auto from = direction == from0to1 ? special_nodes[0].first->first : special_nodes[1].first->first;
    auto to = direction == from0to1 ? special_nodes[1].first->first : special_nodes[0].first->first;

    list<map<string, node>::iterator>::iterator rotation_point = [&]()
    {
      for (auto it = ret.begin(); it != ret.end(); ++it)
      {
        if ((*it)->first == from)
        {
          auto it_cpy = it; ++it_cpy;
          if ((*it_cpy)->first == to)
            return it_cpy;
        }
      }
      return ret.end();
    }();
  
    rotate(ret.begin(), rotation_point, ret.end());
  }
  
  return ret;
}

list<map<string, paired_node>::iterator> find_eulerian_path(paired_graph& g)
{
  vector<pair<map<string, paired_node>::iterator, size_t>> special_nodes;
  enum { from0to1, from1to0 } direction;
  // Find special nodes, such that num of input edges != num of output edges
  for (auto it = g.nodes.begin(); it != g.nodes.end(); ++it)
  {
    size_t n_edges = it->second.output_edges.size();
    size_t count = 0;
    for (const auto& n : g.nodes)
    {
      for (const auto& oe : n.second.output_edges)
      if (oe.to->first == it->first)
        count++;
    }
    if (count != n_edges)
      special_nodes.push_back({ it, count });
  }

  if (!special_nodes.empty())
  {
    // Connect special nodes, such that graph become Eulerian
    if (special_nodes[0].first->second.output_edges.size() < special_nodes[0].second)
    {
      paired_edge e{};
      e.to = special_nodes[1].first;
      special_nodes[0].first->second.output_edges.emplace_back(e), direction = from0to1;
    } 
    else
    {
      paired_edge e{};
      e.to = special_nodes[0].first;
      special_nodes[1].first->second.output_edges.emplace_back(e), direction = from1to0;
    }
      
  }

  // Find Eulerian cycle
  auto ret = find_eulerian_cycle(g);

  if (!special_nodes.empty())
  {
    auto from = direction == from0to1 ? special_nodes[0].first->first : special_nodes[1].first->first;
    auto to = direction == from0to1 ? special_nodes[1].first->first : special_nodes[0].first->first;

    list<map<string, paired_node>::iterator>::iterator rotation_point = [&]()
    {
      for (auto it = ret.begin(); it != ret.end(); ++it)
      {
        if ((*it)->first == from)
        {
          auto it_cpy = it; ++it_cpy;
          if ((*it_cpy)->first == to)
            return it_cpy;
        }
      }
      return ret.end();
    }();

    rotate(ret.begin(), rotation_point, ret.end());
  }

  return ret;
}
