#include <iostream>
#include <vector>
#include <string>
#include "graph.h"

using namespace std;

void de_bruijn_graph(const vector<string>& text, graph& ret);

int main()
{
  string input_string;
  vector<string> input_text
  {
    "GAGG",
    "GGGG",
    "GGGA",
    "CAGG",
    "AGGG",
    "GGAG"
  };
  
  //while (cin >> input_string)
  //  input_text.push_back(input_string);
  
  graph ret;
  de_bruijn_graph(input_text, ret);
  for (const auto& node_item : ret.nodes)
  {
    if (!node_item.output_edges.empty())
    {
      cout << node_item.data << " -> ";
      for (size_t i = 0; i < node_item.output_edges.size(); ++i)
      {
        cout << node_item.output_edges[i].to->data;
        if (i < node_item.output_edges.size() - 1)
          cout << ",";
      }
      cout << endl;
    }
  }
  return 0;
}