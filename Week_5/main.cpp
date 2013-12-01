#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "graph.h"

using namespace std;

vector<string> find_eulerian_path(graph& _graph);
vector<string> split_string(const string& s);

void parse_string(const string& s, graph& ret)
{
  vector<string> v = split_string(s);

  if (ret.find_node(v[0]) == ret.nodes.end())
    ret.nodes.emplace_front(node{ v[0] });

  for (size_t i = 2; i < v.size(); i++)
  {
    if (ret.find_node(v[i]) == ret.nodes.end())
      ret.nodes.emplace_front(node{ v[i] });

    auto p = ret.find_node(v[0]);
    p->output_edges.emplace_back(edge{"", ret.find_node(v[i]), false});
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
  for (const auto& item : input_text)
    parse_string(item, eu_graph);

  auto ret = find_eulerian_path(eu_graph);

  for (const auto& item : ret)
  {
    cout << item << "->"; 
  }

  return 0;
}