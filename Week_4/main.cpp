#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "graph.h"

using namespace std;

vector<string> find_eulerian_cycle(graph& _graph);

void parse_string(const string& input, graph& ret)
{
  string item,o_name;
  istringstream ss(input);
  bool edges_mode = false;

  while (ss >> item)
  {
    if (item == "->")
    {
      edges_mode = true; 
      continue;
    } 

    if (!edges_mode)
    {
      o_name = item;
      if (ret.find_node(item) == ret.nodes.end())
        ret.nodes.push_back(node{ item });
      continue;
    }

    string p_item;
    if (*item.rbegin() == ',')
      p_item = item.substr(0, item.length() - 1);
    else
      p_item = item;

    if (ret.find_node(p_item) == ret.nodes.end())
      ret.nodes.push_back(node{ p_item });

    auto to = ret.find_node(p_item);
    auto from = ret.find_node(o_name);

    from->output_edges.push_back(edge{ "", to, from, false }); 
    to->input_edges.push_back(edge{ "", to, from, false });
  }
}

int main()
{
  string input_string;
  vector<string> input_text;
  while (getline(cin, input_string))
  {
    input_text.push_back(input_string);
  }
  
  graph eu_graph;
  eu_graph.nodes.reserve(input_text.size());

  for (const auto& str : input_text)
    parse_string(str, eu_graph);

  auto ret = find_eulerian_cycle(eu_graph);
  for (const auto& item : ret)
    cout << item << "->";

  return 0;
}