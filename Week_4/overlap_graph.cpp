#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <cassert>
#include "graph.h"

using namespace std;

vector<string> find_input_nodes(const vector<string>& text, const string& find_str)
{
  vector<string> ret;
  for (auto it = text.begin(); it != text.end(); it++)
  {
    string substr = it->substr(1, it->length() - 1);
    if (substr == find_str)
      ret.push_back(*it);
  }
  return ret;
}

vector<string> find_output_nodes(const vector<string>& text, const string& find_str)
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
  assert(text.size() != 0);
  // Add all nodes in graph

  for (const auto& str : text)
    ret.nodes.push_back(node{ str });

  vector<string> text_copy(text);

  while (!text_copy.empty())
  {
    string curr_node_name = *text_copy.begin();
    text_copy.erase(text_copy.begin());

    auto curr_node = find_if(ret.nodes.begin(), ret.nodes.end(), [&](const node& item) { return item.data == curr_node_name; });

    // Find input nodes
    string substr = curr_node_name.substr(0, curr_node_name.length() - 1);
    auto i_nodes = find_input_nodes(text, substr);
    for (const auto& node_name : i_nodes)
    {
      auto it = find_if(ret.nodes.begin(), ret.nodes.end(), [&](const node& item) { return item.data == node_name; });
      if (it != curr_node)
        curr_node->input_edges.push_back(edge{ "", curr_node, it });
    }

    // Find output nodes
    substr = curr_node_name.substr(1, curr_node_name.length() - 1);
    auto o_nodes = find_output_nodes(text, substr);
    for (const auto& node_name : o_nodes)
    {
      auto it = find_if(ret.nodes.begin(), ret.nodes.end(), [&](const node& item) { return item.data == node_name; });
      if (it != curr_node)
        curr_node->output_edges.push_back(edge{ "", it, curr_node });
    }
  }
}