#include <string>
#include "graph.h"
#include <algorithm>

using namespace std;

string to_key(const size_t i, const size_t j)
{
  return to_string(i) + "," + to_string(j);
};

static void gen_graph(graph& g, const string& s1, const string& s2)
{
  int sigma = 1;
  for (size_t i = 0; i < s1.length() + 1; i++)
  {
    for (size_t j = 0; j < s2.length() + 1; j++)
    {
      if (i < s1.length() && j < s2.length())
      {
        g.nodes[to_key(i + 1, j + 1)];
        g.nodes[to_key(i, j + 1)];
        g.nodes[to_key(i + 1, j)];
        g.nodes[to_key(i, j)];

        g.nodes[to_key(i, j)].output_edges.push_back( edge(s1[i] == s2[j] ? 0 : 1, g.nodes.find(to_key(i + 1, j + 1)), matches));
        g.nodes[to_key(i, j)].output_edges.push_back( edge(sigma, g.nodes.find(to_key(i + 1, j)), insertion));
        g.nodes[to_key(i, j)].output_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j + 1)), deletion));

        g.nodes[to_key(i + 1, j + 1)].input_edges.push_back(edge(s1[i] == s2[j] ? 0 : 1, g.nodes.find(to_key(i, j)), matches));
        g.nodes[to_key(i, j + 1)].input_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j)), insertion));
        g.nodes[to_key(i + 1, j)].input_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j)), deletion));
      }

      if (i == s1.length() && j != s2.length())
      {
        g.nodes[to_key(i, j)].output_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j + 1)), deletion));
        g.nodes[to_key(i, j + 1)].input_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j)), deletion));
      }

      if (j == s2.length() && i != s1.length())
      {
        g.nodes[to_key(i, j)].output_edges.push_back(edge(sigma, g.nodes.find(to_key(i + 1, j)), insertion));
        g.nodes[to_key(i + 1, j)].input_edges.push_back(edge(sigma, g.nodes.find(to_key(i, j)), insertion));
      }
    }
  }
}

static map<string, int> memo;

static int traverse_path(graph& g, map<string, node>::iterator n)
{
  int ret = 0;
  vector<int> weights;
  for (const auto& input_edge : n->second.input_edges)
  {
    auto key = input_edge.to->first;
    if (memo.find(key) == memo.end())
      memo[key] = traverse_path(g, input_edge.to);

    weights.push_back(input_edge.weight + memo[key]);
  }

  if (!weights.empty())
  {
    ret = *min_element(weights.begin(), weights.end());
  }

  n->second.value = ret;
  return ret;
}

int edit_distance(const string& s1, const string& s2)
{
  graph g;
  gen_graph(g, s1, s2);
  auto ret = traverse_path(g, g.nodes.find(to_key(s1.length(), s2.length())));
  return ret;
}