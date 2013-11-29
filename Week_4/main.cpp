#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "graph.h"

using namespace std;

void de_bruijn_graph(const vector<string>& text, graph& ret);

int main()
{
  string input_string;
  vector<string> input_text;
  while (getline(cin, input_string))
  {
    input_text.push_back(input_string);
  }
  
  graph eu_graph;
  de_bruijn_graph(input_text, eu_graph);

  for (const auto& item : eu_graph.nodes)
  {
    if (!item.output_edges.empty())
    {
      for (const auto& o : item.output_edges)
        cout << item.data << " -> " << o.to->data << ",";
      cout << endl;
    }
  }

  return 0;
}