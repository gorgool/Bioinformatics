#include <vector>
#include <string>
#include <list>
#include <cassert>
#include "graph.h"

using namespace std;

list<map<string, node_m>::iterator> find_eulerian_cycle(graph_m& g);

list<map<string, node_m>::iterator> find_eulerian_path(graph_m& _graph)
{
  string from_name;
  vector<map<string, node_m>::iterator> special_nodes;
  map<string, node_m>::iterator rotation_node;
  enum { prev, curr } rotation_type;

  // Find special nodes, such that num of input edges != num of output edges
  for (auto it = _graph.nodes.begin(); it != _graph.nodes.end(); ++it)
  {
    size_t n_edges = it->second.output_edges.size();
    size_t count = 0;
    for (const auto& n : _graph.nodes)
    {
      for (const auto& oe : n.second.output_edges)
        if (oe.to->first == it->first)
          count++;
    }
    if (count != n_edges)
    {
      special_nodes.push_back(it);
      // Find rotation node
      if (count + n_edges == 1)
      {
        rotation_node = it;
        // Type of rotation node
        rotation_type = (count == 1) ? prev : curr;
      }
    }    
  }
  
  if (!special_nodes.empty())
  {
    // Connect special nodes, such that graph become Eulerian
    if (special_nodes[0]->second.output_edges.empty())
      special_nodes[0]->second.output_edges.emplace_back(edge_m{ "", special_nodes[1], false });
    else
      special_nodes[1]->second.output_edges.emplace_back(edge_m{ "", special_nodes[0], false });
  }
  
  // Find Eulerian cycle
  auto ret = find_eulerian_cycle(_graph);

  if (!special_nodes.empty())
  {
    // Rotate around rotation node
    rotation_node = rotation_type == prev ? ++rotation_node : rotation_node;
    //rotate(ret.begin(), rotation_node, ret.end());
  }
  
  return ret;
}
