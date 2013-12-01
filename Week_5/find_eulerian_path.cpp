#include <vector>
#include <string>
#include <cassert>
#include "../Week_4/graph.h"

using namespace std;

vector<string> find_eulerian_cycle(graph& g);

vector<string> find_eulerian_path(graph& _graph)
{
  string from_name;
  vector<list<node>::iterator> special_nodes;
  string rotation_node_name;
  enum { prev, curr } rotation_type;

  // Find special nodes, such that num of input edges != num of output edges
  for (auto it = _graph.nodes.begin(); it != _graph.nodes.end(); ++it)
  {
    size_t n_edges = it->output_edges.size();
    size_t count = 0;
    for (const auto& n : _graph.nodes)
    {
      for (const auto& oe : n.output_edges)
        if (oe.to->data == it->data)
          count++;
    }
    if (count != n_edges)
    {
      special_nodes.push_back(it);
      // Find rotation node
      if (count + n_edges == 1)
      {
        rotation_node_name = it->data;
        // Type of rotation node
        rotation_type = (count == 1) ? prev : curr;
      }
    }    
  }
  
  if (!special_nodes.empty())
  {
    if (special_nodes.size() != 2)
      return vector<string>();

    // Connect special nodes, such that graph become Eulerian
    if (special_nodes[0]->output_edges.empty())
      special_nodes[0]->output_edges.emplace_back(edge{ "", special_nodes[1], false });
    else
      special_nodes[1]->output_edges.emplace_back(edge{ "", special_nodes[0], false });
  }

  // Find Eulerian cycle
  auto ret = find_eulerian_cycle(_graph);
  
  // Trancate last node (first and last node are the same)
  ret.resize(ret.size() - 1);
  
  if (!special_nodes.empty())
  {
    // Rotate around rotation node
    auto rotate_node = find(ret.begin(), ret.end(), rotation_node_name);
    rotate_node = rotation_type == prev ? ++rotate_node : rotate_node;
    rotate(ret.begin(), rotate_node, ret.end());
  }
  
  return ret;
}
