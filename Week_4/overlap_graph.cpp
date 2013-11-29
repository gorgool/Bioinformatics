#include <vector>
#include <string>
#include "graph.h"

using namespace std;

static vector<string> find_output_nodes(const vector<string>& text, const string& find_str)
{
  vector<string> ret;
  for (auto it = text.begin(); it != text.end(); it++)
  {
    string substr = it->substr(0, it->length() - 1);
    if (substr == find_str)
      ret.push_back(*it);
  }
  return ret;
}

void overlap_graph(const vector<string>& text, graph& ret)
{
  // Add all nodes in graph

  for (const auto& str : text)
    ret.nodes.emplace_front(node{ str });

  vector<string> text_copy(text);

  while (!text_copy.empty())
  {
    string curr_node_name = *text_copy.begin();
    text_copy.erase(text_copy.begin());

    auto curr_node = ret.find_node(curr_node_name);

    // Find output nodes
    auto substr = curr_node_name.substr(1, curr_node_name.length() - 1);
    auto o_nodes = find_output_nodes(text, substr);
    for (const auto& node_name : o_nodes)
    {
      auto to = ret.find_node(node_name);
      if (to != curr_node)
        curr_node->output_edges.emplace_back(edge{ "", to });
    }
  }
}