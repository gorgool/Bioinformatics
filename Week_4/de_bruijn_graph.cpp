#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <cassert>
#include "graph.h"

using namespace std;

void static graph_construct(const vector<string>& text, const size_t k, graph& ret)
{
  ret.nodes.reserve(text.size());
  // For all edges
  for (const auto& curr_edge : text)
  {
    // Left and right node names
    string lhs{ curr_edge.substr(0, k - 1) };
    string rhs{ curr_edge.substr(1, k - 1) };

    // Add nodes
    auto from = find_if(ret.nodes.begin(), ret.nodes.end(), [&](const node& item) { return item.data == lhs; });
    bool exist = true;

    if (from == ret.nodes.end())
      ret.nodes.push_back(node{ lhs }), exist = false;

    auto to = find_if(ret.nodes.begin(), ret.nodes.end(), [&](const node& item) { return item.data == rhs; });
    if (to == ret.nodes.end())
      ret.nodes.push_back(node{ rhs }), exist = false;

    if (!exist)
    {
      from = find_if(ret.nodes.begin(), ret.nodes.end(), [&](const node& item) { return item.data == lhs; });
      to = find_if(ret.nodes.begin(), ret.nodes.end(), [&](const node& item) { return item.data == rhs; });
    }
    // Add edges
    from->output_edges.push_back(edge{ curr_edge, to, from });
    to->input_edges.push_back(edge{ curr_edge, from, to });
  }
}

void de_bruijn_graph(const string& text, const size_t k, graph& ret)
{
  assert(text.size() != 0);
  // All edges
  vector<string> edges;
  for (size_t pos = 0; pos < text.length() - k + 1; pos++)
  {
    edges.push_back(text.substr(pos, k));
  }
}

void de_bruijn_graph(const vector<string>& text, graph& ret)
{
  graph_construct(text, text[0].length(), ret);
}