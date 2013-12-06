#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "graph.h"

#include <chrono>

using namespace std;

// Pattern generator
map<char, char> alphabeth{ { '1', '0' }, { '0', '1' } };

static string get_next(const string& prev)
{
  string ret(prev);
  for (size_t i = prev.length() - 1; i >= 0; --i)
  {
    if (ret[i] == '0')
    {
      ret[i] = alphabeth[ret[i]];
      break;
    }
    else
      ret[i] = alphabeth[ret[i]];
  }
  return ret;
}

static void gen_graph(graph& g, const size_t k)
{
  string k_mer(k, '0');
  do
  {
    string from_name(k_mer.begin(), k_mer.end() - 1);
    string to_name(k_mer.begin() + 1, k_mer.end());

    if (g.nodes.find(from_name) == g.nodes.end())
      g.nodes.insert({ from_name, node{} });

    if (g.nodes.find(to_name) == g.nodes.end())
      g.nodes.insert({ to_name, node{} });
    
    g.nodes[from_name].output_edges.push_back(edge{ k_mer, g.nodes.find(to_name), false });

    if (k_mer == string(k, '1'))
      break;

    k_mer = get_next(k_mer);

  } while (true);
}

string string_reconstruct(graph& g, const size_t k);

string get_universal_string(const size_t k)
{
  // Generate graph
  graph g;
  gen_graph(g, k);

  return string_reconstruct(g, k);
}