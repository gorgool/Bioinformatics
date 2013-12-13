#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "graph.h"

using namespace std;

string to_key(const size_t i, const size_t j)
{
  return to_string(i) + "," + to_string(j);
}

vector<vector<int>> get_matrix(const size_t num_rows)
{
  vector<vector<int>> ret;
  for (size_t i = 0; i < num_rows; i++)
  {
    string buf;
    getline(cin, buf);
    stringstream ss(buf);
    ret.push_back(vector<int>(istream_iterator<int>{ ss }, istream_iterator<int>{}));
  }
  return ret;
}

void gen_graph(graph& g, const vector<vector<int>>& down_mat, const vector<vector<int>>& right_mat, const size_t n, const size_t m)
{
  for (size_t i = 0; i < n; i++)
  {
    for (size_t j = 0; j < m; j++)
    {
      if (i < n - 1)
      {
        g.nodes[to_key(i + 1, j)];
        g.nodes[to_key(i, j)].output_edges.push_back(edge(down_mat[i][j], g.nodes.find(to_key(i + 1, j))));
        g.nodes[to_key(i + 1, j)].input_edges.push_back(edge(down_mat[i][j], g.nodes.find(to_key(i, j))));
      }

      if (j < m - 1)
      {
        g.nodes[to_key(i, j + 1)];
        g.nodes[to_key(i, j)].output_edges.push_back(edge(right_mat[i][j], g.nodes.find(to_key(i, j + 1))));
        g.nodes[to_key(i, j + 1)].input_edges.push_back(edge(right_mat[i][j], g.nodes.find(to_key(i, j))));
      }
    }
  }
}

static map<string, int> memo;

static int traverse_path(graph& g, map<string, node>::iterator n)
{
  int ret = 0;
  vector<int> weights;
  for (const auto input_edge : n->second.input_edges)
  {
    auto key = input_edge.to->first;
    if (memo.find(key) == memo.end())
      memo[key] = traverse_path(g, input_edge.to);

    weights.push_back(input_edge.weight + memo[key]);
  }

  if (!weights.empty())
  {
    ret = *max_element(weights.begin(), weights.end());
  }

  n->second.value = ret;
  return ret;
}

int find_longest_path(graph& g, const size_t n, const size_t m)
{
  return traverse_path(g, g.nodes.find(to_string(n) + to_string(m)));;
}