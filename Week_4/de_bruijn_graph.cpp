#include <vector>
#include <string>
#include "graph.h"

using namespace std;

void static graph_construct(const vector<string>& text, const size_t k, graph& ret)
{
  // For all edges
  for (const auto& curr_edge : text)
  {
    // Left and right node names
    string lhs{ curr_edge.substr(0, k - 1) };
    string rhs{ curr_edge.substr(1, k - 1) };

    auto from = ret.find_node(lhs);
    if (ret.find_node(lhs) == ret.nodes.end())
    {
      ret.nodes.emplace_front(node{ lhs });
      from = ret.find_node(lhs);
    }
      
    auto to = ret.find_node(rhs);
    if (ret.find_node(rhs) == ret.nodes.end())
    {
      ret.nodes.emplace_front(node{ rhs });
      to = ret.find_node(rhs);
    }
      
    from->output_edges.push_back(edge{ curr_edge, to}); 
  }
}

void de_bruijn_graph(const string& text, const size_t k, graph& ret)
{
  // All edges
  vector<string> edges;
  for (size_t pos = 0; pos < text.length() - k + 1; pos++)
  {
    edges.push_back(text.substr(pos, k));
  }
  graph_construct(edges, k, ret);
}

void de_bruijn_graph(const vector<string>& text, graph& ret)
{
  graph_construct(text, text[0].length(), ret);
}